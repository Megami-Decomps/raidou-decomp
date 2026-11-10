#include "shared/interface/itfMesManager/decls.h"

/* Release/reset selected-entry glyphs and their status; optionally do the same
 * for primary text. Negative window is a no-op; no other slot validation. */
void itfMesMngRequestMessageRelease(s32 id, s32 cls_speaker) {
    instanceMes_tag *mes;
    message_tag *message;
    Speaker_tag *primaryTextBlock;

    if (id < 0) {
        return;
    }
    mes = pinst[id].mes;
    message = &mes->message;
    primaryTextBlock = &mes->speaker;
    if (message->pfont != NULL) {
        frDestroyFontRequestAfter(message->pfont);
        message->pfont = NULL;
    }
    InitializeMessage(message, 0);
    mes->stat &= ~ITF_MES_ENTRY_STATE_MASK;
    mes->stat &= ITF_MES_ENTRY_STATE_HIGH_CLEAR_MASK;
    if (cls_speaker == 0) {
        return;
    }
    if (primaryTextBlock->pfont != NULL) {
        frDestroyFontRequestAfter(primaryTextBlock->pfont);
        primaryTextBlock->pfont = NULL;
    }
    InitializeSpeaker(primaryTextBlock, 0);
}
