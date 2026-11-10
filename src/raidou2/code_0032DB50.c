#include "common.h"

/* Declarations from smt3-decomp src/smt3/facility/fclMisc.c */
#include "common.h"

#include "scr.h"

#include "facility.h"

#include "fpu.h"

#include "libsdf.h"

#include "newdata.h"

#include "ps2asm.h"

#include "event.h"

void fclFreeStage(void);

extern s32 dds3GetProcessStat(void *id);

extern u32 dds3GetRandA(void *name);

extern s32 fclSetWorkValue(s8 index, s32 value);

s32 fclGetFlag(s8 index);

void fclClearFlag(s8 index);

extern s8 gSelMsgPos;

extern s8 gMsgInvFlag;

extern s8 fclFlags[8];

extern s32 gMsgID;

extern s32 gSelMsgNo;

extern s8 fclEndMessageEx(s8 Mode);

extern void evtFreeStage(void);

extern s32 sdfDelayedResrcCheck(void);

extern void fclFilterOFF(void);

extern void sdfFileCmdReadFile(const char *aFileName, s32 aBuffer, s32 aLength, void *aCallback, void *aUserData);

extern void itfMesMngSetVarImmediate(s32 id, u32 index, s32 pstr);

extern s16 itfMesMngGetSelectNo(s32 msgid);

extern s16 itfMesMngGetSelPhase(s32 msgid);

extern s32 fclWorkValue[16];

extern s32 scrGetIntPara(s32 n);

extern void scrSetIntReturnValue(s32 i);

typedef struct
{
    u8 unk_00000[0x1928];
    u8 item[0xA0];
} fclGlobalWorkItemInventory_t;

extern fclGlobalWorkItemInventory_t *fclGlobalWorkItems FAKEMATCH_ASM_NAME("DDS3_GBWK");

extern s32 datAddItem(s32 id, s32 num);

extern s32 dds3KillProcess(s32 id, s32 shutdowntimer);

extern s32 fclChkMessage(void);

extern void fclScriptForcedEnd(u8 *PID);

extern u32 fclBlendColor(u32 firstColor, u32 secondColor, u32 weight);

extern s8 gMsgFlag;

extern void itfMesMngRelease(s32 id);

extern void *dds3GetControlScene(void);

extern void *dds3SceneBasicSearchNameAndType(void *scene, s32 type, char *name);

void fclInitPanelPos(s32 X, s32 Y, s32 Z, s16 W, s16 H, s32 Work, fclPanel_t *pPanelPos);

extern void shuffleIndices(s8 *order, s32 count, s32 rounds);

extern s32 cmpItemOrder(u8 *a, u8 *b);

extern s32 fclScriptStart(s32 pScriptData, s32 StartNo, void *pPID);

extern s32 datGetItemSlotIndex(s32 id);

extern void datAddHearts(s32 id);

s32 fclAddItem(u16 ItemID, s32 Nums);

typedef struct
{
    /* 0x00 */ u8 unk_00[0x18];
    /* 0x18 */ u8 *unk_18;
} fclSceneObj_t;

s32 fcl_213998(char *name, fclSceneObj_t *dst);

extern s32 dds3GetMapNo(void *scene);

extern void evtLoadArea(s32 fld, s32 area);

void fclLoadStage(s32 MajorNo, s32 MinorNo);

extern void dds3SetBlur(s32 frame, s32 alpha, s32 scale);

extern void dds3MultiBlurOff(s32 frame);

extern void dds3DistBlurOff(s32 frame);

extern void Eve_BlurOFF(void);

extern void Eve_PartialMultiBlurOFF(void);

extern void Eve_PartialMultiFilterOFF(void);

extern void Eve_DistortionBlurOFF(void);

extern void Eve_FilterOFF(void);

void fclFilterOFF(void);

void fclFreeStage(void);

void fclInitPanelPos(s32 X, s32 Y, s32 Z, s16 W, s16 H, s32 Work, fclPanel_t *pPanelPos);

