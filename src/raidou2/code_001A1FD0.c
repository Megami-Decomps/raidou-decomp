#include "common.h"
#include "itf.h"
#include "sdfMemory.h"
#include "code_r1_002C8188.h"
#include "dds3KernelCore.h"
#include "sdfChip.h"
#include "dds3KernelDraw_0010A750.h"
#include "ps2asm.h"
#include "code_r1_002CAFD8.h"
#include "code_r1_002CA3F0.h"
#include "dds3KernelDraw.h"
#include "sdk_libc.h"

s32 itfMoverInit(itfMover_t *mv)
{
    if (mv == NULL) {
        return 0;
    }
    mv->cb_cnt = 0;
    mv->cb_num = 0;
    mv->cnt = 0;
    mv->flag = 0;
    mv->total_cnt = 0;
    mv->total_frm = 0;
    return 1;
}

INCLUDE_ASM(const s32, "code_001A1FD0", itfMoverSetData);

s32 itfMoverSetParam(itfMover_t *mv, s32 a_posx, s32 a_posy, s32 a_tgtx, s32 a_tgty, s32 a_frame)
{
    s32 ret = 0;

    if (mv != NULL) {
        itfMoverInit(mv);
        ret = itfMoverSetData(mv, a_posx, a_posy, a_tgtx, a_tgty, 0.0f, 0, 0, a_frame);
    }
    return ret;
}

INCLUDE_ASM(const s32, "code_001A1FD0", itfMoverUpdate);

s32 itfMoverGetPosition(itfMover_t *mv, f32 *a_pPosX, f32 *a_pPosY)
{
    if (mv == NULL || a_pPosX == NULL || a_pPosY == NULL) {
        return 0;
    }
    if (mv->cb_num <= 0) {
        return 0;
    }
    *a_pPosX = mv->cb[mv->cb_cnt].posx;
    *a_pPosY = mv->cb[mv->cb_cnt].posy;
    return 1;
}

INCLUDE_ASM(const s32, "code_001A1FD0", DvlMsgDrawProcess);

INCLUDE_ASM(const s32, "code_001A1FD0", itfMoverGetRatio);

void itfCallSetOt6(DDS3_OT a_otNo)
{
    eglSetOt(a_otNo);
    eglLinkTest(1, 0, 0x80, 3, 0, 0, 1, 2);
    eglLinkAlpha(0);
}

void itfDrawMiscSprite(s32 a_x, s32 a_y, s32 a_z, struct sprHandle_t *a_hSpr, s32 a_sprNo, DDS3_OT a_otNo) {
    itfCallSetOt6(a_otNo);
    itfDrawGridWithResolvedSlot(a_x * 16, a_y * 8, a_z, 0x60, a_hSpr, a_sprNo, a_otNo);
}

void itfDrawMiscSpriteAlpha(s32 a_x, s32 a_y, s32 a_z, u32 a_alphaRate, struct sprHandle_t *a_hSpr, s32 a_sprNo, DDS3_OT a_otNo)
{
    itfCallSetOt6(a_otNo);
    effDrawSpriteWithCornerFade(a_x * 16, a_y * 8, a_z, a_alphaRate, 96, a_hSpr, a_sprNo, a_otNo);
}

INCLUDE_ASM(const s32, "code_001A1FD0", itfCallLinkAlpha);

INCLUDE_ASM(const s32, "code_001A1FD0", itfFunc);

void itfDrawMiscSpriteListSelect(s32 a_x, s32 a_y, s32 a_z, u32 a_alphaRate, struct sprHandle_t *a_hSpr, DDS3_OT a_otNo, itfSprList_t *a_list, s32 a_no)
{
    s32 x;
    s32 y;

    if (a_no < 0) {
        return;
    }
    if (a_list[a_no].flag & 2) {
        x = a_list[a_no].ofsx;
        y = a_list[a_no].ofsy;
    } else {
        x = a_x + a_list[a_no].ofsx;
        y = a_y + a_list[a_no].ofsy;
    }
    if (a_list[a_no].flag & 1) {
        itfDrawMiscSpriteAlpha(x, y, a_z, a_alphaRate, a_hSpr, a_list[a_no].sprNo, a_otNo);
    } else {
        itfDrawMiscSprite(x, y, a_z, a_hSpr, a_list[a_no].sprNo, a_otNo);
    }
}

