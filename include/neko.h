#ifndef NEKO_H
#define NEKO_H

#include "common.h"
#include "sdf_types.h"

typedef struct nekoNeko_t {
    sdfVector4_t rotateQuat;
    s32 fExist; /* 0x10 */
    s32 index;
    f32 x; /* 0x18 */
    f32 y;
    f32 vx;
    f32 vy;
    f32 targetVx;
    f32 targetVy;
    f32 vleng;
    f32 angle;
    s32 state; /* 0x38 */
    s32 homeIndex;
    s32 nextPointIndex;
    s32 prevPointIndex;
    s32 hideCnt;
    s32 existCnt;
} nekoNeko_t;

typedef struct nekoWork_t {
    nekoNeko_t neko[5];
} nekoWork_t;

extern nekoWork_t *Work_neko;

void ChooseNextTarget(struct nekoNeko_t *pNeko);
void UpdateRotation(struct nekoNeko_t *pNeko);
void UpdateTargetVelocity_neko(struct nekoNeko_t *pNeko);
void nekoCallMemFreeDelayedDirect(void);
void nekoUpdate(f32 playerX, f32 playerY);
void stfMapMapNeko(void);

#endif
