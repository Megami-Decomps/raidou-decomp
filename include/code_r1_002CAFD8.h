#ifndef CODE_R1_002_CAFD8_H
#define CODE_R1_002_CAFD8_H

#include "common.h"
#include "code_r1_001CC1D0.h"

void itfDrawGridWithResolvedSlot(s32 offsetX, s32 offsetY, s32 z,
                                 s32 drawFlags, struct sprHandle_t *object,
                                 s32 index, s32 surfaceIndex);

s32 itfGridLookupValueOrDefault(struct sprHandle_t *resource, s32 resourceIndex);
void itfSetGridEntryQuantizedAndRefresh(struct sprHandle_t *resource, s32 resourceIndex, s32 x, s32 y, s32 width, s32 height);
void fclDrawPanelSprite(s32 id, s32 ot);
void uiDrawUniformRgbRange(const u32 *xs, const u32 *ys, u32 z, u32 color, u32 ot);
void fclDrawPanelSprite2(s32 id, s32 ot);

/* Known continuation bit; other effSlotDescription_t.flags meanings remain unspecified. */
enum EffectSlotDescriptionFlag {
    EFF_SLOT_DESCRIPTION_SEQUENCE_CONTINUATION = 0x20,
};

#endif
