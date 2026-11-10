#include "shared/interface/frFont/decls.h"

frFontHandle_t *MessageRequest(s32 X, s32 Y, u8 *pMsg, frFontHandle_t *pFRQ) {
    return MessageRequestEx2(X, Y, 0, 0, 0, 0, 0x80, pMsg, pFRQ);
}
