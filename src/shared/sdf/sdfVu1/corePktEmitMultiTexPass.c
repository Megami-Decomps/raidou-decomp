#include "shared/sdf/sdfVu1/decls.h"

void corePktEmitMultiTexPass(sdfCorePacketCb_t *cb) {
    sdfMatPacket_t *node;
    u32 mode;
    u32 secondaryColor;
    u32 primaryStateWordSecond;

    corePktXfmUV(EE_ADDR_AS(void *, cb->pointBuff), cb->pointCount, cb->uv2Ptr,
                  (u8 *)cb->matPacketPtr + 0x80);
    corePktLerpUVChain(cb->extPointTop);
    node = cb->matPacketPtr;
    mode = node->secondaryMode;
    secondaryColor = node->secondaryColor;
    primaryStateWordSecond = node->diffuse;
    switch (mode) {
        case 0:
        case 1:
        case 2:
            if (primaryStateWordSecond != secondaryColor) {
                u128 parameters[3];

                corePktMakeFaceDivParm((sdfCorePacketFaceDivParm_t *)parameters, corePktCurrLight, cb->color,
                              secondaryColor, node->ambient, node->constant,
                              node->x + cb->fdSoften, node->y + cb->fdBias);
                corePktApplyFaceDivColor(cb, parameters);
            }
            break;
        case 3:
            corePktSetConstColor(cb, secondaryColor);
            break;
    }
    corePktEmitTexTrisCtx2(cb, node->mtexTex1Data,
                                  node->mtexTex0Data,
                                  node->mtexClampData, node->alphaState, 0);
}
