#include "common.h"

s32 fldLeaf27(s32 type, s32 base) {
    switch (type) {
    case 3:
    case 20:
    case 21:
    case 26:
        return base + 10;
    case 18:
    case 33:
        return base + 8;
    }
    return 0;
}
