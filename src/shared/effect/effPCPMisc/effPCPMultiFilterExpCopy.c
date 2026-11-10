#include "shared/effect/effPCPMisc/decls.h"

effPCPMultiBlurExp2_tag *effPCPMultiFilterExpCopy(effPCPMultiBlurExp2_tag *work) {
    effPCPMultiFilterExpParam_t params;

    params.type = work->flags;
    params.life = work->duration;
    params.fade_in = work->fadeIn;
    params.fade_out = work->fadeOut;
    params.area_s = work->startExtent;
    params.area_e = work->endExtent;
    memcpy(&params.res, &work->resource.rectangle->param, sizeof(params.res));
    return effPCPMultiFilterExpCreate(&params);
}
