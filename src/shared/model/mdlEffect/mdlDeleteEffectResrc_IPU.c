#include "shared/model/mdlEffect/decls.h"

void mdlDeleteEffectResrc_IPU(mdlEffectResrc_IPU_t *ipu) {
    if (ipu->playing != 0) {
        sdfIpuFree(&ipu->ipuState);
    }
    sdfMemFreeDirect(ipu->dataMemHandle);
    sdfChipFreeDirect(ipu);
}
