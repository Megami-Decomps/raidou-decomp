#ifndef DDS_SDF_DRAW_PACKET_GROUPS_H

#define DDS_SDF_DRAW_PACKET_GROUPS_H
#include "sdf.h"

#include "sdf_gs_blend.h"

#include "sdf_dma_tag.h"

#include "sdf_model_surface_set.h"

typedef struct SdfDrawPacketGroup {
    sdfDrawTag_t list;
    union {
        sdfPrimTransPacket_t blend;
        SdfDmaTagHeader dma;
    } packet;
} SdfDrawPacketGroup;

typedef struct SdfDrawPacketGroups {
    SdfDrawPacketGroup groups[4];
    sdfDrawTag_t syncList;
    u64 syncPacketWords[2] QWORD_ALIGNED; /* a DMA tag: NEXT, with VIF FLUSHA */
} SdfDrawPacketGroups;

typedef char SdfDrawPacketGroup_size_must_be_0x60[
    (sizeof(SdfDrawPacketGroup) == 0x60) ? 1 : -1];

typedef char SdfDrawPacketGroups_size_must_be_0x1B0[
    (sizeof(SdfDrawPacketGroups) == 0x1B0) ? 1 : -1];

#endif
