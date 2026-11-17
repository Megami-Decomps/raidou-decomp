#include "common.h"
#include "fldProcess.h"

INCLUDE_ASM(const s32, "code_0017B0D8", mapAutoMapDraw3Sub);

INCLUDE_ASM(const s32, "code_0017B0D8", mapAutoMapDraw3);

INCLUDE_ASM(const s32, "code_0017B0D8", mapCallEnableMode2);

INCLUDE_ASM(const s32, "code_0017B0D8", mapAmDrawBasyoName);

INCLUDE_RODATA(const s32, "code_0017B0D8", mapNakamaTaskTable);

INCLUDE_ASM(const s32, "code_0017B0D8", mapNakamaTask);

void mdlSetMotionSpeed(s32 speed)
{
    mapNakamaTaskData = speed;
}

INCLUDE_ASM(const s32, "code_0017B0D8", mapNakamaWachLock_fld);

void mdlChkMotionEnd(void)
{
    mapNakamaTaskData2 = 0;
}

INCLUDE_ASM(const s32, "code_0017B0D8", mapNakamaAlphaFade);

INCLUDE_ASM(const s32, "code_0017B0D8", mapNakamaColorAnime);

INCLUDE_ASM(const s32, "code_0017B0D8", mapNakamaMinAlpDown);

INCLUDE_ASM(const s32, "code_0017B0D8", mapRefDvModel);

INCLUDE_ASM(const s32, "code_0017B0D8", mapNakamaWakeUp);

INCLUDE_ASM(const s32, "code_0017B0D8", mapNakamaPosRot);

INCLUDE_ASM(const s32, "code_0017B0D8", mapCallBtlGetMotionVisualDataSBtlVi);

INCLUDE_RODATA(const s32, "code_0017B0D8", mapNakamaPosRotTable);

INCLUDE_ASM(const s32, "code_0017B0D8", mapCallBtlGetMotionVisualDataSBtlVi2);

INCLUDE_ASM(const s32, "code_0017B0D8", mapNakamaSwapStart);

INCLUDE_ASM(const s32, "code_0017B0D8", fldPlayerMotion_Single);

INCLUDE_ASM(const s32, "code_0017B0D8", mapNakamaReturnStart);

INCLUDE_ASM(const s32, "code_0017B0D8", mapNakamaSkil_ClearBit);

INCLUDE_ASM(const s32, "code_0017B0D8", mapCallNakamaWakeUp3);

INCLUDE_ASM(const s32, "code_0017B0D8", mapCallNakamaWakeUp);

INCLUDE_ASM(const s32, "code_0017B0D8", mapNakamaZeniStart);

INCLUDE_ASM(const s32, "code_0017B0D8", mapNakamaHiroStart);

INCLUDE_ASM(const s32, "code_0017B0D8", mapCallNakamaWakeUp4);

INCLUDE_ASM(const s32, "code_0017B0D8", mapNakamaJoinStart);

INCLUDE_ASM(const s32, "code_0017B0D8", mapCallNakamaWakeUp5);

INCLUDE_ASM(const s32, "code_0017B0D8", mapCallNakamaWakeUp6);

INCLUDE_ASM(const s32, "code_0017B0D8", fldCallNakamaWakeUp);

INCLUDE_ASM(const s32, "code_0017B0D8", fldCallNakamaWakeUp2);

INCLUDE_ASM(const s32, "code_0017B0D8", fldFastTravelStart);

INCLUDE_ASM(const s32, "code_0017B0D8", mapIfToRaidouSelIni);

INCLUDE_ASM(const s32, "code_0017B0D8", mapProcSequenceSub3);

INCLUDE_ASM(const s32, "code_0017B0D8", mapNakamaSearchTask);

INCLUDE_ASM(const s32, "code_0017B0D8", mapCallNakamaWakeUp7);

INCLUDE_ASM(const s32, "code_0017B0D8", mapCallNakamaWakeUp2);
