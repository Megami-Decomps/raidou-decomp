#include "shared/effect/polyManager/decls.h"

void *dds3SpPolyCreate(s32 index, void *arg) {
    dds3SpPoly_Basic_t *effect = dds3SpPolyTable[index].func(arg);

    effect->type = index;
    effect->state = 1;
    return effect;
}
