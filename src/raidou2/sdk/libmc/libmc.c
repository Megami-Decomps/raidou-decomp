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

#include "shared/sdk/libmc/sceMcRead.c"

#include "shared/sdk/libmc/sceMcWrite.c"

#include "shared/sdk/libmc/mcHearAlarm.c"

#include "shared/sdk/libmc/mcDelayThread.c"

INCLUDE_ASM(const s32, "sdk/libmc/libmc", sceMcSync);

#include "shared/sdk/libmc/mceGetInfoApdx.c"

#include "shared/sdk/libmc/sceMcGetInfo.c"

INCLUDE_ASM(const s32, "sdk/libmc/libmc", sceMcUdCheckNewCard);

#include "shared/sdk/libmc/sceMcGetDir.c"

#include "shared/sdk/libmc/mceStorePwd.c"

#include "shared/sdk/libmc/sceMcChdir.c"

#include "shared/sdk/libmc/sceMcFormat.c"

#include "shared/sdk/libmc/sceMcDelete.c"

#include "shared/sdk/libmc/sceMcFlush.c"

#include "shared/sdk/libmc/sceMcSetFileInfo.c"

#include "shared/sdk/libmc/sceMcRename.c"

#include "shared/sdk/libmc/sceMcUnformat.c"

#include "shared/sdk/libmc/sceMcGetEntSpace.c"
