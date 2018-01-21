
#include "main.h"


void get_city_name(char *city_num, char *file_name) {
    FILE *f = fopen(file_name, "r");
    if (f == NULL) {
        printf("您还未设置您所在的城市，请设置您的城市名称：\n    sweather -setloc <location>");
        exit(0);
    }
    fread(city_num, 1, sizeof(char) * 11, f);
    fclose(f);
}

void set_city_name(char *city_num, char *file_name) {
    FILE *f = fopen(file_name, "wb");
    FILE *f_list = fopen(FILE_LIST, "r");
    Location *locations = malloc(sizeof(Location) * 30);
    char *target_city_num;
    target_city_num = malloc(1000 * sizeof(char));
    if (f_list == NULL) {
        printf("发生未知错误%s", FILE_LIST);
        exit(0);
    }
    if (f == NULL) {
        printf("正在准备配置文件");
    }
    int i = 0;
    while (!feof(f_list)) {
        fgets(target_city_num, 1000, f_list);
        if (strstr(target_city_num, city_num) != NULL) {
            sscanf(target_city_num, "%s%s%s%s%s%s%s%s%s%s%s%s",
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

            printf("%3d: %s-%s\n", i + 1, locations[i].area_cn, locations[i].pcity_cn);
            ++i;
        }
    }
    if (i <= 0) {
        printf("你输入的地址有误，请重试");
        exit(-1);
    }
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
            printf("您输入的编号错误,请重试：");
            scanf("%d", &num);
            fflush(stdin);
        }
    }

    fwrite(locations[num - 1].area_num, sizeof(char) * 11, 1, f);
    printf("您已设置成功，请使用命令查询天气");
    free(target_city_num);
    free(locations);
    fclose(f);
    fclose(f_list);
}

int main(int argc, char *argv[]) {
    char *city_num = calloc(12, sizeof(char));
    if (argc < 2) {
        get_weather_default(WEATHER_DEFAULT);
    }
    if (argc == 2) {
        if ((strcmp(argv[1], "-v") == 0) || strcmp(argv[1], "-version") == 0) {
            printf("SimpleWeather --version\n%d.%d\n",
                   VERSION_MAJOR, VERSION_MINOR);
            printf("Curl --version\n    %s\n",
                   curl_version());
            printf("cJSON --info\n    %s\n",
                   "https://github.com/DaveGamble/cJSON");
        } else if ((strcmp(argv[1], "-now") == 0)) {
            get_city_name(city_num, FILE_NAME);
            get_weather(WEATHER_NOW, city_num);
        } else if ((strcmp(argv[1], "-forecast")) == 0) {
            get_city_name(city_num, FILE_NAME);
            get_weather_default(WEATHER_FORECAST);
        } else if ((strcmp(argv[1], "-hourly")) == 0) {
            get_weather(city_num, FILE_NAME);
            get_weather_default(WEATHER_HOURLY);
        } else if((strcmp(argv[1],"-lifestyle"))==0){
            get_weather(city_num, FILE_NAME);
            get_weather_default(WEATHER_LIFESTYLE);
        }
    }

    if (argc == 3) {
        if ((strcmp(argv[1], "-loc") == 0)) {
            get_weather(WEATHER_DEFAULT, argv[2]);
        } else if (strcmp(argv[1], "-setloc") == 0) {
            set_city_name(argv[2], FILE_NAME);
        }
    }
    free(city_num);
    return 0;
}