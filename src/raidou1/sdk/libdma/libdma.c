#include "common.h"

INCLUDE_ASM(const s32, "sdk/libdma/libdma", WaitDma);

INCLUDE_ASM(const s32, "sdk/libdma/libdma", WatchDma);

#include "shared/sdk/libdma/CheckAddress.c"

#include "shared/sdk/libdma/memclr.c"

#include "shared/sdk/libdma/sceDmaGetChan.c"

#include "shared/sdk/libdma/sceDmaReset.c"

INCLUDE_ASM(const s32, "sdk/libdma/libdma", sceDmaPutEnv);

#include "shared/sdk/libdma/sceDmaGetEnv.c"

#include "shared/sdk/libdma/sceDmaPutStallAddr.c"

#include "shared/sdk/libdma/sceDmaSend.c"

#include "shared/sdk/libdma/sceDmaSendN.c"

#include "shared/sdk/libdma/sceDmaSendI.c"

#include "shared/sdk/libdma/sceDmaRecv.c"

#include "shared/sdk/libdma/sceDmaRecvN.c"

#include "shared/sdk/libdma/sceDmaRecvI.c"

#include "shared/sdk/libdma/sceDmaSync.c"

#include "shared/sdk/libdma/sceDmaWatch.c"

#include "shared/sdk/libdma/sceDmaPause.c"

#include "shared/sdk/libdma/sceDmaRestart.c"

INCLUDE_RODATA(const s32, "sdk/libdma/libdma", D_00465C00);

INCLUDE_RODATA(const s32, "sdk/libdma/libdma", D_00465C18);

INCLUDE_RODATA(const s32, "sdk/libdma/libdma", D_00465C40);

INCLUDE_RODATA(const s32, "sdk/libdma/libdma", D_00465C50);

INCLUDE_RODATA(const s32, "sdk/libdma/libdma", D_00465C60);

