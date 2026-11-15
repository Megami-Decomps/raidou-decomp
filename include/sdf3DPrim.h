#ifndef SDF3DPRIM_H
#define SDF3DPRIM_H

#include "common.h"
#include "draw3d.h"
#include "libsdf.h"
#include "sdf_resource.h"

void sdf3DPrimLinkMat(sdfDrawTag_t *aTag, sdfMat_t *aMat, s32 (*aAllocator)(s32));
void sdf3DPrimLinkResetTm(sdfDrawTag_t *aTag, s32 (*aAllocator)(s32));
void sdf3DPrimLinkTm(sdfDrawTag_t *aTag, s32 (*aAllocator)(s32));
u32 *sdf3DPrimMakePoly(sdf3DPrimPolyParm_t *aParm);
void sdf3DPrimSetTmPacket(sdf3DTmPacketData_t *aPacket);

#endif
