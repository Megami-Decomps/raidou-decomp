#include "shared/kernel/dds3KernelMisc/decls.h"

void dds3ConsPanelTagLink(sdfDrawTag_t *tag, s32 x, s32 y, s32 sx, s32 sy) {
    s32 columnSpan = 0xC0, rowSpan = 0x60;
    sdfDrawTagLink(tag, EE_ADDR(dds3PrimFrameRect(x * 0x10 + 0x6FD0, y * 8 + 0x78E8, 0xFEFFFF,
                                           sx * columnSpan + rowSpan, sy * rowSpan + 0x30,
                                           0x60000000, 0x40806020)));
}
