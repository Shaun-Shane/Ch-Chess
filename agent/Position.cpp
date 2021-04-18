#ifndef POSITION_CPP
#define POSITION_CPP

#include "Position.h"

uint_fast8_t charToPiece(char c) {
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

// 根据 x, y 返回序号 0 ~ 255
uint_fast8_t Position::coordXY(int_fast8_t x, int_fast8_t y) {
    return x + (y << 4);
}

// 根据 sq 0 ~ 255 返回行数 y
int_fast8_t Position::getY(uint_fast8_t sq) {
    return sq >> 4;
}

int_fast8_t Position::getX(uint_fast8_t sq) {
    return sq >> 4;
}

// 根据 sq 0 ~ 255 返回列数 x
int_fast8_t Position::getX(uint_fast8_t sq) {
    return sq & 15;
}

// 判断某位置是否在棋盘
bool Position::isInBoard(uint_fast8_t sq) {
    return IN_BOARD[sq];
}

// 初始化棋局数组
void Position::clear() {
    for (uint_fast8_t sq = 0; sq < 256; sq++)
        this->squares[sq] = 0;
}

// 将棋子 pc 添加进棋局中的 sq 位置
void Position::addPiece(uint_fast8_t sq, uint_fast8_t pc) {
    this->squares[sq] = pc;
}

// 通过FEN串初始化棋局
void Position::fromFen(string fen) {
    this->clear();
}



#endif