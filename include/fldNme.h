#ifndef FLDNME_H
#define FLDNME_H

#include "common.h"
#include "kernel.h"
#include "fileManager.h"

/* Work of the name entry process, 0x308 bytes (sdfMemAllocDirect). */
typedef struct fldNmeWork_t {
    s32 unk0;
    dds3ProcessID_t *drawProcess;
    s32 unk8;
    s32 unkC;
    s32 unk10;
    s32 unk14;
    u8 unk18[0x18];
    s32 unk30;
    s32 unk34;
    s32 unk38;
    s32 unk3C;
    s32 unk40;
    s32 unk44;
    s32 unk48;
    u8 unk4C[0x80];
    fileRequest_t *font;
    u8 unkD0[0x238];
} fldNmeWork_t;

#endif /* FLDNME_H */
