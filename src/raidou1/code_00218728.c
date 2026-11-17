#include "common.h"
#include "newdata.h"
#include "libsdf.h"
#include "kernel.h"
#include "basic.h"
#include "dds3KernelCore.h"
#include "effManager.h"
#include "effect.h"
#include "effPacket.h"
#include "kernel_draw.h"
#include "dds3KernelMain.h"
#include "dds3KernelDraw.h"
#include "sdfDraw.h"
#include "sdfPacketBuff.h"
#include "sdf_packet_list.h"
#include "ps2asm.h"

void dds3Leaf41(sdfMatrix4_t *pDst, const sdfMatrix4_t *pRot)
{
    SDF_VU0_LOADMATRIX(pRot);
    SDF_VU0_STOREMATRIX(pDst);
}
