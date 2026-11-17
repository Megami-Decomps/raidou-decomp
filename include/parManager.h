#ifndef PARMANAGER_H
#define PARMANAGER_H

#include "common.h"
#include "sdf_types.h"
#include "sdfRand.h"

extern sdfRandCB_t ParRand;

void dds3ParticleDelete(void *handle);
void dds3ParticleSetPosition(void *handle, const sdfVector4_t *pPos);
void dds3ParticleSetRGBA(void *handle, const sdfVector4_t *abgr);

struct dds3Particle_BlurBuff_t;
void dds3Particle_BlurClearItem(struct dds3Particle_BlurBuff_t *table, s32 slotIndex);

#endif
