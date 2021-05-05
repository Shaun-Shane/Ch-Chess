#include "search.h"
// 不使用 ucci 时注释掉 main.cpp 中也有
#define USE_UCCI

time_t searchSt;

bool debug = false;

HashTable hashTable;
// 搜索主函数
std::pair<int32_t, int32_t> searchMain() {
    auto searchSt = clock();
    int32_t bestVl, bestMv;

    bestMv = pos.zobrist->getMoveFromLib(pos.squares, pos.sidePly, startLib);
    if (bestMv && pos.isLegalMove(bestMv))
        return {0, bestMv};

    memset(historyTable, 0, sizeof(historyTable)); // 历史表清零
    memset(killerTable, 0, sizeof(killerTable));
    pos.distance = 0;

    hashTable.init();

    for (int32_t depth = 4; depth <= 32; depth++) {
        std::tie(bestVl, bestMv) = searchRoot(depth);
        #ifndef USE_UCCI
            std::cout << depth << " " << clock() - searchSt << std::endl;
        #else
            FILE* fpw = fopen("output.txt", "rt+");
            fseek(fpw, 0, 2);
            fprintf(fpw, "depth: %d, time: %lld\n", depth, (long long)(clock() - searchSt));
            fclose(fpw);
        #endif
        if (clock() - searchSt > CLOCKS_PER_SEC) {
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
    int32_t vlBest(-MATE_VALUE), vl;
    int32_t mvBest(0), mv;
    
    pos.generateMoves();
    while ((mv = pos.nextMove())) {
        if (!pos.makeMove(mv)) continue;
        // 判断能否吃掉敌方将军; 注意 makeMove 后 sidePly 变化
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
                vlBest = (vlBest == pos.drawValue()) ? vlBest - 1 : vlBest;
            }
        }
    }
    setHistory(mvBest, depth), setKillerTable(mvBest);
    return {vlBest, mvBest}; // 返回最佳分值与走法
}

int32_t searchFull(int32_t depth, int32_t alpha, int32_t beta, bool noNull) {
    if (depth <= 0) return searchQuiescence(alpha, beta);

    // 检查重复局面
    int32_t vl = pos.repStatus();
    if (vl) return pos.repValue(vl);

    // 置换表裁剪，最佳走法保存在 mvHash[pos.distance]
    vl = hashTable.probeHash(alpha, beta, depth);
    if (vl > -MATE_VALUE) return vl;
    
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

    int32_t vlBest(-MATE_VALUE);
    int32_t mvBest(0), mv, hashFlag = HASH_ALPHA;

    pos.generateMoves(pos.mvHash[pos.distance]);
    while ((mv = pos.nextMove())) {
       if (!pos.makeMove(mv)) continue;

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
            if (vl >= beta) {
                hashFlag = HASH_BETA;
                break;
            }
            if (vl > alpha) {
                hashFlag = HASH_EXACT;
                alpha = vl;
            }
        }
    }

    // 所有走法都走完了，找不到合法着法，输棋
    if (vlBest == -MATE_VALUE) return pos.mateValue();

    hashTable.recordHash(hashFlag, vlBest, depth, mvBest);
    if (vlBest > 0) setHistory(mvBest, depth), setKillerTable(mvBest);

    return vlBest;
}

int32_t searchQuiescence(int32_t alpha, int32_t beta) {
    int32_t vlBest(-MATE_VALUE), vl, ischecked, mv;

    // 1. beta 值比杀棋分数还小，直接返回杀气分数
    vl = pos.mateValue();
    if (vl >= beta) return vl;

    // 检测重复局面...
    vl = pos.repStatus();
    if (vl) return pos.repValue(vl);
 
    // 2. 达到极限深度 QUIESC_LIMIT 返回估值
    if (pos.distance == QUIESC_LIMIT) return evaluate();

    // 3. 生成着法
    if ((ischecked = pos.isChecked())) { // 被将军 生成全部着法
        pos.resetMvKillerHash();
        pos.genAllMoves(); 
    } else { // 不被将军，先进行局面评估是否能截断
        vl = evaluate();
        if (vl > vlBest) {
            if (vl >= beta) return vl;
            vlBest = vl;
            if (vl > alpha) alpha = vl;
        }
        if (!ischecked) { // 将军未被威胁 仅生成吃子着法
            pos.resetMvKillerHash();
            // if (debug) std::cout << pos.distance << std::endl;
            pos.genCapMoves();
        }
    }

    pos.phase[pos.distance] = PHASE::OTHER; // 不进行其它启发
    while ((mv = pos.nextMove())) {
        if (!pos.makeMove(mv)) continue;
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

void HashTable::init() {
    for (int i = 0; i <= this->hashMask; i++) {
        auto& item = this->hashTable[i];
        item.depth = 0, item.flag = HASH_EMPTY;
        item.mv = 0, item.vl = 0, item.zobristLock = 0;
    }
}

int32_t HashTable::probeHash(int32_t alpha, int32_t beta, int32_t depth) {
    auto& item = this->hashTable[pos.zobrist->getCurKey() & this->hashMask];

    if (item.flag == HASH_EMPTY ||
        item.zobristLock != pos.zobrist->getCurLock()) {
        pos.mvHash[pos.distance] = 0;
        return -MATE_VALUE;
    }

    pos.mvHash[pos.distance] = item.mv; // 置换表最佳着法
    bool isMate = false; // 是否为杀棋

    // 如果是杀棋，返回与深度相关的杀棋分数。如果是长将或者和棋，返回-MATE_VALUE。
    if (item.vl > WIN_VALUE) {
        if (item.vl <= BAN_VALUE) return -MATE_VALUE;
        item.vl -= pos.distance;
        isMate = true;
    } else if (item.vl < -WIN_VALUE) {
        if (item.vl >= -BAN_VALUE) return -MATE_VALUE;
        item.vl += pos.distance;
        isMate = true;
    } else if (item.vl == pos.drawValue()) {
        return -MATE_VALUE;
    }

    // 如果置换表中节点的搜索深度小于当前节点，查询失败
    if (item.depth < depth && !isMate) return -MATE_VALUE;

    if (item.flag == HASH_BETA) {
        if (item.vl >= beta) return item.vl;
        else return -MATE_VALUE;
    }

    if (item.flag == HASH_ALPHA) {
        if (item.vl <= alpha) return item.vl;
        else return -MATE_VALUE;
    }

    return item.vl;
}

void HashTable::recordHash(int32_t flag, int32_t vl, int32_t depth, int32_t mv) {
    auto& item = this->hashTable[pos.zobrist->getCurKey() & this->hashMask];

    if (depth < item.depth) return; // 深度优先

    item.flag = flag, item.depth = depth;

    // 长将、和棋没有最佳着法，不记录
    if (vl > WIN_VALUE) { // 杀棋
        if (!mv && vl <= BAN_VALUE) return; // 长将
        item.vl = vl + pos.distance;
    } else if (vl < -WIN_VALUE) { // 杀棋
        if (!mv && vl >= -BAN_VALUE) return; // 长将
        item.vl = vl - pos.distance;
    } else if (!mv && vl == pos.drawValue()) {
        return;
    } else {
        item.vl = vl;
    }
    item.mv = mv;
    item.zobristLock = pos.zobrist->getCurLock();
}