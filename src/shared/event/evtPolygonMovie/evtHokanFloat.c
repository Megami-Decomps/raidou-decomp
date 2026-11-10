#include "shared/event/evtPolygonMovie/decls.h"

f32 evtHokanFloat(s32 hokan, f32 ratio, f32 f1, f32 f2)
{
    if (hokan) {
        return f1 * (1.0f - ratio) + f2 * ratio;
    }
    return f1;
}
