#include "Position.h"

void Position::resetMvKillerHash() {
    this->mvHash[this->distance] = 0;
    this->mvKiller1[this->distance] = 0;
    this->mvKiller2[this->distance] = 0; // 清空置换表 杀手着法
    this->phase[this->distance] = PHASE::HASH; // 从置换表启发开始
}

void Position::generateMoves(int32_t mvHash) { // mvHash 默认为 0
    this->resetMvKillerHash();

    if (this->isChecked()) { // 处于被将军状态 禁用杀手启发
        this->phase[this->distance] = PHASE::OTHER;
        this->genAllMoves(mvHash); // 生成全部着法， mvHash 优先级最大
    } else { // 未被将军 才考虑从 phase-HASH 开始启发
        this->mvHash[this->distance] = mvHash;
        this->mvKiller1[this->distance] = killerTable[this->distance][0];
        this->mvKiller2[this->distance] = killerTable[this->distance][1];
    }
}

// 仅添加一个着法，用于置换表、杀手启发
#define ADD_ONE_MOVE()                                              \
    {                                                               \
        this->curMvCnt[this->distance] = 0;                         \
        this->mvsGen[this->distance][0].mv = mv;                      \
        this->mvsGen[this->distance][0].cap = this->squares[DST(mv)]; \
    }
// 得到下一个走法，无走法返回 0
int32_t Position::nextMove() {
    int32_t mv = 0;
    switch (this->phase[this->distance]) {
        case PHASE::HASH:
            this->phase[this->distance] = PHASE::KILLER_1;
            mv = this->mvHash[this->distance];
            if (mv /*&& legalmove ? */) {
                ADD_ONE_MOVE();
                return mv;
            }
        // 2. 第一个杀手着法
        case PHASE::KILLER_1:
            this->phase[this->distance] = PHASE::KILLER_2;
            mv = this->mvKiller1[this->distance];
            if (mv != this->mvHash[this->distance] && mv &&
                this->isLegalMove(mv)) {
                    ADD_ONE_MOVE();
                    return mv;
                }
        // 3. 第二个杀手着法
        case PHASE::KILLER_2:
            this->phase[this->distance] = PHASE::GEN_MOVES;
            mv = this->mvKiller2[this->distance];
            if (mv != this->mvHash[this->distance] && mv &&
                this->isLegalMove(mv)) {
                    ADD_ONE_MOVE();
                    return mv;
                }

        // 4. 生成所有着法，完成后立即进入下一阶段；
        case PHASE::GEN_MOVES:
            this->phase[this->distance] = PHASE::OTHER;
            this->genAllMoves(); // 着法生成 + 历史表启发
        default:
            while (this->curMvCnt[this->distance] + 1 < this->genNum[this->distance]) {
                this->curMvCnt[this->distance]++;
                mv = (this->mvsGen[this->distance] + this->curMvCnt[this->distance])->mv;
                if (mv != this->mvHash[this->distance] &&
                    mv != this->mvKiller1[this->distance] &&
                    mv != this->mvKiller2[this->distance])
                    return mv;
            }
    }

    return 0; // 没有着法 返回 0
}

// 添加 mv 到 mvsGen 数组
#define ADD_MOVE()                                                           \
    {                                                                        \
        if (!(this->squares[dst] & sideTag)) {                               \
            mvsGenPtr->mv = MOVE(src, dst);                                  \
            mvsGenPtr->vl = (mvsGenPtr->mv == mvHash)                        \
                                ? 1e18 /*置换表着法分值最大*/       \
                                : historyTable[historyIndex(mvsGenPtr->mv)]; \
            mvsGenPtr->cap = this->squares[dst];                             \
            mvsGenPtr++, this->genNum[this->distance]++;                     \
        }                                                                    \
    }

void Position::genAllMoves(int32_t mvHash) { // param-mvHash 默认值为 0
    int32_t sideTag = SIDE_TAG(this->sidePly);
    int32_t src, dst, i, j, k, delta;
    // 0. 该 distance 下的 genNum 清零，curMvCnt 置为 -1
    this->genNum[this->distance] = 0;
    this->curMvCnt[this->distance] = -1;
    Moves* mvsGenPtr = this->mvsGen[this->distance];

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
    
    // 8. 对着法排序
    sortMoves();
}

void Position::sortMoves() {
    std::sort(this->mvsGen[this->distance], this->mvsGen[this->distance] + this->genNum[this->distance]);
}

// 添加 cap_mv 到 mvsGen 数组
#define ADD_CAP_MOVE()                                                       \
    {                                                                        \
        if (!(this->squares[dst] & sideTag) && this->squares[dst]) {         \
            mvsGenPtr->mv = MOVE(src, dst);                                  \
            mvsGenPtr->vl = MVV_LVA(this->squares[dst], this->squares[src]); \
            mvsGenPtr->cap = this->squares[dst];                             \
            mvsGenPtr++, this->genNum[this->distance]++;                     \
        }                                                                    \
    }

