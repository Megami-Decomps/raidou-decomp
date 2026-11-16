#ifndef NMEINPUT0_H
#define NMEINPUT0_H

#include "common.h"
#include "kernel.h"
#include "fileManager.h"
#include "fldNme.h"

/* Work of the name entry input process (HD nmeInput0Work_tag), 0x300 bytes from nmeStart.
 * Laid out like fldNmeWork_t with one slot list. Partial layout. */
typedef struct nmeInput0Work_tag {
    s32 unk0;
    dds3ProcessID_t *proc_draw;
    s32 mode;
    s32 unkC;
    s32 unk10;
    u32 type;
    fldNmeSlotList_t list;
    s32 unk24;
    s32 unk28;
    s32 unk2C;
    s32 unk30;
    s32 unk34;
    s32 unk38;
    s32 unk3C;
    u8 pad40[0xC4 - 0x40];
    fileRequest_t *font;
    fldNmeDraw_t draw;
} nmeInput0Work_tag;

typedef char nmeInput0Work_tagSizeCheck[sizeof(nmeInput0Work_tag) == 0x300 ? 1 : -1];

dds3ProcessID_t *nmeStart(u32 type);

#endif
