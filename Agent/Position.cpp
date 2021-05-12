#ifndef VSC_DEBUG
#include "Zobrist.h"
#else 
#include "Zobrist.cpp"
#include "source.cpp"
#endif

#include "Position.h"
#include "assert.h"
#ifdef POS_DEBUG
#include "windows.h"
#endif

Position pos;
Zobrist zob;

int_fast64_t historyTable[1 << 12] = {0};
int32_t killerTable[MAX_DISTANCE][2] = {0};
int32_t rookCapX[9][1 << 9][2] = {0};
int32_t cannonCapX[9][1 << 9][2] = {0};
int32_t rookCapY[10][1 << 10][2] = {0};
int32_t cannonCapY[10][1 << 10][2] = {0};
int32_t bitMaskY[256] = {0}, bitMaskX[256] = {0};


// 用于判断棋子是否在棋盘上
const int32_t _IN_BOARD[256] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0,
    0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0,
    0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0,
    0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0,
    0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0,
    0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0,
    0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0,
    0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0,
    0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0,
    0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

const int32_t _IN_FORT[256] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

const int32_t KAB_LEGAL_SPAN[512] = {
                         0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 3, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 2, 1, 2, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 2, 1, 2, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 3, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0
};

const int32_t _KNIGHT_PIN[512] = {
                                0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,-16,  0,-16,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0, -1,  0,  0,  0,  1,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0, -1,  0,  0,  0,  1,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0, 16,  0, 16,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0
};

/* 棋子序号对应的棋子类型
 *
 * 棋子序号从0到47，其中0到15不用且类型为空，16到31表示红子，32到47表示黑子。
 * 帅仕仕相相马马车车炮炮兵兵兵兵兵(将士士象象马马车车炮炮卒卒卒卒卒)
 * 判断棋子是红子 "pc < 32"，黑子 "pc >= 32"
 */
const int32_t pieceTypes[48] = {
    7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
    0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6, 6, 6, 6,
    0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6, 6, 6, 6
};

// lva 价值
const int32_t lvaValues[48] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    5, 1, 1, 1, 1, 3, 3, 4, 4, 3, 3, 2, 2, 2, 2, 2,
    5, 1, 1, 1, 1, 3, 3, 4, 4, 3, 3, 2, 2, 2, 2, 2
};

// mvv 价值
const int32_t mvvValues[48] = {
     0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    50, 10, 10, 10, 10, 30, 30, 40, 40, 30, 30, 20, 20, 20, 20, 20,
    50, 10, 10, 10, 10, 30, 30, 40, 40, 30, 30, 20, 20, 20, 20, 20
};

// 棋子走法
const int32_t KING_DELTA[4] = {-16, -1, 1, 16};
const int32_t ADVISOR_DELTA[4] = {-17, -15, 15, 17};
const int32_t KNIGHT_DELTA[4][2] = {{-33, -31}, {-18, 14}, {-14, 18}, {31, 33}};
const int32_t KING_KNIGHT_DELTA[4][2] = {{-18, -33}, {-31, -14}, {31, 14}, {18, 33}}; // 用于判断是否被马将军 和仕关联

// 将FEN串中棋子标识转化为对应棋子类型 需toupper转化为大写
int32_t charToPt(char c) {
    switch (c) {
        case 'K':
            return PIECE_KING;
        case 'A':
            return PIECE_ADVISOR;
        case 'B':
            return PIECE_BISHOP;
        case 'N':
            return PIECE_KNIGHT;
        case 'R':
            return PIECE_ROOK;
        case 'C':
            return PIECE_CANNON;
        case 'P':
            return PIECE_PAWN;
        default:
            return PIECE_EMPTY;
    }
}

// 将棋子转化为 FEN 字符 返回大写字符
char ptToChar(int32_t pt) {
    switch (pt) {
        case PIECE_KING:
            return 'K';
        case PIECE_ADVISOR:
            return 'A';
        case PIECE_BISHOP:
            return 'B';
        case PIECE_KNIGHT:
            return 'N';
        case PIECE_ROOK:
            return 'R';
        case PIECE_CANNON:
            return 'C';
        case PIECE_PAWN:
            return 'P';
        default:
            return 0;
    }
}

