#include "common.h"
#include "kernel.h"
#include "itf.h"
#include "dds3KernelCore.h"
#include "sdfMemory.h"
#include "sdfChip.h"
#include "sdf_resource.h"
#include "dds3KernelDraw.h"
#include "sdk_libc.h"
#include "datCalc.h"
#include "dds3PadManager.h"

INCLUDE_ASM(const s32, "code_00190668", itfMenuChkSetMenu);

INCLUDE_ASM(const s32, "code_00190668", itfCallDevLoadFileEx);

s32 itfMenuGetMenuNo(dds3ProcessID_t *PID, ITFMENU_IMG_DAT a_type, struct sprHandle_t *a_hSpr)
{
    Menu_t *wk = dds3GetProcessData(PID);
    MenuImgDat_t *img;

    if (a_hSpr == NULL || (u32)a_type >= ITFMENU_IMG_DAT_MAX) {
        return 0;
    }
    img = &wk->imgDat[a_type];
    if (img->flag & 2) {
        return 0;
    }
    img->hSpr = a_hSpr;
    return 1;
}

s32 itfMenuLinkSprite(dds3ProcessID_t *PID, struct sprHandle_t *a_hSpr)
{
    s32 ret;
    s32 i;

    if (a_hSpr == NULL) {
        return 0;
    }
    ret = 1;
    for (i = 0; i < ITFMENU_IMG_DAT_MAX; i++) {
        if (itfMenuGetMenuNo(PID, i, a_hSpr) == 0) {
            ret = 0;
        }
    }
    return ret;
}

/* NON_MATCHING: 54 of 55 words match.  */
#ifdef NON_MATCHING
s32 itfMenuSetSpriteData(dds3ProcessID_t *PID, ITFMENU_IMG_DAT a_type, s32 a_baseX, s32 a_baseY, s32 a_ofsX,
                         s32 a_ofsY, s32 a_fontX, s32 a_fontY, u8 *a_strSprName, itfSprList_t *a_pList)
{
    Menu_t *wk = dds3GetProcessData(PID);
    MenuImgDat_t *img;

    if ((u32)a_type >= ITFMENU_IMG_DAT_MAX) {
        return 0;
    }
    img = &wk->imgDat[a_type];
    if (a_strSprName != NULL && img->hSpr == NULL) {
        img->hSpr = itfCallDevLoadFileEx(a_strSprName);
        img->flag |= 2;
    }
    img->pList = a_pList;
    img->sx = a_baseX;
    img->sy = a_baseY;
    img->sw = a_ofsX;
    img->sh = a_ofsY;
    img->fx = a_fontX;
    img->fy = a_fontY;
    return 1;
}
#else
INCLUDE_ASM(const s32, "code_00190668", itfMenuSetSpriteData);
#endif

/* NON_MATCHING: 18 of 19 words match.  */
#ifdef NON_MATCHING
s8 itfMenuSetImageColorSub(Menu_t *wk, ITFMENU_IMG_DAT a_type, u8 a_R, u8 a_G, u8 a_B, u8 a_A)
{
    if ((u32)a_type >= ITFMENU_IMG_DAT_MAX) {
        return 0;
    }
    wk->imgDat[a_type].color = (a_A << 24) + (a_B << 16) + (a_G << 8) + a_R;
    return 1;
}
#else
INCLUDE_ASM(const s32, "code_00190668", itfMenuSetImageColorSub);
#endif

s32 itfMenuSetImageColor(dds3ProcessID_t *PID, ITFMENU_IMG_DAT a_type, u8 a_R, u8 a_G, u8 a_B, u8 a_A)
{
    return itfMenuSetImageColorSub(dds3GetProcessData(PID), a_type, a_R, a_G, a_B, a_A);
}

INCLUDE_ASM(const s32, "code_00190668", GetSelectRootID);

INCLUDE_ASM(const s32, "code_00190668", itfMenuImgGetRatio);

INCLUDE_ASM(const s32, "code_00190668", evtPictureSetPri);

INCLUDE_ASM(const s32, "code_00190668", itfMoverSetMenu);

