
#include <regex.h>

#include "ParseWeather/tool.h"
#include "location.h"
#include "log_info.h"

#define LINE_NUM 86

int get_target_cities(LocationArray *locations, char *city_info) {
    FILE *city_list = fopen(FILE_LIST, "r");
    if (city_list == NULL) {
        exit_err(ERR_FILE_NOT_FOUND, FILE_LIST);
    }

    Location *location = calloc(ARRAY_NUM, sizeof(Location));
    char *target_city_num = calloc(1000, sizeof(char));
    int i = 0;
    while (!feof(city_list) && i < ARRAY_NUM) {
        fgets(target_city_num, 1000, city_list);
        if (strstr(target_city_num, city_info) != NULL) {
            sscanf(target_city_num, "%s%s%s%s%s%s%s%s%s%s%s%s",
                   location[i].area_num,
                   location[i].area_en,
                   location[i].area_cn,
                   location[i].country_num,
                   location[i].country_en,
                   location[i].country_cn,
                   location[i].province_en,
                   location[i].province_cn,
                   location[i].pcity_en,
                   location[i].pcity_cn,
                   location[i].lat,
                   location[i].lon
            );
            ++i;
        }
    }
    locations->location = location;
    locations->length = i;
    return i;
}

void show_loc_info_simple(LocationArray *locationArray) {
    for (int i = 0; i < locationArray->length; i++) {
        printf("%2d: %s-%s-%s\n", i + 1, locationArray->location[i].area_cn,
               locationArray->location[i].pcity_cn, locationArray->location[i].province_cn);
    }
}

void show_loc_header() {
    print_line(LINE_NUM);
    printf("|");
    print_center_mix(LINE_NUM, "位置信息");
    printf("\n");
    print_line(LINE_NUM);
    printf("|");
    print_center_mix(15, "城市/地区编码");
    print_center_mix(10, "国家");
    print_center_mix(8, "省");
    print_center_mix(8, "上级市");
    print_center_mix(11, "城市/地区");
    print_center_mix(14, "经度");
    print_center_mix(14, "维度");
    printf("\n");
}

void show_loc_info(Location *location) {
    print_line(LINE_NUM);
//    char *location_cn = calloc(100, sizeof(char));
//    char *location_en = calloc(100, sizeof(char));
    printf("|");
    print_center_mix(15, location->area_num);
    print_center_mix(10, location->country_cn);
    print_center_mix(8, location->province_cn);
    print_center_mix(8, location->pcity_cn);
    print_center_mix(11, location->area_cn);
    print_center_mix(14, location->lon);
    print_center_mix(14, location->lat);
    printf("\n");
}

void show_location_info_full(LocationArray *locationArray) {
    show_loc_header();
    for (int i = 0; i < locationArray->length; i++) {
        show_loc_info(&(locationArray->location[i]));
    }
    print_line(LINE_NUM);
}

void show_default_location() {
    char *num = calloc(12, sizeof(char));
    FILE_STATE state = get_default_num(num);
    LocationArray *locationArray = calloc(1, sizeof(LocationArray));
    switch (state) {
        case SUCCESS:
            get_target_cities(locationArray, num);
            show_location_info_full(locationArray);
            break;
        case NOT_FOUND:
            printf("请输入\n\tsweather -setloc <address>\n来设置默认城市");
            break;
        case TYPE_INCORRECT:
            printf("获取配置文件错误");
            break;
    }
    if (locationArray != NULL) {
        free(locationArray);
    }
    free(num);
}

void get_location(char *location) {
    LocationArray *locationArray = calloc(1, sizeof(LocationArray));
    int length = get_target_cities(locationArray, location);
    if (length <= 0) {
        printf("您输入的地址有误：%s", location);
        return;
    }
    show_location_info_full(locationArray);
    free(locationArray);
}

void check_location(char *location, char *location_num) {
    LocationArray *locationArray = calloc(1, sizeof(LocationArray));
    get_target_cities(locationArray, location);
    if (locationArray == NULL) {
        printf("输入的地址信息有误:%s", location);
        return;
    }
    int num;
    if (locationArray->length > 1) {
        show_loc_info_simple(locationArray);
        printf("共搜索到%d个区县/城市，列表展示格式为\"区-市-省\"\n", locationArray->length);
        printf("请输入您要选择的城市序号：");
        scanf("%d", &num);
        fflush(stdin);
        while ((num - 1) > locationArray->length || (num - 1) < 0) {
            printf("您输入的编号错误,请重新输入：");
            scanf("%d", &num);
            fflush(stdin);
        }
        strcpy(location_num, (locationArray->location)[num - 1].area_num);
    } else {
        strcpy(location_num, location);
    }
    free(locationArray);
}

/**
 * 获取默认地址
 */
FILE_STATE get_default_num(char *num) {
    int status;
    FILE *f = fopen(FILE_NAME, "r");
    if (f == NULL) {
        return NOT_FOUND;
    }
    int size = (int) fread(num, 1, 11, f);
    fclose(f);
    status = regex_match_result(num);
    if (status != 0 || size != 11) {
        return TYPE_INCORRECT;
    }
    return SUCCESS;
}

/********************************************
 * 正则表达式匹配地址编号                      
 * @param src                               
 * @return                                  
 ********************************************/
int regex_match_result(char *src) {
    int state;
    const char *pattern = "^CN101[0-9]{6}$";
    int cflags = REG_EXTENDED;
    regmatch_t pmatch[1];
    const size_t nmatch = 1;
    regex_t reg;
    regcomp(&reg, pattern, cflags);
    state = regexec(&reg, src, nmatch, pmatch, 0);
    regfree(&reg);
    return state;
}

/**
 * 将城市名称写入文件
 * @param city_num
 */
void set_city_name(char *city_name, char *city_num) {
    LOG_D("the city name is %s\n", city_name);
    LocationArray *locationArray = calloc(1, sizeof(LocationArray));
    int length = get_target_cities(locationArray, city_name);
    FILE *conf = fopen(FILE_NAME, "wb");
    if (length <= 0) {
        exit_err(ERR_ADDRESS_NOT_FOUND, city_name);
    }
    int num = 0;
    if (locationArray->length == 1) {
        num = 1;
    }
    if (locationArray->length > 1) {
        show_loc_info_simple(locationArray);
        printf("共搜索到%d个城市，列表展示格式为\"区-市-省\"\n", locationArray->length);
        printf("请输入您要选择的城市序号：");
        scanf("%d", &num);
        fflush(stdin);
        while ((num - 1) > locationArray->length || (num - 1) < 0) {
            printf("您输入的编号错误,请重新输入：");
            scanf("%d", &num);
            fflush(stdin);
        }
    }
    fwrite(locationArray->location[num - 1].area_num, sizeof(char) * 11, 1, conf);
    printf("您已设置<%s-%s-%s>为默认地址\n",
           locationArray->location[num - 1].province_cn,
           locationArray->location[num - 1].pcity_cn,
           locationArray->location[num - 1].area_cn);
    memcpy(city_num, locationArray->location[num - 1].area_num, 12);
    free(locationArray);
    fclose(conf);
}