#ifndef _MAIN_H
#define _MAIN_H

#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>

/**
 *****************************
 * version*查看当前版本号
 * ****************************
 *
 */
static const char *optString = "vl:nfhLs:";
static const struct option longOpts[] = {
        {"version",      no_argument,       NULL, 'v'},
        {"location",     required_argument, NULL, 0},
        {"now",          no_argument,       NULL, 'n'},
        {"forecast",     no_argument,       NULL, 'f'},
        {"hourly",       no_argument,       NULL, 'h'},
        {"lifestyle",    no_argument,       NULL, 'L'},
        {"set-location", required_argument, NULL, 0},
//        {NULL,        no_argument,       NULL, 0}
};

#define SHOW_VERSION printf("SimpleWeather\n \
Version:%d.%d.%d",VERSION_MAJOR, VERSION_MINOR,VERSION_PATCH)


int init_weather_args(WeatherArgs *weatherArgs);

#endif