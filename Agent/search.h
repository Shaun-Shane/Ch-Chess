#ifndef SEARCH_H
#define SEARCH_H

#include <utility>

// VSC_DEBUG 在 Agent.hpp 中也有
#define VSC_ DEBUG

#ifndef VSC_DEBUG
#include "Position.h"
#include "evaluate.h"
#else
#include "Position.cpp"
#include "genMoves.cpp"
#include "evaluate.cpp"
#endif

constexpr int_fast32_t QUIESC_LIMIT = 32; // 静态搜索限定深度



// 根节点搜索
std::pair<int_fast16_t, int_fast16_t> searchRoot(int_fast16_t depth);

// minMax 搜索
int_fast16_t searchFull(int_fast16_t depth, int_fast16_t alpha, int_fast16_t beta);

int_fast16_t searchQuiescence(int_fast16_t alpha, int_fast16_t beta);

#endif