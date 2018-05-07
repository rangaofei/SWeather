#ifndef GET_WEATHER_H
#define GET_WEATHER_H


#include "strcut_info.h"

#define WEATHER_NOW "weather/now"               //现在天气
#define WEATHER_FORECAST "weather/forecast"     //3天天气
#define WEATHER_HOURLY "weather/hourly"         //逐小时预报
#define WEATHER_LIFESTYLE "weather/lifestyle"   //生活方式
#define WEATHER_DEFAULT "weather"               //天气合集
#define AIR_NOW "air/now"                       //空气质量实况
#define AIR_DEFAULT "air"                       //空气质量合集


void get_weather(WeatherArgs *weatherArgs,char *type,bool isForecast);

int get_weather_default(WeatherArgs *weatherArgs);

void get_weather_by_args(WeatherArgs *weatherArgs);

#endif