void itfDrawMiscSpriteListEx(s32 a_x, s32 a_y, s32 a_z, u32 a_alphaRate, struct sprHandle_t *a_hSpr, DDS3_OT a_otNo, u32 a_flag, itfSprList_t *a_list)
{
    s32 i;
    s32 draw;
    u16 flag;

    if (a_list == NULL) {
        return;
    }
    for (i = 0; a_list[i].sprNo >= 0; i++) {
        flag = a_list[i].flag;
        draw = 1;
        if (a_flag != 0) {
            draw = (flag & a_flag) != 0;
        }
        if (flag & 4) {
            draw = 0;
        }
        if (draw) {
            itfDrawMiscSpriteListSelect(a_x, a_y, a_z, a_alphaRate, a_hSpr, a_otNo, a_list, i);
        }
    }
}

void itfDrawMiscSpriteList(s32 a_x, s32 a_y, s32 a_z, u32 a_alphaRate, struct sprHandle_t *a_hSpr, DDS3_OT a_otNo, itfSprList_t *a_list)
{
    itfDrawMiscSpriteListEx(a_x, a_y, a_z, a_alphaRate, a_hSpr, a_otNo, 0, a_list);
}

INCLUDE_ASM(const s32, "code_001A1FD0", itfCallEnableMode);

INCLUDE_ASM(const s32, "code_001A1FD0", itfDrawFont);

INCLUDE_ASM(const s32, "code_001A1FD0", itfDrawMiscFont);

INCLUDE_ASM(const s32, "code_001A1FD0", itfDrawMiscFontAlpha);

INCLUDE_ASM(const s32, "code_001A1FD0", itfDrawMiscHelp);

INCLUDE_ASM(const s32, "code_001A1FD0", itfDrawMiscGauge);

INCLUDE_ASM(const s32, "code_001A1FD0", itfCallSetOt3);

INCLUDE_ASM(const s32, "code_001A1FD0", itfCallDrawBoxLine);

void itfDrawFanBezierPt3(f32 x0, f32 y0, f32 x1, f32 y1, f32 x2, f32 y2, f32 vx, f32 vy, u32 rgba0, u32 rgba1, u32 rgba2, u32 ot)
{
    u32 col[3];
    u32 base[4];
    s32 diff[4];
    f32 px;
    f32 py;
    s32 cx;
    s32 cy;
    s32 lx;
    s32 ly;
    s32 nx;
    s32 ny;
    u32 n;
    u32 i;
    f32 t;

    n = itfGetDivBezierPt3(x1, y1, x2, y2, vx, vy, 0.1f);
    cx = x0;
    cy = y0;
    col[0] = rgba0;
    col[1] = rgba1;
    col[2] = rgba2;
    base[0] = rgba1 >> 24;
    base[1] = (rgba1 & 0xFF0000) >> 16;
    base[2] = (rgba1 & 0xFF00) >> 8;
    base[3] = rgba1 & 0xFF;
    diff[0] = (rgba2 >> 24) - base[0];
    diff[1] = ((rgba2 & 0xFF0000) >> 16) - base[1];
    diff[2] = ((rgba2 & 0xFF00) >> 8) - base[2];
    diff[3] = (rgba2 & 0xFF) - base[3];
    lx = x1;
    ly = y1;
    for (i = 0; i < n; i++) {
        t = (f32)(i + 1) / (f32)n;
        itfGetRatePosBezierPt3(&px, &py, x1, y1, x2, y2, vx, vy, t);
        nx = px;
        ny = py;
        col[2] = (base[0] + (s32)(diff[0] * t)) << 24;
        col[2] |= (base[1] + (s32)(diff[1] * t)) << 16;
        col[2] |= (base[2] + (s32)(diff[2] * t)) << 8;
        col[2] |= base[3] + (s32)(diff[3] * t);
        eglLinkTri(cx, cy, lx, ly, nx, ny, col[0], col[1], col[2]);
        col[1] = col[2];
        lx = nx;
        ly = ny;
    }
}

