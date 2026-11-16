#include "common.h"

/* Blends packed RGBA channels; weights above 0x100 are mirrored from 0x200. */
u32 fclBlendColor(u32 color0, u32 color1, u32 weight);

#include "include_asm.h"
#include "sdf_resource.h"
#include "billboard.h"
#include "sdf_types.h"
#include "draw.h"
#include "libsdf.h"
#include "sdf_movie_types.h"
#include "effect.h"
#include "code_r1_001CC1D0.h"
#include "code_r1_002CA3F0.h"
#include "code_r1_002CAFD8.h"

extern void *effGetSlotWorkOrOverride(sprHandle_t *, s32);

void effDrawSpriteWithWorkCornerColors(s32 x, s32 y, s32 z, s32 flags, sprHandle_t *set, s32 slot, itfBdWork_t *entry, s32 layer);

void effDrawRepeatedSpriteTiles(s32 x, s32 y, s32 z, s32 length, u32 weight, u32 flags,
                  sprHandle_t *set, s32 slot, s32 layer);

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

void cmpFunc3(s32 x, s32 y, u32 z, s32 width, s32 height, u32 *edges, u8 *edgeValues, effSpriteUV_t *uv,
              u32 *colors, u32 flip, u32 alphaMode, s32 filter, sdfTexHandle_t *tex, s32 ot)
{
    s32 pos[2];
    s32 size[2];
    effSpriteUV_t saved;
    s32 *src = uv->coord;
    u32 edgeColors[4];
    s32 i;

    for (i = 0; i < 4; i++) {
        saved.coord[i] = src[i];
    }
    if (edges[0] != 0) {
        pos[1] = y - edges[0] * 8;
        size[1] = edges[0] * 8 + 4;
        pos[0] = x;
        size[0] = width;
        uv->rect.v1 = uv->rect.v0 + 1;
        sdfCopyCornerWordsWithEdgeByte(colors, edgeColors, edgeValues, 0);
        sdfDrawTexturedQuad(pos[0], pos[1], z, size[0], size[1], uv, edgeColors, flip, 0.0f, alphaMode, filter, tex, ot);
        uv->rect.v1 = saved.rect.v1;
    }
    if (edges[1] != 0) {
        pos[1] = y + height - 4;
        size[1] = edges[1] * 8 + 4;
        pos[0] = x;
        size[0] = width;
        uv->rect.v0 = uv->rect.v1 - 1;
        sdfCopyCornerWordsWithEdgeByte(colors, edgeColors, edgeValues, 1);
        sdfDrawTexturedQuad(pos[0], pos[1], z, size[0], size[1], uv, edgeColors, flip, 0.0f, alphaMode, filter, tex, ot);
        uv->rect.v0 = saved.rect.v0;
    }
    if (edges[2] != 0) {
        pos[0] = x - edges[2] * 16;
        size[0] = edges[2] * 16 + 8;
        pos[1] = y;
        size[1] = height;
        uv->rect.u1 = uv->rect.u0 + 1;
        sdfCopyCornerWordsWithEdgeByte(colors, edgeColors, edgeValues, 2);
        sdfDrawTexturedQuad(pos[0], pos[1], z, size[0], size[1], uv, edgeColors, flip, 0.0f, alphaMode, filter, tex, ot);
        uv->rect.u1 = saved.rect.u1;
    }
    if (edges[3] != 0) {
        pos[0] = x + width - 8;
        size[0] = edges[3] * 16 + 8;
        pos[1] = y;
        size[1] = height;
        uv->rect.u0 = uv->rect.u1 - 1;
        sdfCopyCornerWordsWithEdgeByte(colors, edgeColors, edgeValues, 3);
        sdfDrawTexturedQuad(pos[0], pos[1], z, size[0], size[1], uv, edgeColors, flip, 0.0f, alphaMode, filter, tex, ot);
        uv->rect.u0 = saved.rect.u0;
    }
}

void cmpFunc4(s32 x, s32 y, u32 z, s32 width, s32 height, u8 *counts, u8 *steps, effSpriteUV_t *uv, u32 *colors,
              u32 flip, u32 alphaMode, s32 filter, sdfTexHandle_t *tex, s32 ot)
{
    s32 pos[2];
    s32 size[2];
    s32 offset;
    s32 i;

    offset = 0;
    for (i = 1; i < counts[0]; i++) {
        offset += steps[0] * 16;
        pos[0] = x + offset;
        pos[1] = y;
        size[0] = width;
        size[1] = height;
        sdfDrawTexturedQuad(pos[0], pos[1], z, size[0], size[1], uv, colors, flip, 0.0f, alphaMode, filter, tex, ot);
    }
    offset = 0;
    for (i = 1; i < counts[1]; i++) {
        offset += steps[1] * 8;
        pos[0] = x;
        pos[1] = y + offset;
        size[0] = width;
        size[1] = height;
        sdfDrawTexturedQuad(pos[0], pos[1], z, size[0], size[1], uv, colors, flip, 0.0f, alphaMode, filter, tex, ot);
    }
}

