#include "common.h"

#include "shared/sdk/libkernl/sceSifGetSreg.c"

#include "shared/sdk/libkernl/sceSifSetSreg.c"

#include "shared/sdk/libkernl/sceSifGetDataTable.c"

INCLUDE_ASM(const s32, "sdk/libkernl/sifcmd", sceSifInitCmd);

#include "shared/sdk/libkernl/sceSifExitCmd.c"

INCLUDE_ASM(const s32, "sdk/libkernl/sifcmd", func_00395D70);

INCLUDE_ASM(const s32, "sdk/libkernl/sifcmd", func_00395D88);

#include "shared/sdk/libkernl/sceSifAddCmdHandler.c"

#include "shared/sdk/libkernl/sceSifRemoveCmdHandler.c"

#include "shared/sdk/libkernl/_sceSifSendCmd.c"

#include "shared/sdk/libkernl/sceSifSendCmd.c"

#include "shared/sdk/libkernl/isceSifSendCmd.c"

INCLUDE_ASM(const s32, "sdk/libkernl/sifcmd", _sceSifCmdIntrHdlr);

INCLUDE_ASM(const s32, "sdk/libkernl/sifcmd", sceSifWriteBackDCache);
