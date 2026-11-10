#ifndef DDS_EFF_EXPANDED_LIST_H

#define DDS_EFF_EXPANDED_LIST_H
#include "common.h"

struct RefObj;

struct sdfMemHandle_t;

/* Each serialized entry expands to itself and additionalCount children.
 * Animation sampling uses additionalCount as the segment length, with one
 * boundary frame between segments. payloadSize advances through the payloads. */
typedef struct EffExpandedEntry {
    s32 additionalCount;
    u32 payloadSize;
    u8 unk08[8];
} EffExpandedEntry;

/* Kind-7 reference animation modes; other flag bits are uninterpreted. */
enum EffReferenceFlag {
    EFF_ANIM_SET_LOOP = 0x1,
    EFF_ANIM_SET_DOUBLE_Y_SCALE = 0x4,
};

/* Kind-7 wrapper owns one allocation and retains a RefObj per entry.
 * totalEntryCount is also the animation's total track length. */
typedef struct EffExpandedList {
    u8 unk00[4];
    u32 count;
    u32 flags;
    u8 unk0C[4];
    EffExpandedEntry *entries;
    struct RefObj **handles;
    s32 totalEntryCount;
    u32 refCount;
    s32 unk20;
    struct sdfMemHandle_t *allocation;
} EffExpandedList;

typedef char EffExpandedEntrySizeCheck[(sizeof(EffExpandedEntry) == 0x10) ? 1 : -1];

typedef char EffExpandedListSizeCheck[(sizeof(EffExpandedList) == 0x28) ? 1 : -1];

s32 effCountExpandedEntries(const EffExpandedList *list);

EffExpandedList *effReferenceObjectRetain(EffExpandedList *obj);

void effReleaseReferenceHolder(EffExpandedList *holder);

#endif
