//
// Created by 冉高飞 on 2018/7/22.
//
#include "basicInfo.h"

char *simple_basic_info = "%s,%s,%s\n";

void print_basic_info_simple(BasicInfo *basicInfo) {
    fprintf(stdout, "\n");
    fprintf(stdout, simple_basic_info, basicInfo->location,
            basicInfo->parent_city, basicInfo->admin_area);
    fprintf(stdout, "\n");
}

void print_basic_info_full(BasicInfo *basicInfo){
    fprintf(stdout, "\n");
    fprintf(stdout, simple_basic_info, basicInfo->location,
            basicInfo->parent_city, basicInfo->admin_area);
    fprintf(stdout, "\n");
}

void process_basic_info(cJSON *basicInfo) {
    BasicInfo info;
    info.location = cJSON_GetObjectItem(basicInfo, "location")->valuestring;
    info.cid = cJSON_GetObjectItem(basicInfo, "cid")->valuestring;
    info.lat = cJSON_GetObjectItem(basicInfo, "lat")->valuestring;
    info.lon = cJSON_GetObjectItem(basicInfo, "lon")->valuestring;
    info.parent_city = cJSON_GetObjectItem(basicInfo, "parent_city")->valuestring;
    info.admin_area = cJSON_GetObjectItem(basicInfo, "admin_area")->valuestring;
    info.cnty = cJSON_GetObjectItem(basicInfo, "cnty")->valuestring;
    info.tz = cJSON_GetObjectItem(basicInfo, "tz")->valuestring;
    print_basic_info_simple(&info);
}
