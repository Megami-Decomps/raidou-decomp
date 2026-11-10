#include "common.h"

INCLUDE_ASM(const s32, "sdk/libpad/libpad", _send_to_iop);

INCLUDE_ASM(const s32, "sdk/libpad/libpad", scePadInit);

#include "shared/sdk/libpad/scePadInit2.c"

#include "shared/sdk/libpad/scePadEnd.c"

INCLUDE_ASM(const s32, "sdk/libpad/libpad", scePadPortOpen);

#include "shared/sdk/libpad/scePadPortClose.c"

#include "shared/sdk/libpad/scePadGetDmaStr.c"

#include "shared/sdk/libpad/scePadGetFrameCount.c"

#include "shared/sdk/libpad/scePadRead.c"

#include "shared/sdk/libpad/scePadGetState.c"

INCLUDE_ASM(const s32, "sdk/libpad/libpad", scePadStateIntToStr);

#include "shared/sdk/libpad/scePadSetReqState.c"

#include "shared/sdk/libpad/scePadGetReqState.c"

INCLUDE_ASM(const s32, "sdk/libpad/libpad", scePadReqIntToStr);

#include "shared/sdk/libpad/scePadInfoAct.c"

#include "shared/sdk/libpad/scePadInfoComb.c"

#include "shared/sdk/libpad/scePadInfoMode.c"

#include "shared/sdk/libpad/scePadSetMainMode.c"

#include "shared/sdk/libpad/scePadSetActDirect.c"

#include "shared/sdk/libpad/scePadSetActAlign.c"

#include "shared/sdk/libpad/scePadGetButtonMask.c"

#include "shared/sdk/libpad/scePadSetButtonInfo.c"

#include "shared/sdk/libpad/scePadInfoPressMode.c"

#include "shared/sdk/libpad/scePadEnterPressMode.c"

#include "shared/sdk/libpad/scePadExitPressMode.c"

#include "shared/sdk/libpad/scePadSetVrefParam.c"

#include "shared/sdk/libpad/scePadGetPortMax.c"

#include "shared/sdk/libpad/scePadGetSlotMax.c"

#include "shared/sdk/libpad/scePadGetModVersion.c"

#include "shared/sdk/libpad/scePadSetWarningLevel.c"

INCLUDE_RODATA(const s32, "sdk/libpad/libpad", D_004D8568);

INCLUDE_RODATA(const s32, "sdk/libpad/libpad", D_004D8570);

INCLUDE_RODATA(const s32, "sdk/libpad/libpad", D_004D8578);

INCLUDE_RODATA(const s32, "sdk/libpad/libpad", D_004D8580);

INCLUDE_RODATA(const s32, "sdk/libpad/libpad", D_004D8590);

INCLUDE_RODATA(const s32, "sdk/libpad/libpad", D_004D8598);

INCLUDE_RODATA(const s32, "sdk/libpad/libpad", D_004D85A8);

INCLUDE_RODATA(const s32, "sdk/libpad/libpad", D_004D85B0);

INCLUDE_RODATA(const s32, "sdk/libpad/libpad", D_004D85B8);