s32 itfCalcSpriteWidthSub(f32 a_value, s32 a_shift)
{
    s32 whole = (s32)a_value;

    return (whole << a_shift) + (s32)((a_value - (f32)whole) * (f32)(1 << a_shift));
}

s32 itfGetFigure(s32 a_num)
{
    s32 figure;

    if (a_num < 0) {
        return -1;
    }
    figure = 1;
    while (a_num >= 10) {
        a_num /= 10;
        figure++;
    }
    return figure;
}

s32 itfGetFigureNum(s32 a_num, s32 a_figure)
{
    s32 div;
    s32 i;

    if (a_figure <= 0) {
        return -1;
    }
    div = 1;
    for (i = 1; i < a_figure; i++) {
        div *= 10;
    }
    return (a_num / div) % 10;
}

INCLUDE_ASM(const s32, "code_001A1FD0", itfSwapRGBA);

void itfCalcSpriteWidth(struct sprHandle_t *a_hSpr, s32 a_sprNo, s32 a_num, s32 a_max)
{
    f32 width;
    f32 w;

    if (a_max <= 0) {
        return;
    }
    width = a_hSpr->workEntries[a_sprNo].sourceWidth;
    w = width * a_num / a_max;
    a_hSpr->workEntries[a_sprNo].geometry.bounds[2] = itfCalcSpriteWidthSub(w, 4);
    a_hSpr->workEntries[a_sprNo].parameters[2] = w - width;
}

void itfGetRatePosBezierPt3(f32 *px, f32 *py, f32 x0, f32 y0, f32 x1, f32 y1, f32 vx, f32 vy, f32 rate)
{
    f32 r;
    f32 u;
    f32 a;
    f32 b;

    if (rate < 0.0f) {
        r = 0.0f;
    } else if (rate > 1.0f) {
        r = 1.0f;
    } else {
        r = rate;
    }
    u = 1.0f - r;
    a = u * u;
    b = (u + u) * r;
    *px = a * x0 + b * vx + r * r * x1;
    *py = a * y0 + b * vy + r * r * y1;
}

void itfGetVectorPosBezierPt3(f32 *px, f32 *py, f32 x0, f32 y0, f32 x1, f32 y1, f32 rx, f32 ry, f32 rate)
{
    f32 r;
    f32 u;
    f32 a;
    f32 b;

    if (rate < 0.0f) {
        r = 0.0f;
    } else if (rate > 1.0f) {
        r = 1.0f;
    } else {
        r = rate;
    }
    u = 1.0f - r;
    a = u * u;
    b = (u + u) * r;
    *px = (rx - a * x0 - r * r * x1) / b;
    *py = (ry - a * y0 - r * r * y1) / b;
}

u32 itfGetDivBezierPt3(f32 x0, f32 y0, f32 x1, f32 y1, f32 vx, f32 vy, f32 divLength)
{
    f32 px;
    f32 py;
    f32 rate;
    f32 dx;
    f32 dy;
    u32 n;

    if (divLength <= 0.0f) {
        return 0;
    }
    rate = 1.0f;
    n = 1;
    do {
        rate *= 0.5f;
        itfGetRatePosBezierPt3(&px, &py, x0, y0, x1, y1, vx, vy, rate);
        n++;
        dx = px - x0;
        dy = py - y0;
    } while (!(sdfSqrt(dx * dx + dy * dy) <= divLength));
    return n;
}

