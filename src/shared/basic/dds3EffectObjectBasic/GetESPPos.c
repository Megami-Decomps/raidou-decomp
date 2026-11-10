#include "shared/basic/dds3EffectObjectBasic/decls.h"

s32 GetESPPos(dds3Basic_t *b)
{
    if (CheckESP(b) == NULL) {
        return 0;
    }
    SDF_VU0_LOADVECTOR(&b->poseset->pos);
    return 1;
}
