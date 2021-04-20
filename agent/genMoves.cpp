#ifndef GEN_MOVES_CPP
#define GEN_MOVES_CPP

#include "Position.h"

void Position::genAllMoves() {
    int sideTag = SIDE_TAG(this->sidePly), src, dst, i, j;
    // 0. 该 distance 下的 genNum 清零
    this->genNum[this->distance] = 0;
    MoveObj* mvsGenPtr = this->mvsGen[this->distance];

    // 1. 生成 KING 走法
    src = this->pieces[sideTag + KING_FROM];
    for (i = 0; i < 4; i++) { // KING 的四个方向
        dst = src + KING_DELTA[i];
        if (!IN_FORT(dst)) continue;
        if (!(this->squares[dst] & sideTag)) { // 不是本方棋子
            mvsGenPtr->mv = MOVE(src, dst);
            // mvsGenPtr->vl = ...;
            mvsGenPtr->cap = this->squares[dst];
            // 指向下一个位置
            mvsGenPtr++, this->genNum[this->distance]++;
        }
    }

    // 2. 生成 ADVISOR 走法
    for (i = ADVISOR_FROM; i <= ADVISOR_TO; i++) {
        src = this->pieces[sideTag + i];
        for (j = 0; j < 4; j++) { // ADVISOR 的四个方向
            dst = src + ADVISOR_DELTA[j];
            if (!IN_FORT(dst)) continue;
            if (!(this->squares[dst] & sideTag)) {
                mvsGenPtr->mv = MOVE(src, dst);
                // mvsGenPtr->vl = ...;
                mvsGenPtr->cap = this->squares[dst];
                // 指向下一个位置
                mvsGenPtr++, this->genNum[this->distance]++;
            }
        }
    }

    //3. 生成 BISHIP 走法
    for (i = BISHOP_FROM; i <= BISHOP_TO; i++) {
        src = this->pieces[sideTag + i];
        for (j = 0; j < 4; j++) { // 象的四个方向
            dst = src + ADVISOR_DELTA[j]; // 象眼的四个位置，通过仕的 delta 获得
            if (!IN_BOARD(dst) || !SELF_SIDE(dst, this->sidePly) ||
                this->squares[dst])
                continue;
            dst += ADVISOR_DELTA[j];
            if (!(this->squares[dst] & sideTag)) {
                mvsGenPtr->mv = MOVE(src, dst);
                // mvsGenPtr->vl = ...;
                mvsGenPtr->cap = this->squares[dst];
                // 指向下一个位置
                mvsGenPtr++, this->genNum[this->distance]++;
            }
        }
    }

    //4. 生成 KNIGHT 走法
    for (i = KNIGHT_FROM; i <= KNIGHT_TO; i++) {
        
    }
}

#endif