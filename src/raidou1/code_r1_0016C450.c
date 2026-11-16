#include "common.h"
#include "sdk_libc.h"
#include "fldFileResolver.h"
#include "mdlManager.h"
#include "sdfMath.h"
#include "fpu.h"
#include "sdfQuat.h"
#include "dds3obj.h"
#include "field.h"
#include "newdata.h"
#include "ps2asm.h"
#include "model.h"
#include "motion.h"
#include "basic.h"

extern s32 mapNakamaTaskData;
extern s32 mapNakamaTaskData2;
extern s32 mapNakamaTaskData3;
extern s32 mapNakamaTaskData4;
extern s32 mapNakamaTaskData5;
extern u32 mapNakamaTaskData6;
extern s32 mapNakamaTaskData7;
extern s32 mapNakamaTaskData8;
extern s32 mapNakamaMinAlpDownData;
extern s32 mapData140;
extern s32 mapData141;
extern s32 mapData142;
extern u32 mapData143;
extern u32 mapData144;
extern s32 mapData146;
extern f32 mapNakamaTaskFloat;
extern f32 mapNakamaTaskFloat2;
extern f32 map_summondv_rot;
extern sdfVector4_t mapData47;
extern sdfVector4Unaligned_t mapNakamaTaskTable;
extern dds3Basic_t *summonDvBasic;
extern mdlObj_t *summonDvModel;
extern void mdlChkMotionEnd(void);
extern void mdlSetMotionSpeed(s32 v);
extern s32 fldEncountTestFlag(u32 flag);
extern void mapNakamaMotion(mdlObj_t *model, u32 id, s32 motion, s32 mode);
extern f32 fldPointDistance(f32 x0, f32 y0, f32 z0, f32 x1, f32 y1, f32 z1);
extern void dds3GetRotPoseSet(dds3Basic_t *basic);
extern f32 R360_LR(f32 a, f32 b);
extern void dds3OnPoseSetFlag(dds3PoseSet_t *pose, u32 flag);
extern void mapCallEffectGetState2(void);
extern void mapCallEffectGetState3(void);

#define GRAY(a) (((a) << 24) | ((a) << 16) | ((a) << 8) | (a))

