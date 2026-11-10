#ifndef DDS_DDS3_TEXTURE_BANK_H

#define DDS_DDS3_TEXTURE_BANK_H
#include "sdf.h"

#include "sdf_scene_packet.h"

#include "sdf_cons_matrix_packet.h"

#include "sdf_draw.h"

/* The held texture's packets in one frame bank (dds3FrameDrawBank_t::heldTexture). */
typedef struct dds3TextureBank_t {
    sdfDrawTag_t list;
    SdfSceneDrawPacket scene;
    ConsMatrixPacket matrix;
    sdf3DLightParallelPacket_t lighting;
} dds3TextureBank_t;

typedef char dds3TextureBank_size_must_be_0x340[
    (sizeof(dds3TextureBank_t) == 0x340) ? 1 : -1];

#endif
