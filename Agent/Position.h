#ifndef POSITION_H
#define POSITION_H

#include <iostream>
#include <cctype>
#include <cstring>

#define POS_DEBUG

// 棋子编号
constexpr int_fast16_t PIECE_KING = 0;    // 将
constexpr int_fast16_t PIECE_ADVISOR = 1; // 士
constexpr int_fast16_t PIECE_BISHOP = 2;  // 象
constexpr int_fast16_t PIECE_KNIGHT = 3;  // 马
constexpr int_fast16_t PIECE_ROOK = 4;    // 车
constexpr int_fast16_t PIECE_CANNON = 5;  // 炮
constexpr int_fast16_t PIECE_PAWN = 6;    // 卒
constexpr int_fast16_t PIECE_EMPTY = 7;    // 空

// 每种棋子的开始序号和结束序号 0 ~ 15
constexpr int_fast16_t KING_FROM = 0;    // 将
constexpr int_fast16_t ADVISOR_FROM = 1; // 士
constexpr int_fast16_t ADVISOR_TO = 2;   // 士
constexpr int_fast16_t BISHOP_FROM = 3;  // 象
constexpr int_fast16_t BISHOP_TO = 4;    // 象
constexpr int_fast16_t KNIGHT_FROM = 5;  // 马
constexpr int_fast16_t KNIGHT_TO = 6;    // 马
constexpr int_fast16_t ROOK_FROM = 7;    // 车
constexpr int_fast16_t ROOK_TO = 8;      // 车
constexpr int_fast16_t CANNON_FROM = 9;  // 炮
constexpr int_fast16_t CANNON_TO = 10;   // 炮
constexpr int_fast16_t PAWN_FROM = 11;   // 卒
constexpr int_fast16_t PAWN_TO = 15;     // 卒

constexpr bool DEL_PIECE = true;   // 添加棋子

constexpr int_fast16_t MATE_VALUE = 1e4; // 将军
constexpr int_fast16_t WIN_VALUE = MATE_VALUE - 120; // 赢棋分值 高于 WIN_VALUE 都是赢棋

constexpr int_fast16_t MAX_LIST_SIZE = 1024;  // 最大回滚着法数
constexpr int_fast16_t MAX_GER_NUM = 128; // 最多可能的着法数 不会超过 128
constexpr int_fast16_t MAX_DISTANCE = 128; // 最多搜索层数

// 棋盘范围
constexpr int_fast16_t Y_FROM = 3;
constexpr int_fast16_t Y_TO = 12;
constexpr int_fast16_t X_FROM = 3;
constexpr int_fast16_t X_TO = 11;

// 用于判断棋子是否在棋盘上
extern const int_fast16_t _IN_BOARD[256];
// 用于判断棋子是否在九宫内
extern const int_fast16_t _IN_FORT[256];

// 根据 x, y 返回序号 0 ~ 255
inline int_fast16_t COORD_XY(int_fast16_t x, int_fast16_t y) {
    return x + (y << 4);
}

// 根据 sq 0 ~ 255 返回列数 y 0 ~ 16
inline int_fast16_t GET_Y(int_fast16_t sq) {
    return sq >> 4;
}

// 根据 sq 0 ~ 255 返回列数 y 0 ~ 16
inline int_fast16_t GET_X(int_fast16_t sq) {
    return sq & 15;
}

// 判断某位置是否在棋盘
inline bool IN_BOARD(int_fast16_t sq) {
    return _IN_BOARD[sq];
}

// 判断某位置是否在九宫
inline bool IN_FORT(int_fast16_t sq) {
    return _IN_FORT[sq];
}

// 根据位置 sq 判断是否过河，未过河返回 true；否则返回 false
inline bool SELF_SIDE(int_fast16_t sq, int_fast16_t side) {
    return (sq & 0x80) == (side << 7); // 设定红方 sq 小
}

// 返回向前走一步后的位置 sq
inline int SQ_FORWARED(int_fast16_t sq, int_fast16_t side) {
    return sq + 16 - (side << 5);
}

// 镜像后的位置 注意一开始红方左下角的 sq 为 51，黑方右上角 sq 为 203
inline int SQ_FLIP(int sq) {
    return 254 - sq;
}

// 如果两位置在同一行，返回 true
inline bool SAME_Y(int src, int dst) {
    return ((src ^ dst) & 0xf0) == 0;
}

// 如果两位置在同一列，返回 true
inline bool SAME_X(int src, int dst) {
    return ((src ^ dst) & 0x0f) == 0;
}

/* 棋子序号对应的棋子类型
 *
 * 棋子序号从0到47，其中0到15不用，16到31表示红子，32到47表示黑子。
 * 帅仕仕相相马马车车炮炮兵兵兵兵兵(将士士象象马马车车炮炮卒卒卒卒卒)
 * 判断棋子是红子 "pc < 32"，黑子 "pc >= 32"
 */
extern const int_fast16_t pieceTypes[48];

// 棋子的简单分值，只在简单比较时作参考
extern const int_fast16_t simpleValues[48];

