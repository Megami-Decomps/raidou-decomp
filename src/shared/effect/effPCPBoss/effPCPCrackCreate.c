#include "shared/effect/effPCPBoss/decls.h"

effPCPCrack_t *effPCPCrackCreate(effPCPCrackParam_t *pParam, void *pPB)
{
    effPCPCrack_t *work = sdfChipAllocDirect(0xAC);

    *(effCrackHead_t *)work = pParam->head;
    work->abgr = 0x80808080;
    work->frame = 0;
    work->part = dds3EffectPartCreate(3, pPB);
    effPCPCrackInit(work);
    return work;
}
