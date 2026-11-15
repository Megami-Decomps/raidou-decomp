#ifndef SDFTEX_H
#define SDFTEX_H

#include "common.h"
#include "sdf_resource.h"

sdfTexHandle_t *sdfTexAlloc(s32 aWidth, s32 aHeight, s32 aPixelFormat, s32 aNumOfMipmap, s32 aCLUTFormat, s32 aNumOfCLUT);
sdfTexHandle_t *sdfTexDup(sdfTexHandle_t *aHandle);
void sdfTexFreeDelayed(sdfTexHandle_t *aHandle);
u64 sdfTexGetClampReg(sdfTexHandle_t *aHandle);
s32 sdfTexGetPacketLength(sdfTexHandle_t *aHandle);
u32 sdfTexGetPixelAddr(sdfTexHandle_t *aHandle);
u64 sdfTexGetTex0Reg(sdfTexHandle_t *aHandle);
u64 sdfTexGetTex1Reg(sdfTexHandle_t *aHandle);
void sdfTexInit(void);
void sdfTexMakeGIFPacket(sdfTexHandle_t *aHandle);
sdfTexHandle_t *sdfTexNew(s32 aWidth, s32 aHeight, s32 aPixelFormat, s32 aNumOfMipmap, sdfGsMemSlice_t *aPixelGsHandle, s32 aCLUTFormat, s32 aNumOfCLUT, sdfGsMemSlice_t *aCLUTGsHandle);
sdfTexHandle_t *sdfTexSearch(u32 id);
sdfTexHandle_t *sdfTexSet(TMX_HEADER_t *aTex);
void sdfTexSetClutColor(sdfTexHandle_t *texture, s32 tint, f32 amount);
sdfTexHandle_t *sdfTexSetDirect(TMX_HEADER_t *aTex);
void sdfTexSetWrapMode(sdfTexHandle_t *aHandle, s32 aMode);

#endif
