#include "Position.h"

// 添加 mv 到 mvsGen 数组
#define ADD_MOVE()                                                  \
    {                                                               \
        if (!(this->squares[dst] & sideTag)) {                      \
            mvsGenPtr->mv = MOVE(src, dst);                         \
            /*mvsGenPtr->vl = ...*/                                 \
            mvsGenPtr->cap = this->squares[dst];                    \
            mvsGenPtr++, this->genNum[this->distance]++;            \
        }                                                           \
    }

void Position::genAllMoves() {
    int_fast16_t sideTag = SIDE_TAG(this->sidePly);
    int_fast16_t src, dst, i, j, k, delta;
    // 0. 该 distance 下的 genNum 清零，curMvCnt 置为 -1
    this->genNum[this->distance] = 0;
    this->curMvCnt[this->distance] = -1;
    MoveObj* mvsGenPtr = this->mvsGen[this->distance];

    // 1. 生成 KING 走法 将军死亡不会再调用 genAllMoves的
    src = this->pieces[sideTag + KING_FROM];
    for (i = 0; i < 4; i++) { // KING 的四个方向
        dst = src + KING_DELTA[i];
        if (!IN_FORT(dst)) continue;
        ADD_MOVE();
    }

    // 2. 生成 ADVISOR 走法
    for (i = ADVISOR_FROM; i <= ADVISOR_TO; i++) {
        if (!(src = this->pieces[sideTag + i])) continue;
        for (j = 0; j < 4; j++) { // ADVISOR 的四个方向
            dst = src + ADVISOR_DELTA[j];
            if (!IN_FORT(dst)) continue;
            ADD_MOVE();
        }
    }

    //3. 生成 BISHIP 走法
    for (i = BISHOP_FROM; i <= BISHOP_TO; i++) {
        if (!(src = this->pieces[sideTag + i])) continue;
        for (j = 0; j < 4; j++) { // 象的四个方向
            dst = src + ADVISOR_DELTA[j]; // 象眼的四个位置，通过仕的 delta 获得
            if (!IN_BOARD(dst) || !SELF_SIDE(dst, this->sidePly) ||
                this->squares[dst])
                continue;
            dst += ADVISOR_DELTA[j];
            ADD_MOVE();
        }
    }

    //4. 生成 KNIGHT 走法
    for (i = KNIGHT_FROM; i <= KNIGHT_TO; i++) {
        if (!(src = this->pieces[sideTag + i])) continue;
        for (j = 0; j < 4; j++) {
            dst = src + KING_DELTA[j]; // 马腿位置
            if (this->squares[dst] || !IN_BOARD(dst)) continue; // 越界或马腿有棋子
            for (k = 0; k < 2; k++) {
                dst = src + KNIGHT_DELTA[j][k];
                if (!IN_BOARD(dst)) continue;
                ADD_MOVE();
            }
        }
    }

    // 5. 生成 ROCK 走法
    for (i = ROOK_FROM; i <= ROOK_TO; i++) {
        if (!(src = this->pieces[sideTag + i])) continue;
        for (j = 0; j < 4; j++) {
            delta = KING_DELTA[j];
            dst = src + delta; // 从起点开始，沿着方向 delta 走一步
            while (IN_BOARD(dst)) { // 终点在棋盘内
                if (this->squares[dst] & sideTag) break; // 遇到己方棋子 停止
                ADD_MOVE();
                if (this->squares[dst]) break; // 吃子 停下
                dst += delta; // 沿着方向 delta 走一步
            }
        }
    }

    // 6. 生成 CANNON 走法
    for (i = CANNON_FROM; i <= CANNON_TO; i++) {
        if (!(src = this->pieces[sideTag + i])) continue;
        for (j = 0; j < 4; j++) {
            delta = KING_DELTA[j];
            dst = src + delta; // 从起点开始，沿着方向 delta 走一步
            while (IN_BOARD(dst)) { // 终点在棋盘内
                if (this->squares[dst]) break; // 遇到棋子 停止
                ADD_MOVE();
                dst += delta; // 沿着方向 delta 走一步
            }
            dst += delta; // 检查是否能吃子
            while (IN_BOARD(dst)) {
                if (this->squares[dst]) {
                    ADD_MOVE();
                    break; // 遇到一个棋子则停下
                }
                dst += delta;
            }
        }
    }

    // 7. 生成 PAWN 走法
    for (i = PAWN_FROM; i <= PAWN_TO; i++) {
        if (!(src = this->pieces[sideTag + i])) continue;
        dst = SQ_FORWARED(src, this->sidePly);
        if (IN_BOARD(dst)) ADD_MOVE();
        if (!SELF_SIDE(src, this->sidePly)) { // 过河卒
            if (IN_BOARD(dst = src - 1)) ADD_MOVE();
            if (IN_BOARD(dst = src + 1)) ADD_MOVE();
        }
    }
}