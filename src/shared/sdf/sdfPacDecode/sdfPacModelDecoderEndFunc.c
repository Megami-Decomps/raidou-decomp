#include "shared/sdf/sdfPacDecode/decls.h"

void sdfPacModelDecoderEndFunc(sdfPacDecodeCb_t *cb)
{
    sdfPacDecodeResrc_t *aWork = cb->resrcTail;
    sdfPacReloc_t *reloc = (sdfPacReloc_t *)aWork->data;
    u8 *payload = reloc->payload;
    s32 bytes = reloc->tableBytes;

    aWork->data = payload;
    if (bytes != 0) {
        sdfAddressResolver((s32 *)payload, EE_ADDR_S(payload), payload + reloc->tableOffset, bytes);
        reloc->tableBytes = 0;
    }
    sdfPacEndBlock(cb);
}
