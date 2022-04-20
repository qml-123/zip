//
// Created by ByteDance on 2022/4/13.
//

#include "com.h"

int PORT;
int PORT2;
const long long head = pow(10, 9);
const long long second = pow(10, 8);

int check(char* s) {
    for (int i = 0; s[i]; i++) {
        if (!isdigit(s[i]))
            return 1;
    }
    return 0;
}


int get_file_size_by_stat(const char *file) {
    int ret;
    struct stat file_info;
    ret = stat(file, &file_info);
    return (!ret) ? file_info.st_size : -1;
}