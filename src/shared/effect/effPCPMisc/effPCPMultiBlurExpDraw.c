#include "shared/effect/effPCPMisc/decls.h"

void effPCPMultiBlurExpDraw(effPCPMultiBlurExp_tag *hData) {
    dds3Effect_PartialMultiBlur_t *obj;
    sdfVector4_t pos;

    obj = hData->hBlur;
    SDF_VU0_LOADVECTOR(hData->pos);
    obj->param.area = (s32)((f32)effPCPBlurScreenOffset(hData->area) * hData->ratio);
    SDF_VU0_STOREVECTOR(&pos);
    obj->param.Blur.center[0] = (s32)pos.x - 0x800;
    obj->param.Blur.center[1] = ((s32)pos.y - 0x800) << 1;
    if (obj->param.area < hData->area_min) {
        obj->param.area = hData->area_min;
    }
    if (hData->frame < SDF_FRAMES(hData->blink)) {
        hData->abgr_base = hData->blink_abgr[SDF_FRAME_TICK(hData->frame) & 1];
    } else {
        hData->abgr_base = hData->blink_abgr[0];
    }
    hData->frame++;
    obj->param.Blur.abgr = effMagaCylinderCalcColor(hData->abgr, hData->abgr_base);
    dds3EffectPartialMultiBlurDraw(obj);
}
