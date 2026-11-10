#include "shared/basic/dds3Basic/decls.h"

/* vu0 routine: return the position in vf10 with homogeneous w = 1. */
void dds3GetPosPoseSet(dds3Basic_t *pBasic)
{
    sdfVector4_t *position = &pBasic->poseset->pos;

    SDF_VU0_LOADVECTOR_BARRIER(position);
    SDF_VU0_VECTOR_W_SET_ONE();
}
