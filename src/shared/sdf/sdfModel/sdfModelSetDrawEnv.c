#include "shared/sdf/sdfModel/decls.h"

void sdfModelSetDrawEnv(sdfModelDrawEnv_t *aEnv)
{
    sdfModelDrawEnvSet_t *set = aEnv->set;
    s32 i;

    sdfPrimSetOpaquePacket(&aEnv->set[0].transPacket);
    sdfPrimSetSemiTransPacket(&aEnv->set[1].transPacket);
    sdfPrimSetAddTransPacket(&aEnv->set[2].transPacket);
    sdfPrimSetSubTransPacket(&aEnv->set[3].transPacket);
    for (i = 0; i != 4; i++) {
        set->transPacket.header.dmatag.dat0 = 0x11000000;
        sdfDrawTagInit(&set->drawTag);
        sdfDrawTagLink(&set->drawTag, EE_ADDR(&set->transPacket));
        set++;
    }
    sdfDrawTagInit(&aEnv->term.drawTag);
    *(u64 *)&aEnv->term.footer = 0;
    *(u64 *)&aEnv->term.footer.dat0 = 0x13000000;
    sdfDrawTagLink(&aEnv->term.drawTag, EE_ADDR(&aEnv->term.footer));
}
