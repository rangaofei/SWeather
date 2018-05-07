#include <CJson/cJSON.h>
#include <time.h>
#include <strcut_info.h>
#include "parseweather.h"
/**
 * +----------------------------------------------------------------------------------+
 * |                                   江苏-无锡-梁溪                                   |
 * +----------------------------------------------------------------------------------+
 * |    日期   |          天气        |     日    |     月    |      风     |    其他    |
 * +----------------------------------------------------------------------------------+
 * |三天       |白   天|夜   间|气   温|日  出|日 落|月  出|月 落|风 向|风速|风力|紫外线|能见度|
 * +----------------------------------------------------------------------------------+
 * |2018-01-19|晴间多云|晴间多云|-10-10|06:50|18:00|18:00|5:00|西北风|14 |14 |   1  | 20 |
 * +----------------------------------------------------------------------------------+
 * |                                    天气实况                                       |
 * +----------------------------------------------------------------------------------+
 * | 体感温度 | 温度 | 天气 | 风向角 | 风向 | 风力 |  风  速  | 相对湿度 | 降水量 | 大气压强 | 能见度 | 云量 |
 * +----------------------------------------------------------------------------------+
 */

#define UPDATE_TIME "更新时间：%s"
#define CURRENT_TIME "当前时间：%s"
#define LINE_NUM 120

/**
 * 组合地址信息
 * @param basic
 * @return
 */
char *get_basic(cJSON *basic) {
    char *location = calloc(30, sizeof(char));
    strcpy(location, cJSON_GetObjectItem(basic, "admin_area")->valuestring);
    strcat(location, "-");
    strcat(location, cJSON_GetObjectItem(basic, "parent_city")->valuestring);
    strcat(location, "-");
    strcat(location, cJSON_GetObjectItem(basic, "location")->valuestring);
    return location;
}

/**
 * 组合气温
 * @param data
 * @return
 */
char *get_temp_range(cJSON *data) {
    char *temp_range = calloc(8, sizeof(char));
    strcpy(temp_range, cJSON_GetObjectItem(data, "tmp_min")->valuestring);
    strcat(temp_range, "-");
    strcat(temp_range, cJSON_GetObjectItem(data, "tmp_max")->valuestring);
    return temp_range;
}

void print_time(void) {
    time_t timep;
    struct tm *p;
    time(&timep);
    p = localtime(&timep); //此函数获得的tm结构体的时间，是已经进行过时区转化为本地时间
    printf("当前时间：%d-%02d-%02d %02d:%02d",
           1900 + p->tm_year, 1 + p->tm_mon, p->tm_mday, p->tm_hour, p->tm_min);
}

//打印更新时间字段--"update"
void show_update_info(cJSON *update_time, int loc) {
    if (loc) {
        print_line(LINE_NUM);
        printf("|");
        print_space(LINE_NUM / 2 - 10 - 18);
        print_time();
        printf(" | ");
        printf(UPDATE_TIME, cJSON_GetObjectItem(update_time, "loc")->valuestring);
        print_space(LINE_NUM / 2 - 10 - 18);
        printf(" |\n");
    } else {

    }
}

//打印基本信息字段--"basic"
void show_basic_info(cJSON *basic, int full) {
    print_line(LINE_NUM);
    char *location = get_basic(basic);
    if (!full) {
        int length = ((int) strlen(location) - 2) / 3 * 2 + 2;
        int space_length_left = (LINE_NUM - length) / 2;
        int space_length_right = LINE_NUM - length - space_length_left;
        printf("|");
        print_space(space_length_left);
        printf("%s", location);
        print_space(space_length_right);
        printf("|\n");
    } else {

    }
    free(location);
}