s32 itfCallGetProcessData7(dds3ProcessID_t *PID, u32 a_no, s32 a_val)
{
    Menu_t *wk = dds3GetProcessData(PID);

    if (a_no >= ITFMENU_IMG_DAT_MAX) {
        return 0;
    }
    wk->imgDat[a_no].strSpace = a_val;
    return 1;
}

INCLUDE_ASM(const s32, "code_00190668", itfMenuSetStringWidth);

INCLUDE_ASM(const s32, "code_00190668", itfMenuGetPanelParam);

INCLUDE_ASM(const s32, "code_00190668", itfMenuSetFunc);

INCLUDE_ASM(const s32, "code_00190668", itfMenuSetDrawFunc);

INCLUDE_ASM(const s32, "code_00190668", itfMenuSetFont);

INCLUDE_ASM(const s32, "code_00190668", itfMenuSetDrawParam);

INCLUDE_ASM(const s32, "code_00190668", cmpGetWork);

INCLUDE_ASM(const s32, "code_00190668", itfMenuSetMoveType);

INCLUDE_ASM(const s32, "code_00190668", itfMenuGetCtrlType);

INCLUDE_ASM(const s32, "code_00190668", itfMenuUpdateLink);

INCLUDE_ASM(const s32, "code_00190668", itfMenuSetSelectID);

INCLUDE_ASM(const s32, "code_00190668", itfMenuSetCancelSE);

INCLUDE_ASM(const s32, "code_00190668", itfMenuSetAnimDelMenu);

INCLUDE_ASM(const s32, "code_00190668", itfMenuSetAnimBakMenu);

INCLUDE_ASM(const s32, "code_00190668", itfCallGetProcessData8);

INCLUDE_ASM(const s32, "code_00190668", itfMenuGetFlashCount);

INCLUDE_ASM(const s32, "code_00190668", MenuRemakeLinkSub);

INCLUDE_ASM(const s32, "code_00190668", MenuDelMenuSub);

void MenuInit_itf(Menu_t *wk)
{
    s32 i;

    wk->pHead = NULL;
    wk->pTail = NULL;
    wk->select = NULL;
    wk->nowHierarchy = 0;
    wk->nMenuSet = 0;
    wk->drwDefMax = 8;
    wk->drwDefNum = 8;
    wk->drwMax = 8;
    wk->dispType[0] = 0;
    wk->dispType[1] = 0;
    wk->scrollType = 0;
    wk->ot = 103;
    wk->gflag = 31;
    wk->lflag = 0;
    wk->frame = 0;
    wk->count = 0;
    for (i = 0; i < 4; i++) {
        wk->selNo[i] = 0;
        wk->topNo[i] = 0;
        wk->drwNo[i] = 0;
    }
    for (i = 0; i < ITFMENU_IMG_DAT_MAX; i++) {
        wk->imgDat[i].hSpr = NULL;
        wk->imgDat[i].pList = NULL;
        wk->imgDat[i].color = 0x80808080;
        wk->imgDat[i].flag = 0;
        wk->imgDat[i].strWidth = 0;
        wk->imgDat[i].strStyle = 0;
        wk->imgDat[i].strSpace = 0;
        if (i == ITFMENU_IMG_DAT_SCROLL) {
            wk->imgDat[i].sx = 162;
            wk->imgDat[i].sy = 58;
            wk->imgDat[i].sw = 5;
            wk->imgDat[i].sh = 208;
            wk->imgDat[i].fx = 1;
            wk->imgDat[i].fy = 1;
        } else {
            wk->imgDat[i].sw = 128;
            wk->imgDat[i].sh = 26;
            wk->imgDat[i].fx = 4;
            wk->imgDat[i].fy = 2;
            switch (i) {
            case ITFMENU_IMG_DAT_TITLE:
                wk->imgDat[ITFMENU_IMG_DAT_TITLE].sx = 32;
                wk->imgDat[ITFMENU_IMG_DAT_TITLE].sy = 32;
                break;
            case ITFMENU_IMG_DAT_PANEL:
            case ITFMENU_IMG_DAT_SELECT:
                wk->imgDat[i].sx = 32;
                wk->imgDat[i].sy = 58;
                break;
            }
        }
    }
}

