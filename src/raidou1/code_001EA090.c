#include "common.h"
#include "code_001EA090.h"
#include "code_0027A1D0.h"

f32 effBTLPartCalcScale(SBTL_UNIT *unit) {
    f32 scale = (unit->reach * unit->scale + unit->height * unit->scale * 0.5f) * 0.5f * 0.0114285714f;

    if (3.0f < scale) {
        scale = 3.0f;
    } else if (scale < 0.8f) {
        scale = 0.8f;
    }
    return scale;
}

f32 effLeaf35(struct SBTL_UNIT *arg) {
    return effBTLPartCalcScale(arg);
}
