#include "shared/effect/polyManager/decls.h"

void dds3SpPolyReset(dds3SpPoly_Basic_t *hData) {
    dds3SpPolyResetTable[hData->type](hData);
    hData->state = 1;
}
