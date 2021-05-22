#include <shlwapi.h>
#include <stdio.h>
#include <string.h>
#include <windows.h>

#include <algorithm>

inline char *strcasestr(const char *str1, const char *str2) {
    return StrStrIA(str1, str2);
}

inline bool streqv(const char *str1, const char *str2) {
    return strncasecmp(str1, str2, strlen(str2)) == 0;
}

inline bool streqvskip(const char *&str1, const char *str2) {
    if (strncasecmp(str1, str2, strlen(str2)) == 0) {
        str1 += strlen(str2);
        return true;
    } else {
        return false;
    }
}

inline bool streqvskip(char *&str1, const char *str2) {
    if (strncasecmp(str1, str2, strlen(str2)) == 0) {
        str1 += strlen(str2);
        return true;
    } else {
        return false;
    }
}

inline bool strscanskip(const char *&str1, const char *str2) {
    const char *strPointer;
    strPointer = strcasestr(str1, str2);
    if (strPointer == NULL) {
        return false;
    } else {
        str1 = strPointer + strlen(str2);
        return true;
    }
}

inline bool strscanskip(char *&str1, const char *str2) {
    char *strPointer;
    strPointer = strcasestr(str1, str2);
    if (strPointer == NULL) {
        return false;
    } else {
        str1 = strPointer + strlen(str2);
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
