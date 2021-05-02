#ifndef ZOBRIST_H
#define ZOBRIST_H
#include <algorithm>
#include <vector>

#include "Position.h"
#define CHESS_TYPE_NUM 14  //棋子种类数
#define START_LIBRARY_SIZE 12081
#define BOARD_SIZE 256

extern const unsigned int zobristKeyPlayer;  //代表着子方变化的zobrist key值
extern const unsigned int zobristLockPlayer;  //代表着子方变化的zobrist lock值
//棋子pc在位置sq的zobrist key 值为zobristKeyTable[pc][sq]，lock值同理，
// pc共有14种，0-6为红帅士相马车炮兵，7-13为黑方对应棋子，sq为0-255，对应棋盘相应位置
extern const unsigned int zobristKeyTable[CHESS_TYPE_NUM][BOARD_SIZE];
extern const unsigned int zobristLockTable[CHESS_TYPE_NUM][BOARD_SIZE];

//库数据结构体
struct Library {
    unsigned int situation;  //某一个局面对应的zobrist lock值
    unsigned short moveInfo, moveValue;  //该局面下的移动方法及相应的价值
};

//开局库数组
extern const Library startLib[START_LIBRARY_SIZE];
//在大小为n的库数组中二分查找到zobrist lock 值为lock的局面
int binarySearchLib(const Library lib[], unsigned int lock, int n);
//左右对称的sq位置
int mirrorPos(int sq);
//左右对称的mv移动方式
int mirrorMov(int mv);
//以zobrist lock值为关键字从小到大排序Library结构体
bool comp(const Library &l1, const Library &l2);

class Zobrist {
   protected:
    //当前局面对应的zobrist key和lock值
    unsigned int curZobristKey, curZobristLock;

   public:
    //构造函数，置0
    Zobrist();
    // 置零
    void initZero();
    //返回curZobristKey值
    unsigned int getCurKey();
    //返回curZobristLock值
    unsigned int getCurLock();
    //移动棋子时调用，更新当前局面的zobrist key和lock值，若pc!=0，则
    // curZobristKey和curZobristLock分别异或zobristKeyTable[pc][sq]
    //和zobristLockTable[pc][sq](无论加棋子还是删棋子都一样)
    void zobristUpdateMove(int sq, int pc);
    //改变着子方时更新当前局面的zobrist key和lock值
    void zobristUpdateChangeSide();
    //在squares和sidePlayer确定的局面下搜索库lib找到对应的局面(或镜像局面)，返回移动方法
    int getMoveFromLib(int squares[], int sidePlayer, const Library lib[]);
    //构造函数，由某一个局面特定的squares，sidePlayer值，并根据是否镜像翻转，确定对应的zobrist值
    Zobrist(int squares[], int sidePlayer, bool mirror);
};

extern Zobrist zob;

#endif