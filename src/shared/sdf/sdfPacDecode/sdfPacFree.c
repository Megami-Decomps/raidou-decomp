#include "shared/sdf/sdfPacDecode/decls.h"

void sdfPacFree(sdfPacDecodeCb_t *cb)
{
    sdfPacDecodeResrc_t *cursor = cb->resrcHead;
    sdfPacDecodeResrc_t *current = cursor;

    while (cursor != NULL) {
        cursor = cursor->next;
        sdfChipFreeDirect(current);
        current = cursor;
    }
}
