#include "shared/sdk/libmc/decls.h"

/* Both outputs are generic pointer slots and may alias. */
mcRpcCd *_lmcGetClientPtr(void **reply, void **command)
{
    mcReply *buffer = &mcReplyBuf;

    *reply = buffer;
    *command = &mcLastCmd;
    buffer->sema = mcSema;
    return &mcRpcClient;
}
