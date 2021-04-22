#include "agent\Agent.hpp"
#include "windows.h"

int main() {
    //ios::sync_with_stdio(false), cin.tie(nullptr), cout.tie(nullptr);
    static Agent ai;
    ai.run();
    return 0;
}