#include "shared/sdf/sdfPacDecode/decls.h"

s32 sdfPacStdCallback(sdfPacDecodeCb_t *cb, s32 cause, sdfPacStdBlockHeader_t *hdr)
{
    if (cause == 0) {
        switch (hdr->fileType) {
        case DDS3_FT_EFFECT:
            sdfPacMemDecoderInit(cb, hdr);
            return 0;
        case DDS3_FT_TMX:
            sdfPacTmxDecoderInit(cb, hdr);
            return 0;
        case DDS3_FT_MODEL:
            sdfPacModelDecoderInit(cb, hdr);
            return 0;
        case DDS3_FT_MOTION:
            sdfPacMotionDecoderInit(cb, hdr);
            return 0;
        case DDS3_FT_TEXLIST:
            sdfPacTexListDecoderInit(cb, hdr);
            return 0;
        case DDS3_FT_END:
            return 1;
        default:
            return 3;
        }
    }
    return 0;
}
