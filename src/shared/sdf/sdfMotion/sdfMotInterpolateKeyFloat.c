#include "shared/sdf/sdfMotion/decls.h"

f32 sdfMotInterpolateKeyFloat(sdfMotGetKeyIf_t *aItf)
{
    f32 *key0;
    f32 *key1;

    key0 = aItf->key0;
    key1 = aItf->key1;
    return *key0 + *key1 * aItf->rate - *key0 * aItf->rate;
}