int_fast32_t historyIndex(int32_t mv) {
    return ((((SIDE_TAG(pos.sidePly) - 16) >> 1) +
             PIECE_TYPE(pos.squares[SRC(mv)]))
            << 8) +
           DST(mv);
}

void setHistory(int32_t mv, int32_t depth) {
    historyTable[((((SIDE_TAG(pos.sidePly) - 16) >> 1) +
             PIECE_TYPE(pos.squares[SRC(mv)]))
            << 8) +
           DST(mv)] += depth * depth;
}

void setKillerTable(int32_t mv) {
    auto tablePtr = killerTable[pos.distance];
    if (tablePtr[0] != mv) {
        tablePtr[1] = tablePtr[0]; // newKillerMove -> 0, 0 -> 1
        tablePtr[0] = mv;
    }
}

std::string MOVE_TO_STR(int32_t mv) {
    int src = SRC(mv), dst = DST(mv);
    int preX = GET_X(src) - X_FROM, preY = GET_Y(src) - Y_FROM;
    int toX = GET_X(dst) - X_FROM, toY = GET_Y(dst) - Y_FROM;
    return {(char)('a' + preX), (char)('0' + preY), (char)('a' + toX),
            (char)('0' + toY)};
}

int32_t STR_TO_MOVE(std::string mvStr) {
    int32_t preY = mvStr[1] - '0', preX = mvStr[0] - 'a';
    int32_t toY = mvStr[3] - '0', toX = mvStr[2] - 'a';
    int32_t src = COORD_XY(preX + X_FROM, preY + Y_FROM);
    int32_t dst = COORD_XY(toX + X_FROM, toY + Y_FROM);
    return MOVE(src, dst);
}

// 初始化位行列
void Position::initBit() {
    memset(rookCapY, -1, sizeof(rookCapY));
    memset(cannonCapY, -1, sizeof(cannonCapY));
    memset(rookCapX, -1, sizeof(rookCapX));
    memset(cannonCapX, -1, sizeof(cannonCapX));

    // 初始化 bitMask，行Y的 bitMask 通过计算 X 获得
    for (int32_t sq = 0; sq < 256; sq++) {
        if (IN_BOARD(sq)) {
            bitMaskY[sq] = 1 << (GET_X(sq) - X_FROM);
            bitMaskX[sq] = 1 << (GET_Y(sq) - Y_FROM);
        } else {
            bitMaskY[sq] = 0;
            bitMaskX[sq] = 0;
        }
    }

    // 生成横向预置吃子数组
    for (int32_t i = 0; i < 9; i++) { // 枚举所在列
        for (int32_t st = 0; st < 512; st++){ // 枚举行状态
            for (int32_t j = i + 1, cnt = 0; j < 9; j++) { // 枚举右侧棋子
                if (st & (1 << j)) {
                    ++cnt;
                    if (cnt == 1) rookCapX[i][st][1] = j + X_FROM;
                    else if (cnt == 2) {
                        cannonCapX[i][st][1] = j + X_FROM;
                        break;
                    }
                }
            }

            for (int32_t j = i - 1, cnt = 0; j >= 0; j--) { // 枚举左侧棋子
                if (st & (1 << j)) {
                    ++cnt;
                    if (cnt == 1) rookCapX[i][st][0] = j + X_FROM;
                    else if (cnt == 2) {
                        cannonCapX[i][st][0] = j + X_FROM;
                        break;
                    }
                }
            }
        }
    }

    // 生成竖向预置吃子数组
    for (int32_t i = 0; i < 10; i++) { // 枚举所在行
        for (int32_t st = 0; st < 1024; st++) { // 枚举列状态
            for (int32_t j = i + 1, cnt = 0; j < 10; j++) { // 枚举上方(sq大)棋子
                if (st & (1 << j)) {
                    ++cnt;
                    if (cnt == 1) rookCapY[i][st][1] = j + Y_FROM;
                    else if (cnt == 2) {
                        cannonCapY[i][st][1] = j + Y_FROM;
                        break;
                    }
                }
            }

            for (int32_t j = i - 1, cnt = 0; j >= 0; j--) { // 枚举下方(sq小)棋子
                if (st & (1 << j)) {
                    ++cnt;
                    if (cnt == 1) rookCapY[i][st][0] = j + Y_FROM;
                    else if (cnt == 2) {
                        cannonCapY[i][st][0] = j + Y_FROM;
                        break;
                    }
                }
            }
        }
    }
}

