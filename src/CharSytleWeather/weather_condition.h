//
// Created by 冉高飞 on 2018/7/21.
//

#ifndef SWEATHER_WEATHER_CONDITION_H
#define SWEATHER_WEATHER_CONDITION_H

#include <CJson/cJSON.h>

#define NONE                    "\033[m"
#define RED                     "\033[0;32;31m"
#define LIGHT_RED               "\033[1;31m"
#define SHINING_RED             "\033[5;31m"
#define GREEN                   "\033[0;32;32m"
#define LIGHT_GREEN             "\033[1;32m"
#define BLUE                    "\033[0;32;34m"
#define LIGHT_BLUE              "\033[1;34m"
#define DAR_GRAY                "\033[1;30m"
#define CYAN                    "\033[0;36m"
#define LIGHT_CYAN              "\033[1;36m"
#define PURPLE                  "\033[0;35m"
#define LIGHT_PURPLE            "\033[1;35m"
#define BROWN                   "\033[0;33m"
#define YELLOW                  "\033[0;33m"
#define SHINING_YELLOW          "\033[5;33m"
#define LIGHT_GRAY              "\033[0;37m"
#define WHITE                   "\033[1;37m"
#define TASTE_RED               "\033[0;35m"


typedef struct weather_now {
    char *fl;               //体感温度
    char *tmp;              //温度
    char *cond_code;        //天气状况代码
    char *cond_txt;         //天气
    char *wind_deg;         //风向角
    char *wind_dir;         //风向
    char *wind_sc;          //风力
    char *wind_spd;         //风速
    char *hum;              //相对湿度
    char *pcpn;             //降水量
    char *pres;             //大气压强
    char *vis;              //能见度
    char *cloud;            //云量
} WeatherNow;


void process_now_weather(cJSON *now);

#endif //SWEATHER_WEATHER_CONDITION_H
