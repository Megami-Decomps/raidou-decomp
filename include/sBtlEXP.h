#ifndef SBTLEXP_H
#define SBTLEXP_H

#include "common.h"
#include "enums.h"
#include "sdfMemory.h"

/* Battle experience popup (0x28 bytes in R1): HD layout behind a memory handle. */
typedef struct SBTLEXP_WORK {
    sdfMemHandle_t *handle;
    u8 side;
    s16 mode;
    SBTLEXP_ST status;
    s32 para;
    s32 counter;
    s32 alpha;
    s32 x;
    s32 y;
    struct SBTLEXP_WORK *prev;
    struct SBTLEXP_WORK *next;
} SBTLEXP_WORK;
SIZE_CHECK(sbtlExpWorkSizeCheck, SBTLEXP_WORK, 0x28);

/* 0x34 bytes in R1; field10 has no HD counterpart. */
typedef struct SBTLEXP_LISTWORK {
    s16 num;
    s16 level;
    s16 counter;
    f32 ganpon;
    f32 riritu;
    s32 field10;
    s16 za_cnt;
    s16 mode;
    s32 para;
    s32 alpha;
    s32 ralpha;
    s32 rcounter;
    s32 o_alpha;
    s16 o_counter;
    SBTLEXP_WORK *ewk;
} SBTLEXP_LISTWORK;
SIZE_CHECK(sbtlExpListWorkSizeCheck, SBTLEXP_LISTWORK, 0x34);

SBTLEXP_LISTWORK *InitTarSelProcessData(void);
s32 sbtlRefSbtl60(SBTLEXP_LISTWORK *lwk);
s32 sbtlReleaseEXP_List(SBTLEXP_LISTWORK *lwk, u8 flg);

#endif
