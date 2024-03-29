#include "Position.h"

/* 棋子类型-位置价值表
 * 获取棋子类型见 PIECE_TYPE(pc) 函数
 * 注意黑方在下 初始 sq 大
 */ 
const int32_t SQ_VALUE[PIECE_EMPTY + 1][256] = {
    { // 将
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
        0,  0,  0,  0,  0,  0,  1,  1,  1,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  2,  2,  2,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0, 11, 15, 11,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
    }, {    // 仕
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
        0,  0,  0,  0,  0,  0, 20,  0, 20,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0, 23,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0, 20,  0, 20,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
    }, { // 相
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0, 20,  0,  0,  0, 20,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0, 18,  0,  0,  0, 23,  0,  0,  0, 18,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0, 20,  0,  0,  0, 20,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
    }, { // 马
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0, 90, 90, 90, 96, 90, 96, 90, 90, 90,  0,  0,  0,  0,
        0,  0,  0, 90, 96,103, 97, 94, 97,103, 96, 90,  0,  0,  0,  0,
        0,  0,  0, 92, 98, 99,103, 99,103, 99, 98, 92,  0,  0,  0,  0,
        0,  0,  0, 93,108,100,107,100,107,100,108, 93,  0,  0,  0,  0,
        0,  0,  0, 90,100, 99,103,104,103, 99,100, 90,  0,  0,  0,  0,
        0,  0,  0, 90, 98,101,102,103,102,101, 98, 90,  0,  0,  0,  0,
        0,  0,  0, 92, 94, 98, 95, 98, 95, 98, 94, 92,  0,  0,  0,  0,
        0,  0,  0, 93, 92, 94, 95, 92, 95, 94, 92, 93,  0,  0,  0,  0,
        0,  0,  0, 85, 90, 92, 93, 78, 93, 92, 90, 85,  0,  0,  0,  0,
        0,  0,  0, 88, 85, 90, 88, 90, 88, 90, 85, 88,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
    }, { // 车
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,206,208,207,213,214,213,207,208,206,  0,  0,  0,  0,
        0,  0,  0,206,212,209,216,233,216,209,212,206,  0,  0,  0,  0,
        0,  0,  0,206,208,207,214,216,214,207,208,206,  0,  0,  0,  0,
        0,  0,  0,206,213,213,216,216,216,213,213,206,  0,  0,  0,  0,
        0,  0,  0,208,211,211,214,215,214,211,211,208,  0,  0,  0,  0,
        0,  0,  0,208,212,212,214,215,214,212,212,208,  0,  0,  0,  0,
        0,  0,  0,204,209,204,212,214,212,204,209,204,  0,  0,  0,  0,
        0,  0,  0,198,208,204,212,212,212,204,208,198,  0,  0,  0,  0,
        0,  0,  0,200,208,206,212,200,212,206,208,200,  0,  0,  0,  0,
        0,  0,  0,194,206,204,212,200,212,204,206,194,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
    }, { // 炮
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,100,100, 96, 91, 90, 91, 96,100,100,  0,  0,  0,  0,
        0,  0,  0, 98, 98, 96, 92, 89, 92, 96, 98, 98,  0,  0,  0,  0,
        0,  0,  0, 97, 97, 96, 91, 92, 91, 96, 97, 97,  0,  0,  0,  0,
        0,  0,  0, 96, 99, 99, 98,100, 98, 99, 99, 96,  0,  0,  0,  0,
        0,  0,  0, 96, 96, 96, 96,100, 96, 96, 96, 96,  0,  0,  0,  0,
        0,  0,  0, 95, 96, 99, 96,100, 96, 99, 96, 95,  0,  0,  0,  0,
        0,  0,  0, 96, 96, 96, 96, 96, 96, 96, 96, 96,  0,  0,  0,  0,
        0,  0,  0, 97, 96,100, 99,101, 99,100, 96, 97,  0,  0,  0,  0,
        0,  0,  0, 96, 97, 98, 98, 98, 98, 98, 97, 96,  0,  0,  0,  0,
        0,  0,  0, 96, 96, 97, 99, 99, 99, 97, 96, 96,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
    }, { // 兵
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  9,  9,  9, 11, 13, 11,  9,  9,  9,  0,  0,  0,  0,
        0,  0,  0, 19, 24, 34, 42, 44, 42, 34, 24, 19,  0,  0,  0,  0,
        0,  0,  0, 19, 24, 32, 37, 37, 37, 32, 24, 19,  0,  0,  0,  0,
        0,  0,  0, 19, 23, 27, 29, 30, 29, 27, 23, 19,  0,  0,  0,  0,
        0,  0,  0, 14, 18, 20, 27, 29, 27, 20, 18, 14,  0,  0,  0,  0,
        0,  0,  0,  7,  0, 13,  0, 16,  0, 13,  0,  7,  0,  0,  0,  0,
        0,  0,  0,  7,  0,  7,  0, 15,  0,  7,  0,  7,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
    }, { // 空
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
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
    }
};
// 空头炮威胁分值，行号 0-16
const int HOLLOW_THREAT[16] = {
    0,  0,  0,  0,  0,  0, 60, 65, 70, 75, 80, 80, 80,  0,  0,  0
};
// 炮镇窝心马的威胁分值，行号 0-16 或一般中炮威胁
const int32_t CENTRAL_THREAT[16] = {
    0,  0,  0,  0,  0,  0, 50, 45, 40, 35, 30, 30, 30,  0,  0,  0
};
//沉底炮的威胁分值 列号 0-16
const int32_t BOTTOM_THREAT[16] = {
    0,  0,  0, 40, 30,  0,  0,  0,  0,  0, 30, 40,  0,  0,  0,  0
};
// 缺仕的分值
constexpr int32_t ADVISOR_LEAKAGE = 40; 

