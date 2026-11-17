#include "common.h"
#include "raidou_alloc.h"
#include "basic.h"
#include "event.h"
#include "evtPolygonMovie.h"
#include "effect.h"
#include "kernel_draw.h"
#include "ps2asm.h"
#include "kernel.h"
#include "model.h"
#include "motion.h"
#include "dds3SceneBasic.h"
#include "dds3Basic.h"
#include "evtStage.h"
#include "dds3KernelCore.h"
#include "dds3WorldBasic.h"
#include "dds3ObjectBase.h"
#include "dds3CameraObjectBasic.h"
#include "effEvent.h"
#include "effBlur_Filter.h"
#include "dds3EffectObjectBasic.h"
#include "dds3PadManager.h"
#include "mdlManager.h"
#include "mviManager.h"
#include "sndAt3.h"
#include "scrCommand.h"
#include "sdfSound.h"
#include "sdfMotion.h"
#include "sdfMat.h"
#include "sdfTex.h"
#include "sdfModel.h"
#include "sdfMath.h"
#include "dds3PathBasic.h"
#include "dds3MoverBasic.h"
#include "mdlEffect.h"
#include "evtUnitManager.h"
#include "evtEventViewerMenu.h"
#include "evt.h"
#include "evtSound.h"
#include "fldProcess.h"
#include "evtSky.h"
#include "sdfCoreDraw.h"
#include "code_r1_001CC1D0.h"
#include "effManager.h"
#include "sdfChip.h"
#include "sdfDelayedResrc.h"
#include "sdfMemory.h"
#include "frFont.h"

extern s16 winoff_x;
extern sdfColor32_t evtCallGetFrameIDData;
extern s8 dds3PadButtonState[];
extern const char evtStrE303[];
void evtProcess3Sub(evtViewerInfo_t *viewer);
void evtProcess3Sub2(evtViewerInfo_t *viewer);
void MovieStop(evtViewerInfo_t *viewer);
void evtCallEveRippleBlurSetDefTex(evtViewerInfo_t *viewer);
void evtCallGetFrameID(evtViewerInfo_t *viewer);
void PushMenu(s32 a0, s32 a1, s32 a2, evtViewerInfo_t *viewer);

/* The viewer's current menu, which EnterMain switches on to decide what Enter does
 * from each one. Names from the IDB. */
typedef enum EVT_VIEWER_MENU {
    EVT_VIEWER_MENU_NON = 0,
    EVT_VIEWER_MENU_MAIN = 1,
    EVT_VIEWER_MENU_SELECT = 2,
    EVT_VIEWER_MENU_FILENAME = 3,
    EVT_VIEWER_MENU_OBJLIST = 4,
    EVT_VIEWER_MENU_FRAMELIST = 5,
    EVT_VIEWER_MENU_FRAMECHANGE = 6,
    EVT_VIEWER_MENU_INTCHANGE = 7,
    EVT_VIEWER_MENU_FLOATCHANGE = 8,
    EVT_VIEWER_MENU_FILENAME2 = 9,
    EVT_VIEWER_MENU_BASICLIST = 10,
    EVT_VIEWER_MENU_DATA = 11,
    EVT_VIEWER_MENU_EVENTPOS = 12,
    EVT_VIEWER_MENU_MAX = 13
} EVT_VIEWER_MENU;

extern evtFrameInfo_t *evtAddFrameInfo(evtViewerObject_s *track, s32 frame);

typedef struct polyMovieObject_t polyMovieObject_t;

extern u16 evt_file_minor;
extern u16 evtFileMajor;

#include "sdk_libc.h"

evtFrameInfo_t *AddFrameInfo2(evtViewerObject_s *group, s32 frame, evtViewerInfo_t *viewer);
extern s32 (*evtViewerMenuHandlers[])(s32 action, s32 previousMenu);
extern char evtViewerProcNameSfog[5];
extern char evtViewerProcNameFilter[7];
extern char evtViewerProcNameMfilter[8];

#include "dds3obj.h"
#include "newdata.h"
#include "include_asm.h"
#include "evt_world.h"
#include "sdf_resource.h"
#include "billboard.h"
#include "sdf_types.h"
#include "draw.h"
#include "libsdf.h"
#include "sdf_movie_types.h"
#include "field.h"
#include "draw3d.h"
#include "sdf_lighting.h"
#include "dds3UnitObjectBasic.h"
#include "code_r1_001C4B78.h"
#include "evtEventViewer.h"
#include "code_r1_0010DF68.h"
#include "code_r1_001CA8F8.h"
#include "code_r1_001CF808.h"
#include "dds3KernelMain.h"
#include "evtTmxPreLoad.h"
#include "itfMesManager.h"
#include "dds3KernelDraw.h"
#include "effPCPMisc_00224D10.h"

enum {
    DDS3_OBJECT_OWNER_SLOT = 0,
    DDS3_OBJECT_DATA_SLOT = 1,
    DDS3_OBJECT_SCRIPT_MODEL_SLOT = 3,
    DDS3_OBJECT_ACTION_B_SLOT = 4,
    DDS3_OBJECT_HANDLER_SLOT = 5
};

/* Sparse world-node identities recovered from the paired operation tables and
 * concrete creators. Unlisted values stay unnamed; COUNT remains the full
 * directory extent rather than an enum-completeness claim. */
enum {
    EFF_WORLD_KIND_WORLD_INFO = 0,
    EFF_WORLD_KIND_EVENT_TABLE = 1,
    EFF_WORLD_KIND_WORLD_INDEX_OWNER = 2,
    EFF_WORLD_KIND_SLOT_RING = 3,
    EFF_WORLD_KIND_CAMERA = 4,
    EFF_WORLD_KIND_FOLLOW_MODEL = 5,
    EFF_WORLD_KIND_EFFECT_TRANSFORM = 6,
    EFF_WORLD_KIND_EFFECT_OBJECT = 7,
    EFF_WORLD_KIND_RESOURCE_OWNER = 8,
    EFF_WORLD_KIND_LIGHT = 9,
    EFF_WORLD_KIND_SCRIPT_OBJECT = 10,
    EFF_WORLD_KIND_ACTION_B = 11,
    EFF_WORLD_KIND_ACTION_D = 13,
    EFF_WORLD_KIND_ACTION_10 = 16,
    EFF_WORLD_KIND_TRANSFORM_SOURCE = 17,
    EFF_WORLD_KIND_COUNT = 18
};

/* The primary object returned by dds3GetActiveScene is distinct from the active
 * world's WorldInfo payload. Event lookup, lip-sync and viewer code share this chain:
 * object +0x18 -> table +0x08 -> 0x0C-byte slot entries, each with head +0x04.
 * Node payloads depend on the slot. */
enum {
    EVT_WORLD_SLOT_MOVIE = 3, /* viewer movie-time lists: byte offset 0x28 */
    EVT_WORLD_SLOT_CAMERA = 4, /* linked cameras; active nodes supply the view transform */
    EVT_WORLD_SLOT_UNIT = 5   /* unit/group/lip-sync lists: byte offset 0x40 */
};

/* Copy the source object's transform to the target object, with its rotation offset. */

evtFrameInfo_t *evtEventViewerGetPendingNode(evtViewerInfo_t *viewer);

/* Key selecting one of a model group's motion registrations and its matching
 * per-context controller. The public API keeps the native signed word width;
 * each controller stores the key in its existing signed halfword field. */
typedef s32 MdlMotionSearchId;

void evtEventViewerReleasePendingNode(evtViewerInfo_t *viewer);
void evtEventViewerProcessPending(evtViewerInfo_t *viewer);
s32 evtEventViewerCountEntries(evtViewerInfo_t *viewer);
s32 evtEventViewerSumNodeCounts(EvtViewerTrackFileFormat format, evtViewerInfo_t *viewer);

struct sndMixer_t;

struct EffNodeDescriptor;

extern u8 evtViewerHasUpdateFlag(evtViewerInfo_t *);

typedef struct pmdTmxData_t pmdTmxData_t;

void evtApplyViewerTimelineFrame(s32 time, evtViewerInfo_t *viewer);
void evtViewerPushCommandHistory(s32 actionMode, s32 first, s32 second, evtViewerInfo_t *viewer);
dds3TaskUpdateResult_t evtViewerUpdateFrame(dds3ProcessID_t *task);


struct EffNode;

typedef struct evtViewerObjectData_t {
    u8 pad00[0xC];
    struct EffNode *parameterNode;
} evtViewerObjectData_t;

u16 evtViewerPopHistory(evtViewerInfo_t *viewer);
extern char evtViewerTaskName[];

typedef struct pmdTmxData_t pmdTmxData_t;

extern void ValidatePathFrame(s32 endTime, evtViewerInfo_t *viewer);
extern void evtViewerApplyGlyphLodChannel(s32 position, evtViewerInfo_t *viewer);
extern void ValidateUnitPosChange(s32 position, evtViewerInfo_t *viewer);
evtFrameInfo_t *evtViewFindGlyphAtOrBefore(evtViewerInfo_t *viewer);
extern void evtSetMovieClipPositionClampedToDuration(dds3Basic_t *movie, s32 unused, s32 start, s32 end, s32 offset);
void ValidatePathFrame(s32 endTime, evtViewerInfo_t *viewer);

extern evtFrameInfo_t *GetUnitMotionChangeFrame(evtViewerObject_t *group, s32 frame,
                                                             MdlMotionSearchId channel);

extern u32 sdfGetLodChunkValue(sdfModelRootNode_t *model);
void evtViewerApplyGlyphLodChannel(s32 position, evtViewerInfo_t *viewer);
void ValidateUnitPosChange(s32 position, evtViewerInfo_t *viewer);
u8 evtViewerHasUpdateFlag(evtViewerInfo_t *viewer);

evtFrameInfo_t *GetUnitMotionChangeFrame(evtViewerObject_t *group, s32 frame,
                                                      MdlMotionSearchId channel);

enum {
    EVT_VIEWER_PLAYBACK_PAUSED = 0,
    EVT_VIEWER_PLAYBACK_RUNNING = 1,
};

/* Apply the viewer playback mode to unit, motion and movie-object tracks. */
void evtEventViewerApplyPlaybackMode(s32 playbackMode, u32 frame, evtViewerInfo_t *viewer);

u32 evtViewerReturnZero(void);
u32 evtViewerPushMode5CommandHistory(u32 unused0, u32 unused1, evtViewerInfo_t *viewer);
s32 evtViewCmdSetValue(s32 unused0, s32 unused1, evtViewerInfo_t *viewer);
u32 evtViewerStoreCommandInSelectedField(u32 unused0, u32 unused1, evtViewerInfo_t *viewer);
u32 evtViewCmdNoOp(void);
u32 evtViewCmdCancelSelection(u32 unused0, u32 unused1, evtViewerInfo_t *viewer);
u32 evtViewerClearPendingNodeAndPushHistory(u32 unused0, u32 unused1, evtViewerInfo_t *viewer);
dds3TaskUpdateResult_t evtViewerPickNextHandler(dds3ProcessID_t *task);
extern dds3TaskUpdateResult_t evtViewerPickNextHandler(dds3ProcessID_t *task);
void evtViewerRelease();
void evtEventViewerDestroyActive(dds3ProcessID_t *task);
extern void mnuCampInitFontResource(evtViewerInfo_t *viewer);
void evtEventViewerDestroyTask(void);

/* body: matched in smt3 @ 9401f79e: src/smt3/event/evtEventViewer.c:128 */
void LinkFrameInfo(evtViewerObject_t *parent, evtFrameInfo_t *f) {
    evtFrameInfo_t *cursor = parent->children;

    if (cursor == 0) {
        parent->children = f;
        parent->lastChild = f;
        f->next = 0;
        f->prev = 0;
    } else {
        while (cursor != 0) {
            if (f->frame < cursor->frame) {
                if (cursor->prev == 0) {
                    parent->children = f;
                    cursor->prev = f;
                    f->next = cursor;
                    f->prev = 0;
                } else {
                    cursor->prev->next = f;
                    f->prev = cursor->prev;
                    f->next = cursor;
                    cursor->prev = f;
                }
                break;
            }
            cursor = cursor->next;
        }
        if (cursor == 0) {
            evtFrameInfo_t *tailNode = parent->lastChild;
            tailNode->next = f;
            f->prev = parent->lastChild;
            f->next = 0;
            parent->lastChild = f;
        }
    }
    parent->childCount++;
}

/* body: matched in smt3 @ 9401f79e: src/smt3/event/evtEventViewer.c:166 */
void UnlinkFrameInfo(evtViewerObject_t *parent, evtFrameInfo_t *f) {
    evtFrameInfo_t *nextNode = f->next;
    evtFrameInfo_t *previousNode = f->prev;
    if (previousNode == 0) {
        parent->children = nextNode;
    } else {
        previousNode->next = nextNode;
    }
    {
        evtFrameInfo_t *previousNeighbor = f->prev;
        evtFrameInfo_t *nextNeighbor = f->next;
        if (nextNeighbor == 0) {
            parent->lastChild = previousNeighbor;
        } else {
            nextNeighbor->prev = previousNeighbor;
        }
    }
    {
        s32 nodeCount = parent->childCount;
        f->prev = 0;
        f->next = 0;
        parent->childCount = nodeCount - 1;
    }
}

void SortFrameInfo(evtViewerObject_t *parent) {
    if (parent != 0) {
        evtFrameInfo_t *anchorNode = parent->children;
        while (anchorNode != 0) {
            evtFrameInfo_t *resumeNode = anchorNode->next;
            evtFrameInfo_t *candidateNode = resumeNode;
            while (candidateNode != 0) {
                if (candidateNode->frame < anchorNode->frame) {
                    UnlinkFrameInfo(parent, candidateNode);
                    LinkFrameInfo(parent, candidateNode);
                    resumeNode = candidateNode->next;
                    break;
                }
                candidateNode = candidateNode->next;
            }
            anchorNode = resumeNode;
        }
    }
}

evtFrameInfo_t *evtAddFrameInfo_evtViewer(evtViewerObject_t *obj, s32 frame, evtViewerInfo_t *pInfo) {
    pmdTmxData_t display;
    evtBlendKey_t layout;
    f32 first, second;
    pmdSceneLightTable_t *vectors = NULL;
    evtFrameInfo_t *key;

    switch (obj->type) {
    case 10:
        vectors = sdfChipAllocDirect(sizeof(pmdSceneLightTable_t));
        evtGetValueNewSLightFrame(pInfo, obj, frame, vectors, &first, &second);
        break;
    case 24:
        evtGetValueNewTMXFrame(pInfo, pInfo->framelist_target, pInfo->nowframe, &display);
        break;
    case 25:
        break;
    case 36:
        evtGetValueNewTMXFrame(pInfo, pInfo->framelist_target, pInfo->nowframe, &display);
        break;
    }
    key = sdfChipAllocDirect(sizeof(evtFrameInfo_t));
    memset(key, 0, sizeof(evtFrameInfo_t));
    key->frame = frame;
    key->nameIndex = -1;
    LinkFrameInfo(obj, key);

    switch (obj->type) {
    case 2:
        key->p08.f = 0.5235987306f;
        key->nameIndex = evtAddNameTable(pInfo->nametable[obj->entryHeader], pInfo);
        break;
    case 1:
        key->nameIndex = evtAddNameTable(pInfo->nametable[obj->entryHeader], pInfo);
        key->p08.sb[0] = 0;
        key->p0C.sb[2] = 1;
        key->p0C.sb[3] = 20;
        break;
    case 18:
        key->nameIndex = evtAddNameTable(pInfo->nametable[obj->entryHeader], pInfo);
        key->p08.h[0] = 0;
        key->p08.h[1] = -1;
        key->p08.sb[0] = 0;
        break;
    case 20:
    case 21:
        key->nameIndex = evtAddNameTable(pInfo->nametable[obj->entryHeader], pInfo);
        key->p08.sb[0] = -1;
        key->p08.sb[1] = 0;
        key->p08.h[1] = 0;
        key->p0C.sb[0] = -1;
        key->p0C.sb[1] = -1;
        key->p0C.sb[2] = -1;
        key->p0C.sb[3] = -1;
        key->p10.h[0] = 0;
        break;
    case 3:
    case 26:
        key->nameIndex = evtAddNameTable(pInfo->nametable[obj->entryHeader], pInfo);
        key->p08.sb[0] = -1;
        key->p08.h[1] = 0;
        key->p0C.sb[0] = 0;
        key->p0C.sb[1] = -1;
        key->p0C.h[1] = -1;
        key->p10.h[0] = 0;
        break;
    case 9:
        key->nameIndex = evtAddNameTable(pInfo->nametable[obj->entryHeader], pInfo);
        key->p08.h[0] = -1;
        key->p08.h[1] = 1;
        break;
    case 12:
        key->p08.h[0] = 1;
        key->p08.h[1] = 128;
        key->p0C.i = 0;
        break;
    case 22:
        key->p08.h[0] = 1;
        break;
    case 10:
        key->p08.f = first;
        key->payload = vectors;
        key->p0C.f = second;
        key->p10.h[0] = (u16)obj->setterId;
        key->p14.h[0] = 0;
        key->p14.h[1] = 0;
        key->p18.h[0] = 0;
        key->p18.h[1] = 0;
        key->p1C.b[0] = 0;
        key->p1C.b[1] = 0;
        key->p1C.b[2] = 0;
        key->nameIndex = 0;
        break;
    case 11: {
        pmdSceneFogTable_t *fog = sdfChipAllocDirect(sizeof(pmdSceneFogTable_t));
        fog->fog.vec[0] = FogParam.fogMin;
        fog->fog.color = FogParam.color;
        key->payload = fog;
        fog->fog.vec[1] = FogParam.fogMax;
        fog->fog.vec[2] = FogParam.fogNear;
        fog->fog.vec[3] = FogParam.fogFar;
        key->p08.h[0] = 1;
        break;
    }
    case 13: {
        void *payload = sdfChipAllocDirect(sizeof(dds3Effect_Blur_t));
        key->payload = payload;
        memcpy(payload, &BlurEditorGetParams()->source, sizeof(dds3Effect_Blur_t));
        key->p08.h[0] = 1;
        break;
    }
    case 14: {
        void *payload = sdfChipAllocDirect(sizeof(dds3Effect_PartialMultiBlurParam_t));
        key->payload = payload;
        memcpy(payload, &effMultiBlurSwapSub()->body, sizeof(dds3Effect_PartialMultiBlurParam_t));
        key->p08.h[0] = 1;
        key->p10.h[0] = 0;
        break;
    }
    case 15: {
        void *payload = sdfChipAllocDirect(sizeof(dds3Effect_DistortionBlurParam_t));
        key->payload = payload;
        memcpy(payload, &effDistBlurSwapSub()->params, sizeof(dds3Effect_DistortionBlurParam_t));
        key->p08.h[0] = 1;
        break;
    }
    case 23: {
        void *payload = sdfChipAllocDirect(sizeof(dds3Effect_RippleBlurParam_t));
        key->payload = payload;
        memcpy(payload, &Eve_RippleBlurGetParams()->params, sizeof(dds3Effect_RippleBlurParam_t));
        key->p08.h[0] = 1;
        break;
    }
    case 27: {
        void *payload = sdfChipAllocDirect(sizeof(dds3Effect_Blur_t));
        key->payload = payload;
        memcpy(payload, &effGetCh73Params()->source, sizeof(dds3Effect_Blur_t));
        key->p08.h[0] = 1;
        break;
    }
    case 16: {
        void *payload = sdfChipAllocDirect(sizeof(dds3Effect_Filter_t));
        key->payload = payload;
        memcpy(payload, effFilter2SwapSub(), sizeof(dds3Effect_Filter_t));
        key->p08.h[0] = 1;
        break;
    }
    case 17: {
        void *payload = sdfChipAllocDirect(sizeof(dds3Effect_PartialMultiFilterParam_t));
        key->payload = payload;
        memcpy(payload, &effMFilterSwapSub()->param, sizeof(dds3Effect_PartialMultiFilterParam_t));
        key->p08.h[0] = 1;
        break;
    }
    case 24:
    case 36:
        key->nameIndex = evtAddNameTable(pInfo->nametable[obj->entryHeader], pInfo);
        key->p08.sb[0] = display.enabled;
        key->p08.sb[1] = display.variant;
        key->p0C.h[0] = display.x;
        key->p0C.h[1] = display.y;
        key->p10.sb[0] = display.color[0];
        key->p10.sb[1] = display.color[1];
        key->p10.sb[2] = display.color[2];
        key->p10.sb[3] = display.color[3];
        key->p14.f = display.scaleX;
        key->p18.f = display.scaleY;
        key->p1C.b[0] = display.preserveAlpha;
        break;
    case 25: {
        evtCameraColorPayload_t *payload = sdfChipAllocDirect(sizeof(evtCameraColorPayload_t));
        s32 i;

        key->payload = payload;
        payload->parameters.x = layout.x;
        payload->parameters.w[0] = layout.w[0];
        payload->parameters.w[1] = layout.w[1];
        payload->parameters.w[2] = layout.w[2];
        payload->parameters.w[3] = layout.w[3];
        payload->parameters.flagWord = layout.flagWord;
        for (i = 0; i < 3; i++) {
            payload->parameters.y[i] = layout.y[i];
            payload->parameters.z[i] = layout.z[i];
        }
        break;
    }
    case 33:
        key->p08.h[0] = pInfo->nowframe;
        break;
    case 0:
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
    case 19:
    case 28:
    case 29:
    case 30:
    case 31:
    case 32:
    default:
        break;
    }
    return key;
}

