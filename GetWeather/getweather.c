#include <curl/curl.h>
#include <memory.h>
#include "cJSON.h"
#include "getweather.h"
#include "Location/location.h"

const char *BASE_URL = "https://free-api.heweather.com/s6/";
const char *LOCATION_PRE = "?location=";
const char *DEFAULT_CITY = "CN101190207";
const char *KEY = "&&key=cae08893742340e88cdaeab71d603761";


struct WeatherBody {
    char *memory;
    size_t size;
};

static size_t
WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp) {
    size_t realsize = size * nmemb;
    struct WeatherBody *mem = (struct WeatherBody *) userp;
    mem->memory = realloc(mem->memory, mem->size + realsize + 1);
    if (mem->memory == NULL) {
        /* out of memory! */
        printf("not enough memory (realloc returned NULL)\n");
        return 0;
    }
    memcpy(&(mem->memory[mem->size]), contents, realsize);
    mem->size += realsize;
    mem->memory[mem->size] = 0;
    return realsize;
}


void parseJson(char *chunk, char *type) {
    cJSON *root = cJSON_Parse(chunk);
    cJSON *body_list = cJSON_GetObjectItem(root, "HeWeather6");
    int length = cJSON_GetArraySize(body_list);
    cJSON *item;
    for (int i = 0; i < length; ++i) {
        item = cJSON_GetArrayItem(body_list, i);
        if (strcmp(cJSON_GetObjectItem(item, "status")->valuestring, "ok") != 0) {
            printf("发生错误，请稍后重试");
            continue;
        }
        if (strcmp(type, WEATHER_NOW) == 0) {
            show_weather(item, 1, 0, 1, 0, 0, 1);
        } else if (strcmp(type, WEATHER_FORECAST) == 0) {
            show_weather(item, 1, 1, 0, 0, 0, 1);
        } else if (strcmp(type, WEATHER_DEFAULT) == 0) {
            show_weather(item, 1, 1, 1, 1, 1, 1);
        } else if (strcmp(type, WEATHER_HOURLY) == 0) {
            show_weather(item, 1, 0, 0, 1, 0, 1);
        } else if (strcmp(type, WEATHER_LIFESTYLE) == 0) {
            show_weather(item, 1, 0, 0, 0, 1, 1);
        } else if (strcmp(type, AIR_DEFAULT) == 0) {
            show_air(item, 1, 1, 1);
        }
    }
    if (root) {
        cJSON_Delete(root);
    }
}

char *target_url(const char *style, const char *location) {
    char *str = (char *) malloc(150);
    strcpy(str, BASE_URL);
    strncat(str, style, strlen(style));
    strcat(str, LOCATION_PRE);
    if (location != NULL) {
        strcat(str, location);
    } else {
        strcat(str, DEFAULT_CITY);
    }
    strcat(str, KEY);
    return str;
}

void get_weather(char *weather_style, char *location) {
    char *loc = check_location(location);
    if (loc == NULL) {
        return;
    }
    CURL *curl;
    CURLcode res;
    struct WeatherBody chunk;
    chunk.memory = malloc(1);
    chunk.size = 0;
    curl = curl_easy_init();
    if (curl) {
        char *url = target_url(weather_style, loc);
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 5L);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *) &chunk);
        res = curl_easy_perform(curl);
        if (res == CURLE_OK) {
            parseJson(chunk.memory, weather_style);
        }
        if (res != CURLE_OK)
            fprintf(stderr, "curl_easy_perform() failed: %s\n",
                    curl_easy_strerror(res));
        curl_easy_cleanup(curl);
        free(chunk.memory);
        free(url);
        curl_global_cleanup();
    }
    free(loc);
}

int get_weather_default(char *style) {
    char *location_num = calloc(12, sizeof(char));
    FILE_STATE state = get_default_num(location_num);
    switch (state) {
        case SUCCESS:
            get_weather(style, location_num);
            break;
        case NOT_FOUND:
            printf("请输入\n\tsweather -setloc <address>\n来设置默认城市");
            break;
        case TYPE_INCORRECT:
            printf("获取配置文件错误");
            break;
    }
    free(location_num);
    return 0;
}

int get_air_default(char *style) {
    return 0;
}
