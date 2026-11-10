#include "shared/sdf/sdfVu1/decls.h"

void corePktInitCb(sdfCorePacketCb_t *cb, u16 *packetHdr, u32 attrMask) {
    u8 *payload = (u8 *)(packetHdr + 4);
    u16 ringPosition;
    u16 parameterFlags;
    u16 nextParameter;
    u16 selectedFlags;
    cb->faceCount = packetHdr[0];
    ringPosition = packetHdr[1];
    cb->pointCount = ringPosition;
    parameterFlags = packetHdr[2];
    nextParameter = packetHdr[3];
    selectedFlags = parameterFlags & attrMask;
    cb->orgAttrBits = parameterFlags;
    cb->packetBits = nextParameter;
    cb->attrBits = selectedFlags;
    corePktPrimBase = selectedFlags & 0x78;
    cb->dataPtr = payload;
    cb->packetOpenPtr = 0;
    corePktSetupPointBuff(cb, ringPosition);
}