s32 itfCallMemFreeDelayedDirect(void)
{
    if (itfData5 == NULL) {
        return 0;
    }
    sdfMemFreeDelayedDirect(itfData5);
    itfData5 = NULL;
    return 1;
}

s32 itfLoadIconSprite(void)
{
    if (itfData5 != NULL) {
        return 0;
    }
    itfData5 = sdfDevLoadFileEx("/itf/icon.spr", NULL, NULL);
    return 1;
}

INCLUDE_ASM(const s32, "code_001A1FD0", itfFunc4);

s32 itfCallLoadIconSprite(void)
{
    if (itfData5 == NULL) {
        itfLoadIconSprite();
        if (itfData6 != NULL) {
            itfFunc4();
        }
    }
    if (itfData6 != NULL) {
        return 0;
    }
    itfData6 = effCreateResourceSlotSetFromAllocation(itfData5, 0);
    return 1;
}

void itfIconDrawSprite(s32 a_x, s32 a_y, s32 a_z, u32 a_alphaRate, s32 a_sprNo, DDS3_OT a_otNo)
{
    itfDrawMiscSpriteAlpha(a_x, a_y, a_z, a_alphaRate, itfData6, a_sprNo, a_otNo);
}

void itfIconDrawBadStatus(s32 a_x, s32 a_y, s32 a_z, u32 a_alphaRate, u32 a_bstNo, DDS3_OT a_otNo)
{
    s32 sprNo[20] = { 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 0, 45, 46, 0, 0, 0, 55 };

    if (a_bstNo < 20 && sprNo[a_bstNo] >= 32) {
        itfIconDrawSprite(a_x, a_y, a_z, a_alphaRate, sprNo[a_bstNo], a_otNo);
    }
}

INCLUDE_ASM(const s32, "code_001A1FD0", itfIconDrawBullet);

INCLUDE_ASM(const s32, "code_001A1FD0", itfLeaf19);

struct sprHandle_t *itfDrawBtnIconSub(void) {
    return itfData6;
}

INCLUDE_ASM(const s32, "code_001A1FD0", itfLeaf20);

s32 itfCallLoadIconSprite2(void)
{
    itfLoadIconSprite();
    itfCallLoadIconSprite();
    return itfCallMemFreeDelayedDirect();
}

INCLUDE_ASM(const s32, "code_001A1FD0", itfFunc5);

s32 MenuSetMoverAppear00(itfMover_t *mv, s32 a_no, s32 a_x, s32 a_y, s32 a_w, s32 a_h)
{
    return itfMoverSetParam(mv, a_x, a_y + a_h * a_no, a_x, a_y + a_h * a_no, 10);
}

s32 MenuSetMoverAppear01(itfMover_t *mv, s32 a_no, s32 a_x, s32 a_y, s32 a_w, s32 a_h)
{
    return itfMoverSetParam(mv, a_x, a_y, a_x, a_y + a_h * a_no, 10);
}

INCLUDE_ASM(const s32, "code_001A1FD0", MenuSetMoverAppear02);

s32 itfCallMoverInit(itfMover_t *mv, s32 a_no, s32 a_x, s32 a_y, s32 a_w, s32 a_h)
{
    itfMoverInit(mv);
    return itfMoverSetData(mv, 512, a_y + a_h * a_no, a_x, a_y + a_h * a_no, -0.35f, 0, 0, 10);
}

INCLUDE_ASM(const s32, "code_001A1FD0", MenuSetMoverAppear04);

INCLUDE_ASM(const s32, "code_001A1FD0", MenuSetMoverAppear);

extern f32 MenuSetMoverDisappear00Work;
extern f32 MenuSetMoverDisappear00Work2;

