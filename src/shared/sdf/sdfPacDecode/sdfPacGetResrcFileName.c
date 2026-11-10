#include "shared/sdf/sdfPacDecode/decls.h"

char *sdfPacGetResrcFileName(sdfPacDecodeResrc_t *resrc)
{
    sdfPacStdBlockHeader_t *packet = (sdfPacStdBlockHeader_t *)resrc->header;
    s32 extra = packet->flags & SDF_PAC_EXTRA_MASK;

    if (extra <= 0) {
        return NULL;
    }
    return (char *)(packet + 1);
}
