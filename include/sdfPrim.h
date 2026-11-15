#ifndef SDFPRIM_H
#define SDFPRIM_H

#include "common.h"
#include "draw.h"
#include "sdf_resource.h"

sdfDrawTag_t *sdfPrimAllocDrawTag(s32 (*aAllocator)(s32));
void sdfPrimLinkF3(sdfDrawTag_t *aTag, s32 aColor, s32 aPrim, s32 aX0, s32 aY0, s32 aX1, s32 aY1, s32 aX2, s32 aY2, s32 aZ, s32 (*aAllocator)(s32));
void sdfPrimLinkGt4(sdfDrawTag_t *aTag, s32 aPrim, s32 aX0, s32 aY0, s32 aU0, s32 aV0, s32 aColor0, s32 aX1, s32 aY1, s32 aU1, s32 aV1, s32 aColor1, s32 aX2, s32 aY2, s32 aU2, s32 aV2, s32 aColor2, s32 aX3, s32 aY3, s32 aU3, s32 aV3, s32 aColor3, s32 aZ, s32 (*aAllocator)(s32));
void sdfPrimLinkLine(sdfDrawTag_t *aTag, s32 aColor, s32 aPrim, s32 aX0, s32 aY0, s32 aX1, s32 aY1, s32 aZ, s32 (*aAllocator)(s32));
void sdfPrimLinkRect(sdfDrawTag_t *aTag, s32 aColor, s32 aPrim, s32 aX0, s32 aY0, s32 aX1, s32 aY1, s32 aZ, s32 (*aAllocator)(s32));
void sdfPrimLinkSprite(sdfDrawTag_t *aTag, s32 aColor, s32 aPrim, s32 aX0, s32 aY0, s32 aU0, s32 aV0, s32 aX1, s32 aY1, s32 aU1, s32 aV1, s32 aZ, s32 (*aAllocator)(s32));
void sdfPrimSetAddTransPacket(sdfPrimTransPacket_t *aPkt);
void sdfPrimSetBitBltElem(sdfPrimBitBltElem_t *aElem, u32 aDstGsAddr, s32 aDstBuffWidth, s64 aDstPixelFormat, s64 aDstX, s64 aDstY, u32 aSrcGsAddr, s32 aSrcBuffWidth, s32 aSrcPixelFormat, s32 aSrcX, s32 aSrcY, s32 aWidth, s32 aHeight, s32 aDirMode);
void sdfPrimSetOpaquePacket(sdfPrimTransPacket_t *aPkt);
void sdfPrimSetSemiTransPacket(sdfPrimTransPacket_t *aPkt);
void sdfPrimSetSubTransPacket(sdfPrimTransPacket_t *aPkt);

#endif
