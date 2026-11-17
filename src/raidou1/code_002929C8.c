#include "common.h"
#include "sbtlWork.h"
#include "sBtlUnit.h"
#include "sdfMemory.h"
#include "sdfChip.h"
#include "fileManager.h"
#include "code_r1_002C8188.h"
#include "sBtlEff.h"
#include "code_002929C8.h"

s32 sbtlCheckLoadFlg(void) {
    u32 load;

    if (wk_sbtl == NULL || wk_sbtl->panel == NULL) {
        return 0;
    }
    load = wk_sbtl->panel->flg & 2;
    return load != 0;
}

/* NON_MATCHING: 144 of 151 words match.  */
#ifdef NON_MATCHING
s32 sbtlCallCheckEnd(void) {
    SBTL_PANEL *panel;
    sdfPacDecodeResrc_t *node;
    s32 i;
    u32 flg;

    if (wk_sbtl == NULL || (panel = wk_sbtl->panel) == NULL) {
        return 0;
    }
    flg = panel->flg;
    if (flg & 2) {
        return 1;
    }
    if (wk_sbtl->panelPackPos < 3) {
        if (!pacCheckEnd(wk_sbtl->panelPack)) {
            return 0;
        }
        node = wk_sbtl->panelPack->pac.resrcHead;
        if (wk_sbtl->panelPackPos == 0) {
            i = 0;
            wk_sbtl->panelPackPos = 1;
        } else {
            for (i = 0; i < wk_sbtl->panelPackLen; i++) {
                node = node->next;
            }
        }
        while (node != NULL) {
            if (wk_sbtl->panelPackPos == 1) {
                for (; i < 4; i++) {
                    switch (i) {
                    case 0:
                        panel->resources = effCreateResourceSlotSetFromAllocation(node->resrc, 0);
                        sdfMemFreeDirect(node->resrc);
                        break;
                    case 1:
                        panel->hPanelSpr3 = effCreateResourceSlotSetFromAllocation(node->resrc, 0);
                        sdfMemFreeDirect(node->resrc);
                        break;
                    case 2:
                        panel->hPanelSpr4 = effCreateResourceSlotSetFromAllocation(node->resrc, 0);
                        sdfMemFreeDirect(node->resrc);
                        break;
                    case 3:
                        panel->hPanelSpr5 = effCreateResourceSlotSetFromAllocation(node->resrc, 0);
                        sdfMemFreeDirect(node->resrc);
                        break;
                    }
                    node = node->next;
                    wk_sbtl->panelPackLen++;
                }
                wk_sbtl->panelPackPos = 2;
            }
            while (!sbtlFunc(wk_sbtl->panelPackLen, node->data)) {
            }
            sdfMemFreeDirect(node->resrc);
            wk_sbtl->panelPackLen++;
            node = node->next;
        }
        pacFileHandleFree_(wk_sbtl->panelPack);
        wk_sbtl->panelPackPos = 3;
        panel->flg |= 2;
        return 1;
    }
    panel->flg = flg | 2;
    return 1;
}
#else
INCLUDE_ASM(const s32, "code_002929C8", sbtlCallCheckEnd);
#endif

s32 sbtlLeaf77(struct SBTL_PANEL *panel) {
    if (panel == NULL) {
        return 0;
    }
    if (!(panel->flg & 1)) {
        return 0;
    }
    if (!(panel->flg & 2)) {
        return 0;
    }
    switch (panel->mode) {
    case 1:
        break;
    case 0:
        panel->alpha += 16;
        panel->alpha = (panel->alpha <= 0) ? 0 : (panel->alpha > 128) ? 128 : panel->alpha;
        if (panel->alpha >= 128) {
            panel->mode = 1;
        }
        break;
    case 2:
        panel->alpha -= 16;
        panel->alpha = (panel->alpha <= 0) ? 0 : (panel->alpha > 128) ? 128 : panel->alpha;
        if (panel->alpha <= 0) {
            panel->mode = 3;
        }
        break;
    }
    return 1;
}

s32 sbtlLeaf78(void) {
    return 1;
}

INCLUDE_ASM(const s32, "code_002929C8", sbtlLeaf79);

INCLUDE_ASM(const s32, "code_002929C8", sbtlLeaf80);

INCLUDE_ASM(const s32, "code_002929C8", sbtlBtlDrawTextSub);

INCLUDE_ASM(const s32, "code_002929C8", sbtlLeaf81);

INCLUDE_ASM(const s32, "code_002929C8", sbtlDrawSprite_RGBA2);

INCLUDE_ASM(const s32, "code_002929C8", sbtlRefSbtl50);

INCLUDE_ASM(const s32, "code_002929C8", sbtlCallBtlSearchUnitID17);

INCLUDE_ASM(const s32, "code_002929C8", mphStartLoadID);

INCLUDE_ASM(const s32, "code_002929C8", sbtlRefSbtl28);

INCLUDE_ASM(const s32, "code_002929C8", fldConsumeSceneInputFlags);

INCLUDE_ASM(const s32, "code_002929C8", sbtlCallMemset19);

u32 sbtlCallRefSbtl51(void) {
    sbtlSummonMenuProc(sbtlSearchUnit_ID(wk_sbtl->unitList, wk_sbtl->targetUnitID));
    return 1;
}

INCLUDE_RODATA(const s32, "code_002929C8", sbtlRefSbtl50Table);

INCLUDE_RODATA(const s32, "code_002929C8", mphStartLoadIDTable);

