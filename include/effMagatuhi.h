#ifndef EFFMAGATUHI_H
#define EFFMAGATUHI_H

#include "common.h"
#include "sdf_types.h"

/* The creation callback's returned payload is retained in this 8-byte dispatch owner. */
typedef struct dds3MagatuhiPar_tag {
    u32 type;
    void *pHandle;
} dds3MagatuhiPar_tag;

/* Callback input prefix; type 3 contains an effect-dispatch object at +8. */
typedef struct effMagatuhiCallback_t {
    s32 type;
    u8 pad04[4];
    dds3MagatuhiPar_tag *effect;
} effMagatuhiCallback_t;

/* Smoke convergence area: origin and target rows, initial radius and capture distance. */
typedef struct {
    sdfVector4_t position[2];
    f32 radius[2];
} effMagaSCArea_t;

/* Four Bezier control points (xyz copied, w kept) and their jitter scales. */
typedef struct {
    f32 controlPoints[4][4];
    f32 jitterScales[4];
} effMagaP4Point_t;

void effMagaSCSetArea(effMagatuhiCallback_t *hEffect, effMagaSCArea_t *pMagaSCArea);
void effMagaP4SetPoint(effMagatuhiCallback_t *hEffect, effMagaP4Point_t *pP4Point);
void effMagaP4Randomize(effMagatuhiCallback_t *hEffect);
s32 effMagaRandomize(effMagatuhiCallback_t *hEffect);
u32 effMagaCylinderCalcColor(u32 color, u32 colorB);

#endif
