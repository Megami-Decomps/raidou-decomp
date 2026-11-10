#include "common.h"

#include "shared/sdk/libkernl/QueuePeekWriteDone.c"

#include "shared/sdk/libkernl/QueuePeekReadDone.c"

INCLUDE_ASM(const s32, "sdk/libkernl/tty", sceTtyHandler);

#include "shared/sdk/libkernl/sceTtyWrite.c"

#include "shared/sdk/libkernl/sceTtyRead.c"

#include "shared/sdk/libkernl/sceTtyInit.c"
