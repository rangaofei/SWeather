#include <CJson/cJSON.h>
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
 * |
 * |
 * +----------------------------------------------------------------------------------+
 */

#define UPDATE_TIME "更新时间：%s\n"
#define LINE_NUM 119

/**
 * 打印分割线
 */
void print_line() {
    printf("+");
    for (int i = 0; i < LINE_NUM; i++) {
        printf("-");
    }
    printf("+\n");
}

void print_space(int length) {
    for (int i = 0; i < length; i++) {
        printf(" ");
    }
}

/**
 * 获取地址信息
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

char *get_temp_range(cJSON *data) {
    char *temp_range = calloc(8, sizeof(char));
    strcpy(temp_range, cJSON_GetObjectItem(data, "tmp_min")->valuestring);
    strcat(temp_range, "-");
    strcat(temp_range, cJSON_GetObjectItem(data, "tmp_max")->valuestring);
    return temp_range;
}

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
    print_line();
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

void print_center_cn(int length, char *content) {
    int l = ((int) strlen(content)) / 3 * 2;
    int l_left = (length - l) / 2;
    int l_right = length - l - l_left;
    print_space(l_left);
    printf("%s", content);
    print_space(l_right);
    printf("|");
}


void print_center_en(int length, char *content) {
    int l = (int) strlen(content);
    int l_left = (length - l) / 2;
    int l_right = length - l - l_left;
    print_space(l_left);
    printf("%s", content);
    print_space(l_right);
    printf("|");
}

void print_header() {
    print_line();
    printf("|");
    print_space(4);
    printf("%s", "日期");
    print_space(4);
    printf("|");

    print_space(12);
    printf("%s", "天   气");
    print_space(12);
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

    print_line();
    printf("|");
    print_space(4);
    printf("%s", "三日");
    print_space(4);
    printf("|");

    printf("%s|", "   白天   ");
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


//打印三天的天气字段--"forecast"
void print_forecast(cJSON *forecast_list) {
    int length = cJSON_GetArraySize(forecast_list);
    cJSON *item;
    print_line();
    for (int i = 0; i < length; ++i) {
        item = cJSON_GetArrayItem(forecast_list, i);
        printf("|");
        print_weather(" %s |", item, "date");
        print_center_cn(10, cJSON_GetObjectItem(item, "cond_txt_d")->valuestring);
        print_center_cn(10, cJSON_GetObjectItem(item, "cond_txt_n")->valuestring);
        char *temp_range = get_temp_range(item);
        print_center_en(9, temp_range);
        free(temp_range);
        print_weather(" %5s |", item, "sr");
        print_weather(" %5s |", item, "ss");
        print_weather(" %5s |", item, "mr");
        print_weather(" %5s |", item, "ms");
        print_center_cn(10, cJSON_GetObjectItem(item, "wind_dir")->valuestring);
        print_weather(" %4s |", item, "wind_sc");
        print_weather(" %4s |", item, "wind_spd");
        print_weather(" %6s |", item, "uv_index");
        print_weather(" %6s |\n", item, "vis");
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
        print_header();
        print_forecast(cJSON_GetObjectItem(data, "daily_forecast"));
    }
    if (show_update) {
        print_update(cJSON_GetObjectItem(data, "update"), 1);
    }
}
