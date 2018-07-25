//
// Created by 冉高飞 on 2018/7/22.
//

#include "weather_condition.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/**
 * 天气
 * 温度|体感温度
 * 风向|风速
 * 能见度
 * 降雨量|相对湿度
 */

//\  /       Partly cloudy  │    \  /       Partly cloudy  │    \  /       Partly cloudy  │    \  /       Partly cloudy  │
//│  _ /"".-.     20 °C          │  _ /"".-.     23-24 °C       │  _ /"".-.     26-27 °C       │  _ /"".-.     25-26 °C       │
//│    \_(   ).   ↖ 4 km/h       │    \_(   ).   ← 9-11 km/h    │    \_(   ).   ↖ 16-21 km/h   │    \_(   ).   ↑ 18-33 km/h   │
//│    /(___(__)


/**
 * 天气晴
 */
char *sunny = SHINING_RED"      \\   /       "NONE"天气：%s\n"
              LIGHT_RED"       .-.        "NONE"温度：%s °C | %s °C\n"
              SHINING_RED"    -"NONE LIGHT_RED" ( o ) "NONE SHINING_RED"-     "NONE"风速：%s %s km/h\n"
              LIGHT_RED"       `-’        "NONE"见度：%s km\n"
              SHINING_RED"      /   \\       "NONE"降雨：%s mm | %s%%\n";

/**
 * 天气多云
 */
char *cloudy = LIGHT_CYAN"                  "NONE"天气：%s\n"
               LIGHT_CYAN"         .-.      "NONE"温度：%s °C | %s °C\n"
               LIGHT_CYAN"        (_ "LIGHT_GRAY"."LIGHT_CYAN" )    "NONE"风速：%s %s km/h\n"
               LIGHT_CYAN"     .(  "LIGHT_GRAY"(___)    "NONE"见度：%s km\n"
               LIGHT_CYAN"    (___"LIGHT_GRAY"(______)  "NONE"降雨：%s mm | %s%%\n";
/**
 * 天气阴
 */
char *sunny_cloudy = SHINING_RED"      \\   /       "NONE"天气：%s\n"
                     LIGHT_RED"       .-.        "NONE"温度：%s °C | %s °C\n"
                     SHINING_RED"    - "NONE LIGHT_RED"(   "CYAN".-.     "NONE"风速：%s %s km/h\n"
                     LIGHT_RED"       ` "CYAN"( _ ).   "NONE"见度：%s km\n"
                     SHINING_RED"      / "NONE CYAN"(___(__)  "NONE"降雨：%s mm | %s%%\n";
/**
 * 天气雷阵雨
 */
char *thunder_rain = LIGHT_CYAN"      .-.         "NONE"天气：%s\n"
                     LIGHT_CYAN"     (   ).       "NONE"温度：%s °C | %s °C\n"
                     LIGHT_CYAN"    (___(__)      "NONE"风速：%s %s km/h\n"
                     LIGHT_GRAY"    "SHINING_YELLOW"⚡"LIGHT_GRAY"''"SHINING_YELLOW"⚡"LIGHT_GRAY"''       "NONE"见度：%s km\n"
                     LIGHT_GRAY"     ' ' ' '      "NONE"降水：%s mm | %s%%\n";

char *windy = LIGHT_RED"      \\   /      "NONE"%s\n"
              LIGHT_RED"       .-.        "NONE"%s °C | %s °C\n"
              LIGHT_RED"    - (   ) -     "NONE"%s %s km/h\n"
              LIGHT_RED"       `-’        "NONE"%s km\n"
              LIGHT_RED"      /   \\      "NONE"%s mm | %s%%\n";
/**
 * 天气小雨
 */
char *little_rain = LIGHT_CYAN"      .-.         "NONE"天气：%s\n"
                    LIGHT_CYAN"     (   ).       "NONE"温度：%s °C | %s °C\n"
                    LIGHT_CYAN"    (___(__)      "NONE"风速：%s %s km/h\n"
                    LIGHT_GRAY"    ' ' ' '       "NONE"见度：%s km\n"
                    LIGHT_GRAY"     ' ' ' '      "NONE"降雨：%s mm | %s%%\n";
