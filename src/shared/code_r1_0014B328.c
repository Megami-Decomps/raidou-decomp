#include "common.h"
#include "dds3PointBasic.h"
#include "sdfQuat.h"
#include "ps2asm.h"
#include "mapData.h"
#include "field.h"

/* NON_MATCHING: 16 of 92 words match. Retail strength-reduces the entry address into five registers (frame 0x50); mine keeps one pointer (frame 0x30). */
#ifdef NON_MATCHING
void mapObjPointAdd2(s16 uniq, s16 floor, f32 x, f32 y, f32 z, f32 r)
{
    s32 i;
    sdfVector4Unaligned_t axis = mapObjPointAdd2Table;
    MAP_POINT_t *p;

    for (i = 0; i < MAP_POINT_MAX; i++)
    {
        p = &mapData26[i];
        if (p->use == 0)
        {
            p->use = 1;
            p->x = x;
            p->uniq = uniq;
            p->y = y;
            p->z = z;
            p->floor = floor;
            p->r = r;
            p->basic = NULL;
            p->resource.pos.x = x;
            p->resource.pos.y = y;
            p->resource.pos.z = z;
            p->resource.pos.w = 1.0f;
            SDF_VU0_LOADVECTOR(&axis);
            FromRotation((r + 180.0f) * 3.14f / 180.0f);
            SDF_VU0_STOREVECTOR(&p->resource.rot);
            p->resource.scl.w = 1.0f;
            p->resource.scl.x = 1.0f;
            p->resource.scl.y = 1.0f;
            p->resource.scl.z = 1.0f;
            p->serialID = dds3SerializeID();
            p->basic = dds3AddPointBasic(p->serialID, &p->resource, "mappos");
            return;
        }
    }
}
#else
INCLUDE_ASM(const s32, "code_r1_0014B328", mapObjPointAdd2);
#endif

INCLUDE_RODATA(const s32, "code_r1_0014B328", mapObjPointAdd2Table);

INCLUDE_SDATA(const s32, "code_r1_0014B328", mapObjPointAdd2Data);