s32 MenuSetMoverDisappear00(itfMover_t *mv, s32 a_no, s32 a_x, s32 a_y, s32 a_w, s32 a_h)
{
    itfMoverGetPosition(mv, &MenuSetMoverDisappear00Work, &MenuSetMoverDisappear00Work2);
    return itfMoverSetParam(mv, MenuSetMoverDisappear00Work, MenuSetMoverDisappear00Work2, MenuSetMoverDisappear00Work,
                            MenuSetMoverDisappear00Work2, 10);
}

extern f32 MenuSetMoverDisappear01Work;
extern f32 MenuSetMoverDisappear01Work2;

s32 MenuSetMoverDisappear01(itfMover_t *mv, s32 a_no, s32 a_x, s32 a_y, s32 a_w, s32 a_h)
{
    itfMoverGetPosition(mv, &MenuSetMoverDisappear01Work, &MenuSetMoverDisappear01Work2);
    return itfMoverSetParam(mv, MenuSetMoverDisappear01Work, MenuSetMoverDisappear01Work2, MenuSetMoverDisappear01Work, a_y,
                            10);
}

INCLUDE_ASM(const s32, "code_001A1FD0", MenuSetMoverDisappear02);

s32 MenuSetMoverDisappear03(itfMover_t *mv, s32 a_no, s32 a_x, s32 a_y, s32 a_w, s32 a_h)
{
    itfMoverInit(mv);
    return itfMoverSetData(mv, a_x, a_y + a_h * a_no, 512, a_y + a_h * a_no, -0.35f, 0, 0, 10);
}

INCLUDE_ASM(const s32, "code_001A1FD0", itfCallMoverGetPosition);

INCLUDE_ASM(const s32, "code_001A1FD0", MenuUpdateMover);

MenuCB_tag *MenuGetBlankMenu(Menu_t *wk)
{
    MenuCB_tag *cb;

    for (cb = wk->pHead; cb != NULL; cb = cb->pNext) {
        if ((cb->lflag & 1) == 0) {
            return cb;
        }
    }
    return NULL;
}

INCLUDE_ASM(const s32, "code_001A1FD0", itfCallGetProcessData);

void MenuCreateMenu(Menu_t *wk)
{
    MenuCB_tag *cb = sdfChipAllocZeroDirect(sizeof(MenuCB_tag));

    if (wk->pHead == NULL) {
        cb->pPrev = NULL;
        wk->pHead = cb;
    } else {
        cb->pPrev = wk->pTail;
        wk->pTail->pNext = cb;
    }
    cb->pNext = NULL;
    wk->pTail = cb;
    wk->nMenuSet++;
}

INCLUDE_ASM(const s32, "code_001A1FD0", MenuDeleteMenu);

s32 MenuClearMenuSub(Menu_t *wk, MenuCB_tag *cb)
{
    MenuCB_tag *pPrev;
    MenuCB_tag *pNext;

    if (cb == NULL) {
        return 0;
    }
    pPrev = cb->pPrev;
    pNext = cb->pNext;
    memset(cb, 0, sizeof(MenuCB_tag));
    if (pPrev != NULL) {
        cb->pPrev = pPrev;
    }
    if (pNext != NULL) {
        cb->pNext = pNext;
    }
    return 1;
}

INCLUDE_ASM(const s32, "code_001A1FD0", itfMenuClearMenu);

INCLUDE_ASM(const s32, "code_001A1FD0", itfMenuClearMenuAll);

INCLUDE_ASM(const s32, "code_001A1FD0", _MenuSetMenu);

void itfMenuSetMenuTitle(dds3ProcessID_t *PID, char *a_str)
{
    Menu_t *wk = dds3GetProcessData(PID);

    if (wk != NULL) {
        _MenuSetMenu(&wk->title, -1, -1, a_str, NULL);
    }
}

