#include "search.h"
#include "Position.h"

// 搜索主函数
std::pair<int_fast16_t, int_fast16_t> searchMain() {
    memset(historyTable, 0, sizeof(historyTable)); // 历史表清零
    return searchRoot(6);
}

std::pair<int_fast16_t, int_fast16_t> searchRoot(int_fast16_t depth) {
    int_fast16_t vlBest(-MATE_VALUE), mvBest(0), mv, vl;
    
    pos.genAllMoves();
    while ((mv = pos.nextMove())) {
        if (!pos.makeMove()) continue;

        // 判断能否吃掉敌方棋子; 注意 makeMove 后 sidePly 变化
        if (!pos.pieces[SIDE_TAG(pos.sidePly) + KING_FROM]) { 
            std::cout << "mate" << std::endl;
            pos.undoMakeMove();
            return {MATE_VALUE, mv};
        }

        // PVS
        if (vlBest == -MATE_VALUE) { // 还未找到 PV
            vl = -searchFull(depth - 1, -MATE_VALUE, MATE_VALUE);
        } else {
            vl = -searchFull(depth - 1, -vlBest - 1, -vlBest);
            if (vl > vlBest) {
                vl = -searchFull(depth - 1, -MATE_VALUE, -vlBest);
            }
        }

        pos.undoMakeMove();

        if (vl > vlBest) {
            vlBest = vl, mvBest = mv;
            if (vlBest > -WIN_VALUE && vlBest < WIN_VALUE) { // 增加走法随机性
                vlBest += ((rand() - RAND_MAX) % 7);
                // ...
            }
        }
    }
    return {vlBest, mvBest}; // 返回最佳分值与走法
}

int_fast16_t searchFull(int_fast16_t depth, int_fast16_t alpha, int_fast16_t beta) {
    if (depth <= 0) return evalute();
    
    int_fast16_t vlBest(-MATE_VALUE), mvBest(0), mv, vl;

    pos.genAllMoves();
    while ((mv = pos.nextMove())) {
       if (!pos.makeMove()) continue;

        // PVS
        if (vlBest == -MATE_VALUE) {
            vl = -searchFull(depth - 1, -beta, -alpha);
        } else {
            vl = -searchFull(depth - 1, -alpha - 1, -alpha);
            if (vl > alpha && vl < beta)
                vl = -searchFull(depth - 1, -beta, -alpha);
        }

        pos.undoMakeMove();

        if (vl > vlBest) {
            vlBest = vl, mvBest = mv;
            if (vl >= beta) break;
        }

        if (vl > alpha) alpha = vl;
    }

    // 所有走法都走完了，找不到合法着法，输棋
    if (vlBest == -MATE_VALUE) return pos.mateValue();

    if (vlBest > 0) setHistory(mvBest, depth);

    return vlBest;
}