#ifndef EFFMANAGER_H
#define EFFMANAGER_H

#include "common.h"

/* An effect handle: the type's entry in EffMajorTable plus the instance that
 * entry's create returned. version is stamped from the data revision. */
typedef struct effHandle_t
{
    u32 type;
    u32 arg;
    s32 instance;
    f32 version;
} effHandle_t;

/* Effect resource header: the file data dds3EffectLoad is handed. */
typedef struct effResourceHeader_t
{
    u32 type;
    u32 arg;
    u32 unk08;
    f32 version;
    u8 payload[16];
} effResourceHeader_t;

effHandle_t *dds3EffectCopy(effHandle_t *node);
effHandle_t *dds3EffectCreate(u16 type, u16 arg, saddr_t param);
void dds3EffectDataRevision(effResourceHeader_t *header);
void dds3EffectDelete(effHandle_t *pData);
effHandle_t *dds3EffectLoad(effResourceHeader_t *pD3P);
void dds3EffectSetPosition(effHandle_t *pData, const void *pPos);

#endif
