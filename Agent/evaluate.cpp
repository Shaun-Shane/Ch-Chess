#include "evaluate.h"

int32_t evaluate() {
    if (pos.sidePly == 0) return pos.vlRed - pos.vlBlack + 3;
    else return pos.vlBlack - pos.vlRed + 3;
}