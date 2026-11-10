#include "shared/effect/effPCPBoss/decls.h"

void effPCPCrackSizeInit(effPCPCrack_t *pPCPCrack, effPCPCrackPartFlash_t *pSize)
{
    f32 blend = pPCPCrack->offsetRandomness;
    f32 scale = pPCPCrack->scale;
    f32 t;

    pSize->length = pPCPCrack->offsetDistance *
        (dds3GetRandFloatA(dds3EffectBase) * blend + (1.0f - blend)) * scale;
    blend = pPCPCrack->extentRandomness;
    t = dds3GetRandFloatA(dds3EffectBase) * blend + (1.0f - blend);
    pSize->width_in = pPCPCrack->baseExtent * t * scale;
    pSize->width_out = pPCPCrack->tipExtent * t * scale;
    pSize->type = dds3GetRandA(dds3EffectBase) & 1;
    pSize->frame = -(dds3GetRandA(dds3EffectBase) % pPCPCrack->delaySpread);
}
