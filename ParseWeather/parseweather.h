#include <CJson/cJSON.h>
#include "printweather.h"
#include "CJson/cJSON.h"
#include <stdlib.h>
#include <stdio.h>


#define TAB_LENGTH 26
void print_update(cJSON *update_time, int loc);

void print_basic(cJSON *basic, int full);

void weather_now(cJSON *data, int show_basic, int show_now, int show_update);

void weather_forecast(cJSON *data, int show_basic, int show_forecast, int show_update);