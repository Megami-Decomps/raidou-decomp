#include "common.h"
#include "fpu.h"
#include "fld.h"
#include "code_r1_00128378.h"
#include "sdk_libc.h"
#include "mapData.h"
#include "field.h"

/* NON_MATCHING: 0 of 541 words match. Functionally checked against the asm; instruction scheduling and stack layout are not yet matched (573 words emitted vs 542). */
#ifdef NON_MATCHING
void mapHitWorkCalcSetup(s32 mode, f32 cx, f32 cz)
{
    MAP_WORK_HIT_LIST_t tmp;
    s32 attr[9];
    f32 pos[9][2];
    MAP_WORK_HIT_LIST_t *e;
    f32 min;
    f32 mx;
    f32 mz;
    s32 handled;
    s32 a;
    s32 i;
    s32 j;
    s32 k;

    memset(attr, 0, sizeof(attr));
    map_work_hit_list_num = 0;
    mapHitWorkCalcSetupSub(&pos[0][0], &pos[0][1], cx, cz);
    if (mode != 0)
    {
        attr[0] = mapGetAttr_Btl(pos[0][0], pos[0][1]);
    }
    else
    {
        attr[0] = mapGetAttr(pos[0][0], pos[0][1]);
    }
    mapHitWorkCalcSetupSub(&pos[1][0], &pos[1][1], cx - 100.0f, cz + 100.0f);
    if (mode != 0)
    {
        attr[1] = mapGetAttr_Btl(pos[1][0], pos[1][1]);
    }
    else
    {
        attr[1] = mapGetAttr(pos[1][0], pos[1][1]);
    }
    attr[1] &= 0xF5;
    mapHitWorkCalcSetupSub(&pos[2][0], &pos[2][1], cx, cz + 100.0f);
    if (mode != 0)
    {
        attr[2] = mapGetAttr_Btl(pos[2][0], pos[2][1]);
    }
    else
    {
        attr[2] = mapGetAttr(pos[2][0], pos[2][1]);
    }
    attr[2] &= 0xF7;
    mapHitWorkCalcSetupSub(&pos[3][0], &pos[3][1], cx + 100.0f, cz + 100.0f);
    if (mode != 0)
    {
        attr[3] = mapGetAttr_Btl(pos[3][0], pos[3][1]);
    }
    else
    {
        attr[3] = mapGetAttr(pos[3][0], pos[3][1]);
    }
    attr[3] &= 0xF6;
    mapHitWorkCalcSetupSub(&pos[4][0], &pos[4][1], cx - 100.0f, cz);
    if (mode != 0)
    {
        attr[4] = mapGetAttr_Btl(pos[4][0], pos[4][1]);
    }
    else
    {
        attr[4] = mapGetAttr(pos[4][0], pos[4][1]);
    }
    attr[4] &= 0xFD;
    mapHitWorkCalcSetupSub(&pos[5][0], &pos[5][1], cx + 100.0f, cz);
    if (mode != 0)
    {
        attr[5] = mapGetAttr_Btl(pos[5][0], pos[5][1]);
    }
    else
    {
        attr[5] = mapGetAttr(pos[5][0], pos[5][1]);
    }
    attr[5] &= 0xFE;
    mapHitWorkCalcSetupSub(&pos[6][0], &pos[6][1], cx - 100.0f, cz - 100.0f);
    if (mode != 0)
    {
        attr[6] = mapGetAttr_Btl(pos[6][0], pos[6][1]);
    }
    else
    {
        attr[6] = mapGetAttr(pos[6][0], pos[6][1]);
    }
    attr[6] &= 0xF9;
    mapHitWorkCalcSetupSub(&pos[7][0], &pos[7][1], cx, cz - 100.0f);
    if (mode != 0)
    {
        attr[7] = mapGetAttr_Btl(pos[7][0], pos[7][1]);
    }
    else
    {
        attr[7] = mapGetAttr(pos[7][0], pos[7][1]);
    }
    attr[7] &= 0xFB;
    mapHitWorkCalcSetupSub(&pos[8][0], &pos[8][1], cx + 100.0f, cz - 100.0f);
    if (mode != 0)
    {
        attr[8] = mapGetAttr_Btl(pos[8][0], pos[8][1]);
    }
    else
    {
        attr[8] = mapGetAttr(pos[8][0], pos[8][1]);
    }
    attr[8] &= 0xFA;
    for (k = 0; k < 9; k++)
    {
        a = attr[k];
        handled = 0;
        if (a & 0x10)
        {
            e = &mapData34[map_work_hit_list_num++];
            e->line_x0 = pos[k][0];
            e->line_z0 = pos[k][1];
            e->line_x1 = pos[k][0] + 100.0f;
            e->line_z1 = pos[k][1] - 100.0f;
            mx = (e->line_x1 + pos[k][0]) * 0.5f;
            mz = (e->line_z1 + pos[k][1]) * 0.5f;
            e->line_c_len = fsqrtf((mx - cx) * (mx - cx) + (mz - cz) * (mz - cz));
            mapGetTaniVector(-0.5f, -0.5f, &e->line_vx, &e->line_vz);
            e->type = 1;
            handled = 1;
        }
        else if (a & 0x20)
        {
            e = &mapData34[map_work_hit_list_num++];
            e->line_x0 = pos[k][0];
            e->line_z0 = pos[k][1];
            e->line_x1 = pos[k][0] + 100.0f;
            e->line_z1 = pos[k][1] - 100.0f;
            mx = (e->line_x1 + pos[k][0]) * 0.5f;
            mz = (e->line_z1 + pos[k][1]) * 0.5f;
            e->line_c_len = fsqrtf((mx - cx) * (mx - cx) + (mz - cz) * (mz - cz));
            mapGetTaniVector(0.5f, 0.5f, &e->line_vx, &e->line_vz);
            e->type = 1;
            handled = 1;
        }
        else if (a & 0x40)
        {
            e = &mapData34[map_work_hit_list_num++];
            e->line_x0 = pos[k][0] + 100.0f;
            e->line_z0 = pos[k][1];
            e->line_x1 = e->line_x0 - 100.0f;
            e->line_z1 = pos[k][1] - 100.0f;
            mx = (e->line_x1 + e->line_x0) * 0.5f;
            mz = (e->line_z1 + pos[k][1]) * 0.5f;
            e->line_c_len = fsqrtf((mx - cx) * (mx - cx) + (mz - cz) * (mz - cz));
            mapGetTaniVector(0.5f, -0.5f, &e->line_vx, &e->line_vz);
            e->type = 1;
            handled = 1;
        }
        else if (a & 0x80)
        {
            e = &mapData34[map_work_hit_list_num++];
            e->line_x0 = pos[k][0] + 100.0f;
            e->line_z0 = pos[k][1];
            e->line_x1 = e->line_x0 - 100.0f;
            e->line_z1 = pos[k][1] - 100.0f;
            mx = (e->line_x1 + e->line_x0) * 0.5f;
            mz = (e->line_z1 + pos[k][1]) * 0.5f;
            e->line_c_len = fsqrtf((mx - cx) * (mx - cx) + (mz - cz) * (mz - cz));
            mapGetTaniVector(-0.5f, 0.5f, &e->line_vx, &e->line_vz);
            e->type = 1;
            handled = 1;
        }
        if (!handled)
        {
            if (a & 1)
            {
                e = &mapData34[map_work_hit_list_num++];
                e->line_x0 = pos[k][0];
                e->line_z0 = pos[k][1];
                e->line_x1 = pos[k][0];
                e->line_z1 = pos[k][1] - 100.0f;
                e->line_vx = 1.0f;
                e->line_vz = 0.0f;
                e->type = 0;
                mx = (pos[k][0] + pos[k][0]) * 0.5f;
                mz = (e->line_z1 + pos[k][1]) * 0.5f;
                e->line_c_len = fsqrtf((mx - cx) * (mx - cx) + (mz - cz) * (mz - cz));
            }
            if (a & 2)
            {
                e = &mapData34[map_work_hit_list_num++];
                e->line_x0 = pos[k][0] + 100.0f;
                e->line_z0 = pos[k][1];
                e->line_x1 = e->line_x0;
                e->line_z1 = pos[k][1] - 100.0f;
                e->line_vx = -1.0f;
                e->line_vz = 0.0f;
                e->type = 0;
                mx = (e->line_x0 + e->line_x0) * 0.5f;
                mz = (e->line_z1 + pos[k][1]) * 0.5f;
                e->line_c_len = fsqrtf((mx - cx) * (mx - cx) + (mz - cz) * (mz - cz));
            }
            if (a & 4)
            {
                e = &mapData34[map_work_hit_list_num++];
                e->line_x0 = pos[k][0];
                e->line_z0 = pos[k][1];
                e->line_x1 = pos[k][0] + 100.0f;
                e->line_z1 = pos[k][1];
                e->line_vx = 0.0f;
                e->line_vz = -1.0f;
                e->type = 0;
                mx = (e->line_x1 + pos[k][0]) * 0.5f;
                mz = (pos[k][1] + pos[k][1]) * 0.5f;
                e->line_c_len = fsqrtf((mx - cx) * (mx - cx) + (mz - cz) * (mz - cz));
            }
            if (a & 8)
            {
                e = &mapData34[map_work_hit_list_num++];
                e->line_x0 = pos[k][0];
                e->line_z0 = pos[k][1] - 100.0f;
                e->line_x1 = pos[k][0] + 100.0f;
                e->line_z1 = e->line_z0;
                e->line_vx = 0.0f;
                e->line_vz = 1.0f;
                e->type = 0;
                mx = (e->line_x1 + pos[k][0]) * 0.5f;
                mz = (e->line_z0 + e->line_z0) * 0.5f;
                e->line_c_len = fsqrtf((mx - cx) * (mx - cx) + (mz - cz) * (mz - cz));
            }
        }
    }
    if (map_work_hit_list_num >= 2)
    {
        min = mapData34[0].line_c_len;
        for (i = 0; i < map_work_hit_list_num - 1; i++)
        {
            for (j = i + 1; j < map_work_hit_list_num; j++)
            {
                if (mapData34[j].line_c_len < min)
                {
                    tmp = mapData34[i];
                    mapData34[i] = mapData34[j];
                    mapData34[j] = tmp;
                    min = mapData34[i].line_c_len;
                }
            }
        }
    }
}
#else
INCLUDE_ASM(const s32, "code_r1_00142038", mapHitWorkCalcSetup);
#endif