INCLUDE_ASM(const s32, "code_00190668", itfMenuCtrlSelectSub);

INCLUDE_ASM(const s32, "code_00190668", itfMenuCtrlSelectSub2);

INCLUDE_ASM(const s32, "code_00190668", MenuCtrlSelect);

INCLUDE_ASM(const s32, "code_00190668", itfLeaf);

INCLUDE_ASM(const s32, "code_00190668", MenuMoveNextHierarchy);

INCLUDE_ASM(const s32, "code_00190668", MenuMoveEnd);

INCLUDE_ASM(const s32, "code_00190668", itfCallMenuSetMoverDisappear);

INCLUDE_ASM(const s32, "code_00190668", MenuPlayDecideSE);

INCLUDE_ASM(const s32, "code_00190668", MenuPlayCancelSE);

/* NON_MATCHING: 35 of 439 words match.  */
#ifdef NON_MATCHING
s32 MenuProcess(dds3ProcessID_t *PID) {
    Menu_t *wk = dds3GetProcessData(PID);
    MenuCB_tag *cb;
    s32 ret;

    if (wk->lflag & 0x4000) {
        if (wk->step < 2 || wk->step >= 7) {
            wk->lflag &= ~0x4000;
        } else if (wk->step != 2) {
            MenuDelMenuSub(wk, wk->delId);
        }
    }

    switch (wk->step) {
    case 0:
    case 1:
        if (wk->lflag & 8) {
            MenuUpdatePrevLink(wk);
            wk->nowHierarchy = 0;
            wk->selNo[0] = 0;
            if (!MenuUpdateSelectMenu(wk)) {
                MenuCtrlSelect(wk, 3, 0);
            }
            if (wk->lflag & 0x8000) {
                itfCallGetProcessData5(PID, wk->initSelectID);
                wk->lflag &= ~0x8000;
            }
            wk->lflag &= ~8;
        }
        if (wk->lflag & 0x20) {
            if (wk->lflag & 0x10) {
                wk->lflag &= ~2;
                wk->step = 2;
                wk->bEnableCtrl = 0;
                itfCallMoverInit2(wk, wk->dispType[0]);
                wk->lflag |= 0x2E00;
            }
        }
        break;
    case 2:
        if (MenuUpdateMover(wk)) {
            wk->bEnableCtrl = 0;
            break;
        }
        wk->bEnableCtrl = 1;
        if (wk->gflag & 0x80) {
            wk->gflag &= ~0x80;
            MenuMoveEnd(wk);
            return 0;
        }
        if (wk->lflag & 0x4000) {
            wk->lflag &= ~0x4000;
            MenuDelMenuSub(wk, wk->delId);
        }
        if (wk->gflag & 0x800) {
            if (wk->select->gflag & 0x40) {
                wk->lflag &= ~0x2000;
            } else {
                wk->lflag |= 0x2000;
            }
            MenuSetMoverDisappear(wk, wk->dispType[1]);
            wk->step = 3;
            wk->ctrlType = 6;
            wk->gflag &= ~0x800;
            break;
        }
        if (!(wk->gflag & 8)) {
            break;
        }
        if (wk->gflag & 0x100) {
            return 0;
        }
        if (wk->gflag & 0x40) {
            if (dds3PadButtonState[0x22] < 0 || dds3PadButtonState[0x21] < 0 || dds3PadButtonState[0x23] < 0 || dds3PadButtonState[0x20] < 0 ||
                dds3PadButtonState[0x2A] < 0 || dds3PadButtonState[0x2B] < 0 || dds3PadButtonState[0x28] < 0 || dds3PadButtonState[0x29] < 0) {
                if (wk->select->func != NULL) {
                    wk->select->func();
                    if (wk->moveType != 0) {
                        itfCallMenuSetMoverDisappear(wk, wk->moveType);
                        wk->moveType = 0;
                        return 0;
                    }
                    return 0;
                }
            }
            itfLeaf(wk);
            return 0;
        }
        if (dds3PadButtonState[0x21] < 0 && itfCallMenuSetMoverDisappear(wk, 1)) {
            MenuPlayCancelSE(wk->select);
            return 0;
        }
        if (dds3PadButtonState[0x23] < 0) {
            cb = wk->select;
            if (cb->gflag & 6) {
                if (cb->gflag & 0x20) {
                    if (cb->func != NULL) {
                        cb->func();
                    }
                    itfCallMenuSetMoverDisappear(wk, 1);
                    MenuPlayCancelSE(wk->select);
                    return 0;
                }
                if ((cb->gflag & 0x80) && cb->func != NULL) {
                    wk->step = 5;
                    MenuPlayDecideSE(cb);
                    return 0;
                }
                if ((cb->gflag & 0x100) && cb->func != NULL) {
                    wk->retFunc = cb->func();
                    if (wk->moveType != 0) {
                        ret = itfCallMenuSetMoverDisappear(wk, wk->moveType);
                        if (ret) {
                            if (wk->moveType == 2) {
                                MenuPlayDecideSE(wk->select);
                            } else if (wk->moveType == 1) {
                                MenuPlayCancelSE(wk->select);
                            }
                        }
                        wk->moveType = 0;
                        if (ret) {
                            return 0;
                        }
                    }
                }
                if (itfCallMenuSetMoverDisappear(wk, 2)) {
                    MenuPlayDecideSE(wk->select);
                    return 0;
                }
            }
        }
        itfLeaf(wk);
        return 0;
    case 3:
        MenuUpdateMover(wk);
        wk->step = 4;
        break;
    case 4:
        if (MenuUpdateMover(wk)) {
            return 0;
        }
        switch (wk->ctrlType) {
        case 2:
            itfCallMenuGetSelectMenu(wk);
            itfCallMoverInit2(wk, wk->dispType[0]);
            wk->step = 2;
            break;
        case 3:
            itfCallMenuGetSelectMenu2(wk);
            itfCallMoverInit2(wk, wk->dispType[0]);
            wk->step = 2;
            break;
        case 4:
            cb = wk->select;
            if (cb->gflag & 0x40) {
                wk->lflag |= 0x40;
            }
            wk->lflag = (wk->lflag & ~0x2C20) | 0x2400;
            if (!(cb->gflag & 0x100)) {
                wk->step = 5;
                return 0;
            }
            if (wk->retFunc != 0) {
                wk->step = 6;
                return 0;
            }
            wk->count = 0;
            wk->step = 7;
            return 0;
        case 5:
            wk->lflag = (wk->lflag & ~0x2C20) | 0x2000;
            if (!(wk->gflag & 0x10)) {
                wk->step = 6;
                return 0;
            }
            wk->count = 0;
            wk->step = 7;
            return 0;
        case 6:
            ret = wk->gflag & 0x1000;
            wk->gflag |= 0x2000;
            if (ret) {
                if (wk->select->gflag & 0x40) {
                    wk->lflag &= ~0x2000;
                } else {
                    wk->lflag |= 0x2000;
                }
                itfCallMoverInit2(wk, wk->dispType[0]);
                wk->step = 2;
                wk->gflag &= ~0x3000;
            }
            break;
        }
        break;
    case 5:
        wk->retFunc = wk->select->func();
        if (wk->retFunc == 0) {
            wk->count = 0;
            wk->step = 7;
            return 0;
        }
        if (wk->moveType != 0) {
            itfCallMenuSetMoverDisappear(wk, wk->moveType);
            wk->moveType = 0;
            return 0;
        }
        wk->step = 6;
        return 0;
    case 6:
        if (wk->lflag & 0x20) {
            cb = wk->select;
            wk->lflag |= 0xC00;
            if (cb->gflag & 0x40) {
                wk->lflag &= ~0x2000;
            } else {
                wk->lflag |= 0x2000;
            }
            if (!(cb->gflag & 0x80)) {
                itfCallMoverInit2(wk, wk->dispType[0]);
            }
            wk->ctrlType = 0;
            wk->step = 2;
        }
        break;
    case 7:
        if (wk->count == 0) {
            wk->lflag &= ~0x200;
            wk->count = 1;
        } else if (wk->count >= 2) {
            wk->count = 0;
            wk->step = 8;
        } else {
            wk->count++;
        }
        break;
    case 8:
        return -1;
    }
    return 0;
}
#else
INCLUDE_ASM(const s32, "code_00190668", MenuProcess);
#endif

