#include "common.h"
#include "kernel.h"
#include "libsdf.h"
#include "sdfDraw.h"
#include "dds3GlobalWork.h"

extern s8 dds3PadButtonState[64];
void SelectSub(s32 *pbase, s32 *pm, s32 list, s32 win, s32 pad, s32 pluskey, s32 minuskey);
void dds3ConsPanelTagLink(sdfDrawTag_t *tag, s32 x, s32 y, s32 sx, s32 sy);
void dds3ConsPanelTagLinkZ(sdfDrawTag_t *tag, s32 x, s32 y, s32 sx, s32 sy, s32 z);

void SelectSub(s32 *pbase, s32 *pm, s32 list, s32 win, s32 pad, s32 pluskey, s32 minuskey) {
    s32 backwardState;
    s32 forwardState;

    if (pm == NULL) {
        return;
    }
    if (list < 2) {
        return;
    }
    backwardState = dds3PadButtonState[minuskey + pad * 16];
    if (backwardState < 0) {
        if (pbase == NULL) {
            if (*pm > 0) {
                (*pm)--;
            } else if (list < win) {
                *pm = list - 1;
            } else {
                *pm = win - 1;
            }
        } else if (*pm > 0) {
            (*pm)--;
        } else if (*pbase > 0) {
            (*pbase)--;
        } else if (win < list) {
            *pbase = list - win;
            *pm = win - 1;
        } else {
            *pm = list - 1;
        }
    } else if (dds3PadButtonState[minuskey + pad * 16] & 2) {
        if (pbase == NULL) {
            if (*pm > 0) {
                (*pm)--;
            }
        } else if (*pm > 0) {
            (*pm)--;
        } else if (*pbase > 0) {
            (*pbase)--;
        }
    }
    forwardState = dds3PadButtonState[pluskey + pad * 16];
    if (forwardState < 0) {
        if (pbase == NULL) {
            if (*pm < win - 1) {
                (*pm)++;
            } else {
                *pm = 0;
            }
        } else if (*pm < win - 1) {
            (*pm)++;
        } else if (*pbase + win < list) {
            (*pbase)++;
        } else {
            *pbase = 0;
            *pm = 0;
        }
    } else if (dds3PadButtonState[pluskey + pad * 16] & 2) {
        if (pbase == NULL) {
            if (*pm < win - 1) {
                (*pm)++;
            }
        } else if (*pm < win - 1) {
            (*pm)++;
        } else if (*pbase + win < list) {
            (*pbase)++;
        }
    }
}

INCLUDE_ASM(const s32, "kernel/dds3KernelMisc", dds3SelectMenu);

void dds3ConsPanelTagLink(sdfDrawTag_t *tag, s32 x, s32 y, s32 sx, s32 sy) {
    s32 columnSpan = 0xC0, rowSpan = 0x60;
    sdfDrawTagLink(tag, EE_ADDR(dds3PrimFrameRect(x * 0x10 + 0x6FD0, y * 8 + 0x78E8, 0xFEFFFF,
                                           sx * columnSpan + rowSpan, sy * rowSpan + 0x30,
                                           0x60000000, 0x40806020)));
}

void dds3ConsPanelTagLinkZ(sdfDrawTag_t *tag, s32 x, s32 y, s32 sx, s32 sy, s32 z) {
    s32 columnSpan = 0xC0, rowSpan = 0x60;
    sdfDrawTagLink(tag, EE_ADDR(dds3PrimFrameRect(x * 0x10 + 0x6FD0, y * 8 + 0x78E8, z,
                                           sx * columnSpan + rowSpan, sy * rowSpan + 0x30,
                                           0x60000000, 0x40806020)));
}
