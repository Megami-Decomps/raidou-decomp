#include "shared/sdf/sdfVu1/decls.h"

/* vu0 routine: rotate the basis at vectors+0x40 into vf24-vf26 and cache the original vectors. */
void corePktLoadWorldLightMtx(void *vectors) {
    void *m = EE_ADDR_AS(void *, corePktCurrLight);
    VU0_ROTATE_BASIS_AND_CACHE(vectors, m, corePktWorldLightMtxCache);
}
