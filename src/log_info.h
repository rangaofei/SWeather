//
// Created by 冉高飞 on 2018/5/7.
//

#ifndef SWEATHER_DEBUG_LOG_H
#define SWEATHER_DEBUG_LOG_H

#include <stdio.h>

#define ERR_FILE_NOT_FOUND -1
#define ERR_ADDRESS_NOT_FOUND -2
#define ERR_ADDRESS_NOT_SET -3

void LOG_D(char *format, void *args);

void exit_err(int err_num, char *err_msg);

#endif //SWEATHER_DEBUG_LOG_H
