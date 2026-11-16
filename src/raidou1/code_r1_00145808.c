#include "common.h"
#include "dds3Randomize.h"
#include "dds3obj.h"
#include "dds3KernelDraw.h"
#include "dds3ObjectBase.h"
#include "sdfQuat.h"
#include "sdk_libc.h"
#include "ps2asm.h"
#include "field.h"
#include "fld.h"
#include "sdfMath.h"
#include "mapData.h"
#include "model.h"
#include "motion.h"

extern s32 fldEncountTestFlag(u32 flag);
extern f32 fldPointDistance(f32 x0, f32 y0, f32 z0, f32 x1, f32 y1, f32 z1);
extern void dds3OnPoseSetFlag(dds3PoseSet_t *pose, u32 flag);
extern s32 sdfModelGetMarkPos_sdfModel(sdfModelRootNode_t *root, s32 mark);
extern void sdfModelGetMarkPosMatrix_sdfModel(sdfModelRootNode_t *root, s32 mark);
extern void mapSoundKankyoTask(s32 map, s32 floor, s32 cut);
extern f32 mapCalcLen(f32 x0, f32 y0, f32 z0, f32 x1, f32 y1, f32 z1);
extern s32 mapSeVolCalc(f32 len, f32 maxLen, f32 minLen, s32 baseVol);
extern void mapSoundMoveSeVol(s32 seId, s32 vol);
extern s32 mapBigClipChkF(s32 cut, f32 x, f32 y, f32 z);
extern void eglLinkPoly4(f32 x0, f32 y0, f32 z0, f32 x1, f32 y1, f32 z1, f32 x2, f32 y2, f32 z2, f32 x3, f32 y3, f32 z3, u32 c0, u32 c1, u32 c2, u32 c3);
extern s32 mapObjTaskData12;
extern sdfVector4_t mapObjTaskData4[5][8];
extern const sdfVector4Unaligned_t mapObjTaskTable;
extern u32 mapCtnColor;

/* NON_MATCHING: 14 of 2754 words match. Compiles; helpers are static inline. Draft emits 2224 words vs 2754: retail unrolls the vector memset loop, repeats the case 5 rotation call in both branches and keeps one more door tail copy (gcc cross-jumps two in mine). Call counts match otherwise; behaviour checked against asm and IDA. */
#ifdef NON_MATCHING
static inline s32 mapObjTaskSetColor(MAP_OBJ_t *o)
{
    mapObjColor(o->uniq, mapCtnColor);
    if ((mapCtnColor & 0xFF000000) != 0)
    {
        mapObjVisible(o->uniq, 1);
        return 0;
    }
    mapObjVisible(o->uniq, 0);
    return 1;
}

static inline void mapObjTaskSetLight(dds3Basic_t *basic, f32 x, f32 z, s32 defaultLight)
{
    s32 light;

    light = mapCallSetPlayerTargetData3(x, z);
    if (light == 0)
    {
        light = defaultLight;
    }
    mapUnitSetLight(basic, light, 15);
}

static inline f32 mapObjTaskCamLen(f32 x, f32 y, f32 z)
{
    return mapCalcLen((mapObjTaskData.x + fldGlobalBase.focusX) * 0.5f, (mapObjTaskData.y + fldGlobalBase.focusY) * 0.5f,
                      (mapObjTaskData.z + fldGlobalBase.focusZ) * 0.5f, x, y, z);
}

static inline void mapObjTaskDist3(f32 px, f32 py, f32 pz, const sdfVector4_t *v, f32 *d1, f32 *d2, f32 *d3)
{
    *d1 = fldPointDistance(px, py, pz, v[2].x, v[2].y, v[2].z);
    *d2 = fldPointDistance(px, py, pz, v[3].x, v[3].y, v[3].z);
    *d3 = fldPointDistance(px, py, pz, v[4].x, v[4].y, v[4].z);
}

static inline void mapObjTaskPlayerDist3(const sdfVector4_t *v, f32 *d1, f32 *d2, f32 *d3)
{
    if (fldGlobalBase.summonStop == 0)
    {
        mapObjTaskDist3(fldGlobalBase.focusX, fldGlobalBase.focusY, fldGlobalBase.focusZ, v, d1, d2, d3);
    }
    else
    {
        mapObjTaskDist3(fldGlobalBase.float184, fldGlobalBase.float188, fldGlobalBase.float18C, v, d1, d2, d3);
    }
}

