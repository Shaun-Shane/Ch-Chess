#include "Zobrist.h"

Zobrist zob;

int binarySearchLib(const Library lib[], unsigned int lock, int n) {
    int l = 0, r = n - 1;
    while (l <= r) {
        int mid = (l + r) / 2;
        if (lib[mid].situation > lock)
            r = mid - 1;
        else {
            if (lib[mid].situation < lock)
                l = mid + 1;
            else
                return mid;
        }
    }
    return -1;
}

int mirrorPos(int sq) {
    int x, y;
    x = 14 - GET_X(sq);
    y = GET_Y(sq);
    int newSq = COORD_XY(x, y);
    return newSq;
}

int mirrorMov(int mv) {
    int src, dst;
    src = SRC(mv);
    dst = DST(mv);
    src = mirrorPos(src);
    dst = mirrorPos(dst);
    int newMove = MOVE(src, dst);
    return newMove;
}

bool comp(const Library &l1, const Library &l2) {
    return l1.moveValue > l2.moveValue;
}

Zobrist::Zobrist() {
    curZobristKey = 0;
    curZobristLock = 0;
}

void Zobrist::initZero() {
    curZobristKey = 0;
    curZobristLock = 0;
}

unsigned int Zobrist::getCurKey() { return this->curZobristKey; }
unsigned int Zobrist::getCurLock() { return this->curZobristLock; }

void Zobrist::zobristUpdateChangeSide() {
    this->curZobristKey ^= zobristKeyPlayer;
    this->curZobristLock ^= zobristLockPlayer;
}

void Zobrist::zobristUpdateMove(int sq, int pc) {
    if (!pc) return;
    int pcCopy = pc;
    if (pcCopy & 16) {
        pcCopy = PIECE_TYPE(pcCopy);
    } else {
        pcCopy = PIECE_TYPE(pcCopy) + PIECE_EMPTY;
    }
    this->curZobristKey ^= zobristKeyTable[pcCopy][sq];
    this->curZobristLock ^= zobristLockTable[pcCopy][sq];
}

Zobrist::Zobrist(int squares[], int sidePlayer, bool mirror) {
    this->curZobristKey = 0;
    this->curZobristLock = 0;
    int i;
    for (i = 0; i < 256; i++) {
        int pc = squares[i];
        if (!pc) continue;
        if (mirror)
            this->zobristUpdateMove(mirrorPos(i), pc);
        else
            this->zobristUpdateMove(i, pc);
    }
    if (sidePlayer == 1) this->zobristUpdateChangeSide();
}

int Zobrist::getMoveFromLib(int squares[], int sidePlayer,
                            const Library lib[]) {
    bool mirrorFlag = 0;
    unsigned int lockValue = 0;
    //先搜索当前局面
    int id = binarySearchLib(lib, curZobristLock >> 1, START_LIBRARY_SIZE);
    lockValue = curZobristLock >> 1;
    if (id == -1) {
        //找不到再搜索当前局面对应的左右对称的局面
        mirrorFlag = 1;
        Zobrist zoTemp(squares, sidePlayer, 1);
        id = binarySearchLib(lib, zoTemp.curZobristLock >> 1,
                             START_LIBRARY_SIZE);
        // std::cout << (zoTemp.curZobristLock) << '\n';
        lockValue = zoTemp.curZobristLock >> 1;
    }
    //都找不到返回0
    if (id == -1) return 0;
    //找出所有该局面下可能的走法
    while (id >= 0 && lib[id].situation == lockValue) id--;
    id++;
    std::vector<Library> libVector;
    while (id < START_LIBRARY_SIZE && lib[id].situation == lockValue) {
        Library libTemp;
        libTemp.situation = lib[id].situation;
        //如果是从对称局面中找到的走法也要将其对称处理
        libTemp.moveInfo =
            mirrorFlag ? mirrorMov(lib[id].moveInfo) : lib[id].moveInfo;
        libTemp.moveValue = lib[id].moveValue;
        libVector.push_back(libTemp);
        id++;
    }
    //将所有走法按估值排序
    std::sort(libVector.begin(), libVector.end(), comp);
    //返回值最大的走法
    return libVector.front().moveInfo;
}