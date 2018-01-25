#include "ParseWeather/tool.h"
#include "location.h"

#define LINE_NUM 86

LocationArray *get_target_cities(char *city_info) {
    FILE *city_list = fopen(FILE_LIST, "r");
    if (city_list == NULL) {
        printf("未找到城市列表，请重新下载文件%s", FILE_LIST);
        return NULL;
    }

    Location *locations = calloc(ARRAY_NUM, sizeof(Location));
    char *target_city_num = calloc(1000, sizeof(char));
    int i = 0;
    while (!feof(city_list) && i < ARRAY_NUM) {
        fgets(target_city_num, 1000, city_list);
        if (strstr(target_city_num, city_info) != NULL) {
            sscanf(target_city_num, "%s%s%s%s%s%s%s%s%s%s%s%s",
                   (&locations[i])->area_num,
                   (&locations[i])->area_en,
                   locations[i].area_cn,
                   locations[i].country_num,
                   locations[i].country_en,
                   locations[i].country_cn,
                   locations[i].province_en,
                   locations[i].province_cn,
                   locations[i].pcity_en,
                   locations[i].pcity_cn,
                   locations[i].lat,
                   locations[i].lon
            );
            ++i;
        }
    }
    if (i <= 0) {
        return NULL;
    }
    LocationArray *result = calloc(1, sizeof(LocationArray));
    result->location = locations;
    result->length = i;
    return result;
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
    for (int i = 0; i < locationArray->length; i++) {
        show_loc_header();
        show_loc_info(&(locationArray->location[i]));
    }
    print_line(LINE_NUM);
}

void show_default_location() {
    char *num = calloc(11, sizeof(char));
    FILE_STATE state = get_default_num(num);
    LocationArray *locationArray = NULL;
    switch (state) {
        case SUCCESS:
            locationArray = get_target_cities(num);
            show_location_info_full(locationArray);
            break;
        default:
            break;
    }
    if (locationArray != NULL) {
        free(locationArray);
    }
    free(num);
}

void get_location(char *location) {
    LocationArray *locationArray = get_target_cities(location);
    if (locationArray == NULL) {
        printf("您输入的地址有误：%s", location);
        return;
    }
    show_location_info_full(locationArray);
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
 * 正则表达式匹配地址编号                      *
 * @param src                               *
 * @return                                  *
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
void set_city_name(char *city_num) {
    FILE *city_list = fopen(FILE_LIST, "r");
    if (city_list == NULL) {
        printf("未找到城市列表，请重新下载文件%s", FILE_LIST);
        return;
    }

    Location *locations = malloc(sizeof(Location) * 30);
    char *target_city_num = malloc(1000 * sizeof(char));
    int i = 0;
    while (!feof(city_list) && i < 30) {
        fgets(target_city_num, 1000, city_list);
        if (strstr(target_city_num, city_num) != NULL) {
            sscanf(target_city_num, "%11s%s%s%s%s%s%s%s%s%s%s%s",
                   locations[i].area_num,
                   locations[i].area_en,
                   locations[i].area_cn,
                   locations[i].country_num,
                   locations[i].country_en,
                   locations[i].country_cn,
                   locations[i].province_en,
                   locations[i].province_cn,
                   locations[i].pcity_en,
                   locations[i].pcity_cn,
                   locations[i].lat,
                   locations[i].lon
            );
            printf("%2d: %s-%s\n", i + 1, locations[i].area_cn, locations[i].pcity_cn);
            ++i;
        }
    }


    FILE *conf = fopen(FILE_NAME, "wb");
    if (conf == NULL) {
        printf("准备查询数据列表");
    }
    if (i <= 0) {
        printf("你输入的地址有误，请重试");
    } else {
        int num = 0;
        if (i == 1) {
            num = 1;
        }
        if (i > 1) {
            printf("共搜索到%d个城市，列表展示格式为\"区-市-省\"\n", i);
            printf("请输入您要选择的城市序号：");
            scanf("%d", &num);
            fflush(stdin);
            while ((num - 1) > i || (num - 1) < 0) {
                printf("您输入的编号错误,请重新输入：");
                scanf("%d", &num);
                fflush(stdin);
            }
        }
        fwrite(locations[num - 1].area_num, sizeof(char) * 11, 1, conf);
        printf("您已设置成功，请使用命令查询天气");
    }
    free(target_city_num);
    free(locations);
    fclose(conf);
    fclose(city_list);
}