#include "shared/sdf/sdfMotion/decls.h"

/* vu0 routine: sample and blend the channel using the SDK packed operations. */
void sdfMotInterpolateKeyXYZ(sdfMotGetKeyIf_t *aItf) {
    VU0_LERP_VEC3_KEYS(aItf->key0, aItf->key1, aItf->rate);
}
