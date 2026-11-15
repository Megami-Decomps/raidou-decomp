#include "common.h"
#include "effBattle.h"
#include "nb.h"
#include "event.h"
#include "model.h"
#include "draw3d.h"
#include "datCalc.h"
#include "ps2asm.h"

extern void (*GetPositionPtrTable[])(void *owner, effBTLAimParams_t *request);
extern nbFormation_t *effBTLSkillGetInvokeFormation(void);
extern f32 aViewTarget[4];
extern f32 aViewEye[4];
extern f32 aViewDown[4];
extern f32 effBTL_Ptr_D[];
extern void LookAtMatrix(const void *aTarget, const void *aEye, const void *aDown);
extern void ReverseMatrix(void);
extern void SetRotateMatrixX(f32 aAngle);
extern void SetRotateMatrixY_B(f32 aAngle);
extern void MultMatrixRevNoArg(void);
void effBTLPartGetPositionPtr(nbFormation_t *pFormation, effBTLAimParams_t *pPosPtr, sdfVector4_t *pPos);
void effBTLPartGetRotateMatrixPtr(effBTLAimParams_t *pPosPtr, sdfMatrix4_t *pMatrix);

#ifndef VERSION_DDS1
#define VERSION_DDS1 1
#endif

#include "dds/sdf_projection.h"
#include "dds/common.h"
#include "dds/sdf_vu0.h"
#include "dds/sdf_vu_rotation_axes.h"
#include "dds/dds3obj.h"
#include "dds/btl_effect_position.h"
#include "dds/pcp_vu0.h"
#include "dds/sdf_vu_transform_api.h"
#include "dds/sdf_vu_matrix.h"
#include "dds/sdf_vu_matrix_multiply_api.h"
#include "dds/btl.h"
#include "dds/eff.h"
#include "dds/eff_field_color.h"
#include "dds/eff_battle_transition_duration.h"

extern u32 effBTLFieldColorGetVariantSelector(void);

#include "dds/dds3_world_primary.h"

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
