#include "common.h"
#include "kernel.h"
#include "dds3KernelCore.h"
#include "sdfMath.h"
#include "sdk_libc.h"
#include "itf.h"
#include "frFont.h"

s32 MoneyDrawProcess(dds3ProcessID_t *id)
{
    struct tagITF_MONEY_WORK *wk;
    frFontHandle_t *font;
    f32 scale;
    s32 offX;
    s32 alpha;
    char buf[16];

    wk = dds3GetProcessData(id);
    if (!(wk->flags & 0x80)) {
        return 0;
    }
    scale = (f32)wk->count * 1.5707963f / 10.0f;
    if (wk->step == 1) {
        offX = 32 - (s32)(sdfSin(scale) * 32.0f);
        alpha = (wk->count << 7) / 10;
    } else {
        offX = 0;
        if (wk->step == 4) {
            offX = (s32)(sdfSin(scale) * 32.0f);
            alpha = ((10 - wk->count) << 7) / 10;
        } else {
            alpha = 128;
        }
    }
    MoneyDrawPanel(offX, 0, alpha);
    font = frwGetSmallFontFRQ2((offX + 316) << 4, 2256, 0, ((u32)(alpha << 7) >> 7) | 0xA09DC300, MoneyDrawProcessData[0], 0);
    frSetFontColorNo(font, 3);
    frSetFontSpace(font, -1);
    frDrawFontEx2(font, 1, 106);
    frDestroyFontRequestAfter(font);
    scale = (f32)alpha * 0.0078125f;
    sprintf(buf, "%7d", wk->money);
    itfDrawFont(ITF_DM_STYLE_SNUM, offX + 375, 286, 0, 0, 0, 0xA09DC380, scale, 106, buf);
    return 0;
}

/* Retail has this in .data, not .sdata. MoneyDrawProcess must see it unsized, as in itf.h, or its codegen changes. */
char *MoneyDrawProcessData[2] __attribute__((section(".data"))) = { MoneyDrawProcessDataEntry, NULL };
