#ifndef DDS_SDF_GS_SCENE_STATE_H

#define DDS_SDF_GS_SCENE_STATE_H
#include "sdf_gs_packet.h"

/* A+D writes that draw the centered viewport bounds as a sprite. */
typedef struct SdfGsCenteredBoundsRegisters {
    SdfGsRegisterWrite test;
    SdfGsRegisterWrite primitive;
    SdfGsRegisterWrite rgbaq;
    SdfGsRegisterWrite xyz2[2];
} SdfGsCenteredBoundsRegisters;

typedef char SdfGsCenteredBoundsRegisters_size_must_be_0x50[
    (sizeof(SdfGsCenteredBoundsRegisters) == 0x50) ? 1 : -1];

/* Shared A+D state for GS primitive modes, color clamping, dithering and TEXA. */
typedef struct SdfGsDrawDefaultsRegisters {
    SdfGsRegisterWrite prmodecont;
    SdfGsRegisterWrite colclamp;
    SdfGsRegisterWrite dthe;
    SdfGsRegisterWrite texa;
} SdfGsDrawDefaultsRegisters;

typedef char SdfGsDrawDefaultsRegisters_size_must_be_0x40[
    (sizeof(SdfGsDrawDefaultsRegisters) == 0x40) ? 1 : -1];

/* TEST/ALPHA A+D state for the two independent GS drawing contexts. */
typedef struct SdfGsSceneBlendRegisters {
    sdfPrimTransElem_t contextOne;
    sdfPrimTransElem_t contextTwo;
} SdfGsSceneBlendRegisters;

typedef char SdfGsSceneBlendRegisters_size_must_be_0x40[
    (sizeof(SdfGsSceneBlendRegisters) == 0x40) ? 1 : -1];

#endif
