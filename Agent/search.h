#ifndef SEARCH_H
#define SEARCH_H

#include <utility>
#include <ctime>
#include <tuple>

#ifndef VSC_DEBUG
#include "Position.h"
#include "evaluate.h"
#include "Zobrist.h"
#else
#include "Position.cpp"
#include "genMoves.cpp"
#include "evaluate.cpp"
#endif

constexpr int32_t DEPTH_LIMIT = 48; // 静态搜索限定深度

constexpr int32_t NULL_DEPTH = 2; // 空着减去的深度

constexpr int32_t HASH_LEVEL = 23;

// 节点类型
constexpr int32_t HASH_EMPTY = 0;
constexpr int32_t HASH_ALPHA = 1;
constexpr int32_t HASH_BETA = 2;
constexpr int32_t HASH_EXACT = 3;


struct HashItem {
    uint32_t zobristLock;
    int32_t mv, vl, flag, depth;
};

struct HashTable {
    void init();

    void recordHash(int32_t flag, int32_t vl, int32_t depth, int32_t mv);

    int32_t probeHash(int32_t alpha, int32_t beta, int32_t depth);

    int32_t hashMask = (1 << HASH_LEVEL) - 1;
    HashItem hashTable[1 << HASH_LEVEL];
};

std::pair<int32_t, int32_t> searchMain();

// 根节点搜索
std::pair<int32_t, int32_t> searchRoot(int32_t depth);

// minMax 搜索
int32_t searchFull(int32_t depth, int32_t alpha, int32_t beta, bool noNull = false);

int32_t searchQuiescence(int32_t alpha, int32_t beta);

#endif