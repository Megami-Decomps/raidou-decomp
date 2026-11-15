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

/* NON_MATCHING: 19 of 37 words match. Register allocation differs (wk_sbtl is loaded into a0, retail v1) and the panel-pack stores are scheduled differently. */
#ifdef NON_MATCHING
s32 sbtlSetLoadPanelData(void)
{
    SBTL_PANEL *panel;

    if (wk_sbtl == NULL) {
        return 0;
    }
    panel = wk_sbtl->panel;
    if (panel == NULL) {
        return 0;
    }
    if (panel->flg & 1) {
        return 1;
    }
    if (!(wk_sbtl->flg & 1)) {
        return 1;
    }
    wk_sbtl->panelPack = pacLoad("/battle/SB_PACK.LB");
    wk_sbtl->panelPackPos = 0;
    wk_sbtl->panelPackLen = 0;
    wk_sbtl->flg |= 0x800;
    panel->flg |= 1;
    return 0;
}
#else
INCLUDE_ASM(const s32, "code_r1_00292930", sbtlSetLoadPanelData);
#endif
