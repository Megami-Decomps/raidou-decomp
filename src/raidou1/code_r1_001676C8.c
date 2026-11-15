#include "common.h"
#include "frFont.h"
#include "EventBit.h"

extern const char mapData40[];
extern const char mapData41[];
extern const char mapData42[];
extern const char mapData43[];
extern void mapIfDrawMskSub(s32 width);
extern void mapIfDrawActionWin2(s32 x, s32 y, u32 color, const char *str);

/* NON_MATCHING: 13 of 66 words match. The call tail is laid out differently. */
#ifdef NON_MATCHING
void mapIfDrawMsk(void) {
    const char *str;

    if (!evtBitCheck(0x360)) {
        if (evtBitCheck(0x8E2)) {
            str = mapData40;
            goto draw;
        }
        if (evtBitCheck(0x8E3)) {
            str = mapData41;
            goto draw;
        }
        str = mapData42;
        if (evtBitCheck(0x8F3)) {
draw:
            mapIfDrawMskSub(frPreCalcStrWidth(str, 1, 1) + 5);
            mapIfDrawActionWin2(0x2C, 0x194, 0xA09DC380, str);
        } else {
            str = mapData43;
            if (evtBitCheck(0x8F4)) {
                mapIfDrawMskSub(frPreCalcStrWidth(str, 1, 1) + 5);
                mapIfDrawActionWin2(0x2C, 0x194, 0xA09DC380, str);
            }
        }
    }
}
#else
INCLUDE_ASM(const s32, "code_r1_001676C8", mapIfDrawMsk);
#endif
