#include "location.h"

void get_simple_location(char *loc_num) {

}

void get_full_location(char *loc_num) {

}

void get_num(char *loc_name) {

}

//获取设置的默认地址
FILE_STATE get_default_num(char *num) {
    int status;
    FILE *f = fopen(FILE_NAME, "r");
    if (f == NULL) {
        return NOT_FOUND;
    }
    fread(num, 1, 11, f);
    fclose(f);
    status = regex_match_result(num);
    if (status != 0) {
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