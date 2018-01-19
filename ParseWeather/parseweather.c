#include <CJson/cJSON.h>
#include "parseweather.h"

#define UPDATE_TIME "更新时间：%s\n"

//打印更新时间字段--"update"
void print_update(cJSON *update_time, int loc) {
    if (loc) {
        print_weather(UPDATE_TIME, update_time, "loc");
    } else {
        print_weather(UPDATE_TIME, update_time, "utc");
    }
}

//打印基本信息字段--"basic"
void print_basic(cJSON *basic, int full) {
    if (!full) {
        print_weather_two("城市：%s-%s\n", basic, "parent_city", "location");
    } else {

    }
}

//打印现在的天气信息字段--"now"
void print_now(cJSON *now) {
    print_weather("天气：%s\n", now, "cond_txt");
    print_weather("体感温度：%s\n", now, "fl");
    print_weather("温度：%s\n", now, "tmp");
    print_weather("风向：%s\n", now, "wind_dir");
    print_weather("风力：%s\n", now, "wind_sc");
    print_weather("相对湿度：%s\n", now, "hum");
    print_weather("能见度：%s\n", now, "vis");
}

void print_line() {
    printf("|");
    for (int i = 0; i < TAB_LENGTH * 4 - 1; i++) {
        printf("-");
    }
    printf("|\n");
}

void print_rect() {
    print_line();
    printf("|");
    printf("%10s\t|", "日期");
    printf("%s\t|", "白天天气");
    printf("%s\t|", "夜间天气");
    printf("%7s\t|", "日出");
    printf("%7s\t|", "日落");
    printf("%7s\t|", "温度");
    printf("%7s\t|", "风向");
    printf("%7s\t|", "风力");
    printf("%7s\t|", "紫外线");
    printf("%7s\t|\n", "能见度");
    print_line();
}

//打印三天的天气字段--"forecast"
void print_forecast(cJSON *forecast_list) {
    int length = cJSON_GetArraySize(forecast_list);
    cJSON *item;
    print_rect();
    for (int i = 0; i < length; ++i) {
        item = cJSON_GetArrayItem(forecast_list, i);
        printf("|");
        print_weather("%10s\t|", item, "date");
        print_weather("%10s\t|", item, "cond_txt_d");
        print_weather("%10s\t|", item, "cond_txt_n");
        print_weather("%s\t|", item, "sr");
        print_weather("%s\t|", item, "ss");
        print_weather_two("%s-%s\t|", item, "tmp_min", "tmp_max");
        print_weather("%s\t|", item, "wind_sc");
        print_weather("%s\t|", item, "wind_spd");
        print_weather("%s\t|", item, "uv_index");
        print_weather("%s\t|\n", item, "vis");
    }
    print_line();
}

void weather_now(cJSON *data, int show_basic, int show_now, int show_update) {
    if (show_basic) {
        print_basic(cJSON_GetObjectItem(data, "basic"), 0);
    }
    if (show_now) {
        print_now(cJSON_GetObjectItem(data, "now"));
    }
    if (show_update) {
        print_update(cJSON_GetObjectItem(data, "update"), 1);
    }
}

void weather_forecast(cJSON *data, int show_basic, int show_forecast, int show_update) {
    if (show_basic) {
        print_basic(cJSON_GetObjectItem(data, "basic"), 0);
    }
    if (show_forecast) {
        print_forecast(cJSON_GetObjectItem(data, "daily_forecast"));
    }
    if (show_update) {
        print_update(cJSON_GetObjectItem(data, "update"), 1);
    }
}
