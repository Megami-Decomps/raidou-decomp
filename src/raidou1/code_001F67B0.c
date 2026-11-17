#include "common.h"
#include "effManager.h"
#include "effModelTrackPoly.h"

INCLUDE_ASM(const s32, "code_001F67B0", effReturnEventSetupWord);

INCLUDE_ASM(const s32, "code_001F67B0", effEventSetupRecordNoOp);

INCLUDE_ASM(const s32, "code_001F67B0", effEventSetupBeforeRecordNoOp);

INCLUDE_ASM(const s32, "code_001F67B0", effEventSetupAfterRecordNoOp);

INCLUDE_ASM(const s32, "code_001F67B0", effLeaf48);

void effEctManagerInitSub(void) {
}

/* body: matched in raidou @ 61f09dc: src/shared/effect/effBattleMisc.c:78, match, same function 1.00 (words 2/2, calls 1.00, shape 1.00) */
void effLeaf49(void) {
}

s32 effProcSequenceSub(void) {
    return effProcSequenceSubData;
}

INCLUDE_ASM(const s32, "code_001F67B0", effLeaf50);

INCLUDE_ASM(const s32, "code_001F67B0", effLeaf51);

INCLUDE_ASM(const s32, "code_001F67B0", effLeaf52);

effTrackPolyWork_t *effTrackPolyCreateWork(EffTrackPolyParams *src) {
    effTrackPolyWork_t *dst = sdfChipAllocDirect(sizeof(effTrackPolyWork_t));

    dst->params = *src;
    dst->params.unk1C = 0xC;
    dst->updateCount = 0;
    dst->data = dds3HermitianTrackPolyCreate(dst->params.historyLength, 0xC);
    dds3HermitianTrackPolySetVC(dst->data, src->gradientColors);
    dds3HermitianTrackPolySetBlend(dst->data, src->drawKind);
    return dst;
}

INCLUDE_ASM(const s32, "code_001F67B0", effBTLSys_Nego_Kyusyu_Delete2);

void effLeaf53(effTrackPolyWork_t *work) {
    work->updateCount = 0;
    dds3HermitianTrackPolyReset(work->data);
}
