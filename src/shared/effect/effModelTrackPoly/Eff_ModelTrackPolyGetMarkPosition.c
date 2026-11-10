#include "shared/effect/effModelTrackPoly/decls.h"

void Eff_ModelTrackPolyGetMarkPosition(effModelTrackPoly_t *hMTPoly)
{
    sdfVector4_t pair[2];
    mdlObj_t *object = hMTPoly->params.hModel;

    sdfModelGetMarkPos(object->inner, hMTPoly->params.mark0);
    SDF_VU0_STOREVECTOR_MEMOPS(pair[0]);
    sdfModelGetMarkPos(object->inner, hMTPoly->params.mark1);
    SDF_VU0_STOREVECTOR(&pair[1]);
    dds3HermitianTrackPolySetVertex(hMTPoly->trackPoly, (const u128 *)pair);
}
