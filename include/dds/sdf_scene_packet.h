#ifndef DDS_SDF_SCENE_PACKET_H

#define DDS_SDF_SCENE_PACKET_H
#include "sdf.h"

#include "sdf_packet_patch.h"

#include "sdf_gs_context.h"

#include "sdf_gs_scene_state.h"

struct sdfGrFrameSet_t;

/* Complete DMA/GIF owner used by scene builders and held-texture draw banks. */
typedef struct SdfSceneDrawPacket {
    SdfGsPacketHeader header;
    SdfGsDrawDefaultsRegisters drawDefaults;
    SdfGsContextRegisters contextOne;
    SdfGsContextRegisters contextTwo;
    SdfGsCenteredBoundsRegisters centeredBounds;
    SdfGsSceneBlendRegisters blendState;
} SdfSceneDrawPacket;

typedef char SdfSceneDrawPacket_size_must_be_0x170[
    (sizeof(SdfSceneDrawPacket) == 0x170) ? 1 : -1];

/* Linked metadata precedes the DMA drawing payload by one quadword. */
typedef struct SdfSceneNode {
    sdfPreDrawHook_t link; /* option0 holds the sdfGrFrameSet_t */
    SdfGsPacketHeader header;
    SdfGsDrawDefaultsRegisters drawDefaults;
    SdfGsContextRegisters contextOne;
    SdfGsContextRegisters contextTwo;
    SdfGsCenteredBoundsRegisters centeredBounds;
    SdfGsSceneBlendRegisters blendState;
    SdfGsRegisterWrite frameContextOne; /* 0x180: FRAME_1 with FBMSK 0x00FFFFFF. */
    SdfGsRegisterWrite frameContextTwo; /* 0x190: FRAME_2 with FBMSK 0x00FFFFFF. */
    sdfTexPacket_t texturePackets[2];
} SdfSceneNode;

typedef char SdfSceneNode_size_must_be_0x220[
    (sizeof(SdfSceneNode) == 0x220) ? 1 : -1];

#endif
