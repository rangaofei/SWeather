
#include <memory.h>
#include "getweather.h"
#include "main.h"
#include "strcut_info.h"
#include "location.h"
#include "log_info.h"

WeatherArgs context;

/**
 * 初始化context
 * @param context
 * @return
 */
int init_weather_args(WeatherArgs *context) {
    context->full_info = true;
    context->version = false;
    context->location = NULL;
    context->now = false;
    context->forecast = false;
    context->hourly = false;
    context->lifestyle = false;
    context->air = false;
    context->set_location = NULL;
    context->location_num = calloc(12, sizeof(char));
    return 0;
}

/**
 * 处理args,并把args写入context
 */
void processArgs() {
    if (context.version) {
        SHOW_VERSION;
    }
    if (context.set_location != NULL) {
        set_city_name(context.set_location, context.location_num);
        return;
    }

    if (context.location == NULL) {
        FILE_STATE state = get_default_num(context.location_num);
        switch (state) {
            case SUCCESS:
                break;
            case NOT_FOUND:
            case TYPE_INCORRECT:
                exit_err(ERR_ADDRESS_NOT_SET, "pinyin");
                break;
        }

    } else {
        check_location(context.location, context.location_num);
    }
    get_weather_by_args();
}


int main(int argc, char *argv[]) {
    int opt = 0;
    int longIndex = 0;
    init_weather_args(&context);
    if (argc < 2) {//当只传入sweather时，直接显示全部信息
        context.full_info = true;
    }
    while ((opt = getopt_long(argc, argv, optString, longOpts, &longIndex)) != -1) {
        switch (opt) {
            case 'v':
                context.full_info = false;
                context.version = true;
                break;
            case 'l':
                context.full_info = false;
                context.location = optarg;
                break;
            case 'n':
                context.full_info = false;
                context.now = true;
                break;
            case 'f':
                context.full_info = false;
                context.forecast = true;
                break;
            case 'h':
                context.full_info = false;
                context.hourly = true;
                break;
            case 'L':
                context.full_info = false;
                context.lifestyle = true;
                break;
            case 's':
                context.full_info = false;
                context.set_location = optarg;
                break;
            case 0:
                if (strcmp("set_location", longOpts[longIndex].name) == 0) {
                    context.set_location = optarg;
                } else if (strcmp("location", longOpts[longIndex].name) == 0) {
                    context.location = optarg;
                }
                break;
            default:
                printf("Wrong arguments\n");
                break;
        }
    }
    processArgs();
    return 0;

}




