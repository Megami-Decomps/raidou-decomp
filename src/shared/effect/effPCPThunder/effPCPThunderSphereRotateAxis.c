#include "shared/effect/effPCPThunder/decls.h"

void effPCPThunderSphereRotateAxis(effPCPThunderSpherePoly_t *work, s32 index) {
    f32 axis[4] __attribute__((aligned(16)));
    f32 direction[4] __attribute__((aligned(16)));
    parSystem_t *system = work->hPoly;
    effPCPThunderSpherePolyPart_t *source = &work->pPart[index];
    parCell_t *cell = &system->cells[index];
    u128 *vertices = cell->history;
    s32 rows = system->vertexWordCount / 5;
    s32 i;
    f32 sample;
    f32 angle;

    sample = dds3GetRandFloatA(dds3EffectBase);
    angle = source->rotate * sample * 3.0f;
    direction[0] = source->dir[0];
    direction[1] = 0.0f;
    direction[2] = source->dir[2];
    axis[0] = source->axis[0];
    axis[1] = source->axis[1];
    axis[2] = source->axis[2];
    SetRotateMatrixAxis(angle, (const struct sdfVector3_t *)axis);
    SDF_VU0_LOADVECTOR(direction);
    SDF_VU0_APPLYMATRIX_XYZ();
    SDF_VU0_STOREVECTOR(direction);
    source->dir[0] = direction[0];
    source->dir[2] = direction[2];
    SDF_VU0_LOADVECTOR_B(work->head.origin);
    for (i = 0; i < rows; i++) {
        SDF_VU0_LOADVECTOR(vertices);
        SDF_VU0_SUBVECTOR();
        SDF_VU0_APPLYMATRIX_XYZ();
        SDF_VU0_ADDVECTOR();
        SDF_VU0_STOREVECTOR(vertices);
        SDF_VU0_LOADVECTOR(vertices + 1);
        SDF_VU0_SUBVECTOR();
        SDF_VU0_APPLYMATRIX_XYZ();
        SDF_VU0_ADDVECTOR();
        SDF_VU0_STOREVECTOR(vertices + 1);
        SDF_VU0_LOADVECTOR(vertices + 2);
        SDF_VU0_SUBVECTOR();
        SDF_VU0_APPLYMATRIX_XYZ();
        SDF_VU0_ADDVECTOR();
        SDF_VU0_STOREVECTOR(vertices + 2);
        SDF_VU0_LOADVECTOR(vertices + 3);
        SDF_VU0_SUBVECTOR();
        SDF_VU0_APPLYMATRIX_XYZ();
        SDF_VU0_ADDVECTOR();
        SDF_VU0_STOREVECTOR(vertices + 3);
        SDF_VU0_LOADVECTOR(vertices + 4);
        SDF_VU0_SUBVECTOR();
        SDF_VU0_APPLYMATRIX_XYZ();
        SDF_VU0_ADDVECTOR();
        SDF_VU0_STOREVECTOR(vertices + 4);
        vertices += 5;
    }
}
