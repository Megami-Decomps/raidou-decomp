#include "shared/sdf/sdfTmx/decls.h"

void sdfFileRpcContinueRead(void) {
    s32 chunkBytes = sdfFileRpcState.remainingBytes;

    if (chunkBytes > SDF_STREAM_CHUNK_BYTES) {
        chunkBytes = SDF_STREAM_CHUNK_BYTES;
    }
    sdfFileCmdRead(sdfFileRpcState.deviceState, sdfFileRpcReadBuf, chunkBytes);
}
