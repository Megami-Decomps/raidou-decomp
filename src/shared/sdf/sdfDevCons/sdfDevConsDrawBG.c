#include "shared/sdf/sdfDevCons/decls.h"

void sdfDevConsDrawBG(sdfDevConsCB_tag *aHndl, sdfDevConsDrawCB_t *aCB) {
    vu64 *packet;
    u32 origin;
    u32 extent;

    sdfDevConsTagClose(aCB);
    if (aCB->buffSize < aCB->tag - aCB->start + 0x80) {
        sdfDevConsDrawFlush(aCB);
    }
    packet = (vu64 *)aCB->tag;
    origin = (u16)(aHndl->x - 0x30) | ((u32)aHndl->y << 16);
    extent = (u16)(aHndl->w * 3 * 0x40 + 0x70) | ((u32)(aHndl->h * aCB->stepY) << 16);
    if (aHndl->mode & 4) {
        origin += 0xFFD00000;
        extent += 0x700000;
    } else {
        origin += 0xFFE80000;
        extent += 0x380000;
    }
    packet[0] = 0x5400000000008001ULL;
    packet[1] = 0x5510;
    packet[2] = 0x146;
    packet[3] = 0x30000000;
    packet[4] = origin;
    packet[5] = origin + extent;
    packet[6] = 0x156;
    aCB->tag = (u8 *)packet + 0x40;
    aCB->cur = aCB->tag + 0x10;
}