// 返回 Rook 左右吃子的位置
int32_t Position::getRookCapX(int32_t src, bool tag) {
    int32_t x = rookCapX[GET_X(src) - X_FROM][this->stateY[GET_Y(src)]][tag];
    return x == -1 ? 0 : COORD_XY(x, GET_Y(src));
}

// 返回 Cannon 左右吃子的位置
int32_t Position::getCannonCapX(int32_t src, bool tag) {
    int32_t x = cannonCapX[GET_X(src) - X_FROM][this->stateY[GET_Y(src)]][tag];
    return x == -1 ? 0 : COORD_XY(x, GET_Y(src));
}

// 返回 Rook 上下吃子的位置
int32_t Position::getRookCapY(int32_t src, bool tag) {
    int32_t y = rookCapY[GET_Y(src) - Y_FROM][this->stateX[GET_X(src)]][tag];
    return y == -1 ? 0 : COORD_XY(GET_X(src), y);
}

// 返回 Cannon 上下吃子的位置
int32_t Position::getCannonCapY(int32_t src, bool tag) {
    int32_t y = cannonCapY[GET_Y(src) - Y_FROM][this->stateX[GET_X(src)]][tag];
    return y == -1 ? 0 : COORD_XY(GET_X(src), y);
}

// 初始化
void Position::clear() {
    memset(this->squares, 0, sizeof(this->squares));
    memset(this->pieces, 0, sizeof(this->pieces));
    memset(this->stateY, 0, sizeof(this->stateY));
    memset(this->stateX, 0, sizeof(this->stateX));
    this->sidePly = 0; // 默认红方 可通过 changeSide() 修改
    this->vlRed = this->vlBlack = 0;
    this->moveNum = 0, this->distance = 0;
    this->zobrist = &zob, this->zobrist->initZero();
}

// 将棋子 pc 添加进棋局中的 sq 位置
void Position::addPiece(int32_t sq, int32_t pc, bool del) {
    if (del) {
        (pc & 16) ? vlRed -= SQ_VALUE[PIECE_TYPE(pc)][SQ_FLIP(sq)]
                  : vlBlack -= SQ_VALUE[PIECE_TYPE(pc)][sq];  // 改变分值

        this->squares[sq] = 0, this->pieces[pc] = 0;
    }
    else {
        (pc & 16) ? vlRed += SQ_VALUE[PIECE_TYPE(pc)][SQ_FLIP(sq)]
                  : vlBlack += SQ_VALUE[PIECE_TYPE(pc)][sq];  // 改变分值

        this->squares[sq] = pc, this->pieces[pc] = sq;
    }

    if (pc) { // pc 不为 0 才修改
        this->stateY[GET_Y(sq)] ^= bitMaskY[sq];
        this->stateX[GET_X(sq)] ^= bitMaskX[sq];
    }
    
    this->zobrist->zobristUpdateMove(sq, pc);
}

// 交换走子方
void Position::changeSide() {
    this->sidePly = this->sidePly ^ 1;
    this->zobrist->zobristUpdateChangeSide();
}

// 根据 mvStr 字符串移动棋子
void Position::movePiece(std::string mvStr) {
#ifdef POS_DEBUG
    if (mvStr.size() != 4) {
        std::cout << "wrong move input!" << std::endl;
        std::cout.flush();
        return;
    }
#endif

    int32_t preY = mvStr[1] - '0', preX = mvStr[0] - 'a';
    int32_t toY = mvStr[3] - '0', toX = mvStr[2] - 'a';

#ifdef POS_DEBUG
    if (!(0 <= preY && preY <= 9 && 0 <= preX && preX <= 8)) {
        std::cout << "wrong move input!" << std::endl;
        std::cout.flush();
        return;
    }

    if (!(0 <= toY && toY <= 9 && 0 <= toX && toX <= 8)) {
        std::cout << "wrong move input!" << std::endl;
        std::cout.flush();
        return;
    }
#endif

    int32_t src = COORD_XY(preX + X_FROM, preY + Y_FROM);
    int32_t dst = COORD_XY(toX + X_FROM, toY + Y_FROM);

#ifdef POS_DEBUG
    if (!this->squares[src] ||
        (this->squares[dst] &&
         !((this->squares[src] ^ this->squares[dst]) & 16))) {
        std::cout << "wrong move input!" << std::endl;
        std::cout.flush();
        return;
    }
#endif
    int32_t pc = this->squares[src]; // 起点棋子
    this->addPiece(dst, this->squares[dst], DEL_PIECE); // 删除终点棋子
    this->addPiece(src, this->squares[src], DEL_PIECE); // 删除起点棋子
    this->addPiece(dst, pc); // 在终点添加起点棋子

#ifdef POS_DEBUG
    this->debug();
#endif
}

