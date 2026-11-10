#include "shared/interface/frFont/decls.h"

/* Enable mask bits and return the previous shared flag byte. */
u8 frEnableMode(s32 Flag) {
    u8 previousFlags = gModeFlag;

    Flag |= previousFlags;
    gModeFlag = Flag;
    return previousFlags;
}
