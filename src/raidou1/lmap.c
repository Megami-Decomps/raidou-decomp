#include "common.h"
#include "lmap.h"

INCLUDE_ASM(const s32, "lmap", Chr_GetExLineBase);

INCLUDE_ASM(const s32, "lmap", Chr_UpdatePosShip);

INCLUDE_ASM(const s32, "lmap", Chr_UpdatePosTrain);

INCLUDE_ASM(const s32, "lmap", Chr_UpdatePosShip_lmap);

INCLUDE_ASM(const s32, "lmap", Chr_UpdatePosAuto);

INCLUDE_ASM(const s32, "lmap", Chr_UpdateMove);

INCLUDE_ASM(const s32, "lmap", Chr_GetNearPortTableNo);

INCLUDE_ASM(const s32, "lmap", lmapCallLMAPPOINTFLAG);

INCLUDE_ASM(const s32, "lmap", lmapLockSetPointChrSub);

INCLUDE_ASM(const s32, "lmap", lmapLeaf6);

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

dds3ProcessFunc_t lmapLeaf8(dds3ProcessID_t *id) {
    return ActWnd_Main(id);
}

INCLUDE_ASM(const s32, "lmap", lmapCallMemFreeDelayedDirect);

INCLUDE_ASM(const s32, "lmap", ActWnd_Start);

INCLUDE_ASM(const s32, "lmap", lmapCallTexFreeDelayed);

INCLUDE_ASM(const s32, "lmap", lmapCallExistProcess);

INCLUDE_ASM(const s32, "lmap", lmapCallExistProcess2);

INCLUDE_ASM(const s32, "lmap", lmapCallDevLoadFileEx);

INCLUDE_ASM(const s32, "lmap", lmapCallFileCheckEnd);

INCLUDE_ASM(const s32, "lmap", lmapCallGrAllocPacketBuff);

INCLUDE_ASM(const s32, "lmap", Util_Point3To2);

INCLUDE_ASM(const s32, "lmap", Util_GetPadAnalog);

INCLUDE_ASM(const s32, "lmap", lmapLeaf9);

INCLUDE_ASM(const s32, "lmap", lmapLeaf10);

INCLUDE_ASM(const s32, "lmap", lmapLeaf11);

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

INCLUDE_ASM(const s32, "lmap", lmapLeaf13);

INCLUDE_ASM(const s32, "lmap", lmapCallTexFreeDelayed2);

INCLUDE_ASM(const s32, "lmap", lmapLoadFileLmapSub);

INCLUDE_ASM(const s32, "lmap", cmpSetStockVisible);

INCLUDE_ASM(const s32, "lmap", EffGrid_Release);

INCLUDE_ASM(const s32, "lmap", EffGrid_Main);

INCLUDE_ASM(const s32, "lmap", FldWnd_ProcessStart);

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

INCLUDE_ASM(const s32, "lmap", lmapLeaf14);

INCLUDE_ASM(const s32, "lmap", lmapCallExistProcess3);

INCLUDE_ASM(const s32, "lmap", lmapCallExistProcess4);

INCLUDE_ASM(const s32, "lmap", cmpInitDHWindow);

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
