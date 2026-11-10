#include "shared/interface/frFont/decls.h"

frFontHandle_t *MessageRequestEx(s32 X, s32 Y, s32 Style, s32 ColNo, s32 Tp, s32 Spd, u8 *pMsg, frFontHandle_t *pFRQ) {
    return MessageRequestEx2(X, Y, 0, Style & 0xFF, ColNo & 0xFF, Tp & 0xFF, Spd & 0xFF, pMsg, pFRQ);
}
