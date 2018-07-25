//
// Created by 冉高飞 on 2018/7/24.
//

#ifndef SWEATHER_UPDATETIME_H
#define SWEATHER_UPDATETIME_H

#include "CJson/cJSON.h"
typedef struct update_time {
    char *loc;
    char *utc;
} UpdateTime;

void processUpdateTime(cJSON *item);

#endif //SWEATHER_UPDATETIME_H
