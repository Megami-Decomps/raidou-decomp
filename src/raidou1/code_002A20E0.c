#include "common.h"
#include "sbtlWork.h"
#include "sdfChip.h"
#include "sBtlDvltk.h"
#include "dds3KernelMain.h"
#include "sdfMath.h"
#include "sBtlPlayer.h"
#include "sdfMemory.h"

INCLUDE_ASM(const s32, "code_002A20E0", sbtlCallBtlCheckMsgMemType);

INCLUDE_ASM(const s32, "code_002A20E0", sbtlCreateDvltk_Priority);

INCLUDE_ASM(const s32, "code_002A20E0", sbtlCallMemAllocDirect10);

s32 Proc_sbtlDvltk(struct SBTLDVLTK_LISTWORK *lwk)
{
    SBTLDVLTK_WORK *dwk;
    f32 s;

    if (lwk == NULL) {
        return 0;
    }
    dwk = lwk->dwk;
    if (dwk == NULL || lwk->num == 0) {
        return 0;
    }
    if (sbtlData104 == NULL) {
        if (wk_sbtl->flg & 0x8000000) {
            return 0;
        }
        if (wk_sbtl->flg2 & 0x12) {
            return 0;
        }
    }
    while (dwk != NULL) {
        switch (dwk->status) {
        case SBTLDVLTK_ST_INIT:
            dwk->sincount = (dwk->sincount + 12) % 360;
            if ((u32)dwk->sincount < 180) {
                s = sdfSin(((f32)((dwk->sincount + 90) % 360) / 180.0f) * 3.1415899f);
                dwk->ofs_x = (s + 1.0f) * 0.5f * 128.0f + 386.0f;
                dwk->alpha += 12;
                dwk->alpha = dwk->alpha > 0 ? (dwk->alpha < 129 ? dwk->alpha : 128) : 0;
            } else {
                dwk->ofs_x = 386;
                dwk->alpha = 128;
                dwk->status = SBTLDVLTK_ST_PROC;
                if (dwk->mode == 15) {
                    sbtlSbattle3();
                    wk_sbtl->eventno = 6;
                    wk_sbtl->eventcounter = 0;
                    wk_sbtl->flg |= 0x400;
                    wk_sbtl->eventgstat = 0;
                    sbtlSetNSTOP();
                }
            }
            break;
        case SBTLDVLTK_ST_PROC:
            dwk->counter++;
            dwk->counter = dwk->counter > 0 ? (dwk->counter < dwk->cmpcounter ? dwk->counter : dwk->cmpcounter) : 0;
            if (dwk->counter >= dwk->cmpcounter) {
                dwk->status = SBTLDVLTK_ST_END;
            }
            break;
        }
        dwk = dwk->next;
    }
    return 1;
}

INCLUDE_ASM(const s32, "code_002A20E0", sbtlSearchDvltk);

s32 Draw_sbtlDvltk(struct SBTLDVLTK_LISTWORK *lwk)
{
    SBTLDVLTK_WORK *dwk;
    s32 last = FALSE;

    if (sbtlGetSpriteHandle() == NULL) {
        return 0;
    }
    if (lwk == NULL) {
        return 0;
    }
    dwk = lwk->dwk;
    if (dwk == NULL || lwk->num == 0) {
        return 0;
    }
    if (sbtlData104 == NULL && !dds3BgSetScrollSub()) {
        if ((wk_sbtl->flg & 0x8000000) && dwk->mode != 15) {
            return 0;
        }
        if (wk_sbtl->flg2 & 0x12) {
            return 0;
        }
        if (wk_sbtl->flg2 & 0x80) {
            return 0;
        }
    }
    while (dwk != NULL) {
        if (dwk->mode == 0) {
            last = TRUE;
        }
        if (last == TRUE || dwk->next == NULL) {
            if (dwk->status < SBTLDVLTK_ST_END) {
                sbtlDrawText(dwk);
            }
        }
        dwk = dwk->next;
        if (last == TRUE) {
            return 1;
        }
    }
    return 1;
}

s32 sbtlReleaseDvltk_List(SBTLDVLTK_LISTWORK *stp, u8 flg)
{
    SBTLDVLTK_WORK *node;
    SBTLDVLTK_WORK *next;

    if (stp == NULL) {
        return 0;
    }
    node = stp->dwk;
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
                stp->dwk = node->next;
                if (stp->dwk != NULL) {
                    stp->dwk->prev = NULL;
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

INCLUDE_ASM(const s32, "code_002A20E0", sbtlDeleteDvltkList);

INCLUDE_ASM(const s32, "code_002A20E0", sbtlCallMessageNumSearch3);

INCLUDE_ASM(const s32, "code_002A20E0", sbtlLeaf99);

INCLUDE_ASM(const s32, "code_002A20E0", sbtlDrawText);

INCLUDE_ASM(const s32, "code_002A20E0", sbtlAnimWaku);

INCLUDE_ASM(const s32, "code_002A20E0", sbtlBtlDrawTextSub2);

struct sprHandle_t *sbtlGetSpriteHandle(void)
{
    if (sbtlData104 == NULL) {
        if (wk_sbtl->panel != NULL) {
            return wk_sbtl->panel->hPanelSpr5;
        }
        return NULL;
    }
    return sbtlData105;
}

INCLUDE_ASM(const s32, "code_002A20E0", sbtlExt_CalcProcess);

INCLUDE_ASM(const s32, "code_002A20E0", sbtlExt_DrawProcess);

INCLUDE_ASM(const s32, "code_002A20E0", sbtlExt_Free);

INCLUDE_ASM(const s32, "code_002A20E0", sbtlExt_ShutDown);

INCLUDE_RODATA(const s32, "code_002A20E0", sbtlLeaf99Table);

INCLUDE_RODATA(const s32, "code_002A20E0", sbtlDrawTextTable);

INCLUDE_ASM(const s32, "code_002A20E0", sbtlExt_CreateBuffer);

INCLUDE_ASM(const s32, "code_002A20E0", sbtlExt_FreeBuffer);

INCLUDE_ASM(const s32, "code_002A20E0", sbtlExt_MesReqEx);

INCLUDE_ASM(const s32, "code_002A20E0", sbtlExt_MesReq);

INCLUDE_ASM(const s32, "code_002A20E0", sbtlExt_EndMes);

s32 sbtlExt_ChkMes(void)
{
    return sbtlData104 != NULL;
}

INCLUDE_ASM(const s32, "code_002A20E0", sbtlExtCommand_DVLTK_DSP);

INCLUDE_ASM(const s32, "code_002A20E0", sbtlExtCommand_DVLTK_WAIT);
