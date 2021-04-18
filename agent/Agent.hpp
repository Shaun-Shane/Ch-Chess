#ifndef AGENT_HPP
#define AGENT_HPP

constexpr bool RED = 0, BLACK = 1;

struct Agent {
    // 运行
    void run();

    // 己方颜色 默认红色
    bool side = RED;
};

#endif