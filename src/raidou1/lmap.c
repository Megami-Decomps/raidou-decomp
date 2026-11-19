#include "common.h"
#include "lmap.h"
#include "sdfTex.h"
#include "sdfMemory.h"
#include "sdf_resource.h"
#include "dds3KernelCore.h"
#include "sdk_libc.h"

INCLUDE_ASM(const s32, "lmap", Chr_GetExLineBase);

INCLUDE_ASM(const s32, "lmap", Chr_UpdatePosShip);

INCLUDE_ASM(const s32, "lmap", Chr_UpdatePosTrain);

INCLUDE_ASM(const s32, "lmap", Chr_UpdatePosShip_lmap);

INCLUDE_ASM(const s32, "lmap", Chr_UpdatePosAuto);

INCLUDE_ASM(const s32, "lmap", Chr_UpdateMove);

INCLUDE_ASM(const s32, "lmap", Chr_GetNearPortTableNo);

INCLUDE_ASM(const s32, "lmap", lmapCallLMAPPOINTFLAG);

INCLUDE_ASM(const s32, "lmap", lmapLockSetPointChrSub);

INCLUDE_ASM(const s32, "lmap", Lock_SetLineNo);

INCLUDE_ASM(const s32, "lmap", Lock_SetPointChr);

INCLUDE_ASM(const s32, "lmap", lmapActWndInitSub);

INCLUDE_ASM(const s32, "lmap", lmapLeaf7);

INCLUDE_ASM(const s32, "lmap", lmapCallSetFontString);

INCLUDE_ASM(const s32, "lmap", lmapActWndMainSub);

INCLUDE_ASM(const s32, "lmap", lmapActWndDrawSub);

INCLUDE_ASM(const s32, "lmap", ActWnd_Init);

INCLUDE_ASM(const s32, "lmap", ActWnd_Main);

INCLUDE_ASM(const s32, "lmap", ActWnd_Draw);

void ActWnd_Reset(dds3ProcessID_t *proc) {
    ActWnd_Init(proc);
}

dds3ProcessFunc_t ActWnd_DrawProcess(dds3ProcessID_t *proc) {
    return ActWnd_Draw(proc);
}

INCLUDE_ASM(const s32, "lmap", ActWnd_Initialize);

dds3ProcessFunc_t ActWnd_Process(dds3ProcessID_t *id) {
    return ActWnd_Main(id);
}

INCLUDE_ASM(const s32, "lmap", ActWnd_Shutdown);

INCLUDE_ASM(const s32, "lmap", ActWnd_Start);

INCLUDE_ASM(const s32, "lmap", lmapCallTexFreeDelayed);

INCLUDE_ASM(const s32, "lmap", lmapCallExistProcess);

INCLUDE_ASM(const s32, "lmap", lmapCallExistProcess2);

sdfTexHandle_t *lmapCallDevLoadFileEx(const char *filename)
{
    sdfMemHandle_t *file;
    sdfTexHandle_t *tex;
    u32 info[4];

    file = sdfDevLoadFileEx(filename, info, NULL);
    tex = sdfTexSet(EE_ADDR_AS(TMX_HEADER_t *, info[0]));
    sdfMemFreeDelayedDirect(file);
    return tex;
}

INCLUDE_ASM(const s32, "lmap", lmapCallFileCheckEnd);

INCLUDE_ASM(const s32, "lmap", lmapCallGrAllocPacketBuff);

INCLUDE_ASM(const s32, "lmap", Util_Point3To2);

INCLUDE_ASM(const s32, "lmap", Util_GetPadAnalog);

/* body: matched in raidou @ 88f0d54: src/raidou1/code_0018CE10.c:167, match, same function 1.00 (words 15/15, calls 1.00, shape 1.00) */
s32 UtilFloatToSdf32s(f32 a_value, s32 a_shift)
{
    s32 whole = (s32)a_value;

    return (whole << a_shift) + (s32)((a_value - (f32)whole) * (f32)(1 << a_shift));
}

INCLUDE_ASM(const s32, "lmap", Util_GetVerticalVector);

INCLUDE_ASM(const s32, "lmap", Util_GetVerticalVector2);

INCLUDE_ASM(const s32, "lmap", lmapFunc4);

INCLUDE_ASM(const s32, "lmap", lmapChrUpdatePosShipPadSub);

INCLUDE_ASM(const s32, "lmap", lmapLeaf12);

INCLUDE_ASM(const s32, "lmap", lmapLMAPPAC);

INCLUDE_ASM(const s32, "lmap", lmapCallMemGetPtr);

INCLUDE_ASM(const s32, "lmap", FldWnd_CallStart);

INCLUDE_ASM(const s32, "lmap", _FldWnd_CallEnd);

INCLUDE_ASM(const s32, "lmap", FldWnd_Process);

INCLUDE_ASM(const s32, "lmap", FldWnd_DrawThumbnail);

INCLUDE_ASM(const s32, "lmap", FldWnd_DrawProcess);