evtFrameInfo_t *AddFrameInfo2(evtViewerObject_s *group, s32 frame, evtViewerInfo_t *viewer)
{
    return evtAddFrameInfo(group, frame);
}

evtFrameInfo_t *evtGetSelectFrameInfo(evtViewerInfo_t *viewer) {
    evtViewerObject_t *target;
    evtFrameInfo_t *node;
    s32 i;

    target = viewer->framelist_target;
    if (target == NULL) {
        return NULL;
    }
    node = target->children;
    for (i = 0; i < viewer->frameCursor + viewer->frameFirst && node != NULL; i++) {
        node = node->next;
    }
    return node;
}

/* body: matched in smt3 @ 9401f79e: src/smt3/event/evtEventViewer.c:390 */
void DelFrameInfo(evtViewerObject_t *parent, evtFrameInfo_t *f)
{
    UnlinkFrameInfo(parent, f);
    if (f->payload != NULL) {
        if (parent->type != 0x12) {
            sdfChipFreeDirect(f->payload);
        }
        f->payload = NULL;
    }
    sdfChipFreeDirect(f);
}

void evtDelProcess(evtViewerInfo_t *viewer) {
    evtViewerObject_t *group = viewer->framelist_target;
    evtFrameInfo_t *node = evtGetSelectFrameInfo(viewer);

    switch (group->type) {
    case 3:
    case 20:
    case 21:
    case 26:
        if (node->param.sb[0] >= 0) {
            DelEffectTable_evtEventViewer(node->param.sb[0], viewer);
        }
        break;
    case 18:
        DelBED(node);
        break;
    case 33:
        scrCallPictureSetEndProcess((s16)node->param.sh[1]);
        break;
    }
    DelFrameInfo(group, node);
    if (viewer->frameCursor == 0) {
        if (viewer->frameFirst > 0) {
            viewer->frameFirst--;
        }
    } else if (viewer->frameFirst > 0) {
        viewer->frameFirst--;
    } else {
        viewer->frameCursor--;
    }
}

INCLUDE_ASM(const s32, "event/evtEventViewer", evtCallDelEffectTableEvtEventViewer);

/* Consume queued viewer events until the pending check reports none. */
void evtEventViewerProcessPending(evtViewerInfo_t *viewer) {
    while (evtEventViewerGetPendingNode(viewer) != 0) {
        evtEventViewerReleasePendingNode(viewer);
    }
}

/* body: matched in smt3 @ 9401f79e: src/smt3/event/evtEventViewer.c:436 */
void LinkViewerObject(evtViewerObject_t *obj, evtViewerInfo_t *pInfo) {
    evtViewerObject_t *cursor = pInfo->firstobjinfo;

    if (cursor == 0) {
        pInfo->firstobjinfo = obj;
        pInfo->lastTrack = obj;
        obj->next = 0;
        obj->prev = 0;
    } else {
        while (cursor != 0) {
            if (obj->type < cursor->type) {
                if (cursor->prev == 0) {
                    pInfo->firstobjinfo = obj;
                    cursor->prev = obj;
                    obj->next = cursor;
                    obj->prev = 0;
                } else {
                    cursor->prev->next = obj;
                    obj->prev = cursor->prev;
                    obj->next = cursor;
                    cursor->prev = obj;
                }
                break;
            }
            cursor = cursor->next;
        }
        if (cursor == 0) {
            pInfo->lastTrack->next = obj;
            obj->prev = pInfo->lastTrack;
            obj->next = 0;
            pInfo->lastTrack = obj;
        }
    }
    pInfo->objcnt++;
}

/* body: matched in smt3 @ 9401f79e: src/smt3/event/evtEventViewer.c:472 */
void UnlinkViewerObject(evtViewerObject_t *obj, evtViewerInfo_t *pInfo) {
    if (obj->prev == 0) {
        pInfo->firstobjinfo = obj->next;
    } else {
        obj->prev->next = obj->next;
    }
    if (obj->next == 0) {
        pInfo->lastTrack = obj->prev;
    } else {
        obj->next->prev = obj->prev;
    }
    obj->prev = 0;
    obj->next = 0;
    pInfo->objcnt--;
}

evtViewerObject_t *evtAddViewerObject(s32 type, evtViewerInfo_t *pInfo) {
    evtViewerObject_t *track = sdfChipAllocDirect(sizeof(evtViewerObject_t));

    if (track == NULL) {
        return NULL;
    }
    memset(track, 0, sizeof(evtViewerObject_t));
    track->type = type;
    track->entryHeader = -1;
    track->argument0C = -1;
    LinkViewerObject(track, pInfo);
    return track;
}

s32 evtGetViewerObjectTypeCount(s32 type, evtViewerInfo_t *pInfo)
{
    evtViewerObject_t *obj = pInfo->firstobjinfo;
    s32 count = 0;

    while (obj != NULL) {
        if (obj->type == type) {
            count++;
        }
        obj = obj->next;
    }
    return count;
}

s32 evtEventViewerCountEntries(evtViewerInfo_t *viewer) {
    evtViewerObject_t *entry;
    s32 entryCount;

    entryCount = 0;
    for (entry = viewer->firstobjinfo; entry != NULL; entry = entry->next) {
        entryCount++;
    }
    return entryCount;
}

/* PM2 skips group ids 5/0x13; PM3 serializes only those groups. */
s32 evtEventViewerSumNodeCounts(EvtViewerTrackFileFormat format, evtViewerInfo_t *viewer) {
    evtViewerObject_t *entry;
    s32 total;

    total = 0;
    for (entry = viewer->firstobjinfo; entry != NULL; entry = entry->next) {
        if (format == EVT_VIEWER_TRACK_FILE_FORMAT_PM2) {
            if (entry->type == 5 || entry->type == 0x13) {
                continue;
            }
        } else if (format == EVT_VIEWER_TRACK_FILE_FORMAT_PM3) {
            if (entry->type != 5 && entry->type != 0x13) {
                continue;
            }
        } else {
            continue;
        }
        total += entry->childCount;
    }
    return total;
}

void evtCallDelFrameInfo(evtViewerObject_t *obj, evtViewerInfo_t *pInfo) {
    if (obj->children != NULL) {
        do {
            if (obj->type == 33) {
                scrCallPictureSetEndProcess((s16)obj->children->param.sh[1]);
            }
            DelFrameInfo(obj, obj->children);
        } while (obj->children != NULL);
    }
    if (obj->type == 24) {
        sdfTexReleaseReference(obj->texture);
        obj->texture = NULL;
        while (sdfDelayedResrcCheck()) {
        }
    }
    UnlinkViewerObject(obj, pInfo);
    sdfChipFreeDirect(obj);
}

void evtSetFirstFrame(evtViewerInfo_t *pInfo)
{
    s32 nowframe = pInfo->nowframe;

    pInfo->firstframe = nowframe;
    if (pInfo->lastframe.word < nowframe) {
        pInfo->lastframe.word = nowframe;
    }
}

void evtSetLastFrame(evtViewerInfo_t *pInfo)
{
    s32 nowframe = pInfo->nowframe;

    pInfo->lastframe.word = nowframe;
    if (nowframe < pInfo->firstframe) {
        pInfo->firstframe = nowframe;
    }
}

INCLUDE_ASM(const s32, "event/evtEventViewer", evtCallMemset7);

void FreeInfo(evtViewerInfo_t *pInfo) {
    evtViewerObject_t *obj;

    Eve_PartialMultiBlurSetDefTex();
    Eve_DistortionBlurSetDefTex();
    Eve_RippleBlurSetDefTex();
    Eve_PartialMultiFilterSetDefTex();
    if (pInfo->firstobjinfo != NULL) {
        do {
            evtCallDelFrameInfo(pInfo->firstobjinfo, pInfo);
        } while (pInfo->firstobjinfo != NULL);
    }
    if (pInfo->workdata != NULL) {
        evtFreeWorkData1(pInfo->workdata);
        evtFreeWorkData2and3(pInfo->workdata);
        pInfo->workdata = NULL;
    }
    dds3PadActAllStop();
}

void evtUnRegistUnit(evtViewerInfo_t *pInfo) {
    s32 i;

    if (pInfo->workdata != NULL) {
        for (i = 0; i < pInfo->workdata->unitCount; i++) {
            mdlUnRegist(pInfo->workdata->unitTable[i].MajorNum, pInfo->workdata->unitTable[i].MinorNum);
        }
    }
}

/* body: matched in smt3 @ 9401f79e: src/smt3/event/evtEventViewer.c:595 */
s32 SearchNameTable(char *name, evtViewerInfo_t *pInfo) {
    s32 index = 0;

    if (pInfo->nametablecnt > 0) {
        do {
            if (strcmp(name, pInfo->nametable[index]) == 0) {
                return index;
            }
            index++;
        } while (index < pInfo->nametablecnt);
    }
    return -1;
}

/* body: matched in smt3 @ 9401f79e: src/smt3/event/evtEventViewer.c:610 */
s32 evtAddNameTable(char *name, evtViewerInfo_t *pInfo) {
    s32 existingIndex = SearchNameTable(name, pInfo);
    s32 newIndex;

    if (existingIndex >= 0) {
        return existingIndex;
    }
    newIndex = pInfo->nametablecnt;
    strcpy(pInfo->nametable[newIndex], name);
    pInfo->nametablecnt++;
    return newIndex;
}

dds3Basic_t *SearchBasicFromNameIndex(s32 nameindex, evtViewerInfo_t *pInfo)
{
    if (nameindex < 0) {
        return NULL;
    }
    return dds3SceneBasicSearchNameAndType(dds3GetActiveScene(), 7, pInfo->nametable[nameindex]);
}

void LinkEffectBasic(dds3Basic_t *eff, s32 nameindex, s32 posindex, s32 baseframe, evtViewerInfo_t *pInfo) {
    dds3Basic_t *owner;
    s32 currentFrame;
    void *mover;

    if (eff == NULL) {
        return;
    }
    currentFrame = pInfo->nowframe;
    if (nameindex < 0) {
        return;
    }
    owner = dds3SceneBasicSearchName(dds3GetActiveScene(), pInfo->nametable[nameindex]);
    if (owner == NULL) {
        return;
    }
    switch (owner->type >> 24) {
    case 5:
        if (posindex >= 0) {
            dds3EffectObjectBasicLinkObjectIndex(eff, owner, posindex);
            break;
        }
        /* fallthrough */
    case 4:
    case 6:
    case 7:
    case 8:
    case 9:
    case 16:
        dds3EffectObjectBasicLinkObject(eff, owner);
        break;
    case 15:
        evtPathLinkAndMake(owner, eff);
        mover = dds3GetBaseControlWorkData(eff)->pBasic[1];
        if ((pInfo->flags & 1) != 0) {
            evtStopMover(mover, 0, baseframe, currentFrame);
        } else {
            evtStopMover(mover, 1, baseframe, currentFrame);
        }
        break;
    }
}

s32 AddEffectTable2(s32 nowframe, evtViewerObject_t *object, evtFrameInfo_t *frame, evtViewerInfo_t *pInfo) {
    sdfVector4_t position;
    sdfVector4_t scale;
    dds3Basic_t *effect;
    s32 index;
    s32 type;

    effect = NULL;
    memset(&position, 0, sizeof(position));
    memset(&scale, 0, sizeof(scale));
    scale.w = 1.0f;

    for (index = 0; index < 0x7F; index++) {
        if (pInfo->effectBasic[index] == NULL) {
            break;
        }
    }
    if (index == 0x7F) {
        return -1;
    }

    type = object->type;
    switch (type) {
    case 3:
    case 26:
        if (object->type == 3) {
            effect = dds3EffectObjectBasicAddD3P2(object->resourceData, &position, &scale);
        } else {
            effect = dds3EffectObjectBasicAddEPL(object->resourceData, &position, &scale);
        }
        if (frame->param.sb[4] != 0) {
            dds3EffectObjectPresetMG12(effect);
        }
        if (object->metadata.option == 0) {
            LinkEffectBasic(effect, (s16)frame->param.sh[3], frame->param.sb[5], frame->frame, pInfo);
        } else {
            LinkEffectBasic(effect, (s16)frame->param.sh[3], frame->param.sb[5], 0, pInfo);
        }
        break;
    case 20:
    case 21: {
        dds3Basic_t *part0;
        dds3Basic_t *part1;
        dds3Basic_t *part2;
        dds3Basic_t *part3;

        switch (type) {
        case 20:
            effect = dds3AddMG12EffectObjectBasic2(1, object->resourceData);
            break;
        case 21:
            effect = dds3AddMG12EffectObjectBasic2(2, object->resourceData);
            break;
        }
        part0 = SearchBasicFromNameIndex(frame->param.sb[4], pInfo);
        part1 = SearchBasicFromNameIndex(frame->param.sb[5], pInfo);
        part2 = SearchBasicFromNameIndex(frame->param.sb[6], pInfo);
        part3 = SearchBasicFromNameIndex(frame->param.sb[7], pInfo);
        dds3EffectObjectSetMG12Pos(effect, part0, part1, part2, part3);
        if (frame->param.sb[1] != 0) {
            dds3EffectObjectPresetMG12(effect);
        }
        break;
    }
    }

    pInfo->effectBasic[index] = effect;
    if (effect != NULL) {
        dds3EffectObjectBasicStatOn(effect, 1);
    }
    return index;
}

void DelEffectTable_evtEventViewer(s32 index, evtViewerInfo_t *pInfo) {
    if (pInfo->effectBasic[index] != NULL) {
        dds3DeleteSceneBasicData(pInfo->effectBasic[index]);
        pInfo->effectBasic[index] = NULL;
    }
}

void *AddBED(void *data, s32 cutno, s32 nameindex, s32 posindex, s32 baseframe, evtViewerInfo_t *pInfo) {
    sdfVector4_t position;
    sdfVector4_t scale;
    void *basic;

    memset(&position, 0, sizeof(position));
    memset(&scale, 0, sizeof(scale));
    scale.w = 1.0f;
    basic = dds3AddBEDEffectObjectBasic2(data, &position.x, &scale.x);
    dds3EffectObjectBasicStatOn(basic, 1);
    dds3StartBEDEffectObjectBasic(basic, cutno);
    LinkEffectBasic(basic, nameindex, posindex, baseframe, pInfo);
    return basic;
}

void DelBED(evtFrameInfo_t *frame) {
    if (frame->payload != NULL) {
        dds3DeleteSceneBasicData(frame->payload);
    }
    frame->payload = NULL;
}

mdlObj_t *GetModelHandleFromUID(s32 UID, dds3Basic_t **unit) {
    dds3SceneBasic_t *sceneWork;
    dds3Basic_t *basic;
    mdlObj_t *model;

    sceneWork = dds3GetActiveScene()->workdata;
    for (basic = sceneWork->basictable[BASIC_TYPE_UNITOBJECT].firstbasic; basic != NULL; basic = basic->next) {
        model = ((dds3UnitObjectBasicWorkData_t *)basic->workdata)->pObjectBaseControlWorkData->pModel;
        if (sdfModelGetUniqueID(model->inner) == UID) {
            if (unit != NULL) {
                *unit = basic;
            }
            return model;
        }
    }
    return NULL;
}

sdfMotionCtrlr_t *CreateMotion(mdlObj_t *aHandle, s32 aGroup) {
    sdfMotionPlayBook_t *motionTable;
    s16 motionSlotIndex;
    sdfMotionCtrlr_t *object;

    motionTable = aHandle->sub->playbooks[aGroup].ptr;
    motionSlotIndex = aHandle->sub->playbooks[aGroup].playGroup;
    object = sdfMotionCtrlrNew(aHandle->inner, motionTable);
    object->userId = aGroup;
    aHandle->slots[motionSlotIndex] = object;
    object->userId2 = motionSlotIndex;
    if (motionSlotIndex == 0) {
        aHandle->first = object;
    }
    return object;
}

s32 evtCallGetModelHandleFromUID(evtViewerInfo_t *pInfo, sdfTexHandle_t *pTex) {
    dds3Basic_t *unit;
    mdlObj_t *model;
    sdfModelRootNode_t *root;
    dds3ModelResrc_t *resrc;
    sdfItemArray_t *texList;
    s32 i;

    model = GetModelHandleFromUID(100, &unit);
    if (model == NULL) {
        return 0;
    }
    dds3UnitObjectBasicSetOt(unit, 9);
    root = model->inner;
    resrc = model->sub;
    while (root->motion != NULL) {
        sdfMotionCtrlrFree(root->motion);
    }
    if (root->texList == NULL) {
        return 0;
    }
    if (root->texList->count >= 2) {
        return 0;
    }
    sdfTexListFree(root->texList, 1);
    texList = sdfTexListNew(1);
    sdfTexListAdd(texList, sdfTexDup(pTex));
    sdfModelRootFree(root, 1, 1);
    model->inner = sdfModelMake(texList, resrc->modelDefPtr);
    for (i = 0; i != MDL_SLOT_COUNT; i++) {
        if (resrc->playbooks[i].ptr != NULL && (resrc->playbooks[i].flags & 1)) {
            CreateMotion(model, i);
            if (i > 0 && resrc->playbooks[i].firstMotion >= 0) {
                mdlLoopMotion(model, i, resrc->playbooks[i].firstMotion);
            }
        }
    }
    ValidateUnitMotionFrame(pInfo->nowframe, pInfo, 1);
    return 1;
}

