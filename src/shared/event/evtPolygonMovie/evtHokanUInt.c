#include "shared/event/evtPolygonMovie/decls.h"

u32 evtHokanUInt(s32 hokan, f32 ratio, u32 i1, u32 i2)
{
    if (hokan) {
        return (u32)((f32)i1 * (1.0f - ratio) + (f32)i2 * ratio);
    }
    return i1;
}