void sbtlSummonMenuProc(struct SBTLUNIT_WORK *wk) {
    switch (wk_sbtl->field436) {
    case 1:
        sbtlRefSbtl51Data[wk_sbtl->field440](wk, &wk_sbtl->mode);
        break;
    case 2:
        sbtlRefSbtl51Data2[wk_sbtl->field440](wk, &wk_sbtl->mode);
        break;
    case 3:
        sbtlRefSbtl51Data3[wk_sbtl->field440](wk, &wk_sbtl->mode);
        break;
    case 4:
        sbtlRefSbtl51Data4[wk_sbtl->field440](wk, &wk_sbtl->mode);
        break;
    case 5:
        sbtlRefSbtl51Data5[wk_sbtl->field440](wk, &wk_sbtl->mode);
        break;
    case 6:
        sbtlRefSbtl51Data6[wk_sbtl->field440](wk, &wk_sbtl->mode);
        break;
    case 7:
        sbtlRefSbtl51Data7[wk_sbtl->field440](wk, &wk_sbtl->mode);
        break;
    }
}

INCLUDE_ASM(const s32, "code_002929C8", sbtlResetSummonPanel);

INCLUDE_ASM(const s32, "code_002929C8", SummonMenuFunc_Open00);

INCLUDE_ASM(const s32, "code_002929C8", SummonMenuFunc_00);

INCLUDE_ASM(const s32, "code_002929C8", SummonMenuFunc_10);

INCLUDE_ASM(const s32, "code_002929C8", sbtlGetMenuStatus);

INCLUDE_ASM(const s32, "code_002929C8", sbtlResetMenuPara);

INCLUDE_ASM(const s32, "code_002929C8", sbtlCancelMenuPara);

INCLUDE_RODATA(const s32, "code_002929C8", SummonMenuFunc_10Table);

INCLUDE_ASM(const s32, "code_002929C8", sbtlSetMenuPara);

INCLUDE_ASM(const s32, "code_002929C8", SummonMenuFunc_50);

INCLUDE_ASM(const s32, "code_002929C8", SummonMenuFunc_60);

INCLUDE_ASM(const s32, "code_002929C8", SummonMenuFunc_20);

INCLUDE_ASM(const s32, "code_002929C8", SummonMenuFunc_30);

INCLUDE_ASM(const s32, "code_002929C8", SummonMenuFunc_40);

INCLUDE_ASM(const s32, "code_002929C8", SummonMenuFunc_70);

INCLUDE_ASM(const s32, "code_002929C8", sbtlResetColorFlag);

INCLUDE_ASM(const s32, "code_002929C8", CheckSummonTarget);

struct SBTL_MODELBIND_LIST *effAllocateModelBindings(void) {
    struct SBTL_MODELBIND_LIST *list = sdfChipAllocZeroDirect(sizeof(SBTL_MODELBIND_LIST));

    list->num = 0;
    list->top = NULL;
    return list;
}

INCLUDE_ASM(const s32, "code_002929C8", sbtlCallMemAllocDirect6);

s32 Proc_sbtlEXP(struct SBTL_MODELBIND_LIST *lwk) {
    SBTL_MODELBIND *wk;

    if (lwk == NULL) {
        return 0;
    }
    wk = lwk->top;
    if (wk == NULL || lwk->num == 0) {
        return 0;
    }
    if (wk_sbtl->flg & 0x40) {
        return 0;
    }
    while (wk != NULL) {
        if (wk->status == 1) {
            if (wk->flg & 1) {
                wk_sbtl->field644 += wk->para;
                wk_sbtl->field644 = wk_sbtl->field644 > 0 ? (wk_sbtl->field644 < 1000000 ? wk_sbtl->field644 : 999999) : 0;
                sbtlCallBtlSearchUnitID26(wk);
            }
            if (wk->flg & 2) {
                wk_sbtl->field644 -= wk->para;
                wk_sbtl->field644 = wk_sbtl->field644 > 0 ? (wk_sbtl->field644 < 1000000 ? wk_sbtl->field644 : 999999) : 0;
                sbtlCallBtlSearchUnitID26(wk);
            }
            wk->status = 2;
        }
        wk = wk->next;
    }
    return 1;
}

s32 sbtlCallMemFreeDirect6(struct SBTL_MODELBIND_LIST *stp, u8 flg) {
    struct SBTL_MODELBIND *node;
    struct SBTL_MODELBIND *next;


    if (stp == NULL) {
        return 0;
    }
    node = stp->top;
    if (node == NULL || stp->num == 0) {
        return 0;
    }
    do {
        if (flg) {
            node->status = 2;
        }
        if (node->status == 2) {
            if (node->prev != NULL) {
                node->prev->next = node->next;
            }
            if (node->next != NULL) {
                node->next->prev = node->prev;
            }
            if (node->prev == NULL) {
                stp->top = node->next;
                if (stp->top != NULL) {
                    stp->top->prev = NULL;
                }
            }
            next = node->next;
            sdfMemFreeDirect(node->handle);
            node = next;
            stp->num--;
        } else {
            node = node->next;
        }
    } while (node != NULL);
    return 1;
}

INCLUDE_ASM(const s32, "code_002929C8", sbtlBattleEnd);

INCLUDE_ASM(const s32, "code_002929C8", sbtlCallBtlSearchUnitID26);

INCLUDE_ASM(const s32, "code_002929C8", sbtlLeaf82);

INCLUDE_ASM(const s32, "code_002929C8", sbtlCallBtlCheckTutorialBattle);

INCLUDE_RODATA(const s32, "code_002929C8", sbtlCallBtlSearchUnitID26Table);

INCLUDE_RODATA(const s32, "code_002929C8", sbtlCallBtlSearchUnitID26Table2);

