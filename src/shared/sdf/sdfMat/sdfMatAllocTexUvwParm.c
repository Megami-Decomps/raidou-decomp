#include "shared/sdf/sdfMat/decls.h"

sdfMatUvwParm_t *sdfMatAllocTexUvwParm(void)
{
    sdfMatUvwParm_t *parm;

    parm = sdfChipAllocDirect(sizeof(sdfMatUvwParm_t));
    /* Tile (1, 1), offset (0, 0), angle 0, written as the three quadword halves. */
    *(u64 *)&parm->tile[0] = 0x3F8000003F800000ULL;
    *(u64 *)&parm->offset[0] = 0;
    *(u32 *)&parm->angle = 0;
    return parm;
}