INCLUDE_ASM(const s32, "lmap", FldWnd_Start);

INCLUDE_ASM(const s32, "lmap", FldWnd_CallEnd);

INCLUDE_ASM(const s32, "lmap", lmapCallTexFreeDelayed2);

INCLUDE_ASM(const s32, "lmap", lmapLoadFileLmapSub);

void cmpSetStockVisible(s32 a_bDraw) {
    struct tagLMAP_FLDWND_WORK *wk = g_mainPID_FldWnd->workdata;

    if (a_bDraw << 24) {
        wk->flags |= 2;
    } else {
        wk->flags &= ~2;
    }
}

INCLUDE_ASM(const s32, "lmap", EffGrid_Release);

INCLUDE_ASM(const s32, "lmap", EffGrid_Main);

void FldWnd_ProcessStart(void) {
    sdfMemHandle_t *hMem;
    struct tagLMAP_FLDWND_WORK *wk;

    hMem = sdfMemAllocDirect(sizeof(struct tagLMAP_FLDWND_WORK));
    wk = sdfMemGetPtr(hMem);
    memset(wk, 0, sizeof(struct tagLMAP_FLDWND_WORK));
    wk->hMem = hMem;
    g_mainPID_FldWnd = dds3StartProcess(FldWnd_ProcessStartStrLmapFldWndMainProc, 0x406, 0, 0, (dds3ProcessFunc_t)FldWnd_Process, EffGrid_Main, wk);
    wk->drawPID = dds3StartProcess(FldWnd_ProcessStartStrLmapFldWndDrawProc, 0x2B17, 0, 0, (dds3ProcessFunc_t)FldWnd_DrawProcess, NULL, wk);
    lmapLMAPPAC(wk);
}

INCLUDE_ASM(const s32, "lmap", EffGrid_Finish);

INCLUDE_ASM(const s32, "lmap", EffGrid_InitCB);

INCLUDE_ASM(const s32, "lmap", EffGrid_InitCBAll);

INCLUDE_ASM(const s32, "lmap", EffGrid_SetCB);

INCLUDE_ASM(const s32, "lmap", EffGrid_SetCBAll);

INCLUDE_ASM(const s32, "lmap", EffGrid_UpdateCBMove);

INCLUDE_ASM(const s32, "lmap", EffGrid_UpdateCBHold);

INCLUDE_ASM(const s32, "lmap", EffGrid_UpdateCB);

INCLUDE_ASM(const s32, "lmap", EffGrid_UpdateCBAll);

INCLUDE_ASM(const s32, "lmap", EffGrid_DrawCB);

INCLUDE_ASM(const s32, "lmap", EffGrid_DrawCBAll);

INCLUDE_ASM(const s32, "lmap", EffGrid_CheckWait);

INCLUDE_ASM(const s32, "lmap", EffGrid_Init);

INCLUDE_ASM(const s32, "lmap", lmapCallEffGridSetCBAll);

INCLUDE_ASM(const s32, "lmap", EffGrid_Draw);

dds3ProcessFunc_t EffGrid_DrawProcess(dds3ProcessID_t *proc) {
    return EffGrid_Draw(proc);
}

INCLUDE_ASM(const s32, "lmap", EffGrid_Initialize);

dds3ProcessFunc_t EffGrid_Process(dds3ProcessID_t *proc) {
    return lmapCallEffGridSetCBAll(proc);
}

INCLUDE_ASM(const s32, "lmap", EffGrid_Shutdown);

INCLUDE_ASM(const s32, "lmap", EffGrid_Start);

INCLUDE_ASM(const s32, "lmap", EffGrid_ReleaseDraw);

INCLUDE_ASM(const s32, "lmap", lmapCallExistProcess3);

INCLUDE_ASM(const s32, "lmap", lmapCallExistProcess4);

void cmpInitDHWindow(lmapDHWindow_t *pWin) {
    pWin->unk20 = 0;
    pWin->unk24 = 0;
    pWin->unk28 = 0;
    pWin->unk29 = 0;
    pWin->unk2A = 0;
    pWin->unk2B = -2;
    pWin->unk2C = -2;
    pWin->unk2D = 0;
}

INCLUDE_ASM(const s32, "lmap", lmapCallExistProcess5);

INCLUDE_ASM(const s32, "lmap", lmapCallExistProcess6);

INCLUDE_ASM(const s32, "lmap", lmapCallExistProcess7);

INCLUDE_ASM(const s32, "lmap", lmapFldWndLinkSpriteSub);

INCLUDE_ASM(const s32, "lmap", lmapFunc5);

INCLUDE_ASM(const s32, "lmap", lmapCallInitDHWindow);

INCLUDE_ASM(const s32, "lmap", FldWnd_LinkSprite);

INCLUDE_ASM(const s32, "lmap", lmapCallSetOt);

INCLUDE_ASM(const s32, "lmap", lmapCallMemFreeDirect);

INCLUDE_ASM(const s32, "lmap", EffWave_Start);
