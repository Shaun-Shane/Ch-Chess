#ifndef AGENT_HPP
#define AGENT_HPP

// VSC_DEBUG 在 search.h 中也有
#define VSC_DEBUG

#ifndef VSC_DEBUG
#include "search.h"
#else
#include "search.cpp"
#include<iostream>
#include<string>

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

    // debug 双方分数
    void printVl();

    // 己方颜色 默认红色
    bool aiSide = RED;
};
inline void PrintLn(const char *sz) 
{
  printf("%s\n", sz);
  fflush(stdout);
}
const char* const cszStartFen = "rnbakabnr/9/1c5c1/p1p1p1p1p/9/9/P1P1P1P1P/1C5C1/9/RNBAKABNR w";
UcciCommStruct UcciComm;
void Agent::run1()
{
    if (BootLine() != UCCI_COMM_UCCI) 
    {
        return;
    }
    int bDebug = true;
    int bQuit = false;
    pos.fromFen(cszStartFen);
    PrintLn("ucciok");
    std::pair<int_fast16_t, int_fast16_t>result;
    while(!bQuit)
    {
        switch (IdleLine(UcciComm, bDebug)) 
        {
            
            case UCCI_COMM_ISREADY:
                PrintLn("readyok");
                break;
            case UCCI_COMM_POSITION:
                BuildPos(pos, UcciComm);
                break;
            case UCCI_COMM_GO:
                result = searchRoot(6);
                std::cout << "bestmove " << MOVE_TO_STR(result.second) << std::endl;
                pos.movePiece(result.second);
                fflush(stdout);
                //pos.debug();
                //std::cout.flush();
				break;
            case UCCI_COMM_QUIT:
                bQuit = true;
                break;
            default:
                break;
        }
    }
    PrintLn("bye");
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
            auto st = clock();
            auto result = searchRoot(6);
            std::cout << "time: " << (1.0 * (clock() - st) / CLOCKS_PER_SEC) << std::endl;
            std::cout << "bestMove " << MOVE_TO_STR(result.second) << std::endl;
            pos.movePiece(result.second);
            pos.debug();
            std::cout << "vlBest: " << result.first << std::endl;
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
    for (int_fast16_t j = 9; j >= 0; j--) getline(std::cin, s[j]);
    getline(std::cin, tmp);
    pos.fromStringMap(s, aiSide);
    pos.debug();
#endif
}

void Agent::move() {
    std::string s;
    std::cin >> s;
    pos.movePiece(s);
}

void Agent::printVl() {
    std::cout << "scores: " << pos.vlRed << " " << pos.vlBlack << std::endl;
}

#endif