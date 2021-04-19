#ifndef POSITION_H
#define POSITION_H

#include <bits/stdc++.h>

using namespace std;

// 棋子编号
constexpr uint_fast8_t PIECE_KING = 0;    // 将
constexpr uint_fast8_t PIECE_ADVISOR = 1; // 士
constexpr uint_fast8_t PIECE_BISHOP = 2;  // 象
constexpr uint_fast8_t PIECE_KNIGHT = 3;  // 马
constexpr uint_fast8_t PIECE_ROOK = 4;    // 车
constexpr uint_fast8_t PIECE_CANNON = 5;  // 炮
constexpr uint_fast8_t PIECE_PAWN = 6;    // 卒
constexpr uint_fast8_t PIECE_EMPTY = 7;    // 空

// 每种棋子的开始序号和结束序号 0 ~ 15
constexpr uint_fast8_t KING_FROM = 0;    // 将
constexpr uint_fast8_t ADVISOR_FROM = 1; // 士
constexpr uint_fast8_t ADVISOR_TO = 2;   // 士
constexpr uint_fast8_t BISHOP_FROM = 3;  // 象
constexpr uint_fast8_t BISHOP_TO = 4;    // 象
constexpr uint_fast8_t KNIGHT_FROM = 5;  // 马
constexpr uint_fast8_t KNIGHT_TO = 6;    // 马
constexpr uint_fast8_t ROOK_FROM = 7;    // 车
constexpr uint_fast8_t ROOK_TO = 8;      // 车
constexpr uint_fast8_t CANNON_FROM = 9;  // 炮
constexpr uint_fast8_t CANNON_TO = 10;   // 炮
constexpr uint_fast8_t PAWN_FROM = 11;   // 卒
constexpr uint_fast8_t PAWN_TO = 15;     // 卒

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

/* 棋子序号对应的棋子类型
 *
 * 棋子序号从0到47，其中0到15不用，16到31表示红子，32到47表示黑子。
 * 帅仕仕相相马马车车炮炮兵兵兵兵兵(将士士象象马马车车炮炮卒卒卒卒卒)
 * 判断棋子是红子 "pc < 32"，黑子 "pc >= 32"
 */
constexpr uint_fast8_t pieceTypes[48] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6, 6, 6, 6,
    0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6, 6, 6, 6
};

// 棋子的简单分值，只在简单比较时作参考
constexpr int_fast8_t simpleValues[48] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    5, 1, 1, 1, 1, 3, 3, 4, 4, 3, 3, 2, 2, 2, 2, 2,
    5, 1, 1, 1, 1, 3, 3, 4, 4, 3, 3, 2, 2, 2, 2, 2,
};

/* SIDE_TAG()，红方设为 16，黑方设为 32。
 * 用 "SIDE_TAG() + i" 选择棋子的类型， "i" 从 0 到 15 依次是：
 * 帅仕仕相相马马车车炮炮兵兵兵兵兵(将士士象象马马车车炮炮卒卒卒卒卒)
 * 例如"i"取"KNIGHT_FROM"到"KNIGHT_TO"，则表示依次检查两个马的位置
 */
inline uint_fast8_t SIDE_TAG(uint_fast8_t side) {
    return 16 + (side << 4);
}

inline uint_fast8_t OPP_SIDE_TAG(uint_fast8_t side) {
  return 32 - (side << 4);
}

// 将FEN串中棋子标识转化为对应棋子类型 需toupper转化为大写
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

    // 将棋子 pc 添加进棋局中的 sq 位置 del = true 则删除棋子
    void addPiece(uint_fast8_t sq, uint_fast8_t pc, bool del = false);

    // 通过FEN串初始化棋局
    void fromFen(string fen);

    // 通过棋盘字符串初始化
    void fromStringMap(string* s);

    uint_fast8_t squares[256]; // 每个格子放的棋子，0 为无子
    uint_fast8_t pieces[48]; // 每个棋子放的位置，0 为棋子不存在
    int_fast8_t dist = 2; // dist <= 0 -> eval
} pos;

#endif