#ifndef CODE_R1_002CA3F0_H
#define CODE_R1_002CA3F0_H

#include "common.h"

struct sprHandle_t;

void effDrawSpriteWithCornerFade(s32 x, s32 y, s32 depth, u32 blend, s32 flags, struct sprHandle_t *set, s32 slotIndex, s32 layer);
void effDrawSpriteWithCornerColors(s32 x, s32 y, s32 z, u32 *palette, s32 flags, struct sprHandle_t *set, s32 slot, s32 layer);

#endif