/* NON_MATCHING: 177 of 179 words match. Retail evaluates fclDrawPanelSprite's id before loading ot into the call's delay slot. */
#ifdef NON_MATCHING
void effDrawTextureSlot(s32 x, s32 y, u32 z, u32 *colors, u32 flags, sprHandle_t *set, s32 slot, itfBdWork_t *draw,
                             s32 ot)
{
    effSlotDescription_t *desc;
    sdfTexHandle_t *tex;
    effSpriteUV_t uv;
    u32 alphaMode;
    s32 filter;
    u32 flip;
    s32 descFlags;
    u32 i;
    itfBdWork_t *work;

#ifdef VERSION_RAIDOU2
    if (slot >= set->SpriteNums) {
        return;
    }
#endif
    desc = &set->descriptions[slot];
    tex = set->textureReferences[desc->textureIndex];
    if (tex == NULL) {
        return;
    }
    work = &set->workEntries[slot];
    slot += work->slotOffset;
    if (!(flags & 0x20)) {
        fclDrawPanelSprite(0x5101B, ot);
    }
    if (!(flags & 1)) {
        x += desc->bounds[0] * 16;
        y += desc->bounds[1] * 8;
    }
    if (flags & 0x10) {
        x -= draw->geometry.bounds[0];
        y -= draw->geometry.bounds[1];
    } else {
        x += draw->geometry.bounds[0];
        y += draw->geometry.bounds[1];
    }
    descFlags = desc->flags;
    alphaMode = 16;
    filter = 0;
    if (descFlags & 4) {
        filter = 1;
    }
    if (!(flags & 0x40)) {
        alphaMode = desc->presetMode;
    }
    flip = descFlags & 3;
    if (descFlags & 8) {
        effSelectPresetAndDispatch(x, y, z, draw->geometry.bounds[2], draw->geometry.bounds[3], colors, alphaMode, ot);
    } else {
        for (i = 0; i < 4; i++) {
            uv.coord[i] = desc->uv.coord[i] + draw->parameters[i];
        }
        if (flags & 0x100) {
            x += desc->rect[2] * 16;
            y += desc->rect[0] * 8;
        }
        cmpFunc3(x, y, z, draw->geometry.bounds[2], draw->geometry.bounds[3], desc->rect, draw->bounds.texture.edgeAlpha,
                 &uv, colors, flip, alphaMode, filter, tex, ot);
        cmpFunc4(x, y, z, draw->geometry.bounds[2], draw->geometry.bounds[3], desc->repeatCount, desc->repeatStep, &uv,
                 colors, flip, alphaMode, filter, tex, ot);
        sdfDrawTexturedQuad(x, y, z, draw->geometry.bounds[2], draw->geometry.bounds[3], &uv, colors, flip,
                            draw->geometry.angleDegrees, alphaMode, filter, tex, ot);
    }
    if (flags & 0x80) {
        effUpdateTimedStates(set, slot, draw);
    }
}
#else
INCLUDE_ASM(const s32, "code_r1_002CA3F0", effDrawTextureSlot);
#endif

#ifdef VERSION_RAIDOU2
INCLUDE_ASM(const s32, "code_r1_002CA3F0", cmpCallDrawPanelSprite);

#endif
void effDrawSpriteWithWorkCornerColors(s32 x, s32 y, s32 z, s32 flags, sprHandle_t *set, s32 slot, itfBdWork_t *entry, s32 layer) {
    effDrawTextureSlot(x, y, z, entry->geometry.cornerColors, flags, set, slot, entry, layer);
}

void effDrawSpriteWithCornerColors(s32 x, s32 y, s32 z, u32 *palette, s32 flags, sprHandle_t *set, s32 slot, s32 layer) {
    effDrawTextureSlot(x, y, z, palette, flags, set, slot,
                  (itfBdWork_t *)effGetSlotWorkOrOverride(set, slot), layer);
}

void effDrawSpriteWithCornerFade(s32 x, s32 y, s32 depth, u32 blend, s32 flags,
                   sprHandle_t *set, s32 slotIndex, s32 layer) {
    u32 colors[4];
    itfBdWork_t *work = effGetSlotWorkOrOverride(set, slotIndex);
    s32 i;

    for (i = 0; i < 4; i++) {
        colors[i] = fclBlendColor(work->geometry.cornerColors[i],
                                  work->geometry.cornerColors[i] & 0xFFFFFF00, blend);
    }
    effDrawTextureSlot(x, y, depth, colors, flags, set, slotIndex, work, layer);
}

void effDrawRepeatedSpriteTiles(s32 x, s32 y, s32 z, s32 length, u32 weight, u32 flags,
                  sprHandle_t *set, s32 slot, s32 layer) {
    u32 colors[4];
    void *draw;
    s32 tileWidth;
    s32 i;

    draw = effGetSlotWorkOrOverride(set, slot);
    for (i = 0; i < 4; i++) {
        u32 color = ((effSlotGeometry_t *)((u8 *)draw + 4))->cornerColors[i];
        colors[i] = fclBlendColor(color, color & ~0xFF, weight);
    }
    {
        /* Start with the complete tile; draw callbacks may replace the work array. */
        set->workEntries[slot].parameters[2] = 0;
        tileWidth = set->workEntries[slot].sourceWidth * 16;
        set->workEntries[slot].geometry.bounds[2] = tileWidth;
    }
    for (i = 0; i < length / tileWidth; i++) {
        effDrawTextureSlot(x, y, z, colors, flags, set, slot, draw, layer);
        x += tileWidth;
    }
    {
        /* Publish the crop offset, then the visible width, before the final draw. */
        length = tileWidth - length % tileWidth;
        set->workEntries[slot].parameters[2] = -(length >> 4);
        set->workEntries[slot].geometry.bounds[2] = tileWidth - length;
    }
    effDrawTextureSlot(x, y, z, colors, flags, set, slot, draw, layer);
}
