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
typedef struct SBTLRESOURCE_ENTRY {
    s32 category;                    /* 0x00: preview/selection category */
    s32 value;                       /* 0x04: category-specific index/data */
    s32 id;                          /* 0x08: category-specific resource id */
    char name[0x30];                 /* 0x0C: copied resource/display name */
    struct SBTLRESOURCE_ENTRY *prev;   /* 0x3C */
    struct SBTLRESOURCE_ENTRY *next;   /* 0x40 */
} SBTLRESOURCE_ENTRY;

typedef struct SBTLRESOURCEENTRY_LIST {
    s32 count;                       /* 0x00: number of linked entries */
    char *pathPrefix;                /* 0x04: owned directory prefix */
    SBTLRESOURCE_ENTRY *head;          /* 0x08 */
} SBTLRESOURCEENTRY_LIST;

typedef char SBTLRESOURCE_ENTRYSizeCheck[(sizeof(SBTLRESOURCE_ENTRY) == 0x44) ? 1 : -1];

typedef char SBTLRESOURCEENTRY_LISTSizeCheck[(sizeof(SBTLRESOURCEENTRY_LIST) == 0x0C) ? 1 : -1];

/* Browser viewport/selection and texture ownership; the entry list is borrowed. */
typedef struct SBTLRESOURCE_DESCRIPTOR {
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
    struct SBTLRESOURCE_ENTRY *firstVisibleEntry; /* 0x30 */
    struct SBTLRESOURCE_ENTRY *selectedEntry; /* 0x34 */
    struct SBTLRESOURCE_ENTRY *cachedEntry; /* 0x38: last entry whose preview was updated */
    struct sdfTexHandle_t *texture;        /* 0x3C: owned or borrowed preview texture */
    s32 textureCategory;    /* 0x40: TMX is owned; GENERAL is borrowed. */
    struct SBTLRESOURCEENTRY_LIST *entryList; /* 0x44 */
} SBTLRESOURCE_DESCRIPTOR;

typedef char SBTLRESOURCE_DESCRIPTOR_size_must_be_0x48[
    (sizeof(SBTLRESOURCE_DESCRIPTOR) == 0x48) ? 1 : -1];

/* sdfGrOtTable index the resource browser draws into. */
#ifdef VERSION_RAIDOU1
#define BTL_RESOURCE_DRAW_SURFACE 0x70
#else
#define BTL_RESOURCE_DRAW_SURFACE 0x71
#endif

struct SBTLRESOURCE_DESCRIPTOR *
btlCreateResourceDescriptor(struct SBTLRESOURCEENTRY_LIST *list);

s32 Draw_effLensFlare(struct SBTLRESOURCE_DESCRIPTOR *descriptor);
void btlDrawResourcePreview(struct SBTLRESOURCE_DESCRIPTOR *descriptor);

extern char *dds3EffectFileNameTable2[];
extern char btlFormatSelectedResourceNameData[];
extern u8 btlUpdateResourceNameEditorData[];
extern char btlUpdateResourceNameEditorData2[8];
extern char btlUpdateResourceNameEditorData3[];
extern char fmData[];

typedef struct SBTLRESOURCENAME_RECORD {
    s32 x; /* UI origin for the name and overwrite prompts. */
    s32 y;
    u32 selectionStatus; /* Pending, accepted, or canceled. */
    s32 selection; /* Keyboard selection or overwrite yes/no choice. */
    s32 nameLength;
    u32 maximumNameLength;
    s32 overwritePromptActive;
    char extension[5];
    char resourceName[0x17];
} SBTLRESOURCENAME_RECORD;

typedef char SBTLRESOURCENAME_RECORDSizeCheck[
    (sizeof(SBTLRESOURCENAME_RECORD) == 0x38) ? 1 : -1];

void btlUpdateResourceNameEditor(struct SBTLRESOURCENAME_RECORD *record);

struct SBTLRESOURCEENTRY_LIST *btlScanDirectory(const char *path, s32 flags);

void btlDestroyEntryList(struct SBTLRESOURCEENTRY_LIST *list);

void btlAppendEntry(struct SBTLRESOURCEENTRY_LIST *list, const char *name,
                    s32 category, s32 value, s32 id);

void btlDestroyResourceDescriptor(struct SBTLRESOURCE_DESCRIPTOR *descriptor);

s32 btlFormatSelectedResourceName(struct SBTLRESOURCE_DESCRIPTOR *descriptor,
                                  char *output);

s32 btlTrimResourceName(struct SBTLRESOURCE_DESCRIPTOR *descriptor,
                        char *output);

u32 btlGetResourcePathVariant(const struct SBTLRESOURCE_DESCRIPTOR *descriptor);

void btlSetResourceBrowserPosition(struct SBTLRESOURCE_DESCRIPTOR *descriptor, s32 x, s32 y);

u32 btlGetResourceBrowserSelectionStatus(const struct SBTLRESOURCE_DESCRIPTOR *descriptor);

struct SBTLRESOURCENAME_RECORD *btlCreateResourceNameRecord(const char *extension);

void btlSetResourceNamePosition(struct SBTLRESOURCENAME_RECORD *record, s32 x, s32 y);

void btlResourceRecordSetName(struct SBTLRESOURCENAME_RECORD *record, const char *name);

void btlFormatResourceNameWithExtension(const struct SBTLRESOURCENAME_RECORD *record, char *output);

void btlFormatResourceNameWithoutExtension(const struct SBTLRESOURCENAME_RECORD *record, char *output);

u32 btlGetResourceNameSelectionStatus(const struct SBTLRESOURCENAME_RECORD *record);

void btlSetResourceNameLengthLimit(struct SBTLRESOURCENAME_RECORD *record, u32 maximumNameLength);

void btlDestroyResourceNameRecord(struct SBTLRESOURCENAME_RECORD *record);

void btlDebugDrawOrientedExtentGuide(f32 *center, f32 extent, f32 height, u32 color, f32 (*rotation)[4]);
void btlDebugDrawPoint(f32 *position);
void btlDebugDrawRadiusGuide(f32 *center, f32 radius, u32 color, s32 flags);
void btlDebugDrawSegment(f32 *from, f32 *to, u32 color);
void btlDebugMenuRecordNoOp(void *context);
void btlBossDebugPrintfN(s32 x, s32 y, s32 color, const char *format, ...);

/* Callbacks of the pb debug menus (asm unit code_0017BAE8); the menu slots fix the parameters. */
u32 pbEffectEditCommand(void);
u32 pbFieldLightMenuValue(u32 value);
void pbFieldLightMenuUpdate(u32 job);
void pbFieldLightMenuDestroy(u32 job);
void pbPointLightMenuUpdate(u32 job);
void pbPointLightMenuDestroy(u32 job);
void pbCameraMenuUpdate(u32 job);
void pbCameraMenuDestroy(u32 job);
extern struct effDebugMenu_t pbCameraMenu;
extern struct effDebugMenu_t pbDebugMenu;

#endif
