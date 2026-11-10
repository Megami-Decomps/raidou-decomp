#ifndef DDS_FLD_QUAD_STATE_H

#define DDS_FLD_QUAD_STATE_H
#include "common.h"

#include "sdf.h"

/* Stack state shared by the field resolver's positioned quad helpers. */
typedef struct FldQuadState {
    s32 unk0;
    s32 unk4;
    s32 unk8;
    s32 unkC;
    s32 unk10;
    s32 unk14;
    s32 unk18;
    s32 rowX;
    s32 rowY;
    s32 drawDepth;
    sdfDrawTag_t *packetList;
} FldQuadState;

typedef char FldQuadState_size_must_be_0x2C[
    sizeof(FldQuadState) == 0x2C ? 1 : -1];

#endif
