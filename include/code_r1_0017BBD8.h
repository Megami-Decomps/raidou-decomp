#ifndef CODE_R1_0017_BBD8_H
#define CODE_R1_0017_BBD8_H

#include "common.h"

/* Stored entry categories, including synthetic GENERAL entries; not scan masks. */
enum BtlResourceEntryCategory {
    BTL_RESOURCE_ENTRY_CATEGORY_TMX = 0x01,
    BTL_RESOURCE_ENTRY_CATEGORY_P2A = 0x02,
    BTL_RESOURCE_ENTRY_CATEGORY_PB = 0x04,
    BTL_RESOURCE_ENTRY_CATEGORY_GENERAL = 0x08,
    BTL_RESOURCE_ENTRY_CATEGORY_EPL = 0x10,
    BTL_RESOURCE_ENTRY_CATEGORY_EP = 0x20,
    BTL_RESOURCE_ENTRY_CATEGORY_TLP = 0x40,
    BTL_RESOURCE_ENTRY_CATEGORY_F2 = 0x80,
};

/* Scanned resource rows and their owning list. The providers allocate each
 * entry at 0x44 bytes, populate all six fields, and traverse both links. */
typedef struct BtlResourceEntry {
    s32 category;                    /* 0x00: preview/selection category */
    s32 value;                       /* 0x04: category-specific index/data */
    s32 id;                          /* 0x08: category-specific resource id */
    char name[0x30];                 /* 0x0C: copied resource/display name */
    struct BtlResourceEntry *prev;   /* 0x3C */
    struct BtlResourceEntry *next;   /* 0x40 */
} BtlResourceEntry;

typedef struct BtlResourceEntryList {
    s32 count;                       /* 0x00: number of linked entries */
    char *pathPrefix;                /* 0x04: owned directory prefix */
    BtlResourceEntry *head;          /* 0x08 */
} BtlResourceEntryList;

typedef char BtlResourceEntrySizeCheck[(sizeof(BtlResourceEntry) == 0x44) ? 1 : -1];

typedef char BtlResourceEntryListSizeCheck[(sizeof(BtlResourceEntryList) == 0x0C) ? 1 : -1];

/* Browser viewport/selection and texture ownership; the entry list is borrowed. */
typedef struct BtlResourceDescriptor {
    s32 originX;            /* 0x00: horizontal browser origin */
    s32 originY;            /* 0x04: vertical browser origin */
    u32 selectionStatus;    /* 0x08: pending, accepted, or canceled */
    s32 entryCount;         /* 0x0C */
    u32 word10;             /* 0x10 */
    u32 selectedIndex;      /* 0x14 */
    u32 visibleIndex;       /* 0x18 */
    u32 previewActive;      /* 0x1C */
    u32 repeatDelay;        /* 0x20 */
    u32 drawSurfaceIndex;   /* 0x24 */
    u32 borderColor;        /* 0x28: line-strip color */
    u32 fillColor;          /* 0x2C: sprite-fill color */
    struct BtlResourceEntry *firstVisibleEntry; /* 0x30 */
    struct BtlResourceEntry *selectedEntry; /* 0x34 */
    struct BtlResourceEntry *cachedEntry; /* 0x38: last entry whose preview was updated */
    struct sdfTexHandle_t *texture;        /* 0x3C: owned or borrowed preview texture */
    s32 textureCategory;    /* 0x40: TMX is owned; GENERAL is borrowed. */
    struct BtlResourceEntryList *entryList; /* 0x44 */
} BtlResourceDescriptor;

typedef char BtlResourceDescriptor_size_must_be_0x48[
    (sizeof(BtlResourceDescriptor) == 0x48) ? 1 : -1];

struct BtlResourceDescriptor *
btlCreateResourceDescriptor(struct BtlResourceEntryList *list);

s32 Draw_effLensFlare(struct BtlResourceDescriptor *descriptor);

typedef struct BtlResourceNameRecord {
    s32 x; /* UI origin for the name and overwrite prompts. */
    s32 y;
    u32 selectionStatus; /* Pending, accepted, or canceled. */
    s32 selection; /* Keyboard selection or overwrite yes/no choice. */
    s32 nameLength;
    u32 maximumNameLength;
    s32 overwritePromptActive;
    char extension[5];
    char resourceName[0x17];
} BtlResourceNameRecord;

typedef char BtlResourceNameRecordSizeCheck[
    (sizeof(BtlResourceNameRecord) == 0x38) ? 1 : -1];

void btlUpdateResourceNameEditor(struct BtlResourceNameRecord *record);

struct BtlResourceEntryList *btlScanDirectory(const char *path, s32 flags);

void btlDestroyEntryList(struct BtlResourceEntryList *list);

void btlAppendEntry(struct BtlResourceEntryList *list, const char *name,
                    s32 category, s32 value, s32 id);

void btlDestroyResourceDescriptor(struct BtlResourceDescriptor *descriptor);

s32 btlFormatSelectedResourceName(struct BtlResourceDescriptor *descriptor,
                                  char *output);

s32 btlTrimResourceName(struct BtlResourceDescriptor *descriptor,
                        char *output);

u32 btlGetResourcePathVariant(const struct BtlResourceDescriptor *descriptor);

void btlSetResourceBrowserPosition(struct BtlResourceDescriptor *descriptor, s32 x, s32 y);

u32 btlGetResourceBrowserSelectionStatus(const struct BtlResourceDescriptor *descriptor);

struct BtlResourceNameRecord *btlCreateResourceNameRecord(const char *extension);

void btlSetResourceNamePosition(struct BtlResourceNameRecord *record, s32 x, s32 y);

void btlResourceRecordSetName(struct BtlResourceNameRecord *record, const char *name);

void btlFormatResourceNameWithExtension(const struct BtlResourceNameRecord *record, char *output);

void btlFormatResourceNameWithoutExtension(const struct BtlResourceNameRecord *record, char *output);

u32 btlGetResourceNameSelectionStatus(const struct BtlResourceNameRecord *record);

void btlSetResourceNameLengthLimit(struct BtlResourceNameRecord *record, u32 maximumNameLength);

void btlDestroyResourceNameRecord(struct BtlResourceNameRecord *record);

void btlDebugDrawOrientedExtentGuide(f32 *center, f32 extent, f32 height, u32 color, f32 (*rotation)[4]);
void btlDebugDrawPoint(f32 *position);
void btlDebugDrawRadiusGuide(f32 *center, f32 radius, u32 color, s32 flags);
void btlDebugDrawSegment(f32 *from, f32 *to, u32 color);
void btlDebugMenuRecordNoOp(void *context);

#endif
