#ifndef DDS_MDL_MARK_PARAMS_H

#define DDS_MDL_MARK_PARAMS_H
#include "common.h"

/* Marker-record fields edited by the model viewer; the leading word stays opaque. */
typedef struct EffMarkParams {
    u8 opaque00[8];
    s32 mark0;
    s32 mark1;
    u16 start;
    u16 end;
    u16 interval;
    u8 face;
    u8 blend;
    u32 colors[4];
} EffMarkParams;

typedef char EffMarkParams_size_must_be_0x28[
    (sizeof(EffMarkParams) == 0x28) ? 1 : -1];

#endif /* DDS_MDL_MARK_PARAMS_H */
