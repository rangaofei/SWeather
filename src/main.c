
#include <memory.h>
#include "getweather.h"
#include "main.h"

int init_weather_args(WeatherArgs *weatherArgs) {
    weatherArgs->full_info = true;
    weatherArgs->version = false;
    weatherArgs->location = NULL;
    weatherArgs->now = false;
    weatherArgs->forecast = false;
    weatherArgs->hourly = false;
    weatherArgs->lifestyle = false;
    weatherArgs->air = false;
    weatherArgs->set_location = NULL;
    weatherArgs->location_num = calloc(12, sizeof(char));
    return 0;
}

int main(int argc, char *argv[]) {
    int opt = 0;
    int longIndex = 0;
    WeatherArgs args;
    init_weather_args(&args);
    if (argc < 2) {
        args.full_info = true;
    }
    while ((opt = getopt_long(argc, argv, optString, longOpts, &longIndex)) != -1) {
        switch (opt) {
            case 'v':
                args.full_info = false;
                args.version = true;
                break;
            case 'l':
                args.full_info = false;
                args.location = optarg;
                break;
            case 'n':
                args.full_info = false;
                args.now = true;
                break;
            case 'f':
                args.full_info = false;
                args.forecast = true;
                break;
            case 'h':
                args.full_info = false;
                args.hourly = true;
                break;
            case 'L':
                args.full_info = false;
                args.lifestyle = true;
                break;
            case 's':
                args.full_info = false;
                args.set_location = optarg;
                break;
            case 0:
                printf("no argument\n");
                if (strcmp("set-location", longOpts[longIndex].name) == 0) {
                    args.set_location = optarg;
                } else if (strcmp("location", longOpts[longIndex].name) == 0) {
                    args.location = optarg;
                }
                break;
            default:
//                printf("Wrong arguments\n");
                break;
        }
    }
    get_weather_by_args(&args);
    return 0;

}


