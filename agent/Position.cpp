#ifndef POSITION_CPP
#define POSITION_CPP

#include "Position.h"
#ifdef POS_DEBUG
#include "windows.h"
#endif

// 用于判断棋子是否在棋盘上
const int_fast32_t _IN_BOARD[256] = {
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

const int_fast32_t _IN_FORT[256] = {
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

/* 棋子序号对应的棋子类型
 *
 * 棋子序号从0到47，其中0到15不用，16到31表示红子，32到47表示黑子。
 * 帅仕仕相相马马车车炮炮兵兵兵兵兵(将士士象象马马车车炮炮卒卒卒卒卒)
 * 判断棋子是红子 "pc < 32"，黑子 "pc >= 32"
 */
const int_fast32_t pieceTypes[48] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6, 6, 6, 6,
    0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6, 6, 6, 6
};

// 棋子的简单分值，只在简单比较时作参考
const int_fast32_t simpleValues[48] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    5, 1, 1, 1, 1, 3, 3, 4, 4, 3, 3, 2, 2, 2, 2, 2,
    5, 1, 1, 1, 1, 3, 3, 4, 4, 3, 3, 2, 2, 2, 2, 2
};

// 棋子走法
const int_fast32_t KING_DELTA[4] = {-16, -1, 1, 16};
const int_fast32_t ADVISOR_DELTA[4] = {-17, -15, 15, 17};
const int_fast32_t KNIGHT_DELTA[4][2] = {{-33, -31}, {-18, 14}, {-14, 18}, {31, 33}};

// 将FEN串中棋子标识转化为对应棋子类型 需toupper转化为大写
int_fast32_t charToPt(char c) {
    switch (c) {
        case 'K':
            return PIECE_KING;
        case 'A':
            return PIECE_ADVISOR;
        case 'B':
            return PIECE_BISHOP;
        case 'N':
            return PIECE_KNIGHT;
        case 'R':
            return PIECE_ROOK;
        case 'C':
            return PIECE_CANNON;
        case 'P':
            return PIECE_PAWN;
        default:
            return PIECE_EMPTY;
    }
}

// 将棋子转化为 FEN 字符 返回大写字符
char ptToChar(int_fast32_t pt) {
    switch (pt) {
        case PIECE_KING:
            return 'K';
        case PIECE_ADVISOR:
            return 'A';
        case PIECE_BISHOP:
            return 'B';
        case PIECE_KNIGHT:
            return 'N';
        case PIECE_ROOK:
            return 'R';
        case PIECE_CANNON:
            return 'C';
        case PIECE_PAWN:
            return 'P';
        default:
            return 0;
    }
}

// 初始化
void Position::clear() {
    memset(this->squares, 0, sizeof(this->squares));
    memset(this->pieces, 0, sizeof(this->pieces));
    this->sidePly = 0; // 默认红方 可通过 changeSide() 修改
    this->vlRed = this->vlBlack = 0;
    this->moveNum = 0, this->distance = 0;
}

// 将棋子 pc 添加进棋局中的 sq 位置
void Position::addPiece(int_fast32_t sq, int_fast32_t pc, bool del) {
    if (del) this->squares[sq] = 0, this->pieces[pc] = 0;
    else this->squares[sq] = pc, this->pieces[pc] = sq;
    // this->zobr.Xor(PreGen.zobrTable[pt][sq]);
}

// 交换走子方
void Position::changeSide() { this->sidePly = OPP_SIDE(this->sidePly); }

// 保存状态
void Position::saveStatus() {
    auto rbObjPtr = this->rollBackList + this->moveNum;
    rbObjPtr->vlRed = this->vlRed;
    rbObjPtr->vlBlack = this->vlBlack;
}

void Position::rollBack() {
    auto rbObjPtr = this->rollBackList + this->moveNum;
    this->vlRed = rbObjPtr->vlRed;
    this->vlBlack = rbObjPtr->vlBlack;
}

// 通过FEN串初始化棋局
void Position::fromFen(string fen) {
    this->clear();
    // ...
}

// 根据 mvStr 字符串移动棋子
void Position::movePiece(string mvStr) {
#ifdef POS_DEBUG
    if (mvStr.size() != 4) {
        cout << "wrong move input!" << endl;
        cout.flush();
        return;
    }
#endif

    int_fast32_t preY = mvStr[1] - '0', preX = mvStr[0] - 'a';
    int_fast32_t toY = mvStr[3] - '0', toX = mvStr[2] - 'a';

#ifdef POS_DEBUG
    if (!(0 <= preY && preY <= 9 && 0 <= preX && preX <= 8)) {
        cerr << "wrong move input!" << endl;
        cout.flush();
        return;
    }

    if (!(0 <= toY && toY <= 9 && 0 <= toX && toX <= 8)) {
        cerr << "wrong move input!" << endl;
        cout.flush();
        return;
    }
#endif

    int_fast32_t src = COORD_XY(preX + X_FROM, preY + Y_FROM);
    int_fast32_t dst = COORD_XY(toX + X_FROM, toY + Y_FROM);

#ifdef POS_DEBUG
    if (!this->squares[src] ||
        (this->squares[dst] &&
         !((this->squares[src] ^ this->squares[dst]) & 16))) {
        cerr << "wrong move input!" << endl;
        cout.flush();
        return;
    }
#endif
    int_fast32_t pc = this->squares[src]; // 起点棋子
    this->addPiece(dst, this->squares[dst], DEL_PIECE); // 删除终点棋子
    this->addPiece(src, this->squares[src], DEL_PIECE); // 删除起点棋子
    this->addPiece(dst, pc); // 在终点添加起点棋子

#ifdef POS_DEBUG
    this->debug();
#endif
}

