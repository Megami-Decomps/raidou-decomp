#include "shared/sdk/libcdvd/decls.h"

void _sceCd_cd_read_intr(void *argument)
{
    cdvdReadReplyPrefix_t *reply = EE_ADDR_AS(cdvdReadReplyPrefix_t *, (EE_ADDR(argument) | 0x20000000));
    u8 *destination;
    s32 i;

    if (reply->firstLength > 0) {
        destination = reply->firstDestination;
        for (i = 0; i < reply->firstLength; i++) {
            destination[i] = reply->firstData[i];
        }
    }
    if (reply->secondLength > 0) {
        destination = reply->secondDestination;
        for (i = 0; i < reply->secondLength; i++) {
            destination[i] = reply->secondData[i];
        }
    }
    _sceCd_cd_callback(&cdvdCallbackReason);
}
