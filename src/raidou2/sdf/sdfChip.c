#include "common.h"

#include "shared/sdf/sdfChip/sdfChipAllocNextPage.c"

#include "shared/sdf/sdfChip/sdfChipAllocGetFree.c"

INCLUDE_ASM(const s32, "sdf/sdfChip", sdfChipAllocPageGrow);

INCLUDE_ASM(const s32, "sdf/sdfChip", sdfChipAllocNewPage);

#include "shared/sdf/sdfChip/sdfChipAllocDirect.c"

INCLUDE_ASM(const s32, "sdf/sdfChip", func_003CC708);

#include "shared/sdf/sdfChip/sdfChipFreeDirect.c"

#include "shared/sdf/sdfChip/sdfChipFreeDelayedDirect.c"

#include "shared/sdf/sdfChip/sdfChipCheck.c"

INCLUDE_ASM(const s32, "sdf/sdfChip", sdfChipInit);

#include "shared/sdf/sdfChip/sdfChipGetInfo.c"