int32_t Position::adviShape() {
    int32_t redPenalty(0), blackPenalty(0);
    int32_t src, dst(pieces[SIDE_TAG(0) + KING_FROM]), i;
    if (pieces[SIDE_TAG(0) + ADVISOR_FROM] && pieces[SIDE_TAG(0) + ADVISOR_TO]) {
        if (dst == 0x37) { // 将帅在原位
            if (PIECE_TYPE(squares[0x36]) == PIECE_ADVISOR &&
                PIECE_TYPE(squares[0x38]) == PIECE_ADVISOR) { // 底线双仕
                    for (i = SIDE_TAG(1) + CANNON_FROM; i <= SIDE_TAG(1) + CANNON_TO; i++) {
                        if (!(src = pieces[i])) continue;// 敌方炮位置
                        if (SAME_X(src, dst)) { // 炮与将在同一列
                            if (getRookCapY(src, dst > src) == dst)
                                redPenalty += HOLLOW_THREAT[GET_Y(pieces[i])];  // 空头炮
                            else if (getCannonSupperCapY(src, dst > src) == dst &&
                                     (squares[0x47] == 21 ||
                                      squares[0x47] == 22))
                                redPenalty += CENTRAL_THREAT[GET_Y(pieces[i])];  // 炮镇窝心马的威胁
                        }
                    }
            } else if (PIECE_TYPE(squares[0x47]) == PIECE_ADVISOR &&
                       (PIECE_TYPE(squares[0x36]) == PIECE_ADVISOR ||
                        PIECE_TYPE(squares[0x38]) == PIECE_ADVISOR)) { // 花心+底线仕
                    for (i = SIDE_TAG(1) + CANNON_FROM; i <= SIDE_TAG(1) + CANNON_TO; i++) {
                        if (!(src = pieces[i])) continue;// 敌方炮位置
                        if (SAME_X(src, dst)) {
                            if (getCannonSupperCapY(src, dst > src) == dst) {// 计算普通中炮威胁
                                redPenalty += (CENTRAL_THREAT[GET_Y(pieces[i])] >> 1);
                                // 将门被控制 扣分
                                if (!squares[0x36] && isProtected(1, 0x36)) redPenalty += 20;
                                else if (!squares[0x38] && isProtected(1, 0x38)) redPenalty += 20;
                                // 如果车在底线保护将帅，扣分
                                for (int32_t j = SIDE_TAG(0) + ROOK_FROM; j <= SIDE_TAG(0) + ROOK_TO; j++) {
                                    if (!(src = pieces[j])) continue; // 己方车位置
                                    
                                    if (SAME_Y(src, dst) &&
                                        getRookCapX(src, dst > src) == dst) {
                                        redPenalty += 80;
                                    }
                                }
                            }
                        } else if (SAME_Y(src, dst)) { // 计算沉底炮威胁
                            if (getRookCapX(src, dst > src) == dst)
                                redPenalty += BOTTOM_THREAT[GET_X(pieces[i])];
                        }
                    }
            }
        } else if (dst == 0x47) { redPenalty += 20; } // 帅在花心，影响仕的行动
    } else if (pieces[SIDE_TAG(1) + ROOK_FROM] && pieces[SIDE_TAG(1) + ROOK_TO]) {
        redPenalty += ADVISOR_LEAKAGE; // 缺仕怕双车
    }

    dst = pieces[SIDE_TAG(1) + KING_FROM]; // 获得黑方帅位置
    if (pieces[SIDE_TAG(1) + ADVISOR_FROM] && pieces[SIDE_TAG(1) + ADVISOR_TO]) {
        if (dst == 0xc7) { // 将帅在原位
            if (PIECE_TYPE(squares[0xc6]) == PIECE_ADVISOR &&
                PIECE_TYPE(squares[0xc8]) == PIECE_ADVISOR) { // 底线双仕
                    for (i = SIDE_TAG(0) + CANNON_FROM; i <= SIDE_TAG(0) + CANNON_TO; i++) {
                        if (!(src = pieces[i])) continue;// 敌方炮位置
                        if (SAME_X(src, dst)) { // 炮与将在同一列
                            if (getRookCapY(src, dst > src) == dst)
                                blackPenalty += HOLLOW_THREAT[15 - GET_Y(pieces[i])];  // 空头炮
                            else if (getCannonSupperCapY(src, dst > src) == dst &&
                                     (squares[0xb7] == 37 ||
                                      squares[0xb7] == 38))
                                blackPenalty += CENTRAL_THREAT[15 - GET_Y(pieces[i])];  // 炮镇窝心马的威胁
                        }
                    }
            } else if (PIECE_TYPE(squares[0xb7]) == PIECE_ADVISOR &&
                       (PIECE_TYPE(squares[0xc6]) == PIECE_ADVISOR ||
                        PIECE_TYPE(squares[0xc8]) == PIECE_ADVISOR)) { // 花心+底线仕
                    for (i = SIDE_TAG(0) + CANNON_FROM; i <= SIDE_TAG(0) + CANNON_TO; i++) {
                        if (!(src = pieces[i])) continue;// 敌方炮位置
                        if (SAME_X(src, dst)) {
                            if (getCannonSupperCapY(src, dst > src) == dst) { // 计算普通中炮威胁
                                blackPenalty += (CENTRAL_THREAT[15 - GET_Y(pieces[i])] >> 1);
                                // 将门被控制 扣分
                                if (!squares[0xc6] && isProtected(0, 0xc6)) blackPenalty += 20;
                                else if (!squares[0xc8] && isProtected(0, 0xc8)) blackPenalty += 20;
                                // 如果车在底线保护将帅，扣分
                                for (int32_t j = SIDE_TAG(1) + ROOK_FROM; j <= SIDE_TAG(1) + ROOK_TO; j++) {
                                    if (!(src = pieces[j])) continue; // 己方车位置
                                    
                                    if (SAME_Y(src, dst) &&
                                        getRookCapX(src, dst > src) == dst) {
                                        blackPenalty += 80;
                                    }
                                }
                            }
                        } else if (SAME_Y(src, dst)) { // 计算沉底炮威胁
                            if (getRookCapX(src, dst > src) == dst)
                                blackPenalty += BOTTOM_THREAT[GET_X(pieces[i])];
                        }
                    }
            }
        } else if (dst == 0xb7) { blackPenalty += 20; } // 帅在花心，影响仕的行动
    } else if (pieces[SIDE_TAG(0) + ROOK_FROM] && pieces[SIDE_TAG(0) + ROOK_TO]) {
        blackPenalty += ADVISOR_LEAKAGE; // 缺仕怕双车
    }

    return -redPenalty + blackPenalty;
}

