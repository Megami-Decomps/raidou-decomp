#ifndef ITF_H
#define ITF_H

#include "common.h"
#include "enums.h"
#include "kernel.h"

struct MenuCB_tag;
struct Menu_t;
struct elvMenu_t;
struct itfMover_t;
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
s32 itfCountProcessCheck(dds3ProcessID_t *PID);
void itfCountProcessEnd(dds3ProcessID_t *PID);
dds3ProcessID_t *itfCountProcessStart(s32 a_num, s32 a_frame, s32 a_blank, s32 a_seNo);
dds3ProcessID_t *itfCountProcessStartMoney(s32 a_num);
s32 itfCountSetData(dds3ProcessID_t *PID, s32 a_pDat, ITF_COUNT_TYPE a_type);
s32 itfCountSetMax(dds3ProcessID_t *PID, s32 a_pDat, s32 a_max);
s32 itfCountSetMin(dds3ProcessID_t *PID, s32 a_pDat, s32 a_min);
void itfDrawMiscSpriteList(s32 a_x, s32 a_y, s32 a_z, u32 a_alphaRate, struct sprHandle_t *a_hSpr, s32 a_sprNo, s16 *a_list);
s32 itfDvlMsgCreateData(s32 a_devilID);
s32 itfDvlMsgDeleteData(s32 a_devilID);
void itfDvlMsgEnd(void);
void itfDvlMsgProcessStart(void);
void itfDvlMsgStart(s32 a_devilID, s32 a_msgID);
dds3ProcessID_t *itfGeneralMenuCreatePID(s32 a_x, s32 a_y);
void itfIconDrawSprite(s32 a_x, s32 a_y, s32 a_z, u32 a_alphaRate, s32 a_sprNo, s16 *a_list);
void itfListSetTypeApper(dds3ProcessID_t *PID, s32 a_mode, s32 a_type);
s32 itfMenuClearMenu(dds3ProcessID_t *PID, s32 a_id);
void itfMenuClearMenuAll(dds3ProcessID_t *PID);
ITFMENU_CTRL_TYPE itfMenuGetCtrlType(dds3ProcessID_t *PID);
s32 itfMenuGetFlag(dds3ProcessID_t *PID, u32 a_bit);
s32 itfMenuGetSelectID(dds3ProcessID_t *PID);
ITFMENU_WAIT_TYPE itfMenuGetWaitType(dds3ProcessID_t *PID);
void itfMenuImgGetRatio(dds3ProcessID_t *PID, u32 a_abgr);
s32 itfMenuLinkSprite(dds3ProcessID_t *PID, struct sprHandle_t *a_hSpr);
s32 itfMenuProcessCheck(dds3ProcessID_t *PID);
void itfMenuProcessEnd(dds3ProcessID_t *PID);
dds3ProcessID_t *itfMenuProcessStart(void);
void itfMenuSetDrawParam(dds3ProcessID_t *PID, s32 a_max, s32 a_bFix);
void itfMenuSetFlag(dds3ProcessID_t *PID, ITFMENU_GF a_bit, s32 a_bSet);
s32 itfMenuSetFlagMenu(dds3ProcessID_t *PID, s32 a_id, ITFMENU_CB_GF a_bit, s32 a_bSet);
s32 itfMenuSetImageColor(dds3ProcessID_t *PID, ITFMENU_IMG_DAT a_type, u8 a_R, u8 a_G, u8 a_B, u8 a_A);
void itfMenuStart(dds3ProcessID_t *PID);
void itfMoneyProcessStart(void);
s32 itfMoverGetPosition(struct itfMover_t *mv, f32 *a_pPosX, f32 *a_pPosY);
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
