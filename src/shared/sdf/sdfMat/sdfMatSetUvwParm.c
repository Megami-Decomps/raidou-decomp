#include "shared/sdf/sdfMat/decls.h"

void sdfMatSetUvwParm(sdfMatTexTm_t *aUvwPacket, const sdfMatUvwParm_t *aParm)
{
    f32 offsetU;
    f32 offsetV;
    f32 tileU;
    f32 tileV;
    f32 angle;
    f32 s;
    f32 c;
    f32 m0;
    f32 m1;
    f32 m2;
    f32 m3;

    if (aParm == NULL) {
        aUvwPacket->q[0] = 0x000000003F800000ULL;
        aUvwPacket->q[1] = 0x3F80000000000000ULL;
        aUvwPacket->q[2] = 0;
        return;
    }

    angle = -aParm->angle;
    s = sdfSin(angle);
    c = sdfCos(angle);
    offsetU = aParm->offset[0];
    offsetV = aParm->offset[1];
    tileU = aParm->tile[0];
    tileV = aParm->tile[1];

    m0 = c * tileU;
    m1 = s * tileV;
    m2 = -s * tileU;
    m3 = c * tileV;
    aUvwPacket->m[0] = m0;
    aUvwPacket->m[1] = m1;
    aUvwPacket->m[2] = m2;
    aUvwPacket->m[3] = m3;
    aUvwPacket->m[4] = 0.5f - m0 * (offsetU + 0.5f) - m2 * (0.5f - offsetV);
    aUvwPacket->m[5] = 0.5f - m1 * (offsetU + 0.5f) - m3 * (0.5f - offsetV);
}
