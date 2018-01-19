#include <curl/curl.h>
#include <memory.h>
#include <CJson/cJSON.h>
#include "getweather.h"
#include "Location/location.h"

const char *BASE_URL = "https://free-api.heweather.com/s6/weather/";
const char *LOCATION_PRE = "?location=";
const char *DEFAULT_CITY = "CN101190207";
const char *KEY = "&&key=cae08893742340e88cdaeab71d603761";
const char *NOW = "now";


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

void print_time(void) {
    time_t timep;
    struct tm *p;
    time(&timep);
    p = localtime(&timep); //此函数获得的tm结构体的时间，是已经进行过时区转化为本地时间
    printf("当前时间：%d-%02d-%02d %02d:%02d\n",
           1900 + p->tm_year, 1 + p->tm_mon, p->tm_mday, p->tm_hour, p->tm_min);
    free(p);
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
            weather_now(item, 1, 1, 1);
        } else if (strcmp(type, WEATHER_FORECAST) == 0) {
            weather_forecast(item, 1, 1, 1);
        } else if (strcmp(type, "default") == 0) {
            weather_forecast(item, 1, 1, 1);
        }
    }
    if (root) {
        cJSON_Delete(root);
    }
}

char *target_url(const char *style, const char *location) {
    char *str = (char *) malloc(150);
    strcpy(str, BASE_URL);
    strncat (str, style, strlen(style));
    strncat(str, LOCATION_PRE, strlen(LOCATION_PRE));
    if (location != NULL) {
        strncat(str, location, strlen(location));
    } else {
        strncat(str, DEFAULT_CITY, strlen(DEFAULT_CITY));
    }
    strncat(str, KEY, strlen(KEY));
    return str;
}

void get_weather(char *weather_style, char *location) {
    CURL *curl;
    CURLcode res;
    struct WeatherBody chunk;
    chunk.memory = malloc(1);
    chunk.size = 0;
    curl = curl_easy_init();
    char *url = target_url(weather_style, location);
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 5L);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *) &chunk);
        res = curl_easy_perform(curl);
        print_time();
        parseJson(chunk.memory, weather_style);
        if (res != CURLE_OK)
            fprintf(stderr, "curl_easy_perform() failed: %s\n",
                    curl_easy_strerror(res));
        curl_easy_cleanup(curl);
        free(chunk.memory);
        free(url);
        curl_global_cleanup();
    }
}

int get_weather_default(char *style) {
    int result;
    char *location_num = calloc(12, sizeof(char));
    FILE_STATE state = get_default_num(location_num);
    get_weather(style, location_num);
    free(location_num);
    return 0;
}
