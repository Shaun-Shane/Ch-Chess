#include <shlwapi.h>
#include <stdio.h>
#include <string.h>
#include <windows.h>

#include <algorithm>

inline char *strcasestr(const char *str1, const char *str2) {
    return StrStrIA(str1, str2);
}

inline bool strequal(const char *str1, const char *str2) {
    return strncasecmp(str1, str2, strlen(str2)) == 0;
}

inline bool strequalskip(const char *&str1, const char *str2) {
    if (strncasecmp(str1, str2, strlen(str2)) == 0) {
        str1 += strlen(str2);
        return true;
    } else {
        return false;
    }
}

inline bool strequalskip(char *&str1, const char *str2) {
    if (strncasecmp(str1, str2, strlen(str2)) == 0) {
        str1 += strlen(str2);
        return true;
    } else {
        return false;
    }
}

inline bool strscanskip(const char *&str1, const char *str2) {
    const char *strPtr;
    strPtr = strcasestr(str1, str2);
    if (strPtr == NULL) {
        return false;
    } else {
        str1 = strPtr + strlen(str2);
        return true;
    }
}

inline bool strscanskip(char *&str1, const char *str2) {
    char *strPtr;
    strPtr = strcasestr(str1, str2);
    if (strPtr == NULL) {
        return false;
    } else {
        str1 = strPtr + strlen(str2);
        return true;
    }
}

inline int str2digit(const char *str, int Min, int Max) {
    int Time;
    if (sscanf(str, "%d", &Time) > 0) {
        return std::min(std::max(Time, Min), Max);
    } else {
        return Min;
    }
}
