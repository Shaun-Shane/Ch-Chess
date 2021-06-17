#ifndef POSITION_H
#define POSITION_H

#include <iostream>
#include <cctype>
#include <cstring>
#include <algorithm>

#define POS_DEBUG

// 棋子编号
enum PIECE {
    PIECE_KING,     // 将
    PIECE_ADVISOR,  // 士
    PIECE_BISHOP,   // 象
    PIECE_KNIGHT,   // 马
    PIECE_ROOK,     // 车
    PIECE_CANNON,   // 炮
    PIECE_PAWN,     // 卒
    PIECE_EMPTY     // 空
};

// 启发阶段
enum PHASE {HASH, KILLER_1, KILLER_2, GEN_CAP, CAP, GEN_NCAP, OTHER};

// 每种棋子的开始序号和结束序号 0 ~ 15
constexpr int32_t KING_FROM = 0;                     // 将
constexpr int32_t ADVISOR_FROM = 1, ADVISOR_TO = 2;  // 士
constexpr int32_t BISHOP_FROM = 3, BISHOP_TO = 4;    // 象
constexpr int32_t KNIGHT_FROM = 5, KNIGHT_TO = 6;    // 马
constexpr int32_t ROOK_FROM = 7, ROOK_TO = 8;        // 车
constexpr int32_t CANNON_FROM = 9, CANNON_TO = 10;   // 炮
constexpr int32_t PAWN_FROM = 11, PAWN_TO = 15;      // 卒

constexpr bool DEL_PIECE = true;   // 删除棋子

constexpr int32_t MATE_VAL = 1e4; // 将军
constexpr int32_t BAN_VAL = MATE_VAL - 100; // 长将判负
constexpr int32_t WIN_VAL = MATE_VAL - 200; // 赢棋分值 高于 WIN_VALUE 都是赢棋
constexpr int32_t DRAW_VAL = 20; // 和棋时返回分数 取负值

constexpr int32_t MAX_GEN_NUM = 1 << 7; // 最多可能的着法数 不会超过 128
constexpr int32_t MAX_DISTANCE = 1 << 7; // 最多搜索层数

constexpr int32_t MINI_HASH_MASK = (1 << 15) - 1; // mini 置换表大小

// 棋盘范围
constexpr int32_t Y_FROM = 3, Y_TO = 12;
constexpr int32_t X_FROM = 3, X_TO = 11;

// 车和炮位于某一列(0-8)，在行状态st下，能吃到的最左、最右子的列数(0-15)
extern int32_t rookCapX[9][1 << 9][2];
extern int32_t cannonCapX[9][1 << 9][2];
// 车和炮位于某一行(0-9)，在列状态st下，能吃到的最上、最下子的行数(0-15)
extern int32_t rookCapY[10][1 << 10][2];
extern int32_t cannonCapY[10][1 << 10][2];
// 炮位于某一行(0-9)，在列状态st下， 能隔两子吃到的最上、最下子的行数(0-15)
extern int32_t cannonSupperCapY[10][1 << 10][2];
// sq 在行、列 对应的二进制状态码
extern int32_t bitMaskY[256], bitMaskX[256];
// 马的着法与对应马腿预生成数组
extern int32_t knightMvDst[256][12], knightMvPin[256][8];

// 用于判断棋子是否在棋盘上
extern const int32_t _IN_BOARD[256];
// 用于判断棋子是否在九宫内
extern const int32_t _IN_FORT[256];
// 将仕象的合法走法判断数组
extern const int32_t KAB_LEGAL_SPAN[512];
// 辅助数组，用于校验马的走法是否合理。如果合理，返回对应马脚的方向；否则，返回0
extern const int32_t _KNIGHT_PIN[512];

// 根据 x, y 返回序号 0 ~ 255
inline int32_t COORD_XY(int32_t x, int32_t y) { return x + (y << 4); }

// 根据 sq 0 ~ 255 返回行数 y 0 ~ 16
inline int32_t GET_Y(int32_t sq) { return sq >> 4; }

// 根据 sq 0 ~ 255 返回列数 x 0 ~ 16
inline int32_t GET_X(int32_t sq) { return sq & 15; }

// 判断某位置是否在棋盘
inline bool IN_BOARD(int32_t sq) { return _IN_BOARD[sq]; }

// 判断某位置是否在九宫
inline bool IN_FORT(int32_t sq) { return _IN_FORT[sq]; }

// 校验将（帅）的走法
inline bool K_SPAN(int32_t src, int32_t dst) {
    return KAB_LEGAL_SPAN[dst - src + 256] == 1;
}

// 检验士（仕）的走法
inline bool A_SPAN(int32_t src, int32_t dst) {
    return KAB_LEGAL_SPAN[dst - src + 256] == 2;
}

// 校验象（相）的走法
inline bool B_SPAN(int32_t src, int32_t dst) {
    return KAB_LEGAL_SPAN[dst - src + 256] == 3;
}

