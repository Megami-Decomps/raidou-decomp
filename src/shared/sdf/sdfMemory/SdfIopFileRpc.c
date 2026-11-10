#include "shared/sdf/sdfMemory/decls.h"

/* Pack the two lengths, the terminated name and the optional payload, then
 * return the transport error or the server's result. */
s32 SdfIopFileRpc(const char *aName, s32 aDataSize, const void *aData, s32 *aOutSize)
{
    u8 replyScratch[0x50];
    s32 nameLength = strlen(aName);
    s32 requestBytes = nameLength + aDataSize + SDF_IOP_FILE_RPC_OVERHEAD;
    u32 *request = (u32 *)sdfMemLock(sdfMemAllocDirect(requestBytes));
    u32 *reply;
    s32 result;

    request[0] = nameLength;
    request[1] = aDataSize;
    memcpy(request + 2, aName, nameLength + 1);
    if (aDataSize != 0) {
        memcpy((u8 *)request + nameLength + 9, aData, aDataSize);
    }
    /* The two-word reply has to start on a 64-byte boundary for the SIF. */
    reply = (u32 *)((EE_ADDR(replyScratch) + SDF_IOP_FILE_RPC_REPLY_ALIGN_MASK) &
                    ~SDF_IOP_FILE_RPC_REPLY_ALIGN_MASK);
    result = sceSifMCallRpc((sceSifMClientData *)&sdfIopRpcClient, 1, 0, request, requestBytes, reply, 8, NULL, NULL);
    if (result >= 0) {
        if (aOutSize != NULL) {
            *aOutSize = reply[1];
        }
        result = reply[0];
    }
    return result;
}
