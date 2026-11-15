#include "common.h"

INCLUDE_ASM(const s32, "code_r1_001228B0", datCallGetIntPara4);

INCLUDE_ASM(const s32, "code_r1_001228B0", datCallGetStockDevilNums2);

INCLUDE_ASM(const s32, "code_r1_001228B0", datCallGetIntPara5);

#ifdef VERSION_RAIDOU1
INCLUDE_ASM(const s32, "code_r1_001228B0", datCommand_DEVIL_OUT_AFTER);
#else
INCLUDE_ASM(const s32, "code_r1_001228B0", datCallGetIntPara);

INCLUDE_ASM(const s32, "code_r1_001228B0", datCallGetIntPara2);
#endif

INCLUDE_ASM(const s32, "code_r1_001228B0", datCommand_GET_STOCK_MAX);

INCLUDE_ASM(const s32, "code_r1_001228B0", datCallGetIntPara6);

INCLUDE_ASM(const s32, "code_r1_001228B0", datCommand_GET_INC_STOCK);

INCLUDE_ASM(const s32, "code_r1_001228B0", datCalcMagMax);

INCLUDE_ASM(const s32, "code_r1_001228B0", datCommand_INC_STOCK);

INCLUDE_ASM(const s32, "code_r1_001228B0", datCommand_DEBT_FROM_GOUTO);
#ifdef VERSION_RAIDOU2

INCLUDE_ASM(const s32, "code_r1_001228B0", datCallBtlGetPlayerLuckStep);

INCLUDE_ASM(const s32, "code_r1_001228B0", datCallGetIntPara3);

INCLUDE_ASM(const s32, "code_r1_001228B0", datCallSetIntReturnValue);
#endif
