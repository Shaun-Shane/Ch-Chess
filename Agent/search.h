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

constexpr int32_t QUIESC_LIMIT = 40; // 静态搜索限定深度

constexpr int32_t NULL_DEPTH = 2; // 空着减去的深度

std::pair<int32_t, int32_t> searchMain();

// 根节点搜索
std::pair<int32_t, int32_t> searchRoot(int32_t depth);

// minMax 搜索
int32_t searchFull(int32_t depth, int32_t alpha, int32_t beta, bool noNull = false);

int32_t searchQuiescence(int32_t alpha, int32_t beta);

#endif