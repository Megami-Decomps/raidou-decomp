#include "shared/effect/polyManager/decls.h"

void *dds3SpPolyCopy(dds3SpPoly_Basic_t *effect) {
    dds3SpPoly_Basic_t *result = dds3SpPolyTable[effect->type].func(effect);

    result->type = effect->type;
    result->state = 1;
    return result;
}
