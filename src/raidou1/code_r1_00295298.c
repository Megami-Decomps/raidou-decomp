#include "common.h"
#include "sBtlEff.h"
#include "sBtlUnit.h"
#include "sBtlSkill.h"
#include "sbattle.h"
#include "sbtlWork.h"
#include "EventBit.h"
#include "fileManager.h"
#include "sdk_libc.h"
#include "newdata.h"
#include "sBtlEncount.h"
#include "sBtlCalc.h"
#include "sbtlSub.h"
#include "fld.h"
#include "ps2asm.h"
#include "code_r1_001CC1D0.h"

/* NON_MATCHING: 4 of 62 words match. Retail copies the 4000-byte player data with a runtime alignment test (byte-aligned struct copy); mine copies as the aligned SBTL_PLAYER_DATA. */
#ifdef NON_MATCHING
void sbtlInitTutorial(void)
{
    if (sbtlCheckTutorialBattle()) {
        evtBitOff(2096);
        evtBitOff(2097);
        evtBitOff(2098);
        evtBitOff(2099);
        sbtlCallBtlCheckTutorialBattleData = *sbtlGetPlayerData();
    }
}
#else
INCLUDE_ASM(const s32, "code_r1_00295298", sbtlInitTutorial);
#endif
