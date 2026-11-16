#include "common.h"
#include "field.h"
#include "fld.h"
#include "ps2asm.h"
#include "basic.h"
#include "dds3CameraObjectBasic.h"
#include "dds3KernelDraw.h"

typedef struct fldCutEntry_t {
    s32 id;
    f32 targetX;
    f32 targetZ;
    f32 cameraY;
    f32 cameraX;
    f32 cameraZ;
    f32 targetY;
    f32 fovy;
} fldCutEntry_t;

extern fldCutEntry_t fldWork6[];

extern s32 mapData18[];
extern s8 dds3PadButtonState[];
extern f32 mapCutGetCamData;
extern sdfVector4_t mapCutSetCamData2;
extern sdfVector4_t mapCutSetCamData3;

extern dds3Basic_t *mapData97;
extern s32 effProcSequenceSub(void);

/* NON_MATCHING: 115 of 237 words match. Scheduling and the order of the dir branches differ. */
#ifdef NON_MATCHING
s32 mapCutGetCam(sdfVector4_t *targetPos, sdfVector4_t *cameraPos, f32 *fovy) {
    f32 table[8] = { -50.0f, -100.0f, -200.0f, -400.0f, -600.0f, -800.0f, -1000.0f, -2000.0f };
    fldCutEntry_t *entry;
    s32 result;
    s32 dir;

    if ((u32)(mapData18[0] - 1) < 4) {
        SDF_VU0_COPYVECTOR(targetPos, &mapCutSetCamData2);
        SDF_VU0_COPYVECTOR(cameraPos, &mapCutSetCamData3);
        return 1;
    }
    if (mapData18[0] == 5) {
        if (dds3PadButtonState[0x29] < 0 && fldGlobalBase.camZoom < 7) {
            fldGlobalBase.camZoom++;
        }
        if (dds3PadButtonState[0x2B] < 0 && fldGlobalBase.camZoom > 0) {
            fldGlobalBase.camZoom--;
        }
        if (dds3PadButtonState[0x2A] < 0) {
            fldGlobalBase.camDir = (fldGlobalBase.camDir + 3) % 4;
        }
        if (dds3PadButtonState[0x28] < 0) {
            fldGlobalBase.camDir = (fldGlobalBase.camDir + 1) % 4;
        }
        *fovy = 0.8198888f;
        mapCutGetCamData += (fldGlobalBase.focusY - mapCutGetCamData) * 0.25f;
        targetPos->x = fldGlobalBase.focusX + 0.0f;
        targetPos->y = mapCutGetCamData - 50.0f;
        targetPos->z = fldGlobalBase.focusZ + 0.0f;
        dir = fldGlobalBase.camDir;
        if (dir == 2) {
            cameraPos->x = fldGlobalBase.focusX + 0.0f;
            cameraPos->y = mapCutGetCamData + table[fldGlobalBase.camZoom];
            cameraPos->z = fldGlobalBase.focusZ + 1000.0f;
        } else if (dir >= 3) {
            if (dir == 3) {
                cameraPos->x = fldGlobalBase.focusX - 1000.0f;
                cameraPos->y = mapCutGetCamData + table[fldGlobalBase.camZoom];
                cameraPos->z = fldGlobalBase.focusZ;
            } else {
                goto north;
            }
        } else if (dir == 1) {
            cameraPos->x = fldGlobalBase.focusX + 1000.0f;
            cameraPos->y = mapCutGetCamData + table[fldGlobalBase.camZoom];
            cameraPos->z = fldGlobalBase.focusZ;
        } else {
north:
            cameraPos->x = fldGlobalBase.focusX + 0.0f;
            cameraPos->y = mapCutGetCamData + table[fldGlobalBase.camZoom];
            cameraPos->z = fldGlobalBase.focusZ - 1000.0f;
        }
        return 1;
    }
    result = fldGlobalBase.cutIdx < 128;
    entry = &fldWork6[(fldGlobalBase.amSBlock << 6) + fldGlobalBase.cutIdx];
    fldGlobalBase.camId = entry->id;
    targetPos->x = entry->cameraX * 100.0f;
    targetPos->y = entry->targetY * -100.0f;
    targetPos->z = entry->cameraZ * 100.0f;
    cameraPos->x = entry->targetX * 100.0f;
    cameraPos->y = entry->cameraY * -100.0f;
    cameraPos->z = entry->targetZ * 100.0f;
    *fovy = entry->fovy * 3.14f / 180.0f;
    return result;
}
#else
INCLUDE_ASM(const s32, "code_r1_001403E0", mapCutGetCam);
#endif

void mapCutSetCam(dds3Basic_t *pBasic, s32 mode) {
    sdfVector4_t target;
    sdfVector4_t camera;
    f32 fovy;

    fovy = 0;
    if (pBasic == NULL) {
        return;
    }
    mapData97 = pBasic;
    if (effProcSequenceSub()) {
        SDF_VU0_COPYVECTOR(&mapCutSetCamData2, &aViewTarget);
        SDF_VU0_COPYVECTOR(&mapCutSetCamData3, &aViewEye);
        SDF_VU0_COPYVECTOR(&target, &mapCutSetCamData2);
        SDF_VU0_COPYVECTOR(&camera, &mapCutSetCamData3);
        return;
    }
    if (mapData18[0] == 0 || mapData18[0] == 5) {
        SDF_VU0_COPYVECTOR(&mapCutSetCamData2, &aViewTarget);
        SDF_VU0_COPYVECTOR(&mapCutSetCamData3, &aViewEye);
    }
    if (mapCutGetCam(&target, &camera, &fovy) == 0 && mode == 0) {
        SDF_VU0_COPYVECTOR(&target, &mapCutSetCamData);
        SDF_VU0_COPYVECTOR(&camera, &mapObjTaskData);
    } else {
        dds3CameraObjectBasicSetTgtCam(pBasic, &target, &camera);
        SDF_VU0_COPYVECTOR(&mapCutSetCamData, &target);
        SDF_VU0_COPYVECTOR(&mapObjTaskData, &camera);
        if (fovy > 0.1f) {
            dds3SetScreenFovy(fovy);
        }
    }
}

INCLUDE_RODATA(const s32, "code_r1_001403E0", mapCutGetCamTable);

INCLUDE_SDATA(const s32, "code_r1_001403E0", mapCutGetCamData);

