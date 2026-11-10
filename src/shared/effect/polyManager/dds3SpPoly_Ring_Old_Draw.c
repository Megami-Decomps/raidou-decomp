#include "shared/effect/polyManager/decls.h"

void dds3SpPoly_Ring_Old_Draw(dds3SpPoly_Ring_Old_t3 *hData) {
    dds3SpPoly_Ring_Old_Calc(hData);
    dds3TrackPolyDraw(hData->hTrackPoly);
}
