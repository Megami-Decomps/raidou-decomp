#ifndef SDF_DECOMP_H
#define SDF_DECOMP_H

#include "common.h"
#include "sdf_types.h"

sdfGrPacketHeader_t *effDebugDrawWireModel(const sdfVector4_t *vertices, const u32 *colors, s32 vertexCount, u32 primitiveFlags);
sdfGrPacketHeader_t *sdfDrawRotatedModel(const f32 vertices[][4], const u32 *colors, s32 vertexCount, u32 primitiveFlags);

struct sdfDecompCb_t;

s32 sdfDecomp(struct sdfDecompCb_t *cb, const u8 *src, s32 length);
void sdfDecompInit(struct sdfDecompCb_t *cb, u8 *dst);

#endif
