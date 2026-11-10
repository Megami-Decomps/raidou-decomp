#include "shared/sdf/sdfMotion/decls.h"

/* vu0 routine: blend the two colour keys */
u32 sdfMotInterpolateKeyColor(sdfMotGetKeyIf_t *aItf)
{
    u32 *key0;
    u32 *key1;
    u32 color;

    key0 = aItf->key0;
    key1 = aItf->key1;
    VU0_RGBA_LERP(color, *key0, *key1, aItf->rate, 0.5f);
    return color;
}
