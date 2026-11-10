#include "common.h"

#include "shared/sdf/sdfMemory/sdfMemInit.c"

#include "shared/sdf/sdfMemory/sdfMemGetType.c"

#include "shared/sdf/sdfMemory/sdfMemGetTop.c"

#include "shared/sdf/sdfMemory/sdfMemGetNext.c"

#include "shared/sdf/sdfMemory/sdfMemGetSize.c"

#include "shared/sdf/sdfMemory/sdfMemGetPtr.c"

#include "shared/sdf/sdfMemory/sdfMemAllocDirect.c"

#include "shared/sdf/sdfMemory/sdfMemAllocFromTailDirect.c"

#include "shared/sdf/sdfMemory/sdfMemAlloc2Direct.c"

#include "shared/sdf/sdfMemory/sdfMemReallocRecord.c"

#include "shared/sdf/sdfMemory/sdfMemFreeMerge.c"

#include "shared/sdf/sdfMemory/sdfMemFreeDirect.c"

#include "shared/sdf/sdfMemory/sdfMemFreePtrDirect.c"

#include "shared/sdf/sdfMemory/sdfMemFreeHandleDirect.c"

#include "shared/sdf/sdfMemory/sdfMemFreeDelayedDirect.c"

#include "shared/sdf/sdfMemory/sdfMemLock.c"

#include "shared/sdf/sdfMemory/sdfMemUnlock.c"

#include "shared/sdf/sdfMemory/sdfMemGetHandleByPtr.c"

#include "shared/sdf/sdfMemory/sdfMemGetInfo.c"

INCLUDE_ASM(const s32, "sdf/sdfMemory", func_00357688);

#include "shared/sdf/sdfMemory/SdfIopFileRpc.c"
INCLUDE_SDATA(const s32, "sdf/sdfMemory", D_0046F26C);

INCLUDE_SDATA(const s32, "sdf/sdfMemory", D_0046F270);

INCLUDE_SDATA(const s32, "sdf/sdfMemory", D_0046F271);

