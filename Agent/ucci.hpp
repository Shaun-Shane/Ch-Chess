#include <stdio.h>
#include <string>
#include <iostream>
#include <algorithm>
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
    const char *fenStr;  //
    int moveNum;
    char (*movesCoord)[5];  //记录moves后的走法
    int Time;
};

UcciCommStruct UcciComm;

const char* const startFen =
    "rnbakabnr/9/1c5c1/p1p1p1p1p/9/9/P1P1P1P1P/1C5C1/9/RNBAKABNR w";

constexpr int MAX_MOVE_NUM = 1024;

constexpr int LINE_INPUT_MAX_CHAR = 8192;
static char Fen[LINE_INPUT_MAX_CHAR];
static char coordList[MAX_MOVE_NUM][5];

static bool parsepos(UcciCommStruct &UcciComm, char *strPointer) {
    int i;
    if (streqvskip(strPointer, "fen ")) {
        strcpy(Fen, strPointer);
        UcciComm.fenStr = Fen;
    } else if (streqv(strPointer, "startpos")) {
        UcciComm.fenStr =
            "rnbakabnr/9/1c5c1/p1p1p1p1p/9/9/P1P1P1P1P/1C5C1/9/RNBAKABNR w";
    } else {
        return false;
    }

    UcciComm.moveNum = 0;
    if (strscanskip(strPointer, " moves ")) {
        *(strPointer - strlen(" moves ")) = '\0';
        UcciComm.moveNum = std::min((int)(strlen(strPointer) + 1) / 5, MAX_MOVE_NUM);
        for (i = 0; i < UcciComm.moveNum; i++) {
            coordList[i][0] = *strPointer;
            coordList[i][1] = *(strPointer + 1);
            coordList[i][2] = *(strPointer + 2);
            coordList[i][3] = *(strPointer + 3);
            coordList[i][4] = '\0';
            strPointer += 5;
        }
        UcciComm.movesCoord = coordList;
    }
    return true;
}

UcciCommEnum bootline(void) {
    char lineStr[LINE_INPUT_MAX_CHAR];

    while (!std::cin.getline(lineStr, LINE_INPUT_MAX_CHAR)) {
        Sleep(1);
    }
    if (streqv(lineStr, "ucci")) {
        return UCCI_COMM_UCCI;
    } else {
        return UCCI_COMM_UNKNOWN;
    }
}

UcciCommEnum idleline(UcciCommStruct &UcciComm, bool Debug) {
    char lineStr[LINE_INPUT_MAX_CHAR];
    char *strPointer;

    while (!std::cin.getline(lineStr, LINE_INPUT_MAX_CHAR)) {
        Sleep(1);
    }
    strPointer = lineStr;
    if (Debug) {
        printf("info idleline [%s]\n", strPointer);
        fflush(stdout);
    }
    if (false) {
    } else if (streqv(strPointer, "isready")) {
        return UCCI_COMM_ISREADY;
    }

    else if (streqvskip(strPointer, "position ")) {
        return parsepos(UcciComm, strPointer) ? UCCI_COMM_POSITION : UCCI_COMM_UNKNOWN;
    }

    else if (streqvskip(strPointer, "go")) {
        if (streqvskip(strPointer, " Time ")) {
            UcciComm.Time = str2digit(strPointer, 0, 2000000000);
        }
        return UCCI_COMM_GO;
    }

    else if (streqv(strPointer, "quit")) {
        return UCCI_COMM_QUIT;
    } else {
        return UCCI_COMM_UNKNOWN;
    }
}

inline void println(const char* str) {
    printf("%s\n", str);
    fflush(stdout);
}
