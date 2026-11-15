#include "common.h"
#include "mapData.h"
#include "field.h"

s32 mapLeaf20(s16 uniq)
{
    s32 i;

    for (i = 0; i < MAP_POINT_MAX; i++)
    {
        if (mapData26[i].use != 0 && mapData26[i].uniq == uniq)
        {
            return mapData26[i].serialID;
        }
    }
    return -1;
}