void DrawTmxTextureFrom2Frame(evtViewerInfo_t *pInfo, struct evtViewerObject_t *obj, evtFrameInfo_t *nearframe, evtFrameInfo_t *frame) {
    pmdTmxData_t display;
    f32 ratio = 0.0f;
    sdfIVector2_t maskofs = {0};
    f32 length;
    f32 pos;
    u32 UID;
    sdfTexHandle_t *tex;

    if (nearframe == NULL) {
        return;
    }
    if (frame != NULL) {
        length = frame->frame - nearframe->frame;
        pos = pInfo->nowframe - (nearframe->frame + obj->metadata.value);
        if (length != 0.0f) {
            ratio = pos / length;
        }
    }
    evtHokanTmx(ratio, nearframe, frame, &display);
    switch (obj->type) {
    case 24:
        evtDrawTexture(obj->texture, &display, 0, &maskofs);
        return;
    case 36:
        break;
    default:
        return;
    }
    UID = (pInfo->workdata->eventNo << 16) | nearframe->p08.sh[1];
    tex = evtGetTex(UID);
    if (tex != NULL) {
        if (nearframe->p1C.sb[0] < 3) {
            evtDrawTexture(tex, &display, 0, &maskofs);
            tex = evtGetMaskTex(UID);
            evtGetMaskOfs(UID, &maskofs);
            if (tex != NULL) {
                evtDrawTexture(tex, &display, 1, &maskofs);
            }
        } else if (UID != pInfo->tmxTexID && evtCallGetModelHandleFromUID(pInfo, tex)) {
            pInfo->tmxTexID = UID;
        }
    }
    if (evtGetTex(pInfo->tmxTexID) == NULL && pInfo->tmxTexID != 0) {
        evtCallGetModelHandleFromUID(pInfo, pInfo->blankTex);
        pInfo->tmxTexID = 0;
    }
}

void DrawTmxTexture(evtViewerInfo_t *pInfo) {
    evtViewerObject_t *obj = pInfo->firstobjinfo;
    s32 now = pInfo->nowframe;
    evtFrameInfo_t *frame;
    evtFrameInfo_t *nearframe;

    while (obj != NULL) {
        if (obj->type == 24 || obj->type == 36) {
            if (obj->parameterKeySource == 1) {
                DrawTmxTextureFrom2Frame(pInfo, obj, evtEventViewerGetPendingNode(pInfo), NULL);
            } else {
                for (frame = obj->children; frame != NULL; frame = frame->next) {
                    if (now < frame->frame + obj->metadata.value) {
                        break;
                    }
                }
                if (frame != NULL) {
                    nearframe = frame->prev;
                } else {
                    nearframe = obj->lastChild;
                }
                DrawTmxTextureFrom2Frame(pInfo, obj, nearframe, frame);
            }
        }
        obj = obj->next;
    }
}

void DrawEffect(evtViewerInfo_t *pInfo) {
    if (pInfo->drawblur2 != 0) {
        effDrawBlurRectangle(&BlurEditorGetParams()->source);
    }
    if (pInfo->drawmblur != 0) {
        Eve_PartialMultiBlurON();
    } else {
        Eve_PartialMultiBlurOFF();
    }
    if (pInfo->drawmfilter != 0) {
        Eve_PartialMultiFilterON();
    } else {
        Eve_PartialMultiFilterOFF();
    }
    if (pInfo->drawdblur != 0) {
        Eve_DistortionBlurON();
    } else {
        Eve_DistortionBlurOFF();
    }
    if (pInfo->drawrblur != 0) {
        Eve_RippleBlurON();
    } else {
        Eve_RippleBlurOFF();
    }
    if (pInfo->drawhblur != 0) {
        effEnableFramebufferQuad();
    } else {
        effDisableFramebufferQuad();
    }
    if (pInfo->filter != 0) {
        Eve_FilterON();
    } else {
        Eve_FilterOFF();
    }
    DrawTmxTexture(pInfo);
}

void SetActiveCamera(evtViewerInfo_t *pInfo) {
    dds3Basic_t *camera;

    if (pInfo->activecamera != NULL) {
        camera = pInfo->activecamera;
    } else {
        camera = pInfo->gamecamera;
    }
    if (camera != NULL) {
        dds3SetActiveCameraSceneBasic(dds3GetActiveScene(), camera);
        dds3SetScreenFovy(dds3CameraObjectBasicGetFovy(camera));
    }
}

#ifdef VERSION_RAIDOU1
/* NON_MATCHING: 118 of 122 words match. Fragile: renaming one fldProcess.h prototype swaps two load pairs in the full unit. */
#ifdef NON_MATCHING
void ReqMessage(struct evtViewerObject_t *obj, evtFrameInfo_t *frame, s32 nowframe, s32 length, evtViewerInfo_t *pInfo) {
    s32 mesNo;
    s32 mesType;
    s32 mode;
    s32 conflict;

    mesNo = 0;
    mesType = 0;
    if (pInfo->workdata->MesHandle == -1) {
        return;
    }
    if (evtViewerHasUpdateFlag(pInfo)) {
        return;
    }
    evtGetMessageDataAndTypeFromFrame(frame, &mesNo, &mesType);
    pInfo->last_mestype = mesType;
    if (frame->frame != nowframe) {
        return;
    }
    mode = frame->p0C.sb[0];
    switch (mode) {
    case 0:
    case 1:
        itfMesMngChangeWindowTypeSub(pInfo->workdata->MesHandle, 0, 0);
        itfMesMngMarkBitStat(pInfo->workdata->MesHandle, 0x200000);
        itfMesMngClearBitStat(pInfo->workdata->MesHandle, 0xD00000);
        break;
    case 2:
        itfMesMngChangeWindowTypeSub(pInfo->workdata->MesHandle, 3, 0);
        itfMesMngMarkBitStat(pInfo->workdata->MesHandle, 0x200000);
        itfMesMngMarkBitStat(pInfo->workdata->MesHandle, 0xD00000);
        break;
    }
    pInfo->flags |= 0x100;
    pInfo->last_meswin_mode = frame->p0C.sb[0];
    if (!itfGetMessageType(pInfo->workdata->MesHandle, mesNo)) {
        if (!sndVoiceStatus(frame->p08.sh[1])) {
            return;
        }
        if (scrCheckEnableMovieCancel()) {
            conflict = itfMesSelectTableItemText(pInfo->workdata->MesHandle, mesNo, 0);
            if (conflict == 1) {
                datDebugPrintf("[warning!!!!voice conflict] mesno= %d\n", mesNo);
                scrGetAfterSkipBgmFlag();
                pInfo->voiceState = conflict;
                pInfo->voiceMessageNo = mesNo;
                pInfo->voiceFrame = pInfo->nowframe;
                return;
            }
        }
        itfMesMngRequestMessage_itf(pInfo->workdata->MesHandle, mesNo, 0);
    } else {
        pInfo->branch.var_idx = mesType + 199;
        itfMesMngRequestSelect_itf(pInfo->workdata->MesHandle, mesNo);
    }
}
#else
INCLUDE_ASM(const s32, "event/evtEventViewer", ReqMessage);
#endif
#else
INCLUDE_ASM(const s32, "event/evtEventViewer", ReqMessage);
#endif

void RunFrame1(evtViewerObject_t *obj, evtFrameInfo_t *frame, s32 nowframe, s32 length, evtViewerInfo_t *pInfo) {
    switch (obj->type) {
    case 0:
    case 1:
    case 2:
    case 6:
    case 8:
    case 9:
    case 10:
    case 11:
    case 12:
    case 13:
    case 14:
    case 15:
    case 16:
    case 17:
    case 22:
    case 23:
    case 24:
    case 25:
    case 27:
    case 30:
    case 31:
    case 36:
        break;
    case 3:
    case 20:
    case 21:
    case 26: {
        dds3Basic_t *effect;

        if (nowframe >= frame->frame) {
            if (frame->p08.sb[0] < 0) {
                frame->p08.sb[0] = AddEffectTable2(nowframe, obj, frame, pInfo);
            }
        }
        if (frame->p08.sb[0] < 0) {
            break;
        }
        effect = pInfo->effectBasic[frame->p08.sb[0]];
        if (effect == NULL) {
            break;
        }
        if (obj->type == 21) {
            dds3Basic_t *part0;
            dds3Basic_t *part1;
            dds3Basic_t *part2;
            dds3Basic_t *part3;

            part0 = SearchBasicFromNameIndex(frame->p0C.sb[0], pInfo);
            part1 = SearchBasicFromNameIndex(frame->p0C.sb[1], pInfo);
            part2 = SearchBasicFromNameIndex(frame->p0C.sb[2], pInfo);
            part3 = SearchBasicFromNameIndex(frame->p0C.sb[3], pInfo);
            dds3EffectObjectSetMG12Pos(effect, part0, part1, part2, part3);
            return;
        }
        break;
    }
    case 18:
        if (nowframe == frame->frame) {
            DelBED(frame);
            if (obj->metadata.option == 0) {
                frame->payload = AddBED(obj->soundMixerResource, frame->p0C.sb[0], frame->p08.sh[1], -1, frame->frame, pInfo);
            } else {
                frame->payload = AddBED(obj->soundMixerResource, frame->p0C.sb[0], frame->p08.sh[1], -1, 0, pInfo);
            }
        }
        break;
    case 5:
        if (evtViewerHasUpdateFlag(pInfo) == 1) {
            break;
        }
        if (sndVoiceStatus(frame->p08.sh[1]) == 0) {
            break;
        }
        if (frame->p0C.sb[0] == 0) {
            evtPlaySe(evtFileMajor, frame->p08.sh[0]);
            evtEventRegistBGM(pInfo, frame->p08.sh[0]);
            return;
        } else {
            mnuQueueTitleSoundStreamEntry(frame->p08.sh[0]);
            return;
        }
        break;
    case 19:
        if (evtViewerHasUpdateFlag(pInfo) == 1) {
            break;
        }
        if (sndVoiceStatus(frame->p0C.sh[0]) == 0) {
            break;
        }
        if (frame->p0C.sb[2] == 0) {
            switch (frame->p08.sh[0]) {
            case 0:
                evtPlayEventBgm(evtFileMajor, frame->p08.sh[1]);
                evtEventRegistBGM(pInfo, frame->p08.sh[1]);
                return;
                break;
            case 1:
                evtFadeInEventBgm(evtFileMajor, frame->p08.sh[1]);
                evtEventRegistBGM(pInfo, frame->p08.sh[1]);
                return;
                break;
            case 2:
                evtStopEventBgm(evtFileMajor, frame->p08.sh[1]);
                return;
                break;
            case 3:
                evtTransEventBgm(evtFileMajor, frame->p08.sh[1]);
                return;
                break;
            case 4:
                p2sdBgmVolDown();
                return;
                break;
            case 5:
                p2sdBgmVolUp();
                return;
                break;
            case 6:
                p2sdStopAllBgm();
                return;
                break;
            }
        } else {
            switch (frame->p08.sh[0]) {
            case 0:
                pInfo->At3BgmFlag |= 1;
                pInfo->At3BgmReqID = frame->p08.sh[1];
                break;
            case 1:
                pInfo->At3BgmFlag |= 2;
                pInfo->At3BgmReqID = frame->p08.sh[1];
                break;
            case 2:
                pInfo->At3BgmFlag = 0;
                sndBgmFadeOut();
                return;
                break;
            case 6:
                sndBgmStop();
                sndBgmFree();
                pInfo->At3BgmFlag = 0;
                break;
            case 3:
            case 4:
            case 5:
                break;
            }
        }
        break;
    case 7:
        if (frame->index == 0 && frame->p08.sh[0] == 0) {
            pInfo->flags &= ~2;
            dds3StopQuake();
            return;
        }
        if (nowframe == frame->frame) {
            pInfo->flags |= 2;
            dds3SetQuake(frame->index, frame->p08.sh[0]);
            return;
        }
        break;
    case 4:
        ReqMessage(obj, frame, nowframe, length, pInfo);
        return;
        break;
    case 28:
        if (nowframe == frame->frame + obj->metadata.value) {
            dds3PadAct(frame->p08.sh[0], frame->p08.b[2], frame->index);
            return;
        }
        break;
    case 29:
        if (evtViewerHasUpdateFlag(pInfo) == 1) {
            break;
        }
        if (nowframe != frame->frame) {
            break;
        }
        if (pInfo->nowMovieStat == 0) {
            moviePlay(frame->p08.sh[0]);
            pInfo->nowMovieStat = 1;
            pInfo->MovieStartFrame = frame->frame;
            if (frame->index == 0) {
                pInfo->MovieEndFrame = -1;
            } else {
                pInfo->MovieEndFrame = frame->frame + frame->index;
            }
        } else {
            movieStop();
            moviePlay(frame->p08.sh[0]);
            pInfo->nowMovieStat = 1;
            pInfo->MovieStartFrame = frame->frame;
            if (frame->index == 0) {
                pInfo->MovieEndFrame = -1;
            } else {
                pInfo->MovieEndFrame = frame->frame + frame->index;
            }
        }
        pInfo->real_movie_last_frame = 0;
        break;
    case 32:
        if (nowframe == frame->frame) {
            if (frame->p08.sh[0] == 0) {
                dds3ChangeBistaMode(BISTA_MODE_MOVE);
                dds3BistaIn(frame->index);
                return;
            } else {
                dds3ChangeBistaMode(BISTA_MODE_MOVE);
                dds3BistaOut(frame->index);
                return;
            }
        }
        break;
    case 33:
        if (evtChkUID(frame->p08.sh[1]) == 0 && frame->p08.sh[1] > 0) {
            evtStart(&pInfo->nowframe, pmv_pid, frame->p0C.sh[0] * 16, frame->p0C.sh[1] * 8, 0,
                           frame->frame, frame->p08.sh[0],
                           ((frame->p10.sb[0] + 1) << 16) | (frame->p10.sb[1] + 1), frame->p08.sh[1]);
        }
        break;
    case 34:
        if (!(pInfo->flags & 1)) {
            evtScreenBreakCreate(pmv_pid, 0, frame->p08.sb[0]);
            return;
        }
        break;
    case 35:
        if (!(pInfo->flags & 1)) {
            evtStartCrossFade(pmv_pid, 0, frame->index);
        }
        break;
    }
}

void evtRunFrameSub(evtViewerObject_t *obj, evtFrameInfo_t *frame, s32 nowframe, s32 length, evtViewerInfo_t *pInfo) {
    s32 fade;
    s32 left;
    dds3EffectObjectBasicWorkData_t *work;

    switch (obj->type) {
    case 3:
    case 20:
    case 21:
    case 26:
        if (frame->param.sb[0] < 0 || frame->param.sb[8] == 0) {
            return;
        }
        fade = frame->index;
        left = pInfo->nowframe - frame->frame;
        if (fade < left) {
            break;
        }
        work = pInfo->effectBasic[frame->param.sb[0]]->workdata;
        effSetEffectColorSubSub(work->handle, ((s32)(128.0f / fade * left) << 24) | 0x808080);
        break;
    }
}

void ValidatePLTMX(evtViewerInfo_t *pInfo) {
    s32 prev = -1;
    evtViewerObject_t *obj = pInfo->firstobjinfo;
    s32 now = pInfo->nowframe;
    evtFrameInfo_t *key;
    s32 count;
    s16 slot;

    if (obj == NULL) {
        return;
    }
    while (obj->type != 36) {
        obj = obj->next;
        if (obj == NULL) {
            return;
        }
    }
    for (key = obj->lastChild; key != NULL; key = key->prev) {
        if (now >= key->frame) {
            break;
        }
    }
    if (key == NULL) {
        key = obj->children;
    }
    count = 0;
    while (TRUE) {
        if (key == NULL) {
            break;
        }
        slot = key->p08.sh[1];
        if (slot != prev) {
            count++;
            evtRequest((pInfo->workdata->eventNo << 16) | slot, key->frame);
            prev = slot;
            if (count >= 2) {
                break;
            }
        }
        key = key->next;
    }
}

/* NON_MATCHING: 17 of 103 words match.  */
#ifdef NON_MATCHING
s32 ValidateAt3Bgm(evtViewerInfo_t *pInfo) {
    s32 flags = pInfo->flags;

    if (flags & 0x800) {
        return FALSE;
    }
    if (!(flags & 1)) {
        if (sndSePlay() == 2) {
            sndCallWaitSema();
        }
        if (pInfo->At3BgmFlag == 0) {
            return FALSE;
        }
        if (!(pInfo->At3BgmFlag & 4)) {
            switch (sndBgmStatus_XF()) {
            case 0:
                sndBgmLoadId_XF(pInfo->At3BgmReqID);
                pInfo->At3BgmFlag |= 4;
                break;
            case 1:
                return TRUE;
            case 2:
            case 3:
                if (pInfo->At3BgmFlag & 1) {
                    sndBgmStop();
                    sndBgmFree();
                    break;
                }
                if (!(pInfo->At3BgmFlag & 2)) {
                    return FALSE;
                }
                sndBgmLoadId_XF(pInfo->At3BgmReqID);
                pInfo->At3BgmFlag |= 4;
                return TRUE;
            default:
                return FALSE;
            }
            return TRUE;
        }
        if (pInfo->At3Bgm1stLoad && pInfo->nowframe < pInfo->At3Bgm1stFrame) {
            return FALSE;
        }
        if (sndBgmStatus_XF() != 2) {
            return TRUE;
        }
        if (pInfo->At3BgmFlag & 1) {
            sndBgmPlay();
        }
        if (pInfo->At3BgmFlag & 2) {
            sndBgmFadeIn();
        }
    } else {
        sndBgmStop();
        sndBgmFree();
    }
    pInfo->At3BgmFlag = 0;
    pInfo->At3Bgm1stLoad = FALSE;
    return FALSE;
}
#else
INCLUDE_ASM(const s32, "event/evtEventViewer", ValidateAt3Bgm);
#endif

/* NON_MATCHING: 48 of 82 words match.  */
#ifdef NON_MATCHING
s32 Validate1stAt3Bgm(evtViewerInfo_t *pInfo) {
    s32 found = FALSE;
    evtViewerObject_t *obj;
    evtFrameInfo_t *frame;
    s16 id;

    if (pInfo->flags & 0x800) {
        return FALSE;
    }
    if (pInfo->At3BgmFlag == 0) {
        obj = pInfo->firstobjinfo;
        if (obj == NULL) {
            return FALSE;
        }
        while (obj->type != 19) {
            obj = obj->next;
            if (obj == NULL) {
                return FALSE;
            }
        }
        frame = obj->children;
    next:
        if (frame != NULL) {
            switch (frame->p08.sh[0]) {
            case 0:
                pInfo->At3BgmFlag |= 1;
                sndBgmStop();
                sndBgmFree();
                found = TRUE;
                break;
            case 1:
                found = TRUE;
                pInfo->At3BgmFlag |= 2;
                break;
            }
            if (!found) {
                frame = frame->next;
                goto next;
            }
        }
        if (!found) {
            return FALSE;
        }
        id = frame->p08.sh[1];
        pInfo->At3Bgm1stLoad = TRUE;
        pInfo->At3Bgm1stFrame = frame->frame;
        pInfo->At3BgmReqID = id;
        sndBgmLoadId_XF(id);
        return TRUE;
    }
    if (sndBgmStatus_XF() >= 2) {
        pInfo->At3BgmFlag |= 4;
        return FALSE;
    }
    return TRUE;
}
#else
INCLUDE_ASM(const s32, "event/evtEventViewer", Validate1stAt3Bgm);
#endif

/* NON_MATCHING: 28 of 70 words match.  */
#ifdef NON_MATCHING
void OutFrame1(evtViewerObject_t *obj, evtFrameInfo_t *frame, s32 nowframe, s32 length, evtViewerInfo_t *pInfo) {
    s32 fade;
    s32 end;
    dds3EffectObjectBasicWorkData_t *work;

    switch (obj->type) {
    case 3:
    case 20:
    case 21:
    case 26:
        if (frame->param.sb[0] < 0) {
            break;
        }
        fade = 0;
        if (frame->param.sb[8] == 0) {
            fade = frame->index;
        }
        if (pInfo->nowframe >= frame->frame && pInfo->nowframe < (end = frame->frame + length + fade)) {
            work = pInfo->effectBasic[frame->param.sb[0]]->workdata;
            effSetEffectColorSubSub(work->handle, ((s32)(128.0f / fade * (end - pInfo->nowframe)) << 24) | 0x808080);
            return;
        }
        DelEffectTable_evtEventViewer(frame->param.sb[0], pInfo);
        frame->param.sb[0] = -1;
        break;
    case 18:
        DelBED(frame);
        break;
    case 33:
        break;
    }
}
#else
INCLUDE_ASM(const s32, "event/evtEventViewer", OutFrame1);
#endif

