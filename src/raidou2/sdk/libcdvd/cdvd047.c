#include "common.h"

#include "shared/sdk/libcdvd/sceCdStInit.c"

#include "shared/sdk/libcdvd/sceCdStStart.c"

#include "shared/sdk/libcdvd/sceCdStSeekF.c"

#include "shared/sdk/libcdvd/sceCdStSeek.c"

#include "shared/sdk/libcdvd/sceCdStStop.c"

INCLUDE_ASM(const s32, "sdk/libcdvd/cdvd047", sceCdStRead);

INCLUDE_ASM(const s32, "sdk/libcdvd/cdvd047", sceCdStPause);

INCLUDE_ASM(const s32, "sdk/libcdvd/cdvd047", sceCdStResume);

INCLUDE_ASM(const s32, "sdk/libcdvd/cdvd047", sceCdStStat);