/* NON_MATCHING: 16 of 376 words match. Register allocation and branch layout differ. */
#ifdef NON_MATCHING
void mapNakamaTask(void) {
    sdfVector4_t rot;
    sdfVector4Unaligned_t axis;
    u32 color = 0x80808080;
    s32 changed = 0;
    s32 a;
    f32 dx;
    f32 dz;
    f32 target;
    f32 cur;
    f32 diff;
    f32 step;
    f32 next;
    dds3PoseSet_t *pose;

    memset(&rot, 0, sizeof(rot));
    axis = mapNakamaTaskTable;
    rot.w = 1.0f;
    if (fldGlobalBase.nakamaTaskOff != 0) {
        return;
    }
    if (DDS3_GBWK->mapSummonID <= 0) {
        mapNakamaTaskData = 0;
        mapSummonDvTask();
        return;
    }
    switch (mapNakamaTaskData) {
    case 0:
        mdlChkMotionEnd();
        break;
    case 1:
        mapSummonDvTask();
        break;
    case 2:
        if (fldEncountTestFlag(0x40)) {
            if (fldGlobalBase.summonStop != 0) {
                mdlSetMotionSpeed(5);
            } else {
                mdlSetMotionSpeed(1);
            }
        }
        break;
    case 3:
        if (fldEncountTestFlag(0x40)) {
            if (fldGlobalBase.summonStop != 0) {
                mdlSetMotionSpeed(5);
            } else {
                mdlSetMotionSpeed(1);
            }
            mdlChkMotionEnd();
        }
        break;
    default:
        break;
    }
    if (mapNakamaTaskData4 != 0 && summonDvModel != NULL) {
        if (mapNakamaTaskData5 < mapData142) {
            s32 step5 = mapNakamaTaskData5;
            u32 s = mapData144;
            u32 e = mapNakamaTaskData6;
            s32 ab = (s32)(s >> 24) + ((s32)(e >> 24) - (s32)(s >> 24)) * step5 / mapData142;
            s32 bb = (s32)((s >> 16) & 0xFF) + ((s32)((e >> 16) & 0xFF) - (s32)((s >> 16) & 0xFF)) * step5 / mapData142;
            s32 gb = (s32)((s >> 8) & 0xFF) + ((s32)((e >> 8) & 0xFF) - (s32)((s >> 8) & 0xFF)) * step5 / mapData142;
            s32 rb = (s32)(s & 0xFF) + ((s32)(e & 0xFF) - (s32)(s & 0xFF)) * step5 / mapData142;
            mapNakamaTaskData5++;
            color = (ab << 24) | (rb) | (gb << 8) | (bb << 16);
            mapData143 = color;
        } else {
            color = mapNakamaTaskData6;
        }
        changed = 1;
    }
    if (mapNakamaTaskData3 != 0 && summonDvModel != NULL) {
        a = mapData141 + mapNakamaTaskData3;
        if (a < 0) {
            mapData141 = 0;
            a = 0;
            mapNakamaTaskData3 = 0;
        } else {
            mapData141 = a;
        }
        if (a > 128) {
            a = 128;
            mapNakamaTaskData3 = 0;
            mapData141 = 128;
        }
        changed = 1;
        color = GRAY(a);
    } else {
        if (fldGlobalBase.summonStop == 0) {
            if (mapNakamaMinAlpDownData >= 0) {
                if (mapNakamaMinAlpDownData > 0) {
                    a = mapNakamaTaskData7 + 8;
                    if (a > 128) {
                        a = 128;
                    }
                    mapNakamaTaskData7 = a;
                }
            } else {
                a = mapNakamaTaskData7 - 8;
                if (a < 0) {
                    mapNakamaTaskData7 = 0;
                } else {
                    mapNakamaTaskData7 = a;
                }
            }
            if (mapNakamaMinAlpDownData != 0) {
                changed = 1;
                color = GRAY(mapNakamaTaskData7);
            }
        }
    }
    if (mapNakamaTaskData8 != 0 && summonDvBasic != NULL && fldGlobalBase.summonStop == 0) {
        if (mapNakamaTaskData8 == 1) {
            pose = summonDvBasic->poseset;
            if (fldPointDistance(fldGlobalBase.focusX, fldGlobalBase.focusY, fldGlobalBase.focusZ, pose->pos.x, pose->pos.y, pose->pos.z) > 100.0f) {
                mapNakamaTaskData8 = 2;
            }
        }
        if (mapNakamaTaskData8 == 2) {
            a = mapData146 + 8;
            if (a < 129) {
                mapData146 = a;
            } else {
                a = 128;
                mapNakamaTaskData8 = 0;
                mapData146 = 128;
            }
        } else {
            a = mapData146;
        }
        changed = 1;
        color = GRAY(a);
    }
    if (changed != 0 && summonDvModel != NULL) {
        mdlSetColor(summonDvModel, color);
    }
    if (mapData140 != -1 && summonDvModel != NULL && summonDvModel->first->stat == 5) {
        mapNakamaMotion(summonDvModel, DDS3_GBWK->mapSummonID, mapData140, 3);
    }
    if (mapNakamaTaskData2 != 0 && summonDvBasic != NULL) {
        pose = summonDvBasic->poseset;
        dx = pose->pos.x - mapData47.x;
        dz = pose->pos.z - mapData47.z;
        if (fsqrtf(dx * dx + dz * dz) > 10.0f) {
            target = -(sdfAtan2(dx, dz) * mapNakamaTaskFloat);
            dds3GetRotPoseSet(summonDvBasic);
            cur = map_summondv_rot;
            if (target < 0.0f) {
                target += 360.0f;
            }
            if (cur < 0.0f) {
                cur += 360.0f;
            }
            diff = R360_LR(cur, target);
            step = diff / 10.0f;
            if (step < 0.0f) {
                step = -step;
            }
            if (step < 2.0f) {
                step = 2.0f;
            }
            if (diff >= 0.0f && diff <= step) {
                next = target;
            } else if (diff <= 0.0f && -step <= diff) {
                next = target;
            } else if (diff < 0.0f) {
                next = cur - step;
            } else {
                next = cur + step;
            }
            map_summondv_rot = next;
            SDF_VU0_LOADVECTOR(&axis);
            FromRotation(next * mapNakamaTaskFloat2);
            SDF_VU0_STOREVECTOR(&rot);
            dds3SetRotPoseSet(summonDvBasic, &rot);
            dds3OnPoseSetFlag(summonDvBasic->poseset, 1);
            dds3OnPoseSetFlag(summonDvBasic->poseset, 8);
        }
    }
    mapCallEffectGetState2();
    mapCallEffectGetState3();
}
#else
INCLUDE_RODATA(const s32, "code_r1_0016C450", mapNakamaTaskTable);

INCLUDE_ASM(const s32, "code_r1_0016C450", mapNakamaTask);
#endif