void CalcLightCycle(pmdSceneLightTable_t *sl, evtFrameInfo_t *frame, s32 nowframe, evtViewerInfo_t *pInfo) {
    u16 cycle[3];
    f32 rate[3];
    u16 *pCycle;
    f32 *color;
    s32 elapsed;
    s32 i;
    s32 j;
    f32 angle;

    if (frame == NULL) {
        return;
    }
    elapsed = nowframe - frame->frame;
    cycle[0] = frame->p14.h[1];
    cycle[1] = frame->p18.h[0];
    cycle[2] = frame->p18.h[1];
    rate[0] = frame->p1C.b[0] / 100.0f;
    rate[1] = frame->p1C.b[1] / 100.0f;
    rate[2] = frame->p1C.b[2] / 100.0f;
    for (i = 0, pCycle = cycle; i < 3; i++, pCycle++) {
        if (*pCycle != 0 && rate[i] != 0.0f) {
            angle = (f32)(elapsed % *pCycle) * 360.0f / (f32)*pCycle * 0.017453293f;
            for (j = 0; j < 3; j++) {
                color = &sl->color[i][j];
                *color = *color + *color * sdfSin(angle) * rate[i];
                if (*color < 0.0f) {
                    *color = 0.0f;
                }
                if (*color > 2.0f) {
                    *color = 2.0f;
                }
            }
        }
    }
}

/* NON_MATCHING: 15 of 785 words match.  */
#ifdef NON_MATCHING
extern pmdSceneLightTable_t evtViewerLightParam;
extern evtBlendVecColor_t evtViewerFogParam;
extern dds3Effect_Blur_t evtViewerBlurParam;
extern dds3Effect_PartialMultiBlurParam_t evtViewerMultiBlurParam;
extern dds3Effect_Filter_t evtViewerFilterParam;
extern dds3Effect_PartialMultiFilterParam_t evtViewerMultiFilterParam;
extern dds3Effect_DistortionBlurParam_t evtViewerDistBlurParam;
extern dds3Effect_RippleBlurParam_t evtViewerRippleBlurParam;
extern dds3Effect_Blur_t rf2_hbl;

void RunFrame2(evtViewerObject_t *obj, evtFrameInfo_t *frame, evtFrameInfo_t *nearframe, s32 nowframe, s32 *l, evtViewerInfo_t *pInfo, s32 offset) {
    s32 alpha = 0;
    s32 enable = 1;
    s32 color = 0;
    s32 start, end;
    f32 ratio;

    start = nearframe != NULL ? nearframe->frame : pInfo->firstframe;
    end = frame != NULL ? frame->frame : pInfo->lastframe.word;
    if (end != start) {
        ratio = (f32)(nowframe - start) / (f32)(end - start);
    } else {
        ratio = 0.0f;
    }
    switch (obj->type) {
    case 1:
        break;
    case 2:
        if (nearframe != NULL) {
            s32 distance = nowframe - (nearframe->frame + offset);

            if (distance < *l) {
                *l = distance;
                evt_camera = obj;
            }
        }
        break;
    case 22:
        if (nearframe != NULL && nearframe->p08.sh[0] != 0) {
            evtSetBokasi(1);
        } else {
            evtSetBokasi(0);
        }
        break;
    case 12:
        if (nearframe != NULL) {
            if (pInfo->fldfilter != nearframe->p0C.i) {
                dds3FilterSet(nearframe->p0C.i, nearframe->p08.sh[0], nearframe->index);
                evtSetSkyAlpha(nearframe->index, nearframe->p08.sh[1]);
                pInfo->fldfilter = nearframe->p0C.i;
            } else if (evtCheckSkyAlphaAnime() == 0) {
                if (dds3FilterGetAlpha() != nearframe->p08.sh[1]) {
                    evtSetSkyAlpha(nearframe->index, nearframe->p08.sh[1]);
                }
            }
        } else if (pInfo->fldfilter != 0) {
            dds3FilterSet(0, 1, 0);
            pInfo->fldfilter = 0;
        }
        break;
    case 6:
        if (pInfo->flags & 0x200) {
            break;
        }
        if (evtViewerHasUpdateFlag(pInfo) == 1) {
            break;
        }
        if (nearframe != NULL) {
            f32 fade;
            s32 x[4];
            s32 y[4];

            fade = 1.0f;
            if (nearframe->index != 0 && nowframe < nearframe->frame + nearframe->index) {
                fade = (f32)(nowframe - nearframe->frame) / (f32)nearframe->index;
            }
            switch (nearframe->p08.sh[0]) {
            case 0:
                color = 255;
                dds3FadeModeSet(0);
                alpha = 0;
                if (fade != 1.0f) {
                    alpha = (s32)((1.0f - fade) * 128.0f);
                }
                break;
            case 1:
                color = 255;
                dds3FadeModeSet(0);
                alpha = 128;
                if (fade != 1.0f) {
                    alpha = (s32)(fade * 128.0f);
                }
                break;
            case 2:
                color = 0;
                dds3FadeModeSet(0);
                alpha = 0;
                if (fade != 1.0f) {
                    alpha = (s32)((1.0f - fade) * 128.0f);
                }
                break;
            case 3:
                color = 0;
                dds3FadeModeSet(0);
                alpha = 128;
                if (!(fade >= 0.95f)) {
                    if (fade != 1.0f) {
                        alpha = (s32)(fade * 128.0f);
                    }
                }
                break;
            case 4:
                color = 255;
                dds3FadeModeSet(0);
                alpha = 0;
                if (fade != 1.0f) {
                    x[0] = 0;
                    y[0] = 0;
                    x[1] = nearframe->p0C.sb[0];
                    y[1] = nearframe->p0C.sb[1];
                    x[2] = nearframe->p0C.sb[2];
                    y[2] = nearframe->p0C.sb[3];
                    x[3] = 128;
                    y[3] = 128;
                    alpha = 128 - mnuSolveCubicBezierYForX((s32)(fade * 128.0f), x, y);
                }
                break;
            case 5:
                color = 255;
                dds3FadeModeSet(0);
                alpha = 128;
                if (fade != 1.0f) {
                    x[0] = 0;
                    y[0] = 0;
                    x[1] = nearframe->p0C.sb[0];
                    y[1] = nearframe->p0C.sb[1];
                    x[2] = nearframe->p0C.sb[2];
                    y[2] = nearframe->p0C.sb[3];
                    x[3] = 128;
                    y[3] = 128;
                    alpha = mnuSolveCubicBezierYForX((s32)(fade * 128.0f), x, y);
                }
                break;
            case 6:
                color = 0;
                dds3FadeModeSet(0);
                alpha = 0;
                if (fade != 1.0f) {
                    x[0] = 0;
                    y[0] = 0;
                    x[1] = nearframe->p0C.sb[0];
                    y[1] = nearframe->p0C.sb[1];
                    x[2] = nearframe->p0C.sb[2];
                    y[2] = nearframe->p0C.sb[3];
                    x[3] = 128;
                    y[3] = 128;
                    alpha = 128 - mnuSolveCubicBezierYForX((s32)(fade * 128.0f), x, y);
                }
                break;
            case 7:
                color = 0;
                dds3FadeModeSet(0);
                alpha = 128;
                if (!(fade >= 0.95f)) {
                    if (fade != 1.0f) {
                        x[0] = 0;
                        y[0] = 0;
                        x[1] = nearframe->p14.sb[0];
                        y[1] = nearframe->p14.sb[1];
                        x[2] = nearframe->p14.sb[2];
                        y[2] = nearframe->p14.sb[3];
                        x[3] = 128;
                        y[3] = 128;
                        alpha = mnuSolveCubicBezierYForX((s32)(fade * 128.0f), x, y);
                    }
                }
                break;
            case 8:
                color = 255;
                dds3FadeModeSet(1);
                alpha = 0;
                if (fade != 1.0f) {
                    alpha = (s32)((1.0f - fade) * 128.0f);
                }
                break;
            case 9:
                color = 255;
                dds3FadeModeSet(1);
                alpha = 128;
                if (fade != 1.0f) {
                    alpha = (s32)(fade * 128.0f);
                }
                break;
            }
            pInfo->fadeAlpha = alpha;
            dds3FadeSet(color, color, color, alpha);
            break;
        }
        pInfo->fadeAlpha = 0;
        dds3FadeClear();
        break;
    case 10: {
        pmdSceneLightTable_t *from, *to;
        f32 firstFrom, secondFrom, firstTo, secondTo;
        evtUnit_t *unit;
        s32 kind;

        if (nearframe != NULL) {
            from = nearframe->payload;
            firstFrom = nearframe->p08.f;
            secondFrom = nearframe->p0C.f;
            enable = nearframe->nameIndex;
        } else {
            from = NULL;
            firstFrom = 7.0f;
            secondFrom = 0.0f;
        }
        if (frame != NULL) {
            to = frame->payload;
            firstTo = frame->p08.f;
            secondTo = frame->p0C.f;
        } else {
            to = NULL;
            firstTo = 7.0f;
            secondTo = 0.0f;
        }
        if (from != NULL || to != NULL) {
        evtHokanSceneLight(enable, ratio, from, to, &evtViewerLightParam);
        kind = obj->metadata.option;
        if (kind == 0) {
            break;
        }
        if (kind == 1) {
            s32 i;

            CalcLightCycle(&evtViewerLightParam, nearframe, nowframe, pInfo);
            for (i = 0; i < 3; i++) {
                dds3SetLightColor(0, i, evtViewerLightParam.color[i]);
                dds3SetLightDir(0, i, evtViewerLightParam.direction[i]);
            }
            dds3SetAmbientColor(0, evtViewerLightParam.ambient);
            sdfFaceDivSoften = evtHokanFloat(enable, ratio, firstFrom, firstTo);
            sdfFaceDivBias = evtHokanFloat(enable, ratio, secondFrom, secondTo);
            break;
        }
        unit = NULL;
        if (kind == 3) {
            dds3Basic_t *node = dds3SceneBasicSearchNameAndType(dds3GetActiveScene(), 5,
                                                                pInfo->nametable[obj->metadata.extra2]);

            if (node != NULL) {
                unit = evtGetUnitHandle2(node);
            } else {
                while (1) {
                }
            }
            if (nearframe == NULL) {
                break;
            }
            if (nearframe->index != 0 && nearframe->frame + nearframe->index < nowframe) {
                break;
            }
            if (nearframe->frame == nowframe && nearframe->p14.sh[0] == 1 && !(pInfo->flags & 1)) {
                dds3DrawFlag |= 0x02000000;
                break;
            }
        } else if (kind == 2) {
            if (nearframe != NULL && nearframe->frame == nowframe && nearframe->p14.sh[0] == 1 && !(pInfo->flags & 1)) {
                dds3DrawFlag |= 0x02000000;
                break;
            }
        }
        CalcLightCycle(&evtViewerLightParam, nearframe, nowframe, pInfo);
        evtSetCustomSceneLightData(obj->setterId, obj->metadata.option, unit,
                                   evtViewerLightParam.color, evtViewerLightParam.direction, evtViewerLightParam.ambient);
        evtSetCustomSceneLightData_SoftenAndBias(obj->setterId, evtHokanFloat(enable, ratio, firstFrom, firstTo),
                                                 evtHokanFloat(enable, ratio, secondFrom, secondTo));
        }
        break;
    }
    case 11: {
        evtBlendVecColor_t *from, *to;

        if (nearframe != NULL) {
            from = nearframe->payload;
            enable = nearframe->p08.sh[0];
        } else {
            from = NULL;
        }
        to = frame != NULL ? frame->payload : NULL;
        if (from != NULL || to != NULL) {
            evtHokanSceneFog(enable, ratio, from, to, &evtViewerFogParam);
            dds3SetFogPara(0, evtViewerFogParam.vec[1], evtViewerFogParam.vec[0], evtViewerFogParam.vec[2], evtViewerFogParam.vec[3]);
            FogParam.color = evtViewerFogParam.color;
        }
        break;
    }
    case 13: {
        dds3Effect_Blur_t *from, *to;

        if (nearframe != NULL) {
            from = nearframe->payload;
            enable = nearframe->p08.sh[0];
        } else {
            from = NULL;
        }
        to = frame != NULL ? frame->payload : NULL;
        if (from != NULL || to != NULL) {
            evtHokanBlur2(enable, ratio, from, to, &evtViewerBlurParam);
            BlurSetParam2(&evtViewerBlurParam);
            if ((evtViewerBlurParam.abgr >> 24) == 0) {
                pInfo->drawblur2 = 0;
            } else {
                pInfo->drawblur2 = 1;
            }
        }
        break;
    }
    case 14: {
        dds3Effect_PartialMultiBlurParam_t *from, *to;

        if (nearframe != NULL) {
            from = nearframe->payload;
            enable = nearframe->p08.sh[0];
        } else {
            from = NULL;
        }
        to = frame != NULL ? frame->payload : NULL;
        if (from != NULL || to != NULL) {
            evtHokanMultBlur(enable, ratio, from, to, &evtViewerMultiBlurParam);
            Eve_PartialMultiBlurSetParam(&evtViewerMultiBlurParam);
            if ((evtViewerMultiBlurParam.Blur.abgr >> 24) == 0) {
                pInfo->drawmblur = 0;
            } else {
                pInfo->drawmblur = 1;
            }
        }
        break;
    }
    case 15: {
        dds3Effect_DistortionBlurParam_t *from, *to;

        if (nearframe != NULL) {
            from = nearframe->payload;
            enable = nearframe->p08.sh[0];
        } else {
            from = NULL;
        }
        to = frame != NULL ? frame->payload : NULL;
        if (from != NULL || to != NULL) {
            evtHokanDistBlur(enable, ratio, from, to, &evtViewerDistBlurParam);
            Eve_DistortionBlurSetParam(&evtViewerDistBlurParam);
            if ((evtViewerDistBlurParam.abgr >> 24) == 0) {
                pInfo->drawdblur = 0;
            } else {
                pInfo->drawdblur = 1;
            }
        }
        break;
    }
    case 23: {
        dds3Effect_RippleBlurParam_t *from, *to;

        if (nearframe != NULL) {
            from = nearframe->payload;
            enable = nearframe->p08.sh[0];
        } else {
            from = NULL;
        }
        to = frame != NULL ? frame->payload : NULL;
        if (from != NULL || to != NULL) {
            evtHokanRipBlur(enable, ratio, from, to, &evtViewerRippleBlurParam);
            Eve_RippleBlurSetParam(&evtViewerRippleBlurParam);
            if ((evtViewerRippleBlurParam.abgr >> 24) == 0) {
                pInfo->drawrblur = 0;
            } else {
                pInfo->drawrblur = 1;
            }
        }
        break;
    }
    case 27: {
        dds3Effect_Blur_t *from, *to;

        if (nearframe != NULL) {
            from = nearframe->payload;
            enable = nearframe->p08.sh[0];
        } else {
            from = NULL;
        }
        to = frame != NULL ? frame->payload : NULL;
        if (from != NULL || to != NULL) {
            evtHokanHeikouBlur(enable, ratio, from, to, &rf2_hbl);
            effCopyFramebufferQuadParameters(&rf2_hbl);
            if ((rf2_hbl.abgr >> 24) == 0) {
                pInfo->drawhblur = 0;
            } else {
                pInfo->drawhblur = 1;
            }
        }
        break;
    }
    case 16: {
        dds3Effect_Filter_t *from, *to;

        if (nearframe != NULL) {
            from = nearframe->payload;
            enable = nearframe->p08.sh[0];
        } else {
            from = NULL;
        }
        to = frame != NULL ? frame->payload : NULL;
        if (from != NULL || to != NULL) {
            evtHokanFilter(enable, ratio, from, to, &evtViewerFilterParam);
            Eve_FilterSetParam(&evtViewerFilterParam);
            if (evtViewerFilterParam.rgba[3] == 0) {
                pInfo->filter = 0;
            } else {
                pInfo->filter = 1;
            }
        }
        break;
    }
    case 17: {
        dds3Effect_PartialMultiFilterParam_t *from, *to;

        if (nearframe != NULL) {
            from = nearframe->payload;
            enable = nearframe->p08.sh[0];
        } else {
            from = NULL;
        }
        to = frame != NULL ? frame->payload : NULL;
        if (from != NULL || to != NULL) {
            evtHokanMultFilter(enable, ratio, from, to, &evtViewerMultiFilterParam);
            Eve_PartialMultiFilterSetParam(&evtViewerMultiFilterParam);
            if (evtViewerMultiFilterParam.Filter.color[3] == 0) {
                pInfo->drawmfilter = 0;
            } else {
                pInfo->drawmfilter = 1;
            }
        }
        break;
    }
    case 8:
        if (nearframe != NULL) {
            s16 scale, opacity;
            evtFrameInfo_t *next;

            if (nearframe->p0C.sh[0] == 0 || (next = nearframe->next) == NULL) {
                opacity = nearframe->p08.sh[0];
                scale = nearframe->p08.sh[1];
            } else {
                s32 elapsed = nowframe - nearframe->frame;
                s32 interval = next->frame - nearframe->frame;
                s32 remaining = interval - elapsed;

                opacity = (nearframe->p08.sh[0] * remaining + next->p08.sh[0] * elapsed) / interval;
                scale = (nearframe->p08.sh[1] * remaining + next->p08.sh[1] * elapsed) / interval;
            }
            if (opacity != 0 || scale != 0) {
                pInfo->flags |= 4;
                dds3ScreenBlurFlag = 1;
                dds3ScreenBlurAlpha = opacity;
                dds3ScreenBlurScale = scale;
                break;
            }
        }
        pInfo->flags &= ~4;
        dds3ScreenBlurFlag = 0;
        break;
    }
}
#else
INCLUDE_ASM(const s32, "event/evtEventViewer", RunFrame2);
#endif

void RunFrame3(s32 nowframe, evtViewerInfo_t *pInfo) {
    dds3Basic_t *scene;
    dds3SceneBasic_t *sceneWork;
    dds3Basic_t *outerBasic;
    evtViewerObject_t *track;
    evtFrameInfo_t *frameInfo;
    evtFrameInfo_t *bestFrame;
    dds3Basic_t *lightBasic;
    evtUnit_t *unit;
    s32 bestFrameNumber;

    if (dds3GetActiveScene() == NULL) {
        return;
    }
    scene = dds3GetActiveScene();
    sceneWork = scene->workdata;
    for (outerBasic = sceneWork->basictable[BASIC_TYPE_UNITOBJECT].firstbasic;
         outerBasic != NULL; outerBasic = outerBasic->next) {
        if (outerBasic->name == NULL) {
            continue;
        }
        bestFrame = NULL;
        lightBasic = NULL;
        bestFrameNumber = -1;
        for (track = pInfo->firstobjinfo; track != NULL;
             track = track->next) {
            if (track->type != 9) {
                continue;
            }
            for (frameInfo = track->children; frameInfo != NULL;
                 frameInfo = frameInfo->next) {
                dds3Basic_t *namedBasic;

                if (nowframe < frameInfo->frame + track->metadata.value ||
                    (s16)frameInfo->param.sh[0] < 0) {
                    continue;
                }
                scene = dds3GetActiveScene();
                namedBasic = dds3SceneBasicSearchNameAndType(
                    scene, BASIC_TYPE_UNITOBJECT,
                    pInfo->nametable[
                        (s16)frameInfo->param.sh[0]]);
                if (outerBasic != namedBasic ||
                    frameInfo->frame + track->metadata.value <= bestFrameNumber) {
                    continue;
                }
                bestFrameNumber = frameInfo->frame + track->metadata.value;
                bestFrame = frameInfo;
                lightBasic = track->info;
            }
        }

        unit = evtGetUnitHandle2(outerBasic);
        if (bestFrame != NULL) {
            if ((s16)bestFrame->param.sh[1] != 0) {
                if (unit->currentTransitionValue != EE_ADDR_S(lightBasic) ||
                    (unit->flags & EVT_UNIT_FLAG_TARGET_TRANSITION) == 0) {
                    evtLinkLightObjectBasic(unit, lightBasic, bestFrame->index);
                }
            } else {
                if ((unit->flags & EVT_UNIT_FLAG_TARGET_TRANSITION) == 0) {
                    continue;
                }
                if ((unit->flags & EVT_UNIT_FLAG_TARGET_BLEND_OUT) != 0) {
                    continue;
                }
                evtUnlinkLightObjectBasic(unit, bestFrame->index);
            }
        } else if (unit->currentTransitionValue != 0) {
            evtUnlinkLightObjectBasic(unit, 0);
        }
    }
}

