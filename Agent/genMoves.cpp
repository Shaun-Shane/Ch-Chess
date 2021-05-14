#include "Position.h"

void Position::resetMvKillerHash() {
    this->mvHash[this->distance] = 0;
    this->mvKiller1[this->distance] = 0;
    this->mvKiller2[this->distance] = 0; // 清空置换表 杀手着法
    this->phase[this->distance] = PHASE::HASH; // 从置换表启发开始
}

void Position::genMovesInit(int32_t mvHash) { // mvHash 默认为 0
    this->resetMvKillerHash();
    this->mvHash[this->distance] = mvHash;

    if (this->isChecked()) { // 处于被将军状态 禁用杀手启发
        this->genAllMoves(); // 生成全部着法
    } else { // 未被将军 才考虑从杀手着法
        this->mvKiller1[this->distance] = killerTable[this->distance][0];
        this->mvKiller2[this->distance] = killerTable[this->distance][1];
    }
}

// 得到下一个走法，无走法返回 0
int32_t Position::nextMove() {
    int32_t mv = 0;
    switch (this->phase[this->distance]) {
        case PHASE::HASH:
            this->phase[this->distance] = PHASE::KILLER_1;
            mv = this->mvHash[this->distance];
            if (mv && this->isLegalMove(mv)) {
                return mv;
            }
        // 2. 第一个杀手着法
        case PHASE::KILLER_1:
            this->phase[this->distance] = PHASE::KILLER_2;
            mv = this->mvKiller1[this->distance];
            if (mv != this->mvHash[this->distance] && mv &&
                this->isLegalMove(mv)) {
                    return mv;
                }
        // 3. 第二个杀手着法
        case PHASE::KILLER_2:
            this->phase[this->distance] = PHASE::GEN_MOVES;
            mv = this->mvKiller2[this->distance];
            if (mv != this->mvHash[this->distance] && mv &&
                this->isLegalMove(mv)) {
                    return mv;
                }

        // 4. 生成所有着法，完成后立即进入下一阶段；
        case PHASE::GEN_MOVES:
            this->phase[this->distance] = PHASE::OTHER;
            this->genAllMoves(); // 置换表 + 历史表启发
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

void Position::genAllMoves() {
    this->genCapMoves(), this->genNonCapMoves();
}

// 添加非吃子 mv 到 mvsGen 数组
#define ADD_NCAP_MOVE()                                                \
    {                                                                  \
        if (!this->squares[dst]) {                                     \
            mvsGenPtr->mv = MOVE(src, dst);                            \
            mvsGenPtr->vl = historyTable[historyIndex(mvsGenPtr->mv)]; \
            mvsGenPtr++, this->genNum[this->distance]++;               \
        }                                                              \
    }

void Position::genNonCapMoves() {
    int32_t sideTag = SIDE_TAG(this->sidePly);
    int32_t src, dst, i, j, delta;
    
    // 从吃子着法后开始生成非吃子着法
    int32_t genNumSt(this->genNum[this->distance]);
    Moves* mvsGenPtr = this->mvsGen[this->distance] + this->genNum[this->distance];

    // 1. 生成 KING 走法 将军死亡不会再调用 genAllMoves的
    src = this->pieces[sideTag + KING_FROM];
    for (i = 0; i < 4; i++) { // KING 的四个方向
        dst = src + KING_DELTA[i];
        if (!IN_FORT(dst)) continue;
        ADD_NCAP_MOVE();
    }

    // 2. 生成 ADVISOR 走法
    for (i = ADVISOR_FROM; i <= ADVISOR_TO; i++) {
        if (!(src = this->pieces[sideTag + i])) continue;
        for (j = 0; j < 4; j++) { // ADVISOR 的四个方向
            dst = src + ADVISOR_DELTA[j];
            if (!IN_FORT(dst)) continue;
            ADD_NCAP_MOVE();
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
            ADD_NCAP_MOVE();
        }
    }

    //4. 生成 KNIGHT 走法
    for (i = KNIGHT_FROM; i <= KNIGHT_TO; i++) {
        if (!(src = this->pieces[sideTag + i])) continue;
        j = 0;
        dst = knightMvDst[src][j];
        while(dst) {
            // 马腿无棋子
            if (!this->squares[knightMvPin[src][j]]) ADD_NCAP_MOVE();
            j++;
            dst = knightMvDst[src][j];
        }
    }

    // 5. 生成 ROCK 走法
    for (i = ROOK_FROM; i <= ROOK_TO; i++) {
        if (!(src = this->pieces[sideTag + i])) continue;
        for (j = 0; j < 4; j++) {
            delta = KING_DELTA[j];
            dst = src + delta; // 从起点开始，沿着方向 delta 走一步
            while (IN_BOARD(dst)) { // 终点在棋盘内
                if (this->squares[dst]) break; // 遇到棋子 停止
                ADD_NCAP_MOVE();
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
                ADD_NCAP_MOVE();
                dst += delta; // 沿着方向 delta 走一步
            }
        }
    }

    // 7. 生成 PAWN 走法
    for (i = PAWN_FROM; i <= PAWN_TO; i++) {
        if (!(src = this->pieces[sideTag + i])) continue;
        dst = SQ_FORWARED(src, this->sidePly);
        if (IN_BOARD(dst)) ADD_NCAP_MOVE();
        if (!SELF_SIDE(src, this->sidePly)) { // 过河卒
            if (IN_BOARD(dst = src - 1)) ADD_NCAP_MOVE();
            if (IN_BOARD(dst = src + 1)) ADD_NCAP_MOVE();
        }
    }
    
    // 8. 对着法排序 从吃子着法后开始
    std::sort(this->mvsGen[this->distance] + genNumSt,
              this->mvsGen[this->distance] + this->genNum[this->distance]);
}

// 添加 cap_mv 到 mvsGen 数组
#define ADD_CAP_MOVE()                                                       \
    {                                                                        \
        if (this->squares[dst] & oppSideTag) {                               \
            mvsGenPtr->mv = MOVE(src, dst);                                  \
            mvsGenPtr->vl = MVV_LVA(this->squares[dst], this->squares[src]); \
            mvsGenPtr++, this->genNum[this->distance]++;                     \
        }                                                                    \
    }

// 生成吃子着法
void Position::genCapMoves() {
    int32_t sideTag = SIDE_TAG(this->sidePly);
    int32_t oppSideTag = OPP_SIDE_TAG(this->sidePly);
    int32_t src, dst, i, j;
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
        j = 0;
        dst = knightMvDst[src][j];
        while(dst) {
            // 马腿无棋子
            if (!this->squares[knightMvPin[src][j]]) ADD_CAP_MOVE();
            j++;
            dst = knightMvDst[src][j];
        }
    }

    // 5. 生成 ROCK 吃子走法
    for (i = ROOK_FROM; i <= ROOK_TO; i++) {
        if (!(src = this->pieces[sideTag + i])) continue;
        dst = this->getRookCapY(src, false);
        ADD_CAP_MOVE();
        dst = this->getRookCapY(src, true);
        ADD_CAP_MOVE();
        dst = this->getRookCapX(src, false);
        ADD_CAP_MOVE();
        dst = this->getRookCapX(src, true);
        ADD_CAP_MOVE();
    }

    // 6. 生成 CANNON 吃子走法
    for (i = CANNON_FROM; i <= CANNON_TO; i++) {
        if (!(src = this->pieces[sideTag + i])) continue;
        dst = this->getCannonCapY(src, false);
        ADD_CAP_MOVE();
        dst = this->getCannonCapY(src, true);
        ADD_CAP_MOVE();
        dst = this->getCannonCapX(src, false);
        ADD_CAP_MOVE();
        dst = this->getCannonCapX(src, true);
        ADD_CAP_MOVE();
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
    std::sort(this->mvsGen[this->distance],
              this->mvsGen[this->distance] + this->genNum[this->distance]);
}