#include "shared/event/evtPolygonMovie/decls.h"

/* vu0 routine: blend a vector and an RGBA8888 colour by t (missing input =
 * default vector/colour) */
void evtHokanSceneFog(s32 hokan, f32 ratio, evtBlendVecColor_t *key1, evtBlendVecColor_t *key2, evtBlendVecColor_t *d)
{

    if (hokan == 0) {
        ratio = 0.0f;
    }
    if (key1 != NULL) {
        SDF_VU0_SETVECTOR_X(key1->vec[0]);
        SDF_VU0_SETVECTOR_Y(key1->vec[1]);
        SDF_VU0_SETVECTOR_Z(key1->vec[2]);
        SetBlendVecW(key1->vec[3]);
    } else {
        SDF_VU0_SETVECTOR_X(evtHokanFogParam[1]);
        SDF_VU0_SETVECTOR_Y(evtHokanFogParam[0]);
        SDF_VU0_SETVECTOR_Z(evtHokanFogParam[2]);
        SetBlendVecW(evtHokanFogParam[3]);
    }
    SDF_VU0_SCALEVECTOR_BITS(1.0f - ratio);
    SDF_VU0_COPYVECTOR_A_TO_B();
    if (key2 != NULL) {
        SDF_VU0_SETVECTOR_X(key2->vec[0]);
        SDF_VU0_SETVECTOR_Y(key2->vec[1]);
        SDF_VU0_SETVECTOR_Z(key2->vec[2]);
        SetBlendVecW(key2->vec[3]);
    } else {
        SDF_VU0_SETVECTOR_X(evtHokanFogParam[1]);
        SDF_VU0_SETVECTOR_Y(evtHokanFogParam[0]);
        SDF_VU0_SETVECTOR_Z(evtHokanFogParam[2]);
        SetBlendVecW(evtHokanFogParam[3]);
    }
    SDF_VU0_SCALEVECTOR_BITS(ratio);
    SDF_VU0_ADDVECTOR();
    SDF_VU0_GETVECTOR_X(d->vec[0]);
    SDF_VU0_GETVECTOR_Y(d->vec[1]);
    SDF_VU0_GETVECTOR_Z(d->vec[2]);
    SDF_VU0_GETVECTOR_W(d->vec[3]);
    if (key1 != NULL) {
        UnpackRGBAWordFloat(key1->color, 1.0f / 128.0f);
    } else {
        UnpackRGBAWordFloat(evtSceneFogDefaultColor[0], 1.0f / 128.0f);
    }
    SDF_VU0_SCALEVECTOR_BITS(1.0f - ratio);
    SDF_VU0_COPYVECTOR_A_TO_B();
    if (key2 != NULL) {
        UnpackRGBAWordFloat(key2->color, 1.0f / 128.0f);
    } else {
        UnpackRGBAWordFloat(evtSceneFogDefaultColor[0], 1.0f / 128.0f);
    }
    SDF_VU0_SCALEVECTOR_BITS(ratio);
    SDF_VU0_ADDVECTOR();
    d->color = PackRGBAWord(128.0f);
}
