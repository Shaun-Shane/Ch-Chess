#include "search.h"

time_t searchSt;

// 搜索主函数
std::pair<int32_t, int32_t> searchMain() {
    memset(historyTable, 0, sizeof(historyTable)); // 历史表清零
    auto searchSt = clock();
    int32_t bestVl;
    int32_t bestMv;
    int mv = 0;
    mv = pos.zobrist->getMoveFromLib(pos.squares, pos.sidePly, startLib);
    if (mv && pos.isLegalMove(mv))
        return {0, mv};
    for (int32_t depth = 3; depth <= 32; depth++) {
       
        std::tie(bestVl, bestMv) = searchRoot(depth);
        #ifndef USE_UCCI
            std::cout << clock() - searchSt << std::endl;
        #endif
        if (clock() - searchSt > CLOCKS_PER_SEC / 3) {
           // std::cout << "depth: " << depth << std::endl;
            break;
        }
        if (bestVl > WIN_VALUE || bestVl < -WIN_VALUE) {
           // std::cout << "depth: " << depth << std::endl;
            break;
        }
    }
    return {bestVl, bestMv};
}

std::pair<int32_t, int32_t> searchRoot(int32_t depth) {
    int32_t vlBest(-MATE_VALUE), vl(0);
    int32_t mvBest(0), mv;
    pos.genAllMoves();
    while ((mv = pos.nextMove())) {
        if (!pos.makeMove()) continue;

        // 判断能否吃掉敌方棋子; 注意 makeMove 后 sidePly 变化
        if (!pos.pieces[SIDE_TAG(pos.sidePly) + KING_FROM]) {
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

int32_t searchFull(int32_t depth, int32_t alpha, int32_t beta, bool noNull) {
    if (depth <= 0) return searchQuiescence(alpha, beta);
    
    int32_t vlBest(-MATE_VALUE), vl;
    int32_t mvBest(0), mv;

    // 空着裁剪
    if (!noNull && pos.nullOkay() && !pos.isChecked()){
        pos.makeNullMove();
        vl = -searchFull(depth - NULL_DEPTH - 1, -beta, -beta + 1, true);
        pos.undoMakeNullMove();
        if (vl >= beta) { // 存在截断
            if (pos.nullSafe()) return vl;
            if (searchFull(depth - NULL_DEPTH, alpha, beta, true) >= beta)
                return vl;
        }
    }

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
            if (vl > alpha) alpha = vl;
        }
    }

    // 所有走法都走完了，找不到合法着法，输棋
    if (vlBest == -MATE_VALUE) return pos.mateValue();

    if (vlBest > 0) setHistory(mvBest, depth);

    return vlBest;
}

int32_t searchQuiescence(int32_t alpha, int32_t beta) {
    int32_t vlBest(-MATE_VALUE), vl, ischecked;

    // 1. beta 值比杀棋分数还小，直接返回杀气分数
    vl = pos.mateValue();
    if (vl >= beta) return vl;

    // 检测重复局面...
 
    // 2. 达到极限深度 QUIESC_LIMIT 返回估值
    if (pos.distance == QUIESC_LIMIT) return evaluate();

    // 3. 生成着法
    if ((ischecked = pos.isChecked())) pos.genAllMoves(); // 被将军 生成全部着法
    else { // 不被将军，先进行局面评估是否能截断
        vl = evaluate();
        if (vl > vlBest) {
            if (vl >= beta) return vl;
            vlBest = vl;
            if (vl > alpha) alpha = vl;
        }
        if (!ischecked) pos.genCapMoves(); // 将军未被威胁 仅生成吃子着法
    }

    while (pos.nextMove()) {
        if (!pos.makeMove()) continue;
        vl = -searchQuiescence(-beta, -alpha);
        pos.undoMakeMove();

        if (vl > vlBest) {
            if (vl >= beta) return vl;
            vlBest = vl;
            if (vl > alpha) alpha = vl;
        }
    }
    if (vlBest == -MATE_VALUE) return pos.mateValue();
    return vlBest;
}