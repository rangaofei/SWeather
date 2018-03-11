#include <stdio.h>
#include <curl/curl.h>
#include <memory.h>
#include <stdlib.h>
#include "SimpleWeatherConfigure.h"
#include "GetWeather/getweather.h"
#include "Location/location.h"

#define SHOW_VERSION printf("SimpleWeather\n \
Version:%d.%d.%d",VERSION_MAJOR, VERSION_MINOR,VERSION_PATCH)