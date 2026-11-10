#include "shared/effect/effPCPBoss/decls.h"

effPCPBossBael_t *effPCPBossBaelCreate(void *pParam, void *pPB, void *pPCF)
{
    effPCPBossBael_t *work;

    work = sdfChipAllocDirect(sizeof(effPCPBossBael_t));
    memcpy(&work->param, pParam, 0x10);
    work->abgr = 0x80808080;
    work->frame = 0;
    work->hPB = dds3EffectPartCreate(3, pPB);
    work->hPCF = dds3EffectPartCreate(6, pPCF);
    return work;
}
