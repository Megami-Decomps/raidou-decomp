#ifndef ITF_H
#define ITF_H

#include "common.h"
#include "enums.h"
#include "kernel.h"

struct MenuCB_tag;
struct Menu_t;
struct elvMenu_t;
typedef struct itfMoverCB_t {
    f32 posx;
    f32 posy;
    f32 tgtx;
    f32 tgty;
    f32 movx;
    f32 movy;
    f32 rate;
    s8 wait[2];
    s16 frame;
} itfMoverCB_t;

typedef struct itfMover_t {
    itfMoverCB_t cb[4];
    s16 cb_cnt;
    s16 cb_num;
    s16 cnt;
    u16 flag;
    s32 total_cnt;
    s32 total_frm;
} itfMover_t;
SIZE_CHECK(itfMoverSizeCheck, itfMover_t, 0x90);
struct sprHandle_t;
struct tagITFDVLMSG_CB;
struct tagITFDVLMSG_WORK;
struct tagITF_MONEY_WORK {
    u8 pad00[12];
    u8 mode;
    u8 pad0D[2];
    u8 flg;
    s32 frame;
    s32 money;
};

void CountShutdown(dds3ProcessID_t *PID);
s32 DvlMsgCreateCB(struct tagITFDVLMSG_WORK *wk);
void DvlMsgDeleteCB(struct tagITFDVLMSG_WORK *wk, struct tagITFDVLMSG_CB *cb);
dds3ProcessFunc_t DvlMsgDrawProcess(dds3ProcessID_t *PID);
void DvlMsgFreeMsg(struct tagITFDVLMSG_CB *cb);
void DvlMsgFreeTex(struct tagITFDVLMSG_CB *cb);
struct tagITFDVLMSG_CB *DvlMsgGetCB(struct tagITFDVLMSG_WORK *wk, s32 a_devilID);
s32 DvlMsgLoadFile(struct tagITFDVLMSG_WORK *wk, struct tagITFDVLMSG_CB *cb, s32 a_devilID);
void DvlMsgLoadSprite(struct tagITFDVLMSG_WORK *wk);
dds3ProcessFunc_t DvlMsgProcess(dds3ProcessID_t *PID);
void DvlMsgRelease(struct tagITFDVLMSG_WORK *wk);
void DvlMsgSetMover(struct tagITFDVLMSG_WORK *wk, s32 a_mode);
void DvlMsgShutdown(dds3ProcessID_t *PID);
s32 MenuClearMenuSub(struct Menu_t *wk, struct MenuCB_tag *cb);
void MenuCreateMenu(struct Menu_t *wk);
s32 MenuDelMenuSub(struct Menu_t *wk, s32 a_id);
void MenuDeleteMenu(struct Menu_t *wk, struct MenuCB_tag *cb);
void MenuGetBlankMenu(dds3ProcessID_t *PID);
struct MenuCB_tag *MenuGetMenu(struct Menu_t *wk, s32 a_hierarchy, s32 a_no);
void MenuInit_itf(struct Menu_t *wk);
s32 MenuRemakeLinkSub(struct Menu_t *wk);
u8 MoneyCheckProcEnd(struct tagITF_MONEY_WORK *wk);
void itfDrawFont(ITF_DM_STYLE a_style, s32 a_x, s32 a_y, s32 a_z, s32 a_w, s32 a_space, u32 a_color, f32 a_alphaRate, DDS3_OT a_ot, char *a_str);
void MoneyDrawPanel(s32 a_offX, s32 a_offY, u8 a_alpha);
s32 MoneyDrawProcess(dds3ProcessID_t *PID);
extern char MoneyDrawProcessDataEntry[];
extern char *MoneyDrawProcessData[];
dds3ProcessFunc_t MoneyProcess(dds3ProcessID_t *PID);
void MoneyShutdown(dds3ProcessID_t *PID);
s32 _DvlMsgDeleteData(struct tagITFDVLMSG_WORK *wk, struct tagITFDVLMSG_CB *cb);
void awdMenuProcessEnd(void);
void awdMenuStart(void);
void elvMenuProcessStart(void);
void itfCalcSpriteWidthSub(f32 alpha, char *money);
void itfCalcSpriteWidth(struct sprHandle_t *a_hSpr, s32 a_sprNo, s32 a_num, s32 a_max);
void itfIconDrawBullet(s32 a_x, s32 a_y, s32 a_z, u32 a_alphaRate, s32 a_itemID, DDS3_OT a_otNo);
s32 itfCountProcessCheck(dds3ProcessID_t *PID);
void itfCountProcessEnd(dds3ProcessID_t *PID);
dds3ProcessID_t *itfCountProcessStart(s32 a_num, s32 a_frame, s32 a_blank, s32 a_seNo);
dds3ProcessID_t *itfCountProcessStartMoney(s32 a_num);
s32 itfCountSetData(dds3ProcessID_t *PID, s32 a_pDat, ITF_COUNT_TYPE a_type);
s32 itfCountSetMax(dds3ProcessID_t *PID, s32 a_pDat, s32 a_max);
s32 itfCountSetMin(dds3ProcessID_t *PID, s32 a_pDat, s32 a_min);
void itfCallSetOt2(s32 a_x, s32 a_y, s32 a_z, u32 a_alphaRate, struct sprHandle_t *a_hSpr, s32 a_sprNo, DDS3_OT a_otNo);
void itfDrawMiscSpriteList(s32 a_x, s32 a_y, s32 a_z, u32 a_alphaRate, struct sprHandle_t *a_hSpr, s32 a_sprNo, s16 *a_list);
s32 itfDvlMsgCreateData(s32 a_devilID);
s32 itfDvlMsgDeleteData(s32 a_devilID);
void itfDvlMsgEnd(void);
void itfDvlMsgProcessStart(void);
void itfDvlMsgStart(s32 a_devilID, s32 a_msgID);
dds3ProcessID_t *itfGeneralMenuCreatePID(s32 a_x, s32 a_y);
void itfIconDrawSprite(s32 a_x, s32 a_y, s32 a_z, u32 a_alphaRate, s32 a_sprNo, s16 *a_list);
void itfIconDrawBadStatus(s32 a_x, s32 a_y, s32 a_z, u32 a_alphaRate, u32 a_bstNo, s32 a_otNo);