void fclScriptForcedEnd(u8 *PID);

s32 fclScriptStart(s32 pScriptData, s32 StartNo, void *pPID);

typedef struct
{
    /* 0x00 */ u8 count;
    /* 0x01 */ u8 unk_01;
    /* 0x02 */ u16 item[0xA0];
} fclItemList_t;

void func_00213BB8(fclItemList_t *list);

extern s32 cmpItemHasFlag(u16 item, s8 which);

void fclCreateItemListEx(fclItemList_t *list, s8 which);

s32 cmpItemOrder(u8 *a, u8 *b);

s32 func_00213CD8(u8 *list, s32 count, u8 value);

void shuffleIndices(s8 *order, s32 count, s32 rounds);

extern char *text2[];

char *func_00213DC0(datUnitWork_t *unit);

u32 func_00213DF0(u32 color, u32 scaleFactor);

u32 fclBlendColor(u32 firstColor, u32 secondColor, u32 weight);

extern s32 itfMesMngInitialize(void *data);

extern void itfMesMngChangeWindowType(s32 id, s32 type, s32 alpha);

s8 fclInitMessage(void *pData);

extern void itfMesMngMarkBitStat(s32 id, s32 flag);

extern void itfMesMngRequestMessage(s32 msg, s32 id, s32 flag);

extern void itfMesMngSetSelPhase(s32 msgid, s32 phase);

s32 fclStartMessage(s32 MsgNo);

s32 fclStartSelMessage(s32 SelMsgNo);

void fclSetSelMessagePos(s8 SelPos);

s8 fclGetCurrentSelMessagePos(void);

s8 fclGetSelMessagePos(void);

extern void itfMesMngSetPhase(s32 msgid, s32 phase);

extern void itfMesMngMsgWndCls(s32 msgid);

extern void itfMesMngRequestMessageRelease(s32 id, s32 cls_speaker);

extern void fclSetMessageInvalid(s8 Mode);

s8 fclEndMessageEx(s8 Mode);

s8 fclEndMessage(void);

s32 fclDestroyMessage(void);

s32 fclChkMessage(void);

s32 fclChkSelMessage(void);

extern s8 itfMesMngGetPhase(s32 msgid);

extern s32 itfMesMngSetSelectDefKey(s32 id, s32 key, s32 index);

extern void itfMesMngSetSelect(s32 id, s32 selno);

extern void itfMesMngRequestSelect(s32 id, s32 select);

void fclUpdateMessage(void);

void fclSetMessageVar_20743088(s8 Index, saddr_t pStr);

s8 fcl_214270(void);

extern void itfMesMngClearBitStat(s32 id, s32 flag);

void fclSetMessageInvalid(s8 Mode);

extern void itfMesMngSetMesPosition(s32 id, s32 x, s32 y);

extern void itfMesMngSetOffsetY1(s32 msgid, s32 y1);

void fcl_2142F0(s32 x, s32 y);

s32 fclChkMessageDraw(void *PID);

void fclClearFlag(s8 index);

s32 fclGetFlag(s8 index);

s32 fclSetWorkValue(s8 index, s32 value);

s8 fclGetWorkValue(s8 index);

s32 fcl_214428(void);

s32 fcl_214458(void);

s32 fclLoadFileDone(s32 unused, s32 cmd, s32 value, s32 unused2, s32 *out);

void fclLoadFileAsync(const char *name, void *buf);

extern s32 gMsgID;

extern s8 gMsgFlag;

extern s8 gMsgInvFlag;

extern s32 gSelMsgNo;

extern s8 gSelMsgPos;

extern char fclStrFee[4];

extern u32 D_0054FC68[2];

extern char fclStrOwn[4];

extern u32 D_0054FC78[2];

extern u32 D_0054FC80[2];

extern char fclStrTotal[6];

extern char fclStrMoney[6];

extern char fclStrNo[3];

