#include "common.h"
#include "effBattle.h"
#include "nb.h"
#include "event.h"
#include "model.h"
#include "draw3d.h"
#include "datCalc.h"
#include "ps2asm.h"
#include "sdfVu0.h"
#include "dds3WorldBasic.h"
#include "dds3SceneBasic.h"
#include "dds3Basic.h"
#include "dds3CameraObjectBasic.h"

extern void (*GetPositionPtrTable[])(void *owner, effBTLAimParams_t *request);
extern nbFormation_t *effBTLSkillGetInvokeFormation(void);
extern f32 effBTL_Ptr_D[];
extern f32 effBTLAimVerticalScale[];
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

void effBattleMiscQueryMaskedTargetPosition(u32 actorFlagsMask, effVectorRequest_t *request);
void effBattleMiscQueryOriginalTargetPosition(u32 unused, effVectorRequest_t *request);
void effBattleMiscQueryVariantTargetPosition(u32 unused, effVectorRequest_t *request);

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

void effBattleMiscQueryOriginalTargetPosition(u32 unused, effVectorRequest_t *request) {
    SBTL_UNIT *ctx = (SBTL_UNIT *)effBTLSkillGetInvokeFormation();

    (void)unused;
    effBattleMiscQueryMaskedTargetPosition(ctx->status.flags & 0xE00, request);
}

void effBattleMiscQueryVariantTargetPosition(u32 unused, effVectorRequest_t *request) {
    SBTL_UNIT *ctx = (SBTL_UNIT *)effBTLFieldColorGetVariantSelector();

    (void)unused;
    effBattleMiscQueryMaskedTargetPosition(ctx->status.flags & 0xE00, request);
}

void effBTLPartGetWorldPositionPtr(SBTL_UNIT *unit, effBTLAimParams_t *request) {
    sdfVector4_t result;
    sdfVector4_t direction;
    f32 radius;
    f32 vertical;
    u32 aimMode;
    u32 directionMode;

    radius = 750.0f;
    aimMode = request->aimMode;
    directionMode = request->directionMode;
    if (request->rangeOverride != 0) {
        radius = (f32)request->rangeOverride;
    }

    if (directionMode == 9 || aimMode == 4) {
        dds3Basic_t *camera;

        camera = dds3GetActiveCameraSceneBasic(dds3GetActiveScene());
        direction.z = 750.0f;
        direction.y = 750.0f;
        direction.x = 750.0f;
        dds3CameraObjectBasicGetTargetPos(camera);
        SDF_VU0_COPYVECTOR_A_TO_B();
        dds3GetPosPoseSet(camera);
        SDF_VU0_SUBVECTOR();
        SDF_VU0_NORMALIZE();
        SDF_VU0_LOADVECTOR_B(&direction);
        SDF_VU0_MULVECTOR();
    } else {
        if (aimMode == 5) {
            if (directionMode == 8 || directionMode == 10) {
                vertical = -1.0f;
                if (request->rangeOverride != 0) {
                    vertical = -radius;
                }
            } else {
                vertical = -1.0f;
            }
        } else {
            vertical = effBTLAimVerticalScale[aimMode] * 250.0f;
        }

        if (directionMode == 8 || directionMode == 10) {
            result.y = vertical;
            result.x = 0.0f;
            result.z = 0.0f;
        } else {
            direction.x = 0.0f;
            direction.y = 0.0f;
            direction.z = (unit->status.flags & 0x200) ? 1.0f : -1.0f;
            SetRotateMatrixY(effBTL_Ptr_D[directionMode]);
            SDF_VU0_LOADVECTOR(&direction);
            SDF_VU0_APPLYMATRIX_XYZ();
            SDF_VU0_STOREVECTOR(&direction);
            result.x = radius * direction.x;
            result.z = radius * direction.z;
            result.y = radius * direction.y + vertical;
        }
        SDF_VU0_LOADVECTOR_MEMOPS(result);
    }
}

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
        LookAtMatrix(&aViewTarget, &aViewEye, &aViewDown);
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