INCLUDE_ASM(const s32, "code_00190668", MenuDrawFont);

INCLUDE_ASM(const s32, "code_00190668", cmpItfmenuScrollbarTexture);

INCLUDE_ASM(const s32, "code_00190668", MenuDrawScrollBar);

INCLUDE_ASM(const s32, "code_00190668", MenuDrawPanelEGL);

INCLUDE_ASM(const s32, "code_00190668", MenuDrawPanel);

INCLUDE_ASM(const s32, "code_00190668", MenuDrawTitle);

INCLUDE_ASM(const s32, "code_00190668", MenuCalcAlpha);

/* NON_MATCHING: 22 of 203 words match.  */
#ifdef NON_MATCHING
s32 MenuDrawProcess(dds3ProcessID_t *PID)
{
    Menu_t *wk;
    s32 i;
    MenuCB_tag *select;
    MenuCB_tag *parent;
    MenuCB_tag *cb;
    MenuMover_t *mv;
    char *str;
    s32 type;
    s32 x;
    s32 y;
    u8 alpha;
    s8 attr;
    s32 draw;

    str = NULL;
    wk = dds3GetProcessData(PID);
    for (i = 3; i >= 0; i--) {
        wk->imgDat[i].alpha = 0;
    }
    if (!(wk->lflag & 0x200)) {
        return 0;
    }
    if (wk->step < 2) {
        return 0;
    }
    if (wk->step >= 7) {
        return 0;
    }
    if (!(wk->gflag & 1)) {
        return 0;
    }
    g_ot = wk->ot;
    eglSetOt(wk->ot);
    eglLinkTest(1, 0, 0x80, 3, 0, 0, 1, 2);
    eglLinkAlpha(0);
    if (wk->gflag & 2) {
        if (wk->lflag & 0x400) {
            select = wk->select;
            type = 0;
            if (!(select->gflag & 0x200) && (select == NULL || wk->step == 6)) {
                if (select == NULL) {
                    type = 1;
                    str = wk->title.str;
                } else if (select->gflag & 0x40) {
                    str = select->str;
                    type = 2;
                }
            } else {
                parent = select->pParent;
                if (parent == NULL) {
                    type = 1;
                    str = wk->title.str;
                } else {
                    str = parent->str;
                    type = 3;
                }
            }
            if (type > 0) {
                itfMoverGetPosition(&wk->mv_title.mover, &itfMenuDrawProcess_fx, &itfMenuDrawProcess_fy);
                x = itfMenuDrawProcess_fx;
                y = itfMenuDrawProcess_fy;
                MenuDrawTitle(wk, x, y, MenuCalcAlpha(&wk->mv_title, wk->lflag), str);
            }
        }
    }
    if (wk->lflag & 0x800) {
        mv = wk->mv;
        for (i = 0; i < wk->drwDefNum; i++, mv++) {
            alpha = MenuCalcAlpha(mv, wk->lflag);
            cb = mv->cb;
            attr = 0;
            if (cb != NULL) {
                if ((wk->gflag & 8) && (cb->lflag & 4)) {
                    attr = 6;
                }
                if (cb->gflag & 8) {
                    attr |= 2;
                }
                if (cb->gflag & 0x10) {
                    attr |= 4;
                }
                if (!(cb->gflag & 1) || !(cb->gflag & 2)) {
                    attr |= 1;
                }
            }
            MenuDrawPanel(wk, mv, alpha, attr);
        }
        alpha = MenuCalcAlpha(wk->mv, wk->lflag);
        draw = 0;
        wk->imgDat[1].alpha = alpha;
        if (wk->scrollType == 0) {
            if (wk->drwMax < wk->selMax) {
                draw = 1;
            }
        } else if (wk->scrollType == 2) {
            draw = 1;
        }
        if (draw > 0 && alpha == 0xFF) {
            cmpItfmenuScrollbarTexture(wk, wk->topNo[wk->nowHierarchy], wk->selMax, 0xFF);
        }
    }
    eglSetOt(0x67);
    eglLinkTest(1, 0, 0x80, 3, 0, 0, 1, 2);
    eglLinkAlpha(0);
    return 0;
}
#else
INCLUDE_ASM(const s32, "code_00190668", MenuDrawProcess);
#endif