extern char fclStrYes[4];
/* End of declarations from src/smt3/facility/fclMisc.c */

INCLUDE_ASM(const s32, "code_0032DB50", uiDrawSurfaceAtNearDepth);

INCLUDE_ASM(const s32, "code_0032DB50", func_0032DBA8);

u32 fclBlendColor(u32 firstColor, u32 secondColor, u32 weight)
{
    u32 a0 = firstColor >> 24;
    u32 r0 = (firstColor & 0xFF0000) >> 16;
    u32 g0 = (firstColor & 0xFF00) >> 8;
    u32 b0 = firstColor & 0xFF;
    u32 a1 = secondColor >> 24;
    u32 r1 = (secondColor & 0xFF0000) >> 16;
    u32 g1 = (secondColor & 0xFF00) >> 8;
    u32 b1 = secondColor & 0xFF;
    u32 inv;

    if (weight > 0x100) {
        weight = 0x200 - weight;
    }
    inv = 0x100 - weight;
    a0 = (a0 * weight + a1 * inv) >> 8;
    r0 = (r0 * weight + r1 * inv) >> 8;
    g0 = (g0 * weight + g1 * inv) >> 8;
    b0 = (b0 * weight + b1 * inv) >> 8;
    return (a0 << 24) | (r0 << 16) | (g0 << 8) | b0;
}

void func_0032DCA8(void) {
}

void itfInitializeAdminInterfaceNoop(void) {
}

INCLUDE_ASM(const s32, "code_0032DB50", itfReturnZeroDuplicate);

INCLUDE_ASM(const s32, "code_0032DB50", func_0032DCC0);

INCLUDE_ASM(const s32, "code_0032DB50", func_0032DCF0);

INCLUDE_ASM(const s32, "code_0032DB50", func_0032DD38);

INCLUDE_ASM(const s32, "code_0032DB50", func_0032DE50);

INCLUDE_ASM(const s32, "code_0032DB50", itfDestroyGridTextWidget);

INCLUDE_ASM(const s32, "code_0032DB50", itfDestroyGridTextWidgetTree);

INCLUDE_ASM(const s32, "code_0032DB50", func_0032DF40);

INCLUDE_ASM(const s32, "code_0032DB50", itfGetGridListLinkFlags);

INCLUDE_ASM(const s32, "code_0032DB50", itfAppendGridTextItem);

INCLUDE_ASM(const s32, "code_0032DB50", func_0032E0F8);

INCLUDE_ASM(const s32, "code_0032DB50", itfRemoveSelectedGridTextItem);

INCLUDE_ASM(const s32, "code_0032DB50", itfReplaceGridTextAndExpandColumn);

INCLUDE_ASM(const s32, "code_0032DB50", func_0032E350);

INCLUDE_ASM(const s32, "code_0032DB50", itfAdvanceGridScrollPosition);

INCLUDE_ASM(const s32, "code_0032DB50", itfAdvanceSelectedGridScroll);

INCLUDE_ASM(const s32, "code_0032DB50", itfReverseGridScrollPosition);

INCLUDE_ASM(const s32, "code_0032DB50", itfReverseSelectedGridScroll);

INCLUDE_ASM(const s32, "code_0032DB50", func_0032E678);

INCLUDE_ASM(const s32, "code_0032DB50", func_0032E6E8);

INCLUDE_ASM(const s32, "code_0032DB50", func_0032E8E0);

INCLUDE_ASM(const s32, "code_0032DB50", itfFindGridNodeByKey);

INCLUDE_ASM(const s32, "code_0032DB50", func_0032EB88);

INCLUDE_ASM(const s32, "code_0032DB50", func_0032EC00);
INCLUDE_SDATA(const s32, "code_0032DB50", D_004E3740);

INCLUDE_SDATA(const s32, "code_0032DB50", D_004E3748);

INCLUDE_SDATA(const s32, "code_0032DB50", D_004E3750);

INCLUDE_SDATA(const s32, "code_0032DB50", D_004E3758);