typedef s32 (*itfMenuFunc_t)(void);

/* Sprite list entry; a list ends at sprNo -1. */
typedef struct itfSprList_t {
    s16 sprNo;
    s16 ofsx;
    s16 ofsy;
    u16 flag;
} itfSprList_t;

void itfListSetTypeApper(dds3ProcessID_t *PID, s32 a_mode, s32 a_type);
s32 itfMenuClearMenu(dds3ProcessID_t *PID, s32 a_id);
void itfMenuClearMenuAll(dds3ProcessID_t *PID);
ITFMENU_CTRL_TYPE itfMenuGetCtrlType(dds3ProcessID_t *PID);
s32 itfMenuGetFlag(dds3ProcessID_t *PID, u32 a_bit);
s32 itfMenuGetSelectID(dds3ProcessID_t *PID);
u8 GetSelectRootID(dds3ProcessID_t *PID, u32 a_no);
ITFMENU_WAIT_TYPE itfMenuGetWaitType(dds3ProcessID_t *PID);
void itfMenuImgGetRatio(dds3ProcessID_t *PID, u32 a_abgr);
s32 itfMenuLinkSprite(dds3ProcessID_t *PID, struct sprHandle_t *a_hSpr);
s32 itfMenuProcessCheck(dds3ProcessID_t *PID);
void itfMenuProcessEnd(dds3ProcessID_t *PID);
dds3ProcessID_t *itfMenuProcessStart(void);
void cmpSklTypeSet(dds3ProcessID_t *PID);
s32 itfMenuSetRootMenu(dds3ProcessID_t *PID, s32 a_id, char *a_str, itfMenuFunc_t a_func);
s32 itfMenuSetMenu(dds3ProcessID_t *PID, s32 a_id, s32 a_parentId, char *a_str, itfMenuFunc_t a_func);
void itfGeneralMenuCreatePIDSub(dds3ProcessID_t *PID, s32 a_type);
void itfGeneralMenuCreatePIDSub2(dds3ProcessID_t *PID, s32 a_type);
void itfMenuSetOt(dds3ProcessID_t *PID);
s32 itfGetFigure(s32 a_num);
s32 itfGetFigureNum(s32 a_num, s32 a_figure);
s32 itfCallGetProcessData7(dds3ProcessID_t *PID, u32 a_no, s8 a_val);
void itfMenuSetDrawParam(dds3ProcessID_t *PID, s32 a_max, s32 a_bFix);
void itfMenuSetFlag(dds3ProcessID_t *PID, ITFMENU_GF a_bit, s32 a_bSet);
s32 itfMenuSetFlagMenu(dds3ProcessID_t *PID, s32 a_id, ITFMENU_CB_GF a_bit, s32 a_bSet);
s32 itfMenuSetSpriteData(dds3ProcessID_t *PID, ITFMENU_IMG_DAT a_type, s32 a_baseX, s32 a_baseY, s32 a_ofsX,
                         s32 a_ofsY, s32 a_fontX, s32 a_fontY, u8 *a_strSprName, itfSprList_t *a_pList);
s32 itfMenuSetImageColor(dds3ProcessID_t *PID, ITFMENU_IMG_DAT a_type, u8 a_R, u8 a_G, u8 a_B, u8 a_A);
void itfMenuStart(dds3ProcessID_t *PID);
void itfCallGetProcessData6(dds3ProcessID_t *PID);
void itfMoneyProcessStart(void);
s32 itfMoverGetPosition(struct itfMover_t *mv, f32 *a_pPosX, f32 *a_pPosY);
s32 itfMoverInit(struct itfMover_t *mv);
f32 itfMoverGetRatio(struct itfMover_t *mv);
s32 itfMoverSetParam(struct itfMover_t *mv, s32 a_posx, s32 a_posy, s32 a_tgtx, s32 a_tgty, s32 a_frame);
ITF_MOVER_UPDATE itfMoverUpdate(struct itfMover_t *mv);
s32 itfSetMesDispPriority(void);
dds3ProcessID_t *mapMenuCreateMenuPID(void);
s32 mapMenuDrawMagWindow(s32 a_initID);
s32 mapMenuGetSelectUNIQ(void);
void mapMenuProcessStart(void);
void mapMenuRelease(struct elvMenu_t *wk);
void mapMenuShutdown(dds3ProcessID_t *PID);
dds3ProcessFunc_t rstCallGetProcessData35(dds3ProcessID_t *PID);

#endif
