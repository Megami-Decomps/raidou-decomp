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

s32 fcl_213998(char *name, fclSceneObj_t *dst)
{
    fclSceneObj_t *obj = dds3SceneBasicSearchNameAndType(dds3GetControlScene(), 9, name);

    if (obj == NULL) {
        return 0;
    }
    *(s32 *)(dst->unk_18 + 0x80) = *(s32 *)(obj->unk_18 + 0x78);
    return 1;
}

void fclLoadStage(s32 MajorNo, s32 MinorNo)
{
    s32 id = (MajorNo << 16) + MinorNo;

    if (dds3GetMapNo(dds3GetControlScene()) == id) {
        return;
    }
    evtLoadArea(MajorNo, MinorNo);
}

INCLUDE_ASM(const s32, "facility/fclMisc", func_00123BF8);

INCLUDE_ASM(const s32, "facility/fclMisc", func_00123C58);

INCLUDE_ASM(const s32, "facility/fclMisc", func_00123C78);

void fclScriptForcedEnd(u8 *PID)
{
    if (PID != NULL && dds3GetProcessStat(PID) != 0) {
        dds3KillProcess(PID, 0);
        PID = NULL;
    }
}

s32 fclScriptStart(s32 pScriptData, s32 StartNo, void *pPID)
{
    saddr_t handle;

    if (pPID != NULL)
    {
        fclScriptForcedEnd(*(u8 **)pPID);
    }
    handle = EE_ADDR_S(scrStartScript2(0x7D0, EE_ADDR_AS(void *, pScriptData), StartNo));
    fclClearFlag(0);
    if (pPID != NULL)
    {
        *(s32 *)pPID = handle;
    }
    return handle;
}

INCLUDE_ASM(const s32, "facility/fclMisc", func_00123D48);

INCLUDE_ASM(const s32, "facility/fclMisc", func_00123D70);

void shuffleIndices(s8 *order, s32 count, s32 rounds)
{
    s32 n;

    if (rounds <= 0) {
        return;
    }
    n = rounds;
    while (n != 0) {
        s32 first = (u32)dds3GetRandA(0) % (u32)count;
        s32 second = (u32)dds3GetRandA(0) % (u32)count;
        u8 value = order[first];

        order[first] = order[second];
        order[second] = value;
        n--;
    }
}

INCLUDE_ASM(const s32, "facility/fclMisc", evtLoadResourcePair);

INCLUDE_ASM(const s32, "facility/fclMisc", evtReleaseResourcePairHandle);

s8 fclInitMessage(void *pData)
{
    s32 ret;

    if (gMsgID < 0) {
        gMsgID = itfMesMngInitialize(pData);
        itfMesMngChangeWindowType(gMsgID, 2, 0);
        ret = 1;
    } else {
        ret = 0;
    }
    return ret;
}

INCLUDE_ASM(const s32, "facility/fclMisc", evtSetMessageWindowPageValue);

s32 fclStartMessage(s32 MsgNo)
{
    if (gMsgID < 0)
    {
        return 0;
    }
    itfMesMngMarkBitStat(gMsgID, 0x200000);
    itfMesMngRequestMessage(gMsgID, MsgNo, 0);
    itfMesMngSetSelPhase(gMsgID, -1);
    gMsgFlag = 1;
    return 1;
}

INCLUDE_ASM(const s32, "facility/fclMisc", func_00123F80);

INCLUDE_ASM(const s32, "facility/fclMisc", func_00123FB0);

INCLUDE_ASM(const s32, "facility/fclMisc", func_00123FC8);

INCLUDE_ASM(const s32, "facility/fclMisc", dspReadWindowSecondPanelValue);

INCLUDE_ASM(const s32, "facility/fclMisc", evtGetCapturedWindowPanelValue);

INCLUDE_ASM(const s32, "facility/fclMisc", func_00124010);

INCLUDE_ASM(const s32, "facility/fclMisc", evtFinishMessageWindowAndNotify);

s32 fclDestroyMessage(void)
{
    if (gMsgID < 0) {
        return 0;
    }
    itfMesMngRelease(gMsgID);
    gMsgID = -1;
    gMsgFlag = 0;
    gMsgInvFlag = 0;
    return 1;
}

s32 fclChkMessage(void)
{
    if (gMsgID < 0) {
        return 0;
    }
    if (gMsgInvFlag != 0 && gMsgFlag == 2) {
        return 0;
    }
    return (s8)(u8)gMsgFlag;
}

INCLUDE_ASM(const s32, "facility/fclMisc", func_00124130);

INCLUDE_ASM(const s32, "facility/fclMisc", func_00124178);

INCLUDE_ASM(const s32, "facility/fclMisc", dspRequestMessageWindowModeOne);

INCLUDE_ASM(const s32, "facility/fclMisc", evtCopyEntryStringToActiveWindow);

INCLUDE_ASM(const s32, "facility/fclMisc", func_00124298);

INCLUDE_ASM(const s32, "facility/fclMisc", func_001242A0);

void fcl_2142F0(s32 x, s32 y)
{
    itfMesMngSetMesPosition(gMsgID, x * 16, y * 8);
    itfMesMngSetOffsetY1(gMsgID, -((0x15F - y) * 8));
}

INCLUDE_ASM(const s32, "facility/fclMisc", func_00124358);

INCLUDE_ASM(const s32, "facility/fclMisc", func_00124380);

INCLUDE_ASM(const s32, "facility/fclMisc", func_00124390);

INCLUDE_ASM(const s32, "facility/fclMisc", func_001243A0);

INCLUDE_ASM(const s32, "facility/fclMisc", func_001243C8);

INCLUDE_ASM(const s32, "facility/fclMisc", func_001243F0);

s32 fcl_214458(void)
{
    s32 index = scrGetIntPara(0);

    if (index >= 0x10) {
        index = 0xF;
    }
    scrSetIntReturnValue(fclWorkValue[index]);
    return 1;
}
s32 gMsgID = -1;

s8 gMsgFlag = 0;

s8 gMsgInvFlag = 0;

INCLUDE_SDATA(const s32, "facility/fclMisc", D_0046B5C0);

INCLUDE_SDATA(const s32, "facility/fclMisc", D_0046B5C4);

INCLUDE_SDATA(const s32, "facility/fclMisc", D_0046B5C5);

