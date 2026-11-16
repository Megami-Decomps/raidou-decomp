#ifndef SDF2D_H
#define SDF2D_H

#include "common.h"
#include "sdf_resource.h"
#include "sdf_types.h"

sdfGrPacketHeader_t *sdf2DAllocSprite(s32 aNumOfSpr);
s32 sdf2DGetPrimPacketSize(s32 aNumOfRegs, s32 aNumOfPrim);
u32 sdf2DGetTexPacketSize(sdfTexHandle_t *aTexHndl);
void *sdf2DGetPrimPtr(sdfGrPacketHeader_t *aPacketPtr);
s32 sdf2DLinkTexPacket(sdfDrawTag_t *aTag, sdfTexHandle_t *aTexHndl, s32 aCtx);
sdfGrPacketHeader_t *sdf2DMakePrimPacketHeader(sdfGrPacketHeader_t *aPacketPtr, s32 aPrimReg, s32 aNumOfRegs, s64 aGifRegs, s32 aNumOfPrim);
u32 sdf2DMakeStdPacket(u32 aPacket, s32 aLeng);

#endif
