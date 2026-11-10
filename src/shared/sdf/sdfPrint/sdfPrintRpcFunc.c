#include "shared/sdf/sdfPrint/decls.h"

/* Copy a positive source/end span into the RPC reply buffer; otherwise return NULL. */
void *sdfPrintRpcFunc(s32 unused, sifCommand_t *packet) {
    s32 copyBytes = packet->end - packet->source;
    if (copyBytes > 0) {
        memcpy(packet, EE_ADDR_AS(void *, packet->source), copyBytes);
        return packet;
    }
    return NULL;
}
