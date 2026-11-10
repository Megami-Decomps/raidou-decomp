#include "shared/effect/effPCPMisc/decls.h"

effPCPHwave01_tag *effPCPHwave01Create(void *src) {
    effPCPHwave01_tag *dst;
    effPartHandle_t *handle;

    dst = sdfChipAllocDirect(sizeof(effPCPHwave01_tag));
    dst->hPB = dds3EffectPCPDataPartCreate(src, 0);
    handle = dds3EffectPCPDataPartCreate(src, 1);
    dst->pos[0] = 0;
    dst->hD3P = handle;
    dst->pos[1] = 0;
    dst->abgr = 0x80808080;
    dst->pos[2] = 0;
    return dst;
}
