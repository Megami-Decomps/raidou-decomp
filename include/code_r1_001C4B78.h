#ifndef CODE_R1_001_C4_B78_H
#define CODE_R1_001_C4_B78_H

#include "common.h"
#include "sdf_resource.h"
#include "basic.h"
#include "kernel.h"
#include "event.h"

/* Index into EvtRuntime.entryName, with the lookup API's missing-name value. */
typedef s32 EvtViewerNameIndex;

/* Script start holds viewer setup until the script releases this flag. */
typedef u32 EvtPolygonMovieWorkFlags;

/* Signed index into the fixed message-window pool; -1 means no window. */
typedef s32 ItfMesWindowIndex;

/* PMD2 records and the complete 0x11C-byte owner cleared by both games'
 * InitPolygonMovieWorkData. Movie parsing, asynchronous resource loading,
 * viewer dialogs and message-window lifetime all use this single object. */
typedef struct PmdEntry {
    u32 type;     /* 0x00 */
    u32 elementSize; /* 0x04: serialized payload bytes per element. */
    u32 value;    /* 0x08 */
    u32 offset;   /* 0x0C */
} PmdEntry;

typedef struct PmdHeader {
    s8 FileType;
    s8 FileFormat;
    s16 UserID;
    s32 FileSize;
    char MagicCode[4];
    s32 ExpandSize;
    s32 count;    /* 0x10 */
    s32 kind;     /* 0x14 */
    s32 Reserve2;
    s32 Reserve3;
    const PmdEntry entries[1]; /* 0x20: serialized records are read-only. */
} PmdHeader;

typedef struct PolyMovieResourceSlot {
    struct fileRequest *request;
    sdfMemHandle_t *handle;
    PmdHeader *address;
} PolyMovieResourceSlot;

