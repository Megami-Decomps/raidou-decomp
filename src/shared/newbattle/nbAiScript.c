#include "common.h"
#include "kernel.h"
#include "code_r1_001CC1D0.h"

INCLUDE_ASM(const s32, "newbattle/nbAiScript", gohCmb2Destroy);

#ifdef VERSION_RAIDOU1
s32 gohRecoverInit(dds3ProcessID_t *PID);
s32 gohErrorDrawProcess(dds3ProcessID_t *PID);
s32 gohCyuuseiUpdateProcess(dds3ProcessID_t *PID);
s32 gohCmb1Destroy(dds3ProcessID_t *PID);
s32 gohCmb1Init(dds3ProcessID_t *PID);
s32 gohCmbSelDestroy(dds3ProcessID_t *PID);
s32 gohCmbSelDrawProcess(dds3ProcessID_t *PID);
s32 gohCmbSelUpdateProcess(dds3ProcessID_t *PID);
s32 gohCalcProcess(dds3ProcessID_t *PID);
s32 gohSwordDestroy(dds3ProcessID_t *PID);
s32 gohRecoverDestroy(dds3ProcessID_t *PID);
s32 gohSwordInit(dds3ProcessID_t *PID);
s32 gohCmbSelInit(dds3ProcessID_t *PID);
s32 gohConfirmInit(dds3ProcessID_t *PID);
s32 gohConfirmCalcProcess(dds3ProcessID_t *PID);
s32 gohCyuuseiInit(dds3ProcessID_t *PID);
s32 gohCyuuseiCalcProcess(dds3ProcessID_t *PID);
s32 gohCmbSelCalcProcess(dds3ProcessID_t *PID);
s32 gohCmb1UpdateProcess(dds3ProcessID_t *PID);
s32 gohCmb2UpdateProcess(dds3ProcessID_t *PID);
s32 gohStatDrawProcess(dds3ProcessID_t *PID);
s32 gohStatUpdateProcess(dds3ProcessID_t *PID);
s32 gohConfirmDrawProcess(dds3ProcessID_t *PID);
s32 gohConfirmUpdateProcess(dds3ProcessID_t *PID);
s32 gohTalkDrawProcess(dds3ProcessID_t *PID);
s32 gohCmb1CalcProcess(dds3ProcessID_t *PID);
s32 gohTalkUpdateProcess(dds3ProcessID_t *PID);
s32 gohMiscCalcProcess(dds3ProcessID_t *PID);
s32 gohMiscDrawProcess(dds3ProcessID_t *PID);
s32 gohMiscUpdateProcess(dds3ProcessID_t *PID);
s32 gohSinanInit(dds3ProcessID_t *PID);
s32 gohSinanDrawProcess(dds3ProcessID_t *PID);
s32 gohSinanUpdateProcess(dds3ProcessID_t *PID);
s32 gohErrorInit(dds3ProcessID_t *PID);
s32 gohErrorUpdateProcess(dds3ProcessID_t *PID);
s32 gohKarteRootInit(dds3ProcessID_t *PID);
s32 gohCmb1DrawProcess(dds3ProcessID_t *PID);
s32 gohKarteRootDrawProcess(dds3ProcessID_t *PID);
s32 gohKarteRootUpdateProcess(dds3ProcessID_t *PID);
s32 gohCmb2CalcProcess(dds3ProcessID_t *PID);
s32 gohCmb2DrawProcess(dds3ProcessID_t *PID);
s32 gohSwordCalcProcess(dds3ProcessID_t *PID);
s32 gohSwordDrawProcess(dds3ProcessID_t *PID);
s32 gohStatCalcProcess(dds3ProcessID_t *PID);
s32 gohStatDestroy(dds3ProcessID_t *PID);
s32 gohKarteRootCalcProcess(dds3ProcessID_t *PID);
s32 gohKarteRootDestroy(dds3ProcessID_t *PID);
s32 gohCyuuseiDrawProcess(dds3ProcessID_t *PID);
s32 gohDrawProcess(dds3ProcessID_t *PID);
s32 gohCmb2Init(dds3ProcessID_t *PID);
s32 gohConfirmDestroy(dds3ProcessID_t *PID);
s32 gohCyuuseiDestroy(dds3ProcessID_t *PID);
s32 gohMiscDestroy(dds3ProcessID_t *PID);
s32 gohSinanDestroy(dds3ProcessID_t *PID);
s32 gohErrorDestroy(dds3ProcessID_t *PID);
s32 gohInit(dds3ProcessID_t *PID);
s32 gohDestroy(dds3ProcessID_t *PID);
s32 gohMiscInit(dds3ProcessID_t *PID);
s32 gohRecoverCalcProcess(dds3ProcessID_t *PID);
s32 gohRecoverDrawProcess(dds3ProcessID_t *PID);
s32 gohRecoverUpdateProcess(dds3ProcessID_t *PID);
s32 gohRootCalcProcess(dds3ProcessID_t *PID);
s32 gohRootDestroy(dds3ProcessID_t *PID);
s32 gohRootDrawProcess(dds3ProcessID_t *PID);
s32 gohRootInit(dds3ProcessID_t *PID);
s32 gohRootUpdateProcess(dds3ProcessID_t *PID);
s32 gohRootWaitCalcProcess(dds3ProcessID_t *PID);
s32 gohRootWaitDrawProcess(dds3ProcessID_t *PID);
s32 gohRootWaitUpdateProcess(dds3ProcessID_t *PID);
s32 gohErrorCalcProcess(dds3ProcessID_t *PID);
s32 gohSinanCalcProcess(dds3ProcessID_t *PID);
s32 gohStatInit(dds3ProcessID_t *PID);
s32 gohSwordUpdateProcess(dds3ProcessID_t *PID);
s32 gohTalkCalcProcess(dds3ProcessID_t *PID);
s32 gohTalkDestroy(dds3ProcessID_t *PID);
s32 gohTalkInit(dds3ProcessID_t *PID);
s32 gohUpdateProcess(dds3ProcessID_t *PID);
s32 gohCmb2Destroy(dds3ProcessID_t *PID);

