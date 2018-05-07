
#include "getweather.h"
#include "location.h"
#include "log_info.h"
#include <stdlib.h>
#include <memory.h>
#include <ParseWeather/parseweather.h>
#include <curl/curl.h>

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
        printf("not enough memory (realloc returned NULL)\n");
        return 0;
    }
    memcpy(&(mem->memory[mem->size]), contents, realsize);
    mem->size += realsize;
    mem->memory[mem->size] = 0;
    return realsize;
}

void parseJson(char *chunk, WeatherArgs *weatherArgs, bool isForecast) {
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
        if (isForecast) {
            show_weather_by_args(item, weatherArgs);
        } else {
            show_air_by_args(item);
        }
    }
    if (root) {
        cJSON_Delete(root);
    }
}

char *target_url(const char *type, const char *location) {
    char *str = (char *) malloc(150);
    strcpy(str, BASE_URL);
    strncat(str, type, strlen(type));
    strcat(str, LOCATION_PRE);
    if (location != NULL) {
        strcat(str, location);
    } else {
        strcat(str, DEFAULT_CITY);
    }
    strcat(str, KEY);
    return str;
}

void get_weather(WeatherArgs *weatherArgs, char *type, bool isForecast) {
    if (weatherArgs == NULL) {
        return;
    }
    CURL *curl;
    CURLcode res;
    struct WeatherBody chunk;
    chunk.memory = malloc(1);
    chunk.size = 0;
    curl = curl_easy_init();
    if (curl) {
        char *url = target_url(type, weatherArgs->location_num);
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 5L);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *) &chunk);
        res = curl_easy_perform(curl);
        if (res == CURLE_OK) {
            parseJson(chunk.memory, weatherArgs, isForecast);
        }
        if (res != CURLE_OK)
            fprintf(stderr, "curl_easy_perform() failed: %s\n",
                    curl_easy_strerror(res));

        curl_easy_cleanup(curl);
        free(chunk.memory);
        free(url);
        curl_global_cleanup();
    }
}

int get_weather_default(WeatherArgs *weatherArgs) {
    char *location_num = calloc(12, sizeof(char));
    FILE_STATE state = get_default_num(location_num);
    switch (state) {
        case SUCCESS:
            get_weather(weatherArgs, WEATHER_DEFAULT, false);
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

void get_weather_by_args(WeatherArgs *weatherArgs) {
    if (weatherArgs->set_location != NULL) {
        set_city_name(weatherArgs->set_location, weatherArgs->location_num);
        weatherArgs->full_info = true;
    }
    LOG_D("the location_num is %s\n", weatherArgs->location_num);
    if (weatherArgs->location == NULL) {
        FILE_STATE state = get_default_num(weatherArgs->location_num);
        switch (state) {
            case SUCCESS:
                break;
            case NOT_FOUND:
            case TYPE_INCORRECT:
                exit_err(ERR_ADDRESS_NOT_SET, "pinyin");
                break;
        }

    } else {
        check_location(weatherArgs->location, weatherArgs->location_num);
    }
    get_weather(weatherArgs, WEATHER_DEFAULT, true);
    if (weatherArgs->air || weatherArgs->full_info) {
        get_weather(weatherArgs, AIR_DEFAULT, false);
    }
}