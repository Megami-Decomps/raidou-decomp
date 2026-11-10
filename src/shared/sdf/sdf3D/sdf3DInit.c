#include "shared/sdf/sdf3D/decls.h"

void sdf3DInit(void) {
    s64 textureWord;
    sdf3DInitSpecularTex2();
    textureWord = sdfTexGetTex0Reg(sdf3DSpecularTex);
    sdf3DRefMapParam = textureWord;
    vu1Tex0Slot0[0] = textureWord;
    vu1Tex0Slot0[1] = textureWord >> 32;
    sdf3DSlot2Tex = sdfTexSetDirect((TMX_HEADER_t *)sdf3DSlot2TmxData);
    textureWord = sdfTexGetTex0Reg(sdf3DSlot2Tex);
    vu1Tex0Slot2[0] = textureWord;
    vu1Tex0Slot2[1] = textureWord >> 32;
    sdf3DSlot1Tex = sdfTexSetDirect((TMX_HEADER_t *)sdf3DSlot1TmxData);
    textureWord = sdfTexGetTex0Reg(sdf3DSlot1Tex);
    vu1Tex0Slot1[0] = textureWord;
    vu1Tex0Slot1[1] = textureWord >> 32;
    sdfModelInitSystem();
    sdfMatInitSystem();
}
