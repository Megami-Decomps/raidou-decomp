#include "shared/sdf/sdfMat/decls.h"

sdfMat_t *sdfMatNew(void)
{
    sdfMat_t *mat;
    u32 *packet;
    s32 i;

    sdfMatAllocCount++;
    mat = sdfChipAllocZeroDirect(sizeof(sdfMat_t));
    mat->duty = 0xFF;
    for (i = 0; i != 2; i++) {
        packet = sdfChipAllocDirect(sizeof(sdfMatPacket_t));
        mat->packet[i] = (sdfMatPacket_t *)packet;
        packet[0] = 0x6E05C000;
        packet[0x18 / 4] = 0x6005C005;
        packet[0x30 / 4] = 0;
        packet[0x34 / 4] = 0x640CC00A;
        ((sdfMatPacket_t *)packet)->cmd_call_spec = 0x0400000C;
        ((sdfMatPacket_t *)packet)->cmd_call_kind = 0x14000000;
    }
    mat->fdBias = 0;
    mat->fdSoften = 0;
    mat->ambient = 0x80808080;
    mat->diffuse = 0x80808080;
    mat->multiTexDiffuse = 0x80808080;
    return mat;
}
