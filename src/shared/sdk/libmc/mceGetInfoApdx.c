#include "shared/sdk/libmc/decls.h"

void mceGetInfoApdx(void *response)
{
    s32 *reply = EE_ADDR_AS(s32 *, (EE_ADDR(response) | 0x20000000));

    if (mcReplyDst0) {
        *EE_ADDR_AS(s32 *, mcReplyDst0) = reply[0];
    }
    if (mcReplyDst1) {
        *EE_ADDR_AS(s32 *, mcReplyDst1) = reply[1];
    }
    if (mcReplyDst2) {
        *EE_ADDR_AS(s32 *, mcReplyDst2) = reply[0x90 / sizeof(s32)];
    }
}
