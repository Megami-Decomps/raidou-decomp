#include "shared/sdf/sdfPacDecode/decls.h"

s32 sdfPacDecode(sdfPacDecodeCb_t *cb, const u8 *data, s32 length)
{
    cb->srcData = data;
    cb->srcLength = length;
    while (cb->srcLength > 0) {
        switch ((s8)cb->stat) {
        case -1:
            return -1;
        case 0:
            sdfPacNewBlock(cb);
            break;
        case 1: {
            s32 len = cb->rest;

            if (cb->srcLength < len) {
                len = cb->srcLength;
            }
            sdfPacUseSrc(cb, len);
            cb->rest -= len;
            if (cb->rest == 0) {
                cb->stat = 0;
            }
            break;
        }
        case 2:
            cb->decoder(cb);
            break;
        }
    }
    return (s8)cb->stat;
}
