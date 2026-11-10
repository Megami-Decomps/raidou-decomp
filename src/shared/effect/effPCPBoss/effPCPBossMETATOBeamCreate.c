#include "shared/effect/effPCPBoss/decls.h"

effPCPBossMETATOBeam_t *effPCPBossMETATOBeamCreate(void *pParam)
{
    effPCPBossThunderParam_t thunder;
    effPCPBossMETATOBeam_t *work = sdfChipAllocDirect(sizeof(effPCPBossMETATOBeam_t));

    memcpy(work, pParam, 0x70);
    work->abgr = 0x80808080;
    work->frame = 0;
    memcpy(&thunder.head, work->unk10, 0x54);
    thunder.count = 4;
    work->polyLists[0] = effPCP_ThunderPolyListCreate(&thunder);
    work->polyLists[1] = effPCP_ThunderPolyListCreate(&thunder);
    return work;
}
