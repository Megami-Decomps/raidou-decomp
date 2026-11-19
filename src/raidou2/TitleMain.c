#include "common.h"
#include "TitleMain.h"
#include "cfg.h"
#include "dds3PadManager.h"
#include "sdfSound.h"
#include "sdk_libc.h"

INCLUDE_ASM(const s32, "TitleMain", LMP_NPC_DELETE);

INCLUDE_ASM(const s32, "TitleMain", dds3CallListStart);

INCLUDE_ASM(const s32, "TitleMain", dds3Func2);

INCLUDE_ASM(const s32, "TitleMain", dds3CallWorkActive);

INCLUDE_ASM(const s32, "TitleMain", dds3CallMcPollMkdir);

INCLUDE_ASM(const s32, "TitleMain", dds3Leaf2);

INCLUDE_ASM(const s32, "TitleMain", dds3Func3);

INCLUDE_ASM(const s32, "TitleMain", dds3CallEffectDirClose);

INCLUDE_ASM(const s32, "TitleMain", dds3Leaf3);

INCLUDE_ASM(const s32, "TitleMain", dds3Leaf4);

INCLUDE_ASM(const s32, "TitleMain", dds3Leaf5);

INCLUDE_ASM(const s32, "TitleMain", dds3CtrlActionWindowLmapSub);

INCLUDE_ASM(const s32, "TitleMain", dds3Func4);

INCLUDE_ASM(const s32, "TitleMain", dds3Func5);

INCLUDE_ASM(const s32, "TitleMain", dds3Func6);

INCLUDE_ASM(const s32, "TitleMain", dds3Func7);

INCLUDE_ASM(const s32, "TitleMain", dds3Func8);

INCLUDE_ASM(const s32, "TitleMain", dds3Leaf6);

INCLUDE_ASM(const s32, "TitleMain", dds3EffectDirClose);

INCLUDE_ASM(const s32, "TitleMain", dds3Func9);

INCLUDE_ASM(const s32, "TitleMain", dds3Leaf7);

INCLUDE_ASM(const s32, "TitleMain", dds3Func10);

INCLUDE_ASM(const s32, "TitleMain", dds3Leaf8);

INCLUDE_ASM(const s32, "TitleMain", dds3Func11);

INCLUDE_ASM(const s32, "TitleMain", dds3Leaf9);

INCLUDE_ASM(const s32, "TitleMain", dds3Leaf10);

INCLUDE_ASM(const s32, "TitleMain", dds3Func12);

INCLUDE_ASM(const s32, "TitleMain", dds3Leaf11);

INCLUDE_ASM(const s32, "TitleMain", dds3Leaf12);

INCLUDE_ASM(const s32, "TitleMain", dds3Leaf13);

INCLUDE_ASM(const s32, "TitleMain", dds3Leaf14);

INCLUDE_ASM(const s32, "TitleMain", mselFunc);

INCLUDE_ASM(const s32, "TitleMain", mselLmapSrcCM);

INCLUDE_ASM(const s32, "TitleMain", mselLeaf);

INCLUDE_ASM(const s32, "TitleMain", mselLeaf2);

INCLUDE_ASM(const s32, "TitleMain", mselCallSetRotateMatrixX);

INCLUDE_ASM(const s32, "TitleMain", mselCallSetRotateMatrixY);

INCLUDE_ASM(const s32, "TitleMain", mselCallSetRotateMatrixZ);

INCLUDE_ASM(const s32, "TitleMain", mselLeaf3);

INCLUDE_ASM(const s32, "TitleMain", mselLeaf4);

INCLUDE_ASM(const s32, "TitleMain", mselLeaf5);

INCLUDE_ASM(const s32, "TitleMain", mselLmapSrcCM2);

INCLUDE_ASM(const s32, "TitleMain", mselDrawSaveNum);

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
    return (key[i].value + (key[i + 1].value - key[i].value) * frame / (key[i + 1].frame - key[i].frame)) * 2;
}

INCLUDE_ASM(const s32, "TitleMain", evtLeaf31);

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
