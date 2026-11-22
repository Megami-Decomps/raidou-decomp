#ifndef ITF_H
#define ITF_H

#include "common.h"
#include "enums.h"
#include "kernel.h"

struct MenuCB_tag;
struct Menu_t;
/* Map menu work, 20 bytes from sdfMemAllocDirect; hMem is its own allocation.
 * The rest is HD's tagMAP_MENU_WORK up to selectID. */
typedef struct tagMAP_MENU_WORK {
    struct sdfMemHandle_t *hMem; /* 0x00 */
    struct sprHandle_t *hSpr;    /* 0x04 */
    dds3ProcessID_t *menuPID;    /* 0x08 */
    u8 step;                     /* 0x0C */
    u8 flag;                     /* 0x0D */
    s16 count;                   /* 0x0E */
    s32 selectID;                /* 0x10: -1 at start */
#ifdef VERSION_RAIDOU2
    s32 unk14;                   /* 0x14: 0x10000 at start */
    s32 unk18;                   /* 0x18 */
#endif
} tagMAP_MENU_WORK;

/* HD's awdMenu_t and elvMenu_t, with the sdfMemAllocDirect handle first as in tagMAP_MENU_WORK. */
typedef struct awdMenu_t {
    struct sdfMemHandle_t *hMem; /* 0x00 */
    struct sprHandle_t *hSpr;    /* 0x04 */
    dds3ProcessID_t *listPID;    /* 0x08 */
    u8 flag;                     /* 0x0C */
    u8 step;                     /* 0x0D */
    s16 count;                   /* 0x0E */
    s32 selectID;                /* 0x10 */
    s32 selectMsg;               /* 0x14 */
} awdMenu_t;

typedef struct elvMenu_t {
    struct sdfMemHandle_t *hMem; /* 0x00 */
    struct sprHandle_t *hSpr;    /* 0x04 */
    dds3ProcessID_t *listPID;    /* 0x08 */
    u8 flag;                     /* 0x0C */
    u8 step;                     /* 0x0D */
    s16 count;                   /* 0x0E */
    s32 selectID;                /* 0x10 */
    s32 initID;                  /* 0x14 */
    s32 top;                     /* 0x18 */
} elvMenu_t;
/* Bust-up morph work (HD mphWork_t); the 0x80.. fields line up with HD's alpha
 * through fileStat. mphSetAlpha/Ot/Z write them. */
#define MPH_DEFAULT_OT 103

typedef struct mphWork_t {
    u32 unk00;
    struct sdfMemHandle_t *hMem; /* 0x04 */
    u8 unk08[0x78];
    u8 alpha; /* 0x80 */
    u8 pad81[3];
    s32 ot; /* 0x84 */
    s32 z;  /* 0x88 */
    u8 unk8C[0x20];
    s32 fileStat; /* 0xAC */
    u8 unkB0[0xB8];
} mphWork_t;
#ifdef VERSION_RAIDOU1
SIZE_CHECK(mphWorkSizeCheck, mphWork_t, 0x168);
#endif
extern mphWork_t *mphWork;
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

/* Sprite list entry; a list ends at sprNo -1. */
typedef struct itfSprList_t {
    s16 sprNo;
    s16 ofsx;
    s16 ofsy;
    u16 flag;
} itfSprList_t;

typedef s32 (*itfMenuFunc_t)(void);
/* A row's draw function: its id, its panel's position, then z, color and alpha. */
typedef void (*itfMenuDrawFunc_t)(s32 a_id, s32 a_x, s32 a_y, s32 a_z, u32 a_color, u8 a_alpha);

typedef struct MenuImgDat_t {
    struct sprHandle_t *hSpr;
    itfSprList_t *pList;
    s16 sx;
    s16 sy;
    s16 sw;
    s16 sh;
    s16 fx;
    s16 fy;
    u32 color;
    u8 alpha;
    s8 strStyle;
    s8 strSpace;
    s8 dummy8;
    s16 strWidth;
    u16 flag;
#ifdef VERSION_RAIDOU2
    s16 field20[4][2]; /* cleared by MenuInit_itf */
#endif
} MenuImgDat_t;
#ifdef VERSION_RAIDOU2
SIZE_CHECK(MenuImgDatSizeCheck, MenuImgDat_t, 0x30);
#else
SIZE_CHECK(MenuImgDatSizeCheck, MenuImgDat_t, 0x20);
#endif

