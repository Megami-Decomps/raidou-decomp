#include "shared/effect/effEvent/decls.h"

/* Forward block 0, block 1's word-2 kind (narrowed to u16), and block 1 unchanged. */
effAPPoint4_tag *effAPPoint4CreatePCF(void *data) {
    void *block0 = dds3EffectPCPDataGetData(data, 0);
    void *block1 = dds3EffectPCPDataGetData(data, 1);

    return effAPPoint4Create(block0, dds3EffectPCPDataGetType(data, 1), block1);
}