typedef struct PolyMovieWork {
    EvtPolygonMovieWorkFlags flags; /* 0x00 */
    PolyMovieResourceSlot mainResource; /* 0x04 */
    PmdHeader *data;   /* 0x10 */
    const PmdEntry *entries; /* 0x14: borrowed directory records. */
    u8 *mainEntry1Data;   /* 0x18 */
    u32 mainEntry1Value; /* 0x1C: value from the main type-1 entry. */
    u8 *mainEntry2Data;   /* 0x20 */
    u32 mainEntry2Value; /* 0x24: value from the main type-2 entry. */
    u8 *mainEntry10Data;  /* 0x28 */
    u8 *mainEntry11Data;  /* 0x2C */
    u8 *mainEntry12Data;  /* 0x30 */
    u8 *mainEntry3Data;   /* 0x34 */
    u32 mainEntry3Count; /* 0x38: rows in the main type-3 package table */
    u8 *mainEntry9Data;   /* 0x3C */
    u8 *mainEntry7Data;   /* 0x40 */
    u32 mainEntry7Count; /* 0x44: PMD type-7 resource row count */
    u8 *mainEntry8Data;   /* 0x48 */
    u8 *mainEntry6Data;   /* 0x4C */
    u8 *mainEntry22Data;  /* 0x50 */
    u32 mainEntry22Count; /* 0x54: PMD type-22 texture row count */
    u8 *mainEntry23Data;  /* 0x58 */
    PolyMovieResourceSlot secondaryResource; /* 0x5C */
    PolyMovieResourceSlot tertiaryResource; /* 0x68 */
    PmdHeader *sub;    /* 0x74 */
    const PmdEntry *subEntries; /* 0x78: borrowed directory records. */
    u8 *subEntry1Data;    /* 0x7C */
    u32 subEntry1Value; /* 0x80: value from the secondary type-1 entry. */
    u8 *subEntry0Data;    /* 0x84 */
    const struct EvtCompactRow *subEntry4Kind4Data; /* 0x88 */
    const struct EvtExtendedRow *subEntry4OtherData; /* 0x8C */
    PmdHeader *sub2;   /* 0x90 */
    const PmdEntry *sub2Entries; /* 0x94: borrowed directory records. */
    const struct EvtExtendedRow *secondEntry4Data; /* 0x98 */
    u32 extendedEntry4Count; /* 0x9C: rows in the extended type-4 table */
    u32 subEntry4Count; /* 0xA0 */
    u8 *subEntry5Data;    /* 0xA4 */
    u32 subEntry5Value; /* 0xA8: value from the secondary type-5 entry. */
    u8 *subEntry13Data;   /* 0xAC */
    u32 subEntry13Value; /* 0xB0: value from the secondary type-13 entry. */
    u8 *subEntry14Data;   /* 0xB4 */
    u32 subEntry14Value; /* 0xB8: value from the secondary type-14 entry. */
    u8 *subEntry15Data;   /* 0xBC */
    u32 subEntry15Value; /* 0xC0: value from the secondary type-15 entry. */
    u8 *subEntry16Data;   /* 0xC4 */
    u32 subEntry16Value; /* 0xC8: value from the secondary type-16 entry. */
    u8 *subEntry17Data;   /* 0xCC */
    u32 subEntry17Value; /* 0xD0: value from the secondary type-17 entry. */
    u8 *subEntry18Data;   /* 0xD4 */
    u32 subEntry18Value; /* 0xD8: value from the secondary type-18 entry. */
    u8 *subEntry19Data;   /* 0xDC */
    u32 subEntry19Value; /* 0xE0: value from the secondary type-19 entry. */
    u8 *subEntry20Data;   /* 0xE4 */
    u32 subEntry20Value; /* 0xE8: value from the secondary type-20 entry. */
    u8 *subEntry24Data;   /* 0xEC */
    u32 subEntry24Value; /* 0xF0: value from the secondary type-24 entry. */
    u8 *subEntry21Data;   /* 0xF4 */
    u32 subEntry21Count; /* 0xF8: rows in the sub type-21 group metadata table */
    u8 *subEntry25Data;   /* 0xFC */
    u32 subEntry25Count;       /* 0x100 */
    ItfMesWindowIndex messageManagerHandle; /* 0x104: owned index for the main type-6 message window. */
    struct dds3ProcessID_t *task; /* 0x108: scheduler task created for this viewer */
    s32 eventId;       /* 0x10C */
    s32 sceneId;       /* 0x110 */
    u32 unk_114;       /* 0x114 */
    s32 *buffer;       /* 0x118 */
} PolyMovieWork;

typedef char PolyMovieWork_size_must_be_0x11C[(sizeof(PolyMovieWork) == 0x11C) ? 1 : -1];

/* A timeline key's scalar formats are selected by its track kind. */
typedef union EvtViewParam {
    f32 f; s32 i; u32 u; u16 h[2]; s16 sh[2]; u8 b[4]; s8 sb[4];
} EvtViewParam;

/* Complete 0x38-byte key allocated by the viewer and linked on a group. */
typedef struct EvtRuntimeChild {
    u16 frame;
    u16 duration; /* Also the consecutive index in type-0xB groups. */
    s32 serializedValue;
    union {
        s8 parameterBytes[0x20];
        union {
            s32 words[8];
            struct {
                u8 pad00[2]; u16 groupTypeIndex;
                u8 pad04[6]; u16 groupTypeAIndex;
            } f;
        } body;
        struct {
            EvtViewParam p08, p0C, p10, p14, p18, p1C, p20, p24;
        };
    };
    u8 pad28[4];
    void *payload; /* Type 0x12 owns a dds3Basic_t; other kinds hold buffers or borrowed data. */
    struct EvtRuntimeChild *next;
    struct EvtRuntimeChild *prev;
} EvtRuntimeChild;

