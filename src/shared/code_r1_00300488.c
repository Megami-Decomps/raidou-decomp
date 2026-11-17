#include "common.h"
#include "fldNme.h"

s32 titleFunc(s32 page, s32 x, s32 y) {
    return nmeLeaf2(page, x, y) != 0xFFFF;
}
