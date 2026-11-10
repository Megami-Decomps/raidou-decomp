#include "shared/effect/polyManager/decls.h"

void dds3TrackPolyVCSet(dds3TrackPoly_t *hTrackPoly, s32 type, void *abgr_in, void *abgr_out) {
    u16 id = hTrackPoly->type;

    VCSetFunc[id * 3 + type](hTrackPoly, abgr_in, abgr_out);
}
