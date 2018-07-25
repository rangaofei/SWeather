//
// Created by 冉高飞 on 2018/7/22.
//

#ifndef SWEATHER_BASICINFO_H
#define SWEATHER_BASICINFO_H

#include "CJson/cJSON.h"

typedef struct basic_info {
    char *location;
    char *cid;
    char *lat;
    char *lon;
    char *parent_city;
    char *admin_area;
    char *cnty;
    char *tz;
} BasicInfo;

void process_basic_info(cJSON *basicInfo);

#endif //SWEATHER_BASICINFO_H