/* Serialized and embedded group-link metadata share these eight bytes. */
typedef struct EvtGroupMetadata {
    u8 type;
    u8 flag;
    s16 entry;
    s16 value;
    s8 option; /* +0x06: type-specific group mode chosen by the viewer. */
    s8 extra2;
} EvtGroupMetadata;

typedef s32 EvtViewerParameterKeySource;

typedef enum EvtObjectAttachmentState {
    EVT_OBJECT_ATTACHMENT_DETACHED = 0,
    EVT_OBJECT_ATTACHMENT_ATTACHED = 1
} EvtObjectAttachmentState;

/* The timed action's end word is either a frame limit or an open-ended policy. */
typedef enum EvtTimedActionEndMode {
    EVT_TIMED_ACTION_END_UNBOUNDED = -1,
    EVT_TIMED_ACTION_END_PRESERVE_MOVIE_ON_RELEASE = -2
} EvtTimedActionEndMode;

typedef union EvtTimedActionEnd {
    s32 frame;
    EvtTimedActionEndMode mode;
} EvtTimedActionEnd;

typedef union EvtCommandArgument { s32 word; char *text; } EvtCommandArgument;

typedef union EvtFrameRange {
    s32 word;
    struct { u16 end; u16 unk02; } f;
} EvtFrameRange;

typedef char EvtRuntimeChild_size[(sizeof(EvtRuntimeChild) == 0x38) ? 1 : -1];

typedef char EvtGroupMetadata_size[(sizeof(EvtGroupMetadata) == 8) ? 1 : -1];

/* Complete 0x84-byte entry: metadata, selected object and owned key list. */
typedef struct EvtRuntimeGroup {
    s32 type;
    union {
        struct { u8 metadataFlag; u8 pad05[3]; };
        s32 setterId;
    };
    EvtViewerNameIndex entryHeader; /* 0x08: signed runtime name-table index. */
    s32 argument0C;
    dds3Basic_t *info;
    union {
        void *resourceData; /* Generic borrowed payload for other track kinds. */
        struct EffNodeDescriptor *effectDescriptor; /* Type 3, 0x14, and 0x15. */
        struct SoundMixer *soundMixerResource; /* Type 0x12 borrowed BED template. */
        struct { u16 resourceGroup, resourceId; } resourceIds;
    };
    EvtGroupMetadata metadata;
    u8 pad20[4];
    sdfTexHandle_t *texture; /* Retained kind-0x18 texture; released with the group. */
    EvtViewerParameterKeySource parameterKeySource; /* 0x28: timeline or pending key. */
    f32 savedPosition[4];
    f32 savedRotation[4];
    EvtObjectAttachmentState attachmentState;
    s32 childCount;
    EvtRuntimeChild *children;
    EvtRuntimeChild *lastChild;
    EvtRuntimeChild *motionKeyCache[4]; /* Last applied key for each model motion channel. */
    u8 pad6C[0x10];
    struct EvtRuntimeGroup *next;
    struct EvtRuntimeGroup *prev;
} EvtRuntimeGroup;

typedef char EvtRuntimeGroup_size[(sizeof(EvtRuntimeGroup) == 0x84) ? 1 : -1];

/* One allocation retained by the EventViewer task. Camp-named timeline
 * helpers receive this same pointer; their old types omitted the voice tail. */
