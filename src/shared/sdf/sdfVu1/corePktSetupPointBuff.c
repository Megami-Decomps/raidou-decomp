#include "shared/sdf/sdfVu1/decls.h"

void corePktSetupPointBuff(sdfCorePacketCb_t *cb, s32 vertCount) {
    void *ringEnd = (u8 *)cb + 0x78;
    if (vertCount < 0x80) {
        cb->freePointCount = 0x80 - vertCount;
        cb->extPointBuff = EE_ADDR_PTR(corePktPointBuff);
        cb->freePointBuff = EE_ADDR_PTR(vertCount * 96 + SDF_SCRATCHPAD_POINT_BUFFER);
        cb->extPointCount = 0x2000;
        cb->pointBuff = SDF_SCRATCHPAD_POINT_BUFFER;
    } else if (vertCount == 0x80) {
        cb->pointBuff = SDF_SCRATCHPAD_POINT_BUFFER;
        cb->freePointBuff = EE_ADDR_PTR(corePktPointBuff);
        cb->freePointCount = 0x2000;
        cb->extPointBuff = 0;
        cb->extPointCount = 0;
    } else {
        cb->freePointCount = 0x80;
        cb->pointBuff = corePktPointBuff;
        cb->freePointBuff = EE_ADDR_PTR(SDF_SCRATCHPAD_POINT_BUFFER);
        cb->extPointBuff = EE_ADDR_PTR(corePktPointBuff + vertCount * 96);
        cb->extPointCount = 0x2000 - vertCount;
    }
    cb->extPointLink = ringEnd;
}
