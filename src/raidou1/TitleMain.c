#include "common.h"
#include "libsdf.h"
#include "kernel.h"
#include "basic.h"
#include "event.h"
#include "scr.h"
#include "dds3SceneBasic.h"
#include "dds3KernelCore.h"
#include "TitleMain.h"
#include "code_r1_002CA3F0.h"
#include "cfg.h"
#include "dds3PadManager.h"
#include "sdfSound.h"
#include "sdk_libc.h"
#include "lmap.h"

extern titleSpritePlacement_t evtLeaf31Data[];

/* body: matched in store @ 2026-11-16: /home/wsl/.local/state/raidou-decomp-work/clusters/store/raidou1/002F5470.c:1, match */
s32 evtLeaf29(void) {
    return NpcDelete();
}

s32 evtLeaf30(s32 value, s32 low, s32 high) {
    if (value >= low && value < high) {
        return 1;
    }
    return 0;
}

s32 evtFunc9(s32 frame, const titleKey_t *key, s32 count) {
    s32 i;

    for (i = 0; i < count - 1; i++) {
        if (evtLeaf30(frame, key[i].frame, key[i + 1].frame)) {
            break;
        }
    }
    frame -= key[i].frame;
#ifdef TITLE_KEY_STOP_AT_LAST
    /* Outside the keys retail divides by key[count].frame - key[i].frame, read
     * past the table from the caller's stack, which a host stack can make zero. */
    if (i == count - 1) {
        return key[i].value * 2;
    }
#endif
    return (key[i].value + (key[i + 1].value - key[i].value) * frame / (key[i + 1].frame - key[i].frame)) * 2;
}

/* NON_MATCHING: 59 of 68 words match.  */
#ifdef NON_MATCHING
void evtLeaf31(s32 slot, s32 value, const f32 *size, const f32 *scale, void **sprite) {
    sprHandle_t *set;
    s32 slotIndex;
    s32 x;
    s32 y;
    itfBdWork_t *work;
    f32 width;
    f32 height;
    f32 scaledWidth;
    f32 scaledHeight;

    set = sprite[evtLeaf31Data[slot].spriteIndex];
    slotIndex = evtLeaf31Data[slot].slotIndex;
    x = evtLeaf31Data[slot].x;
    y = evtLeaf31Data[slot].y;
    work = &set->workEntries[slotIndex];
    width = (s32)((f32)(work->sourceWidth * 16) * size[0]);
    height = (s32)((f32)(work->sourceHeight * 8) * size[1]);
    scaledWidth = width * scale[0];
    scaledHeight = height * scale[1];
    work->geometry.bounds[2] = scaledWidth;
    work->geometry.bounds[3] = scaledHeight;
    effDrawSpriteWithCornerFade(x + (s32)((width - scaledWidth) * 0.5f), y + (s32)((height - scaledHeight) * 0.5f), 0,
                                value, 0, set, slotIndex, 79);
}
#else
INCLUDE_ASM(const s32, "TitleMain", evtLeaf31);
#endif

void evtFunc10(s32 slot, s32 value, void **sprite) {
    f32 scale[2] = { 1.0f, 1.0f };

    evtLeaf31(slot, value, scale, scale, sprite);
}

void TitleInitCursor(titleCursor_t *cursor) {
    memset(cursor, 0, sizeof(titleCursor_t));
    cursor->ScaleTimer = 6;
}

INCLUDE_ASM(const s32, "TitleMain", evtTitleDLCInitSub);

s32 TitleCalcCursor(titleCursor_t *pCursor) {
    s32 ret = 0;
    u32 sel;
    u32 i;

    if (pCursor->Index != 3) {
        if (dds3PadButtonState[38] & 2) {
            ret = 1;
            if (pCursor->Index != 0 || !pCursor->LastPush) {
                pCursor->Timer = 0;
                pCursor->ScaleTimer = 6;
                pCursor->Index--;
                p2sdPlaySE(0x500001, 127, 63);
            }
            if (pCursor->Index < 0) {
                pCursor->Index = 2;
            }
        }
        if (dds3PadButtonState[39] & 2) {
            ret = 1;
            if (pCursor->Index != 2 || !pCursor->LastPush) {
                pCursor->Timer = 0;
                pCursor->ScaleTimer = 6;
                pCursor->Index++;
                p2sdPlaySE(0x500001, 127, 63);
            }
            if (pCursor->Index >= 3) {
                pCursor->Index = 0;
            }
        }
        if (dds3PadButtonState[38] != 0 || dds3PadButtonState[39] != 0) {
            pCursor->LastPush = 1;
        } else {
            pCursor->LastPush = 0;
        }
        sel = pCursor->Index;
    } else {
        if (dds3PadButtonState[37] < 0) {
            if (++pCursor->ConfigIndex >= 2) {
                pCursor->ConfigIndex = 0;
            }
            p2sdPlaySE(0x500001, 127, 63);
        }
        if (dds3PadButtonState[36] < 0) {
            if (--pCursor->ConfigIndex < 0) {
                pCursor->ConfigIndex = 1;
            }
            p2sdPlaySE(0x500001, 127, 63);
        }
        if (pCursor->ConfigIndex == 0) {
            dds3RefDDS3GBWK2(0, 1);
        } else {
            dds3RefDDS3GBWK2(0, 0);
        }
        sel = pCursor->Index + pCursor->ConfigIndex;
    }

    for (i = 0; i < 5; i++) {
        if (i == sel) {
            s32 up = pCursor->ItemAlpha[i];

            if (up < 128) {
                pCursor->ItemAlpha[i] = up + 32;
                up = pCursor->ItemAlpha[i];
            }
            if (up > 128) {
                pCursor->ItemAlpha[i] = 128;
            }
        } else {
            s32 down = pCursor->ItemAlpha[i];

            if (down > 0) {
                pCursor->ItemAlpha[i] = down - 32;
                down = pCursor->ItemAlpha[i];
            }
            if (down < 0) {
                pCursor->ItemAlpha[i] = 0;
            }
        }
    }
    if (pCursor->ScaleTimer > 0) {
        pCursor->ScaleTimer--;
    }
    return ret;
}
