#include "common.h"

#include "shared/event/evtEventPack/evtSearchEventPack.c"

#include "shared/event/evtEventPack/evtGetEventPackStat.c"

#include "shared/event/evtEventPack/evtGetPackProcessData.c"

#include "shared/event/evtEventPack/evtGetResorceData.c"

INCLUDE_ASM(const s32, "event/evtEventPack", func_001DFF00);

INCLUDE_ASM(const s32, "event/evtEventPack", func_001DFF88);

INCLUDE_ASM(const s32, "event/evtEventPack", evtInitMotionSeProcess);

#include "shared/event/evtEventPack/evtRegistBEField.c"

INCLUDE_ASM(const s32, "event/evtEventPack", evtMotionSeProcess1);

#include "shared/event/evtEventPack/evtFreeScriptProcessData.c"

INCLUDE_ASM(const s32, "event/evtEventPack", evtLoadModelSe);

INCLUDE_ASM(const s32, "event/evtEventPack", evtLoadEventScript);

#include "shared/event/evtEventPack/evtCheckEventScriptLoaded.c"

INCLUDE_ASM(const s32, "event/evtEventPack", evtCheckScriptDone);

#include "shared/event/evtEventPack/evtWaitEventPackLoad.c"

#include "shared/event/evtEventPack/evtLoadEventPack.c"

#include "shared/event/evtEventPack/evtKillEventPack.c"

INCLUDE_ASM(const s32, "event/evtEventPack", func_001E0930);
INCLUDE_SDATA(const s32, "event/evtEventPack", D_004DFF28);

INCLUDE_SDATA(const s32, "event/evtEventPack", D_004DFF30);

INCLUDE_SDATA(const s32, "event/evtEventPack", D_004DFF38);

