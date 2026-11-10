#include "shared/effect/effPCPMisc/decls.h"

effPCPThunderSphereExp_tag *effPCPThunderSphereExpCopy(effPCPThunderSphereExp_tag *work) {
    effPCPThunderSphereExpParam_t params;
    effPCPThunderSpherePolyParam_t *res = effPCPThunderSpherePolyGetParam(work->pThunderSpherePoly);

    params.life = work->life;
    params.fade_in = work->fade_in;
    params.fade_out = work->fade_out;
    params.radius_s = work->radius_s;
    params.radius_e = work->radius_e;
    params.ThunderSpherePoly = *res;
    return effPCPThunderSphereExpCreate(&params);
}
