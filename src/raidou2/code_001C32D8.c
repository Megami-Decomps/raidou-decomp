#include "common.h"
#include "dds3KernelCore.h"
#include "scrTraceCode.h"
#include "evtCutIn.h"
#include "evtPicture.h"
#include "newdata.h"
#include "EventBit.h"

INCLUDE_ASM(const s32, "code_001C32D8", evtCallGetScriptHandle);

INCLUDE_ASM(const s32, "code_001C32D8", evtCallGetIntPara16);

INCLUDE_ASM(const s32, "code_001C32D8", evtCommand_CALL_FIELD);

INCLUDE_ASM(const s32, "code_001C32D8", evtCallGetIntPara17);

s32 CUTIN_DSP(void) {
    scrProcessWork_s *work;
    s32 x;
    s32 y;
    s32 type;
    s32 uid;
    dds3ProcessID_t *pid;

    work = scrGetScriptHandle();
    if (work == NULL) {
        return 1;
    }
    x = scrGetIntPara(0);
    y = scrGetIntPara(1);
    type = scrGetIntPara(2);
    uid = scrGetIntPara(3);
    pid = evtGetPIDfromUID(uid);
    scrSetIntReturnValue((s32)pid);
    if (pid == NULL) {
        evtStartCommand(work->ProcessID, x, y, type, uid);
        return 0;
    }
    return evtPictureLoadCheckProcess(pid) != 0;
}

u32 sndCallGetIntPara(void)
{
    void *id;

    id = EE_ADDR_AS(void *, scrGetIntPara(0));
    if (dds3ExistProcess(id) != 0) {
        evtEndCommand(id);
    }
    return 1;
}

INCLUDE_ASM(const s32, "code_001C32D8", sndWaitForEarringPlayback);

INCLUDE_ASM(const s32, "code_001C32D8", sndFunc);

INCLUDE_ASM(const s32, "code_001C32D8", sndCallProcTrigSequenceSlMain);

INCLUDE_ASM(const s32, "code_001C32D8", sndCallCheckLastSkipStat);

INCLUDE_ASM(const s32, "code_001C32D8", nbCreateBasicEffect);

INCLUDE_ASM(const s32, "code_001C32D8", prm_DH_PLUS);

INCLUDE_ASM(const s32, "code_001C32D8", evtMoonTakaraCntReset);

INCLUDE_ASM(const s32, "code_001C32D8", evtMoonZeniCntReset);

INCLUDE_ASM(const s32, "code_001C32D8", evtMoonTakumiCntReset);

INCLUDE_ASM(const s32, "code_001C32D8", evtMoonTeisatuCntReset);

void prmRefDDS3GBWK5(void) {
    DDS3_GBWK->unk1F8A = 2;
    evtBitOff(0x991);
}

INCLUDE_ASM(const s32, "code_001C32D8", prmCallGetRandIntA);

INCLUDE_ASM(const s32, "code_001C32D8", prmCallGetRandIntA2);

INCLUDE_ASM(const s32, "code_001C32D8", prmRefDDS3GBWK4);
