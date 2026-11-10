#include "shared/sdf/sdf3D/decls.h"

void sdf3DMakeFogPacket(sdf3DFogPacket_t *aPacket, sdf3DFogParam_t *aParam) {
    f32 rangeMax = aParam->fogMin;
    f32 rangeMin = aParam->fogMax;
    f32 nearZ = aParam->fogNear;
    f32 farZ = aParam->fogFar;
    f32 depthRange = farZ - nearZ;
    s32 fogColor = aParam->color;
    aPacket->dmaTag = 0x20000004;
    aPacket->vifUnpackCode = 0x6C03C00013000000ULL;
    aPacket->gifTag = 0x1000000000008001ULL;
    aPacket->gifRegister = SDF_GIF_REGISTER_AD;
    aPacket->registerValue = (u32)fogColor;
    aPacket->fogColorRegister = SDF_GS_FOGCOL_REGISTER;
    aPacket->mscalCommand = 0x14000014;
    aPacket->reservedC = 0;
    aPacket->right = rangeMax;
    aPacket->left = rangeMin;
    aPacket->reservedA = 0;
    aPacket->reservedB = 0;
    aPacket->mid = (((rangeMax - rangeMin) * (farZ + nearZ)) / depthRange + (rangeMax + rangeMin)) * 0.5f;
    aPacket->scale = ((farZ * nearZ) * (rangeMin - rangeMax)) / depthRange;
}