typedef struct MenuCB_tag {
    s32 id;
    s32 parentId;
    s8 hierarchy;
    u8 alphaRate;
    s16 no;
#ifdef VERSION_RAIDOU2
    u32 gflag;
    u32 lflag; /* bit 0: slot is blank */
#else
    u16 gflag;
    u16 lflag; /* bit 0: slot is blank */
#endif
    s32 decideSE;
    s32 cancelSE;
    char *str;
    struct MenuCB_tag *pParent;
    struct MenuCB_tag *pPrev;
    struct MenuCB_tag *pNext;
    itfMenuFunc_t func;
    void *funcDraw;
} MenuCB_tag;
#ifdef VERSION_RAIDOU2
SIZE_CHECK(MenuCBSizeCheck, MenuCB_tag, 0x34);
#else
SIZE_CHECK(MenuCBSizeCheck, MenuCB_tag, 0x30);
#endif

typedef struct MenuMover_t {
    itfMover_t mover;
    MenuCB_tag *cb;
    s32 x;
    s32 y;
    s32 count;
} MenuMover_t;
SIZE_CHECK(MenuMoverSizeCheck, MenuMover_t, 0xA0);

typedef struct Menu_t {
    struct sdfMemHandle_t *hMem;
    dds3ProcessID_t *drawPID;
    MenuImgDat_t imgDat[4];
    MenuCB_tag *pHead;
    MenuCB_tag *pTail;
    MenuCB_tag *select;
    MenuCB_tag title;
    MenuMover_t mv_title;
    MenuMover_t mv[16];
    s8 step;
    s8 nowHierarchy;
    u8 nMenuSet;
    u8 ctrlType;
    s8 dispType[2];
    s8 updateType;
    s8 scrollType;
    DDS3_OT ot;
    u8 selMax;
    u8 drwMax;
    u8 drwDefNum;
    u8 drwDefMax;
    s8 selNo[4];
    s8 topNo[4];
    s8 drwNo[4];
    s32 delId;                  /* 0xB80: menu id MenuDelMenuSub removes */
    s32 initSelectID;           /* 0xB84 */
    u8 moveType;                /* 0xB88 */
    u8 retFunc;                 /* 0xB89: last select->func result */
    u8 bEnableCtrl;             /* 0xB8A */
    s8 dummy[1];
#ifdef VERSION_RAIDOU2
    u32 gflag;
    u32 lflag;
#else
    u16 gflag;
    u16 lflag;
#endif
    s32 frame;
    s32 count;
    s32 pri;                    /* 0xB98: set by evtPictureSetPri */
} Menu_t;
#ifdef VERSION_RAIDOU2
SIZE_CHECK(MenuSizeCheck, Menu_t, 0xBE4);
#else
SIZE_CHECK(MenuSizeCheck, Menu_t, 0xB9C);
#endif
/* One loaded devil message, 0x1C bytes (sdfChipAllocZeroDirect in DvlMsgCreateCB). */
struct tagITFDVLMSG_CB {
    u8 unk00[0x10];
    struct tagITFDVLMSG_CB *pPrev; /* 0x10 */
    struct tagITFDVLMSG_CB *pNext; /* 0x14 */
    s32 devilID;                   /* 0x18 */
};
SIZE_CHECK(tagITFDVLMSG_CBSizeCheck, struct tagITFDVLMSG_CB, 0x1C);

/* Process data of itfDvlMsgMainProc, 0x1CC bytes. */
struct tagITFDVLMSG_WORK {
    struct sdfMemHandle_t *hMem;   /* 0x00 */
    dds3ProcessID_t *drawPID;      /* 0x04 */
    u8 unk08[0xC];
    struct tagITFDVLMSG_CB *pHead; /* 0x14 */
    struct tagITFDVLMSG_CB *pTail; /* 0x18 */
    s32 datNum;                    /* 0x1C */
    u8 unk20[0x1A4];
    u16 flag;                      /* 0x1C4: itfDvlMsgEnd sets bit 1 */
    u8 unk1C6[6];
};
SIZE_CHECK(tagITFDVLMSG_WORKSizeCheck, struct tagITFDVLMSG_WORK, 0x1CC);

