
#include "getweather.h"
#include "location.h"
#include "log_info.h"
#include <stdlib.h>
#include <memory.h>
#include <ParseWeather/parseweather.h>
#include <curl/curl.h>
#include "CharSytleWeather/weather_condition.h"
#include "CharSytleWeather/basicInfo.h"
#include "CharSytleWeather/updateTime.h"

const char *BASE_URL = "https://free-api.heweather.com/s6/";
const char *LOCATION_PRE = "?location=";
const char *DEFAULT_CITY = "CN101190207";
const char *KEY = "&key=cae08893742340e88cdaeab71d603761";


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

void parseJson(char *chunk, bool isForecast) {
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
            if (cJSON_GetObjectItem(item, "basic") != NULL) {
                cJSON *basic = cJSON_GetObjectItem(item, "basic");
                process_basic_info(basic);
            }
            if (cJSON_GetObjectItem(item, "update") != NULL) {
                cJSON *update = cJSON_GetObjectItem(item, "update");
                processUpdateTime(update);
            }
            if (cJSON_GetObjectItem(item, "now") != NULL) {
                cJSON *weather_now = cJSON_GetObjectItem(item, "now");
                process_now_weather(weather_now);
            }
        } else {
            show_air_by_args(item);
        }
    }
    if (root) {
        cJSON_Delete(root);
    }
}


void processAir() {

}

char *target_url(const char *location, char *type) {
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

void get_weather(WeatherArgs *weatherArgs, bool isForecast) {
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
        char *url;
        if (isForecast) {
            url = target_url(weatherArgs->location_num, WEATHER_DEFAULT);
        } else {
            url = target_url(weatherArgs->location, AIR_DEFAULT);
        }
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 5L);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *) &chunk);
        res = curl_easy_perform(curl);
        if (res == CURLE_OK) {
            parseJson(chunk.memory, isForecast);
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


void get_weather_by_args() {
    get_weather(&context, true);
}