INCLUDE_ASM(const s32, "code_00190668", cmpSklTypeSet);

INCLUDE_ASM(const s32, "code_00190668", cmpCallMenuDeleteMenu);

INCLUDE_ASM(const s32, "code_00190668", MenuShutdown);

dds3ProcessID_t *itfMenuProcessStart(void)
{
    struct sdfMemHandle_t *hMem = sdfMemAllocDirect(sizeof(Menu_t));
    Menu_t *wk = sdfMemGetPtr(hMem);
    dds3ProcessID_t *PID;

    memset(wk, 0, sizeof(Menu_t));
    wk->hMem = hMem;
    PID = dds3StartProcess("itfmenu_main", 1022, 0, 0, MenuProcess, MenuShutdown, wk);
    wk->drawPID = dds3StartProcess("itfmenu_draw", 11029, 0, 0, MenuDrawProcess, NULL, wk);
    MenuInit_itf(wk);
    wk->lflag |= 2;
    return PID;
}

void itfMenuProcessEnd(dds3ProcessID_t *PID)
{
    Menu_t *wk;

    if (PID != NULL && dds3ExistProcess(PID)) {
        wk = dds3GetProcessData(PID);
        if ((wk->lflag & 1) == 0) {
            wk->lflag |= 1;
            wk->step = 7;
            wk->count = 0;
        }
    }
}

