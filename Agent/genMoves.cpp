#include "Position.h"

// 用于判断棋子是否在棋盘上
const int32_t _IN_BOARD[256] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0,
    0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0,
    0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0,
    0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0,
    0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0,
    0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0,
    0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0,
    0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0,
    0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0,
    0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

const int32_t _IN_FORT[256] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

const int32_t KAB_LEGAL_SPAN[512] = {
                         0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 3, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 2, 1, 2, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 2, 1, 2, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 3, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0
};

const int32_t _KNIGHT_PIN[512] = {
                                0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,-0x10,  0,-0x10,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0, -1,  0,  0,  0,  1,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0, -1,  0,  0,  0,  1,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0, 0x10,  0, 0x10,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0
};

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
    
    } else { // 未被将军 才考虑从杀手着法
        this->mvKiller1[this->distance] = killerTable[this->distance][0];
        this->mvKiller2[this->distance] = killerTable[this->distance][1];
    }
}

// 得到下一个走法，无走法返回 0
int32_t Position::nextMove() {
    int32_t mv = 0;
    switch (this->phase[this->distance]) {
        // 1. 置换表启发阶段
        case PHASE::HASH:
            this->phase[this->distance] = PHASE::KILLER_1;
            mv = this->mvHash[this->distance];
            if (mv && this->isLegalMove(mv)) { // 保险起见还是加上合法着法判断
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
            this->phase[this->distance] = PHASE::GEN_CAP;
            mv = this->mvKiller2[this->distance];
            if (mv != this->mvHash[this->distance] && mv &&
                this->isLegalMove(mv)) {
                    return mv;
                }
        // 4. 生成吃子着法，完成后立即进入下一阶段；
        case PHASE::GEN_CAP:
            this->phase[this->distance] = PHASE::CAP;
            this->genCapMoves();
        case PHASE::CAP:
            while (this->curMvCnt[this->distance] + 1 < this->genNum[this->distance]) {
                this->curMvCnt[this->distance]++;
                mv = (this->mvsGen[this->distance] + this->curMvCnt[this->distance])->mv;
                if (mv != this->mvHash[this->distance] &&
                    mv != this->mvKiller1[this->distance] &&
                    mv != this->mvKiller2[this->distance])
                    return mv;
            }
            this->phase[this->distance] = PHASE::GEN_NCAP; // 无吃子走法了，进入下一阶段
        // 5. 生成非吃子着法，完成后立即进入下一阶段；
        case PHASE::GEN_NCAP:
            this->phase[this->distance] = PHASE::OTHER;
            this->genNonCapMoves(); // 历史表启发
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
        dst = SQ_FORWARD(src, this->sidePly);
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
            mvsGenPtr->vl = mvvLva(this->squares[dst], this->squares[src]); \
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
        dst = SQ_FORWARD(src, this->sidePly);
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

// 判断是否被将军 是则返回 true
int32_t Position::isChecked() {
    int32_t sideTag = SIDE_TAG(this->sidePly);
    int32_t oppSideTag = OPP_SIDE_TAG(this->sidePly);
    int32_t src, dst, i, pin;
    
    // 1. 检查对将 通过位列实现
    src = this->pieces[sideTag + KING_FROM];
    dst = this->pieces[oppSideTag + KING_FROM];
    if (!dst) return false; // 防止敌方走出非解将走法 而己方不吃掉将军
    if (this->getRookCapY(src, dst > src) == dst) return true;
    
    // 2. 检查马
    for (i = KNIGHT_FROM; i <= KNIGHT_TO; i++) {
        if ((dst = this->pieces[oppSideTag + i])) {
            pin = K_PIN(dst, src); // 注意是从 dst 出发判断
            if (pin != dst && !this->squares[pin]) return true;
        }
    }

    // 3. 检查车 通过位行、列实现
    for (i = ROOK_FROM; i <= ROOK_TO; i++) {
        dst = this->pieces[oppSideTag + i]; // 敌方 ROOK 位置
        if (SAME_X(src, dst)) { // 同一列
            if (dst == this->getRookCapY(src, dst > src)) return true;
        } else if (SAME_Y(src, dst)) { // 同一行
            if (dst == this->getRookCapX(src, dst > src)) return true;
        }
    }

    // 4. 检查炮 通过位行、列实现
    for (i = CANNON_FROM; i <= CANNON_TO; i++) {
        dst = this->pieces[oppSideTag + i]; // 敌方 CANNON 位置
        if (SAME_X(src, dst)) { // 同一列
            if (dst == this->getCannonCapY(src, dst > src)) return true;
        } else if (SAME_Y(src, dst)) { // 同一行
            if (dst == this->getCannonCapX(src, dst > src)) return true;
        }
    }

    // 5. 检查兵
    dst = SQ_FORWARD(src, this->sidePly); // 向前走一步
    if (PIECE_TYPE(this->squares[dst]) == PIECE_PAWN &&
        !(this->squares[dst] & sideTag))
        return true;
    // 两侧的兵不需要判断颜色
    if (PIECE_TYPE(this->squares[src - 1]) == PIECE_PAWN) return true;
    if (PIECE_TYPE(this->squares[src + 1]) == PIECE_PAWN) return true;
    return false;
}

// 下为着法判断部分 和着法生成写法类似

int32_t Position::isLegalMove(int32_t mv) {
    int32_t src = SRC(mv); // 获取着法起点
    int32_t sideTag = SIDE_TAG(this->sidePly);

    // 起点不是己方棋子或无子
    if (!(this->squares[src] & sideTag)) return false;

    int32_t dst = DST(mv);
    // 终点是己方棋子
    if (this->squares[dst] & sideTag) return false;

    int32_t pin(0), dirTag(0), rCapDst(0), cCapDist(0);
    switch(PIECE_TYPE(this->squares[src])) { // 判断己方棋子类型
        case PIECE_KING: // 将（帅）
            return IN_FORT(dst) && K_SPAN(src, dst);
        case PIECE_ADVISOR: // 仕（仕）
            return IN_FORT(dst) && A_SPAN(src, dst);
        case PIECE_BISHOP: // 象（相）
            return SELF_SIDE(dst, this->sidePly) && B_SPAN(src, dst) &&
                   !this->squares[B_PIN(src, dst)];
        case PIECE_KNIGHT: // 马
            pin = K_PIN(src, dst);
            return pin != src && !this->squares[pin];
        case PIECE_ROOK:
        case PIECE_CANNON: // 车和炮判断                                            一样
            if (SAME_X(src, dst)) { // 同一列
                dirTag = dst > src ? true : false;
                rCapDst = this->getRookCapY(src, dirTag);
                if (PIECE_TYPE(this->squares[src]) == PIECE_ROOK) {
                    if (rCapDst == dst || !rCapDst) return true; // 吃子走法或该方向无子
                    else if (dirTag && dst < rCapDst) return true;
                    else if (!dirTag && dst > rCapDst) return true;
                    else return false;
                } else {
                    cCapDist = this->getCannonCapY(src, dirTag);
                    if (cCapDist == dst || !rCapDst) return true; // 吃子走法或该方向无子
                    else if (dirTag && dst < rCapDst) return true;
                    else if (!dirTag && dst > rCapDst) return true;
                    else return false;
                }
            } else if (SAME_Y(src, dst)) {
                dirTag = dst > src ? true : false;
                rCapDst = this->getRookCapX(src, dirTag);
                if (PIECE_TYPE(this->squares[src]) == PIECE_ROOK) {
                    if (rCapDst == dst || !rCapDst) return true; // 吃子走法或该方向无子
                    else if (dirTag && dst < rCapDst) return true;
                    else if (!dirTag && dst > rCapDst) return true;
                    else return false;
                } else {
                    cCapDist = this->getCannonCapX(src, dirTag);
                    if (cCapDist == dst || !rCapDst) return true; // 吃子走法或该方向无子
                    else if (dirTag && dst < rCapDst) return true;
                    else if (!dirTag && dst > rCapDst) return true;
                    else return false;
                }
            } else return false; // 不在同一列同一行 不合法

        case PIECE_PAWN:
            // 已过河，可以向左右两个方向走
            if (!SELF_SIDE(src, this->sidePly) && (dst - src == 1 || src - dst == 1))
                return true;
            // 否则只能向前走
            return dst == SQ_FORWARD(src, this->sidePly);
        default: return false;
    }
    return false;
}

// 判断一个位置是否被保护 保护方为 side
int32_t Position::isProtected(int32_t side, int32_t dst, int32_t sqExcp) {
    int32_t sideTag = SIDE_TAG(side);
    int32_t src, i, pin;
    
    if (SELF_SIDE(dst, side)) {
        // 1. 受到己方将保护
        src = this->pieces[sideTag + KING_FROM];
        if (src && K_SPAN(src, dst)) return true;

        // 2. 受到己方士保护
        for (i = ADVISOR_FROM; i <= ADVISOR_TO; i++) {
            src = this->pieces[sideTag + i];
            if (src && A_SPAN(src, dst)) return true;
        }

        // 3. 受到己方象的保护
        for (i = BISHOP_FROM; i <= BISHOP_TO; i++) {
            src = this->pieces[sideTag + i];
            if (src && B_SPAN(src, dst) &&
                !this->squares[B_PIN(src, dst)])
                return true;
        }
    } else {
        src = dst - 1;
        if ((this->squares[src] & sideTag) &&
            PIECE_TYPE(this->squares[src]) == PIECE_PAWN)
            return true;
        src = dst + 1;
        if ((this->squares[src] & sideTag) &&
            PIECE_TYPE(this->squares[src]) == PIECE_PAWN)
            return true;
    }
    // 4. 受到己方马的保护
    for (i = KNIGHT_FROM; i <= KNIGHT_TO; i++) {
        if ((src = this->pieces[sideTag + i])) {
            pin = K_PIN(src, dst);
            if (pin != src && !this->squares[pin]) return true;
        }
    }

    // 3. 受到己方车的保护 通过位行、列实现
    for (i = ROOK_FROM; i <= ROOK_TO; i++) {
        src = this->pieces[sideTag + i];
        if (SAME_X(src, dst)) { // 同一列
            if (src == this->getRookCapY(dst, src > dst)) return true;
        } else if (SAME_Y(src, dst)) { // 同一行
            if (src == this->getRookCapX(dst, src > dst)) return true;
        }
    }

    // 4. 受到己方炮的保护 通过位行、列实现
    for (i = CANNON_FROM; i <= CANNON_TO; i++) {
        src = this->pieces[sideTag + i];; // 敌方 CANNON 位置
        if (SAME_X(src, dst)) { // 同一列
            if (src == this->getCannonCapY(dst, src > dst)) return true;
        } else if (SAME_Y(src, dst)) { // 同一行
            if (src == this->getCannonCapX(dst, src > dst)) return true;
        }
    }

    // 5. 受到己方兵的保护
    src = SQ_FORWARD(dst, side ^ 1); // 向后走一步，能否遇到己方兵
    if ((this->squares[src] & sideTag) &&
        PIECE_TYPE(this->squares[src]) == PIECE_PAWN)
        return true;

    return false;
}

int32_t Position::pcChased(int32_t mv) {
    int32_t src(DST(mv)); // 注意棋子已被 moved
    int32_t i, j, dst, sideTag(SIDE_TAG(this->sidePly)), cap, sameX;

    switch (PIECE_TYPE(this->squares[src])) {
        case PIECE_KNIGHT:
            j = 0, dst = knightMvDst[src][j];
            while (dst) {
                // 马腿无棋子
                if (!this->squares[knightMvPin[src][j]]) {
                    cap = this->squares[dst];
                    if (cap & sideTag) { // 被捉子
                        cap -= sideTag;
                        if (cap <= ROOK_TO) {
                            // 马捉车，仕、马、象不考虑
                            if (cap >= ROOK_FROM) return cap;
                        } else {
                            if (cap <= CANNON_TO) {
                                // 马捉无保护的炮
                                if (!this->isProtected(this->sidePly, dst)) return cap;
                            } else {
                                // 马捉过河无保护的卒
                                if (!SELF_SIDE(dst, this->sidePly) &&
                                    !this->isProtected(this->sidePly, dst))
                                    return cap;
                            }
                        }
                    }
                }
                j++, dst = knightMvDst[src][j];
            }
            break;

        case PIECE_ROOK:
            sameX = SAME_X(SRC(mv), src);
            for (i = 0; i < 2; i++) {
                // 竖向移动，判断横向捉子；横向移动，判断竖向捉子
                dst = sameX ? this->getRookCapX(src, i) : this->getRookCapY(src, i);
                cap = this->squares[dst];
                if (cap & sideTag) {
                    cap -= sideTag;
                    if (cap <= ROOK_TO) {
                        // 车捉无保护马，其余情况不考虑
                        if (KNIGHT_FROM <= cap && cap <= KNIGHT_TO &&
                            !this->isProtected(this->sidePly, dst)) {
                            return cap;
                        }
                    } else {
                        if (cap <= CANNON_TO) {
                            // 车捉无保护的炮
                            if (!this->isProtected(this->sidePly, dst)) return cap;
                        } else {
                            // 车捉过河无保护的卒
                            if (!SELF_SIDE(dst, this->sidePly) &&
                            !this->isProtected(this->sidePly, dst))
                                return cap;
                        }
                    }
                }
            }
            break;
        
        case PIECE_CANNON:
            sameX = SAME_X(SRC(mv), src);
            for (i = 0; i < 2; i++) {
                // 竖向移动，判断横向捉子；横向移动，判断竖向捉子
                dst = sameX ? this->getCannonCapX(src, i) : this->getCannonCapY(src, i);
                cap = this->squares[dst];
                if (cap & sideTag) {
                    cap -= sideTag;
                    if (cap <= ROOK_TO) {
                        // 炮捉无保护马 或 车，其余情况不考虑
                        if (cap >= KNIGHT_FROM) {
                            if (cap <= KNIGHT_TO) {
                                // 炮捉无保护马
                                if (!this->isProtected(this->sidePly, dst)) return cap;
                            } else return cap; // 炮捉车
                        }
                    } else {
                        // 炮捉无保护兵，炮捉炮不考虑
                        if (cap >= PAWN_FROM) {
                            if (!SELF_SIDE(dst, this->sidePly) &&
                            !this->isProtected(this->sidePly, dst))
                                return cap;
                        }
                    }
                }
            }
            break;
    }
    return 0;
}