#include "shared/sdk/libkernl/decls.h"

s32 sceTtyRead(char *buffer, s32 length)
{
    s32 count;

    for (count = 0; count < length; count++) {
        while (ttyState.queue->count == 0) {
        }
        buffer[count] = *ttyState.queue->read;
        QueuePeekReadDone(ttyState.queue);
        if (buffer[count] == 10 || buffer[count] == 13) {
            return count + 1;
        }
    }
    return count;
}