// 根据整型 mv 移动棋子；mv 见 MOVE() 函数
void Position::movePiece(int32_t mv) {
    int32_t src = SRC(mv), dst = DST(mv);
    int32_t pc = this->squares[src]; // 起点棋子 可能为 0
    this->addPiece(dst, this->squares[dst], DEL_PIECE); // 删除终点棋子
    this->addPiece(src, this->squares[src], DEL_PIECE); // 删除起点棋子
    this->addPiece(dst, pc); // 在终点添加起点棋子
}

// 撤销移动棋子
void Position::undoMovePiece(int32_t mv, int32_t cap) {
    this->movePiece(mv);
    this->addPiece(SRC(mv), cap);
}

// 执行走法
bool Position::makeMove(int32_t mv) {
    auto mvListPtr = this->moveList + this->moveNum; // 将走法加入到mvList
    mvListPtr->mv = mv, mvListPtr->cap = this->squares[DST(mv)];
    mvListPtr->key = this->zobrist->getCurKey();

    this->movePiece(mv); // 执行走法
    if (this->isChecked()) {
        this->undoMovePiece(MOVE(DST(mv), SRC(mv)), mvListPtr->cap);
        return false;
    }

    this->changeSide(); // 交换走子方

    mvListPtr->chk = this->isChecked();
    this->distance++;
    this->moveNum++;
    return true;
}

void Position::undoMakeMove() {
    this->distance--;
    this->moveNum--;

    this->changeSide(); // 交换走子方

    auto mvListPtr = this->moveList + this->moveNum; // 撤销走子
    this->undoMovePiece(MOVE(DST(mvListPtr->mv), SRC(mvListPtr->mv)), mvListPtr->cap); // 撤销走法
}

// 空着
void Position::makeNullMove() {
    auto mvListPtr = this->moveList + this->moveNum; // 将走法加入到mvList
    mvListPtr->mv = 0;
    mvListPtr->cap = 0;
    mvListPtr->key = this->zobrist->getCurKey();
    mvListPtr->chk = 0;

    this->changeSide(); // 交换走子方
    this->distance++;
    this->moveNum++;
}
// 空着
void Position::undoMakeNullMove() {
    this->distance--;
    this->moveNum--;
    this->changeSide(); // 交换走子方
}

// 当前局面的优势是否足以进行空步搜索
int32_t Position::nullOkay() {
    return (this->sidePly ? this->vlBlack : this->vlRed) > 400;
}
// 空步搜索得到的分值是否有效
int32_t Position::nullSafe() {
    return (this->sidePly ? this->vlBlack : this->vlRed) > 200;
}

int32_t Position::repValue(int32_t vl) {
    int32_t tmp = ((vl & 2) ? this->banValue() : 0) + ((vl & 4) ? -this->banValue() : 0);
    return tmp ? tmp : this->drawValue();
}
// 长将判负分值 与深度有关
int32_t Position::banValue() {
    return -BAN_VALUE + this->distance;
}
// 输棋分值 与深度有关
int32_t Position::mateValue() {
    return -MATE_VALUE + this->distance;
}

int32_t Position::drawValue() {
    return (this->distance & 1) ? DRAW_VALUE : -DRAW_VALUE;
}

