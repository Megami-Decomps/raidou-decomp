#include "common.h"
#include "dds3KernelDraw.h"
#include "sdfMath.h"
#include "sbtlSub_0027ED08.h"
#include "mapMenuWin.h"

extern void eglSetOt(s32 ot);
extern void eglLinkTest(s32 ate, s32 atst, s32 aref, s32 afail, s32 date, s32 datm, s32 zte, s32 ztst);
extern void evtSubmitDefaultDepthGradientRect(s32 x, s32 y, s32 w, s32 h, u32 c0, u32 c1, u32 c2, u32 c3);
extern void mapIfDrawActionWin2(s32 x, s32 y, u32 color, const char *str);
extern void mapIgnite3(s32 id, const char **name1, const char **name2);
extern const char *mapCallSetOtData;

/* NON_MATCHING: 25 of 175 words match. Retail spills the window pointer and twice the index to the stack, mine does not. */
#ifdef NON_MATCHING
void mapCallSetOt(s32 idx) {
    mapMenuWin_t *win = &mapWork11[idx];
    s32 x;
    s32 y;
    s32 w;
    s32 h;
    s32 devilBase;
    s32 i;
    s32 pulse;
    const char *name1;
    const char *name2;

    if (win->state == 1) {
        return;
    }
    x = win->x + (s32)win->scroll;
    y = win->y;
    h = win->h;
    w = win->w;
    devilBase = win->devilBase;
    eglSetOt(0x67);
    eglLinkAlpha(0);
    eglLinkTest(1, 0, 0x80, 3, 0, 0, 1, 1);
    evtSubmitDefaultDepthGradientRect(x, y, w, h, 0x80606060, 0x80403830, 0x80303030, 0x80101010);
    if (win->state == 0) {
        pulse = (s32)(sdfSin(win->phase) * 16.0f) + 0x6F;
        evtSubmitDefaultDepthGradientRect(x + 4, y + win->sel * 22 + 10, w - 8, 20, pulse | 0x80100000, pulse | 0x40100000, pulse | 0x80100000, pulse | 0x40100000);
    }
    if (idx == 1) {
        evtSubmitDefaultDepthGradientRect(x, 246, 150, 78, 0x50101010, 0x50101010, 0x50101010, 0x50101010);
        mapIfDrawActionWin2(x + 16, 250, 0xA09DC380, mapCallSetOtData);
        for (i = 0; i < 7; i++) {
            mapIfDrawActionWin2(x + 16, y + 11 + i * 22, 0xA09DC380, sdatDevilName(devilBase + i));
        }
        mapIgnite3(devilBase + mapWork11[idx].sel, &name1, &name2);
        if (name1 != NULL) {
            mapIfDrawActionWin2(x + 16, 280, 0xA09DC380, name1);
        }
        if (name2 != NULL) {
            mapIfDrawActionWin2(x + 16, 300, 0xA09DC380, name2);
        }
    }
}
#else
INCLUDE_ASM(const s32, "code_r1_0016A298", mapCallSetOt);
#endif
