#ifndef VSC_DEBUG
#include "ucci.hpp"
#include "search.h"
#else
#include "search.cpp"
#include "ucci.hpp"
#include <iostream>
#include <string>
#endif

#include <ctime>

constexpr bool RED = 0, BLACK = 1;

struct Agent {
    // 运行
    void run_debug();
    void run1();
    // 初始化
    void init();

    // 移动棋子
    void move();

    // 设置局面
    void set();

    void buildPos(const UcciCommStruct &UcciComm);

    // debug 双方分数
    void printVl();

    void ucciPrintFile(FILE* fpw, std::pair<int32_t, int32_t> result);

    // 己方颜色 默认红色
    bool aiSide = RED;
};

void Agent::buildPos(const UcciCommStruct &UcciComm) {
    pos.fromFen(UcciComm.fenStr);
    for (int i = 0; i < UcciComm.moveNum; i++) {
        std::string mvStr = UcciComm.movesCoord[i];
        pos.makeMove(STR_TO_MOVE(mvStr)); // makeMove 会 changeSide
        // 吃子则 moveList 清空
        if (pos.moveNum && pos.moveList[pos.moveNum - 1].cap) {
            pos.moveNum = 0;
            memset(miniHash, 0, sizeof(miniHash));
        }
    }
}

void Agent::run1() {
    pos.preGen(); // 初始化位行、位列
    if (bootline() != UCCI_COMM_UCCI) {
        return;
    }
    int quit = false;
    pos.fromFen(startFen);
    println("ucciok");
    std::pair<int32_t, int32_t> result;
#ifdef FILE_DEBUG
    FILE *fpw = NULL;
    fpw = fopen("output.txt", "w");
    fclose(fpw);
#endif
    while (!quit) {
        switch (idleline(UcciComm)) {
            case UCCI_COMM_ISREADY:
                println("readyok");
                break;
            case UCCI_COMM_POSITION:
                this->buildPos(UcciComm);
                break;
            case UCCI_COMM_GO:

                result = searchMain();
                if (result.second == 0) std::cout << "nobestmove" << std::endl;
                else {
                    std::cout << "bestmove " << MOVE_TO_STR(result.second)
                          << std::endl;
                    pos.makeMove(result.second), pos.changeSide();
                }
                fflush(stdout);

            #ifdef FILE_DEBUG
                fpw = fopen("output.txt", "rt+");
                this->ucciPrintFile(fpw, result);
                fclose(fpw);
            #endif
                break;
            case UCCI_COMM_QUIT:
                quit = true;

                break;
            default:
                break;
        }
    }
    println("bye");
    return;
}

void Agent::run_debug() {
    pos.preGen(); // 初始化位行、位列
#ifdef FILE_DEBUG
    FILE *fpw = NULL;
    fpw = fopen("output.txt", "w");
    fclose(fpw);
#endif
    while (true) {
        std::cout << "$ ";
        std::cout.flush();
        std::string opt, x;
        std::cin >> opt;
        if (opt == "start") {
            std::cin >> x; //input color r | b
            aiSide = (x == "r" ? RED : BLACK);
            init();
            // printVl();
        } else if (opt == "turn") { // my turn
            auto result = searchMain();
            std::cout << "bestmove " << MOVE_TO_STR(result.second) << std::endl;
            pos.makeMove(result.second), pos.changeSide();
            fflush(stdout);

#ifdef FILE_DEBUG
            fpw = fopen("output.txt", "rt+");
            this->ucciPrintFile(fpw, result);
            fclose(fpw);
 #endif
        } else if (opt == "move") {
            move();
            // printVl();
        } else if (opt == "end") break;
        else if (opt == "set") set()/*, printVl()*/;
        else std::cout << "invalid input" << std::endl;
    }
}

void Agent::init() {
#ifdef POS_DEBUG
    std::string s[] = {"0|R N B A K A B N R |0", "1|* * * * * * * * * |1",
                  "2|* C * * * * * C * |2", "3|P * P * P * P * P |3",
                  "4|* * * * * * * * * |4", "5|* * * * * * * * * |5",
                  "6|p * p * p * p * p |6", "7|* c * * * * * c * |7",
                  "8|* * * * * * * * * |8", "9|r n b a k a b n r |9"};
    pos.fromStringMap(s, aiSide);
    pos.debug();
#endif
    // ... 
}

void Agent::set() {
#ifdef POS_DEBUG
    std::string s[10], tmp;
    std::cin.ignore('\n');
    getline(std::cin, tmp);
    for (int32_t j = 9; j >= 0; j--) getline(std::cin, s[j]);
    getline(std::cin, tmp);
    pos.fromStringMap(s, aiSide);
    pos.debug();
    std::cout << pos.isChecked() << std::endl;
    std::cout << pos.isProtected(0, 0x97) << std::endl;
#endif
}

void Agent::move() {
    std::string s;
    std::cin >> s;
    if (!pos.makeMove(STR_TO_MOVE(s))) {
        std::cout << "checked" << std::endl;
    }
    // pos.changeSide();
    pos.debug();
    std::cout << pos.repPosition() << std::endl;
}

void Agent::printVl() {
    std::cout << "scores: " << pos.vlRed << " " << pos.vlBlack << std::endl;
}

void Agent::ucciPrintFile(FILE* fpw, std::pair<int32_t, int32_t> result) {
    fseek(fpw, 0, 2);
    // 输出棋盘
    fprintf(fpw, "  a b c d e f g h i\n");
    for (int32_t y = Y_TO; y >= Y_FROM; y--) { // 行
        fprintf(fpw, "%d|", y - Y_FROM);
        for (int32_t x = X_FROM; x <= X_TO; x++) { // 列
            auto pc = pos.squares[COORD_XY(x, y)]; // (x, y) 处棋子
            if (pc == 0) fprintf(fpw, "* ");
            else if (pc < 32) { // 红
                fprintf(fpw, "%c ", ptToChar(PIECE_TYPE(pc)));
            } else {
                fprintf(fpw, "%c ", (char)tolower(ptToChar(PIECE_TYPE(pc))));
            }
        }
        fprintf(fpw, "|%d\n", y - Y_FROM);
    }
    fprintf(fpw, "  a b c d e f g h i\n");

    fprintf(fpw, "vlBest: %d\n", result.first);
    fprintf(fpw, "repStatus: %d\n", pos.repPosition());
    fprintf(fpw, "\n");
}