/**
 * 天气大雨
 */
char *big_rain = LIGHT_CYAN"      .-.         "NONE"天气：%s\n"
                 LIGHT_CYAN"     (   ).       "NONE"温度：%s °C | %s °C\n"
                 LIGHT_CYAN"    (___(__)      "NONE"风速：%s %s km/h\n"
                 DAR_GRAY  "    ‚‘‚‘‚‘‚‘      "NONE"见度：%s km\n"
                 DAR_GRAY  "    ‚‘‚‘‚‘‚‘      "NONE"降雨：%s mm | %s%%\n";

char *patch = LIGHT_RED"      \\   /      "NONE"天气：%s\n"
              LIGHT_RED"       .-.        "NONE"温度：%s °C | %s °C\n"
              LIGHT_RED"    - (   ) -     "NONE"风速：%s %s km/h\n"
              LIGHT_RED"       `-’        "NONE"见度：%s km\n"
              LIGHT_RED"      /   \\      "NONE"降雨：%s mm | %s%%\n";
//东风，东南风，南风，西南风，西风，东北风，北风，西北风
char *wind_dir_icon[9] = {"→", "↘", "↓", "↙", "←", "↗", "↑", "↖", "O"};
char *wind_dir_txt[9] = {"东风", "东南风", "南风", "西南风", "西风", "东北风", "北风", "西北风", "无持续风向"};

void print_now_format(const char *cond, const char *dir, WeatherNow *now) {
    fprintf(stdout, "\n");
    fprintf(stdout, cond, now->cond_txt,
            now->tmp, now->fl,
            dir, now->wind_spd,
            now->vis,
            now->pcpn, now->hum);
    fprintf(stdout, "\n");
}

void print_now(WeatherNow *now) {
    char *format = NULL;
    char *dir = NULL;
    switch (strtol(now->cond_code, NULL, 0)) {
        case 100:
            format = sunny;
            break;
        case 101:
        case 102:
            format = cloudy;
            break;
        case 103:
        case 104:
            format = sunny_cloudy;
            break;
        case 302:
            format = thunder_rain;
            break;
        case 305:
            format = little_rain;
            break;
        case 307:
            format = big_rain;
            break;
        default:

            break;
    }
    for (int i = 0; i < 9; i++) {
        if (strcmp(now->wind_dir, wind_dir_txt[i]) == 0) {
            dir = wind_dir_icon[i];
            break;
        }
    }
    print_now_format(format, dir, now);
}

void process_now_weather(cJSON *now) {
    WeatherNow weatherNow;
    weatherNow.fl = cJSON_GetObjectItem(now, "fl")->valuestring;
    weatherNow.tmp = cJSON_GetObjectItem(now, "tmp")->valuestring;
    weatherNow.cond_code = cJSON_GetObjectItem(now, "cond_code")->valuestring;
    weatherNow.cond_txt = cJSON_GetObjectItem(now, "cond_txt")->valuestring;
    weatherNow.wind_deg = cJSON_GetObjectItem(now, "wind_deg")->valuestring;
    weatherNow.wind_dir = cJSON_GetObjectItem(now, "wind_dir")->valuestring;
    weatherNow.wind_sc = cJSON_GetObjectItem(now, "wind_sc")->valuestring;
    weatherNow.wind_spd = cJSON_GetObjectItem(now, "wind_spd")->valuestring;
    weatherNow.hum = cJSON_GetObjectItem(now, "hum")->valuestring;
    weatherNow.pcpn = cJSON_GetObjectItem(now, "pcpn")->valuestring;
    weatherNow.pres = cJSON_GetObjectItem(now, "pres")->valuestring;
    weatherNow.vis = cJSON_GetObjectItem(now, "vis")->valuestring;
    weatherNow.cloud = cJSON_GetObjectItem(now, "cloud")->valuestring;
    print_now(&weatherNow);
}
