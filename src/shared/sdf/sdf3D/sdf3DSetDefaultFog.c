#include "shared/sdf/sdf3D/decls.h"

void sdf3DSetDefaultFog(u8 *aFog) {
    sdf3DFogDefault = EE_ADDR(aFog);
}
