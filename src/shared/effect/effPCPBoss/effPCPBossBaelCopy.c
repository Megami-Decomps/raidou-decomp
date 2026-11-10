#include "shared/effect/effPCPBoss/decls.h"

effPCPBossBael_t *effPCPBossBaelCopy(effPCPBossBael_t *hData, s32 bPos, s32 tbldata)
{
    effPCPBossBael_t *work;

    work = sdfChipAllocDirect(sizeof(effPCPBossBael_t));
    memcpy(&work->param, &hData->param, 0x10);
    work->frame = 0;
    work->abgr = 0x80808080;
    work->hPB = dds3EffectPartCopy(hData->hPB);
    work->hPCF = dds3EffectPartCopy(hData->hPCF);
    return work;
}
