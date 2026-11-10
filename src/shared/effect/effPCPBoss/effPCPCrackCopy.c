#include "shared/effect/effPCPBoss/decls.h"

effPCPCrack_t *effPCPCrackCopy(effPCPCrack_t *src)
{
    effPCPCrack_t *work;

    work = sdfChipAllocDirect(sizeof(effPCPCrack_t));
    memcpy(work, src, sizeof(effCrackHead_t));
    work->frame = 0;
    work->abgr = 0x80808080;
    work->part = dds3EffectPartCopy(src->part);
    effPCPCrackInit(work);
    return work;
}
