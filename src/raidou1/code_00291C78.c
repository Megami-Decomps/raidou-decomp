#include "common.h"
#include "sbtlWork.h"
#include "sBtlDraw.h"
#include "sdfMemory.h"
#include "sdk_libc.h"
#include "nbDebugProcess.h"
#include "code_002929C8.h"
#include "code_00291C78.h"

/* NON_MATCHING: 13 of 56 words match.  */
#ifdef NON_MATCHING
s32 sbtlReleaseSTP_List(SBTL_STP *stp, u8 flg) {
    SBTL_STP_NODE *node;
    SBTL_STP_NODE *next;

    if (stp == NULL) {
        return 1;
    }
    node = stp->head;
    if (node == NULL) {
        return 0;
    }
    if (stp->enabled == 0) {
        return 0;
    }
    do {
        if (flg) {
            node->mode = 3;
        }
        if (node->mode == 3) {
            if (node->prev != NULL) {
                node->prev->next = node->next;
            }
            if (node->next != NULL) {
                node->next->prev = node->prev;
            }
            if (node->prev == NULL) {
                stp->head = node->next;
                if (stp->head != NULL) {
                    stp->head->prev = NULL;
                }
            }
            next = node->next;
            sdfMemFreeDirect(node->handle);
            node = next;
            stp->enabled--;
        } else {
            node = node->next;
        }
    } while (node != NULL);
    return 1;
}
#else
INCLUDE_ASM(const s32, "code_00291C78", sbtlReleaseSTP_List);
#endif

INCLUDE_ASM(const s32, "code_00291C78", sbtlDeleteSTPList);

INCLUDE_ASM(const s32, "code_00291C78", sbtlCallBtlSearchUnitID16);

void sbtlRefSbtl45(void) {
    SBTL_PANEL *panel = sbtlCallMemAllocDirect5();

    panel->resources = NULL;
    panel->hPanelSpr2 = NULL;
    panel->hPanelSpr3 = NULL;
    panel->hPanelSpr4 = NULL;
    panel->hPanelSpr5 = NULL;
    wk_sbtl->panel = panel;
    panel->mds_waku0.x = 314;
    panel->mds_waku0.y = 2;
    panel->mds_waku1.x = 314;
    panel->mds_waku1.y = 2;
    panel->mds_light.x = 297;
    panel->mds_light.y = 0;
    wk_sbtl->field630 = sbtlCallMemAllocDirect4();
}

INCLUDE_ASM(const s32, "code_00291C78", sbtlRefSbtl46);

SBTL_PANEL *sbtlCallMemAllocDirect5(void) {
    sdfMemHandle_t *handle = sdfMemAllocDirect(sizeof(SBTL_PANEL));
    SBTL_PANEL *panel = sdfMemLock(handle);

    memset(panel, 0, sizeof(SBTL_PANEL));
    panel->handle = handle;
    panel->resources = NULL;
    panel->hPanelSpr2 = NULL;
    panel->hPanelSpr3 = NULL;
    panel->hPanelSpr4 = NULL;
    panel->hPanelSpr5 = NULL;
    panel->list = InitDebugProcessData(0);
    panel->alpha = 0;
    panel->mode = 0;
    return panel;
}

INCLUDE_ASM(const s32, "code_00291C78", sbtlCallMemFreeDirect5);

s32 sbtlRefSbtl47(void) {
    SBTL_PANEL *panel;

    if (wk_sbtl == NULL || (panel = wk_sbtl->panel) == NULL) {
        return 0;
    }
    if (!(panel->flg & 2)) {
        return 0;
    }
    sbtlLeaf77(panel);
    sbtlFunc13();
    sbtlCallCommandFLDSETTAKARAOPEN();
    sbtlRefSbtl42(wk_sbtl->field630);
    return 1;
}

s32 sbtlFunc13(void) {
    sbtlRefSbtl49();
    return 1;
}

INCLUDE_ASM(const s32, "code_00291C78", Proc_sbtlPanel);

INCLUDE_ASM(const s32, "code_00291C78", sbtlResetMidashiAnim);

INCLUDE_ASM(const s32, "code_00291C78", sbtlRefSbtl48);

s32 sbtlRefSbtl49(void) {
    return wk_sbtl->mode == 2;
}

s32 sbtlCallCommandFLDSETTAKARAOPEN(void) {
    sbtlCallRefSbtl51();
    return 1;
}

INCLUDE_ASM(const s32, "code_00291C78", sbtlCheckSelectAnim);

INCLUDE_ASM(const s32, "code_00291C78", sbtlSetSelectAnim);

INCLUDE_ASM(const s32, "code_00291C78", sbtlResetSelectAnim);

/* NON_MATCHING: 8 of 31 words match.  */
#ifdef NON_MATCHING
s32 sbtlRefSbtl27(void) {
    SBTL_PANEL *panel = wk_sbtl->panel;

    if (panel != NULL) {
        if (panel->resources == NULL) {
            return 0;
        }
        if (panel->mode != 3 && (panel->flg & 2)) {
            sbtlCallBtlSearchUnitID37();
            sbtlCallEventBattleStopChack2();
            sbtlCallBtlGetPlayerHitTarget();
            return sbtlLeaf78();
        }
    }
    return 0;
}
#else
INCLUDE_ASM(const s32, "code_00291C78", sbtlRefSbtl27);
#endif