static inline f32 mapObjTaskMotionFrame(s16 uniq)
{
    mdlObj_t *model;

    model = dds3ObjectBaseGetModelHandle(mapObjGetBasic(uniq));
    return model->first->frame;
}

/* Slot rows 0-3 are the corners, 4-7 the axes; v[2..4] are the three trigger points. */
static inline void mapObjTaskDoorSetup(MAP_OBJ_t *o, dds3PoseSet_t *pose, sdfVector4_t *v, s32 slot, f32 dir)
{
    sdfVector4_t *row;

    row = mapObjTaskData4[slot];
    o->x = pose->pos.x;
    o->y = pose->pos.y;
    o->z = pose->pos.z;
    v[0].y = 0.0f;
    v[2].z = o->z;
    v[0].z = o->z - 50.0f;
    v[0].x = o->x + 170.0f;
    v[2].y = o->y;
    v[3].y = o->y;
    v[4].y = o->y;
    v[2].x = o->x + dir * 200.0f;
    v[3].x = o->x + dir * 350.0f;
    v[4].x = o->x + dir * 500.0f;
    v[3].z = o->z;
    v[4].z = o->z;
    SDF_VU0_LOADVECTOR(&v[0]);
    SDF_VU0_STOREVECTOR(&row[0]);
    v[0].y = 0.0f;
    v[0].x = o->x - 170.0f;
    v[0].z = o->z - 50.0f;
    SDF_VU0_LOADVECTOR(&v[0]);
    SDF_VU0_STOREVECTOR(&row[1]);
    v[0].y = 0.0f;
    v[0].x = o->x + 170.0f;
    v[0].z = o->z + 50.0f;
    SDF_VU0_LOADVECTOR(&v[0]);
    SDF_VU0_STOREVECTOR(&row[2]);
    v[0].y = 0.0f;
    v[0].x = o->x - 170.0f;
    v[0].z = o->z + 50.0f;
    SDF_VU0_LOADVECTOR(&v[0]);
    SDF_VU0_STOREVECTOR(&row[3]);
    v[0].x = 0.0f;
    v[0].y = 0.0f;
    v[0].z = -1.0f;
    SDF_VU0_LOADVECTOR(&v[0]);
    SDF_VU0_NORMALIZE();
    SDF_VU0_STOREVECTOR(&row[4]);
    v[0].x = -1.0f;
    v[0].y = 0.0f;
    v[0].z = 0.0f;
    SDF_VU0_LOADVECTOR(&v[0]);
    SDF_VU0_NORMALIZE();
    SDF_VU0_STOREVECTOR(&row[5]);
    v[0].x = 0.0f;
    v[0].y = 0.0f;
    v[0].z = 1.0f;
    SDF_VU0_LOADVECTOR(&v[0]);
    SDF_VU0_NORMALIZE();
    SDF_VU0_STOREVECTOR(&row[6]);
    v[0].x = 1.0f;
    v[0].y = 0.0f;
    v[0].z = 0.0f;
    SDF_VU0_LOADVECTOR(&v[0]);
    SDF_VU0_NORMALIZE();
    SDF_VU0_STOREVECTOR(&row[7]);
}

