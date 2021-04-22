#ifndef AGENT_HPP
#define AGENT_HPP

#define DEBUG

#ifndef DEBUG
#include "Position.h"
#else
#include "Position.cpp"
#include "genMoves.cpp"
#endif

constexpr bool RED = 0, BLACK = 1;

Position pos;

struct Agent {
    // 运行
    void run();

    // 初始化
    void init();

    // 移动棋子
    void move();

#ifdef POS_DEBUG
    void set();
    void printVl();
#endif

    // 己方颜色 默认红色
    bool aiSide = RED;
};

void Agent::run() {
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
            // ...
            // pos.genAllMoves();
            // std::cout << pos.distance << std::endl;
            // std::cout << pos.genNum[pos.distance] << std::endl;
            // while (pos.nextMove()) {
            //     pos.makeMove();
            //     pos.debug();
            //     // printVl();
            //     pos.undoMakeMove();
            // }
            // std::cout << pos.distance << std::endl;
            // pos.debug();
            // printVl();
        } else if (opt == "move") {
            move();
            // printVl();
        } else if (opt == "end") break;
#ifdef POS_DEBUG
        else if (opt == "set") set()/*, printVl()*/;
#endif
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

#ifdef POS_DEBUG
void Agent::set() {
    std::string s[10], tmp;
    std::cin.ignore('\n');
    getline(std::cin, tmp);
    for (int_fast32_t j = 9; j >= 0; j--) getline(std::cin, s[j]);
    getline(std::cin, tmp);
    pos.fromStringMap(s, aiSide);
    pos.debug();
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

#endif