#include "common.h"

INCLUDE_ASM(const s32, "sdf/sdfGsMemory", sdfGsMemGetBitCount);

INCLUDE_ASM(const s32, "sdf/sdfGsMemory", sdfGetTexelBitCount);

#include "shared/sdf/sdfGsMemory/sdfGsMemCheckHandle.c"

#include "shared/sdf/sdfGsMemory/sdfGsMemAllocFromHead.c"

#include "shared/sdf/sdfGsMemory/sdfGsMemAllocFromTail.c"

#include "shared/sdf/sdfGsMemory/sdfGsMemAllocRect.c"

#include "shared/sdf/sdfGsMemory/sdfGsMemCheckMerge.c"

#include "shared/sdf/sdfGsMemory/sdfGsMemCheckLinkConsistence.c"

#include "shared/sdf/sdfGsMemory/sdfGsMemFree.c"

#include "shared/sdf/sdfGsMemory/sdfGsMemFreeDelayed.c"

#include "shared/sdf/sdfGsMemory/sdfGsMemInit.c"

INCLUDE_ASM(const s32, "sdf/sdfGsMemory", sdfGsAllocFrameBuff);
INCLUDE_SDATA(const s32, "sdf/sdfGsMemory", D_004E42AC);

INCLUDE_SDATA(const s32, "sdf/sdfGsMemory", D_004E42B0);