void RunFrameUnit(evtViewerObject_t *track, s32 nowframe) {
    evtFrameInfo_t *key;
    evtFrameInfo_t *cur;
    evtFrameInfo_t *next;
    evtUnit_t *handle = NULL;
    s32 mode = 0;
    s32 otherMode = 2;
    s32 i;
    u32 downflag = 0;
    s32 extMode = 8;
    u32 addflag = 0;
    s32 type;
    f32 ratio;
    s32 start, end;

    key = track->children;
    if (key != NULL) {
        do {
            if (nowframe < key->frame) {
                break;
            }
            switch (key->p08.sb[0]) {
            case 0:
            case 1:
                if (sndVoiceStatus(key->p10.sh[0]) == 1) {
                    mode = key->p08.sb[0];
                }
                break;
            case 2:
            case 3:
            case 4:
                if (sndVoiceStatus(key->p10.sh[0]) == 1) {
                    otherMode = key->p08.sb[0];
                    handle = evtGetUnitHandle2(track->info);
                }
                break;
            case 8:
            case 9:
                extMode = key->p08.sb[0];
                break;
            }
            key = key->next;
        } while (key != NULL);
    }
    for (i = 0; i < 1; i++) {
        cur = NULL;
        for (key = track->children; key != NULL; key = key->next) {
            if ((key->p08.sb[0] == 3 || key->p08.sb[0] == 4) && sndVoiceStatus(key->p10.sh[0]) == 1) {
                if (nowframe >= key->frame) {
                    cur = key;
                } else {
                    break;
                }
            }
        }
        if (cur == NULL) {
            break;
        }
        next = NULL;
        type = cur->p08.sb[0];
        for (key = track->children; key != NULL; key = key->next) {
            if (key->p08.sb[0] == type && sndVoiceStatus(key->p10.sh[0]) == 1 && nowframe < key->frame) {
                next = key;
                break;
            }
        }
        if (next == NULL) {
            handle->ShadowAlpha = cur->p0C.b[1];
            handle->ShadowY = cur->p14.f;
        } else {
            start = cur->frame;
            end = next->frame;
            if (end != start) {
                ratio = (f32)(nowframe - start) / (f32)(end - start);
            } else {
                ratio = 0.0f;
            }
            if (next->p0C.sb[0] == 0 || next->p0C.sb[0] == 2) {
                ratio = 0.0f;
            }
            handle->ShadowAlpha = evtHokanInt(0, ratio, cur->p0C.b[1], next->p0C.b[1]);
            if (next->p0C.sb[0] == 0 || next->p0C.sb[0] == 1) {
                ratio = 0.0f;
            }
            handle->ShadowY = evtHokanFloat(0, ratio, cur->p14.f, next->p14.f);
        }
        if (cur->p0C.sb[0] == 1 || cur->p0C.sb[0] == 3) {
            addflag |= 0x4000;
        } else {
            downflag |= 0x4000;
        }
        switch (cur->p0C.sb[0]) {
        case 2:
        case 3:
            addflag |= 0x8000;
            break;
        default:
            downflag |= 0x8000;
            break;
        }
    }
    evtRunUnitMode(track->info, mode, 0, 0);
    evtRunUnitMode(track->info, otherMode, addflag, downflag);
    if (sdfModelGetUniqueID(evtGetUnitHandle2(track->info)->owner->inner) != 100) {
        evtRunUnitMode(track->info, extMode, 0, 0);
    }
}

void evtRunFrame(s32 nowframe, evtViewerInfo_t *pInfo) {
    evtViewerObject_t *track;
    evtFrameInfo_t *frame;
    evtFrameInfo_t *next;
    evtFrameInfo_t *previous;
    s32 length;
    s32 endFrame;
    s32 offset;
    s32 nearestFrame;
    s32 slot = 0;

    pInfo->drawblur2 = 0;
    pInfo->drawmblur = 0;
    pInfo->filter = 0;
    pInfo->flags &= ~2;
    pInfo->flags &= ~4;
    pInfo->drawmfilter = 0;
    pInfo->drawdblur = 0;
    pInfo->drawrblur = 0;
    pInfo->drawhblur = 0;
    evtInitCustomSceneLight();
    ValidatePathFrame(nowframe, pInfo);
    ValidateUnitMotionFrame(nowframe, pInfo, 0);
    evtViewerApplyGlyphLodChannel(nowframe, pInfo);
    ValidateUnitPosChange(nowframe, pInfo);
    CalcNextMesSet(pInfo);
    evtEventViewerApplyEffectChannels(nowframe, pInfo);
    ValidateMovie(pInfo);
    ValidateRenderTex(pInfo);
    ValidatePLTMX(pInfo);
    for (track = pInfo->firstobjinfo; track != NULL; track = track->next) {
        if (track->type == 1) {
            RunFrameUnit(track, nowframe);
            continue;
        }
        for (frame = track->children; frame != NULL; frame = frame->next) {
            switch (track->type) {
            case 3:
            case 20:
            case 21:
            case 26:
                endFrame = frame->p08.sh[1];
                if (endFrame == 0) {
                    length = pInfo->totalframe - frame->frame;
                } else {
                    length = endFrame - frame->frame;
                    if (length < 0) {
                        length = 0;
                    }
                }
                break;
            case 18:
                endFrame = frame->p08.sh[0];
                if (endFrame == 0) {
                    length = pInfo->totalframe - frame->frame;
                } else {
                    length = endFrame - frame->frame;
                    if (length < 0) {
                        length = 0;
                    }
                }
                break;
            case 9:
                for (next = frame->next; next != NULL; next = next->next) {
                    if (next->p08.sh[0] == frame->p08.sh[0]) {
                        break;
                    }
                }
                if (next != NULL) {
                    length = next->frame - frame->frame;
                } else {
                    length = pInfo->totalframe - frame->frame;
                }
                break;
            case 33:
                length = frame->p08.sh[0] - frame->frame;
                if (length < 0) {
                    length = 0;
                }
                break;
            case 34:
            case 35:
                length = 0;
                break;
            default:
                length = frame->index;
                break;
            }
            if ((length == 0 && nowframe == frame->frame) ||
                (length > 0 && nowframe >= frame->frame && nowframe < frame->frame + length)) {
                RunFrame1(track, frame, nowframe, length, pInfo);
                evtRunFrameSub(track, frame, nowframe, length, pInfo);
            } else {
                OutFrame1(track, frame, nowframe, length, pInfo);
            }
        }
    }
    nearestFrame = 20000;
    evt_camera = NULL;
    for (track = pInfo->firstobjinfo; track != NULL; track = track->next) {
        offset = track->metadata.value;
        for (frame = track->children; frame != NULL; frame = frame->next) {
            if (track->type == 2) {
                if (sndVoiceStatus(frame->p0C.sh[0]) == 0) {
                    continue;
                }
            } else if (track->type == 16 || track->type == 17 || track->type == 12) {
                if (sndVoiceStatus(frame->p14.sh[0]) == 0) {
                    continue;
                }
            }
            if (nowframe < frame->frame + offset) {
                break;
            }
        }
        previous = frame != NULL ? frame->prev : track->lastChild;
        if (track->type == 2 || track->type == 16 || track->type == 17 || track->type == 12) {
            switch (track->type) {
            case 2:
                slot = 1;
                break;
            case 12:
                slot = 3;
                break;
            case 16:
            case 17:
                slot = 3;
                break;
            }
            for (; previous != NULL; previous = previous->prev) {
                if (sndVoiceStatus((s16)previous->param.sh[slot * 2]) == 1) {
                    break;
                }
            }
        }
        RunFrame2(track, frame, previous, nowframe, &nearestFrame, pInfo, offset);
    }
    if (evt_camera != NULL) {
        pInfo->gamecamera = evt_camera->info;
        SetActiveCamera(pInfo);
    }
    RunFrame3(nowframe, pInfo);
    if (pInfo->flags & 1) {
        if (!(pInfo->flags & 2)) {
            dds3StopQuake();
        }
        if (!(pInfo->flags & 4)) {
            dds3ScreenBlurFlag = 0;
        }
    }
}

/* Returns the nearest kind-2 key at/before the current frame, or NULL.
 * Equal-distance ties retain the first key visited. */
evtFrameInfo_t *evtViewFindGlyphAtOrBefore(evtViewerInfo_t *viewer) {
    evtFrameInfo_t *result = NULL;
    s32 best = 99999;
    evtViewerObject_t *node = viewer->firstobjinfo;

    while (node != NULL) {
        if (node->type == 2) {
            evtFrameInfo_t *glyph = node->children;

            if (glyph != NULL) {
                do {
                    s32 frame = glyph->frame;

                    if (frame <= viewer->nowframe) {
                        s32 distance = viewer->nowframe - frame;

                        if (distance < best) {
                            best = distance;
                            result = glyph;
                        }
                    }
                    glyph = glyph->next;
                } while (glyph != NULL);
            }
        }
        node = node->next;
    }
    return result;
}

/* Clamps each movie object's playback interval using its linked track's first
 * key frame (or track offset), endTime, and the current key's extra parameter. */
void ValidatePathFrame(s32 endTime, evtViewerInfo_t *viewer) {
    evtWorldTable_t *worldTable;
    evtWorldSlot_t *worldSlots;
    dds3Basic_t *movieObject;
    evtViewerObject_t *trackEntry;
    s32 startFrame;
    s32 extra;
    evtFrameInfo_t *glyph;

    if (dds3GetActiveScene() != 0) {
        worldTable = (evtWorldTable_t *)((dds3Basic_t *)dds3GetActiveScene())->workdata;
        if (worldTable != 0) {
            worldSlots = worldTable->slots;
            if (worldSlots != 0) {
                movieObject = worldSlots[EVT_WORLD_SLOT_MOVIE].head;
                if (movieObject != 0) {
                    do {
                        trackEntry = viewer->firstobjinfo;
                        while (trackEntry != NULL) {
                            if (trackEntry->info != NULL && movieObject == dds3ObjectBaseGetControlBasic(trackEntry->info, DDS3_OBJECT_DATA_SLOT)) {
                                if (trackEntry->type == 2) {
                                    startFrame = 0;
                                    if (trackEntry->childCount != 0) {
                                        startFrame = trackEntry->children->frame;
                                    }
                                    glyph = evtViewFindGlyphAtOrBefore(viewer);
                                    extra = 0;
                                    if (glyph != NULL) {
                                        extra = glyph->p0C.sh[1];
                                    }
                                } else {
                                    startFrame = trackEntry->metadata.value;
                                    extra = 0;
                                }
                                evtSetMovieClipPositionClampedToDuration(movieObject, 0, startFrame, endTime, extra);
                                break;
                            }
                            trackEntry = trackEntry->next;
                        }
                        movieObject = movieObject->next;
                    } while (movieObject != 0);
                }
            }
        }
    }
}

void evtUnitMotionChange(s32 nowframe, dds3Basic_t *b, evtViewerObject_t *obj, evtViewerInfo_t *pInfo, s32 mode) {
    mdlObj_t *model = ((effObjectData_t *)b->workdata)->modelHolder->pModel;
    evtFrameInfo_t *key;
    s32 i;
    s32 loop;
    s32 blend;
    s32 frame;
    s32 group;

    if (model->first == NULL) {
        return;
    }
    for (i = 0; i < 4; i++) {
        if (mdlGetMotionCtrlr(model, i) == NULL) {
            continue;
        }
        mdlGetMotionCtrlr(model, i)->speed = 0.5f;
        key = GetUnitMotionChangeFrame(obj, nowframe, i);
        if (key == NULL) {
            continue;
        }
        loop = key->p0C.sb[2] == 0;
        blend = key->p18.sh[0];
        if (!loop && nowframe - key->frame + blend > mdlGetMotionLengthEx(model, i) * 2 - 1) {
            mode = 1;
        }
        frame = 0;
        if (blend > 0) {
            mode = 1;
            if (key->p0C.sb[3] > 0) {
                frame = -blend / 2;
            }
        }
        if (mode == 0 && obj->motionKeyCache[i] == key) {
            continue;
        }
        sdfMotionChangeBlend(mdlGetMotionCtrlr(model, i), key->p0C.sb[1], loop, frame, key->p0C.sb[3]);
        group = mdlGetMotionCtrlr(model, i)->userId2;
        mdlStopEffect(model, group);
        mdlGenEffect(model, key->p0C.sb[1], group);
        if (mode == 1) {
            if (loop == 1) {
                if (mdlGetMotionLengthEx(model, i) != 0) {
                    frame = (nowframe - key->frame + blend) % (mdlGetMotionLengthEx(model, i) * 2);
                } else {
                    frame = 0;
                }
            } else if (nowframe - key->frame + blend > mdlGetMotionLengthEx(model, i) * 2 - 1) {
                frame = mdlGetMotionLengthEx(model, i) * 2 - 1;
            } else {
                frame = nowframe - key->frame + blend;
            }
            sdfMotionSeek(mdlGetMotionCtrlr(model, i), frame * 0.5f);
        }
        if (pInfo->flags & 1) {
            sdfMotionPause(mdlGetMotionCtrlr(model, i));
            obj->motionKeyCache[i] = NULL;
        } else {
            sdfMotionPauseOff(mdlGetMotionCtrlr(model, i));
            obj->motionKeyCache[i] = key;
        }
    }
}

void ValidateUnitMotionFrame(s32 nowframe, evtViewerInfo_t *pInfo, s32 mode) {
    dds3SceneBasic_t *sceneWork;
    dds3Basic_t *b;
    evtViewerObject_t *obj;
    evtViewerObject_t *found;

    if (dds3GetActiveScene() == NULL) {
        return;
    }
    sceneWork = dds3GetActiveScene()->workdata;
    for (b = sceneWork->basictable[BASIC_TYPE_UNITOBJECT].firstbasic; b != NULL; b = b->next) {
        found = NULL;
        for (obj = pInfo->firstobjinfo; obj != NULL; obj = obj->next) {
            if (obj->info == b) {
                found = obj;
                break;
            }
        }
        if (found != NULL) {
            evtUnitMotionChange(nowframe, b, obj, pInfo, mode);
        }
    }
}

/* Applies the latest kind-6 key at/before position to a kind-1 track's LOD
 * byte, provided the requested signed-byte level is supported by its chunk. */
void evtViewerApplyGlyphLodChannel(s32 position, evtViewerInfo_t *viewer) {
    evtViewerObject_t *node = viewer->firstobjinfo;
    evtFrameInfo_t *glyph;
    evtFrameInfo_t *best;
    s32 bestFrame;
    sdfModelRootNode_t *lod;
    s8 level;

    while (node != NULL) {
        if (node->type == 1) {
            glyph = node->children;
            bestFrame = -1;
            best = NULL;
            if (glyph != NULL) {
                do {
                    if (position >= glyph->frame && bestFrame < glyph->frame && glyph->p08.sb[0] == 6) {
                        bestFrame = glyph->frame;
                        best = glyph;
                    }
                    glyph = glyph->next;
                } while (glyph != NULL);
            }
            lod = ((mdlObj_t *)((effObjectData_t *)node->info->workdata)->modelHolder->pModel)->inner;
            if (best == NULL) {
                lod->lod = 0;
            } else {
                level = best->p0C.sb[0];
                if ((s32)sdfGetLodChunkValue(lod) >= level) {
                    lod->lod = best->p0C.sb[0];
                }
            }
        }
        node = node->next;
    }
}

/* At an exact kind-7 key frame, attaches the indexed world object. Index -1
 * restores the saved vectors; losing the active key restores them once too. */
void ValidateUnitPosChange(s32 position, evtViewerInfo_t *viewer) {
    evtViewerObject_t *node = viewer->firstobjinfo;

    while (node != NULL) {
        if (node->type == 1) {
            evtFrameInfo_t *glyph = node->children;
            s32 bestFrame = -1;
            evtFrameInfo_t *best = NULL;

            if (glyph != NULL) {
                do {
                    if (glyph->p08.sb[0] == 7 && position >= glyph->frame &&
                        bestFrame < glyph->frame) {
                        bestFrame = glyph->frame;
                        best = glyph;
                    }
                    glyph = glyph->next;
                } while (glyph != NULL);
            }
            if (best == NULL) {
                if (node->attachmentState == EVT_OBJECT_ATTACHMENT_ATTACHED) {
                    dds3SetPosPoseSet(node->info,
                                           (const sdfVector4_t *)node->savedPosition);
                    dds3SetRotPoseSet(node->info,
                                            (const sdfVector4_t *)node->savedRotation);
                    dds3GetPosPoseSet(node->info);
                    SDF_VU0_STOREVECTOR(&node->info->poseset->pos2);
                    node->attachmentState = EVT_OBJECT_ATTACHMENT_DETACHED;
                }
            } else if (best->frame == position) {
                s16 channel = best->p0C.sh[0];

                if (channel == -1) {
                    dds3SetPosPoseSet(node->info,
                                           (const sdfVector4_t *)node->savedPosition);
                    dds3SetRotPoseSet(node->info,
                                            (const sdfVector4_t *)node->savedRotation);
                    dds3GetPosPoseSet(node->info);
                    SDF_VU0_STOREVECTOR(&node->info->poseset->pos2);
                    node->attachmentState = EVT_OBJECT_ATTACHMENT_DETACHED;
                } else {
                    dds3Basic_t *object = dds3SceneBasicSearchName(
                        dds3GetActiveScene(), viewer->nametable[channel]);

                    evtSetUnitPos(
                        node->info->serialID, object->serialID);
                    node->attachmentState = EVT_OBJECT_ATTACHMENT_ATTACHED;
                }
            }
        }
        node = node->next;
    }
}

void evtMessageDelete(evtViewerInfo_t *pInfo, s32 mode) {
    if (pInfo->workdata != NULL && pInfo->workdata->MesHandle != -1) {
        if (mode) {
            switch (pInfo->last_meswin_mode) {
            case 1:
                break;
            case 0:
            case 2:
                itfMesMngMsgWndCls(pInfo->workdata->MesHandle);
                break;
            }
        } else {
            itfMesMngRequestMessageRelease(pInfo->workdata->MesHandle, 1);
            itfMesMngMsgWndCls(pInfo->workdata->MesHandle);
            itfMesMngClearCursor(pInfo->workdata->MesHandle);
        }
        pInfo->flags &= ~0x100;
        itfMesMngSetSelPhase(pInfo->workdata->MesHandle, 0);
        itfMesMngRequestSelectRelease(pInfo->workdata->MesHandle);
        pInfo->branch.branchfilter_flag = 0;
        pInfo->branch.branchfilter_count = 0;
    }
}

s32 rcvIsHealPhase(struct evtPolygonMovieWork_t *wdata) {
    if (wdata->eventNo == 690 && wdata->cutNo == 1) {
        return 1;
    }
    return 0;
}

void CalcNextMesSet(evtViewerInfo_t *pInfo) {
    if (evtViewerHasUpdateFlag(pInfo)) {
        pInfo->fadeCount++;
    }
}

