#ifndef EVALUATE_H
#define EVALUATE_H

#include "Position.h"

// 空头炮威胁分值，行号 0-16
extern const int32_t HOLLOW_THREAT[16];
// 炮镇窝心马的威胁分值，行号 0-16 或一般中炮威胁
extern const int32_t CENTRAL_THREAT[16];
//沉底炮的威胁分值 列号 0-16
extern const int32_t BOTTOM_THREAT[16];

constexpr int32_t ADVISOR_LEAKAGE = 50; // 缺仕的分值

int32_t advisorShape();

int32_t evaluate();

#endif