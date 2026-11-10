#include "shared/model/mdlManager/decls.h"

s32 mdlPackCheck(sdfPacDecodeResrc_t *resrc)
{
    s32 result;

    result = 0;
    if (((sdfPacStdBlockHeader_t *)resrc->header)->fileType == DDS3_FT_EFFECT)
    {
        result = ((sdfPacStdBlockHeader_t *)resrc->header)->magicCode == PIB_TAG_PIB0;
    }
    return result;
}