s8 itfMenuSetMenu(dds3ProcessID_t *PID, s32 a_id, s32 a_parentId, char *a_str, itfMenuFunc_t a_func)
{
    Menu_t *wk = dds3GetProcessData(PID);
    MenuCB_tag *cb;

    if (a_id < 0) {
        return 0;
    }
    cb = MenuGetBlankMenu(wk);
    if (cb == NULL) {
        MenuCreateMenu(wk);
        cb = wk->pTail;
    }
    _MenuSetMenu(cb, a_id, a_parentId, a_str, a_func);
    return 1;
}

s32 itfMenuSetRootMenu(dds3ProcessID_t *PID, s32 a_id, char *a_str, itfMenuFunc_t a_func)
{
    return itfMenuSetMenu(PID, a_id, -1, a_str, a_func);
}

void itfListSetTypeApper(dds3ProcessID_t *PID, s32 a_mode, s32 a_type)
{
    Menu_t *wk = dds3GetProcessData(PID);

    if ((u32)a_mode < 2) {
        wk->dispType[a_mode] = a_type;
    }
}

void itfGeneralMenuCreatePIDSub(dds3ProcessID_t *PID, s32 a_type)
{
    itfListSetTypeApper(PID, 0, a_type);
}

void itfGeneralMenuCreatePIDSub2(dds3ProcessID_t *PID, s32 a_type)
{
    itfListSetTypeApper(PID, 1, a_type);
}

void itfMenuSetFlag(dds3ProcessID_t *PID, u32 a_bit, s8 a_bSet)
{
    Menu_t *wk = dds3GetProcessData(PID);

    if (a_bSet) {
        wk->gflag |= a_bit;
    } else {
        wk->gflag &= ~a_bit;
    }
}

s8 itfMenuSetFlagMenuSub(Menu_t *wk, s32 a_id, ITFMENU_CB_GF a_bit, s8 a_bSet, s8 a_bGlobal)
{
    MenuCB_tag *cb;

    if (a_id < 0) {
        for (cb = wk->pHead; cb != NULL; cb = cb->pNext) {
            if (!(cb->lflag & 1)) {
                continue;
            }
            if (a_bSet) {
                if (a_bGlobal) {
                    cb->gflag |= a_bit;
                } else {
                    cb->lflag |= a_bit;
                }
            } else {
                if (a_bGlobal) {
                    cb->gflag &= ~a_bit;
                } else {
                    cb->lflag &= ~a_bit;
                }
            }
        }
        return 1;
    }
    for (cb = wk->pHead; cb != NULL; cb = cb->pNext) {
        if ((cb->lflag & 1) && cb->id == a_id) {
            if (a_bSet) {
                if (a_bGlobal) {
                    cb->gflag |= a_bit;
                } else {
                    cb->lflag |= a_bit;
                }
            } else {
                if (a_bGlobal) {
                    cb->gflag &= ~a_bit;
                } else {
                    cb->lflag &= ~a_bit;
                }
            }
            return 1;
        }
    }
    return 0;
}

s32 itfMenuSetFlagMenu(dds3ProcessID_t *PID, s32 a_id, ITFMENU_CB_GF a_bit, s32 a_bSet)
{
    s8 bSet = a_bSet;

    return itfMenuSetFlagMenuSub(dds3GetProcessData(PID), a_id, a_bit, bSet, 1);
}

s32 itfMenuGetFlag(dds3ProcessID_t *PID, u32 a_bit)
{
    Menu_t *wk = dds3GetProcessData(PID);

    return (wk->gflag & a_bit) != 0;
}

s32 itfMenuGetFlagMenu(dds3ProcessID_t *PID, s32 a_id, ITFMENU_CB_GF a_bit)
{
    Menu_t *wk = dds3GetProcessData(PID);
    MenuCB_tag *cb;

    for (cb = wk->pHead; cb != NULL; cb = cb->pNext) {
        if ((cb->lflag & 1) && cb->id == a_id) {
            return (cb->gflag & a_bit) != 0;
        }
    }
    return 0;
}

