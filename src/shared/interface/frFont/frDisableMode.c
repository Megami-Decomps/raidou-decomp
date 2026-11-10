#include "shared/interface/frFont/decls.h"

/* Clear flag bits from the shared font flag byte; returns the previous value. */
u8 frDisableMode(u8 Flag) {
    u8 previousFlags = gModeFlag;

    gModeFlag = previousFlags & ~Flag;
    return previousFlags;
}
