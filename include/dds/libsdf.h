#ifndef DDS_LIBSDF_H

#define DDS_LIBSDF_H
#include "../libsdf.h"
/* sdf engine core: math, the chip and heap allocators, GS memory, textures,
 * materials and draw tags. Sizes and offsets are verified against SLUS_209.11;
 * see include/README.md for the evidence per struct. */

#include "common.h"

#include "sdf_resource.h"

#include "sdf_types.h"

/* Bodies in sdf_lighting.h and draw3d.h; draw tags only need pointers here. */
struct sdf3DLightParallelPacket_t;

struct sdf3DFogPacket_t;

/* The .pac stream decoder. sdfPacInit memsets 56 bytes, and sdfPacDecode runs
 * phase as a three-state machine: 0 starts the next packet, 1 skips bytes, 2
 * hands the input to decodeFunc. */

#endif /* DDS_LIBSDF_H */
