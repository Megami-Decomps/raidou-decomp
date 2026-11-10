#include "shared/sdf/sdfPacketBuff/decls.h"

s32 sdfGrGetPacketBuffRemain(void) {
    return sdfGrPacketBuffLimit - sdfGrPacketBuffPtr;
}
