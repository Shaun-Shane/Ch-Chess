#ifndef POSITION_H
#define POSITION_H

#include <bits/stdc++.h>

using namespace std;

/*
* 红方棋子 [8 - 15] & 1 ！= 0
* 黑方棋子 [16 - 22] & 1 = 0

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

// 棋子编号
constexpr uint_fast8_t PIECE_KING = 0;    // 将
constexpr uint_fast8_t PIECE_ADVISOR = 1; // 士
constexpr uint_fast8_t PIECE_BISHOP = 2;  // 象
constexpr uint_fast8_t PIECE_KNIGHT = 3;  // 马
constexpr uint_fast8_t PIECE_ROOK = 4;    // 车
constexpr uint_fast8_t PIECE_CANNON = 5;  // 炮
constexpr uint_fast8_t PIECE_PAWN = 6;    // 卒
constexpr uint_fast8_t PIECE_EMPTY = 7;    // 空

// 棋盘范围
constexpr uint_fast8_t RANK_TOP = 3;
constexpr uint_fast8_t RANK_BOTTOM = 12;
constexpr uint_fast8_t RANK_LEFT = 3;
constexpr uint_fast8_t RANK_RIGHT = 11;

// 用于判断棋子是否在棋盘上
constexpr uint_fast8_t IN_BOARD[256] = {
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

uint_fast8_t charToPiece(char c);

struct Position {
    // 根据 x, y 返回序号 0 ~ 255
    uint_fast8_t coordXY(int_fast8_t x, int_fast8_t y);

    // 根据 sq 0 ~ 255 返回行数 y
    int_fast8_t getY(uint_fast8_t sq);

    // 根据 sq 0 ~ 255 返回列数 x
    int_fast8_t getX(uint_fast8_t sq);

    // 判断某位置是否在棋盘
    bool isInBoard(uint_fast8_t sq);

    // 初始化棋局数组
    void clear();

    // 将棋子 pc 添加进棋局中的 sq 位置
    void addPiece(uint_fast8_t sq, uint_fast8_t pc);

    // 通过FEN串初始化棋局
    void fromFen(string fen);

    uint_fast8_t squares[256];
} position;

#endif