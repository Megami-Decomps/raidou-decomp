#include "shared/interface/frFont/decls.h"

/* Advance by eight times cursor spacing and request both context updates. */
void frMessageReturnCode(frMsgInfo_t *pMsgInfo) {
    pMsgInfo->Y += gMessageStepY * FR_FONT_CONTEXT_CURSOR_SCALE;
    pMsgInfo->FrqFlag = 1;
    pMsgInfo->RetFlag = 1;
}