// 象眼位置
inline int32_t B_PIN(int32_t src, int32_t dst) { return (src + dst) >> 1; }

// 如果马的走法合法，则返回相应马脚的位置，否则返回 src。
inline int32_t K_PIN(int32_t src, int32_t dst) {
    return src + _KNIGHT_PIN[dst - src + 256];
}

// 根据位置 sq 判断是否过河，未过河返回 true；否则返回 false
inline bool SELF_SIDE(int32_t sq, int32_t side) {
    return (sq & 0x80) == (side << 7);  // 设定红方 sq 小
}

// 返回向前走一步后的位置 sq
inline int32_t SQ_FORWARD(int32_t sq, int32_t side) {
    return sq + 16 - (side << 5);
}

// 水平翻转后的位置 注意一开始红方左下角的 sq 为 51，黑方右上角 sq 为 203
inline int32_t SQ_FLIP(int32_t sq) { return 254 - sq; }

// 如果两位置在同一行，返回 true
inline bool SAME_Y(int32_t src, int32_t dst) {
    return ((src ^ dst) & 0xf0) == 0;
}

// 如果两位置在同一列，返回 true
inline bool SAME_X(int32_t src, int32_t dst) {
    return ((src ^ dst) & 0x0f) == 0;
}

/* 棋子序号对应的棋子类型
 * 16到31表示红子，32到47表示黑子。
 * 帅仕仕相相马马车车炮炮兵兵兵兵兵(将士士象象马马车车炮炮卒卒卒卒卒)
 */
extern const int32_t pieceTypes[48];
// lva 价值
extern const int32_t lvaValues[48];
// mvv 价值
extern const int32_t mvvValues[48];

// 获得棋子类型
inline int32_t PIECE_TYPE(int32_t pc) { return pieceTypes[pc]; }

/* SIDE_TAG()，红方设为 16，黑方设为 32。*/
inline int32_t SIDE_TAG(int32_t side) { return 16 + (side << 4); }

inline int32_t OPP_SIDE_TAG(int32_t side) { return 32 - (side << 4); }

// 着法对象
struct Moves {
    int32_t mv; // 着法
    int_fast64_t vl; /*分值*/
};

struct MoveListNode {
    int32_t mv, cap, chk; // 着法 吃的子 是否将军
    uint32_t key; // zobrist 键值
};

// 着法比较函数
inline bool operator<(const Moves& lhs, const Moves& rhs) {
    return lhs.vl > rhs.vl;
}

// 得到着法起点
inline int32_t SRC(int32_t mv) { return mv & 255; }

// 得到着法终点
inline int32_t DST(int32_t mv) { return mv >> 8; }

// 由起点和终点得到着法
inline int32_t MOVE(int32_t src, int32_t dst) {
    return src + (dst << 8);
}

// 棋子走法
extern const int32_t KING_DELTA[4];
extern const int32_t ADVISOR_DELTA[4];
extern const int32_t KNIGHT_DELTA[4][2];

/* 棋子类型-位置价值表
 * 获取棋子类型见 PIECE_TYPE(pc) 函数
 * 注意黑方在下 初始 sq 大
 * 见 evaluate.cpp
 */ 
extern const int32_t SQ_VALUE[PIECE_EMPTY + 1][256];

// 将FEN串中棋子标识转化为对应棋子类型 pt 需toupper转化为大写
int32_t charToPt(char c);

/* 将棋子转化为 FEN 字符 返回大写字符
 * pt 为棋子类型
 */
char ptToChar(int32_t pt);

std::string MOVE_TO_STR(int32_t mv);

int32_t STR_TO_MOVE(std::string mvStr);

class Zobrist;

struct Position {
    // 初始化位行列、马走法
    void preGen();
    // 返回 Rook 左右吃子的位置，没有则返回 0
    int32_t getRookCapX(int32_t src, bool tag);
    // 返回 Cannon 左右吃子的位置，没有则返回 0
    int32_t getCannonCapX(int32_t src, bool tag);
    // 返回 Rook 上下吃子的位置，没有则返回 0
    int32_t getRookCapY(int32_t src, bool tag);
    // 返回 Cannon 上下吃子的位置，没有则返回 0
    int32_t getCannonCapY(int32_t src, bool tag);
    // 返回 Cannon 上下隔两子吃子的位置，没有则返回 0
    int32_t getCannonSupperCapY(int32_t src, bool tag);

    // 初始化棋局数组
    void clear();

    // 将棋子 pc 添加进棋局中的 sq 位置 del = true 则删除棋子
    void addPiece(int32_t sq, int32_t pc, bool del = false);

    // 交换走子方
    void changeSide();

    // 根据 mvStr 字符串移动棋子
    void movePiece(std::string mvStr);

    // 根据整型 mv 移动棋子；mv 见 MOVE() 函数
    void movePiece(int32_t mv);
    // 撤销移动棋子
    void undoMovePiece(int32_t mv, int32_t cap);