void EventSkipStart(evtViewerInfo_t *pInfo) {
    s32 nowframe;

    if (evtViewerHasUpdateFlag(pInfo) != 1) {
        if (evtFindInputBranchMessage(pInfo) == 1 || !scrRefDDS3GBWK3()) {
            if (pInfo->dontskip_messagetime == 0) {
                pInfo->dontskip_messagetime = 1;
            }
        } else {
            nowframe = pInfo->nowframe;
            if (nowframe < pInfo->lastframe.word - 30 && pInfo->firstframe + 20 < nowframe) {
                pInfo->flags |= 0x10;
                last_skip_stat = 1;
                if (evtEventSkipStartSub(pInfo) == 2) {
                    pInfo->fadeCount = 23;
                } else {
                    pInfo->fadeCount = 0;
                }
                evtEventFadeOutAllBgm(pInfo);
                if (scrCheckEnableMovieCancel() == 1) {
                    scrGetAfterSkipBgmFlag();
                } else if (pInfo->nowMovieStat == 1) {
                    sdfSndSetChannelCount(10);
                }
                if (!rcvIsHealPhase(pInfo->workdata) && sndBgmStatus_XF() == 3) {
                    sndBgmFadeOut();
                }
                evtMessageDelete(pInfo, 0);
            }
        }
    }
}

u8 evtViewerHasUpdateFlag(evtViewerInfo_t *viewer) {
    s32 flags = viewer->flags;
    return (flags & 0x10) > 0;
}

void evtCallGetFrameID(evtViewerInfo_t *pInfo) {
    s32 alpha = 0;

    dds3GetFrameID();
    if (pInfo->fadeCount != 25) {
        alpha = 128.0f - (pInfo->fadeAlpha + (128.0f - pInfo->fadeAlpha) / 25.0f * pInfo->fadeCount);
    }
    if (alpha < 0) {
        alpha = 0;
    }
    if (alpha < 128 && evtViewerHasUpdateFlag(pInfo)) {
        switch (evtEventSkipStartSub(pInfo)) {
        case 0:
            evtCallGetFrameIDData.r = evtCallGetFrameIDData.g = evtCallGetFrameIDData.b = 0;
            break;
        case 1:
            evtCallGetFrameIDData.r = evtCallGetFrameIDData.g = evtCallGetFrameIDData.b = 0xFF;
            break;
        case 2:
            return;
        }
        dds3FadeSet(evtCallGetFrameIDData.r, evtCallGetFrameIDData.g, evtCallGetFrameIDData.b, 128 - alpha);
    }
}

void evtEventViewerApplyEffectChannels(s32 nowframe, evtViewerInfo_t *pInfo) {
    struct evtViewerObject_t *obj;
    evtFrameInfo_t *key;
    sdfTexHandle_t *tex;

    for (obj = pInfo->firstobjinfo; obj != NULL; obj = obj->next) {
        if ((u32)(obj->type - 14) < 2 || obj->type == 23 || obj->type == 17) {
            key = obj->children;
            tex = NULL;
            while (key != NULL) {
                if (obj->type != 17 || sndVoiceStatus(key->p14.sh[0])) {
                    if (nowframe < key->frame + obj->metadata.value) {
                        break;
                    }
                    if (key->p10.sh[0] != 0) {
                        tex = NULL;
                        if (key->p10.sh[0] != 1) {
                            tex = mnuCampFindEntryByName(pInfo, pInfo->nametable[key->p10.sh[0] - 2])->texture;
                        }
                    }
                }
                key = key->next;
            }
            switch (obj->type) {
            case 14:
                if (pInfo->ch71 != tex) {
                    pInfo->ch71 = tex;
                    if (tex == NULL) {
                        Eve_PartialMultiBlurSetDefTex();
                    } else {
                        Eve_PartialMultiBlurSetTex(tex);
                    }
                }
                break;
            case 15:
                if (pInfo->ch72 != tex) {
                    pInfo->ch72 = tex;
                    if (tex == NULL) {
                        Eve_DistortionBlurSetDefTex();
                    } else {
                        Eve_DistortionBlurSetTex(tex);
                    }
                }
                break;
            case 23:
                if (pInfo->ch76 != tex) {
                    pInfo->ch76 = tex;
                    if (tex == NULL) {
                        Eve_RippleBlurSetDefTex();
                    } else {
                        Eve_RippleBlurSetTex(tex);
                    }
                }
                break;
            case 17:
                if (pInfo->ch75 != tex) {
                    pInfo->ch75 = tex;
                    if (tex == NULL) {
                        Eve_PartialMultiFilterSetDefTex();
                    } else {
                        Eve_PartialMultiFilterSetTex(tex);
                    }
                }
                break;
            }
        }
    }
}

void MovieStop(evtViewerInfo_t *pInfo) {
    movieStop();
    pInfo->nowMovieStat = 0;
    pInfo->MovieStartFrame = 0;
    pInfo->MovieEndFrame = 0;
}

void ValidateMovie(evtViewerInfo_t *pInfo) {
    if (pInfo->nowMovieStat == 1) {
        if (pInfo->nowframe < pInfo->MovieStartFrame) {
            MovieStop(pInfo);
            pInfo->real_movie_last_frame = -1;
        } else if (pInfo->MovieEndFrame < 0) {
            if (mviValidateMovieSub() == 1) {
                pInfo->real_movie_last_frame = -1;
            }
        } else if (pInfo->nowframe >= pInfo->MovieEndFrame) {
            movieStop();
            pInfo->nowMovieStat = 0;
            pInfo->MovieStartFrame = 0;
            pInfo->MovieEndFrame = 0;
            pInfo->real_movie_last_frame = -1;
        }
    }
}

/* NON_MATCHING: 65 of 126 words match.  */
#ifdef NON_MATCHING
void ValidateRenderTex(evtViewerInfo_t *pInfo) {
    struct evtViewerObject_t *obj;
    evtFrameInfo_t *key;
    evtFrameInfo_t *found;
    s32 nowframe;
    s32 offset;
    s32 alpha;
    f32 ratio;

    obj = pInfo->firstobjinfo;
    nowframe = pInfo->nowframe;
    if (obj == NULL) {
        return;
    }
    while (obj->type != 31) {
        obj = obj->next;
        if (obj == NULL) {
            return;
        }
    }
    offset = obj->metadata.value;
    for (key = obj->children; key != NULL; key = key->next) {
        if (nowframe < key->frame + offset) {
            break;
        }
    }
    if (key != NULL) {
        found = key->prev;
    } else {
        found = obj->lastChild;
    }
    if (found != NULL && found->frame == nowframe && found->p08.sh[0] == 0) {
        evtRenderTexStart(pInfo);
        }
    found = NULL;
    for (key = obj->children; key != NULL && nowframe >= key->frame + offset; key = key->next) {
        if (key->p08.sh[0] == 1) {
            found = key;
        }
    }
    if (found == NULL) {
        pInfo->renderTexInfo.draw = 0;
        return;
    }
    if (found->p08.sh[1] > 0) {
        pInfo->renderTexInfo.draw = 1;
    }
    for (key = obj->children; key != NULL; key = key->next) {
        if (nowframe < key->frame + offset && key->p08.sh[0] == 1) {
            break;
        }
    }
    if (key == NULL) {
        alpha = found->p08.sh[1];
    } else {
        if (key->frame != found->frame) {
            ratio = (f32)(nowframe - found->frame) / (f32)(key->frame - found->frame);
        } else {
            ratio = 0.0f;
        }
        alpha = evtHokanInt(1, ratio, found->p08.sh[1], key->p08.sh[1]);
    }
    pInfo->renderTexInfo.alpha = alpha;
}
#else
INCLUDE_ASM(const s32, "event/evtEventViewer", ValidateRenderTex);
#endif

void evtProcess3Sub(evtViewerInfo_t *viewer) {
}

void evtCallDrawFont(evtViewerInfo_t *pInfo) {
    s32 time;

    if (pInfo->nowframe < pInfo->lastframe.word - 3 && pInfo->dontskip_messagetime > 0) {
        frDrawFont(pInfo->glyph);
        time = pInfo->dontskip_messagetime + 1;
        pInfo->dontskip_messagetime = time;
        if (time >= 30) {
            pInfo->dontskip_messagetime = 0;
        }
    }
}

void evtProcess3Sub2(evtViewerInfo_t *viewer) {
}

/* Returns the latest eligible kind-5 motion key at/before frame for channel,
 * or NULL. Its condition must pass; equal-frame ties retain the first key. */
evtFrameInfo_t *GetUnitMotionChangeFrame(evtViewerObject_t *group, s32 frame,
                                                      MdlMotionSearchId channel) {
    s32 bestFrame = -1;
    evtFrameInfo_t *best = NULL;
    evtFrameInfo_t *key = group->children;

    if (key != NULL) {
        do {
            if (frame >= key->frame && bestFrame < key->frame && key->p08.sb[0] == 5 &&
                key->p0C.sb[0] == channel &&
                sndVoiceStatus(key->p10.sh[0]) ==
                    EVT_VIEWER_INDEXED_CONDITION_MATCHED) {
                bestFrame = key->frame;
                best = key;
            }
            key = key->next;
        } while (key != NULL);
    }
    return best;
}

void evtViewerStopFrame(s32 sw, u32 nowframe, evtViewerInfo_t *pInfo)
{
    dds3Basic_t *scene;
    dds3SceneBasic_t *sceneWork;
    dds3Basic_t *basic;
    dds3Basic_t *mover;
    dds3MatterObjectBasicWorkData_t *matterWork;
    dds3MoverBasicWorkData_t *moverWork;
    sdfMotionCtrlr_t *motion;
    evtViewerObject_t *obj;
    evtViewerObject_t *found;
    evtFrameInfo_t *frame;
    s32 slot;
    s32 stop;

    if (dds3GetActiveScene() == NULL) {
        return;
    }
    scene = dds3GetActiveScene();
    sceneWork = scene->workdata;
    for (basic = sceneWork->basictable[BASIC_TYPE_UNITOBJECT].firstbasic;
         basic != NULL; basic = basic->next) {
        found = NULL;
        for (obj = pInfo->firstobjinfo; obj != NULL; obj = obj->next) {
            if (obj->info == basic) {
                found = obj;
                break;
            }
        }
        if (found != NULL) {
            evtUnitMotionChange(nowframe, basic, obj, pInfo, 1);
        }
    }
    for (basic = sceneWork->basictable[BASIC_TYPE_MATTEROBJECT].firstbasic;
         basic != NULL; basic = basic->next) {
        matterWork = basic->workdata;
        motion = matterWork->pObjectBaseControlWorkData->pMot;
        if (motion != NULL) {
            if (sw == 0) {
                sdfMotionSeek(motion, (f32)nowframe);
                sdfMotionPause(motion);
            } else {
                sdfMotionPauseOff(motion);
            }
        }
    }
    for (basic = sceneWork->basictable[BASIC_TYPE_MOVER].firstbasic;
         basic != NULL; basic = basic->next) {
        moverWork = basic->workdata;
        if (moverWork->pPathControl != NULL) {
            if (sw == 0) {
                dds3PathBasicPauseOn(moverWork->pPathControl);
            } else {
                dds3PathBasicPauseOff(moverWork->pPathControl);
            }
        }
    }
    for (obj = pInfo->firstobjinfo; obj != NULL; obj = obj->next) {
        switch (obj->type) {
        case 3: case 18: case 20: case 21: case 26:
            break;
        default:
            continue;
        }
        stop = 0;
        for (frame = obj->children; frame != NULL; frame = frame->next) {
            switch (obj->type) {
            case 3: case 26:
                stop = obj->metadata.option;
                /* fall through */
            case 20: case 21:
                slot = frame->param.sb[0];
                if (slot < 0) {
                    continue;
                }
                basic = pInfo->effectBasic[slot];
                break;
            case 18:
                basic = frame->payload;
                stop = obj->metadata.option;
                break;
            default:
                continue;
            }
            if (basic != NULL) {
                mover = dds3GetBaseControlWorkData(basic)->pBasic[1];
                if (mover != NULL) {
                    if (stop == 0) {
                        evtStopMover(mover, sw, frame->frame, nowframe);
                    } else {
                        evtStopMover(mover, sw, 0, nowframe);
                    }
                }
            }
        }
    }
}

/* Dispatch one of two viewer modes based on its lowest flag bit. */
void evtViewerDispatchFlagMode(evtViewerInfo_t *viewer) {
    if ((viewer->flags & 1) != 0) {
        evtEventViewerApplyPlaybackMode(EVT_VIEWER_PLAYBACK_PAUSED, viewer->nowframe, viewer);
        return;
    }
    evtEventViewerApplyPlaybackMode(EVT_VIEWER_PLAYBACK_RUNNING, viewer->nowframe, viewer);
}

/* body: matched in raidou @ 1424963: src/shared/event/evtEventViewer.c:2458, near, same function 0.99 (words 32/32, calls 1.00, shape 0.97) */
evtFrameInfo_t *evtLeaf6(evtViewerInfo_t *viewer) {
    evtFrameInfo_t *result = NULL;
    s32 best = 99999;
    evtViewerObject_t *node = viewer->firstobjinfo;

    while (node != NULL) {
        if (node->type == 2) {
            evtFrameInfo_t *glyph = node->children;

            if (glyph != NULL) {
                do {
                    s32 frame = glyph->frame;

                    if (frame > viewer->nowframe) {
                        s32 distance = frame - viewer->nowframe;

                        if (distance < best) {
                            best = distance;
                            result = glyph;
                        }
                    }
                    glyph = glyph->next;
                } while (glyph != NULL);
            }
        }
        node = node->next;
    }
    return result;
}

/* body: matched in raidou @ 1424963: src/shared/event/evtEventViewer.c:2458, near, same function 0.99 (words 32/32, calls 1.00, shape 0.97) */
evtFrameInfo_t *evtLeaf7(evtViewerInfo_t *viewer) {
    evtFrameInfo_t *result = NULL;
    s32 best = 99999;
    evtViewerObject_t *node = viewer->firstobjinfo;

    while (node != NULL) {
        if (node->type == 2) {
            evtFrameInfo_t *glyph = node->children;

            if (glyph != NULL) {
                do {
                    s32 frame = glyph->frame;

                    if (frame < viewer->nowframe) {
                        s32 distance = viewer->nowframe - frame;

                        if (distance < best) {
                            best = distance;
                            result = glyph;
                        }
                    }
                    glyph = glyph->next;
                } while (glyph != NULL);
            }
        }
        node = node->next;
    }
    return result;
}

void PushMenu(s32 no, s32 x, s32 y, evtViewerInfo_t *viewer) {
    s32 depth = viewer->sp + 1;

    viewer->nowmenu = no;
    viewer->sp = depth;
    viewer->menuStack[depth].mode = no;
    viewer->menuStack[depth].unk02 = x;
    viewer->menuStack[depth].unk04 = y;
    if (no > 0) {
        if (no >= 3) {
            if (no == 3) {
                viewer->filename_x = 0;
                viewer->filename_y = 0;
                viewer->nameStorage[0] = 0;
                viewer->nameStorage[0xC] = 0;
            }
        }
    }
}

u16 evtViewerPopHistory(evtViewerInfo_t *viewer) {
    u16 historyId;
    s32 historyIndex;

    historyIndex = viewer->sp - 1;
    if (viewer->sp == 0) {
        viewer->nowmenu = 0;
        return 0;
    }
    viewer->sp = historyIndex;
    historyId = viewer->menuStack[historyIndex].mode;
    viewer->nowmenu = (u32)historyId;
    return historyId;
}

void MessageManager(evtViewerInfo_t *pInfo) {
    if (pInfo->workdata != NULL) {
        if (pInfo->workdata->MesHandle == -1) {
            return;
        }
        if (itfMesMngGetSelPhase(pInfo->workdata->MesHandle) < 0) {
            itfMesMngSetSelPhase(pInfo->workdata->MesHandle, 0);
            DDS3_GBWK->integers[pInfo->branch.var_idx] = itfMesMngGetSelect(pInfo->workdata->MesHandle);
            itfMesMngRequestSelectRelease(pInfo->workdata->MesHandle);
        }
    }
}

EvtViewerIndexedConditionResult sndVoiceStatus(EvtViewerIndexedCondition condition) {
    u32 index;
    u32 value;

    index = ((u32)condition << 16) >> 28;
    value = (u32)condition & 0xFFF;
    if (index == 0) {
        return EVT_VIEWER_INDEXED_CONDITION_MATCHED;
    }
    return (DDS3_GBWK->integers[199 + index] ^ value) == 0;
}

/* NON_MATCHING: 64 of 112 words match.  */
#ifdef NON_MATCHING
s32 DrawMessage(evtViewerInfo_t *pInfo) {
    u32 flags;
    s32 ret;
    s32 last;
    s32 frame;
    s32 next;

    flags = pInfo->flags;
    ret = 0;
    if (!(flags & 1)) {
        if (pInfo->nowframe >= pInfo->lastframe.word) {
            if (!(flags & 8)) {
                pInfo->flags = flags ^ 1;
                evtViewerDispatchFlagMode(pInfo);
            }
        } else {
            if (pInfo->nowMovieStat == 1) {
                if (mviDrawMessageSub() == 2) {
                    last = pInfo->real_movie_last_frame;
                    if (last >= 0) {
                        frame = movieDecodeWork.frameIndex;
                        if (frame < movieDecodeWork.frameCount) {
                            if (last / 2 == frame) {
                                if (last & 1) {
                                    return 0;
                                }
                                next = frame * 2 + 1;
                            } else {
                                next = frame * 2;
                            }
                        } else {
                            next = -1;
                        }
                        pInfo->real_movie_last_frame = next;
                    }
                    ret = 1;
                    if (pInfo->workdata != NULL && pInfo->workdata->MesHandle != -1
                        && itfMesMngGetPhase(pInfo->workdata->MesHandle) == -1) {
                        evtMessageDelete(pInfo, 1);
                    }
                }
            } else if (pInfo->workdata != NULL && pInfo->workdata->MesHandle != -1) {
                if (flags & 0x100) {
                    if (!itfMesMngChkWaitTag(pInfo->workdata->MesHandle)) {
                        ret = itfMesMngGetSelPhase(pInfo->workdata->MesHandle) <= 0;
                    }
                } else {
                    ret = 1;
                }
                if (pInfo->last_mestype == 0) {
                    if (itfMesMngGetPhase(pInfo->workdata->MesHandle) == -1) {
                        evtMessageDelete(pInfo, 1);
                    }
                } else if (itfMesMngGetSelPhase(pInfo->workdata->MesHandle) == 0) {
                    evtMessageDelete(pInfo, 1);
                }
            } else {
                ret = 1;
            }
        }
    }
    return ret;
}
#else
INCLUDE_ASM(const s32, "event/evtEventViewer", DrawMessage);
#endif

u32 evtViewerReturnZero(void) {
    return 0;
}

/* NON_MATCHING: 26 of 121 words match.  */
#ifdef NON_MATCHING
extern const char evtStrFrameSetOk[], evtStrStartFrameSelect[], evtStrSetBistamode[], evtStrSetSkipmode[];
extern char *evtFRAMESETOKData[], *evtFRAMESETOKData2[], *evtFRAMESETOKData3[], *evtFRAMESETOKData4[];
extern s32 evtData16;

