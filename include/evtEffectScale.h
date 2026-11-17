#ifndef EVT_EFFECT_SCALE_H
#define EVT_EFFECT_SCALE_H

#include "common.h"
#include "effEvent.h"

/* Party/enemy model-table entry; only the scale field is established. */
typedef struct EvtModelScaleEntry {
    u8 pad00[0x18];
    f32 modelScale; /* 0x18 */
    u8 pad1C[0x254];
} EvtModelScaleEntry;

/* Workdata of a BED effect object; only the cut handle used for scaling is established. */
typedef struct EvtEffectInner {
    u8 pad00[0x2C];
    effEveBEDCut_t *scaledObject; /* 0x2C */
} EvtEffectInner;

typedef char EvtModelScaleEntry_size_check[(sizeof(EvtModelScaleEntry) == 0x270) ? 1 : -1];

#endif
