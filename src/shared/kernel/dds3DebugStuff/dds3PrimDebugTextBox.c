#include "shared/kernel/dds3DebugStuff/decls.h"

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
