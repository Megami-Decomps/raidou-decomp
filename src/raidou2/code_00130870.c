#include "common.h"
#include "fldProcess.h"
#include "dds3CameraObjectBasic.h"
#include "field.h"
#include "dds3Basic.h"
#include "ps2asm.h"
#include "mapData.h"
#include "fld.h"

INCLUDE_ASM(const s32, "code_00130870", fldCallElvReset);

INCLUDE_ASM(const s32, "code_00130870", fldCallElvReset2);

INCLUDE_ASM(const s32, "code_00130870", fldBtlDebugDevilNameListDrawSub);

INCLUDE_ASM(const s32, "code_00130870", fldTitle_Shutdown);

INCLUDE_ASM(const s32, "code_00130870", fldCallMotionPauseOff);

INCLUDE_ASM(const s32, "code_00130870", mapRetuenFromSave);

INCLUDE_ASM(const s32, "code_00130870", mapRetuenFromLoad);

INCLUDE_ASM(const s32, "code_00130870", mapRetuenFromNakamaDead);

INCLUDE_ASM(const s32, "code_00130870", mapRetuenFromNakamaAna);

INCLUDE_ASM(const s32, "code_00130870", sbtlEpSc);

s32 mapExitFieldSub(void) {
    return ProcSequenceData4;
}

INCLUDE_ASM(const s32, "code_00130870", sbtlReLoadFldScene);

INCLUDE_ASM(const s32, "code_00130870", sbtlRefMapRndEncLoadMode);

INCLUDE_ASM(const s32, "code_00130870", mapRndEncReturn_BgmLoadEndCheck);

INCLUDE_ASM(const s32, "code_00130870", ProcMskSub_A);

INCLUDE_ASM(const s32, "code_00130870", ProcMskSub_B);

INCLUDE_ASM(const s32, "code_00130870", ProcRandomEncountCheck);

INCLUDE_ASM(const s32, "code_00130870", mapKiretaLabel);

INCLUDE_ASM(const s32, "code_00130870", CutSet);

INCLUDE_ASM(const s32, "code_00130870", mapIdouGo_Idx);

void mapIsFldKeyFreeSub3(void) {
}

INCLUDE_RODATA(const s32, "code_00130870", mapIdouGo_IdxTable);

