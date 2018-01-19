//
// Created by saka on 2018/1/13.
//
#include <stdio.h>
#include <CJson/cJSON.h>
#include "printweather.h"

void print_weather(char *format_string, cJSON* obj, char *key) {
    printf(format_string, cJSON_GetObjectItem(obj, key)->valuestring);
}

void print_weather_two(char *format_string, cJSON* obj, char *key_one, char *key_two) {
    printf(format_string, cJSON_GetObjectItem(obj, key_one)->valuestring,
           cJSON_GetObjectItem(obj, key_two)->valuestring);
}