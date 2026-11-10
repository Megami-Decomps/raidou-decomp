#include "shared/sdk/libmc/decls.h"

s32 sceMcWrite(s32 a, const void *b, s32 c)
{
    s32 ret;
    u32 i;

    if (PollSema(mcSema) < 0) {
        return -200;
    }
    if (mcRpcClient.bound == 0) {
        SignalSema(mcSema);
        return -100;
    }
    mcRequest.write.descriptor = a;
    if (c < 17) {
        mcRequest.write.inlineSize = c;
        mcRequest.write.bulkSize = 0;
        mcRequest.write.bulkBuffer = 0;
    } else {
        /* The IOP side wants the bulk transfer 16-byte aligned, so the leading
         * bytes up to the next boundary travel inside the request itself. */
        addr_t top = (EE_ADDR(b) - 1) & ~15;
        addr_t bot = EE_ADDR(b) - 16;
        s32 head = top - bot;

        mcRequest.write.inlineSize = head;
        mcRequest.write.bulkSize = c - head;
        mcRequest.write.bulkBuffer = (const u8 *)b + head;
    }
    for (i = 0; i < mcRequest.write.inlineSize; i++) {
        mcRequest.write.inlineData[i] = ((const u8 *)b)[i];
    }
    FlushCache(0);
    ret = sceSifCallRpc(&mcRpcClient, 6, 1, &mcRequest, 48, &mcReplyBuf, 4, 0, 0);
    if (ret == 0) {
        mcLastCmd = 6;
    } else {
        SignalSema(mcSema);
    }
    return ret;
}
