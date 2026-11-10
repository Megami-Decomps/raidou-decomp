#include "shared/sdf/sdfMotion/decls.h"

void sdfMotInterpolateKeyUvw(sdfMotGetKeyIf_t *aItf, void *aDst)
{
    sdfMotInterpolateKeyUvwSub(aDst, aItf->key0, aItf->key1, aItf->rate);
}
