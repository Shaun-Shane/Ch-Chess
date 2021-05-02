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

    // 己方颜色 默认红色
    bool aiSide = RED;
};

void Agent::buildPos(const UcciCommStruct &UcciComm) {
    pos.fromFen(UcciComm.szFenStr);
    for (int i = 0; i < UcciComm.nMoveNum; i++) {
        std::string mvStr = UcciComm.lpdwMovesCoord[i];
        pos.makeMove(STR_TO_MOVE(mvStr));
        // pos.movePiece(mvStr);
        // pos.changeSide();
    }
}

void Agent::run1() {
    if (bootline() != UCCI_COMM_UCCI) {
        return;
    }
    int bDebug = true;
    int bQuit = false;
    pos.fromFen(cszStartFen);
    println("ucciok");
    std::pair<int32_t, int32_t> result;
    FILE* fpw = NULL;
    char file[11] = "output.txt";
	int tell;
    fpw=fopen(file,"w");
    fclose(fpw);
    while (!bQuit) {
        switch (idleline(UcciComm, bDebug)) {
            case UCCI_COMM_ISREADY:
                println("readyok");
                break;
            case UCCI_COMM_POSITION:
                this->buildPos(UcciComm);
                break;
            case UCCI_COMM_GO:
                
                result = searchMain();
                std::cout<<"info message "<<"asdfasfasd"<<std::endl;
                fflush(stdout);
                std::cout << "bestmove " << MOVE_TO_STR(result.second)
                          << std::endl;
                pos.movePiece(result.second);
                fflush(stdout);
                fpw = fopen(file, "rt+");
                fseek(fpw,0,2);
	            fprintf(fpw, "%d\n", 1);
	            fclose(fpw);
                // pos.debug();
                // std::cout.flush();
                
                break;
            case UCCI_COMM_QUIT:
                bQuit = true;
                
                break;
            default:
                break;
        }
    }
    println("bye");
    return;
}

void Agent::run_debug() {
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
            std::cout << "repStatus: " << pos.repStatus() << std::endl;
            auto st = clock();
            auto result = searchMain();
            std::cout << "time: " << (1.0 * (clock() - st) / CLOCKS_PER_SEC) << std::endl;
            std::cout << "bestMove " << MOVE_TO_STR(result.second) << std::endl;
            pos.movePiece(result.second);
            pos.debug();
            std::cout << "vlBest: " << result.first << std::endl;
            std::cout << "repStatus: " << pos.repStatus() << std::endl;
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
#endif
}

void Agent::move() {
    std::string s;
    std::cin >> s;
    // pos.movePiece(s);
    pos.makeMove(STR_TO_MOVE(s));
    pos.changeSide();
}

void Agent::printVl() {
    std::cout << "scores: " << pos.vlRed << " " << pos.vlBlack << std::endl;
}