INCLUDE_ASM(const s32, "code_001A1FD0", itfCallGetProcessData3);

ITFMENU_WAIT_TYPE itfMenuGetWaitType(dds3ProcessID_t *PID)
{
    Menu_t *wk = dds3GetProcessData(PID);

    if (wk->step == 1) {
        return ITFMENU_WAIT_TYPE_SET;
    }
    if (wk->step == 6) {
        if (wk->ctrlType == 5) {
            return ITFMENU_WAIT_TYPE_CANCEL;
        }
        if (wk->ctrlType == 4) {
            return ITFMENU_WAIT_TYPE_DECIDE;
        }
    }
    return ITFMENU_WAIT_TYPE_NONE;
}

INCLUDE_ASM(const s32, "code_001A1FD0", itfMenuGetWaitType2);

MenuCB_tag *MenuGetMenu(Menu_t *wk, s32 a_hierarchy, s32 a_no)
{
    MenuCB_tag *cb;

    for (cb = wk->pHead; cb != NULL; cb = cb->pNext) {
        if ((cb->lflag & 1) && cb->hierarchy == a_hierarchy && cb->no == a_no) {
            return cb;
        }
    }
    return NULL;
}

MenuCB_tag *MenuGetSelectMenu(Menu_t *wk)
{
    return MenuGetMenu(wk, wk->nowHierarchy, wk->selNo[wk->nowHierarchy]);
}

INCLUDE_ASM(const s32, "code_001A1FD0", MenuMakeLink);

INCLUDE_ASM(const s32, "code_001A1FD0", MenuUpdateLinkMover);

INCLUDE_ASM(const s32, "code_001A1FD0", itfCallMenuGetSelectMenu);

INCLUDE_ASM(const s32, "code_001A1FD0", itfCallMenuGetSelectMenu2);

void MenuUpdatePrevLink(Menu_t *wk)
{
    MenuCB_tag *parent;
    MenuCB_tag *cb;
    s32 h;

    wk->selMax = 0;
    for (cb = wk->pHead; cb != NULL; cb = cb->pNext) {
        if (!(cb->lflag & 1)) {
            continue;
        }
        cb->hierarchy = -1;
        cb->lflag &= ~2;
        cb->pParent = NULL;
        cb->no = -1;
        if (cb->parentId == -1) {
            cb->hierarchy = 0;
            cb->no = wk->selMax;
            wk->selMax++;
        } else if (cb->id == cb->parentId) {
            cb->no = -1;
            cb->hierarchy = 0;
        }
    }
    for (h = 1; h < 4; h++) {
        for (parent = wk->pHead; parent != NULL; parent = parent->pNext) {
            if (!(parent->lflag & 1) || parent->hierarchy != h - 1) {
                continue;
            }
            for (cb = wk->pHead; cb != NULL; cb = cb->pNext) {
                if ((cb->lflag & 1) && cb->parentId == parent->id) {
                    cb->hierarchy = h;
                    cb->pParent = parent;
                    parent->lflag |= 2;
                }
            }
        }
    }
    MenuUpdateLinkMover(wk);
    wk->lflag |= 0x10;
}

void itfMenuSetOt(dds3ProcessID_t *PID)
{
    Menu_t *wk = dds3GetProcessData(PID);

    wk->lflag |= 8;
}

void itfMenuStart(dds3ProcessID_t *PID)
{
    Menu_t *wk = dds3GetProcessData(PID);

    if (!(wk->lflag & 0x20)) {
        if (!(wk->lflag & 0x10)) {
            itfMenuSetOt(PID);
        }
        wk->lflag |= 0x20;
    }
}

s32 itfMenuGetSelectID(dds3ProcessID_t *PID)
{
    Menu_t *wk = dds3GetProcessData(PID);

    if (wk->select == NULL) {
        return -1;
    }
    return wk->select->id;
}