void show_forecast_header() {
    print_line(LINE_NUM);
    printf("|");
    print_space(4);
    printf("%s", "日期");
    print_space(4);
    printf("|");

    print_space(12);
    printf("%s", "天   气");
    print_space(13);
    printf("|");

    print_space(6);
    printf("%s", "日");
    print_space(7);
    printf("|");

    print_space(6);
    printf("%s", "月");
    print_space(7);
    printf("|");

    print_space(11);
    printf("%s", "风");
    print_space(11);
    printf("|");

    print_space(7);
    printf("%s", "其他");
    print_space(6);
    printf("|\n");

    print_line(LINE_NUM);
    printf("|");
    print_space(4);
    printf("%s", "七日");
    print_space(4);
    printf("|");

    printf("%s|", "   白天    ");
    printf("%s|", "   夜间   ");
    printf("%s|", "  温 度  ");
    printf("%s|", " 日 出 ");
    printf("%s|", " 日 落 ");
    printf("%s|", " 月 升 ");
    printf("%s|", " 月 落 ");
    printf("%s|", "  风  向  ");
    printf("%s|", " 风力 ");
    printf("%s|", " 风速 ");
    printf("%s|", " 紫外线 ");
    printf("%s|\n", " 能见度 ");
}

//打印三天的天气字段--"forecast"
void show_forecast_info(cJSON *forecast_list) {
    int length = cJSON_GetArraySize(forecast_list);
    cJSON *item;
    print_line(LINE_NUM);
    for (int i = 0; i < length; ++i) {
        item = cJSON_GetArrayItem(forecast_list, i);
        printf("|");
        print_weather(" %s |", item, "date");
        print_center_mix(11, cJSON_GetObjectItem(item, "cond_txt_d")->valuestring);
        print_center_mix(10, cJSON_GetObjectItem(item, "cond_txt_n")->valuestring);
        char *temp_range = get_temp_range(item);
        print_center_mix(9, temp_range);
        free(temp_range);
        print_weather(" %5s |", item, "sr");
        print_weather(" %5s |", item, "ss");
        print_weather(" %5s |", item, "mr");
        print_weather(" %5s |", item, "ms");
        print_center_mix(10, cJSON_GetObjectItem(item, "wind_dir")->valuestring);
        print_weather(" %4s |", item, "wind_sc");
        print_weather(" %4s |", item, "wind_spd");
        print_weather(" %6s |", item, "uv_index");
        print_weather(" %6s |\n", item, "vis");
    }
}

void show_now_header() {
    print_line(LINE_NUM);
    printf("|");
    print_center_mix(LINE_NUM, "天气实况");
    printf("\n");
    print_line(LINE_NUM);
    printf("| %s |", "体感温度");
    printf(" %s |", "温  度");
    printf(" %s |", "天  气");
    printf(" %s |", "风 向 角");
    printf(" %s |", " 风   向 ");
    printf(" %s |", "风  力");
    printf(" %s |", "风  速");
    printf(" %s |", "相对湿度");
    printf(" %s |", "降 水 量");
    printf(" %s |", "大气压强");
    printf(" %s |", "能见度");
    printf(" %s |\n", "云  量");
}

void show_now_info(cJSON *weather_now) {
    print_line(LINE_NUM);
    printf("|");
    print_center_mix(10, cJSON_GetObjectItem(weather_now, "fl")->valuestring);
    print_center_mix(8, cJSON_GetObjectItem(weather_now, "tmp")->valuestring);
    print_center_mix(8, cJSON_GetObjectItem(weather_now, "cond_txt")->valuestring);
    print_center_mix(11, cJSON_GetObjectItem(weather_now, "wind_deg")->valuestring);
    print_center_mix(10, cJSON_GetObjectItem(weather_now, "wind_dir")->valuestring);
    print_center_mix(8, cJSON_GetObjectItem(weather_now, "wind_sc")->valuestring);
    print_center_mix(8, cJSON_GetObjectItem(weather_now, "wind_spd")->valuestring);
    print_center_mix(10, cJSON_GetObjectItem(weather_now, "hum")->valuestring);
    print_center_mix(10, cJSON_GetObjectItem(weather_now, "pcpn")->valuestring);
    print_center_mix(10, cJSON_GetObjectItem(weather_now, "pres")->valuestring);
    print_center_mix(8, cJSON_GetObjectItem(weather_now, "vis")->valuestring);
    print_center_mix(8, cJSON_GetObjectItem(weather_now, "cloud")->valuestring);
    printf("\n");
}

