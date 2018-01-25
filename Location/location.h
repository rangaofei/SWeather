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

#define ARRAY_NUM 30
#define PATTERN "^CN\\d{5,}$"

/**************************
 * 定义结构体存储地址信息    *
 **************************/
typedef struct {
    char area_num[12];
    char area_en[30];
    char area_cn[20];
    char country_num[3];
    char country_en[10];
    char country_cn[10];
    char province_en[10];
    char province_cn[10];
    char pcity_en[30];
    char pcity_cn[30];
    char lat[12];
    char lon[12];
} Location;

typedef struct {
    Location *location;
    int length;
} LocationArray;

typedef enum {
    SUCCESS = 1, NOT_FOUND, TYPE_INCORRECT
} FILE_STATE;


FILE_STATE get_default_num(char *num);

int regex_match_result(char *des);

void set_city_name(char *city_num);

void get_location(char *location);
char *check_location(char *location);

void show_default_location();