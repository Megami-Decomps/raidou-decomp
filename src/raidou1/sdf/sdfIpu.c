#include "common.h"

#include "shared/sdf/sdfIpu/sdfIpuDetachQueue.c"

#include "shared/sdf/sdfIpu/sdfIpuAttachQueue.c"

#include "shared/sdf/sdfIpu/sdfIpuAttachTask.c"

#include "shared/sdf/sdfIpu/sdfIpuDetachTask.c"

#include "shared/sdf/sdfIpu/sdfIpuAllocBuff.c"

#include "shared/sdf/sdfIpu/sdfIpuMakeToIpuDmaTag.c"

#include "shared/sdf/sdfIpu/sdfIpuMakeBltPacket.c"

#include "shared/sdf/sdfIpu/sdfIpuInitCb.c"

#include "shared/sdf/sdfIpu/sdfIpuSetDecodeParams.c"

#include "shared/sdf/sdfIpu/sdfIpuStrmDecodeInit.c"

#include "shared/sdf/sdfIpu/sdfIpuAllocGsRect.c"

#include "shared/sdf/sdfIpu/sdfIpuFree.c"

#include "shared/sdf/sdfIpu/sdfIpuHandleDmaEnd.c"

#include "shared/sdf/sdfIpu/sdfIpuSetToIpuDma.c"

#include "shared/sdf/sdfIpu/sdfIpuSetFromIpuDma.c"

#include "shared/sdf/sdfIpu/IpuWriteCommand.c"

INCLUDE_ASM(const s32, "sdf/sdfIpu", sdfIpuDecode);

#include "shared/sdf/sdfIpu/sdfIpuExecQueue.c"

INCLUDE_ASM(const s32, "sdf/sdfIpu", sdfIpuDecodeThread);

#include "shared/sdf/sdfIpu/sdfIpuToDmacHandler.c"

INCLUDE_ASM(const s32, "sdf/sdfIpu", func_00373D98);

INCLUDE_ASM(const s32, "sdf/sdfIpu", func_00373DC0);

#include "shared/sdf/sdfIpu/sdfIpuFinishActiveFrame.c"

#include "shared/sdf/sdfIpu/sdfIpuSetChainTag.c"

INCLUDE_ASM(const s32, "sdf/sdfIpu", func_00373EF0);

INCLUDE_ASM(const s32, "sdf/sdfIpu", func_00374058);

#include "shared/sdf/sdfIpu/sdfIpuSetOutputTex.c"

INCLUDE_ASM(const s32, "sdf/sdfIpu", func_00374128);

INCLUDE_ASM(const s32, "sdf/sdfIpu", sdfIpuInit);

#include "shared/sdf/sdfIpu/sdfIpuClose.c"

INCLUDE_SDATA(const s32, "sdf/sdfIpu", D_0046F5AC);