s32 evtFRAMESETOK(s32 arg0, s32 arg1, evtViewerInfo_t *viewer)
{
    switch (viewer->fileMode) {
    case 0:
    case 1:
    case 2:
        viewer->filename_x = 0;
        viewer->filename_y = 0;
        PushMenu(9, 228, 60, viewer);
        break;
    case 5:
        viewer->max = 20000;
        viewer->min = viewer->lastframe.word;
        viewer->value = viewer->totalframe - 1;
        PushMenu(7, 180, 120, viewer);
        break;
    case 3:
    case 4:
        viewer->select_header = (char *)evtStrFrameSetOk;
        viewer->menuItemCount = 2;
        viewer->itemNames = evtFRAMESETOKData;
        viewer->select_y = 0;
        PushMenu(2, 228, 60, viewer);
        break;
    case 6:
        viewer->unk2540 = 1;
        viewer->flags |= 1;
        evtViewerDispatchFlagMode(viewer);
        viewer->nowmenu = 0;
        viewer->menusw = 0;
        viewer->sp = 0;
        evtData16 = 0;
        return 1;
    case 7:
        viewer->select_y = evtMainMenuItemSub(viewer);
        viewer->select_header = (char *)evtStrStartFrameSelect;
        viewer->itemNames = evtFRAMESETOKData4;
        viewer->menuItemCount = 2;
        PushMenu(2, 228, 60, viewer);
        break;
    case 8:
        viewer->min = -5000;
        viewer->max = 5000;
        viewer->value = 0;
        PushMenu(7, 180, 120, viewer);
        break;
    case 9:
        viewer->select_y = evtMainMenuItemSub(viewer);
        viewer->select_header = (char *)evtStrSetBistamode;
        viewer->itemNames = evtFRAMESETOKData2;
        viewer->menuItemCount = 2;
        PushMenu(2, 228, 60, viewer);
        break;
    case 10:
        viewer->select_y = evtEventSkipStartSub(viewer);
        viewer->select_header = (char *)evtStrSetSkipmode;
        viewer->itemNames = evtFRAMESETOKData3;
        viewer->menuItemCount = 3;
        PushMenu(2, 228, 60, viewer);
        break;
    }
    return 0;
}
#else
INCLUDE_RODATA(const s32, "event/evtEventViewer", evtStrWhiteOut);

INCLUDE_RODATA(const s32, "event/evtEventViewer", evtStrBlackOut);

INCLUDE_RODATA(const s32, "event/evtEventViewer", evtStrCurrentFrame);

INCLUDE_RODATA(const s32, "event/evtEventViewer", evtStrFirstFrame);

INCLUDE_ASM(const s32, "event/evtEventViewer", evtFRAMESETOK);
#endif

INCLUDE_ASM(const s32, "event/evtEventViewer", evtCallSetFirstFrame);

u32 evtViewerPushMode5CommandHistory(u32 unused0, u32 unused1, evtViewerInfo_t *viewer) {
    evtViewerPushCommandHistory(5, 0x90, 0x48, viewer);
    return 0;
}

INCLUDE_RODATA(const s32, "event/evtEventViewer", evtStrYesAllDelete);

INCLUDE_RODATA(const s32, "event/evtEventViewer", evtStrWhiteOut2);

INCLUDE_RODATA(const s32, "event/evtEventViewer", evtStrWhiteIn2);

INCLUDE_RODATA(const s32, "event/evtEventViewer", evtStrBlackOutB);

INCLUDE_RODATA(const s32, "event/evtEventViewer", evtStrBlackInB);

INCLUDE_RODATA(const s32, "event/evtEventViewer", evtStrWhiteOutB);

INCLUDE_RODATA(const s32, "event/evtEventViewer", evtStrWhiteInB);

INCLUDE_RODATA(const s32, "event/evtEventViewer", evtStrBlackIn);

INCLUDE_RODATA(const s32, "event/evtEventViewer", evtStrWhiteIn);

INCLUDE_RODATA(const s32, "event/evtEventViewer", evtStrMaskOff);

INCLUDE_RODATA(const s32, "event/evtEventViewer", evtStrPosChange);

INCLUDE_RODATA(const s32, "event/evtEventViewer", evtStrLodSelect);

INCLUDE_RODATA(const s32, "event/evtEventViewer", evtStrMotionChange);

INCLUDE_RODATA(const s32, "event/evtEventViewer", evtStrVshadowOn);

INCLUDE_RODATA(const s32, "event/evtEventViewer", evtStrMshadowOn);

INCLUDE_RODATA(const s32, "event/evtEventViewer", evtStrShadowOff);

INCLUDE_RODATA(const s32, "event/evtEventViewer", evtStrDispOff);

INCLUDE_RODATA(const s32, "event/evtEventViewer", evtStr1StopDraw);

INCLUDE_RODATA(const s32, "event/evtEventViewer", evtStrBistaOut);

INCLUDE_RODATA(const s32, "event/evtEventViewer", evtStrBistaIn);

INCLUDE_RODATA(const s32, "event/evtEventViewer", evtStrFadeOut);

INCLUDE_RODATA(const s32, "event/evtEventViewer", evtStrEndStop2);

INCLUDE_ASM(const s32, "event/evtEventViewer", evtDELETEFRAMEOK);

INCLUDE_ASM(const s32, "event/evtEventViewer", EnterFrameChange);

/* NON_MATCHING: 54 of 167 words match.  */
#ifdef NON_MATCHING
s32 evtCallGetPIDfromUID(s32 unused, s32 command, evtViewerInfo_t *viewer) {
    s32 value = viewer->value;
    f32 fvalue = viewer->fvalue;
    evtFrameInfo_t *key;

    if (viewer->menusw == 0) {
        switch (viewer->fileMode) {
        case 5:
            viewer->totalframe = value + 1;
            evtViewerPopHistory(viewer);
            return 0;
        case 8:
            evtCallMessageDelete(viewer, value, viewer->nowframe);
            mnuAdjustTimelineDurations(viewer, viewer->nowframe, value);
            evtViewerPopHistory(viewer);
            return 0;
        case 7:
            viewer->unk2540 = value;
            viewer->flags |= 1;
            viewer->nowframe = viewer->unk2544;
            evtViewerDispatchFlagMode(viewer);
            viewer->nowmenu = 0;
            viewer->sp = 0;
            viewer->menusw = 0;
            evtData16 = 0;
            return 1;
        }
    } else if (viewer->menusw == 1) {
        if (command == 0xF) {
            viewer->framelist_target->metadata.value = value;
            evtViewerPopHistory(viewer);
            return 0;
        }
    }
    key = evtEventViewerGetPendingNode(viewer);
    if (key == NULL) {
        return 0;
    }
    switch (evtViewerFrameColumnCategories[viewer->framelist_x + viewer->framelist_target->type * 10]) {
    case 1:
        key->index = value;
        break;
    case 2:
        if (viewer->framelist_target->type == 7) {
            key->p08.sh[0] = value;
        }
        break;
    case 3:
        switch (viewer->framelist_target->type) {
        case 8:
            key->p08.sh[0] = value;
            break;
        case 12:
            key->p08.sh[1] = value;
            break;
        case 31:
            key->p08.sh[1] = value;
            break;
        }
        break;
    case 4:
        key->p08.sh[1] = value;
        break;
    case 7:
        key->p08.sh[1] = value;
        break;
    case 6:
        key->p08.f = fvalue * 0.017453292f;
        break;
    case 8:
        switch (viewer->framelist_target->type) {
        case 3:
            key->p0C.sb[1] = value;
            break;
        case 26:
            key->p0C.sb[1] = value;
            break;
        case 4:
            key->p08.sh[0] = value;
            break;
        case 5:
            key->p08.sh[0] = value;
            break;
        case 29:
            key->p08.sh[0] = value;
            break;
        case 30:
            key->p08.sh[0] = value;
            break;
        case 12:
            key->p0C.i = value;
            break;
        case 18:
            key->p0C.sb[0] = value;
            break;
        case 19:
            key->p08.sh[1] = value;
            break;
        case 28:
            key->p08.sh[1] = value;
            break;
        case 33:
        case 36:
            scrCallPictureSetEndProcess(key->p08.sh[1]);
            key->p08.sh[1] = value;
            break;
        }
        break;
    case 22:
        switch (viewer->framelist_target->type) {
        case 1:
            key->p18.sh[0] = value;
            break;
        case 2:
            key->p0C.sh[1] = value;
            break;
        }
        break;
    case 19:
        if (key->p08.sb[0] == 6) {
            key->p0C.sb[0] = value;
        }
        break;
    }
    evtRunFrame(viewer->nowframe, viewer);
    evtViewerPopHistory(viewer);
    return 0;
}
#else
INCLUDE_ASM(const s32, "event/evtEventViewer", evtCallGetPIDfromUID);
#endif

/* Store the command value as a halfword and clear its extra halfword when tagged. */
s32 evtViewCmdSetValue(s32 unused0, s32 unused1, evtViewerInfo_t *viewer) {
    s32 commandValue = viewer->value;
    evtFrameInfo_t *entry = evtEventViewerGetPendingNode(viewer);

    if (entry == NULL) {
        return 0;
    }
    entry->p08.h[0] = commandValue;
    if (((u32)(commandValue << 16) >> 28) != 0) {
        entry->p08.h[1] = 0;
    }
    evtApplyViewerTimelineFrame(viewer->nowframe, viewer);
    evtViewerPopHistory(viewer);
    return 0;
}

/* Store the command value in the selected halfword of a viewer entry. */
u32 evtViewerStoreCommandInSelectedField(u32 unused0, u32 unused1, evtViewerInfo_t *viewer) {
    evtFrameInfo_t *entry;
    u32 commandValue;
    s32 slot;
    commandValue = viewer->value;
    entry = evtEventViewerGetPendingNode(viewer);
    if (entry != 0) {
        slot = viewer->framelist_target->type - 1;
        if ((u32)slot < 0x13u) {
            switch (slot) {
            case 0:
                entry->p10.h[0] = commandValue;
                break;
            case 11:
            case 15:
            case 16:
                entry->p14.h[0] = commandValue;
                break;
            case 3:
                entry->p08.h[1] = commandValue;
                break;
            case 4:
                entry->p08.h[1] = commandValue;
                break;
            case 1:
                entry->p0C.h[0] = commandValue;
                break;
            case 18:
                entry->p0C.h[0] = commandValue;
                break;
            }
        }
        evtApplyViewerTimelineFrame(viewer->nowframe, viewer);
        evtViewerPopHistory(viewer);
        return 0;
    }
    return 0;
}

/* Registered viewer-command handler with no action; return 0. */
u32 evtViewCmdNoOp(void) {
    return 0;
}

u32 evtCallGetPIDfromUID2(u32 unused0, u32 unused1, evtViewerInfo_t *viewer) {
    evtFrameInfo_t *entry = evtEventViewerGetPendingNode(viewer);

    if (entry != NULL) {
        entry->p0C.i = viewer->value;
        entry->p1C.i = viewer->value2;
        evtApplyViewerTimelineFrame(viewer->nowframe, viewer);
        evtViewerPopHistory(viewer);
        return 0;
    }
    return 0;
}

s32 evtEventViewerCommitGroupSelection(s32 unused0, s32 unused1, evtViewerInfo_t *viewer) {
    evtViewerObject_t *obj;
    s32 selection;
    s32 target;
    s32 count;

    selection = viewer->work_base + viewer->work_y;
    if (selection == 0) {
        viewer->framelist_target->metadata.option = 0;
    } else if (selection == 1) {
        viewer->framelist_target->metadata.option = 1;
    } else if (selection == 2) {
        viewer->framelist_target->metadata.option = 2;
    } else {
        viewer->framelist_target->metadata.option = 3;
        target = viewer->work_base + viewer->work_y - 3;
        count = 0;
        for (obj = viewer->firstobjinfo; obj != NULL; obj = obj->next) {
            if (obj->type == 1) {
                if (count == target) {
                    viewer->framelist_target->metadata.extra2 = evtAddNameTable(obj->info->name, viewer);
                    break;
                }
                count++;
            }
        }
    }
    evtApplyViewerTimelineFrame(viewer->nowframe, viewer);
    evtViewerPopHistory(viewer);
    return 0;
}

INCLUDE_ASM(const s32, "event/evtEventViewer", evtCallGetPIDfromUID3);

s32 evtCallGetPIDfromUID4(s32 unused0, s32 unused1, evtViewerInfo_t *viewer) {
    evtFrameInfo_t *entry = evtEventViewerGetPendingNode(viewer);

    if (entry == NULL) {
        return 0;
    }
    if (viewer->framelist_target == NULL) {
        return 0;
    }
    entry->p08.f = viewer->unk2428;
    entry->p0C.f = viewer->unk242C;
    evtApplyViewerTimelineFrame(viewer->nowframe, viewer);
    evtViewerPopHistory(viewer);
    return 0;
}

u32 evtViewCmdCancelSelection(u32 unused0, u32 unused1, evtViewerInfo_t *viewer) {
    evtViewerPopHistory(viewer);
    return 0;
}

/* NON_MATCHING: 52 of 53 words match.  */
#ifdef NON_MATCHING
s32 evtViewCmdResolveSlot(s32 operation, void *argument, evtViewerInfo_t *viewer) {
    s32 count = 0;
    evtFrameInfo_t *key;
    evtViewerObject_t *obj;
    s32 selection;

    key = evtEventViewerGetPendingNode(viewer);
    selection = viewer->work_base + viewer->work_y;
    if (selection == 0) {
        key->p10.h[0] = 0;
    } else if (selection == 1) {
        key->p10.h[0] = 1;
    } else {
        s32 ordinal = selection - 2;

        obj = viewer->firstobjinfo;
        while (obj != NULL) {
            if (obj->type == 0x18) {
                if (count == ordinal) {
                    key->p10.h[0] = (u16)obj->entryHeader + 2;
                    break;
                }
                count++;
            }
            obj = obj->next;
        }
    }
    evtApplyViewerTimelineFrame(viewer->nowframe, viewer);
    evtViewerPopHistory(viewer);
    return 0;
}
#else
INCLUDE_ASM(const s32, "event/evtEventViewer", evtViewCmdResolveSlot);
#endif

s32 evtCallGetPIDfromUID5(s32 unused0, s32 unused1, evtViewerInfo_t *viewer) {
    evtFrameInfo_t *entry = evtEventViewerGetPendingNode(viewer);

    entry->p0C.b[0] = viewer->shadowMode;
    entry->p0C.b[1] = viewer->shadowAlpha;
    entry->p14.f = viewer->shadowY;
    evtApplyViewerTimelineFrame(viewer->nowframe, viewer);
    evtViewerPopHistory(viewer);
    return 0;
}

INCLUDE_ASM(const s32, "event/evtEventViewer", evtCallSprintf);

INCLUDE_ASM(const s32, "event/evtEventViewer", evtViewCmdSetObjectTarget);

u32 evtViewerClearPendingNodeAndPushHistory(u32 unused0, u32 unused1, evtViewerInfo_t *viewer) {
    if (evtEventViewerGetPendingNode(viewer) != 0) {
        viewer->work_base = 0;
        viewer->work_y = 0;
        evtViewerPushCommandHistory(0xa, 0x9c, 0x54, viewer);
        return 0;
    }
    return 0;
}

INCLUDE_RODATA(const s32, "event/evtEventViewer", evtStrE303);

s32 EnterData(s32 unused0, s32 unused1, evtViewerInfo_t *viewer) {
    evtFrameInfo_t *entry = evtEventViewerGetPendingNode(viewer);
    pmdSceneLightTable_t *light;
    pmdSceneFogTable_t *fog;
    s32 i;

    switch (viewer->framelist_target->type) {
    case 10:
        switch (viewer->framelist_type) {
        case 13:
            light = entry->payload;
            for (i = 0; i < 3; i++) {
                f32 *color = light->color[i];
                sdf3DLight_t *src = dds3LightParam.parallel[i];
                SDF_VU0_COPYVECTOR(color, &src->color);
                SDF_VU0_COPYVECTOR(light->direction[i], &src->v);
            }
            SDF_VU0_COPYVECTOR(light->ambient, &dds3LightParam.ambient);
            break;
        case 14:
            entry->p08.f = sdfFaceDivSoften;
            entry->p0C.f = sdfFaceDivBias;
            break;
        }
        break;
    case 11:
        fog = entry->payload;
        fog->fog.vec[0] = dds3FogState.fogMin;
        fog->fog.color = dds3FogState.color;
        fog->fog.vec[1] = dds3FogState.fogMax;
        fog->fog.vec[2] = dds3FogState.fogNear;
        fog->fog.vec[3] = dds3FogState.fogFar;
        break;
    case 13:
        memcpy(entry->payload, &BlurEditorGetParams()->source, sizeof(dds3Effect_Blur_t));
        break;
    case 14:
        memcpy(entry->payload, mblurGetParam(), sizeof(dds3Effect_PartialMultiBlurParam_t));
        break;
    case 15:
        memcpy(entry->payload, dblurGetParam(), sizeof(dds3Effect_DistortionBlurParam_t));
        break;
    case 23:
        memcpy(entry->payload, rblurGetParam(), sizeof(dds3Effect_RippleBlurParam_t));
        break;
    case 27:
        memcpy(entry->payload, effGetLoadDescD(), sizeof(dds3Effect_Blur_t));
        break;
    case 16:
        memcpy(entry->payload, dblurGetParam2(), sizeof(dds3Effect_Filter_t));
        break;
    case 17:
        memcpy(entry->payload, mfilterGetParam(), sizeof(dds3Effect_PartialMultiFilterParam_t));
        break;
    case 12:
    case 18:
    case 19:
    case 20:
    case 21:
    case 22:
    case 24:
    case 25:
    case 26:
    case 28:
    case 29:
    case 30:
    case 31:
    case 32:
    case 33:
        break;
    }
    evtRunFrame(viewer->nowframe, viewer);
    evtViewerPopHistory(viewer);
    return 0;
}

/* Hand control back to the frame update whether or not the task still owns a viewer. */
dds3TaskUpdateResult_t evtViewerPickNextHandler(dds3ProcessID_t *task) {
    if (dds3GetProcessData(task) != 0) {
        return (s32)evtViewerUpdateFrame;
    }
    return (s32)evtViewerUpdateFrame;
}

void evtCallEveRippleBlurSetDefTex(evtViewerInfo_t *pInfo) {
    evtViewerObject_t *obj;
    evtViewerObject_t *nextobj;
    evtFrameInfo_t *frame;
    evtFrameInfo_t *nextframe;

    Eve_PartialMultiBlurSetDefTex();
    Eve_DistortionBlurSetDefTex();
    Eve_RippleBlurSetDefTex();
    Eve_PartialMultiFilterSetDefTex();
    for (obj = pInfo->firstobjinfo; obj != NULL; obj = nextobj) {
        nextobj = obj->next;
        switch (obj->type) {
        case 0:
            break;
        case 1:
        case 2:
        case 3:
        case 18:
        case 20:
        case 21:
        case 24:
        case 26:
            for (frame = obj->children; frame != NULL; frame = nextframe) {
                nextframe = frame->next;
                switch (obj->type) {
                case 3:
                case 20:
                case 21:
                case 26:
                    if (frame->param.sb[0] >= 0) {
                        DelEffectTable_evtEventViewer(frame->param.sb[0], pInfo);
                    }
                    break;
                case 18:
                    DelBED(frame);
                    break;
                }
                DelFrameInfo(obj, frame);
            }
            break;
        default:
            evtCallDelFrameInfo(obj, pInfo);
            break;
        }
    }
    dds3PadActAllStop();
}

void UnitMotionChangeForWait(s32 nowframe, dds3Basic_t *b, evtViewerObject_t *obj, evtViewerInfo_t *pInfo) {
    mdlObj_t *model = ((effObjectData_t *)b->workdata)->modelHolder->pModel;
    evtFrameInfo_t *key;
    s32 i;
    s32 type;

    if (model->first != NULL) {
        for (i = 0; i < 4; i++) {
            if (mdlGetMotionCtrlr(model, i) == NULL) {
                continue;
            }
            mdlGetMotionCtrlr(model, i)->speed = 0.5f;
            key = GetUnitMotionChangeFrame(obj, nowframe, i);
            if (key == NULL) {
                continue;
            }
            if (mdlGetMotionCtrlr(model, i)->stat == SDF_MOTION_STAT_NOCUT && key->p1C.sb[0] != 0) {
                type = key->p1C.sb[2] == 0;
                sdfMotionChangeBlend(mdlGetMotionCtrlr(model, i), key->p1C.sb[1], type, 0.0f, key->p1C.sb[3]);
            }
        }
    }
}

