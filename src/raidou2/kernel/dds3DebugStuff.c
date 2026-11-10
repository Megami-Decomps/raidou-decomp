#include "common.h"

/* Declarations from smt3-decomp src/smt3/kernel/dds3DebugStuff.c */
#include "common.h"

#include "sdk_libgcc.h"

#include "basic.h"

#include "kernel.h"

#include "libsdf.h"

#include "ps2asm.h"

#include "newdata.h"

#include "field.h"

extern u8 dds3PrimDebugTextBoxAttrs[];

extern char *dds3PrimDebugTextBoxLabels[];

extern char dds3DebugLabelB[], dds3DebugLabelG[], dds3DebugLabelR[], dds3DebugFmtInt3[], dds3DebugFmtFloat3[];

extern void sdfDrawTagLink(sdfDrawTag_t *aTag, addr_t aPacket);

extern void *sdfDevPrintf(s32 aX, s32 aY, s32 aZ, s32 aAttr, const char *aStr, ...);

extern void dds3MenuStepValue(char *text, f32 value);

extern u8 dds3PadButtonState[];

extern void dds3DebugFloatEditEx(f32 *value, u8 *pad, f32 step, f32 min, f32 max, f32 scale);

extern void dds3DebugIntEditEx(void *aValue, s32 aValueSize, s64 aMin, s64 aMax, s64 aSmall, s64 aLarge, s8 *aPadMap);

extern s32 dds3DebugColorEditEx(u32 *aValue, s32 aColumn, s8 *aPadMap);

u64 *dds3PrimFrameRect(s32 aX, s32 aY, s32 aZ, s32 aW, s32 aH, u32 aBgCol, u32 aFrameCol);

extern void rsetOtOfs(s32 otofs);

extern void rsetAlpha(s32 mode);

extern f32 sdfSin(f32 aRadius);

extern f32 sdfCos(f32 aRadius);

extern void fldDrawPoly3(s32 c0, s32 c1, s32 c2, f32 x0, f32 y0, f32 z0, f32 x1, f32 y1, f32 z1,
                         f32 x2, f32 y2, f32 z2);

void dds3DebugFloatEditEx(f32 *controlledValue, u8 *padState, f32 minimum, f32 maximum, f32 step, f32 bigStep);

void dds3DebugFloatEdit(f32 *value, f32 step, f32 min, f32 max, f32 scale);

void dds3DebugIntEditEx(void *aValue, s32 aValueSize, s64 aMin, s64 aMax, s64 aSmall, s64 aLarge, s8 *aPadMap);

void dds3DebugIntEdit(void *aValue, s32 aValueSize, s64 aMin, s64 aMax, s64 aSmall, s64 aLarge);

s32 dds3DebugColorEditEx(u32 *aValue, s32 aColumn, s8 *aPadMap);

void dds3DebugColorEdit(u32 *aValue, s32 aColumn);

void dds3MenuStepValue(char *out, f32 value);

void dds3PrimDebugTextBox(void *packetList, s32 x, s32 y, s32 selected, f32 *values);

void dds3PrimDebugTextBoxRGB(void *packetList, s32 x, s32 y, s32 selected,
                   u32 color, s32 showNormalized);

u32 dds3NullDrawProc(void);

void dds3DrawUnitObjectShadow(s32 fade, f32 x, f32 y, f32 z, f32 radius);

extern char dds3DebugLabelB[8];

extern char dds3DebugLabelG[8];

extern char dds3DebugLabelR[8];

extern char dds3DebugFmtInt3[8];

extern char dds3DebugFmtFloat3[8];

extern f32 fldCamDirLock;

extern f32 fldCamPitch;

extern f32 fldCamLength;
/* End of declarations from src/smt3/kernel/dds3DebugStuff.c */

void dds3DebugFloatEditEx(f32 *controlledValue, u8 *padState, f32 minimum, f32 maximum, f32 step, f32 bigStep) {
    f32 adjustedValue = *controlledValue;

    if ((s8)padState[5] & 0x80) {
        adjustedValue += bigStep;
        if (maximum < adjustedValue) {
            adjustedValue = minimum;
        }
    } else if (padState[5] & 2) {
        adjustedValue += bigStep;
        if (maximum < adjustedValue) {
            adjustedValue = maximum;
        }
    } else if ((s8)padState[4] & 0x80) {
        adjustedValue -= bigStep;
        if (adjustedValue < minimum) {
            adjustedValue = maximum;
        }
    } else if (padState[4] & 2) {

        adjustedValue -= bigStep;
        if (adjustedValue < minimum) {
            adjustedValue = minimum;
        }
    } else if ((s8)padState[7] & 0x80) {
        adjustedValue += step;
        if (maximum < adjustedValue) {
            adjustedValue = minimum;
        }
    } else if (padState[7] & 2) {
        adjustedValue += step;
        if (maximum < adjustedValue) {
            adjustedValue = maximum;
        }
    } else if ((s8)padState[6] & 0x80) {
        adjustedValue -= step;
        if (adjustedValue < minimum) {
            adjustedValue = maximum;
        }
    } else if (padState[6] & 2) {
        adjustedValue -= step;
        if (adjustedValue < minimum) {
            adjustedValue = minimum;
        }
    } else {
        return;
    }
    *controlledValue = adjustedValue;
}

