#include "shared/sdf/sdfPacDecode/decls.h"

sdfPacDecodeResrc_t *sdfPacFreeResrc(sdfPacDecodeResrc_t *resrc)
{
    sdfPacDecodeCb_t *aState = resrc->cb;
    sdfPacDecodeResrc_t **link = &aState->resrcHead;
    sdfPacDecodeResrc_t *current = aState->resrcHead;
    sdfPacDecodeResrc_t *previous = NULL;
    sdfPacDecodeResrc_t *next;

    if (current != resrc) {
        do {
            previous = current;
            current = previous->next;
            link = &previous->next;
        } while (current != resrc);
    }
    next = resrc->next;
    *link = next;
    if (aState->resrcTail == resrc) {
        aState->resrcTail = previous;
    }
    sdfChipFreeDirect(resrc);
    return next;
}