// 获得棋子类型
inline int_fast16_t PIECE_TYPE(int_fast16_t pc) { return pieceTypes[pc]; }

// 获得棋子简单分值
inline int_fast16_t SIMPLE_VALUE(int_fast16_t pc) { return simpleValues[pc]; }

/* SIDE_TAG()，红方设为 16，黑方设为 32。
 * 用 "SIDE_TAG() + i" 选择棋子的类型， "i" 从 0 到 15 依次是：
 * 帅仕仕相相马马车车炮炮兵兵兵兵兵(将士士象象马马车车炮炮卒卒卒卒卒)
 * 例如"i"取"KNIGHT_FROM"到"KNIGHT_TO"，则表示依次检查两个马的位置
 * OPP_SIDE 返回对方颜色 0 为红方 1 为黑方
 */
inline int_fast16_t SIDE_TAG(int_fast16_t side) { return 16 + (side << 4); }

inline int_fast16_t OPP_SIDE_TAG(int_fast16_t side) { return 32 - (side << 4); }

inline int_fast16_t OPP_SIDE(int_fast16_t sd) { return sd ^ 1; }

// 着法对象
struct MoveObj { int_fast16_t mv, vl, cap; /* 着法、分值、捕获的棋子*/ };

// 得到着法起点
inline int_fast16_t SRC(uint_fast16_t mv) { return mv & 255; }

// 得到着法终点
inline int_fast16_t DST(uint_fast16_t mv) { return mv >> 8; }

// 由起点和终点得到着法
inline uint_fast16_t MOVE(int_fast16_t src, int_fast16_t dst) {
    return src + (dst << 8);
}

// 棋子走法
extern const int_fast16_t KING_DELTA[4];
extern const int_fast16_t ADVISOR_DELTA[4];
extern const int_fast16_t KNIGHT_DELTA[4][2];
extern const int_fast16_t KING_KNIGHT_DELTA[4][2];

/* 棋子类型-位置价值表
 * 获取棋子类型见 PIECE_TYPE(pc) 函数
 * 注意黑方在下 初始 sq 大
 */ 
extern const int_fast16_t SQ_VALUE[PIECE_EMPTY + 1][256];

// 将FEN串中棋子标识转化为对应棋子类型 pt 需toupper转化为大写
int_fast16_t charToPt(char c);

/* 将棋子转化为 FEN 字符 返回大写字符
 * pt 为棋子类型
 */
char ptToChar(int_fast16_t pt);

std::string MOVE_TO_STR(int_fast16_t mv);

// 回滚对象
struct RollbackObj {
  unsigned long long zobrist; // Zobrist
  int_fast16_t vlRed, vlBlack; // 红方和黑方的子力价值
  MoveObj mvObj;       // 着法
}; // rbOjb

struct Position {
    // 初始化棋局数组
    void clear();

    // 将棋子 pc 添加进棋局中的 sq 位置 del = true 则删除棋子
    void addPiece(int_fast16_t sq, int_fast16_t pc, bool del = false);

    // 交换走子方
    void changeSide();
    // 保存状态
    void saveStatus();
    // 回到之前状态
    void rollBack();

    // 通过FEN串初始化棋局
    void fromFen(const char* fen);

    // 根据 mvStr 字符串移动棋子
    void movePiece(std::string mvStr);

    // 根据整型 mv 移动棋子；mv 见 MOVE() 函数
    void movePiece(int_fast16_t mv);
    // 撤销移动棋子
    void undoMovePiece(int_fast16_t mv, int_fast16_t cap);

    // 执行走法
    bool makeMove();
    // 撤销走法
    void undoMakeMove();
    // 得到下一个走法，无走法返回 0
    int_fast16_t nextMove();

    // 输棋分值 与深度有关
    int_fast16_t mateValue();
    // 判断是否被将军 是则返回 true
    int_fast16_t isChecked();

    // 着法生成 见 genMoves.cpp 帅仕相马车炮兵
    void genAllMoves();

#ifdef POS_DEBUG
    // 通过棋盘字符串初始化
    void fromStringMap(std::string* s, int_fast16_t side);
    void debug();
#endif
    // 棋子-棋盘联系组
    int_fast16_t squares[256]; // 每个格子放的棋子，0 为无子
    int_fast16_t pieces[48]; // 每个棋子放的位置，0 为棋子不存在

    int_fast16_t sidePly; // 走子方，0 为 红方，1 为 黑方

    int_fast16_t vlRed, vlBlack; // 红方、黑方估值

    int_fast16_t moveNum, distance; // 着法数、搜索步数
    RollbackObj rollBackList[MAX_LIST_SIZE]; // 回滚列表

    int_fast16_t genNum[MAX_DISTANCE]; // 某一层的着法数
    int_fast16_t curMvCnt[MAX_DISTANCE]; // 当前层枚举到的走法下标
    MoveObj mvsGen[MAX_DISTANCE][MAX_GER_NUM]; // 某一层的着法
};

extern Position pos;
#endif