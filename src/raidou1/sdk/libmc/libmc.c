#include "common.h"

INCLUDE_ASM(const s32, "sdk/libmc/libmc", sceMcInit);

#include "shared/sdk/libmc/sceMcEnd.c"

#include "shared/sdk/libmc/_lmcGetClientPtr.c"

#include "shared/sdk/libmc/sceMcChangeThreadPriority.c"

#include "shared/sdk/libmc/sceMcGetSlotMax.c"

#include "shared/sdk/libmc/sceMcOpen.c"

#include "shared/sdk/libmc/sceMcMkdir.c"

#include "shared/sdk/libmc/sceMcClose.c"

#include "shared/sdk/libmc/sceMcSeek.c"

#include "shared/sdk/libmc/mceIntrReadFixAlign.c"

INCLUDE_ASM(const s32, "sdk/libmc/libmc", sceMcRead);

#include "shared/sdk/libmc/sceMcWrite.c"

#include "shared/sdk/libmc/mcHearAlarm.c"

INCLUDE_ASM(const s32, "sdk/libmc/libmc", mcDelayThread);

INCLUDE_ASM(const s32, "sdk/libmc/libmc", sceMcSync);

#include "shared/sdk/libmc/mceGetInfoApdx.c"

INCLUDE_ASM(const s32, "sdk/libmc/libmc", sceMcGetInfo);

INCLUDE_ASM(const s32, "sdk/libmc/libmc", sceMcUdCheckNewCard);

#include "shared/sdk/libmc/sceMcGetDir.c"

#include "shared/sdk/libmc/mceStorePwd.c"

INCLUDE_ASM(const s32, "sdk/libmc/libmc", sceMcChdir);

#include "shared/sdk/libmc/sceMcFormat.c"

#include "shared/sdk/libmc/sceMcDelete.c"

#include "shared/sdk/libmc/sceMcFlush.c"

#include "shared/sdk/libmc/sceMcSetFileInfo.c"

#include "shared/sdk/libmc/sceMcRename.c"

#include "shared/sdk/libmc/sceMcUnformat.c"

#include "shared/sdk/libmc/sceMcGetEntSpace.c"