// 判断是否被将军 是则返回 true
int32_t Position::isChecked() {
    int32_t sideTag = SIDE_TAG(this->sidePly);
    int32_t oppSideTag = OPP_SIDE_TAG(this->sidePly);
    int32_t src, dst, i, j, k;
    bool dirTag = false;
    
    // 1. 检查对将 通过位列实现
    src = this->pieces[sideTag + KING_FROM];
    dst = this->pieces[oppSideTag + KING_FROM];
    if (dst > src) dirTag = true; // dst 大 tag 为 true
    if (this->getRookCapY(src, dirTag) == dst) return true;
    
    // 2. 检查马
    src = this->pieces[sideTag + KING_FROM];
    for (j = 0; j < 4; j++) {
        dst = src + ADVISOR_DELTA[j]; // 马腿
        if (this->squares[dst] || !IN_BOARD(dst)) continue; // 越界 或者 马腿有棋子
        for (k = 0; k < 2; k++) {
            dst = src + KING_KNIGHT_DELTA[j][k];
            if (!IN_BOARD(dst) || (this->squares[dst] & sideTag)) continue; // 越界或己方棋子
            if (PIECE_TYPE(this->squares[dst]) == PIECE_KNIGHT) return true;
        }
    }

    // 3. 检查车 通过位行、列实现
    for (i = ROOK_FROM; i <= ROOK_TO; i++) {
        src = this->pieces[sideTag + KING_FROM];
        dst = this->pieces[oppSideTag + i]; // 敌方 ROOK 位置
        if (SAME_X(src, dst)) { // 同一列
            dirTag = (dst > src) ? true : false;
            if (dst == this->getRookCapY(src, dirTag)) return true;
        } else if (SAME_Y(src, dst)) { // 同一行
            dirTag = (dst > src) ? true : false;
            if (dst == this->getRookCapX(src, dirTag)) return true;
        }
    }

    // 4. 检查炮 通过位行、列实现
    for (i = CANNON_FROM; i <= CANNON_TO; i++) {
        src = this->pieces[sideTag + KING_FROM];
        dst = this->pieces[oppSideTag + i]; // 敌方 CANNON 位置
        if (SAME_X(src, dst)) { // 同一列
            dirTag = (dst > src) ? true : false;
            if (dst == this->getCannonCapY(src, dirTag)) return true;
        } else if (SAME_Y(src, dst)) { // 同一行
            dirTag = (dst > src) ? true : false;
            if (dst == this->getCannonCapX(src, dirTag)) return true;
        }
    }

    // 5. 检查兵
    src = this->pieces[sideTag + KING_FROM];
    dst = SQ_FORWARED(src, this->sidePly); // 向前走一步
    if (PIECE_TYPE(this->squares[dst]) == PIECE_PAWN &&
        !(this->squares[dst] & sideTag))
        return true;
    // 两侧的兵只不需要判断颜色
    if (PIECE_TYPE(this->squares[src - 1]) == PIECE_PAWN) return true;
    if (PIECE_TYPE(this->squares[src + 1]) == PIECE_PAWN) return true;
    return false;
}

int32_t Position::isLegalMove(int32_t mv) {
    int32_t src = SRC(mv); // 获取着法起点
    int32_t sideTag = SIDE_TAG(this->sidePly);

    // 起点不是己方棋子或无子
    if (!(this->squares[src] & sideTag)) return false;

    int32_t dst = DST(mv);
    // 终点是己方棋子
    if (this->squares[dst] & sideTag) return false;

    int32_t pin(0), dirTag(0), rCapDst(0), cCapDist(0);
    switch(PIECE_TYPE(this->squares[src])) { // 判断己方棋子类型
        case PIECE_KING: // 将（帅）
            return IN_FORT(dst) && KING_SPAN(src, dst);
        case PIECE_ADVISOR: // 仕（仕）
            return IN_FORT(dst) && ADVISOR_SPAN(src, dst);
        case PIECE_BISHOP: // 象（相）
            return SELF_SIDE(dst, this->sidePly) && BISHOP_SPAN(src, dst) &&
                   !this->squares[BISHOP_PIN(src, dst)];
        case PIECE_KNIGHT: // 马
            pin = KNIGHT_PIN(src, dst);
            return pin != src && !this->squares[pin];
        case PIECE_ROOK:
        case PIECE_CANNON: // 车和炮判断                                            一样
            if (SAME_X(src, dst)) { // 同一列
                dirTag = dst > src ? true : false;
                rCapDst = this->getRookCapY(src, dirTag);
                if (PIECE_TYPE(this->squares[src]) == PIECE_ROOK) {
                    if (rCapDst == dst) return true; // 吃子走法
                    else if (dirTag && dst < rCapDst) return true;
                    else if (!dirTag && dst > rCapDst) return true;
                    else return false;
                } else {
                    cCapDist = this->getCannonCapY(src, dirTag);
                    if (cCapDist == dst) return true; // 吃子走法
                    else if (dirTag && dst < rCapDst) return true;
                    else if (!dirTag && dst > rCapDst) return true;
                    else return false;
                }
            } else if (SAME_Y(src, dst)) {
                dirTag = dst > src ? true : false;
                rCapDst = this->getRookCapX(src, dirTag);
                if (PIECE_TYPE(this->squares[src]) == PIECE_ROOK) {
                    if (rCapDst == dst) return true; // 吃子走法
                    else if (dirTag && dst < rCapDst) return true;
                    else if (!dirTag && dst > rCapDst) return true;
                    else return false;
                } else {
                    cCapDist = this->getCannonCapX(src, dirTag);
                    if (cCapDist == dst) return true; // 吃子走法
                    else if (dirTag && dst < rCapDst) return true;
                    else if (!dirTag && dst > rCapDst) return true;
                    else return false;
                }
            } else return false; // 不在同一列同一行 不合法

        case PIECE_PAWN:
            // 已过河，可以向左右两个方向走
            if (!SELF_SIDE(src, this->sidePly) && (dst - src == 1 || src - dst == 1))
                return true;
            // 否则只能向前走
            return dst == SQ_FORWARED(src, this->sidePly);
        default: return false;
    }
    assert(false); // shouldn't reach here
    return false;
}

