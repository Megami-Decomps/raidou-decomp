#include "shared/effect/polyManager/decls.h"

void dds3TrackPolyDraw(dds3TrackPoly_t *node) {
    node->next = dds3TrackPolyTag;
    dds3TrackPolyTag = node;
}