int32_t Position::rookMobility() {
    int32_t side, i, sideTag, src, rookMob[2], x, y;
    for (side = 0; side < 2; side++) {
        rookMob[side] = 0, sideTag = SIDE_TAG(side);
        for (i = sideTag + ROOK_FROM; i <= sideTag + ROOK_TO; i++) {
            if (!(src = pieces[i])) continue;
            // 最右非吃子
            x = rookCapX[GET_X(src) - X_FROM][stateY[GET_Y(src)]][1];
            if (~x) {
                if (!(squares[COORD_XY(x, GET_Y(src))] & sideTag)) rookMob[side]++;
                x--;
            } else x = X_TO;
            rookMob[side] += x - GET_X(src);

            // 最左非吃子
            x = rookCapX[GET_X(src) - X_FROM][stateY[GET_Y(src)]][0];
            if (~x) {
                if (!(squares[COORD_XY(x, GET_Y(src))] & sideTag)) rookMob[side]++;
                x++;
            } else x = X_FROM;
            rookMob[side] += GET_X(src)- x;

            // 最上(sq大)非吃子
            y = rookCapY[GET_Y(src) - Y_FROM][this->stateX[GET_X(src)]][1];
            if (~y) {
                if (!(squares[COORD_XY(GET_X(src), y)] & sideTag)) rookMob[side]++;
                y--;
            } else y = Y_TO;
            rookMob[side] += y - GET_Y(src);

            // 最下(sq小)非吃子
            y = rookCapY[GET_Y(src) - Y_FROM][this->stateX[GET_X(src)]][0];
            if (~y) {
                if (!(squares[COORD_XY(GET_X(src), y)] & sideTag)) rookMob[side]++;
                y++;
            } else y = Y_FROM;
            rookMob[side] += GET_Y(src) - y;
        }
    }
    return rookMob[0] - rookMob[1];
}