int32_t Position::repStatus(int32_t repCount) {
    bool selfSide = false;
    bool perpetualCheck = true;
    bool oppPerpetualCheck = true;
    int32_t i = this->moveNum - 1;
    while (i >= 0 && this->moveList[i].mv && !this->moveList[i].cap) {
        if (selfSide) {
            perpetualCheck &= this->moveList[i].chk;
            if (this->moveList[i].key == this->zobrist->getCurKey()) {
                repCount--;
                if (repCount == 0) {
                    return 1 + (perpetualCheck ? 2 : 0) +
                           (oppPerpetualCheck ? 4 : 0);
                }
            }
        } else {
            oppPerpetualCheck &= this->moveList[i].chk;
        }
        selfSide ^= true;
        i--;
    }
    return 0;
}

// 通过FEN串初始化棋局
void Position::fromFen(const char* fen) {
    int32_t pcRed[PIECE_EMPTY], pcBlack[PIECE_EMPTY];
    pcRed[0] = SIDE_TAG(0) + KING_FROM;
    pcRed[1] = SIDE_TAG(0) + ADVISOR_FROM;
    pcRed[2] = SIDE_TAG(0) + BISHOP_FROM;
    pcRed[3] = SIDE_TAG(0) + KNIGHT_FROM;
    pcRed[4] = SIDE_TAG(0) + ROOK_FROM;
    pcRed[5] = SIDE_TAG(0) + CANNON_FROM;
    pcRed[6] = SIDE_TAG(0) + PAWN_FROM;
    for (int32_t i = 0; i < PIECE_EMPTY; i++) pcBlack[i] = pcRed[i] + 16;
    this->clear();

    const char* lpFen;

    lpFen = fen;
    if (*lpFen == '\0') {
        // SetIrrev();
        return;
    }
    // 2. 读取棋盘上的棋子
    int r = 9;  //行
    int c = 0;  // 列

    while (*lpFen != ' ') {
        if (*lpFen == '/') {
            c = 0;
            r--;
            if (r < 0) {
                break;
            }
        } else if (*lpFen >= '1' && *lpFen <= '9') {
            for (int k = 0; k < (*lpFen - '0'); k++) {
                if (c > 8) {
                    break;
                }
                c++;
            }
        } else if (*lpFen >= 'A' && *lpFen <= 'Z') {
            if (c <= 8) {
                int32_t pcType = charToPt(*lpFen);

                if (pcType < 7) {
                    int32_t x = c + X_FROM;
                    int32_t y = r + Y_FROM;
                    this->addPiece(COORD_XY(x, y), pcRed[pcType]);
                    pcRed[pcType]++;
                }
                c++;
            }
        } else if (*lpFen >= 'a' && *lpFen <= 'z') {
            if (c <= 8) {
                int32_t pcType = charToPt(*lpFen + 'A' - 'a');
                if (pcType < 7) {
                    int32_t x = c + X_FROM;
                    int32_t y = r + Y_FROM;
                    this->addPiece(COORD_XY(x, y), pcBlack[pcType]);
                    pcBlack[pcType]++;
                }
                c++;
            }
        }
        lpFen++;
        if (*lpFen == '\0') {
            // SetIrrev();
            return;
        }
    }
    lpFen++;
    // 3. 确定轮到哪方走
    if (*lpFen == 'b') {
        this->changeSide();
    }
}

