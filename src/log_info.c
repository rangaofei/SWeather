//
// Created by 冉高飞 on 2018/5/7.
//
#include <stdlib.h>
#include "log_info.h"

void LOG_D(char *format, void *args) {
#ifdef ENABLE_DEBUG
    fprintf(stdout, format, args);
#endif
}

char *format_msg;

void exit_err(int err_num, char *err_msg) {
    switch (err_num) {
        case ERR_FILE_NOT_FOUND:
            format_msg = "error occurred : the file <%s> not found.please retry!!";
            break;
        case ERR_ADDRESS_NOT_FOUND:
            format_msg = "error occurred : the address <%s> not found.please retry!";
            break;
        case ERR_ADDRESS_NOT_SET:
            format_msg = "error occurred : You haven't set the default city,please"
                         "use the command to set:\nsweather --set-location <city_name_%s>\n";
            break;
        default:
            break;
    }
    fprintf(stderr, format_msg, err_msg);
    exit(err_num);
}