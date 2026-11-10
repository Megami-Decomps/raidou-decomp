#include "shared/sdf/sdfVu1/decls.h"

void corePktSetFaceDivParm(sdfCorePacketFaceDivParm_t *out, sdfMatPacket_t *work, u32 packedColor, f32 deltaX, f32 deltaY) {
    corePktMakeFaceDivParm(out, corePktCurrLight, packedColor,
                  work->diffuse, work->ambient,
                  work->constant,
                  work->x + deltaX,
                  work->y + deltaY);
}
