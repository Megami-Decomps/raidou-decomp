#include "common.h"
#include "itf.h"
#include "sdfMemory.h"
#include "code_r1_002C8188.h"
#include "dds3KernelCore.h"
#include "sdfChip.h"
#include "dds3KernelDraw_0010A750.h"
#include "ps2asm.h"

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

INCLUDE_ASM(const s32, "code_001A1FD0", itfMoverSetParam);

INCLUDE_ASM(const s32, "code_001A1FD0", itfMoverUpdate);

INCLUDE_ASM(const s32, "code_001A1FD0", itfMoverGetPosition);

INCLUDE_ASM(const s32, "code_001A1FD0", DvlMsgDrawProcess);

INCLUDE_ASM(const s32, "code_001A1FD0", itfMoverGetRatio);

INCLUDE_ASM(const s32, "code_001A1FD0", itfCallSetOt6);

INCLUDE_ASM(const s32, "code_001A1FD0", itfDrawMiscSprite);

INCLUDE_ASM(const s32, "code_001A1FD0", itfDrawMiscSpriteAlpha);

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

INCLUDE_ASM(const s32, "code_001A1FD0", itfLeaf15);

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

INCLUDE_ASM(const s32, "code_001A1FD0", itfLeaf16);

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

INCLUDE_ASM(const s32, "code_001A1FD0", itfLeaf21);

INCLUDE_ASM(const s32, "code_001A1FD0", itfLeaf22);

INCLUDE_ASM(const s32, "code_001A1FD0", itfLeaf23);

INCLUDE_ASM(const s32, "code_001A1FD0", itfCallMoverInit);

INCLUDE_ASM(const s32, "code_001A1FD0", itfLeaf24);

INCLUDE_ASM(const s32, "code_001A1FD0", MenuSetMoverAppear);

INCLUDE_ASM(const s32, "code_001A1FD0", MenuSetMoverDisappear00);

INCLUDE_ASM(const s32, "code_001A1FD0", MenuSetMoverDisappear01);

INCLUDE_ASM(const s32, "code_001A1FD0", MenuSetMoverDisappear02);

INCLUDE_ASM(const s32, "code_001A1FD0", MenuSetMoverDisappear03);

INCLUDE_ASM(const s32, "code_001A1FD0", itfCallMoverGetPosition);

INCLUDE_ASM(const s32, "code_001A1FD0", itfCallMoverUpdate);

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

INCLUDE_ASM(const s32, "code_001A1FD0", MenuClearMenuSub);

INCLUDE_ASM(const s32, "code_001A1FD0", itfMenuClearMenu);

INCLUDE_ASM(const s32, "code_001A1FD0", itfMenuClearMenuAll);

INCLUDE_ASM(const s32, "code_001A1FD0", _MenuSetMenu);

INCLUDE_ASM(const s32, "code_001A1FD0", itfMenuSetMenuTitle);

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

INCLUDE_ASM(const s32, "code_001A1FD0", itfMenuSetFlagMenuSub);

INCLUDE_ASM(const s32, "code_001A1FD0", itfMenuSetFlagMenu);

INCLUDE_ASM(const s32, "code_001A1FD0", itfMenuGetFlag);

INCLUDE_ASM(const s32, "code_001A1FD0", itfMenuGetFlagMenu);

INCLUDE_ASM(const s32, "code_001A1FD0", itfCallGetProcessData3);

INCLUDE_ASM(const s32, "code_001A1FD0", itfMenuGetWaitType);

INCLUDE_ASM(const s32, "code_001A1FD0", itfMenuGetWaitType2);

INCLUDE_ASM(const s32, "code_001A1FD0", MenuGetMenu);

INCLUDE_ASM(const s32, "code_001A1FD0", MenuGetSelectMenu);

INCLUDE_ASM(const s32, "code_001A1FD0", MenuMakeLink);

INCLUDE_ASM(const s32, "code_001A1FD0", MenuUpdateSelectMenu);

INCLUDE_ASM(const s32, "code_001A1FD0", itfCallMenuGetSelectMenu);

INCLUDE_ASM(const s32, "code_001A1FD0", itfCallMenuGetSelectMenu2);

INCLUDE_ASM(const s32, "code_001A1FD0", MenuUpdatePrevLink);

void itfMenuSetOt(dds3ProcessID_t *PID)
{
    Menu_t *wk = dds3GetProcessData(PID);

    wk->lflag |= 8;
}

INCLUDE_ASM(const s32, "code_001A1FD0", itfMenuStart);

INCLUDE_ASM(const s32, "code_001A1FD0", itfMenuGetSelectID);
