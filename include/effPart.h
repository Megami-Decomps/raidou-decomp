#ifndef EFFPART_H
#define EFFPART_H

#include "common.h"

/* An effect part: its type and the object dds3EffectPartCreate made for it. */
typedef struct effPartHandle_t {
    u16 type;
    void *pHandle;
} effPartHandle_t;

effPartHandle_t *dds3EffectPartCopy(effPartHandle_t *hEffPartSrc);
effPartHandle_t *dds3EffectPartCreate(u16 type, void *pData);
void dds3EffectPartDelete(effPartHandle_t *hEffPart);
void dds3EffectPartDraw(effPartHandle_t *hEffPart);

#endif
