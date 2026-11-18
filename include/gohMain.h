#ifndef GOHMAIN_H
#define GOHMAIN_H

#include "common.h"
#include "kernel.h"
#include "code_r1_001CC1D0.h"

struct sdatUnitWork_t;

/* Work of the goh menu processes. HD holds a pointer to the sequence handle; R1 embeds it. Partial layout. */
typedef struct gohData_t {
    struct sdfMemHandle_t *hMem;
    u32 flag;
    jmsSeqHandle_t panel;
    s32 seqUnit;
    u8 pad58[0x8C - 0x58];
    s32 unk8C;
    u32 unk90[10];
    u8 padB8[0xCC - 0xB8];
    s32 unkCC;
} gohData_t;

/* Partial layout of the goh status process work. */
typedef struct gohStatWork_t {
    u8 unk00[0x20330];
    s32 unk20330;
} gohStatWork_t;

void gohCallMemset2(void);
s32 MahjanInit(dds3ProcessID_t *PID);
dds3ProcessFunc_t MahjanUpdateProcess(dds3ProcessID_t *PID);
s32 evtBDispatchSyncD2(struct gohData_t *pData);
dds3ProcessFunc_t gohAccidentCalcProcess(dds3ProcessID_t *PID);
dds3ProcessFunc_t gohBakBakDestroy(dds3ProcessID_t *PID);
dds3ProcessFunc_t gohCallGetProcessData29(dds3ProcessID_t *PID);
dds3ProcessFunc_t gohCallGetProcessData30(dds3ProcessID_t *PID);
s32 gohCommandDEVILOUTAFTERSub(struct sdatUnitWork_t *unit, u32 *flag, s32 mode);
s32 gohCreateRecoverList(struct gohData_t *pData);
s32 gohCreateSwordList(struct gohData_t *pData);
dds3ProcessFunc_t gohCyuuseiDrawProcess(dds3ProcessID_t *PID);
s32 gohCyuuseiStartDirection(struct gohData_t *pData);
s32 gohMiscInit(dds3ProcessID_t *PID);
dds3ProcessFunc_t gohRecoverCalcProcess(dds3ProcessID_t *PID);
dds3ProcessFunc_t gohRecoverDrawProcess(dds3ProcessID_t *PID);
dds3ProcessFunc_t gohRecoverUpdateProcess(dds3ProcessID_t *PID);
dds3ProcessFunc_t gohRootCalcProcess(dds3ProcessID_t *PID);
dds3ProcessFunc_t gohRootDrawProcess(dds3ProcessID_t *PID);
dds3ProcessFunc_t gohRootUpdateProcess(dds3ProcessID_t *PID);
dds3ProcessFunc_t gohRootWaitCalcProcess(dds3ProcessID_t *PID);
dds3ProcessFunc_t gohRootWaitDrawProcess(dds3ProcessID_t *PID);
dds3ProcessFunc_t gohRootWaitUpdateProcess(dds3ProcessID_t *PID);
dds3ProcessFunc_t gohTalkCalcProcess(dds3ProcessID_t *PID);
s32 jmsCheckScrRun(dds3ProcessID_t *PID);
s32 scrLogStart(struct gohData_t *pData);
extern jmsSeqUnit_t gohData5;

#endif
