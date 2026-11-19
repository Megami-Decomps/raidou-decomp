#ifndef CMPBOOK_H
#define CMPBOOK_H

#include "common.h"
#include "kernel.h"

void sbtlCallCalcSpriteWidth(s32 a_id, s32 a_x, s32 a_y, s32 a_z, u32 a_fontColor, u8 a_alphaRate);

#ifdef VERSION_RAIDOU2
/* Event bit of each keyword entry. */
extern s16 cmpCallBitCheckData[64];
extern s8 cmpData46;
extern dds3ProcessID_t *cmpCallGetCharPIDData3;
s32 cmpCallBitCheck(s32 a_no);
s32 cmpUpdateKeyWord(void);
#endif

#endif