// 根据整型 mv 移动棋子；mv 见 MOVE() 函数
void Position::movePiece(int_fast32_t mv) {
    int_fast32_t src = SRC(mv), dst = DST(mv);
    int_fast32_t pc = this->squares[src]; // 起点棋子
    this->addPiece(dst, this->squares[dst], DEL_PIECE); // 删除终点棋子
    this->addPiece(src, this->squares[src], DEL_PIECE); // 删除起点棋子
    this->addPiece(dst, pc); // 在终点添加起点棋子
}

// 撤销移动棋子
void Position::undoMovePiece(int_fast32_t mv, int_fast32_t cap) {
    this->movePiece(mv);
    this->addPiece(SRC(mv), cap);
}

// 得到下一个走法
bool Position::nextMove() {
    if (this->curMvCnt[this->distance] + 1 >= this->genNum[this->distance])
        return false; // 达到最大着法数 返回 false 结束循环
    // 判断合法走法 ...

    this->curMvCnt[this->distance]++; // 增加为下一个走法的下标
    return true;
}

// 执行走法
void Position::makeMove() {
    auto mvPtr = this->mvsGen[this->distance] + this->curMvCnt[this->distance];
    this->movePiece(mvPtr->mv); // 执行走法

    this->saveStatus();
    this->changeSide(); // 交换走子方

    this->distance++;
    this->moveNum++;
}

void Position::undoMakeMove() {
    this->distance--;
    this->moveNum--;

    this->changeSide(); // 交换走子方
    this->rollBack();

    auto mvPtr = this->mvsGen[this->distance] + this->curMvCnt[this->distance];
    int mv = MOVE(DST(mvPtr->mv), SRC(mvPtr->mv));
    this->undoMovePiece(mv, mvPtr->cap); // 撤销走法
}

/* 通过棋盘字符串初始化
 * debug 初始化 大写红方
  a b c d e f g h i   
9|r n b a k a b n r |9
8|* * * * * * * * * |8
7|* c * * * * * c * |7
6|p * p * p * p * p |6
5|* * * * * * * * * |5
4|* * * * * * * * C |4
3|P * P * P * P * P |3
2|* C * * * * * * * |2
1|* * * * * * * * * |1
0|R N B A K A B N R |0
  a b c d e f g h i
 */
#ifdef POS_DEBUG // 仅 debug 模式下使用
void Position::fromStringMap(string* s, int_fast32_t side) { // 9 - 0 行的字符串
    // pcRed 和 pcBlack 分别代表红方和黑方每个兵种即将占有的序号
    int_fast32_t pcRed[PIECE_EMPTY], pcBlack[PIECE_EMPTY];
    pcRed[0] = SIDE_TAG(0) + KING_FROM;
    pcRed[1] = SIDE_TAG(0) + ADVISOR_FROM;
    pcRed[2] = SIDE_TAG(0) + BISHOP_FROM;
    pcRed[3] = SIDE_TAG(0) + KNIGHT_FROM;
    pcRed[4] = SIDE_TAG(0) + ROOK_FROM;
    pcRed[5] = SIDE_TAG(0) + CANNON_FROM;
    pcRed[6] = SIDE_TAG(0) + PAWN_FROM;
    for (int_fast32_t i = 0; i < PIECE_EMPTY; i++)
        pcBlack[i] = pcRed[i] + 16;
    // 清空棋盘
    this->clear();

    // 读取棋子
    for (int_fast32_t j = 9, x = 0, y = 9, pcType; ~j; j--) {
        for (int_fast32_t i = 2; i + 2 < (int_fast32_t)s[j].size(); i += 2) {
            x = i / 2 - 1 + X_FROM;
            y = j + Y_FROM;
            pcType = charToPt(toupper(s[j][i])); // 获得棋子类型
            if (pcType == PIECE_EMPTY) continue;
            if (isupper(s[j][i])) {
                if (pcRed[pcType] < 32) {
                    if (this->pieces[pcRed[pcType]] == 0) {
                        this->addPiece(COORD_XY(x, y), pcRed[pcType]);
                        pcRed[pcType]++;
                    }
                }
            } else {
                if (pcBlack[pcType] < 48) {
                    if (this->pieces[pcBlack[pcType]] == 0) {
                        this->addPiece(COORD_XY(x, y), pcBlack[pcType]);
                        pcBlack[pcType]++;
                    }
                }
            }
        }
    }
    if (side) this->changeSide();
}

void Position::debug() { // 仅 debug 模式下使用
    cout << "side: " << this->sidePly << endl;
    cout << "  a b c d e f g h i" << endl;
    for (int_fast32_t y = Y_TO; y >= Y_FROM; y--) { // 行
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY);
        cout << y - Y_FROM << "|";
        for (int_fast32_t x = X_FROM; x <= X_TO; x++) { // 列
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY);
            auto pc = this->squares[COORD_XY(x, y)]; // (x, y) 处棋子
            if (pc == 0) cout << "*" << " ";
            else if (pc < 32) { // 红
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);
                cout << ptToChar(PIECE_TYPE(pc)) << " ";
            } else {
                cout << (char)tolower(ptToChar(PIECE_TYPE(pc))) << " ";
            }
        }
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY);
        cout << "|" << y - Y_FROM << endl;
    }
    cout << "  a b c d e f g h i" << endl;
    cout << endl;
    cout.flush() << endl;
}

#endif

#endif