// 生成吃子着法
void Position::genCapMoves() {
    int32_t sideTag = SIDE_TAG(this->sidePly);
    int32_t src, dst, i, j, k, delta;
    // 0. 该 distance 下的 genNum 清零，curMvCnt 置为 -1
    this->genNum[this->distance] = 0;
    this->curMvCnt[this->distance] = -1;
    Moves* mvsGenPtr = this->mvsGen[this->distance];

    // 1. 生成 KING 吃子走法
    src = this->pieces[sideTag + KING_FROM];
    for (i = 0; i < 4; i++) { // KING 的四个方向
        dst = src + KING_DELTA[i];
        if (!IN_FORT(dst)) continue;
        ADD_CAP_MOVE();
    }

    // 2. 生成 ADVISOR 吃子走法
    for (i = ADVISOR_FROM; i <= ADVISOR_TO; i++) {
        if (!(src = this->pieces[sideTag + i])) continue;
        for (j = 0; j < 4; j++) { // ADVISOR 的四个方向
            dst = src + ADVISOR_DELTA[j];
            if (!IN_FORT(dst)) continue;
            ADD_CAP_MOVE();
        }
    }

    //3. 生成 BISHIP 吃子走法
    for (i = BISHOP_FROM; i <= BISHOP_TO; i++) {
        if (!(src = this->pieces[sideTag + i])) continue;
        for (j = 0; j < 4; j++) { // 象的四个方向
            dst = src + ADVISOR_DELTA[j]; // 象眼的四个位置，通过仕的 delta 获得
            if (!IN_BOARD(dst) || !SELF_SIDE(dst, this->sidePly) ||
                this->squares[dst])
                continue;
            dst += ADVISOR_DELTA[j];
            ADD_CAP_MOVE();
        }
    }

    //4. 生成 KNIGHT 吃子走法
    for (i = KNIGHT_FROM; i <= KNIGHT_TO; i++) {
        if (!(src = this->pieces[sideTag + i])) continue;
        for (j = 0; j < 4; j++) {
            dst = src + KING_DELTA[j]; // 马腿位置
            if (this->squares[dst] || !IN_BOARD(dst)) continue; // 越界或马腿有棋子
            for (k = 0; k < 2; k++) {
                dst = src + KNIGHT_DELTA[j][k];
                if (!IN_BOARD(dst)) continue;
                ADD_CAP_MOVE();
            }
        }
    }

    // 5. 生成 ROCK 吃子走法
    for (i = ROOK_FROM; i <= ROOK_TO; i++) {
        if (!(src = this->pieces[sideTag + i])) continue;
        for (j = 0; j < 4; j++) {
            delta = KING_DELTA[j];
            dst = src + delta; // 从起点开始，沿着方向 delta 走一步
            while (IN_BOARD(dst)) { // 终点在棋盘内
                if (this->squares[dst] & sideTag) break; // 遇到己方棋子 停止
                if (this->squares[dst]) {
                    ADD_CAP_MOVE();
                    break; // 吃子 停下
                }
                dst += delta; // 沿着方向 delta 走一步
            }
        }
    }

    // 6. 生成 CANNON 吃子走法
    for (i = CANNON_FROM; i <= CANNON_TO; i++) {
        if (!(src = this->pieces[sideTag + i])) continue;
        for (j = 0; j < 4; j++) {
            delta = KING_DELTA[j];
            dst = src + delta; // 从起点开始，沿着方向 delta 走一步
            while (IN_BOARD(dst)) { // 终点在棋盘内
                if (this->squares[dst]) break; // 遇到棋子 停止
                dst += delta; // 沿着方向 delta 走一步
            }
            dst += delta; // 检查是否能吃子
            while (IN_BOARD(dst)) {
                if (this->squares[dst]) {
                    ADD_CAP_MOVE();
                    break; // 遇到一个棋子则停下
                }
                dst += delta;
            }
        }
    }

    // 7. 生成 PAWN 吃子走法
    for (i = PAWN_FROM; i <= PAWN_TO; i++) {
        if (!(src = this->pieces[sideTag + i])) continue;
        dst = SQ_FORWARED(src, this->sidePly);
        if (IN_BOARD(dst)) ADD_CAP_MOVE();
        if (!SELF_SIDE(src, this->sidePly)) { // 过河卒
            if (IN_BOARD(dst = src - 1)) ADD_CAP_MOVE();
            if (IN_BOARD(dst = src + 1)) ADD_CAP_MOVE();
        }
    }
    
    // 8. 对着法排序
    sortMoves();
}