#include "common.h"

#include "shared/sdk/libkernl/_lf_bind.c"

#include "shared/sdk/libkernl/_lf_version.c"

#include "shared/sdk/libkernl/sceSifLoadFileReset.c"

#include "shared/sdk/libkernl/_sceSifLoadModuleBuffer.c"

#include "shared/sdk/libkernl/sceSifStopModule.c"

#include "shared/sdk/libkernl/sceSifUnloadModule.c"

#include "shared/sdk/libkernl/sceSifSearchModuleByName.c"

#include "shared/sdk/libkernl/sceSifSearchModuleByAddress.c"

#include "shared/sdk/libkernl/sceSifLoadModuleBuffer.c"

#include "shared/sdk/libkernl/sceSifLoadStartModuleBuffer.c"

#include "shared/sdk/libkernl/_sceSifLoadModule.c"

#include "shared/sdk/libkernl/sceSifLoadModule.c"

#include "shared/sdk/libkernl/sceSifLoadStartModule.c"

#include "shared/sdk/libkernl/_sceSifLoadElfPart.c"

#include "shared/sdk/libkernl/sceSifLoadElfPart.c"

INCLUDE_ASM(const s32, "sdk/libkernl/eeloadfile", sceSifLoadElf);

#include "shared/sdk/libkernl/sceSifGetIopAddr.c"

#include "shared/sdk/libkernl/sceSifSetIopAddr.c"
