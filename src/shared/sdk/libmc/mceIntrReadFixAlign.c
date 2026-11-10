#include "shared/sdk/libmc/decls.h"

void mceIntrReadFixAlign(void *argument)
{
    mcReadReplyPrefix_t *reply = EE_ADDR_AS(mcReadReplyPrefix_t *, (EE_ADDR(argument) | 0x20000000));
    u8 *destination;
    s32 i;

    if (reply->firstLength != 0) {
        destination = reply->firstDestination;
        for (i = 0; i < reply->firstLength; i++) {
            *destination++ = reply->firstData[i];
        }
    }
    if (reply->secondLength != 0) {
        destination = reply->secondDestination;
        for (i = 0; i < reply->secondLength; i++) {
            *destination++ = reply->secondData[i];
        }
    }
}