extern dds3ProcessID_t *DevilMessage_g_mainPID;
s32 itfCallGetProcessData9(dds3ProcessID_t *PID);
void DvlMsgInit(struct tagITFDVLMSG_WORK *wk);
struct tagITF_MONEY_WORK {
    struct sdfMemHandle_t *hMem;
    dds3ProcessID_t *drawPID;
    dds3ProcessID_t *countPID;
    u8 step;
    u8 type;
    u8 pad0E;
    u8 flags;
    s32 count;
    s32 money;
    s32 calc;
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
void cmpCallMenuDeleteMenu(struct Menu_t *wk);
#ifdef VERSION_RAIDOU2
/* raidou1 names this function cmpCallMenuDeleteMenu. */
void MenuRelease(struct Menu_t *wk);
#endif
MenuCB_tag *MenuGetBlankMenu(Menu_t *wk);
struct MenuCB_tag *MenuGetMenu(struct Menu_t *wk, s32 a_hierarchy, s32 a_no);
void MenuInit_itf(struct Menu_t *wk);
void MenuUpdatePrevLink(Menu_t *wk);
u8 MenuCalcAlpha(MenuMover_t *mv, u32 lflag);
void MenuDrawTitle(Menu_t *wk, s32 a_x, s32 a_y, u8 a_alpha, char *a_str);
void MenuDrawPanel(Menu_t *wk, MenuMover_t *mv, u8 a_alpha, s8 a_attr);
void cmpItfmenuScrollbarTexture(Menu_t *wk, s32 a_topno, s32 a_selmax, u8 a_alphaRate);
void MenuUpdateLinkMover(Menu_t *wk);
s32 MenuUpdateSelectMenu(Menu_t *wk);
s32 MenuRemakeLinkSub(struct Menu_t *wk);
void MenuCtrlSelect(Menu_t *wk, s32 a_keytype, s32 a_bPlaySE);
void itfCallMoverInit2(Menu_t *wk, s32 a_type);
void MenuSetMoverDisappear(Menu_t *wk, s32 a_type);
s32 MenuUpdateMover(Menu_t *wk);
void MenuMoveEnd(Menu_t *wk);
void MenuPlayDecideSE(MenuCB_tag *cb);
void MenuPlayCancelSE(MenuCB_tag *cb);
s32 itfCallMenuSetMoverDisappear(Menu_t *wk, s32 a_type);
void itfCallMenuGetSelectMenu(Menu_t *wk);
void itfCallMenuGetSelectMenu2(Menu_t *wk);
void MenuPadCtrlSelect(Menu_t *wk);
void itfCallGetProcessData5(dds3ProcessID_t *PID, s32 a_id);
u8 MoneyCheckProcEnd(struct tagITF_MONEY_WORK *wk);
void itfDrawFont(ITF_DM_STYLE a_style, s32 a_x, s32 a_y, s32 a_z, s32 a_w, s32 a_space, u32 a_color, f32 a_alphaRate, DDS3_OT a_ot, char *a_str);
void itfDrawMiscFont(ITF_DM_STYLE a_style, s32 a_x, s32 a_y, s32 a_z, s32 a_space, u32 a_color, u8 a_alpha, char *a_str);
void MoneyDrawPanel(s32 a_offX, s32 a_offY, u8 a_alpha);
s32 MoneyDrawProcess(dds3ProcessID_t *PID);
extern char MoneyDrawProcessDataEntry[];
extern char *MoneyDrawProcessData[];
extern char MoneyDrawProcessData2[]; /* "%7d" */
s32 MoneyProcess(dds3ProcessID_t *PID);
void itfMoneyProcessSub(struct tagITF_MONEY_WORK *wk);
void MoneyShutdown(dds3ProcessID_t *PID);
s32 _DvlMsgDeleteData(struct tagITFDVLMSG_WORK *wk, struct tagITFDVLMSG_CB *cb);
void awdMenuProcessEnd(void);
void awdMenuStart(void);
void elvMenuProcessStart(void);
s32 itfCalcSpriteWidthSub(f32 a_value, s32 a_shift);
void itfCalcSpriteWidth(struct sprHandle_t *a_hSpr, s32 a_sprNo, s32 a_num, s32 a_max);
void itfGetRatePosBezierPt3(f32 *px, f32 *py, f32 x0, f32 y0, f32 x1, f32 y1, f32 vx, f32 vy, f32 rate);
void itfGetVectorPosBezierPt3(f32 *px, f32 *py, f32 x0, f32 y0, f32 x1, f32 y1, f32 rx, f32 ry, f32 rate);
u32 itfGetDivBezierPt3(f32 x0, f32 y0, f32 x1, f32 y1, f32 vx, f32 vy, f32 divLength);
void itfDrawFanBezierPt3(f32 x0, f32 y0, f32 x1, f32 y1, f32 x2, f32 y2, f32 vx, f32 vy, u32 rgba0, u32 rgba1, u32 rgba2, u32 ot);
void itfIconDrawBullet(s32 a_x, s32 a_y, s32 a_z, u32 a_alphaRate, s32 a_itemID, DDS3_OT a_otNo);
s32 itfCountProcessCheck(dds3ProcessID_t *PID);
void itfCountProcessEnd(dds3ProcessID_t *PID);
dds3ProcessID_t *itfCountProcessStart(s32 a_num, s32 a_frame, s32 a_blank, s32 a_seNo);
dds3ProcessID_t *itfCountProcessStartMoney(s32 a_num);
s32 itfCountSetData(dds3ProcessID_t *PID, s32 *a_pDat, ITF_COUNT_TYPE a_type);
s32 itfCountSetMax(dds3ProcessID_t *PID, s32 *a_pDat, s32 a_max);
s32 itfCountSetMin(dds3ProcessID_t *PID, s32 *a_pDat, s32 a_min);
void itfCallSetOt6(DDS3_OT a_otNo);
void itfCallLinkAlpha(s32 a_x, s32 a_y, s32 a_z, u32 a_alphaRate, struct sprHandle_t *a_hSpr, s32 a_sprNo, DDS3_OT a_otNo);
void itfDrawMiscSprite(s32 a_x, s32 a_y, s32 a_z, struct sprHandle_t *a_hSpr, s32 a_sprNo, DDS3_OT a_otNo);
void itfDrawMiscSpriteAlpha(s32 a_x, s32 a_y, s32 a_z, u32 a_alphaRate, struct sprHandle_t *a_hSpr, s32 a_sprNo, DDS3_OT a_otNo);
void itfDrawMiscSpriteList(s32 a_x, s32 a_y, s32 a_z, u32 a_alphaRate, struct sprHandle_t *a_hSpr, DDS3_OT a_otNo, itfSprList_t *a_list);
void itfDrawMiscFont(ITF_DM_STYLE a_style, s32 a_x, s32 a_y, s32 a_z, s32 a_space, u32 a_color, u8 a_alpha, char *a_str);
void itfDrawMiscSpriteListEx(s32 a_x, s32 a_y, s32 a_z, u32 a_alphaRate, struct sprHandle_t *a_hSpr, DDS3_OT a_otNo, u32 a_flag, itfSprList_t *a_list);
void itfDrawMiscSpriteListSelect(s32 a_x, s32 a_y, s32 a_z, u32 a_alphaRate, struct sprHandle_t *a_hSpr, DDS3_OT a_otNo, itfSprList_t *a_list, s32 a_no);
s32 itfDvlMsgCreateData(s32 a_devilID);
s32 itfDvlMsgDeleteData(s32 a_devilID);
void itfDvlMsgEnd(void);
void itfDvlMsgProcessStart(void);
void itfDvlMsgStart(s32 a_devilID, s32 a_msgID);
dds3ProcessID_t *itfGeneralMenuCreatePID(s32 a_x, s32 a_y);
void itfIconDrawSprite(s32 a_x, s32 a_y, s32 a_z, u32 a_alphaRate, s32 a_sprNo, DDS3_OT a_otNo);
s32 itfLoadIconSprite(void);
s32 itfCallLoadIconSprite(void);
s32 itfCallLoadIconSprite2(void);
s32 itfCallMemFreeDelayedDirect(void);
s32 itfFunc4(void);
extern struct sdfMemHandle_t *itfData5;
extern struct sprHandle_t *generalMenu_g_hSpr;
extern struct sprHandle_t *itfData6;
extern DDS3_OT g_ot;
extern f32 itfMenuDrawProcess_fx;
extern f32 itfMenuDrawProcess_fy;
void itfIconDrawBadStatus(s32 a_x, s32 a_y, s32 a_z, u32 a_alphaRate, u32 a_bstNo, DDS3_OT a_otNo);



void itfListSetTypeApper(dds3ProcessID_t *PID, s32 a_mode, s32 a_type);
s32 itfMenuClearMenu(dds3ProcessID_t *PID, s32 a_id);
void itfMenuClearMenuAll(dds3ProcessID_t *PID);
s32 itfMenuGetMenuNum(dds3ProcessID_t *PID);
s32 itfMenuSetFunc(dds3ProcessID_t *PID, s32 a_id, itfMenuFunc_t a_func);
ITFMENU_CTRL_TYPE itfMenuGetCtrlType(dds3ProcessID_t *PID);
s32 itfMenuGetFlag(dds3ProcessID_t *PID, u32 a_bit);
s32 itfMenuGetSelectID(dds3ProcessID_t *PID);
s32 itfMenuGetFlashCount(dds3ProcessID_t *PID, s32 a_id);
u8 GetSelectRootID(dds3ProcessID_t *PID, u32 a_no);
s32 itfMenuChkMenuTop(dds3ProcessID_t *PID, s32 a_id);
s32 itfMenuGetFlagMenu(dds3ProcessID_t *PID, s32 a_id, u16 a_bit);
#ifdef VERSION_RAIDOU2
/* Draws help text a_helpID at (a_x * 16, a_y * 8) faded by a_alphaRate (0.0 to 1.0). */
void itfDrawMiscHelp(s32 a_x, s32 a_y, s32 a_type, s32 a_space, u32 a_color, s32 a_otNo, s8 a_mode, u16 a_helpID, f32 a_alphaRate, s32 a_arg);
#endif
ITFMENU_WAIT_TYPE itfMenuGetWaitType(dds3ProcessID_t *PID);
s32 itfMenuGetWaitType2(dds3ProcessID_t *PID);
void itfMenuSetSelectID(dds3ProcessID_t *PID, s32 a_id, s32 a_select);
f32 itfMenuImgGetRatio(dds3ProcessID_t *PID, u32 a_abgr);
s32 itfMenuLinkSprite(dds3ProcessID_t *PID, struct sprHandle_t *a_hSpr);
s32 itfMenuProcessCheck(dds3ProcessID_t *PID);
void itfMenuProcessEnd(dds3ProcessID_t *PID);
dds3ProcessID_t *itfMenuProcessStart(void);
s32 MenuProcess(dds3ProcessID_t *PID);
s32 MenuShutdown(dds3ProcessID_t *PID);
s32 MenuDrawProcess(dds3ProcessID_t *PID);
void cmpSklTypeSet(dds3ProcessID_t *PID);
s32 itfMenuSetRootMenu(dds3ProcessID_t *PID, s32 a_id, char *a_str, itfMenuFunc_t a_func);
s8 itfMenuSetMenu(dds3ProcessID_t *PID, s32 a_id, s32 a_parentId, char *a_str, itfMenuFunc_t a_func);
void itfMenuSetMenuTitle(dds3ProcessID_t *PID, char *a_str);
s32 itfMenuSetFont(dds3ProcessID_t *PID, s32 a_id, char *a_str);
void itfMenuSetDrawFunc(dds3ProcessID_t *PID, s32 a_id, itfMenuDrawFunc_t a_func);
void itfMenuSetCancelSE(dds3ProcessID_t *PID, s32 a_id, s32 a_se);
void itfCallGetProcessData(dds3ProcessID_t *PID, s32 a_id);
void itfGeneralMenuCreatePIDSub(dds3ProcessID_t *PID, s32 a_type);
void itfGeneralMenuCreatePIDSub2(dds3ProcessID_t *PID, s32 a_type);
void itfMenuSetOt(dds3ProcessID_t *PID);
s32 itfGetFigure(s32 a_num);
s32 itfGetFigureNum(s32 a_num, s32 a_figure);
s32 itfCallGetProcessData7(dds3ProcessID_t *PID, u32 a_no, s32 a_val);
#ifdef VERSION_RAIDOU2
s32 cmpCallMenuProcessCheck(dds3ProcessID_t *PID, u32 a_no, u32 a_idx, s32 a_x, s32 a_y);
struct frFontHandle_t *itfDrawBankTextWithLayoutFlags(s32 a_x, s32 a_y, s32 a_z, u16 a_num, struct frTextBank_t *a_bank, s32 a_flag);
#endif
void itfMenuSetDrawParam(dds3ProcessID_t *PID, s32 a_max, s32 a_bFix);
#ifdef VERSION_RAIDOU2
f32 itfCriLipSyncManagerOff(s32 major, s32 minor);
void itfMenuSetFlag(dds3ProcessID_t *PID, u32 a_bit, s8 a_bSet);
#else
void itfMenuSetFlag(dds3ProcessID_t *PID, u16 a_bit, s8 a_bSet);
#endif
s32 itfMenuSetFlagMenu(dds3ProcessID_t *PID, s32 a_id, ITFMENU_CB_GF a_bit, s32 a_bSet);
struct sprHandle_t *itfCallDevLoadFileEx(u8 *a_strSprName);
s32 itfMenuSetSpriteData(dds3ProcessID_t *PID, ITFMENU_IMG_DAT a_type, s32 a_baseX, s32 a_baseY, s32 a_ofsX,
                         s32 a_ofsY, s32 a_fontX, s32 a_fontY, u8 *a_strSprName, itfSprList_t *a_pList);
s32 itfMenuSetImageColor(dds3ProcessID_t *PID, ITFMENU_IMG_DAT a_type, u8 a_R, u8 a_G, u8 a_B, u8 a_A);
s8 itfMenuSetImageColorSub(Menu_t *wk, ITFMENU_IMG_DAT a_type, u8 a_R, u8 a_G, u8 a_B, u8 a_A);
s32 itfMenuGetMenuNo(dds3ProcessID_t *PID, ITFMENU_IMG_DAT a_type, struct sprHandle_t *a_hSpr);
s32 _MenuSetMenu(MenuCB_tag *cb, s32 a_id, s32 a_parentId, char *a_str, itfMenuFunc_t a_func);
void itfMenuStart(dds3ProcessID_t *PID);
void itfCallGetProcessData6(dds3ProcessID_t *PID);
void itfMoneyProcessStart(void);
s32 itfMoverGetPosition(struct itfMover_t *mv, f32 *a_pPosX, f32 *a_pPosY);
s32 itfMoverInit(struct itfMover_t *mv);
f32 itfMoverGetRatio(struct itfMover_t *mv);
s32 itfMoverSetParam(struct itfMover_t *mv, s32 a_posx, s32 a_posy, s32 a_tgtx, s32 a_tgty, s32 a_frame);
ITF_MOVER_UPDATE itfMoverUpdate(struct itfMover_t *mv);
s32 itfSetMesDispPriority(void);
extern s32 itfSetMesDispPriorityData[];
dds3ProcessID_t *mapMenuCreateMenuPID(void);
s32 mapMenuDrawMagWindow(s32 a_initID);
s32 mapMenuGetSelectUNIQ(void);
void mapMenuProcessStart(void);
#ifdef VERSION_RAIDOU2
s32 lmapMenuProcess(dds3ProcessID_t *PID);
#endif
void mapMenuRelease(tagMAP_MENU_WORK *wk);
void mapMenuShutdown(dds3ProcessID_t *PID);
dds3ProcessFunc_t TitleAdvertiseDrawProcess(dds3ProcessID_t *PID);
void mphCallMemAllocDirect(void);

#endif