    // 执行走法
    bool makeMove(int32_t mv);
    // 撤销走法
    void undoMakeMove();
    // 得到下一个走法，无走法返回 0
    int32_t nextMove();
    // 空着
    void makeNullMove();
    void undoMakeNullMove();

    // 当前局面的优势是否足以进行空步搜索
    int32_t nullCan();
    // 空步搜索得到的分值是否有效
    int32_t nullValid();

    // 仕的棋形评估 包括空头炮、中炮、沉底炮、窝心马 见 evaluate.cpp
    int32_t adviShape();
    // 车的灵活性
    int32_t rookMobility();
    // 马受阻碍评价
    int32_t knightBlock();
    // 局面评估函数
    int32_t evaluate();

    // 重复局面分数
    int32_t repVal(int32_t vl);
    // 长将判负分值 与深度有关
    int32_t banVal();
    // 和棋分值
    int32_t drawVal();
    // 输棋分值 与深度有关
    int32_t mateVal();
    // 判断重复局面
    int32_t repPosition(int32_t repCount = 1);
    
    // 获得MVV/LVA分值 pcV 为被吃子
    int32_t mvvLva(int32_t pcV, int32_t pcA);

    // 部分着法生成，被将军时生成全部着法，之后按不同阶段启发 见 genMoves.cpp
    void genMovesInit(int32_t mvHash = 0);
    // 全部着法生成
    void genAllMoves();
    // 非吃子着法生成 见 genMoves.cpp 帅仕相马车炮兵
    void genNonCapMoves();
    // 吃子着法生成 见 genMoves.cpp
    void genCapMoves();
    // 将 mvKiller, mvHash 清零
    void resetMvKillerHash();
    // 判断是否被将军 是则返回 true。 和着法生成类似 也放在 genMoves.cpp 中
    int32_t isChecked();
    // 判断着法 mv 是否合法
    int32_t isLegalMove(int32_t mv);
    // 判断一个位置是否被保护 保护方为 side
    int32_t isProtected(int32_t side, int32_t src, int32_t sqExcp = -1);
    // 判断捉 返回被捉的棋子类型 (0~15)
    int32_t pcChased(int32_t mv);

    // 通过FEN串初始化棋局
    void fromFen(const char* fen);
#ifdef POS_DEBUG
    // 通过棋盘字符串初始化
    void fromStringMap(std::string* s, int32_t side);
    void debug();
#endif
    // 棋子-棋盘联系组
    int32_t squares[256]; // 每个格子放的棋子，0 为无子
    int32_t pieces[48]; // 每个棋子放的位置，0 为棋子不存在
    int32_t stateY[16]; // 每一行的二进制状态
    int32_t stateX[16]; // 每一列的二进制状态

    int32_t sidePly; // 走子方，0 为 红方，1 为 黑方

    int32_t vlRed, vlBlack; // 红方、黑方估值

    int32_t moveNum, distance; // 着法数、搜索步数
    MoveListNode moveList[MAX_DISTANCE << 2]; // 着法列表

    int32_t genNum[MAX_DISTANCE]; // 某一层的着法数
    int32_t curMvCnt[MAX_DISTANCE]; // 当前层枚举到的走法下标
    int32_t phase[MAX_DISTANCE]; // 启发阶段
    int32_t mvHash[MAX_DISTANCE]; // 置换表着法
    int32_t mvKiller1[MAX_DISTANCE], mvKiller2[MAX_DISTANCE]; // 两个杀手着法
    Moves mvsGen[MAX_DISTANCE][MAX_GEN_NUM]; // 某一层的着法

    Zobrist* zobrist;
};

extern Position pos;

// 历史表
extern int_fast64_t historyTable[1 << 12];
// 杀手着法表
extern int32_t killerTable[MAX_DISTANCE][2];
// 重复局面检测迷你置换表
extern int32_t miniHash[MINI_HASH_MASK + 1];

// 更新历史表
inline void setHistory(int32_t mv, int32_t depth) {
    historyTable[((((SIDE_TAG(pos.sidePly) - 16) >> 1) +
             PIECE_TYPE(pos.squares[SRC(mv)]))
            << 8) +
           DST(mv)] += depth * depth;
}

// 获得 mv 对应的历史表下标
inline int32_t historyIndex(int32_t mv) {
    return ((((SIDE_TAG(pos.sidePly) - 16) >> 1) +
             PIECE_TYPE(pos.squares[SRC(mv)]))
            << 8) +
           DST(mv);
}

// 更新杀手着法表
inline void setKillerTable(int32_t mv) {
    auto tablePtr = killerTable[pos.distance];
    if (tablePtr[0] != mv) {
        tablePtr[1] = tablePtr[0]; // newKillerMove -> 0, 0 -> 1
        tablePtr[0] = mv;
    }
}

#endif