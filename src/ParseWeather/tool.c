#include <regex.h>
#include "tool.h"


void print_line(int length) {
    printf("+");
    for (int i = 0; i < length; i++) {
        printf("-");
    }
    printf("+\n");
}

void print_space(int length) {
    for (int i = 0; i < length; i++) {
        printf(" ");
    }
}

void print_center_cn(int length, char *content) {
    int l = ((int) strlen(content)) / 3 * 2;
    int l_left = (length - l) / 2;
    int l_right = length - l - l_left;
    print_space(l_left);
    printf("%s", content);
    print_space(l_right);
    printf("|");
}


void print_center_en(int length, char *content) {
    int l = (int) strlen(content);
    int l_left = (length - l) / 2;
    int l_right = length - l - l_left;
    print_space(l_left);
    printf("%s", content);
    print_space(l_right);
    printf("|");
}

void print_center_mix(int length, char *content) {
    int cn_count = regex_cn(content);
    int char_length = (int) strlen(content);
    int en_count = char_length - cn_count * 3;
    int l = char_length - cn_count;
    int l_left = (length - l) / 2;
    int l_right = length - l - l_left;
    print_space(l_left);
    printf("%s", content);
    print_space(l_right);
    printf("|");
}


int regex_cn(char *buf) {
    int n = 0;
    for (int i = 0; buf[i] && i < strlen(buf); i++)
        if (buf[i] < 0) {
            i = i + 2;
            n++;
        }
    return n;
}