/* NON_MATCHING: 80 of 88 words match. Argument setup is scheduled differently in both loops. */
#ifdef NON_MATCHING
s32 mapHitWorkCalc(f32 newx, f32 newz, f32 r, f32 *retx, f32 *retz)
{
    f32 hitx;
    f32 hitz;
    s32 hit;
    s32 i;
    MAP_WORK_HIT_LIST_t *p;

    hit = 0;
    i = 0;
    while (i < map_work_hit_list_num)
    {
        p = &mapData34[i++];
        if (mapHamidasiWHitSubSub(&hitx, &hitz, newx, newz, r, p->line_x0, p->line_z0, p->line_x1, p->line_z1, p->line_vx, p->line_vz))
        {
            newx = hitx;
            newz = hitz;
            hit = 1;
        }
    }
    for (i = 0; i < map_work_hit_list_num; i++)
    {
        if (mapHamidasiMoveBoxHitSub(&hitx, &hitz, newx, newz, r, mapData34[i].line_x0, mapData34[i].line_z0, mapData34[i].line_x1, mapData34[i].line_z1, mapData34[i].line_vx, mapData34[i].line_vz))
        {
            newx = hitx;
            newz = hitz;
            hit = 1;
            break;
        }
    }
    *retx = newx;
    *retz = newz;
    return hit;
}
#else
INCLUDE_ASM(const s32, "code_r1_00142038", mapHitWorkCalc);
#endif

INCLUDE_SDATA(const s32, "code_r1_00142038", map_work_hit_list_num);

