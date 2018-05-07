//
// Created by 冉高飞 on 2018/5/6.
//

#ifndef SWEATHER_STRCUT_INFO_H
#define SWEATHER_STRCUT_INFO_H

#include <stdbool.h>

typedef struct weather_args {
    bool full_info;         //显示全部信息
    bool version;           //显示版本
    char *location;         //输入地址
    bool now;               //显示现在天气
    bool forecast;          //显示预报天气
    bool hourly;            //显示逐小时预报
    bool lifestyle;         //显示生活指数
    bool air;               //显示空气质量
    char *set_location;
    char *location_num;     //地区编号
} WeatherArgs;


#endif //SWEATHER_STRCUT_INFO_H
