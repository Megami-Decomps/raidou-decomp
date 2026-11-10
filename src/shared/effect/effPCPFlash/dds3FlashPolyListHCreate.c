#include "shared/effect/effPCPFlash/decls.h"

dds3FlashPolyList_t *dds3FlashPolyListHCreate(u32 face) {
    dds3FlashPolyList_t *pool = dds3FlashPolyListCreate(face);

    SDF_VU0_STOREIDENTITY(pool->matrix);
    return pool;
}
