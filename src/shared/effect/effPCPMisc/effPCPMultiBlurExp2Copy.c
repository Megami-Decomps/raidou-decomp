#include "shared/effect/effPCPMisc/decls.h"

effPCPMultiBlurExp2_tag *effPCPMultiBlurExp2Copy(effPCPMultiBlurExp2_tag *work) {
    effPCPMultiBlurExp2Param_t params;

    params.type = work->flags;
    params.life = work->duration;
    params.fade_in = work->fadeIn;
    params.fade_out = work->fadeOut;
    params.area_s = work->startExtent;
    params.area_e = work->endExtent;
    params.MultiBlur = work->resource.blur->param;
    return effPCPMultiBlurExp2Create(&params);
}
