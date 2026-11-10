#include "shared/sdf/sdfMat/decls.h"

void sdfMatUpdatePacketMultiTex(sdfMat_t *aMat, sdfMatPacket_t *aPacket)
{
    sdfTexHandle_t *handle = aMat->multiTexHandle;
    u32 method;

    aPacket->mtexDiffuse = aMat->multiTexDiffuse;
    method = aMat->multiTexMethod;
    aPacket->mtexAlgo = method;
    aPacket->mtexAlpha = sdfMatMultiTexAlphaTable[method];
    if (handle != NULL) {
        aPacket->mtexTex1Data = sdfTexGetTex1Reg(handle);
        aPacket->mtexTex0Data = sdfTexGetTex0Reg(handle);
        aPacket->mtexClampData = sdfTexGetClampReg(handle);
    }
    sdfMatSetUvwParm(&aPacket->mtexTexTm, aMat->multiTexUvwParm);
}