void evtValidateUnitMotionFrameForWait(s32 nowframe, evtViewerInfo_t *pInfo) {
    dds3SceneBasic_t *sceneWork;
    dds3Basic_t *b;
    evtViewerObject_t *obj;
    evtViewerObject_t *found;

    if (dds3GetActiveScene() == NULL) {
        return;
    }
    sceneWork = dds3GetActiveScene()->workdata;
    for (b = sceneWork->basictable[BASIC_TYPE_UNITOBJECT].firstbasic; b != NULL; b = b->next) {
        found = NULL;
        for (obj = pInfo->firstobjinfo; obj != NULL; obj = obj->next) {
            if (obj->info == b) {
                found = obj;
                break;
            }
        }
        if (found != NULL) {
            UnitMotionChangeForWait(nowframe, b, obj, pInfo);
        }
    }
}

#ifdef NON_MATCHING
/* 47 of 190 words differ: same length; two branch delay slots (after the flags & 8 test in
 * the tail) stay unfilled where retail fills them. */
dds3ProcessFunc_t Process3(dds3ProcessID_t *task) {
    evtViewerInfo_t *viewer = (evtViewerInfo_t *)dds3GetProcessData(task);
    s32 frame;
    s32 flags;
    s32 flagsNow;
    s32 playIndex;

    winoff_x = 0;
    if (!dds3BgSetScrollSub() && viewer->framedisp == 1) {
        evtProcess3Sub(viewer);
    }
    DrawEffect(viewer);
    flags = viewer->flags;
    if (!(flags & 8)) {
        if (dds3PadButtonState[0x22] < 0) {
            viewer->flags = flags | 1;
            evtViewerDispatchFlagMode(viewer);
            viewer->nowmenu = 0;
            viewer->sp = 0;
            viewer->menusw = 0;
            PushMenu(1, 0x24, 0x18, viewer);
            return (dds3ProcessFunc_t)evtViewerPickNextHandler;
        }
        evtProcess3Sub2(viewer);
    } else if (viewer->nowframe == viewer->firstframe) {
        viewer->flags = flags & ~1;
        evtViewerDispatchFlagMode(viewer);
    } else if (dds3PadButtonState[0x2C] < 0) {
        EventSkipStart(viewer);
    }
    if (viewer->voiceState == 1) {
        if (!evtViewerHasUpdateFlag(viewer) && !scrCheckEnableMovieCancel()) {
            itfMesMngRequestMessage_itf(viewer->workdata->MesHandle, viewer->voiceMessageNo, 0);
            viewer->voiceState = 0;
            datDebugPrintf("[conflict voice play      ] mesno= %d\n", viewer->voiceMessageNo);
        }
    }
    frame = viewer->nowframe;
    if (frame != viewer->oldframe) {
        evtRunFrame(frame, viewer);
    } else if (!(viewer->flags & 1)) {
        evtValidateUnitMotionFrameForWait(frame, viewer);
    }
    if (ValidateAt3Bgm(viewer) && viewer->nowframe < 2) {
        return NULL;
    }
    viewer->oldframe = viewer->nowframe;
    if (viewer->flags & 8) {
        SetActiveCamera(viewer);
    }
    MessageManager(viewer);
    evtCallGetFrameID(viewer);
    evtCallAllocRenderTexture(viewer);
    flagsNow = viewer->flags;
    if (flagsNow & 8) {
        if (flagsNow & 0x20) {
            return evtChkLoad(1) ? NULL : (dds3ProcessFunc_t)-1;
        }
        if (viewer->nowframe >= viewer->lastframe.word) {
            viewer->flags |= 0x80;
        }
        if (flagsNow & 0x40) {
            if (viewer->nowMovieStat == 1) {
                MovieStop(viewer);
                viewer->real_movie_last_frame = -1;
            }
            playIndex = viewer->playIndex;
            viewer->workdata->seqIndex = playIndex;
            viewer->flags &= ~0xC0;
            evtCallEveRippleBlurSetDefTex(viewer);
            evtGetWorkData2and3(viewer->workdata, viewer->workdata->pm2Cuts[playIndex], viewer->workdata->pm3Cuts[playIndex]);
            evtSetGameData2and3(viewer->workdata, viewer, 1);
            return NULL;
        }
        if (evtViewerHasUpdateFlag(viewer) == 1 && viewer->fadeCount >= 25) {
            return (dds3ProcessFunc_t)-1;
        }
    }
    viewer->nowframe += DrawMessage(viewer);
    return NULL;
}
#else
INCLUDE_ASM(const s32, "event/evtEventViewer", Process3);
#endif

#ifdef VERSION_RAIDOU1
dds3ProcessFunc_t Process2_evtViewer(dds3ProcessID_t *id) {
    evtViewerInfo_t *pInfo = dds3GetProcessData(id);

    if (!evtProcessStart(NULL, DDS3_KERNELPROC_PRIORITY)) {
        evtRunFrame(pInfo->nowframe, pInfo);
        evtViewerDispatchFlagMode(pInfo);
        pInfo->blankTex = evtCallMemAllocDirect();
    }
    if (evtChkLoad(1) || Validate1stAt3Bgm(pInfo)) {
        dds3DrawFlag |= 0x2000000;
        return NULL;
    }
    if (pInfo->lastframe.word >= 10) {
        dds3KernelDebugSign = 0;
    }
    evtRunFrame(pInfo->nowframe, pInfo);
    dds3LinkProcess(id, evtInitFrameVar());
    dds3DrawFlag |= 0x2000000;
    pmv_pid = id;
    last_skip_stat = 0;
    return (dds3ProcessFunc_t)Process3;
}
#else
INCLUDE_ASM(const s32, "event/evtEventViewer", Process2_evtViewer);
#endif

INCLUDE_ASM(const s32, "event/evtEventViewer", evtCallGetProcessData10);

dds3ProcessFunc_t evtProcess2_RealPlay(dds3ProcessID_t *id) {
    evtViewerInfo_t *pInfo = dds3GetProcessData(id);
    u32 flags;

    evtCheckLoad(pInfo->workdata);
    flags = pInfo->workdata->loadFlags;
    if (!(flags & 8)) {
        dds3DrawFlag |= 0x2000000;
        return NULL;
    }
    if (flags & 1) {
        return NULL;
    }
    evtSetGameData1(pInfo->workdata, pInfo);
    evtSetGameData2and3(pInfo->workdata, pInfo, 0);
    dds3DrawFlag |= 0x2000000;
    return (dds3ProcessFunc_t)Process2_evtViewer;
}

#ifdef NON_MATCHING
/* 7 words off, register allocation: the work pointer lands in a different register. */
dds3TaskUpdateResult_t evtCallGetProcessData11(dds3ProcessID_t *task) {
    evtViewerInfo_t *viewer = (evtViewerInfo_t *)dds3GetProcessData(task);
    evtPolygonMovieWork_t *work = viewer->workdata;
    u16 minor = work->cutNoLow;
    u16 major = work->eventNoLow;

    evt_file_minor = minor;
    evtFileMajor = major;
    sprintf(viewer->filename, evtStrE303, major, minor);
    viewer->flags |= 1;
    evtViewerDispatchFlagMode(viewer);
    viewer->framedisp = 0;
    viewer->flags |= 8;
    dds3DrawFlag |= 0x2000000;
    return EE_ADDR(evtProcess2_RealPlay);
}
#else
INCLUDE_ASM(const s32, "event/evtEventViewer", evtCallGetProcessData11);
#endif

s32 evtEndEvtViewerSub(struct evtPolygonMovieWork_t *wdata) {
    return 0;
}

#ifdef VERSION_RAIDOU1
s32 End_evtViewer(evtViewerInfo_t *pInfo) {
    f32 zero;

    if (!evtEndEvtViewerSub(pInfo->workdata)) {
        evtEventFadeOutAllBgm(pInfo);
    }
    evtInitCustomSceneLight();
    evtMessageDelete(pInfo, 0);
    mnuCampLinkFontGlyph(pInfo);
    dds3StopQuake();
    zero = 0.0f;
    gMapScrollReset[3] = zero;
    gMapScrollReset[2] = zero;
    gMapScrollReset[1] = zero;
    gMapScrollReset[0] = zero;
    if (pInfo->nowMovieStat == 1) {
        if (pInfo->MovieEndFrame == -2 && evtViewerHasUpdateFlag(pInfo) != 1) {
            pInfo->nowMovieStat = 0;
        } else {
            movieStop();
            pInfo->nowMovieStat = 0;
        }
    }
    while (sdfDelayedResrcCheck()) {
    }
    evtFreeStage();
    while (sdfDelayedResrcCheck()) {
    }
    if (pInfo->renderTexInfo.hImageMem != NULL) {
        sdfMemFreeDelayedDirect(pInfo->renderTexInfo.hImageMem);
        pInfo->renderTexInfo.hImageMem = NULL;
        pInfo->renderTexInfo.pImage = NULL;
    }
    while (sdfDelayedResrcCheck()) {
    }
    dds3FreeRenderTexture();
    while (sdfDelayedResrcCheck()) {
    }
    evtUnRegistUnit(pInfo);
    FreeInfo(pInfo);
    sdfMemFreeDirect(pInfo->block);
    while (sdfDelayedResrcCheck()) {
    }
    dds3ChangeBistaMode(0);
    dds3DrawFlag |= 0x2000000;
    dds3SetScreenFovy(0.6283185f);
    dds3ScreenParam.ox = 2048.0f;
    dds3ScreenParam.oy = 2048.0f;
    evtProcessEnd();
    if (pInfo->blankTex != NULL) {
        sdfTexReleaseReference(pInfo->blankTex);
    }
    evtCallChipFreeDirect();
    dds3KernelDebugSign = 1;
    pmv_pid = NULL;
    return 1;
}
#else
INCLUDE_ASM(const s32, "event/evtEventViewer", End_evtViewer);
#endif

void evtEventViewerDestroyActive(dds3ProcessID_t *task) {
    evtViewerInfo_t *viewer;

    viewer = (evtViewerInfo_t *)dds3GetProcessData(task);
    evtViewerRelease(viewer);
}

void evtEventViewerDestroyActiveAlternate(dds3ProcessID_t *task) {
    evtViewerInfo_t *viewer;

    viewer = (evtViewerInfo_t *)dds3GetProcessData(task);
    evtViewerRelease(viewer);
}

void evtEventViewerInitCampFontResource(evtViewerInfo_t *viewer) {
    mnuCampInitFontResource(viewer);
}

void mapInit(void) {
    sdfMemHandle_t *block;
    evtViewerInfo_t *pInfo;
    dds3ProcessID_t *id;

    dds3BackColor = 0x80000000;
    block = sdfMemAllocDirect(0x2568);
    pInfo = sdfMemLock(block);
    memset(pInfo, 0, 0x2568);
    pInfo->block = block;
    id = dds3StartProcess(evtViewerTaskName, 0x3EB, 1, 1, evtCallGetProcessData10, (dds3ProcessFunc_t)evtEventViewerDestroyActive, pInfo);
    dds3LinkProcess(id, evtInitSky());
    evtEventViewerInitCampFontResource(pInfo);
}

void evtEventViewerDestroyTask(void) {
    dds3KillProcessName(evtViewerTaskName, 1);
}

void evtEnd(dds3ProcessID_t *id) {
    evtViewerInfo_t *pInfo = dds3GetProcessData(id);

    if (pInfo != NULL) {
        pInfo->flags |= 0x20;
    }
}

void evtChgSeq(dds3ProcessID_t *id, s32 seqno) {
    evtViewerInfo_t *pInfo = dds3GetProcessData(id);

    if (seqno != 0 && pInfo != NULL) {
        pInfo->flags |= 0x40;
        pInfo->playIndex = seqno - 1;
        pInfo->flags &= ~0x80;
        pInfo->flags &= ~0x200;
        pInfo->fldfilter = -1;
    }
}

void evtIgnoreFade(dds3ProcessID_t *id) {
    evtViewerInfo_t *pInfo = dds3GetProcessData(id);

    if (pInfo != NULL) {
        pInfo->flags |= 0x200;
    }
}

void evtNoSkipMes(dds3ProcessID_t *id) {
    evtViewerInfo_t *pInfo = dds3GetProcessData(id);

    if (pInfo != NULL) {
        pInfo->flags |= 0x400;
    }
}

INCLUDE_ASM(const s32, "event/evtEventViewer", evtCallGetProcessData12);

s32 evtCheckProcess(void) {
    return pmv_pid != NULL;
}

s32 evtCheckLastSkipStat(void) {
    return last_skip_stat;
}

/* body: matched in raidou @ 1424963: src/shared/event/evtEventViewer.c:3721, match, same function 1.00 (words 8/8, calls 1.00, shape 1.00) */
void evtSkipStart(dds3ProcessID_t *task) {
    evtViewerInfo_t *viewer;

    viewer = (evtViewerInfo_t *)dds3GetProcessData(task);
    EventSkipStart(viewer);
}

s32 evtSetFrame(dds3ProcessID_t *id, s32 Frame) {
    evtViewerInfo_t *pInfo = dds3GetProcessData(id);

    if (pInfo == NULL) {
        return -1;
    }
    if (Frame >= 0) {
        pInfo->nowframe = Frame;
        pInfo->fldfilter = -1;
    }
    return pInfo->nowframe;
}

void evtIgnoreAtracPlay(dds3ProcessID_t *id) {
    evtViewerInfo_t *pInfo = dds3GetProcessData(id);

    if (pInfo != NULL) {
        pInfo->flags |= 0x800;
    }
}

/* NON_MATCHING: 21 of 43 words match.  */
#ifdef NON_MATCHING
void oldEventSceneUnload(evtPolygonMovieWork_t *wdata) {
    s32 offset;
    s32 size;
    u8 *data;
    pmdHeader_t *cut;

    offset = 0;
    data = wdata->pm2Data;
    wdata->pm2CutCount = 0;
    do {
        if (data == NULL) {
            break;
        }
        size = wdata->pm2Size;
        if (size == 0) {
            break;
        }
        cut = (pmdHeader_t *)(data + offset);
        offset += cut->FileSize;
        wdata->pm2Cuts[wdata->pm2CutCount++] = cut;
    } while (offset < size);
    offset = 0;
    data = wdata->pm3Data;
    wdata->pm3CutCount = 0;
    do {
        if (data == NULL) {
            break;
        }
        size = wdata->pm3Size;
        if (size == 0) {
            break;
        }
        cut = (pmdHeader_t *)(data + offset);
        offset += cut->FileSize;
        wdata->pm3Cuts[wdata->pm3CutCount++] = cut;
    } while (offset < size);
}
#else
INCLUDE_ASM(const s32, "event/evtEventViewer", oldEventSceneUnload);
#endif

INCLUDE_RODATA(const s32, "event/evtEventViewer", evtViewerTaskName);

INCLUDE_SDATA(const s32, "event/evtEventViewer", evtFileMajor);

INCLUDE_SDATA(const s32, "event/evtEventViewer", evt_file_minor);

INCLUDE_SDATA(const s32, "event/evtEventViewer", evtData14);

INCLUDE_SDATA(const s32, "event/evtEventViewer", winoff_x);

INCLUDE_SDATA(const s32, "event/evtEventViewer", pmv_pid);

INCLUDE_SDATA(const s32, "event/evtEventViewer", last_skip_stat);

INCLUDE_SDATA(const s32, "event/evtEventViewer", evt_camera);

INCLUDE_SDATA(const s32, "event/evtEventViewer", evtCallGetFrameIDData);

INCLUDE_SDATA(const s32, "event/evtEventViewer", evtData16);

INCLUDE_SDATA(const s32, "event/evtEventViewer", evtFRAMESETOKDataEntry);

INCLUDE_SDATA(const s32, "event/evtEventViewer", evtFRAMESETOKDataEntry2);

INCLUDE_SDATA(const s32, "event/evtEventViewer", evtFRAMESETOKData2Entry);

INCLUDE_SDATA(const s32, "event/evtEventViewer", evtFRAMESETOKData2Entry2);

INCLUDE_SDATA(const s32, "event/evtEventViewer", evtFRAMESETOKData3Entry);

INCLUDE_SDATA(const s32, "event/evtEventViewer", evtDELETEFRAMEOKData2Entry);

INCLUDE_SDATA(const s32, "event/evtEventViewer", evtDELETEFRAMEOKData2Entry2);

INCLUDE_SDATA(const s32, "event/evtEventViewer", evtDELETEFRAMEOKData3Entry);

INCLUDE_SDATA(const s32, "event/evtEventViewer", evtDELETEFRAMEOKData3Entry2);

INCLUDE_SDATA(const s32, "event/evtEventViewer", evtDELETEFRAMEOKData14Entry);

INCLUDE_SDATA(const s32, "event/evtEventViewer", evtDELETEFRAMEOKData5Entry);

INCLUDE_SDATA(const s32, "event/evtEventViewer", evtDELETEFRAMEOKData5Entry2);

INCLUDE_SDATA(const s32, "event/evtEventViewer", evtDELETEFRAMEOKData5Entry3);

INCLUDE_SDATA(const s32, "event/evtEventViewer", evtDELETEFRAMEOKData5Entry4);

INCLUDE_SDATA(const s32, "event/evtEventViewer", evtDELETEFRAMEOKData5Entry5);

INCLUDE_SDATA(const s32, "event/evtEventViewer", evtDELETEFRAMEOKData5Entry6);

INCLUDE_SDATA(const s32, "event/evtEventViewer", evtDELETEFRAMEOKData5Entry7);

INCLUDE_SDATA(const s32, "event/evtEventViewer", evtDELETEFRAMEOKData6Entry);

INCLUDE_SDATA(const s32, "event/evtEventViewer", evtDELETEFRAMEOKData6Entry2);

INCLUDE_SDATA(const s32, "event/evtEventViewer", evtDELETEFRAMEOKData12Entry);

INCLUDE_SDATA(const s32, "event/evtEventViewer", evtDELETEFRAMEOKData12Entry2);

INCLUDE_SDATA(const s32, "event/evtEventViewer", evtDELETEFRAMEOKData8Entry);

INCLUDE_SDATA(const s32, "event/evtEventViewer", evtDELETEFRAMEOKData9Entry);

INCLUDE_SDATA(const s32, "event/evtEventViewer", evtDELETEFRAMEOKData9Entry2);

INCLUDE_SDATA(const s32, "event/evtEventViewer", evtDELETEFRAMEOKData10Entry);

INCLUDE_SDATA(const s32, "event/evtEventViewer", evtDELETEFRAMEOKData10Entry2);

INCLUDE_SDATA(const s32, "event/evtEventViewer", evtDELETEFRAMEOKData12Entry3);

INCLUDE_SDATA(const s32, "event/evtEventViewer", evtDELETEFRAMEOKData12Entry4);

INCLUDE_SDATA(const s32, "event/evtEventViewer", evtDELETEFRAMEOKData13Entry);

INCLUDE_SDATA(const s32, "event/evtEventViewer", evtDELETEFRAMEOKData15Entry);

INCLUDE_SDATA(const s32, "event/evtEventViewer", evtDELETEFRAMEOKData15Entry2);

INCLUDE_SDATA(const s32, "event/evtEventViewer", evtDELETEFRAMEOKData15Entry3);

INCLUDE_SDATA(const s32, "event/evtEventViewer", evtDELETEFRAMEOKData16Entry);

INCLUDE_SDATA(const s32, "event/evtEventViewer", evtDELETEFRAMEOKData17Entry);

INCLUDE_SDATA(const s32, "event/evtEventViewer", evtViewerProcNameSfog);

INCLUDE_SDATA(const s32, "event/evtEventViewer", evtViewerProcNameFilter);

INCLUDE_SDATA(const s32, "event/evtEventViewer", evtViewerProcNameMfilter);

