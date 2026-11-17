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

/* Serialized and embedded group-link metadata share these eight bytes. */
typedef struct evtGroupMetadata_t {
    u8 type;
    u8 flag;
    s16 entry;
    s16 value;
    s8 option; /* +0x06: type-specific group mode chosen by the viewer. */
    s8 extra2;
} evtGroupMetadata_t;

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

typedef char evtGroupMetadata_size[(sizeof(evtGroupMetadata_t) == 8) ? 1 : -1];

/* Complete 0x84-byte entry: metadata, selected object and owned key list. */
typedef struct evtViewerObject_t {
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
        struct sndMixer_t *soundMixerResource; /* Type 0x12 borrowed BED template. */
        struct { u16 resourceGroup, resourceId; } resourceIds;
    };
    evtGroupMetadata_t metadata;
    u8 pad20[4];
    sdfTexHandle_t *texture; /* Retained kind-0x18 texture; released with the group. */
    EvtViewerParameterKeySource parameterKeySource; /* 0x28: timeline or pending key. */
    f32 savedPosition[4];
    f32 savedRotation[4];
    EvtObjectAttachmentState attachmentState;
    s32 childCount;
    evtFrameInfo_t *children;
    evtFrameInfo_t *lastChild;
    evtFrameInfo_t *motionKeyCache[4]; /* Last applied key for each model motion channel. */
    u8 pad6C[0x10];
    struct evtViewerObject_t *next;
    struct evtViewerObject_t *prev;
} evtViewerObject_t;

typedef char evtViewerObject_size[(sizeof(evtViewerObject_t) == 0x84) ? 1 : -1];

typedef struct evtRenderInfo_t {
    void *pImage;
    sdfMemHandle_t *hImageMem;
    s32 stage;
    s32 draw;
    s32 alpha;
} evtRenderInfo_t;

/* Message-select branch: var_idx is the script integer that receives the selection. */
typedef struct evtBranchInfo_t {
    s8 branchfilter_count;
    s8 branchfilter_flag;
    s16 var_idx;
} evtBranchInfo_t;

/* One allocation retained by the EventViewer task. Camp-named timeline
 * helpers receive this same pointer; their old types omitted the voice tail. */
struct evtViewerInfo_t {
    sdfMemHandle_t *block;
    u32 flags;
    struct evtPolygonMovieWork_t *workdata;
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
    evtViewerObject_t *firstobjinfo;
    evtViewerObject_t *lastTrack;
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
    char nameStorage[0xC];
    char extension[8];
    union { char filename[0x14]; struct { char filenameText[0xC]; s32 objlist_y; s32 objlist_base; }; };
    s32 framelist_x;
    s32 frameFirst;
    s32 frameCursor;
    evtViewerObject_t *framelist_target;
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
    u8 pad23C8[8];
    u8 pad23D0[0x20];
    s32 glyphTickCount;
    s32 editField;
    s32 framebufferQuadEnabled;
    const char *frameTextFormat; /* 0x23FC: format of the frame-list cell being measured */
    s32 frameTextWidth;          /* 0x2400: columns advanced by the formatter */
    s32 unk2404;
    u8 pad2408[8];
    s32 unk2410; /* 0/1 viewer-menu toggle (evtCallMenuDisp2) */
    s32 timedActive;
    s32 timedStart;
    EvtTimedActionEnd timedEnd;
    u8 pad2420[8];
    u8 *pendingFrameBuffer; /* Locked byte view used as the descriptor frame image. */
    sdfMemHandle_t *pendingFrameBufferAllocation; /* Owner released after queued draws finish. */
    s32 menuState;
    s32 shopFlag;
    u32 auxResource;
    u8 pad243C[4];
    s32 fadeCount;
    evtBranchInfo_t branch;
    s32 tableColumn;
    u8 pad244C[4];
    sdfTexHandle_t *ch71; /* last texture given to each effect: types 14, 15, 23, 17 */
    sdfTexHandle_t *ch72;
    sdfTexHandle_t *ch76;
    sdfTexHandle_t *ch75;
    s32 selectedEntry;
    s32 commandFirst;
    EvtCommandArgument commandSecond;
    EvtCommandArgument commandThird;
    s32 dontskip_messagetime;
    u8 pad2474[4];
    s32 drawhblur;
    u8 pad247C[0x10];
    s32 colorEditorActive;
    struct frFontHandle_t *glyph; /* camp font string */
    s32 nowMovieStat; /* 1 while a movie plays */
    s32 MovieStartFrame;
    s32 MovieEndFrame;
    u8 shadowMode;
    u8 shadowAlpha;
    u8 pad24A2[2];
    f32 shadowY;
    evtRenderInfo_t renderTexInfo;
    union { s32 headerMetadata; u32 optionFlags; };
    s32 titleStreamWaitFrames;
    s32 registeredCount;
    s32 registeredIds[20];
    s32 At3BgmFlag;
    s32 At3BgmReqID;
    s32 At3Bgm1stFrame;
    s32 At3Bgm1stLoad;
    s32 voiceState;
    s32 voiceMessageNo;
    s32 fadeAlpha; /* Initial background alpha, interpolated to 128 over 25 updates. */
    s32 optionFrameBase;
    s32 voiceFrame; /* nowframe when a voice conflict deferred the message */
    u8 pad253C[0xC];
    s32 curveComponent; /* 0/1: which of the two bezier control points is edited */
    s32 playIndex;
    s32 last_meswin_mode;
    s32 last_mestype;
    u32 tmxTexID; /* UID of the TMX texture applied to model 100, 0 none */
    sdfTexHandle_t *blankTex; /* 32x16 texture from evtCallMemAllocDirect */
    s32 real_movie_last_frame;
};

struct pmdProcessWork_t;

s32 evtSetGameData1(struct evtPolygonMovieWork_t *wdata, evtViewerInfo_t *pInfo);
s32 evtSetGameData2and3(struct evtPolygonMovieWork_t *wdata, evtViewerInfo_t *pInfo, s32 mode);
u16 evtSetGameData2and3Sub(struct evtPolygonMovieWork_t *wdata, s32 index);
s16 evtSetGameData2and3Sub2(struct evtPolygonMovieWork_t *wdata, s32 index);
u16 evtSetGameData2and3Sub3(struct evtPolygonMovieWork_t *wdata, s32 index);
u16 evtSetGameData2and3Sub4(struct evtPolygonMovieWork_t *wdata, s32 index);
union pmdFrameData_t *evtSetGameData2and3Sub5(struct evtPolygonMovieWork_t *wdata, s32 index);
void evtSetGameData2and3Sub6(pmdSceneLightTable_t *light, pmdSceneLightTable1_t *light1);
u32 itfGetMessageNo(ItfMesWindowIndex window);
void evtNotFoundLinkslight(struct evtPolygonMovieWork_t *wdata, evtViewerInfo_t *pInfo, evtGroupMetadata_t *link);

#endif
