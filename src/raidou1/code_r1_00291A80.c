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
#include "sbtlSub_0027ED08.h"
#include "code_r1_002CA3F0.h"

/* NON_MATCHING: 36 of 126 words match. Stack frame 80 vs 96 and unrolled colour loop scheduling; table copied from rodata like retail. */
#ifdef NON_MATCHING
s32 Draw_sbtlSTP(SBTL_STP *stp)
{
    s32 slotTable[5] = { 0, 20, 19, 17, 18 };
    SBTLPROJECTEDSCREEN_POINT point;
    u32 palette[4];
    SBTL_STP_NODE *node;
    sprHandle_t *set;
    s32 slot;
    s32 i;

    if (stp == NULL) {
        return 0;
    }
    node = stp->head;
    if (node == NULL || stp->enabled == 0) {
        return 0;
    }
    if (wk_sbtl->panel == NULL || wk_sbtl->panel->resources == NULL) {
        return 0;
    }
    if ((wk_sbtl->flg2 & 0x40) != 0) {
        return 0;
    }
    for (; node != NULL; node = node->next) {
        if (node->mode >= 3 || node->mode < 0) {
            continue;
        }
        VU0_LOAD_VF(vf10, &node->pos);
        btlProjectForwardPositionToScreen(&point);
        set = wk_sbtl->panel->resources;
        slot = slotTable[node->id];
        for (i = 0; i < 4; i++) {
            palette[i] = (set->workEntries[slot].savedColors[i] & 0xFFFFFF00) | node->alpha;
        }
        effDrawSpriteWithCornerColors(point.x * 16, (s32)((f32)point.y + node->y) * 8, 0, palette, 0, set, slot, 102);
    }
    return 1;
}
#else
INCLUDE_ASM(const s32, "code_r1_00291A80", Draw_sbtlSTP);
#endif

INCLUDE_RODATA(const s32, "code_r1_00291A80", Draw_sbtlSTPTable);

