
#include "main.h"

int main(int argc, char *argv[]) {
    if (argc < 2) {
        get_weather_default(WEATHER_DEFAULT);
        return 0;
    }
    if (argc == 2) {
        if ((strcmp(argv[1], "-v") == 0) || strcmp(argv[1], "-version") == 0) {
            SHOW_VERSION;
        } else if ((strcmp(argv[1], "-loc")) == 0) {
            show_default_location();
        } else if ((strcmp(argv[1], "-now") == 0)) {
            get_weather_default(WEATHER_NOW);
        } else if ((strcmp(argv[1], "-forecast")) == 0) {
            get_weather_default(WEATHER_FORECAST);
        } else if ((strcmp(argv[1], "-hourly")) == 0) {
            get_weather_default(WEATHER_HOURLY);
        } else if ((strcmp(argv[1], "-lifestyle")) == 0) {
            get_weather_default(WEATHER_LIFESTYLE);
        } else if ((strcmp(argv[1], "-air")) == 0) {
            get_weather_default(AIR_DEFAULT);
        } else if ((strcmp(argv[1], "-airnow")) == 0) {
            get_weather_default(AIR_NOW);
        } else if (argv[1][0] != '-') {
            get_weather(WEATHER_DEFAULT, argv[1]);
        }
    }

    if (argc == 3) {
        if ((strcmp(argv[1], "-loc") == 0)) {
            get_location(argv[2]);
        } else if (strcmp(argv[1], "-air") == 0) {
            get_weather(AIR_DEFAULT, argv[2]);
        } else if (strcmp(argv[1], "-setloc") == 0) {
            set_city_name(argv[2]);
        }
    }
    return 0;
}