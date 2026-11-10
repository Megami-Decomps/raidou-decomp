#include "shared/effect/effPCPMisc/decls.h"

effPCPThunderSphereExp_tag *effPCPThunderSphereExpCreate(effPCPThunderSphereExpParam_t *pParam) {
    effPCPThunderSphereExp_tag *work = sdfChipAllocDirect(sizeof(effPCPThunderSphereExp_tag));
    effPCPThunderSpherePoly_t *handle;

    pParam->ThunderSpherePoly.width = (f32)pParam->radius_s;
    pParam->ThunderSpherePoly.height = (f32)pParam->radius_s;
    handle = effPCPThunderSpherePolyCreate(&pParam->ThunderSpherePoly);
    work->life = pParam->life;
    work->fade_in = pParam->fade_in;
    work->fade_out = pParam->fade_out;
    work->radius_s = pParam->radius_s;
    work->radius_e = pParam->radius_e;
    work->abgr = 0x80808080;
    work->frame = 0;
    work->scale = 1.0f;
    work->pos[0] = pParam->ThunderSpherePoly.origin[0];
    work->pos[1] = pParam->ThunderSpherePoly.origin[1];
    work->pos[2] = pParam->ThunderSpherePoly.origin[2];
    work->pThunderSpherePoly = handle;
    return work;
}
