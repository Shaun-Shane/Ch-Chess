#ifndef AGENT_HPP
#define AGENT_HPP

#include <bits/stdc++.h>
#include "Position.cpp"

using namespace std;

constexpr bool RED = 0, BLACK = 1;

struct Agent {
    // 运行
    void run();

    // 初始化
    void init();

    // 移动棋子
    void move();

#ifdef POS_DEBUG
    void set();
#endif

    // 己方颜色 默认红色
    bool aiSide = RED;
};

void Agent::run() {
    while (true) {
        cout << "$ ";
        cout.flush();
        string opt, x;
        cin >> opt;
        if (opt == "start") {
            cin >> x; //input color r | b
            aiSide = (x == "r" ? RED : BLACK);
            init();
        } else if (opt == "turn") { // my turn
            // ...
        } else if (opt == "move") {
            move();
        } else if (opt == "end") break;
#ifdef POS_DEBUG
        else if (opt == "set") set();
#endif
        else cout << "invalid input" << endl;
    }
}

void Agent::init() {
#ifdef POS_DEBUG
    string s[] = {"0|R N B A K A B N R |0", "1|* * * * * * * * * |1",
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
    string s[10], tmp;
    cin.ignore('\n');
    getline(cin, tmp);
    for (int_fast32_t j = 9; j >= 0; j--) getline(cin, s[j]);
    getline(cin, tmp);
    pos.fromStringMap(s, aiSide);
    pos.debug();
}

void Agent::move() {
    string s;
    cin >> s;
    pos.movePiece(s);
    pos.debug();
}
#endif

#endif