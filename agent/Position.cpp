#ifndef POSITION_CPP
#define POSITION_CPP

#include "Position.h"
#ifdef POS_DEBUG
#include "windows.h"
#endif

// 将FEN串中棋子标识转化为对应棋子类型 需toupper转化为大写
uint_fast8_t charToPt(char c) {
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
char ptToChar(uint_fast8_t pt) {
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

// 根据 x, y 返回序号 0 ~ 255
uint_fast8_t Position::coordXY(int_fast8_t x, int_fast8_t y) {
    return x + (y << 4);
}

// 根据 sq 0 ~ 255 返回行数 y
int_fast8_t Position::getY(uint_fast8_t sq) {
    return sq >> 4;
}

// 根据 sq 0 ~ 255 返回列数 x
int_fast8_t Position::getX(uint_fast8_t sq) {
    return sq & 15;
}

// 判断某位置是否在棋盘
bool Position::isInBoard(uint_fast8_t sq) {
    return IN_BOARD[sq];
}

// 初始化
void Position::clear() {
    memset(this->squares, 0, sizeof(this->squares));
    memset(this->pieces, 0, sizeof(this->pieces));
}

// 将棋子 pc 添加进棋局中的 sq 位置
void Position::addPiece(uint_fast8_t sq, uint_fast8_t pc, bool del) {
    if (del) this->squares[sq] = 0, this->pieces[pc] = 0;
    else this->squares[sq] = pc, this->pieces[pc] = sq;
    // this->zobr.Xor(PreGen.zobrTable[pt][sq]);
}

// 通过FEN串初始化棋局
void Position::fromFen(string fen) {
    this->clear();
    // ...
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
void Position::fromStringMap(string* s) { // 9 - 0 行的字符串
    // pcRed 和 pcBlack 分别代表红方和黑方每个兵种即将占有的序号
    uint_fast8_t pcRed[PIECE_EMPTY], pcBlack[PIECE_EMPTY];
    pcRed[0] = SIDE_TAG(0) + KING_FROM;
    pcRed[1] = SIDE_TAG(0) + ADVISOR_FROM;
    pcRed[2] = SIDE_TAG(0) + BISHOP_FROM;
    pcRed[3] = SIDE_TAG(0) + KNIGHT_FROM;
    pcRed[4] = SIDE_TAG(0) + ROOK_FROM;
    pcRed[5] = SIDE_TAG(0) + CANNON_FROM;
    pcRed[6] = SIDE_TAG(0) + PAWN_FROM;
    for (int_fast8_t i = 0; i < PIECE_EMPTY; i++)
        pcBlack[i] = pcRed[i] + 16;
    // 清空棋盘
    this->clear();

    // 读取棋子
    for (int_fast8_t j = 9, x = 0, y = 9, pcType; ~j; j--) {
        for (int_fast8_t i = 2; i < s[j].size() - 2; i += 2) {
            x = i / 2 - 1 + RANK_LEFT;
            y = j + RANK_TOP;
            pcType = charToPt(toupper(s[j][i])); // 获得棋子类型
            if (pcType == PIECE_EMPTY) continue;
            if (isupper(s[j][i])) {
                if (pcRed[pcType] < 32) {
                    if (this->pieces[pcRed[pcType]] == 0) {
                        this->addPiece(this->coordXY(x, y), pcRed[pcType]);
                        pcRed[pcType]++;
                    }
                }
            } else {
                if (pcBlack[pcType] < 48) {
                    if (this->pieces[pcBlack[pcType]] == 0) {
                        this->addPiece(this->coordXY(x, y), pcBlack[pcType]);
                        pcBlack[pcType]++;
                    }
                }
            }
        }
    }
}

void Position::debug() { // 仅 debug 模式下使用
    cout << "  a b c d e f g h i" << endl;
    for (int y = RANK_BOTTOM; y >= RANK_TOP; y--) { // 行
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY);
        cout << y - RANK_TOP << "|";
        for (int x = RANK_LEFT; x <= RANK_RIGHT; x++) { // 列
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY);
            auto pc = this->squares[this->coordXY(x, y)]; // (x, y) 处棋子
            if (pc == 0) cout << "*" << " ";
            else if (pc < 32) { // 红
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);
                cout << ptToChar(PIECE_TYPE(pc)) << " ";
            } else {
                cout << (char)tolower(ptToChar(PIECE_TYPE(pc))) << " ";
            }
        }
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY);
        cout << "|" << y - RANK_TOP << endl;
    }
    cout << "  a b c d e f g h i" << endl;
    cout << endl;
    cout.flush() << endl;
}

#endif

#endif