/* NON_MATCHING: 19 of 20 words match.  */
#ifdef NON_MATCHING
s32 itfMenuProcessCheck(dds3ProcessID_t *PID)
{
    if (PID == NULL) {
        return 0;
    }
    if (dds3GetProcessStat(PID) != 3 && dds3GetProcessStat(PID) != 0) {
        return 1;
    }
    return 0;
}
#else
INCLUDE_ASM(const s32, "code_00190668", itfMenuProcessCheck);
#endif

INCLUDE_ASM(const s32, "code_00190668", itfCallGetProcessData2);

INCLUDE_ASM(const s32, "code_00190668", itfCountSetData);

INCLUDE_ASM(const s32, "code_00190668", itfCountSetMax);

INCLUDE_ASM(const s32, "code_00190668", itfCountSetMin);

INCLUDE_ASM(const s32, "code_00190668", CountShutdown);

INCLUDE_ASM(const s32, "code_00190668", itfCountProcessStart);

INCLUDE_ASM(const s32, "code_00190668", itfCountProcessStartMoney);

INCLUDE_ASM(const s32, "code_00190668", itfCountProcessEnd);

INCLUDE_ASM(const s32, "code_00190668", itfCountProcessCheck);

INCLUDE_ASM(const s32, "code_00190668", DvlMsgCreateCB);

INCLUDE_ASM(const s32, "code_00190668", DvlMsgDeleteCB);

INCLUDE_ASM(const s32, "code_00190668", DvlMsgGetCB);

INCLUDE_ASM(const s32, "code_00190668", DvlMsgFreeMsg);

INCLUDE_ASM(const s32, "code_00190668", DvlMsgFreeTex);

INCLUDE_ASM(const s32, "code_00190668", DvlMsgLoadFile);

INCLUDE_ASM(const s32, "code_00190668", itfDvlMsgCreateData);

INCLUDE_ASM(const s32, "code_00190668", _DvlMsgDeleteData);

INCLUDE_ASM(const s32, "code_00190668", itfDvlMsgDeleteData);

