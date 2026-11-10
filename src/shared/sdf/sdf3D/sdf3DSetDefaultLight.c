#include "shared/sdf/sdf3D/decls.h"

void sdf3DSetDefaultLight(u8 *aLight) {
    sdf3DLightDefault = EE_ADDR(aLight);
}