INCLUDE_ASM(const s32, "kernel/dds3DebugStuff", dds3DebugFloatEdit);

void dds3DebugIntEditEx(void *aValue, s32 aValueSize, s64 aMin, s64 aMax, s64 aSmall, s64 aLarge, s8 *aPadMap) {
    s64 adjustedValue;
    switch (aValueSize) {
    case 1:
        adjustedValue = *(u8 *)aValue;
        break;
    case 2:
        adjustedValue = *(u16 *)aValue;
        break;
    case 4:
        adjustedValue = *(u32 *)aValue;
        break;
    case -1:
        adjustedValue = *(s8 *)aValue;
        break;
    case -2:
        adjustedValue = *(s16 *)aValue;
        break;
    case -4:
        adjustedValue = *(s32 *)aValue;
        break;
    default:
        return;
    }
    if (aPadMap[5] & 0x80) {
        adjustedValue += aLarge;
        if (aMax < adjustedValue) {
            adjustedValue = aMin;
        }
    } else if (aPadMap[5] & 2) {
        adjustedValue += aLarge;
        if (aMax < adjustedValue) {
            adjustedValue = aMax;
        }
    } else if (aPadMap[4] & 0x80) {
        adjustedValue -= aLarge;
        if (adjustedValue < aMin) {
            adjustedValue = aMax;
        }
    } else if (aPadMap[4] & 2) {
        adjustedValue -= aLarge;
        if (adjustedValue < aMin) {
            adjustedValue = aMin;
        }
    } else if (aPadMap[7] & 0x80) {
        adjustedValue += aSmall;
        if (aMax < adjustedValue) {
            adjustedValue = aMin;
        }
    } else if (aPadMap[7] & 2) {
        adjustedValue += aSmall;
        if (aMax < adjustedValue) {
            adjustedValue = aMax;
        }
    } else if (aPadMap[6] & 0x80) {
        adjustedValue -= aSmall;
        if (adjustedValue < aMin) {
            adjustedValue = aMax;

        }
    } else if (aPadMap[6] & 2) {
        adjustedValue -= aSmall;
        if (adjustedValue < aMin) {
            adjustedValue = aMin;
        }
    } else {
        return;
    }
    switch (aValueSize) {
    case -1:
    case 1:
        *(u8 *)aValue = adjustedValue;
        break;
    case -2:
    case 2:
        *(u16 *)aValue = adjustedValue;
        break;
    case -4:
    case 4:
        *(u32 *)aValue = adjustedValue;
        break;
    }
}

INCLUDE_ASM(const s32, "kernel/dds3DebugStuff", dds3DebugIntEdit);

s32 dds3DebugColorEditEx(u32 *aValue, s32 aColumn, s8 *aPadMap) {
    s32 originalColor = *aValue;
    s32 channelValue = originalColor;
    s32 updatedColor;
    switch (aColumn) {
    case 0:
        break;
    case 1:
        channelValue = originalColor >> 8;
        break;
    case 2:
        channelValue = originalColor >> 16;
        break;
    case 3:
        channelValue = originalColor >> 24;
        break;
    }
    channelValue &= 0xFF;
    if (((aPadMap[5] & 0x80) || (aPadMap[7] & 0x80)) && channelValue == 0xFF) {
        channelValue = 0;
    } else if (aPadMap[5] & 2) {
        channelValue += 10;
        if (channelValue >= 0x100) {
            channelValue = 0xFF;
        }
    } else if (((aPadMap[4] & 0x80) || (aPadMap[6] & 0x80)) && channelValue == 0) {
        channelValue = 0xFF;
    } else if (aPadMap[4] & 2) {
        channelValue -= 10;
        if (channelValue < 0) {
            channelValue = 0;
        }
    } else if (aPadMap[7] & 2) {
        channelValue += 1;
        if (channelValue >= 0x100) {
            channelValue = 0xFF;
        }
    } else if (aPadMap[6] & 2) {
        channelValue -= 1;
        if (channelValue < 0) {
            channelValue = 0;
        }
    } else {
        return 0;
    }
    switch (aColumn) {
    case 0:
        updatedColor = (originalColor & 0xFFFFFF00) | channelValue;
        break;
    case 1:
        updatedColor = (originalColor & 0xFFFF00FF) | (channelValue << 8);
        break;
    case 2:
        updatedColor = (originalColor & 0xFF00FFFF) | (channelValue << 16);
        break;
    default:
        updatedColor = (originalColor & 0x00FFFFFF) | (channelValue << 24);
        break;
    }
    *aValue = updatedColor;
    return updatedColor != originalColor;
}

