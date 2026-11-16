#include "common.h"
#include "libsdf.h"
#include "kernel.h"
#include "basic.h"
#include "event.h"
#include "scr.h"
#include "dds3SceneBasic.h"
#include "dds3KernelCore.h"
#include "evtEventViewer.h"
#include "scrTraceCode.h"
#include "evtCutIn.h"
#include "evtPicture.h"
#include "newdata.h"
#include "sBtlContinue.h"
#include "dds3Randomize.h"

INCLUDE_ASM(const s32, "code_001AD4E8", evtCallGetScriptHandle);

INCLUDE_ASM(const s32, "code_001AD4E8", evtCallGetIntPara16);

INCLUDE_ASM(const s32, "code_001AD4E8", evtCommand_CALL_FIELD);

INCLUDE_ASM(const s32, "code_001AD4E8", evtCallGetIntPara17);

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
    dds3ProcessID_t *id;

    id = EE_ADDR_AS(dds3ProcessID_t *, scrGetIntPara(0));
    if (dds3ExistProcess(id) != 0) {
        evtEndCommand(id);
    }
    return 1;
}

INCLUDE_ASM(const s32, "code_001AD4E8", sndWaitForEarringPlayback);

INCLUDE_ASM(const s32, "code_001AD4E8", sndFunc);

INCLUDE_ASM(const s32, "code_001AD4E8", sndCallProcTrigSequenceSlMain);

void sndCallCheckLastSkipStat(void) {
    scrSetIntReturnValue(evtCheckLastSkipStat());
}

s32 evtMoonTakaraCntReset(void) {
    s32 count;

    count = dds3GetRandIntA(NULL, 5) + 6;
    DDS3_GBWK->takaraCnt = count;
    return count;
}

s32 evtMoonZeniCntReset(void) {
    s32 count;

    count = dds3GetRandIntA(NULL, 5) + 6;
    DDS3_GBWK->zeniCnt = count;
    return count;
}

s32 evtMoonTakumiCntReset(void) {
    s32 count;

    count = dds3GetRandIntA(NULL, 5) + 16;
    DDS3_GBWK->takumiCnt = count;
    return count;
}

/* NON_MATCHING: 0 of 4 words match.  */
#ifdef NON_MATCHING
s32 evtMoonTeisatuCntReset(void) {
    DDS3_GBWK->teisatuCnt = 1;
    return 1;
}
#else
INCLUDE_ASM(const s32, "code_001AD4E8", evtMoonTeisatuCntReset);
#endif

INCLUDE_RODATA(const s32, "code_001AD4E8", evtMsgTexLoadNoProc2);

