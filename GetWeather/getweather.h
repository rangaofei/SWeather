#include <stdlib.h>
#include <stdio.h>
#include "ParseWeather/parseweather.h"


#define WEATHER_NOW "now"
#define WEATHER_FORECAST "forecast"

void get_weather(char *weather_style, char *location);

int get_weather_default(char *style);
