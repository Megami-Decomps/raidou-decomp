#ifndef CODE_00346258_H
#define CODE_00346258_H

#include "common.h"
#include "kernel.h"
#include "jmsInit.h"
extern dds3ProcessID_t *g_mainPID_gohMenu;

/* Partial layout of the goh menu process work. */
typedef struct gohMenuWork_t {
    struct sdfMemHandle_t *hMem;
    u8 unk04[4];
    dds3ProcessID_t *drawPID;
    u8 unk0C[4];
    u8 step;
    u8 flags;
    u16 count;
    s32 backStat;
} gohMenuWork_t;

void DrawModel(jmsData_t *pData, s32 CursorIndex);

extern const char cmpCalcProcName3[];
extern const char campDrawProcName3[];
extern const char campUpdateProcName3[];

#endif