// 不利于马的位置
const int32_t KNIGHT_BAD_SQUARES[256] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0,
    0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0,
    0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0,
    0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0,
    0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0,
    0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0,
    0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0,
    0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0,
    0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0,
    0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

int32_t Position::knightBlock() {
    int32_t side, i, j, sideTag, src, dst, knightPenalty[2], cnt;

    for (side = 0; side < 2; side++) {
        knightPenalty[side] = 0, sideTag = SIDE_TAG(side);
        for (i = sideTag + KNIGHT_FROM; i <= sideTag + KNIGHT_TO; i++) {
            if (!(src = pieces[i])) continue;
            cnt = j = 0;
            dst = knightMvDst[src][j];
            while(dst) {
                if (!this->squares[knightMvPin[src][j]] &&
                    !KNIGHT_BAD_SQUARES[dst] && !(squares[dst] & sideTag) &&
                    !isProtected(side ^ 1, dst)) {
                    if ((++cnt) > 1) break;
                }
                j++;
                dst = knightMvDst[src][j];
            }
            if (cnt == 0) knightPenalty[side] += 10; // 没有好走法 罚分
            else if (cnt == 1) knightPenalty[side] += 5; // 只有一个好走法 也罚分
        }
    }
    return -knightPenalty[0] + knightPenalty[1];
}

int32_t Position::evaluate() {
    int32_t vl = vlRed - vlBlack;

    // 对特殊棋形评价
    vl += adviShape();
    // 车的灵活性评价
    vl += rookMobility();
    // 马的阻碍评价
    vl += knightBlock();

    if (sidePly) vl = -vl + 3;
    else vl = vl + 3;

    // 与和棋分数区分开来
    if (vl == drawVal()) vl = vl - 1;

    return vl;
}