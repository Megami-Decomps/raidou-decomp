#include "shared/model/mdlEffect/decls.h"

void mdlStartEffectResrc_IPU(mdlEffectResrc_IPU_t *ipu, sdfTexHandle_t *tex, mdlIpuParams_t *parm) {
    if (ipu->playing == 0) {
        ipu->playing = 1;
        sdfIpuSetOutputTex(&ipu->ipuState, parm, EE_ADDR_AS(u8 *, ipu->data), ipu->dataSize, tex);
    }
}
