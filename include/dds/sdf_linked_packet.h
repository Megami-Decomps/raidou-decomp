#ifndef DDS_SDF_LINKED_PACKET_H

#define DDS_SDF_LINKED_PACKET_H
#include "../libsdf.h"
#include "common.h"

#include "sdf_packet_patch.h"

struct sdfDrawTag_t;

/* Frame submission joins these 0x10-byte linked owners through head/tail. */
typedef struct SdfPacketChain {
    sdfPreDrawTag_t *head;
    sdfPreDrawTag_t *tail;
} SdfPacketChain;

typedef char SdfPacketChain_size_must_be_8[sizeof(SdfPacketChain) == 8 ? 1 : -1];

#endif
