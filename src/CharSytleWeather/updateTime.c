//
// Created by 冉高飞 on 2018/7/24.
//
#include "updateTime.h"

void print_update_time(UpdateTime *updateTime) {

    fprintf(stdout, "更新时间：%s\n", updateTime->loc);
    fprintf(stdout, "\n");
}

void processUpdateTime(cJSON *item) {
    UpdateTime updateTime;
    updateTime.loc = cJSON_GetObjectItem(item, "loc")->valuestring;
    updateTime.utc = cJSON_GetObjectItem(item, "utc")->valuestring;
    print_update_time(&updateTime);
}
