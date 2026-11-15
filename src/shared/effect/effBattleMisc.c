#include "common.h"
#include "effBattle.h"
#include "nb.h"
#include "event.h"
#include "model.h"
#include "draw3d.h"
#include "datCalc.h"
#include "ps2asm.h"
#include "sdfVu0.h"

extern void (*GetPositionPtrTable[])(void *owner, effBTLAimParams_t *request);
extern nbFormation_t *effBTLSkillGetInvokeFormation(void);
extern f32 aViewTarget[4];
extern f32 aViewEye[4];
extern f32 aViewDown[4];
extern f32 effBTL_Ptr_D[];
void effBTLPartGetPositionPtr(nbFormation_t *pFormation, effBTLAimParams_t *pPosPtr, sdfVector4_t *pPos);
void effBTLPartGetRotateMatrixPtr(effBTLAimParams_t *pPosPtr, sdfMatrix4_t *pMatrix);

#include "include_asm.h"
#include "newdata.h"
#include "sdf_resource.h"
#include "billboard.h"
#include "sdf_types.h"
#include "draw.h"
#include "libsdf.h"
#include "sdf_movie_types.h"
#include "effect.h"
#include "code_0027A1D0.h"
#include "effBattleMisc.h"

extern u32 effBTLFieldColorGetVariantSelector(void);

struct dds3Basic_t;

void effBattleMiscQueryMaskedTargetPosition(u32 actorFlagsMask, EffectVectorRequest *request);
void effBattleMiscQueryOriginalTargetPosition(u32 unused, EffectVectorRequest *request);
void effBattleMiscQueryVariantTargetPosition(u32 unused, EffectVectorRequest *request);

/* The selected position provider leaves its vector in vf10. */
void effBTLPartGetPositionPtr(nbFormation_t *pFormation, effBTLAimParams_t *pPosPtr, sdfVector4_t *pPos) {
    GetPositionPtrTable[pPosPtr->scaleKind](pFormation, pPosPtr);
    SDF_VU0_STOREVECTOR(pPos);
}

INCLUDE_ASM(const s32, "effect/effBattleMisc", nbGetUnitFromAction);

INCLUDE_ASM(const s32, "effect/effBattleMisc", effBTLPartGetTargetPartyPositionPtr);

void effLeaf36(void) {
}

void effLeaf37(void) {
}

void effLeaf38(void) {
}

void effLeaf39(void) {
}

void effBattleMiscQueryOriginalTargetPosition(u32 unused, EffectVectorRequest *request) {
    BtlUnit *ctx = (BtlUnit *)effBTLSkillGetInvokeFormation();

    (void)unused;
    effBattleMiscQueryMaskedTargetPosition(ctx->status.flags & 0xE00, request);
}

void effBattleMiscQueryVariantTargetPosition(u32 unused, EffectVectorRequest *request) {
    BtlUnit *ctx = (BtlUnit *)effBTLFieldColorGetVariantSelector();

    (void)unused;
    effBattleMiscQueryMaskedTargetPosition(ctx->status.flags & 0xE00, request);
}

INCLUDE_ASM(const s32, "effect/effBattleMisc", effBTLPartGetWorldPositionPtr);

void effLeaf40(void) {
}

INCLUDE_ASM(const s32, "effect/effBattleMisc", effLeaf41);

void effLeaf42(void) {
}

/* Build a 4x4 basis matrix into `out`: look-at for anchor kind 4 / direction mode 9, else a table-driven rotation. */
void effBTLPartGetRotateMatrixPtr(effBTLAimParams_t *pPosPtr, sdfMatrix4_t *pMatrix) {
    f32 angle = 0;
    f32 tilt = 0;

    if (pPosPtr->aimMode == 4 || pPosPtr->directionMode == 9) {
        LookAtMatrix(aViewTarget, aViewEye, aViewDown);
        ReverseMatrix();
        SDF_VU0_RESET_ROW3();
    } else {
        if (pPosPtr->aimMode == 5 || pPosPtr->aimMode == 3 || pPosPtr->directionMode == 8 || pPosPtr->directionMode == 10) {
            tilt = 3.14159265f / 2.0f;
        }
        if (pPosPtr->directionMode < 9) {
            angle = effBTL_Ptr_D[pPosPtr->directionMode];
        }
        SetRotateMatrixX(tilt);
        SetRotateMatrixY_B(angle);
        MultMatrixRevNoArg();
    }
    SDF_VU0_STOREMATRIX(pMatrix);
}
