#include <shlwapi.h>
#include <stdio.h>
#include <string.h>
#include <windows.h>

#include <algorithm>

//转换为小写字母
inline char lower(char ch) {
    if (ch >= 'A' && ch <= 'Z') {
        ch += 32;
    }
    return ch;
}

//判断str2是否出现在str1开头,不区分大小写
inline bool comparetopstr(char *&str1, const char *str2, int skip) {
    if (strlen(str1) < strlen(str2)) return 0;
    for (int i = 0; i < strlen(str2); i++) {
        if (lower(str1[i]) != lower(str2[i])) return 0;
    }
    if (skip) str1 += strlen(str2);
    return 1;
}

//查找str2在str1中第一次出现的位置，不区分大小写
inline char *searchstr(char *&str1, const char *str2) {
    if (strlen(str1) < strlen(str2)) return nullptr;
    for (int i = 0; i < strlen(str1) - strlen(str2) + 1; i++) {
        int flag = 0;
        for (int j = 0; j < strlen(str2); j++) {
            if (lower(str1[i + j]) != lower(str2[j])) {
                flag = 1;
                break;
            }
        }
        if (flag == 0) {
            str1 += i;
            str1 += strlen(str2);
            return str1;
        }
    }
    return nullptr;
}