INCLUDE_ASM(const s32, "kernel/dds3DebugStuff", dds3DebugColorEdit);

void dds3MenuStepValue(char *out, f32 value)
{
    s32 i;
    s32 f1;
    s32 f2;

    value += 0.005f;
    i = (s32)value;
    if (i >= 10) {
        f1 = 9;
        f2 = 9;
    } else {
        value -= (f32)i;
        value *= 10.0f;
        f1 = (s32)value;
        value -= (f32)f1;
        f2 = (s32)(value * 10.0f);
    }
    out[0] = i + '0';
    out[1] = '.';
    out[2] = f1 + '0';
    out[3] = f2 + '0';
    out[4] = '\0';
}

void dds3PrimDebugTextBox(void *packetList, s32 x, s32 y, s32 selected, f32 *values) {
    char text[16];
    u32 color = 0x80000000;
    s32 i;
    s32 style;

    if (values != NULL) {
        color = 0;
        for (i = 0; i != 3; i++) {
            s32 channel = values[i] * 128.0f;
            if (channel > 0) {
                if (channel >= 256) {
                    channel = 255;
                }
                color |= channel << (i * 8);
            }
        }
        color |= 0x80000000;
    }
    sdfDrawTagLink(packetList, EE_ADDR(dds3PrimFrameRect(x + 0x510, y + 0x18,
                     0xFF0080, 0x240, 0xF0, color, 0x40806020)));
    for (i = 0; i != 3; i++) {
        sdfDrawTagLink(packetList, EE_ADDR(sdfDevPrintf(x, y,
                        0xFF0080, dds3PrimDebugTextBoxAttrs[i], dds3PrimDebugTextBoxLabels[i])));
        style = selected == i ? 6 : 0;
        if (values == NULL) {
            text[0] = '-';
            text[1] = '-';
            text[2] = '-';
            text[3] = '-';
            text[4] = 0;
        } else {
            dds3MenuStepValue(text, values[i]);
        }
        sdfDrawTagLink(packetList, EE_ADDR(sdfDevPrintf(x + 0x180,
                        y, 0xFF0080, style, text)));
        y += 0x60;
    }
}

void dds3PrimDebugTextBoxRGB(void *packetList, s32 x, s32 y, s32 selected,
                   u32 color, s32 showNormalized) {
    s32 i;
    s32 style;
    s32 channel;
    const char *label;

    color = (color & 0xFFFFFF) | 0x80000000;
    sdfDrawTagLink(packetList, EE_ADDR(dds3PrimFrameRect(x + 0x510, y + 0x18,
                    0xFF0080, 0x240, 0xF0, color, 0x40806020)));
    for (i = 0; i != 3; i++) {
        switch (i) {
        case 0:
            style = 2;
            label = dds3DebugLabelR;
            channel = color & 0xFF;
            break;
        case 1:
            style = 4;
            label = dds3DebugLabelG;
            channel = (color >> 8) & 0xFF;
            break;
        default:
            style = 5;
            label = dds3DebugLabelB;
            channel = (color >> 16) & 0xFF;
            break;
        }
        sdfDrawTagLink(packetList, EE_ADDR(sdfDevPrintf(
                        x, y, 0xFF0080, style, label)));
        sdfDrawTagLink(packetList, EE_ADDR(sdfDevPrintf(
                        x + 0x180, y, 0xFF0080, i == selected ? 6 : 0,
                        dds3DebugFmtInt3, channel)));
        if (showNormalized != 0) {
            sdfDrawTagLink(packetList, EE_ADDR(sdfDevPrintf(
                            x, y + 0x180, 0xFF0080, style, label)));
            sdfDrawTagLink(packetList, EE_ADDR(sdfDevPrintf(
                            x + 0x240, y + 0x180, 0xFF0080, 0,
                            dds3DebugFmtFloat3, fptodp(channel * (1.0f / 255.0f)))));
        }
        y += 0x60;
    }
}
INCLUDE_SDATA(const s32, "kernel/dds3DebugStuff", D_004DE6E8);

INCLUDE_SDATA(const s32, "kernel/dds3DebugStuff", D_004DE6F0);

INCLUDE_SDATA(const s32, "kernel/dds3DebugStuff", D_004DE6F8);

INCLUDE_SDATA(const s32, "kernel/dds3DebugStuff", D_004DE700);

INCLUDE_SDATA(const s32, "kernel/dds3DebugStuff", D_004DE708);

