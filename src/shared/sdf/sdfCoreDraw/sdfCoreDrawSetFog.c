#include "shared/sdf/sdfCoreDraw/decls.h"

void sdfCoreDrawSetFog(u8 *fog) {
    if (corePktCurrFog != EE_ADDR(fog)) {
        corePktCurrFog = EE_ADDR(fog);
        SDF_VU0_COPYVECTOR(&corePktFogParm, fog + 0x10);
    }
}
