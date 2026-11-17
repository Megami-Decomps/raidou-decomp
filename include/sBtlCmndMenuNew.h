#ifndef SBTLCMNDMENUNEW_H
#define SBTLCMNDMENUNEW_H

#include "common.h"

void DrawFuncSkl(s32 a_id, s32 a_x, s32 a_y, s32 a_z, u32 a_fontColor, u8 a_alphaRate);
void btlLoadBossEventTable(void);

#ifdef VERSION_RAIDOU2
#include "kernel.h"
#include "sdfMemory.h"

typedef struct {
    sdfMemHandle_t *hMem;
    dds3ProcessID_t *drawPID;
    u8 unk8[0xA];
    u8 unk12;
    u8 unk13[0x39];
} sbtlCmndMenuNew_t;

extern const char sbtlStrCmndMenu[];
extern const char sbtlStrCmndMenuDraw[];
extern dds3ProcessID_t *sbtlData181;
s32 Proc_sbtlCmndMenuNew(dds3ProcessID_t *id);
void Shutdown_sbtlCmndMenuNew(dds3ProcessID_t *id);
s32 DrawProc(dds3ProcessID_t *id);
extern u32 sbtlCheckData;
extern u32 btlLoadBossEventTableData;
#endif

#endif
