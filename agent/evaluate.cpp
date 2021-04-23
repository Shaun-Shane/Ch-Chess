#include "evaluate.h"

int_fast16_t evalute() {
    if (pos.sidePly == 0) return pos.vlRed - pos.vlBlack;
    else return pos.vlBlack - pos.vlRed;
}