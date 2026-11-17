#include "common.h"
#include "newdata.h"
#include "libsdf.h"
#include "kernel.h"
#include "basic.h"
#include "dds3KernelCore.h"
#include "fclMenu.h"
#include "fclDrawBack.h"
#include "sdfMemory.h"
#include "fclMsg.h"

INCLUDE_ASM(const s32, "fclMenu", MoverUpdate);

INCLUDE_ASM(const s32, "fclMenu", fclSetApperTitle);

INCLUDE_ASM(const s32, "fclMenu", MoverSetApper);

INCLUDE_ASM(const s32, "fclMenu", fclSetDisapperTitle);

INCLUDE_ASM(const s32, "fclMenu", MoverSetDisapper);

INCLUDE_ASM(const s32, "fclMenu", _SetParam);

INCLUDE_ASM(const s32, "fclMenu", fclSetParamTitle);

INCLUDE_ASM(const s32, "fclMenu", fclSetParamFull);

INCLUDE_ASM(const s32, "fclMenu", fclSetParam);

INCLUDE_ASM(const s32, "fclMenu", fclSetFlagSelect);

INCLUDE_ASM(const s32, "fclMenu", fclSetFlagDecideEnd);

INCLUDE_ASM(const s32, "fclMenu", fclSetCursor);

INCLUDE_ASM(const s32, "fclMenu", GetMenu);

INCLUDE_ASM(const s32, "fclMenu", GetSelectMenu);

INCLUDE_ASM(const s32, "fclMenu", UpdateTargetMenu);

INCLUDE_ASM(const s32, "fclMenu", UpdateNextLink);

INCLUDE_ASM(const s32, "fclMenu", UpdatePrevLink);

INCLUDE_ASM(const s32, "fclMenu", fclSetLink);

INCLUDE_ASM(const s32, "fclMenu", fclStartUpdate);

INCLUDE_ASM(const s32, "fclMenu", fclGetSelectId);

INCLUDE_ASM(const s32, "fclMenu", fclGetSelectIdEx);

INCLUDE_ASM(const s32, "fclMenu", fclGetHierarchy);

INCLUDE_ASM(const s32, "fclMenu", fclSetTargetId);

INCLUDE_ASM(const s32, "fclMenu", fclGetTitleAlphaRate);

INCLUDE_ASM(const s32, "fclMenu", Init_fclMenu);

INCLUDE_ASM(const s32, "fclMenu", LoadFile_fclMenu);

INCLUDE_ASM(const s32, "fclMenu", Select_fclMenu);

INCLUDE_ASM(const s32, "fclMenu", fclUpdate);

INCLUDE_ASM(const s32, "fclMenu", DrawSpriteAlpha);

INCLUDE_ASM(const s32, "fclMenu", DrawSpriteList);

INCLUDE_ASM(const s32, "fclMenu", DrawFontAlpha);

INCLUDE_ASM(const s32, "fclMenu", DrawPanel);

INCLUDE_ASM(const s32, "fclMenu", DrawTitle);

void DrawCursor(s32 a_x, s32 a_y, u8 a_alpha) {
    DrawSpriteList(a_x, a_y, 0x100, a_alpha, &DrawCursorData);
}

INCLUDE_ASM(const s32, "fclMenu", DrawCursorSelect);

INCLUDE_ASM(const s32, "fclMenu", CalcAlpha);

INCLUDE_ASM(const s32, "fclMenu", fclGetPanelAlpha);

INCLUDE_ASM(const s32, "fclMenu", fclGetTitleAlpha);

INCLUDE_ASM(const s32, "fclMenu", fclDraw);

INCLUDE_ASM(const s32, "fclMenu", fclCreate);

INCLUDE_ASM(const s32, "fclMenu", fclCallMemFreeDelayedDirect);

void fclCallMemFreeDirect(fclMsgWork_t *wk) {
    if (wk->hData != NULL) {
        sdfMemFreeDirect(wk->hData);
        wk->hData = NULL;
    }
    wk->data = NULL;
}

INCLUDE_ASM(const s32, "fclMenu", dds3EffectBlurDraw);

INCLUDE_ASM(const s32, "fclMenu", fclLoadData);

INCLUDE_ASM(const s32, "fclMenu", fclLinkDataEx);

INCLUDE_ASM(const s32, "fclMenu", fclLinkData);

INCLUDE_ASM(const s32, "fclMenu", fclResetEx);

INCLUDE_ASM(const s32, "fclMenu", ButtonLineHelpDispSet);

INCLUDE_ASM(const s32, "fclMenu", fclSetSelInvild);

INCLUDE_ASM(const s32, "fclMenu", fclStart1);

INCLUDE_ASM(const s32, "fclMenu", fclStart2);

INCLUDE_ASM(const s32, "fclMenu", fclGetSelect);

INCLUDE_ASM(const s32, "fclMenu", fclSetMessagePosEx);

INCLUDE_ASM(const s32, "fclMenu", SetMessagePos);

INCLUDE_ASM(const s32, "fclMenu", fclSetClsModeEx);

INCLUDE_ASM(const s32, "fclMenu", fclRefMainPIDFclMsg);

INCLUDE_ASM(const s32, "fclMenu", _Update);

INCLUDE_ASM(const s32, "fclMenu", fclUpdateEx);

INCLUDE_ASM(const s32, "fclMenu", Update_fclMsg);

INCLUDE_ASM(const s32, "fclMenu", Process_fclMsg);

INCLUDE_ASM(const s32, "fclMenu", Create_fclMsg);

INCLUDE_ASM(const s32, "fclMenu", Delete_fclMsg);
