#include "shared/effect/effModelTrackPoly/decls.h"

void dds3HermitianTrackPolySetVC(dds3HermitianTrackPoly_tag *hPoly, u32 *pABGR) {
    f32 t = 0.0f;
    u32 count = hPoly->point >> 1;
    f32 step = 1.0f / count;
    u32 i = 0;
    u32 *colors = hPoly->pVC;

    if (count != 0) {
        do {
            colors[0] = effColorBlend(pABGR[2], pABGR[0], t);
            colors[1] = effColorBlend(pABGR[3], pABGR[1], t);
            t += step;
            colors += 2;
            i++;
        } while (i < count);
    }
}
