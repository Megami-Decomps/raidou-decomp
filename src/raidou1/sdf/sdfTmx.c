#include "common.h"

#include "shared/sdf/sdfTmx/sdfTmxSetHeader.c"

#include "shared/sdf/sdfTmx/sdfFileRpcContinueRead.c"

INCLUDE_ASM(const s32, "sdf/sdfTmx", sdfFileRpcReadCallback);

#include "shared/sdf/sdfTmx/sdfFileRpcHandler.c"

INCLUDE_ASM(const s32, "sdf/sdfTmx", SdfFileRpcServerThreadProc);

#include "shared/sdf/sdfTmx/sdfStartFileRpcServer.c"

#include "shared/sdf/sdfTmx/LoadMcModules.c"

INCLUDE_RODATA(const s32, "sdf/sdfTmx", D_004656B0);

INCLUDE_RODATA(const s32, "sdf/sdfTmx", D_004656C0);

INCLUDE_RODATA(const s32, "sdf/sdfTmx", D_004656D0);

INCLUDE_RODATA(const s32, "sdf/sdfTmx", D_004656E0);