/* 通过棋盘字符串初始化
 * debug 初始化 大写红方
  a b c d e f g h i   
9|r n b a k a b n r |9
8|* * * * * * * * * |8
7|* c * * * * * c * |7
6|p * p * p * p * p |6
5|* * * * * * * * * |5
4|* * * * * * * * C |4
3|P * P * P * P * P |3
2|* C * * * * * * * |2
1|* * * * * * * * * |1
0|R N B A K A B N R |0
  a b c d e f g h i
 */
#ifdef POS_DEBUG // 仅 debug 模式下使用
void Position::fromStringMap(std::string* s, int32_t side) { // 9 - 0 行的字符串
    // pcRed 和 pcBlack 分别代表红方和黑方每个兵种即将占有的序号
    int32_t pcRed[PIECE_EMPTY], pcBlack[PIECE_EMPTY];
    pcRed[0] = SIDE_TAG(0) + KING_FROM;
    pcRed[1] = SIDE_TAG(0) + ADVISOR_FROM;
    pcRed[2] = SIDE_TAG(0) + BISHOP_FROM;
    pcRed[3] = SIDE_TAG(0) + KNIGHT_FROM;
    pcRed[4] = SIDE_TAG(0) + ROOK_FROM;
    pcRed[5] = SIDE_TAG(0) + CANNON_FROM;
    pcRed[6] = SIDE_TAG(0) + PAWN_FROM;
    for (int32_t i = 0; i < PIECE_EMPTY; i++)
        pcBlack[i] = pcRed[i] + 16;
    // 清空棋盘
    this->clear();

    // 读取棋子
    for (int32_t j = 9, x = 0, y = 9, pcType; ~j; j--) {
        for (int32_t i = 2; i + 2 < (int32_t)s[j].size(); i += 2) {
            x = i / 2 - 1 + X_FROM;
            y = j + Y_FROM;
            pcType = charToPt(toupper(s[j][i])); // 获得棋子类型
            if (pcType == PIECE_EMPTY) continue;
            if (isupper(s[j][i])) {
                if (pcRed[pcType] < 32) {
                    if (this->pieces[pcRed[pcType]] == 0) {
                        this->addPiece(COORD_XY(x, y), pcRed[pcType]);
                        pcRed[pcType]++;
                    }
                }
            } else {
                if (pcBlack[pcType] < 48) {
                    if (this->pieces[pcBlack[pcType]] == 0) {
                        this->addPiece(COORD_XY(x, y), pcBlack[pcType]);
                        pcBlack[pcType]++;
                    }
                }
            }
        }
    }
    if (side) this->changeSide();
}

void Position::debug() { // 仅 debug 模式下使用
    std::cout << "side: " << this->sidePly << std::endl;
    std::cout << "  a b c d e f g h i" << std::endl;
    for (int32_t y = Y_TO; y >= Y_FROM; y--) { // 行
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY);
        std::cout << y - Y_FROM << "|";
        for (int32_t x = X_FROM; x <= X_TO; x++) { // 列
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY);
            auto pc = this->squares[COORD_XY(x, y)]; // (x, y) 处棋子
            if (pc == 0) std::cout << "*" << " ";
            else if (pc < 32) { // 红
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);
                std::cout << ptToChar(PIECE_TYPE(pc)) << " ";
            } else {
                std::cout << (char)tolower(ptToChar(PIECE_TYPE(pc))) << " ";
            }
        }
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY);
        std::cout << "|" << y - Y_FROM << std::endl;
    }
    std::cout << "  a b c d e f g h i" << std::endl;
    std::cout.flush() << std::endl;
}

#endif