INCLUDE_ASM(const s32, "code_00190668", DvlMsgLoadSprite);

INCLUDE_ASM(const s32, "code_00190668", itfDvlMsgStart);

INCLUDE_ASM(const s32, "code_00190668", itfDvlMsgEnd);

INCLUDE_ASM(const s32, "code_00190668", DvlMsgSetMover);

INCLUDE_ASM(const s32, "code_00190668", itfCallDvlMsgGetCB);

INCLUDE_ASM(const s32, "code_00190668", DvlMsgRelease);

INCLUDE_ASM(const s32, "code_00190668", DvlMsgProcess);

INCLUDE_ASM(const s32, "code_00190668", itfCallMessageNumSearch);

INCLUDE_ASM(const s32, "code_00190668", itfCallSetOt4);

INCLUDE_ASM(const s32, "code_00190668", itfCallGetProcessData9);

INCLUDE_ASM(const s32, "code_00190668", itfLeaf25);

INCLUDE_ASM(const s32, "code_00190668", DvlMsgShutdown);

INCLUDE_RODATA(const s32, "code_00190668", itfCallSetOt4Table);

INCLUDE_ASM(const s32, "code_00190668", itfDvlMsgProcessStart);

INCLUDE_ASM(const s32, "code_00190668", itfDvlMsgProcessEnd_itf);

INCLUDE_ASM(const s32, "code_00190668", itfCallExistProcess);

INCLUDE_ASM(const s32, "code_00190668", itfMoneyProcessSub);

INCLUDE_ASM(const s32, "code_00190668", MoneyCheckProcEnd);

s32 MoneyProcess(dds3ProcessID_t *PID)
{
    struct tagITF_MONEY_WORK *wk = dds3GetProcessData(PID);

    switch (wk->step) {
    case 0:
        if (wk->flags & 2) {
            wk->step = 1;
            wk->flags = (wk->flags & ~2) | 0x80;
            wk->count = 0;
        }
        break;
    case 1:
        if (wk->count < 10) {
            wk->count++;
        } else {
            wk->count = 0;
            wk->step = 2;
        }
        break;
    case 2:
        if (wk->flags & 8) {
            wk->flags &= ~8;
            wk->countPID = NULL;
            if (wk->calc > 0) {
                if (wk->type == 2) {
                    datAddMaka(wk->calc);
                } else if (wk->type == 1) {
                    datAddMaka(-wk->calc);
                }
                wk->countPID = itfCountProcessStartMoney(wk->calc);
                itfCountSetData(wk->countPID, &wk->money, wk->type);
                itfCountSetMin(wk->countPID, &wk->money, 0);
            }
            wk->step = 3;
        } else if (wk->flags & 4) {
            wk->step = 4;
            wk->flags &= ~4;
            wk->count = 0;
            wk->countPID = NULL;
        }
        break;
    case 3:
        if (itfCountProcessCheck(wk->countPID) != 0) {
            break;
        }
        wk->countPID = NULL;
        if (wk->flags & 4) {
            wk->step = 4;
            wk->flags &= ~4;
            wk->count = 0;
        }
        break;
    case 4:
        if (wk->count < 10) {
            wk->count++;
        } else {
            wk->step = 0;
            wk->count = 0;
            wk->flags &= ~0x80;
        }
        break;
    case 5:
        if (wk->count == 0) {
            itfMoneyProcessSub(wk);
        }
        if (MoneyCheckProcEnd(wk) == 0) {
            wk->count = 0;
            wk->step = 6;
        } else {
            wk->count++;
        }
        break;
    case 6:
        if (wk->count == 0) {
            wk->flags &= ~0x80;
        } else if (wk->count >= 2) {
            wk->count = 0;
            wk->step = 7;
            break;
        }
        wk->count++;
        break;
    case 7:
        return -1;
    }
    return 0;
}

INCLUDE_ASM(const s32, "code_00190668", MoneyDrawPanel);

INCLUDE_RODATA(const s32, "code_00190668", MoneyDrawPanelTable);

INCLUDE_RODATA(const s32, "code_00190668", MoneyDrawPanelTable2);

