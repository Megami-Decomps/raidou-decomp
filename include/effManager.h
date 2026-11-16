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
effHandle_t *dds3EffectCreate(u16 major, u16 minor, saddr_t pData);
void dds3EffectDataRevision(effResourceHeader_t *header);
void dds3EffectDelete(effHandle_t *pData);
void dds3EffectReset(effHandle_t *effect);
effHandle_t *dds3EffectLoad(effResourceHeader_t *pD3P);
void dds3EffectSetPosition(effHandle_t *pData, const void *pPos);
void dds3EffectSetScale(effHandle_t *effect, f32 scale);

void dds3EffectDraw_dds3Effect(effHandle_t *hEffect);
effHandle_t *dds3EffectLoadEPL(void *pEPL);
void dds3EffectManagerInit(void);
void dds3EffectSetLoop_dds3Effect(effHandle_t *hEffect, u8 loop);

extern s8 effProcSequenceSubData;
void effEctManagerInitSub(void);
s32 effProcSequenceSub(void);

#endif
