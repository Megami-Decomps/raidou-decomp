#include "common.h"

/* Declarations from dds-decomp src/dds1/game/code_002BEDC8.c */
#ifndef VERSION_DDS1
#define VERSION_DDS1 1
#endif

#include "dds/ui_color.h"

/* Blends packed RGBA channels; weights above 0x100 are mirrored from 0x200. */
u32 fclBlendColor(u32 color0, u32 color1, u32 weight);

#include "dds/common.h"

#include "dds/ui_color.h"

#include "dds/eff.h"

extern void *effGetSlotWorkOrOverride(EffectSlotSet *, s32);

/* 0x80 leaves the copied corner words unchanged. */
void sdfCopyCornerWordsWithEdgeByte(u32 *source, u32 *destination, u8 *edgeValues, u32 edge);

extern void sdfDrawTexturedQuad(s32, s32, s32, s32, s32, const u32 *, const u32 *, u32,
                          f32, u32, u32, sdfTexHandle_t *, u32);

/* Draw enabled border strips and restore the temporary texture-coordinate changes. */
void effDrawTexturedRectBorderStrips(s32 x, s32 y, s32 depth, s32 width, s32 height,
                  const u32 *edgeExtents, u8 *edgeColorBytes,
                  u32 *textureCoordinates, u32 *sourceCornerColors, u32 kind,
                  u32 mode, u32 flip, u32 texture, u32 buffer);

extern void sdfSubmitGsTestOneRegisterPacket(u64, u32);

extern void effSelectPresetAndDispatch(u32, u32, u32, u32, u32, u32, u32, u32);

extern EffectSlotSet *effUpdateTimedStates(EffectSlotSet *, u32, void *);

void effDrawTextureSlot(s32 x, s32 y, s32 z, s32 color, u32 flags,
                  EffectSlotSet *set, s32 slotIndex, BdWork *draw, s32 buffer);

void effDrawSpriteWithWorkCornerColors(s32 x, s32 y, s32 z, s32 flags, EffectSlotSet *set, s32 slot, BdWork *entry, s32 layer);

void effDrawSpriteWithCornerColors(s32 x, s32 y, s32 z, u32 *palette, s32 flags, EffectSlotSet *set, s32 slot, s32 layer);

void effDrawSpriteWithCornerFade(s32 x, s32 y, s32 depth, u32 blend, s32 flags,
                   EffectSlotSet *set, s32 slotIndex, s32 layer);

void effDrawRepeatedSpriteTiles(s32 x, s32 y, s32 z, s32 length, u32 weight, u32 flags,
                  EffectSlotSet *set, s32 slot, s32 layer);
/* End of declarations from src/dds1/game/code_002BEDC8.c */

/* 0x80 leaves the copied corner words unchanged. */
void sdfCopyCornerWordsWithEdgeByte(u32 *source, u32 *destination, u8 *edgeValues, u32 edge) {
    u8 value = edgeValues[edge];

    destination[0] = source[0];
    destination[1] = source[1];
    destination[2] = source[2];
    destination[3] = source[3];
    if (value != 0x80) {
        switch (edge) {
        case 0:
            destination[0] = (destination[0] & ~0xFF) | value;
            destination[1] = (destination[1] & ~0xFF) | value;
            break;
        case 1:
            destination[2] = (destination[2] & ~0xFF) | value;
            destination[3] = (destination[3] & ~0xFF) | value;
            break;
        case 2:
            destination[0] = (destination[0] & ~0xFF) | value;
            destination[2] = (destination[2] & ~0xFF) | value;
            break;
        case 3:
            destination[1] = (destination[1] & ~0xFF) | value;
            destination[3] = (destination[3] & ~0xFF) | value;
            break;
        }
    }
}

INCLUDE_ASM(const s32, "code_0032A6A0", func_r1_002CA4C8);

INCLUDE_ASM(const s32, "code_0032A6A0", func_r1_002CA7D8);

INCLUDE_ASM(const s32, "code_0032A6A0", func_r1_002CA978);

INCLUDE_ASM(const s32, "code_0032A6A0", func_0032AF00);

void effDrawSpriteWithWorkCornerColors(s32 x, s32 y, s32 z, s32 flags, EffectSlotSet *set, s32 slot, BdWork *entry, s32 layer) {
    effDrawTextureSlot(x, y, z, (s32)entry->geometry.cornerColors, flags, set, slot, entry, layer);
}

void effDrawSpriteWithCornerColors(s32 x, s32 y, s32 z, u32 *palette, s32 flags, EffectSlotSet *set, s32 slot, s32 layer) {
    effDrawTextureSlot(x, y, z, (s32)palette, flags, set, slot,
                  (BdWork *)effGetSlotWorkOrOverride(set, slot), layer);
}

void effDrawSpriteWithCornerFade(s32 x, s32 y, s32 depth, u32 blend, s32 flags,
                   EffectSlotSet *set, s32 slotIndex, s32 layer) {
    u32 colors[4];
    BdWork *work = effGetSlotWorkOrOverride(set, slotIndex);
    s32 i;

    for (i = 0; i < 4; i++) {
        colors[i] = fclBlendColor(work->geometry.cornerColors[i],
                                  work->geometry.cornerColors[i] & 0xFFFFFF00, blend);
    }
    effDrawTextureSlot(x, y, depth, (s32)colors, (u32)flags, set, slotIndex, work, layer);
}

void effDrawRepeatedSpriteTiles(s32 x, s32 y, s32 z, s32 length, u32 weight, u32 flags,
                  EffectSlotSet *set, s32 slot, s32 layer) {
    u32 colors[4];
    void *draw;
    s32 tileWidth;
    s32 i;

    draw = effGetSlotWorkOrOverride(set, slot);
    for (i = 0; i < 4; i++) {
        u32 color = ((EffectSlotGeometry *)((u8 *)draw + 4))->cornerColors[i];
        colors[i] = fclBlendColor(color, color & ~0xFF, weight);
    }
    {
        /* Start with the complete tile; draw callbacks may replace the work array. */
        set->workEntries[slot].parameters[2] = 0;
        tileWidth = set->workEntries[slot].sourceWidth * 16;
        set->workEntries[slot].geometry.bounds[2] = tileWidth;
    }
    for (i = 0; i < length / tileWidth; i++) {
        effDrawTextureSlot(x, y, z, (s32)colors, flags, set, slot, draw, layer);
        x += tileWidth;
    }
    {
        /* Publish the crop offset, then the visible width, before the final draw. */
        length = tileWidth - length % tileWidth;
        set->workEntries[slot].parameters[2] = -(length >> 4);
        set->workEntries[slot].geometry.bounds[2] = tileWidth - length;
    }
    effDrawTextureSlot(x, y, z, (s32)colors, flags, set, slot, draw, layer);
}
