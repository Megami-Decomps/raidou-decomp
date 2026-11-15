#ifndef PARMANAGER_H
#define PARMANAGER_H

#include "common.h"
#include "sdf_types.h"

void dds3ParticleDelete(void *handle);
void dds3ParticleSetPosition(void *handle, const sdfVector4_t *pPos);
void dds3ParticleSetRGBA(void *handle, const sdfVector4_t *abgr);

#endif
