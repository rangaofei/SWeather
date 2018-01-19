#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <regex.h>

#ifdef ENABLE_DEBUG
#define FILE_NAME "../src/city.txt"
#define FILE_LIST "../src/china-city-list.txt"
#else
#define FILE_NAME "/usr/local/etc/city.txt"
#define FILE_LIST "/usr/local/etc/china-city-list.txt"
#endif

#define PATTERN "^CN\\d{5,}$"

/**************************
 * 定义结构体存储地址信息    *
 **************************/
typedef struct {
    char area_num[11];
    char area_en[30];
    char area_cn[20];
    char country_num[2];
    char country_en[10];
    char country_cn[10];
    char province_en[10];
    char province_cn[10];
    char pcity_en[30];
    char pcity_cn[30];
    char lat[10];
    char lon[10];
} Location;

typedef enum {
    SUCCESS=1, NOT_FOUND, TYPE_INCORRECT
} FILE_STATE;

void get_simple_location(char *loc_num);

void get_full_location(char *loc_num);

FILE_STATE get_default_num(char *num);

int regex_match_result(char *des);