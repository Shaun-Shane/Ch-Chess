#ifndef POSITION_CPP
#define POSITION_CPP

#include "Position.h"

// 将FEN串中棋子标识转化为对应棋子类型 需toupper转化为大写
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

// 根据 sq 0 ~ 255 返回列数 x
int_fast8_t Position::getX(uint_fast8_t sq) {
    return sq & 15;
}

// 判断某位置是否在棋盘
bool Position::isInBoard(uint_fast8_t sq) {
    return IN_BOARD[sq];
}

// 初始化
void Position::clear() {
    memset(this->squares, 0, sizeof(this->squares));
    memset(this->pieces, 0, sizeof(this->pieces));
}

// 将棋子 pc 添加进棋局中的 sq 位置
void Position::addPiece(uint_fast8_t sq, uint_fast8_t pc, bool del) {
    if (del) this->squares[sq] = 0, this->pieces[pc] = 0;
    else this->squares[sq] = pc, this->pieces[pc] = sq;
    // this->zobr.Xor(PreGen.zobrTable[pt][sq]);
}

// 通过FEN串初始化棋局
void Position::fromFen(string fen) {
    this->clear();
    // ...
}

/* 通过棋盘字符串初始化
 * debug 初始化
  a b c d e f g h i   
9|R N B A K A B N R |9
8|* * * * * * * * * |8
7|* C * * * * * C * |7
6|P * P * P * P * P |6
5|* * * * * * * * * |5
4|* * * * * * * * * |4
3|p * p * p * p * p |3
2|* c * * * * * c * |2
1|* * * * * * * * * |1
0|r n b a k a b n r |0
  a b c d e f g h i
 */
void Position::fromStringMap(string* s) { // 9 - 0 行的字符串
    // pcRed 和 pcBlack 分别代表红方和黑方每个兵种即将占有的序号
    uint_fast8_t pcRed[PIECE_EMPTY], pcBlack[PIECE_EMPTY];
    pcRed[0] = SIDE_TAG(0) + KING_FROM;
    pcRed[1] = SIDE_TAG(0) + ADVISOR_FROM;
    pcRed[2] = SIDE_TAG(0) + BISHOP_FROM;
    pcRed[3] = SIDE_TAG(0) + KNIGHT_FROM;
    pcRed[4] = SIDE_TAG(0) + ROOK_FROM;
    pcRed[5] = SIDE_TAG(0) + CANNON_FROM;
    pcRed[6] = SIDE_TAG(0) + PAWN_FROM;
    for (int_fast8_t i = 0; i < 7; i++)
        pcBlack[i] = pcRed[i] + 16;
    // 清空棋盘
    this->clear();

    // 读取棋子
    for (int_fast8_t y = 9, x = 0, pcType; ~y; y--) {
        for (int_fast8_t i = 2; i < s[y].size(); i++) {
            x = i - 2;
            pcType = charToPiece(toupper(s[y][i]));

        }
    }
}



#endif