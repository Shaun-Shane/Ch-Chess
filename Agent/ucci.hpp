#include <stdio.h>

#include <algorithm>
#include <iostream>
#include <string>

#include "parse.h"

constexpr int UCCI_MAX_DEPTH = 32;

enum UcciCommEnum {
    UCCI_COMM_UNKNOWN,
    UCCI_COMM_UCCI,
    UCCI_COMM_ISREADY,
    UCCI_COMM_POSITION,
    UCCI_COMM_GO,
    UCCI_COMM_QUIT
};

struct UcciCommStruct {
    const char *fenStr;     // fen串
    int moveNum;            // moves后续走法个数
    char (*movesCoord)[5];  //记录moves后的走法
    int Time;
};

UcciCommStruct UcciComm;

const char *const startFen =
    "rnbakabnr/9/1c5c1/p1p1p1p1p/9/9/P1P1P1P1P/1C5C1/9/RNBAKABNR w";

constexpr int MAX_MOVE_NUM = 1024;

constexpr int LINE_INPUT_MAX_CHAR = 8192;
static char Fen[LINE_INPUT_MAX_CHAR];
static char coordList[MAX_MOVE_NUM][5];

static bool parsepos(UcciCommStruct &UcciComm, char *strPtr) {
    int i;
    //获得棋盘布局
    if (comparetopstr(strPtr, "fen ", 1)) {
        strcpy(Fen, strPtr);
        UcciComm.fenStr = Fen;
    } else if (comparetopstr(strPtr, "startpos", 0)) {
        UcciComm.fenStr =
            "rnbakabnr/9/1c5c1/p1p1p1p1p/9/9/P1P1P1P1P/1C5C1/9/RNBAKABNR w";
    } else {
        return false;
    }
    //获得后续着法
    UcciComm.moveNum = 0;
    if (searchstr(strPtr, " moves ")) {
        UcciComm.moveNum = 1;
        int i = 0;
        while (strPtr[i] != '\0') {
            if (strPtr[i] == ' ') {
                UcciComm.moveNum++;
            }
            i++;
        }
        for (i = 0; i < UcciComm.moveNum; i++) {
            for (int j = 0; j < 4; j++) {
                coordList[i][j] = strPtr[j];
            }
            coordList[i][4] = '\0';
            strPtr += 5;
        }
        UcciComm.movesCoord = coordList;
    }
    return true;
}
//引导状态
UcciCommEnum bootline(void) {
    char lineStr[LINE_INPUT_MAX_CHAR];
    char *strPtr;
    while (!std::cin.getline(lineStr, LINE_INPUT_MAX_CHAR)) {
        Sleep(1);
    }
    strPtr = lineStr;
    if (comparetopstr(strPtr, "ucci", 0)) {
        return UCCI_COMM_UCCI;
    } else {
        return UCCI_COMM_UNKNOWN;
    }
}
//空闲状态
UcciCommEnum idleline(UcciCommStruct &UcciComm) {
    char lineStr[LINE_INPUT_MAX_CHAR];
    char *strPtr;

    while (!std::cin.getline(lineStr, LINE_INPUT_MAX_CHAR)) {
        Sleep(1);
    }
    strPtr = lineStr;

    if (comparetopstr(strPtr, "isready", 0)) {
        return UCCI_COMM_ISREADY;
    }

    else if (comparetopstr(strPtr, "position ", 1)) {
        return parsepos(UcciComm, strPtr) ? UCCI_COMM_POSITION
                                          : UCCI_COMM_UNKNOWN;
    }

    else if (comparetopstr(strPtr, "go", 1)) {
        return UCCI_COMM_GO;
    }

    else if (comparetopstr(strPtr, "quit", 0)) {
        return UCCI_COMM_QUIT;
    } else {
        return UCCI_COMM_UNKNOWN;
    }
}

inline void println(const char *str) {
    printf("%s\n", str);
    fflush(stdout);
}
