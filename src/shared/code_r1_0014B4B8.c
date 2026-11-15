#include "common.h"
#include "mapData.h"
#include "field.h"

s32 mapObjPointGet(s16 uniq, f32 *x, f32 *y, f32 *z, f32 *r)
{
    s32 i;

    for (i = 0; i < MAP_POINT_MAX; i++)
    {
        if (mapData26[i].use != 0 && mapData26[i].uniq == uniq)
        {
            *x = mapData26[i].x;
            *y = mapData26[i].y;
            *z = mapData26[i].z;
            *r = mapData26[i].r;
            return 1;
        }
    }
    datDEBUGCONSOLE("NoData!! Point:%04x [MAP:%d FLOOR:%d]\n", uniq, fldGlobalBase.amSBlock, fldGlobalBase.nameU);
    *x = 0.0f;
    *y = 0.0f;
    *z = 0.0f;
    *r = 0.0f;
    return 0;
}
