#ifndef NEKO_H
#define NEKO_H

#include "common.h"

struct nekoNeko_t;

void ChooseNextTarget(struct nekoNeko_t *pNeko);
void UpdateRotation(struct nekoNeko_t *pNeko);
void UpdateTargetVelocity_neko(struct nekoNeko_t *pNeko);
void nekoCallMemFreeDelayedDirect(void);
void nekoUpdate(f32 playerX, f32 playerY);
void stfMapMapNeko(void);

#endif
