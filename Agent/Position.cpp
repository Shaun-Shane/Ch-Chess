#ifndef POSITION_CPP
#define POSITION_CPP

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

int_fast64_t historyTable[1 << 12] = {0};

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



/* 棋子类型-位置价值表
 * 获取棋子类型见 PIECE_TYPE(pc) 函数
 * 注意黑方在下 初始 sq 大
 */ 
const int32_t SQ_VALUE[PIECE_EMPTY + 1][256] = {
    { // 将
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
        0,  0,  0,  0,  0,  0,  1,  1,  1,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  2,  2,  2,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0, 11, 15, 11,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
    }, {    // 仕
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
        0,  0,  0,  0,  0,  0, 20,  0, 20,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0, 23,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0, 20,  0, 20,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
    }, { // 相
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0, 20,  0,  0,  0, 20,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0, 18,  0,  0,  0, 23,  0,  0,  0, 18,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0, 20,  0,  0,  0, 20,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
    }, { // 马
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0, 90, 90, 90, 96, 90, 96, 90, 90, 90,  0,  0,  0,  0,
        0,  0,  0, 90, 96,103, 97, 94, 97,103, 96, 90,  0,  0,  0,  0,
        0,  0,  0, 92, 98, 99,103, 99,103, 99, 98, 92,  0,  0,  0,  0,
        0,  0,  0, 93,108,100,107,100,107,100,108, 93,  0,  0,  0,  0,
        0,  0,  0, 90,100, 99,103,104,103, 99,100, 90,  0,  0,  0,  0,
        0,  0,  0, 90, 98,101,102,103,102,101, 98, 90,  0,  0,  0,  0,
        0,  0,  0, 92, 94, 98, 95, 98, 95, 98, 94, 92,  0,  0,  0,  0,
        0,  0,  0, 93, 92, 94, 95, 92, 95, 94, 92, 93,  0,  0,  0,  0,
        0,  0,  0, 85, 90, 92, 93, 78, 93, 92, 90, 85,  0,  0,  0,  0,
        0,  0,  0, 88, 85, 90, 88, 90, 88, 90, 85, 88,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
    }, { // 车
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,206,208,207,213,214,213,207,208,206,  0,  0,  0,  0,
        0,  0,  0,206,212,209,216,233,216,209,212,206,  0,  0,  0,  0,
        0,  0,  0,206,208,207,214,216,214,207,208,206,  0,  0,  0,  0,
        0,  0,  0,206,213,213,216,216,216,213,213,206,  0,  0,  0,  0,
        0,  0,  0,208,211,211,214,215,214,211,211,208,  0,  0,  0,  0,
        0,  0,  0,208,212,212,214,215,214,212,212,208,  0,  0,  0,  0,
        0,  0,  0,204,209,204,212,214,212,204,209,204,  0,  0,  0,  0,
        0,  0,  0,198,208,204,212,212,212,204,208,198,  0,  0,  0,  0,
        0,  0,  0,200,208,206,212,200,212,206,208,200,  0,  0,  0,  0,
        0,  0,  0,194,206,204,212,200,212,204,206,194,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
    }, { // 炮
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,100,100, 96, 91, 90, 91, 96,100,100,  0,  0,  0,  0,
        0,  0,  0, 98, 98, 96, 92, 89, 92, 96, 98, 98,  0,  0,  0,  0,
        0,  0,  0, 97, 97, 96, 91, 92, 91, 96, 97, 97,  0,  0,  0,  0,
        0,  0,  0, 96, 99, 99, 98,100, 98, 99, 99, 96,  0,  0,  0,  0,
        0,  0,  0, 96, 96, 96, 96,100, 96, 96, 96, 96,  0,  0,  0,  0,
        0,  0,  0, 95, 96, 99, 96,100, 96, 99, 96, 95,  0,  0,  0,  0,
        0,  0,  0, 96, 96, 96, 96, 96, 96, 96, 96, 96,  0,  0,  0,  0,
        0,  0,  0, 97, 96,100, 99,101, 99,100, 96, 97,  0,  0,  0,  0,
        0,  0,  0, 96, 97, 98, 98, 98, 98, 98, 97, 96,  0,  0,  0,  0,
        0,  0,  0, 96, 96, 97, 99, 99, 99, 97, 96, 96,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
    }, { // 兵
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  9,  9,  9, 11, 13, 11,  9,  9,  9,  0,  0,  0,  0,
        0,  0,  0, 19, 24, 34, 42, 44, 42, 34, 24, 19,  0,  0,  0,  0,
        0,  0,  0, 19, 24, 32, 37, 37, 37, 32, 24, 19,  0,  0,  0,  0,
        0,  0,  0, 19, 23, 27, 29, 30, 29, 27, 23, 19,  0,  0,  0,  0,
        0,  0,  0, 14, 18, 20, 27, 29, 27, 20, 18, 14,  0,  0,  0,  0,
        0,  0,  0,  7,  0, 13,  0, 16,  0, 13,  0,  7,  0,  0,  0,  0,
        0,  0,  0,  7,  0,  7,  0, 15,  0,  7,  0,  7,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
    }, { // 空
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
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
    }
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

std::string MOVE_TO_STR(int32_t mv) {
    int src = SRC(mv), dst = DST(mv);
    int preX = GET_X(src) - X_FROM, preY = GET_Y(src) - Y_FROM;
    int toX = GET_X(dst) - X_FROM, toY = GET_Y(dst) - Y_FROM;
    return {(char)('a' + preX), (char)('0' + preY), (char)('a' + toX),
            (char)('0' + toY)};
}

// 初始化
void Position::clear() {
    memset(this->squares, 0, sizeof(this->squares));
    memset(this->pieces, 0, sizeof(this->pieces));
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
    // this->zobr.Xor(PreGen.zobrTable[pt][sq]);
    this->zobrist->zobristUpdateMove(sq, pc);
}

// 交换走子方
void Position::changeSide() { this->sidePly = this->sidePly ^ 1;this->zobrist->zobristUpdateChangeSide(); }

// 保存状态
void Position::saveStatus() {
    auto rbObjPtr = this->rollBackList + this->moveNum;
    rbObjPtr->vlRed = this->vlRed;
    rbObjPtr->vlBlack = this->vlBlack;
}

void Position::rollBack() {
    auto rbObjPtr = this->rollBackList + this->moveNum;
    this->vlRed = rbObjPtr->vlRed;
    this->vlBlack = rbObjPtr->vlBlack;
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

// 得到下一个走法，无走法返回 0
int32_t Position::nextMove() {
    if (this->curMvCnt[this->distance] + 1 >= this->genNum[this->distance])
        return 0; // 达到最大着法数 返回 false 结束循环
    // 判断合法走法 ...

    this->curMvCnt[this->distance]++; // 增加为下一个走法的下标
    return (this->mvsGen[this->distance] + this->curMvCnt[this->distance])->mv;
}

// 执行走法
bool Position::makeMove() {
    auto mvPtr = this->mvsGen[this->distance] + this->curMvCnt[this->distance];
    this->movePiece(mvPtr->mv); // 执行走法
    if (this->isChecked()) {
        this->undoMovePiece(MOVE(DST(mvPtr->mv), SRC(mvPtr->mv)), mvPtr->cap);
        return false;
    }

    this->saveStatus();
    this->changeSide(); // 交换走子方

    this->distance++;
    this->moveNum++;
    return true;
}

void Position::undoMakeMove() {
    this->distance--;
    this->moveNum--;

    this->changeSide(); // 交换走子方
    this->rollBack();

    auto mvPtr = this->mvsGen[this->distance] + this->curMvCnt[this->distance];
    this->undoMovePiece(MOVE(DST(mvPtr->mv), SRC(mvPtr->mv)), mvPtr->cap); // 撤销走法
}

// 空着
void Position::makeNullMove() {
    this->saveStatus();
    this->changeSide(); // 交换走子方
    this->distance++;
    this->moveNum++;
}
// 空着
void Position::undoMakeNullMove() {
    this->distance--;
    this->moveNum--;
    this->changeSide(); // 交换走子方
    this->rollBack();
}

// 当前局面的优势是否足以进行空步搜索
int32_t Position::nullOkay() {
    return (this->sidePly ? this->vlBlack : this->vlRed) > 400;
}
// 空步搜索得到的分值是否有效
int32_t Position::nullSafe() {
    return (this->sidePly ? this->vlBlack : this->vlRed) > 200;
}

// 输棋分值 与深度有关
int32_t Position::mateValue() {
    return -MATE_VALUE + this->distance;
}

#define CHECK_PIECE()                   \
    {                                 \
        src = src + delta;            \
        while (src != dst) {          \
            if (this->squares[src]) { \
                checked = false;      \
                break;                \
            }                         \
            src += delta;             \
        }                             \
    }
// 判断是否被将军 是则返回 true
int32_t Position::isChecked() {
    int32_t sideTag = SIDE_TAG(this->sidePly);
    int32_t oppSideTag = OPP_SIDE_TAG(this->sidePly);
    int32_t src, dst, delta, checked, i, j, k;
    
    // 1. 检查对将
    src = this->pieces[sideTag + KING_FROM];
    dst = this->pieces[oppSideTag + KING_FROM];
    checked = true;
    if (SAME_X(src, dst)) { // 相同列
        delta = (dst > src) ? 16 : -16;
        CHECK_PIECE();
        if (checked) return true;
    }
    
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

    // 3. 检查车
    for (i = ROOK_FROM; i <= ROOK_TO; i++) {
        src = this->pieces[sideTag + KING_FROM];
        dst = this->pieces[oppSideTag + i]; // 敌方 ROOK 位置
        checked = true;
        if (SAME_X(src, dst)) { // 同一列
            delta = (dst > src) ? 16 : -16;
            CHECK_PIECE();
            if (checked) return true;
        } else if (SAME_Y(src, dst)) { // 同一行
            delta = (dst > src)? 1 : -1;
            CHECK_PIECE();
            if (checked) return true;
        }
    }

    // 4. 检查炮
    for (i = CANNON_FROM; i <= CANNON_TO; i++) {
        src = this->pieces[sideTag + KING_FROM];
        dst = this->pieces[oppSideTag + i]; // 敌方 CANNON 位置
        checked = true;
        if (SAME_X(src, dst)) { // 同一列
            delta = (dst > src) ? 16 : -16;
            CHECK_PIECE();
            if (!checked) { // 将与炮中间有子
                checked = true;
                CHECK_PIECE(); // 判断子与炮之间是否还有子，若有 checked = false
                if (checked) return true;
            }
        } else if (SAME_Y(src, dst)) { // 同一行
            delta = (dst > src)? 1 : -1;
            CHECK_PIECE();
            if (!checked) { // 将与炮中间有子
                checked = true;
                CHECK_PIECE(); // 判断子与炮之间是否还有子，若有 checked = false
                if (checked) return true;
            }
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

    int32_t pin(0), delta(0);
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
            if (SAME_X(src, dst)) delta = (dst > src) ? 16 : -16; // 同一列
            else if (SAME_Y(src, dst)) delta = (dst > src) ? 1 : -1; // 同一行
            else return false; // 不在同一列同一行 不合法

            pin = src + delta; // 向前走一步 再继续直到遇到棋子
            while (pin != dst && !this->squares[pin]) pin += delta;

            if (pin == dst)  // 起点与终点间没有棋子
                return !this->squares[dst] || PIECE_TYPE(this->squares[src]) == PIECE_ROOK;

            // 此时只可能是炮且跳吃
            if (!this->squares[dst] || PIECE_TYPE(this->squares[src]) != PIECE_CANNON)
                return false;

            pin += delta; // 向前走一步
            while (pin != dst && !this->squares[pin]) pin += delta;
            return pin == dst; // 判断中间是否还有子

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

#endif