/* Door state for the two-sided doors: the other doors' triggers, then the player's. */
static inline s32 mapObjTaskDoorState(MAP_OBJ_t *o, sdfVector4_t *v, s16 refUniq)
{
    s32 state;
    f32 frame;
    f32 d1;
    f32 d2;
    f32 d3;
    dds3PoseSet_t *pose;

    state = o->user_param[0];
    frame = mapObjTaskMotionFrame(-16384);
    if (o->user_param[0] != 0)
    {
        if (!(o->x < 2800.0f && o->x > 2300.0f && frame > 1000.0f && frame < 1200.0f))
        {
            state = 0;
        }
    }
    else if (o->x < 2800.0f && o->x > 2300.0f && frame > 1000.0f && frame < 1200.0f)
    {
        state = 1;
    }
    if (state != 0)
    {
        return state;
    }
    frame = mapObjTaskMotionFrame(-16383);
    if (o->user_param[0] != 0)
    {
        state = 0;
        if (o->x < 2800.0f && o->x > 2300.0f && frame > 700.0f)
        {
            state = frame < 900.0f;
        }
    }
    else if (o->x < 2800.0f && o->x > 2300.0f && frame > 700.0f && frame < 900.0f)
    {
        state = 1;
    }
    if (state != 0)
    {
        return state;
    }
    pose = mapObjGetBasic(refUniq)->poseset;
    mapObjTaskDist3(pose->pos.x, pose->pos.y, pose->pos.z, v, &d1, &d2, &d3);
    if (d1 < 100.0f || d2 < 100.0f || d3 < 100.0f)
    {
        state = 1;
    }
    else if (d2 <= 100.0f || d3 <= 600.0f)
    {
        state = o->user_param[0] == 1;
    }
    if (state != 0)
    {
        return state;
    }
    mapObjTaskPlayerDist3(v, &d1, &d2, &d3);
    if (d1 < 120.0f)
    {
        state = 1;
    }
    else if (d2 > 120.0f && d3 > 120.0f)
    {
        state = 0;
    }
    else if (o->user_param[0] == 1)
    {
        state = 1;
    }
    return state;
}

/* Applies a door state change, then the colour, light and sound updates shared by the doors. Returns the hidden flag. */
static inline s32 mapObjTaskDoorUpdate(MAP_OBJ_t *o, s32 state, s32 id, sdfVector4_t *objPos)
{
    s32 hidden;
    dds3PoseSet_t *pose;

    if (state != o->user_param[0])
    {
        if (state != 0)
        {
            mapObjTaskSub(id, 1);
            mapObjMotionCng(o->uniq, 0, 1);
            o->user_param[0] = 1;
        }
        else
        {
            mapObjTaskSub(id, 0);
            mapObjMotionCng(o->uniq, 1, 1);
            o->user_param[0] = 0;
        }
    }
    hidden = mapObjTaskSetColor(o);
    pose = o->basic->poseset;
    mapObjTaskSetLight(o->basic, pose->pos.x, pose->pos.z, 2);
    SDF_VU0_COPYVECTOR(objPos, &pose->pos);
    if (o->user_param[0] == 0)
    {
        f32 len;

        len = mapObjTaskCamLen(objPos->x, objPos->y, objPos->z);
        mapSoundMoveSeVol(49, mapSeVolCalc(len, 4000.0f, 500.0f, 112));
    }
    return hidden;
}

