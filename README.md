# Chinese Chess Agent
## Introduction
This repository contains a Chinese Chess agent based on MiniMAX with Alpha-Beta Pruning, supporting Universal Chinese Chess Protocol (UCCI) [1].

The agent demonstrates a search capability reaching a depth of 11, coupled with a quiescent search that remains within a depth limit of 64, all accomplished within a 60-second time frame.

The agent's skill level is akin to that of a Chess Wizard grandmaster [2], securing the top rank in a CS department competition.



## Usage
```
git clone git@github.com:Shaun1280/ChineseChess.git && cd ChineseChess
g++ -o main.exe main.cpp Agent/Position.cpp Agent/genMoves.cpp Agent/evaluate.cpp Agent/search.cpp Agent/Zobrist.cpp Agent/source.cpp -std=c++11 -DNDEBUG -O4 -static-libgcc -static-libstdc++ -Wl,-Bstatic -lstdc++ -lpthread -Wl,-Bdynamic
```

Then load main.exe into a Chinese Chess software like [2].

## Reference
- [1] [Universal Chinese Chess Protocol](https://www.xqbase.com/protocol/cchess_ucci.htm)
- [2] [Chess Wizard](https://www.chess-wizard.com/).
