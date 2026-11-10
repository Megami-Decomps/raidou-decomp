#include "shared/effect/effPCPMisc/decls.h"

effPCPHwave01_tag *effPCPHwave01Copy(effPCPHwave01_tag *src) {
    effPCPHwave01_tag *dst;
    effPartHandle_t *handle;

    dst = sdfChipAllocDirect(sizeof(effPCPHwave01_tag));
    dst->hPB = dds3EffectPartCopy(src->hPB);
    handle = dds3EffectPartCopy(src->hD3P);
    dst->pos[0] = 0;
    dst->hD3P = handle;
    dst->pos[1] = 0;
    dst->abgr = 0x80808080;
    dst->pos[2] = 0;
    return dst;
}