jmsSeqUnit_t gohSeqUnit[16] = {
    {0, NULL, NULL, gohRootWaitCalcProcess, gohRootWaitDrawProcess, gohRootWaitUpdateProcess, NULL},
    {0, gohRootInit, gohRootDestroy, gohRootCalcProcess, gohRootDrawProcess, gohRootUpdateProcess, NULL},
    {0, gohCmbSelInit, gohCmbSelDestroy, gohCmbSelCalcProcess, gohCmbSelDrawProcess, gohCmbSelUpdateProcess, NULL},
    {0, gohCmb1Init, gohCmb1Destroy, gohCmb1CalcProcess, gohCmb1DrawProcess, gohCmb1UpdateProcess, NULL},
    {0, gohCmb2Init, gohCmb2Destroy, gohCmb2CalcProcess, gohCmb2DrawProcess, gohCmb2UpdateProcess, NULL},
    {0, gohSwordInit, gohSwordDestroy, gohSwordCalcProcess, gohSwordDrawProcess, gohSwordUpdateProcess, NULL},
    {0, gohStatInit, gohStatDestroy, gohStatCalcProcess, gohStatDrawProcess, gohStatUpdateProcess, NULL},
    {0, gohConfirmInit, gohConfirmDestroy, gohConfirmCalcProcess, gohConfirmDrawProcess, gohConfirmUpdateProcess, NULL},
    {0, gohCyuuseiInit, gohCyuuseiDestroy, gohCyuuseiCalcProcess, gohCyuuseiDrawProcess, gohCyuuseiUpdateProcess, NULL},
    {0, gohKarteRootInit, gohKarteRootDestroy, gohKarteRootCalcProcess, gohKarteRootDrawProcess, gohKarteRootUpdateProcess, NULL},
    {0, gohRecoverInit, gohRecoverDestroy, gohRecoverCalcProcess, gohRecoverDrawProcess, gohRecoverUpdateProcess, NULL},
    {0, gohTalkInit, gohTalkDestroy, gohTalkCalcProcess, gohTalkDrawProcess, gohTalkUpdateProcess, NULL},
    {0, gohMiscInit, gohMiscDestroy, gohMiscCalcProcess, gohMiscDrawProcess, gohMiscUpdateProcess, NULL},
    {0, gohSinanInit, gohSinanDestroy, gohSinanCalcProcess, gohSinanDrawProcess, gohSinanUpdateProcess, NULL},
    {0, gohErrorInit, gohErrorDestroy, gohErrorCalcProcess, gohErrorDrawProcess, gohErrorUpdateProcess, NULL},
    {0, gohInit, gohDestroy, gohCalcProcess, gohDrawProcess, gohUpdateProcess, NULL},
};
#endif