void mapObjTask(void)
{
    sdfVector4_t markPos;
    sdfVector4_t objPos;
    sdfVector4Unaligned_t axis;
    sdfVector4_t rot;
    sdfVector4_t v[5];
    f32 angle;
    f32 d1;
    f32 d2;
    f32 d3;
    s32 i;
    s32 k;
    s32 state;
    s32 hidden;
    s32 slot;
    MAP_OBJ_t *o;
    dds3Basic_t *basic;
    dds3PoseSet_t *pose;
    mdlObj_t *model;
    u32 rand;
    f32 len;
    f32 speed;
    s32 vol;

    memset(&markPos, 0, sizeof(markPos));
    markPos.w = 1.0f;
    memset(&objPos, 0, sizeof(objPos));
    objPos.w = 1.0f;
    axis = mapObjTaskTable;
    memset(&rot, 0, sizeof(rot));
    rot.w = 1.0f;
    for (k = 0; k < 5; k++)
    {
        memset(&v[k], 0, sizeof(v[k]));
        v[k].w = 1.0f;
    }
    mapObjTaskData12++;
    if (fldGlobalBase.texture2B0 != NULL && fldGlobalBase.cutIdx < 64)
    {
        fldGlobalBase.float2B4 = fldGlobalBase.float2B4 - 0.049999997f / mapData33[fldGlobalBase.cutIdx].skyScale;
        if (fldGlobalBase.float2B4 >= 512.0f)
        {
            fldGlobalBase.float2B4 = fldGlobalBase.float2B4 - 512.0f;
        }
        if (fldGlobalBase.float2B4 < 0.0f)
        {
            fldGlobalBase.float2B4 = fldGlobalBase.float2B4 + 512.0f;
        }
        angle = sdfAtan2(mapObjTaskData.x - mapCutSetCamData.x, mapCutSetCamData.z - mapObjTaskData.z) * 57.324841f;
        if (angle < 0.0f)
        {
            angle = angle + 360.0f;
        }
        fldGlobalBase.float2B8 = angle * 1024.0f / 360.0f;
        if (fldGlobalBase.float2B8 >= 512.0f)
        {
            fldGlobalBase.float2B8 = fldGlobalBase.float2B8 - 512.0f;
        }
        if (fldGlobalBase.float2B8 < 0.0f)
        {
            fldGlobalBase.float2B8 = fldGlobalBase.float2B8 + 512.0f;
        }
    }
    mapSoundKankyoTask(fldGlobalBase.amSBlock, fldGlobalBase.nameU, fldGlobalBase.cutIdx);
    mapNpcPathTask();
    for (i = 127; i >= 0; i--)
    {
        o = &mapObjTaskData2[127 - i];
        if (o->use != 1 || o->basic == NULL || o->npc_idx <= 0 || o->model_id1 != 3)
        {
            continue;
        }
        if ((o->model_id2 >= 98 || o->model_id2 == 93) && o->model_id2 != 105)
        {
            continue;
        }
        if (o->npc_kuse_time > 0)
        {
            o->npc_kuse_time++;
            if (mapObjMotionEndCheck(o->uniq))
            {
                rand = dds3GetRandA(NULL);
                mapObjNpcKuse(o->uniq, 60 * ((rand >> 4) % 12) + 360);
                mapObjMotionCng(o->uniq, 0, 1);
            }
        }
        if (!fldEncountTestFlag(0x40))
        {
            continue;
        }
        if (o->npc_kuse_wait > 0)
        {
            o->npc_kuse_wait--;
        }
        if (!dds3ObjectBaseCheckFlag(o->basic, 1))
        {
            if (o->npc_kuse_wait != 0 || o->npc_kuse_time != 0)
            {
                continue;
            }
            model = dds3ObjectBaseGetModelHandle(o->basic);
            if (model->current.h.motionIndex != 0)
            {
                continue;
            }
            o->npc_kuse_time = 1;
            mapObjMotionCng(o->uniq, 2, 2);
        }
        else if (o->npc_kuse_wait < 10 && o->npc_kuse_time == 0)
        {
            rand = dds3GetRandA(NULL);
            mapObjNpcKuse(o->uniq, 60 * ((rand >> 4) % 12) + 360);
        }
    }
    if (fldGlobalBase.amSBlock == 8 && (u32)fldGlobalBase.nameU < 2)
    {
        if (fldGlobalBase.nameU != 0)
        {
            mapObjPos(-8192, 0.0f, 0.0f, -100.0f);
        }
        else
        {
            mapObjPos(-8192, 0.0f, 0.0f, 0.0f);
        }
    }
    for (i = 0; i < 128; i++)
    {
        o = &mapObjTaskData2[i];
        if (o->use != 1 || o->basic == NULL)
        {
            continue;
        }
        basic = o->basic;
        switch (o->spcial)
        {
            case 4:
                pose = basic->poseset;
                o->x = pose->pos.x;
                o->y = pose->pos.y;
                o->z = pose->pos.z;
                hidden = mapObjTaskSetColor(o);
                mapObjTaskSetLight(basic, o->x, o->z, 0);
                len = mapObjTaskCamLen(o->x, o->y, o->z);
                mapSoundMoveSeVol(50, mapSeVolCalc(len, 2000.0f, 300.0f, 112));
                if (hidden)
                {
                    break;
                }
                if (fldGlobalBase.cutIdx == 1 || fldGlobalBase.cutIdx == 27 || fldGlobalBase.cutIdx == 13 ||
                    fldGlobalBase.cutIdx == 30)
                {
                    dds3ObjectBaseOnFlag(o->basic, 1);
                }
                else if (mapBigClipChkF(fldGlobalBase.cutIdx, o->x, o->y, o->z) != 0)
                {
                    dds3ObjectBaseOffFlag(o->basic, 1);
                }
                else
                {
                    dds3ObjectBaseOnFlag(o->basic, 1);
                }
                break;
            case 5:
                pose = basic->poseset;
                if (fldGlobalBase.focusZ > -2500.0f)
                {
                    pose->pos.x = 3500.0f;
                    pose->pos.z = -1950.0f;
                    SDF_VU0_LOADVECTOR(&axis);
                    FromRotation(3.14f);
                }
                else
                {
                    pose->pos.x = 3200.0f;
                    pose->pos.z = -2800.0f;
                    SDF_VU0_LOADVECTOR(&axis);
                    FromRotation(0.0f);
                }
                SDF_VU0_STOREVECTOR(&rot);
                dds3SetRotPoseSet(basic, &rot);
                dds3OnPoseSetFlag(basic->poseset, 1);
                mapUnitSetLight(basic, mapCallSetPlayerTargetData3(basic->poseset->pos.x, basic->poseset->pos.z), 15);
                eglSetOt(23);
                eglLinkTest(1, 1, 128, 3, 0, 0, 1, 2);
                eglLinkAlpha(1);
                pose = basic->poseset;
                eglLinkPoly4(pose->pos.x - 1000.0f, 4.0f, pose->pos.z + 1000.0f, pose->pos.x + 1000.0f, 4.0f,
                             pose->pos.z + 1000.0f, pose->pos.x - 1000.0f, 4.0f, pose->pos.z - 1000.0f,
                             pose->pos.x + 1000.0f, 4.0f, pose->pos.z - 1000.0f, 0x808080, 0x808080, 0x808080,
                             0x808080);
                eglLinkTest(1, 5, 128, 1, 0, 0, 1, 2);
                eglLinkAlpha(0);
                break;
            case 6:
            case 8:
                pose = basic->poseset;
                if (pose->pos.z - 3.0f < 3000.0f)
                {
                    pose->pos.z = 9000.0f;
                }
                else
                {
                    pose->pos.z = pose->pos.z - 3.0f;
                }
                SDF_VU0_COPYVECTOR(&pose->pos2, &pose->pos);
                dds3OnPoseSetFlag(pose, 1);
                dds3OnPoseSetFlag(basic->poseset, 8);
                eglSetOt(38);
                eglLinkTest(1, 1, 128, 3, 0, 0, 1, 2);
                eglLinkAlpha(1);
                pose = basic->poseset;
                eglLinkPoly4(pose->pos.x - 1200.0f, 0.0f, pose->pos.z + 1200.0f, pose->pos.x + 1200.0f, 0.0f,
                             pose->pos.z + 1200.0f, pose->pos.x - 1200.0f, 0.0f, pose->pos.z - 1200.0f,
                             pose->pos.x + 1200.0f, 0.0f, pose->pos.z - 1200.0f, 0x808080, 0x808080, 0x808080,
                             0x808080);
                eglLinkTest(1, 5, 128, 1, 0, 0, 1, 2);
                eglLinkAlpha(0);
                break;
            case 7:
            case 9:
                pose = basic->poseset;
                if (pose->pos.z + 3.0f > 9000.0f)
                {
                    pose->pos.z = 3000.0f;
                }
                else
                {
                    pose->pos.z = pose->pos.z + 3.0f;
                }
                SDF_VU0_COPYVECTOR(&pose->pos2, &pose->pos);
                dds3OnPoseSetFlag(pose, 1);
                dds3OnPoseSetFlag(basic->poseset, 8);
                eglSetOt(38);
                eglLinkTest(1, 1, 128, 3, 0, 0, 1, 2);
                eglLinkAlpha(1);
                pose = basic->poseset;
                eglLinkPoly4(pose->pos.x - 1200.0f, 0.0f, pose->pos.z + 1200.0f, pose->pos.x + 1200.0f, 0.0f,
                             pose->pos.z + 1200.0f, pose->pos.x - 1200.0f, 0.0f, pose->pos.z - 1200.0f,
                             pose->pos.x + 1200.0f, 0.0f, pose->pos.z - 1200.0f, 0x808080, 0x808080, 0x808080,
                             0x808080);
                eglLinkTest(1, 5, 128, 1, 0, 0, 1, 2);
                eglLinkAlpha(0);
                break;
            case 10:
                mapObjTaskDoorSetup(o, basic->poseset, v, 2, 1.0f);
                mapObjTaskPlayerDist3(v, &d1, &d2, &d3);
                state = o->user_param[0];
                if (d1 < 120.0f)
                {
                    state = 1;
                }
                else if (120.0f < d2 && 120.0f < d3)
                {
                    state = 0;
                }
                hidden = mapObjTaskDoorUpdate(o, state, 1, &objPos);
                if (fldGlobalBase.cutIdx == 13)
                {
                    dds3ObjectBaseOnFlag(o->basic, 0x100000);
                }
                else if (!hidden)
                {
                    dds3ObjectBaseOffFlag(o->basic, 0x100000);
                }
                break;
            case 11:
                mapObjTaskDoorSetup(o, basic->poseset, v, 3, -1.0f);
                state = mapObjTaskDoorState(o, v, -16126);
                hidden = mapObjTaskDoorUpdate(o, state, 2, &objPos);
                if (fldGlobalBase.cutIdx == 13)
                {
                    dds3ObjectBaseOnFlag(o->basic, 0x100000);
                }
                else if (!hidden)
                {
                    dds3ObjectBaseOffFlag(o->basic, 0x100000);
                }
                break;
            case 12:
                mapObjTaskDoorSetup(o, basic->poseset, v, 4, -1.0f);
                state = mapObjTaskDoorState(o, v, -16127);
                hidden = mapObjTaskDoorUpdate(o, state, 3, &objPos);
                if (o->z < -2800.0f)
                {
                    dds3ObjectBaseOnFlag(o->basic, 0x100000);
                    break;
                }
                if (fldGlobalBase.cutIdx == 13)
                {
                    dds3ObjectBaseOnFlag(o->basic, 0x100000);
                }
                else if (!hidden)
                {
                    dds3ObjectBaseOffFlag(o->basic, 0x100000);
                }
                break;
            case 13:
            case 14:
                model = dds3ObjectBaseGetModelHandle(basic);
                slot = o->spcial != 13;
                if (sdfModelGetMarkPos_sdfModel(model->inner, 0))
                {
                    SDF_VU0_STOREVECTOR(&markPos);
                    sdfModelGetMarkPosMatrix_sdfModel(model->inner, 0);
                    v[0].x = -510.0f;
                    SDF_VU0_LOADVECTOR(&v[0]);
                    SDF_VU0_APPLYMATRIX();
                    SDF_VU0_STOREVECTOR(&v[1]);
                    SDF_VU0_STOREVECTOR(&v[2]);
                    v[0].x = -650.0f;
                    SDF_VU0_LOADVECTOR(&v[0]);
                    SDF_VU0_APPLYMATRIX();
                    SDF_VU0_STOREVECTOR(&v[3]);
                    v[0].x = -370.0f;
                    SDF_VU0_LOADVECTOR(&v[0]);
                    SDF_VU0_APPLYMATRIX();
                    SDF_VU0_STOREVECTOR(&v[4]);
                    v[0].x = -450.0f;
                    v[0].z = 100.0f;
                    SDF_VU0_LOADVECTOR(&v[0]);
                    SDF_VU0_APPLYMATRIX();
                    SDF_VU0_STOREVECTOR(&mapObjTaskData4[slot][0]);
                    v[0].x = 450.0f;
                    v[0].z = 100.0f;
                    SDF_VU0_LOADVECTOR(&v[0]);
                    SDF_VU0_APPLYMATRIX();
                    SDF_VU0_STOREVECTOR(&mapObjTaskData4[slot][1]);
                    v[0].x = -450.0f;
                    v[0].z = -100.0f;
                    SDF_VU0_LOADVECTOR(&v[0]);
                    SDF_VU0_APPLYMATRIX();
                    SDF_VU0_STOREVECTOR(&mapObjTaskData4[slot][2]);
                    v[0].x = 450.0f;
                    v[0].z = -100.0f;
                    SDF_VU0_LOADVECTOR(&v[0]);
                    SDF_VU0_APPLYMATRIX();
                    SDF_VU0_STOREVECTOR(&mapObjTaskData4[slot][3]);
                    v[0].x = 0.0f;
                    v[0].z = 1.0f;
                    v[0].y = 0.0f;
                    SDF_VU0_LOADVECTOR(&v[0]);
                    SDF_VU0_APPLYMATRIX();
                    SDF_VU0_LOADVECTOR_B(&markPos);
                    SDF_VU0_SUBVECTOR();
                    SDF_VU0_NORMALIZE();
                    SDF_VU0_STOREVECTOR(&mapObjTaskData4[slot][4]);
                    v[0].x = 1.0f;
                    v[0].y = 0.0f;
                    v[0].z = 0.0f;
                    SDF_VU0_LOADVECTOR(&v[0]);
                    SDF_VU0_APPLYMATRIX();
                    SDF_VU0_LOADVECTOR_B(&markPos);
                    SDF_VU0_SUBVECTOR();
                    SDF_VU0_NORMALIZE();
                    SDF_VU0_STOREVECTOR(&mapObjTaskData4[slot][5]);
                    v[0].x = 0.0f;
                    v[0].y = 0.0f;
                    v[0].z = -1.0f;
                    SDF_VU0_LOADVECTOR(&v[0]);
                    SDF_VU0_APPLYMATRIX();
                    SDF_VU0_LOADVECTOR_B(&markPos);
                    SDF_VU0_SUBVECTOR();
                    SDF_VU0_NORMALIZE();
                    SDF_VU0_STOREVECTOR(&mapObjTaskData4[slot][6]);
                    v[0].x = -1.0f;
                    v[0].y = 0.0f;
                    v[0].z = 0.0f;
                    SDF_VU0_LOADVECTOR(&v[0]);
                    SDF_VU0_APPLYMATRIX();
                    SDF_VU0_LOADVECTOR_B(&markPos);
                    SDF_VU0_SUBVECTOR();
                    SDF_VU0_NORMALIZE();
                    SDF_VU0_STOREVECTOR(&mapObjTaskData4[slot][7]);
                }
                mapObjTaskPlayerDist3(v, &d1, &d2, &d3);
                if (d1 < 140.0f || d3 < 140.0f)
                {
                    if (o->motion_pause_flag == 0)
                    {
                        mapObjMotionPause(o->uniq, 1);
                        o->motion_pause_spd = 0.0f;
                    }
                }
                else if (d2 > 240.0f)
                {
                    if (o->motion_pause_flag == 1)
                    {
                        mapObjMotionPause(o->uniq, 0);
                    }
                    speed = o->motion_pause_spd + 0.005f;
                    if (speed > 0.5f)
                    {
                        speed = 0.5f;
                    }
                    if (o->motion_pause_spd < 0.5f)
                    {
                        mapObjMotionSpeed(o->uniq, speed);
                    }
                }
                else
                {
                    speed = o->motion_pause_spd - 0.01f;
                    if (speed < 0.2f)
                    {
                        speed = 0.2f;
                    }
                    mapObjMotionSpeed(o->uniq, speed);
                }
                hidden = mapObjTaskSetColor(o);
                mapObjTaskSetLight(basic, markPos.x, markPos.z, 2);
                SDF_VU0_COPYVECTOR(&objPos, &markPos);
                len = mapObjTaskCamLen(objPos.x, objPos.y, objPos.z);
                vol = mapSeVolCalc(len, 5000.0f, 500.0f, 112);
                vol = (s32)((f32)vol * (o->motion_pause_spd + o->motion_pause_spd));
                if (vol > 0)
                {
                    mapSoundMoveSeVol(48, vol);
                }
                if (fldGlobalBase.cutIdx == 5 || fldGlobalBase.cutIdx == 9)
                {
                    dds3ObjectBaseOnFlag(o->basic, 0x100000);
                }
                else if (!hidden)
                {
                    dds3ObjectBaseOffFlag(o->basic, 0x100000);
                }
                break;
            default:
                break;
        }
    }
    mapSoundMoveSeTask();
}
#else
INCLUDE_RODATA(const s32, "code_r1_00145808", mapObjTaskTable);

INCLUDE_ASM(const s32, "code_r1_00145808", mapObjTask);
#endif

INCLUDE_SDATA(const s32, "code_r1_00145808", mapObjTaskData12);

