#ifndef DDS_DDS3_FRAME_DRAW_BANK_H

#define DDS_DDS3_FRAME_DRAW_BANK_H
#include "sdf.h"

#include "sdf_dma_packet_header.h"

#include "sdf_linked_packet.h"

#include "sdf_scene_packet.h"

#include "sdf_cons_matrix_packet.h"

#include "sdf_draw.h"

#include "dds3_texture_bank.h"

#include "sdf_draw_packet_groups.h"

/* Two frame banks for the main scene and overlay packet chains. */
typedef struct dds3FrameDrawBank_t {
    sdfDrawTag_t initialList;             /* 0x0000 */
    DmaPacketHeader programReference;    /* 0x0020 */
    SdfDrawPacketGroups packetGroups[13]; /* 0x0040 */
    sdfDrawTag_t sceneList;               /* 0x1630 */
    sdfPreDrawTag_t linkedList;       /* 0x1650 */
    SdfSceneNode scene;                  /* 0x1660 */
    ConsMatrixPacket sceneMatrix;         /* 0x1880 */
    sdf3DLightParallelPacket_t scenePacketStorage; /* 0x1950 */
    dds3TextureBank_t heldTexture;          /* 0x1A30: the held texture's packets */
    sdfDrawTag_t thirdList;               /* 0x1D70 */
    ConsMatrixPacket thirdMatrix;        /* 0x1D90 */
    sdf3DLightParallelPacket_t overlayLightPacket; /* 0x1E60 */
} dds3FrameDrawBank_t;

typedef char dds3FrameDrawBank_size_must_be_0x1F40[
    (sizeof(dds3FrameDrawBank_t) == 0x1F40) ? 1 : -1];

#endif