void show_hourly_header() {
    print_line(LINE_NUM);
    printf("|");
    print_center_mix(LINE_NUM, "逐小时天气预报");
    printf("\n");
    print_line(LINE_NUM);
    printf("|");
    print_center_mix(18, "预报时间");
    printf(" %s |", "温 度");
    printf(" %s |", "  天  气  ");
    printf(" %s |", "风向角");
    printf(" %s |", " 风  向 ");
    printf(" %s |", "  风力  ");
    printf(" %s |", "风 速");
    printf(" %s |", "相对湿度");
    printf(" %s |", "大气压强");
    printf(" %s |", "露点温度");
    printf(" %s |\n", "云  量");
}

void show_hourly_info(cJSON *weather_hourly) {
    print_line(LINE_NUM);
    int length = cJSON_GetArraySize(weather_hourly);
    cJSON *item;
    for (int i = 0; i < length; i++) {
        item = cJSON_GetArrayItem(weather_hourly, i);
        printf("| %s |", cJSON_GetObjectItem(item, "time")->valuestring);
        print_center_mix(7, cJSON_GetObjectItem(item, "tmp")->valuestring);
        print_center_mix(12, cJSON_GetObjectItem(item, "cond_txt")->valuestring);
        print_center_mix(8, cJSON_GetObjectItem(item, "wind_deg")->valuestring);
        print_center_mix(10, cJSON_GetObjectItem(item, "wind_dir")->valuestring);
        print_center_mix(10, cJSON_GetObjectItem(item, "wind_sc")->valuestring);
        print_center_mix(7, cJSON_GetObjectItem(item, "wind_spd")->valuestring);
        print_center_mix(10, cJSON_GetObjectItem(item, "hum")->valuestring);
        print_center_mix(10, cJSON_GetObjectItem(item, "pres")->valuestring);
        print_center_mix(10, cJSON_GetObjectItem(item, "dew")->valuestring);
        print_center_mix(8, cJSON_GetObjectItem(item, "cloud")->valuestring);
        printf("\n");
    }
}

char *get_type(cJSON *data) {
    char *result = calloc(20, sizeof(char));
    char *tmp = calloc(10, sizeof(char));
    strcpy(tmp, cJSON_GetObjectItem(data, "type")->valuestring);
    if (strcmp(tmp, "comf") == 0) {
        result = "舒适度";
    } else if (strcmp(tmp, "cw") == 0) {
        result = "洗车指数";
    } else if (strcmp(tmp, "drsg") == 0) {
        result = "穿衣指数";
    } else if (strcmp(tmp, "flu") == 0) {
        result = "感冒指数";
    } else if (strcmp(tmp, "sport") == 0) {
        result = "运动指数";
    } else if (strcmp(tmp, "trav") == 0) {
        result = "旅行指数";
    } else if (strcmp(tmp, "uv") == 0) {
        result = "紫外线指数";
    } else if (strcmp(tmp, "air") == 0) {
        result = "空气扩散指数";
    }
    free(tmp);
    return result;
}

void show_lifestyle_header() {
    print_line(LINE_NUM);
    printf("|");
    print_center_mix(LINE_NUM, "生活指数");
    printf("\n");
    print_line(LINE_NUM);
    printf("|");
    print_center_mix(14, "类型");
    print_center_mix(10, "舒适度");
    print_center_mix(LINE_NUM - 16 - 10, "详细信息");
    printf("\n");
}

void show_lifestyle_info(cJSON *lifestyle) {
    print_line(LINE_NUM);
    int length = cJSON_GetArraySize(lifestyle);
    char *type;
    for (int i = 0; i < length; i++) {
        cJSON *item = cJSON_GetArrayItem(lifestyle, i);
        type = get_type(item);
        printf("|");
        print_center_mix(14, type);
//        print_center_mix(10, cJSON_GetObjectItem(item, "brf").valuestring);
        print_center_mix(LINE_NUM - 15, cJSON_GetObjectItem(item, "txt")->valuestring);
        printf("\n");
    }
}

