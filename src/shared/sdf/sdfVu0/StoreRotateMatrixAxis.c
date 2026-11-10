#include "shared/sdf/sdfVu0/decls.h"

void StoreRotateMatrixAxis(f32 aAngle, const sdfVector3_t *aAxis, sdfMatrix4_t *aMtx) {
    sdfVector4_t normalizedAxis;
    f32 x;
    f32 y;
    f32 z;
    f32 cosine = cosf(aAngle);
    f32 sine = sinf(aAngle);

    SDF_VU0_LOADVECTOR(aAxis);
    SDF_VU0_NORMALIZE();
    SDF_VU0_STOREVECTOR(&normalizedAxis);

    x = normalizedAxis.x;
    y = normalizedAxis.y;
    z = normalizedAxis.z;

    aMtx->m[0] = x * x + (1.0f - x * x) * cosine;
    aMtx->m[1] = x * y * (1.0f - cosine) + z * sine;
    aMtx->m[2] = x * z * (1.0f - cosine) - y * sine;
    aMtx->m[3] = 0.0f;
    aMtx->m[4] = x * y * (1.0f - cosine) - z * sine;
    aMtx->m[5] = y * y + (1.0f - y * y) * cosine;
    aMtx->m[6] = y * z * (1.0f - cosine) + x * sine;
    aMtx->m[7] = 0.0f;
    aMtx->m[8] = x * z * (1.0f - cosine) + y * sine;
    aMtx->m[9] = y * z * (1.0f - cosine) - x * sine;
    aMtx->m[10] = z * z + (1.0f - z * z) * cosine;
    aMtx->m[11] = 0.0f;
    SDF_VU0_STOREIDENTITYVECTOR(&aMtx->m[12]);
}
