#ifndef DDS_DRAW3D_H

#define DDS_DRAW3D_H
#include "../billboard.h"
#include "../draw3d.h"
/* sdf3D and sdfCoreDraw: the camera, light and fog packets VU1 shades with,
 * the per-node transform packet, and the generated-geometry path that turns a
 * mesh into GS primitives on VU0.
 *
 * sdf_lighting.h owns the parallel-light packet; sdf3DFogPacket_t is opaque
 * in libsdf.h so sdfDrawTag_t can point at it.
 * See include/README.md for the evidence per struct. */

#include "common.h"

#include "libsdf.h"

#include "sdf_lighting.h"

/* sdfCorePacketGen's own threshold for a blend shape being worth applying. */

#endif /* DDS_DRAW3D_H */
