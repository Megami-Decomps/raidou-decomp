#ifndef MDLEFFECT_H
#define MDLEFFECT_H

#include "common.h"
#include "model.h"

/* Relative-linked viewer records share packed payload and parameter words.
 * Kinds 1/2 use the count and part-index halfwords at 0x0C/0x0E; kind 3
 * reads the whole parameter word. Kind 4 reads both selector halfwords at 0x08/0x0A. */
typedef struct pibHeaderChunk_t {
    s32 group;      /* 0x00: 0xFFFF terminates the record chain */
    s32 nextOffset; /* 0x04: relative byte offset to next record */
    union {
        u32 word;               /* 0x08 */
        s32 id;                 /* Kind 1/2/5 resource-record ID. */
        struct {
            u16 selectorA;
            u16 selectorB;
        } stream;
    } payload;
    union {
        u32 word;               /* 0x0C */
        s32 minimumTime;        /* Kind 5 deferred initialization threshold. */
        struct {
            u16 count;
            u16 partIndex;
        } part;
    } parameter;
    union {
        u32 word10; /* 0x10: group-four stream parameter */
        f32 anchorScale; /* Optional kind 1/2 extension when nextOffset >= 0x11. */
        struct {
            u8 playbackCadenceStep;
            u8 loopMode;
            u16 partIndex;
        } object;
        struct {
            u16 slotCount; /* Kind 1/2 group records: following entity-slot packets. */
            u16 unk12;
        } half;
    } tail;
} pibHeaderChunk_t;

SIZE_CHECK(pibHeaderChunk_size_must_be_0x14, pibHeaderChunk_t, 0x14);


/* Track marker record edited by the model viewer; the leading words stay opaque. */
typedef struct mdlTrackParams_t {
    u8 opaque00[8];
    s32 mark0;
    s32 mark1;
    u16 start;
    u16 end;
    u16 interval;
    u8 face;
    u8 blend;
    u32 colors[4];
} mdlTrackParams_t;

SIZE_CHECK(mdlTrackParams_size_must_be_0x28, mdlTrackParams_t, 0x28);

void mdlFreeEffect(dds3ModelEffectCtrlr_t *aCtrlr);
void mdlGenEffect(mdlObj_t *aHandle, s32 aGroup, s32 aPlayGroup);
void mdlLinkEffect(mdlObj_t *owner, dds3ModelEffectCtrlr_t *anchorRecord);
s32 mdlPibCheckItemType(pibHeaderChunk_t *data, s32 type);
void mdlPibDrawEffectInfo(sdfDrawTag_t *tag, s32 x, s32 y, s32 z, s32 attr, pibHeaderChunk_t *data);
void mdlPibDrawTrackInfo(sdfDrawTag_t *tag, s32 x, s32 y, s32 z, mdlTrackParams_t *data, s32 cursor);
void mdlPibEditTrackInfo(mdlTrackParams_t *data, s16 *cursor_p);
pibHeaderChunk_t *mdlPibGetChunk(mdlObj_t *handle, s32 group);
u16 mdlPibGetChunkCount(pibHeaderChunk_t *record);
s32 mdlPibGetVersion(pibHeaderChunk_t *record);
s32 mdlPibGetChunkItemCount(pibHeaderChunk_t *data);
u16 mdlPibGetEffectCount(pibHeaderChunk_t *record);
pibHeaderChunk_t *mdlPibGetFirstChunkItem(pibHeaderChunk_t *data);
u16 mdlPibGetMotionCount(const pibHeaderChunk_t *record);
pibHeaderChunk_t *mdlPibGetNextChunkItem(pibHeaderChunk_t *data);
void mdlSetEffectColorSub(mdlObj_t *aHandle, dds3ModelEffectCtrlr_t *aEffect, u32 aColor);
void mdlSetEffectScaleSub(mdlObj_t *aHandle, dds3ModelEffectCtrlr_t *aEffect, float aScale);
void mdlStopEffect(mdlObj_t *aHandle, s32 aPlayGroup);

#endif
