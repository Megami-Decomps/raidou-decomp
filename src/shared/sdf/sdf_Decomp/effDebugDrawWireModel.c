#include "shared/sdf/sdf_Decomp/decls.h"

sdfGrPacketHeader_t *effDebugDrawWireModel(const sdfVector4_t *vertices, const u32 *colors, s32 vertexCount, u32 primitiveFlags) {
    SDF_VU0_LOADMATRIX(sdf3DScreenTm);
    return sdfDrawModelWithMatrix((const f32 (*)[4])vertices, colors, vertexCount, primitiveFlags);
}