void show_weather(cJSON *data, int show_basic, int show_forecast, int show_now,
                  int show_hourly, int show_lifestyle, int show_time) {
    if (show_basic) {
        if (cJSON_GetObjectItem(data, "basic") != NULL) {
            show_basic_info(cJSON_GetObjectItem(data, "basic"), 0);
        }
    }
    if (show_forecast) {
        if (cJSON_GetObjectItem(data, "daily_forecast") != NULL) {
            show_forecast_header();
            show_forecast_info(cJSON_GetObjectItem(data, "daily_forecast"));
        }
    }

    if (show_now) {
        if (cJSON_GetObjectItem(data, "now") != NULL) {
            show_now_header();
            show_now_info(cJSON_GetObjectItem(data, "now"));
        }
    }
    if (show_hourly) {
        if (cJSON_GetObjectItem(data, "hourly") != NULL) {
            show_hourly_header();
            show_hourly_info(cJSON_GetObjectItem(data, "hourly"));
        }
    }
    if (show_lifestyle) {
        if (cJSON_GetObjectItem(data, "lifestyle") != NULL) {
            show_lifestyle_header();
            show_lifestyle_info(cJSON_GetObjectItem(data, "lifestyle"));
        }
    }
    if (show_time) {
        if (cJSON_GetObjectItem(data, "update") != NULL) {
            show_update_info(cJSON_GetObjectItem(data, "update"), 1);
        }
    }
    print_line(LINE_NUM);
}

void show_air_header() {
    print_line(LINE_NUM);
    printf("|");
    print_center_mix(LINE_NUM, "AQI城市实况");
    printf("\n");
    print_line(LINE_NUM);
    printf("|");
    print_center_mix(18, "更新时间");
    print_center_mix(14, "空气质量指数");
    print_center_mix(12, "主要污染物");
    print_center_mix(10, "空气质量");
    print_center_mix(10, "PM10含量");
    print_center_mix(10, "PM25含量");
    print_center_mix(10, "二氧化氮");
    print_center_mix(10, "二氧化硫");
    print_center_mix(10, "一氧化碳");
    print_center_mix(7, "臭氧");
    printf("\n");
}

void show_air_info(cJSON *data) {
    print_line(LINE_NUM);
    printf("|");
    print_center_mix(18, cJSON_GetObjectItem(data, "pub_time")->valuestring);
    print_center_mix(14, cJSON_GetObjectItem(data, "aqi")->valuestring);
    print_center_mix(12, cJSON_GetObjectItem(data, "main")->valuestring);
    print_center_mix(10, cJSON_GetObjectItem(data, "qlty")->valuestring);
    print_center_mix(10, cJSON_GetObjectItem(data, "pm10")->valuestring);
    print_center_mix(10, cJSON_GetObjectItem(data, "pm25")->valuestring);
    print_center_mix(10, cJSON_GetObjectItem(data, "no2")->valuestring);
    print_center_mix(10, cJSON_GetObjectItem(data, "so2")->valuestring);
    print_center_mix(10, cJSON_GetObjectItem(data, "co")->valuestring);
    print_center_mix(7, cJSON_GetObjectItem(data, "o3")->valuestring);
    printf("\n");
}

void show_air(cJSON *air, int show_basic, int show_air, int show_time) {
    if (show_basic) {
        if (cJSON_GetObjectItem(air, "basic") != NULL) {
            show_basic_info(cJSON_GetObjectItem(air, "basic"), 0);
        }
    }
    if (show_air) {
        if (cJSON_GetObjectItem(air, "air_now_city") != NULL) {
            show_air_header();
            show_air_info(cJSON_GetObjectItem(air, "air_now_city"));
        }
    }
    if (show_time) {
        if (cJSON_GetObjectItem(air, "update") != NULL) {
            show_update_info(cJSON_GetObjectItem(air, "update"), 1);
        }
    }
    print_line(LINE_NUM);
}

void show_weather_by_args(cJSON *data, WeatherArgs *weatherArgs) {
    if (weatherArgs->full_info) {
        show_weather(data, 1, 1, 1, 1, 1, 1);
    } else {
        show_weather(data, 1, weatherArgs->forecast, weatherArgs->now, weatherArgs->hourly, weatherArgs->lifestyle, 1);
    }
};

void show_air_by_args(cJSON *data) {
    show_air(data, 1, 1, 1);
}