struct evtViewerInfo_t {
    sdfMemHandle_t *block;
    u32 flags;
    PolyMovieWork *workdata;
    s32 totalframe;
    s32 firstframe;
    EvtFrameRange lastframe;
    s32 nowframe;
    s32 oldframe;
    s32 nametablecnt;
    char nametable[256][32];
    dds3Basic_t *activecamera;
    u8 freecamera[4];
    dds3Basic_t *gamecamera;
    s32 objcnt;
    EvtRuntimeGroup *firstobjinfo;
    EvtRuntimeGroup *lastTrack;
    dds3Basic_t *effectBasic[127];
    s32 framedisp;
    evtViewerMenuEntry_t menuStack[8];
    s32 sp;
    s32 nowmenu;
    s32 menusw;
    s32 fldfilter; /* Last type-12 fade area applied by the viewer. */
    struct dds3ProcessID_t *menuProcess;
    s32 drawblur2;
    s32 drawmblur;
    s32 drawdblur;
    s32 filter;
    s32 drawmfilter;
    s32 drawrblur;
    s32 fileMode;
    s32 work_base;
    u8 work_x[4];
    s32 work_y;
    s32 eventpos_y;
    s32 select_y;
    s32 menuItemCount;
    char *select_header;
    char **itemNames;
    s32 filename_x;
    s32 filename_y;
    char nameStorage[0xD];
    u8 unk22E1[7];
    union { char filename[0x14]; struct { char filenameText[0xC]; s32 objlist_y; s32 objlist_base; }; };
    s32 framelist_x;
    s32 frameFirst;
    s32 frameCursor;
    EvtRuntimeGroup *framelist_target;
    s32 framelist_type;
    s32 value;
    s32 min;
    s32 max;
    f32 fvalue;
    f32 floatMin;
    f32 floatMax;
    s32 lightEditorSelectionIndex;
    s32 lightEditorEditMode;
    f32 commandMatrix[12];
    f32 savedCommandMatrix[12];
    s32 messageField;
    s32 compareField;
    s32 fieldIndex;
    s32 lightPanelOffset; /* 0x239C: light/color editor horizontal offset. */
    s32 floatSelection;
    s32 floatEditMode;
    f32 floatEditX;
    f32 floatEditY;
    f32 savedFloatEditX;
    f32 savedFloatEditY;
    u8 savedOverlayFlag;
    u8 pad23B9[3];
    s32 horizontalOffset;
    s32 updateCount;
    s8 windowShadeFade; /* +0x23C4: signed viewer shade ramp, capped at 94. */
    s8 windowActive;
    s16 windowResultVariableIndex; /* VM integer destination for the completed message-window result. */
    s32 tableColumn;
    s32 cameraColorActive;
    sdfTexHandle_t *ch71;
    sdfTexHandle_t *ch72;
    sdfTexHandle_t *ch76;
    sdfTexHandle_t *ch75;
    s32 selectedEntry;
    s32 commandFirst;
    EvtCommandArgument commandSecond;
    EvtCommandArgument commandThird;
    s32 glyphTickCount;
    s32 editField;
    s32 framebufferQuadEnabled;
    const char *frameTextFormat; /* 0x23FC: format of the frame-list cell being measured */
    s32 frameTextWidth;          /* 0x2400: columns advanced by the formatter */
    s32 unk2404;
    s32 colorSelection;
    s32 colorEditorActive;
    u32 glyph;
    s32 timedActive;
    s32 timedStart;
    EvtTimedActionEnd timedEnd;
    u8 shadowMode;
    u8 shadowAlpha;
    u8 pad2422[2];
    f32 shadowY;
    u8 *pendingFrameBuffer; /* Locked byte view used as the descriptor frame image. */
    sdfMemHandle_t *pendingFrameBufferAllocation; /* Owner released after queued draws finish. */
    s32 menuState;
    s32 shopFlag;
    u32 auxResource;
    u8 padRaidou[0x80];
    union { s32 headerMetadata; u32 optionFlags; };
    s32 titleStreamWaitFrames;
    s32 registeredCount;
    s32 registeredIds[20];
    s32 voicePending;
    s32 voiceMessage;
    s32 backgroundFadeStartAlpha; /* Initial background alpha, interpolated to 128 over 25 updates. */
    u8 pad24A4[4];
    s32 voiceFrame;
    u8 pad24AC[4];
    s32 commandStart;
    s32 optionFrameBase;
    s32 curveComponent;
};

struct pmdProcessWork_t;

s32 evtSetGameData1(struct pmdProcessWork_t *wdata, evtViewerInfo_t *pInfo);

#endif
