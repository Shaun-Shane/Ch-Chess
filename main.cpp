#include <bits/stdc++.h>

using namespace std;

#define error(args...) { string _s = #args; replace(_s.begin(), _s.end(), ',', ' '); \
stringstream _ss(_s); istream_iterator<string> _it(_ss); err(_it, args); }

void err(istream_iterator<string> it) { cout << endl; }

template <typename T, typename... Args>
void err(istream_iterator<string> it, T a, Args... args) {
    cerr << *it << " = " << a << " ";
    err(++it, args...);
}

#include "agent\Agent.hpp"
#include "windows.h"

using namespace std;

int main() {
    //ios::sync_with_stdio(false), cin.tie(nullptr), cout.tie(nullptr);
    static Agent ai;
    ai.run();
    return 0;
}