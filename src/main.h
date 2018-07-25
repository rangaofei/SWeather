#ifndef _MAIN_H
#define _MAIN_H

#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <SimpleWeatherConfigure.h>

/**
 *********************************
 * version----------查看当前版本号 *
 * *******************************
 * location-----------设置位置信息 *
 * *******************************
 * now------------显示当前天气信息 *
 * *******************************
 * forecast--------------七天天气 *
 * *******************************
 * hourly-----------逐小时天气预报 *
 * *******************************
 * lifestyle--------- 生活方式预报 *
 * *******************************
 * set-location----------设置地点 *
 */
static const char *optString = "vl:nfhLs:";
static const struct option longOpts[] = {
        {"version",      no_argument,       NULL, 'v'},
        {"location",     required_argument, NULL, 0},
        {"now",          no_argument,       NULL, 'n'},
        {"forecast",     no_argument,       NULL, 'f'},
        {"hourly",       no_argument,       NULL, 'h'},
        {"lifestyle",    no_argument,       NULL, 'L'},
        {"set_location", required_argument, NULL, 0},
//        {NULL,        no_argument,       NULL, 0}
};


#define SHOW_VERSION printf("SimpleWeather\t \
Version:%d.%d.%d\n",VERSION_MAJOR, VERSION_MINOR,VERSION_PATCH)


int init_weather_args(WeatherArgs *context);

#endif