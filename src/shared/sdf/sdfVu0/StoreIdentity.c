#include "shared/sdf/sdfVu0/decls.h"

/* vu0 routine: store the unit matrix built from vf0 by MMI lane shuffles */
void StoreIdentity(f32 *aMatrix) {
    SDF_VU0_STOREIDENTITY(aMatrix);
}
