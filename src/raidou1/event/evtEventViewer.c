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

#ifndef VERSION_DDS2
#define VERSION_DDS2 1
#endif

#include "dds3obj.h"
#include "newdata.h"
#include "dds/btl.h"
#include "dds/dds3_object_control_slot.h"

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
 * DDS1 and DDS2 use the same observed layout. Node payloads depend on the slot. */
enum {
    EVT_WORLD_SLOT_MOVIE = 3, /* viewer movie-time lists: byte offset 0x28 */
    EVT_WORLD_SLOT_CAMERA = 4, /* linked cameras; active nodes supply the view transform */
    EVT_WORLD_SLOT_UNIT = 5   /* unit/group/lip-sync lists: byte offset 0x40 */
};



/* Copy the source object's transform to the target object, with its rotation offset. */

#include "dds/evt_viewer_track_file_format.h"
#include "dds/evt_viewer.h"
#include "dds/evt_viewer_indexed_condition.h"
#include "dds/eff_object.h"
#include "dds/mdl.h"
#include "dds/mdl_motion_controller_lookup_api.h"

void evtEventViewerReleasePendingNode(dds_evtViewerInfo_t *viewer);
void evtEventViewerProcessPending(dds_evtViewerInfo_t *viewer);
s32 evtEventViewerCountEntries(dds_evtViewerInfo_t *viewer);
s32 evtEventViewerSumNodeCounts(EvtViewerTrackFileFormat format, dds_evtViewerInfo_t *viewer);

struct SoundMixer;

struct EffNodeDescriptor;

extern u8 evtViewerHasUpdateFlag(dds_evtViewerInfo_t *);

typedef struct CampDisplayDefaults CampDisplayDefaults;

void evtApplyViewerTimelineFrame(s32 time, dds_evtViewerInfo_t *viewer);
void evtViewerPushCommandHistory(s32 actionMode, s32 first, s32 second, dds_evtViewerInfo_t *viewer);
dds3TaskUpdateResult_t evtViewerUpdateFrame(dds3ProcessID_t *task);

struct dds_evtViewerInfo_t;

struct EffNode;

typedef struct EvtViewerObjectData {
    u8 pad00[0xC];
    struct EffNode *parameterNode;
} EvtViewerObjectData;

u16 evtViewerPopHistory(dds_evtViewerInfo_t *viewer);
extern char evtViewerTaskName[];

typedef struct CampDisplayDefaults CampDisplayDefaults;

extern void ValidatePathFrame(s32 endTime, dds_evtViewerInfo_t *viewer);
extern void evtViewerApplyGlyphLodChannel(s32 position, dds_evtViewerInfo_t *viewer);
extern void ValidateUnitPosChange(s32 position, dds_evtViewerInfo_t *viewer);
EvtRuntimeChild *evtViewFindGlyphAtOrBefore(dds_evtViewerInfo_t *viewer);
extern void evtSetMovieClipPositionClampedToDuration(dds3Basic_t *movie, s32 unused, s32 start, s32 end, s32 offset);
void ValidatePathFrame(s32 endTime, dds_evtViewerInfo_t *viewer);

extern EvtRuntimeChild *GetUnitMotionChangeFrame(EvtRuntimeGroup *group, s32 frame,
                                                             MdlMotionSearchId channel);

extern u32 sdfGetLodChunkValue(dds_sdfModelRootNode_t *model);
void evtViewerApplyGlyphLodChannel(s32 position, dds_evtViewerInfo_t *viewer);
void ValidateUnitPosChange(s32 position, dds_evtViewerInfo_t *viewer);
u8 evtViewerHasUpdateFlag(dds_evtViewerInfo_t *viewer);

EvtRuntimeChild *GetUnitMotionChangeFrame(EvtRuntimeGroup *group, s32 frame,
                                                      MdlMotionSearchId channel);

enum {
    EVT_VIEWER_PLAYBACK_PAUSED = 0,
    EVT_VIEWER_PLAYBACK_RUNNING = 1,
};

/* Apply the viewer playback mode to unit, motion and movie-object tracks. */
void evtEventViewerApplyPlaybackMode(s32 playbackMode, u32 frame, dds_evtViewerInfo_t *viewer);


u32 evtViewerReturnZero(void);
u32 evtViewerPushMode5CommandHistory(u32 unused0, u32 unused1, dds_evtViewerInfo_t *viewer);
s32 evtViewCmdSetValue(s32 unused0, s32 unused1, dds_evtViewerInfo_t *viewer);
u32 evtViewerStoreCommandInSelectedField(u32 unused0, u32 unused1, dds_evtViewerInfo_t *viewer);
u32 evtViewCmdNoOp(void);
u32 evtViewCmdCancelSelection(u32 unused0, u32 unused1, dds_evtViewerInfo_t *viewer);
u32 evtViewerClearPendingNodeAndPushHistory(u32 unused0, u32 unused1, dds_evtViewerInfo_t *viewer);
dds3TaskUpdateResult_t evtViewerPickNextHandler(dds3ProcessID_t *task);
extern dds3TaskUpdateResult_t evtViewerPickNextHandler(dds3ProcessID_t *task);
void evtViewerRelease();
void evtEventViewerDestroyActive(dds3ProcessID_t *task);
void evtEventViewerDestroyActiveAlternate(dds3ProcessID_t *task);
extern void mnuCampInitFontResource(dds_evtViewerInfo_t *viewer);
void evtEventViewerInitCampFontResource(dds_evtViewerInfo_t *viewer);
void evtEventViewerDestroyTask(void);

INCLUDE_ASM(const s32, "event/evtEventViewer", LinkFrameInfo);

INCLUDE_ASM(const s32, "event/evtEventViewer", UnlinkFrameInfo);

INCLUDE_ASM(const s32, "event/evtEventViewer", SortFrameInfo);

INCLUDE_ASM(const s32, "event/evtEventViewer", evtAddFrameInfo_evtViewer);

evtFrameInfo_t *AddFrameInfo2(evtViewerObject_s *group, s32 frame, evtViewerInfo_t *viewer)
{
    return evtAddFrameInfo(group, frame);
}

INCLUDE_ASM(const s32, "event/evtEventViewer", evtGetPIDfromUID);

INCLUDE_ASM(const s32, "event/evtEventViewer", DelFrameInfo);

INCLUDE_ASM(const s32, "event/evtEventViewer", evtDelProcess);

INCLUDE_ASM(const s32, "event/evtEventViewer", evtCallDelEffectTableEvtEventViewer);

/* Consume queued viewer events until the pending check reports none. */
void evtEventViewerProcessPending(dds_evtViewerInfo_t *viewer) {
    while (evtEventViewerGetPendingNode(viewer) != 0) {
        evtEventViewerReleasePendingNode(viewer);
    }
}

INCLUDE_ASM(const s32, "event/evtEventViewer", LinkViewerObject);

INCLUDE_ASM(const s32, "event/evtEventViewer", UnlinkViewerObject);

INCLUDE_ASM(const s32, "event/evtEventViewer", evtAddViewerObject);

INCLUDE_ASM(const s32, "event/evtEventViewer", evtGetViewerObjectTypeCount);

s32 evtEventViewerCountEntries(dds_evtViewerInfo_t *viewer) {
    EvtRuntimeGroup *entry;
    s32 entryCount;

    entryCount = 0;
    for (entry = viewer->groups; entry != NULL; entry = entry->next) {
        entryCount++;
    }
    return entryCount;
}

/* PM2 skips group ids 5/0x13; PM3 serializes only those groups. */
s32 evtEventViewerSumNodeCounts(EvtViewerTrackFileFormat format, dds_evtViewerInfo_t *viewer) {
    EvtRuntimeGroup *entry;
    s32 total;

    total = 0;
    for (entry = viewer->groups; entry != NULL; entry = entry->next) {
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

INCLUDE_ASM(const s32, "event/evtEventViewer", evtCallDelFrameInfo);

INCLUDE_ASM(const s32, "event/evtEventViewer", evtSetFirstFrame);

INCLUDE_ASM(const s32, "event/evtEventViewer", evtSetLastFrame);

INCLUDE_ASM(const s32, "event/evtEventViewer", evtCallMemset7);

INCLUDE_ASM(const s32, "event/evtEventViewer", FreeInfo);

INCLUDE_ASM(const s32, "event/evtEventViewer", evtUnRegistUnit);

INCLUDE_ASM(const s32, "event/evtEventViewer", SearchNameTable);

INCLUDE_ASM(const s32, "event/evtEventViewer", evtAddNameTable);

INCLUDE_ASM(const s32, "event/evtEventViewer", SearchBasicFromNameIndex);

INCLUDE_ASM(const s32, "event/evtEventViewer", LinkEffectBasic);

INCLUDE_ASM(const s32, "event/evtEventViewer", AddEffectTable2);

INCLUDE_ASM(const s32, "event/evtEventViewer", DelEffectTable_evtEventViewer);

INCLUDE_ASM(const s32, "event/evtEventViewer", AddBED);

INCLUDE_ASM(const s32, "event/evtEventViewer", DelBED);

INCLUDE_ASM(const s32, "event/evtEventViewer", GetModelHandleFromUID);

INCLUDE_ASM(const s32, "event/evtEventViewer", CreateMotion);

INCLUDE_ASM(const s32, "event/evtEventViewer", evtCallGetModelHandleFromUID);

INCLUDE_ASM(const s32, "event/evtEventViewer", DrawTmxTextureFrom2Frame);

INCLUDE_ASM(const s32, "event/evtEventViewer", DrawTmxTexture);

INCLUDE_ASM(const s32, "event/evtEventViewer", DrawEffect);

INCLUDE_ASM(const s32, "event/evtEventViewer", SetActiveCamera);

INCLUDE_ASM(const s32, "event/evtEventViewer", ReqMessage);

INCLUDE_ASM(const s32, "event/evtEventViewer", RunFrame1);

INCLUDE_ASM(const s32, "event/evtEventViewer", evtRunFrameSub);

INCLUDE_ASM(const s32, "event/evtEventViewer", ValidatePLTMX);

INCLUDE_ASM(const s32, "event/evtEventViewer", ValidateAt3Bgm);

INCLUDE_ASM(const s32, "event/evtEventViewer", Validate1stAt3Bgm);

INCLUDE_ASM(const s32, "event/evtEventViewer", OutFrame1);

INCLUDE_ASM(const s32, "event/evtEventViewer", CalcLightCycle);

INCLUDE_ASM(const s32, "event/evtEventViewer", RunFrame2);

INCLUDE_ASM(const s32, "event/evtEventViewer", RunFrame3);

INCLUDE_ASM(const s32, "event/evtEventViewer", RunFrameUnit);

INCLUDE_ASM(const s32, "event/evtEventViewer", evtRunFrame);

/* Returns the nearest kind-2 key at/before the current frame, or NULL.
 * Equal-distance ties retain the first key visited. */
EvtRuntimeChild *evtViewFindGlyphAtOrBefore(dds_evtViewerInfo_t *viewer) {
    EvtRuntimeChild *result = NULL;
    s32 best = 99999;
    EvtRuntimeGroup *node = viewer->groups;

    while (node != NULL) {
        if (node->type == 2) {
            EvtRuntimeChild *glyph = node->children;

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
void ValidatePathFrame(s32 endTime, dds_evtViewerInfo_t *viewer) {
    evtWorldTable_t *worldTable;
    evtWorldSlot_t *worldSlots;
    dds3Basic_t *movieObject;
    EvtRuntimeGroup *trackEntry;
    s32 startFrame;
    s32 extra;
    EvtRuntimeChild *glyph;

    if (dds3GetActiveScene() != 0) {
        worldTable = (evtWorldTable_t *)((dds3Basic_t *)dds3GetActiveScene())->workdata;
        if (worldTable != 0) {
            worldSlots = worldTable->slots;
            if (worldSlots != 0) {
                movieObject = worldSlots[EVT_WORLD_SLOT_MOVIE].head;
                if (movieObject != 0) {
                    do {
                        trackEntry = viewer->groups;
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

INCLUDE_ASM(const s32, "event/evtEventViewer", evtUnitMotionChange);

INCLUDE_ASM(const s32, "event/evtEventViewer", ValidateUnitMotionFrame);

/* Applies the latest kind-6 key at/before position to a kind-1 track's LOD
 * byte, provided the requested signed-byte level is supported by its chunk. */
void evtViewerApplyGlyphLodChannel(s32 position, dds_evtViewerInfo_t *viewer) {
    EvtRuntimeGroup *node = viewer->groups;
    EvtRuntimeChild *glyph;
    EvtRuntimeChild *best;
    s32 bestFrame;
    dds_sdfModelRootNode_t *lod;
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
            lod = ((dds3ModelHandle_t *)((EffectObjectData *)node->info->workdata)->modelHolder->resourceHandle)->inner;
            if (best == NULL) {
                lod->lodIndex = 0;
            } else {
                level = best->p0C.sb[0];
                if ((s32)sdfGetLodChunkValue(lod) >= level) {
                    lod->lodIndex = best->p0C.sb[0];
                }
            }
        }
        node = node->next;
    }
}

/* At an exact kind-7 key frame, attaches the indexed world object. Index -1
 * restores the saved vectors; losing the active key restores them once too. */
void ValidateUnitPosChange(s32 position, dds_evtViewerInfo_t *viewer) {
    EvtRuntimeGroup *node = viewer->groups;

    while (node != NULL) {
        if (node->type == 1) {
            EvtRuntimeChild *glyph = node->children;
            s32 bestFrame = -1;
            EvtRuntimeChild *best = NULL;

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
                                           (u128 *)node->savedPosition);
                    dds3SetRotPoseSet(node->info,
                                            (u128 *)node->savedRotation);
                    dds3GetPosPoseSet(node->info);
                    SDF_VU0_STOREVECTOR(&node->info->poseset->pos2);
                    node->attachmentState = EVT_OBJECT_ATTACHMENT_DETACHED;
                }
            } else if (best->frame == position) {
                s16 channel = best->p0C.sh[0];

                if (channel == -1) {
                    dds3SetPosPoseSet(node->info,
                                           (u128 *)node->savedPosition);
                    dds3SetRotPoseSet(node->info,
                                            (u128 *)node->savedRotation);
                    dds3GetPosPoseSet(node->info);
                    SDF_VU0_STOREVECTOR(&node->info->poseset->pos2);
                    node->attachmentState = EVT_OBJECT_ATTACHMENT_DETACHED;
                } else {
                    dds3Basic_t *object = dds3SceneBasicSearchName(
                        dds3GetActiveScene(), viewer->entryName[channel]);

                    evtSetUnitPos(
                        node->info->serialID, object->serialID);
                    node->attachmentState = EVT_OBJECT_ATTACHMENT_ATTACHED;
                }
            }
        }
        node = node->next;
    }
}

INCLUDE_ASM(const s32, "event/evtEventViewer", evtMessageDelete);

INCLUDE_ASM(const s32, "event/evtEventViewer", rcvIsHealPhase);

INCLUDE_ASM(const s32, "event/evtEventViewer", CalcNextMesSet);

INCLUDE_ASM(const s32, "event/evtEventViewer", EventSkipStart);

u8 evtViewerHasUpdateFlag(dds_evtViewerInfo_t *viewer) {
    s32 flags = viewer->flags;
    return (flags & 0x10) > 0;
}

INCLUDE_ASM(const s32, "event/evtEventViewer", evtCallGetFrameID);

INCLUDE_ASM(const s32, "event/evtEventViewer", evtCallVoiceStatus);

INCLUDE_ASM(const s32, "event/evtEventViewer", dds3FontReqListAdd);

INCLUDE_ASM(const s32, "event/evtEventViewer", ValidateMovie);

INCLUDE_ASM(const s32, "event/evtEventViewer", ValidateRenderTex);

void evtProcess3Sub(void) {
}

INCLUDE_ASM(const s32, "event/evtEventViewer", evtCallDrawFont);

void evtProcess3Sub2(void) {
}

/* Returns the latest eligible kind-5 motion key at/before frame for channel,
 * or NULL. Its condition must pass; equal-frame ties retain the first key. */
EvtRuntimeChild *GetUnitMotionChangeFrame(EvtRuntimeGroup *group, s32 frame,
                                                      MdlMotionSearchId channel) {
    s32 bestFrame = -1;
    EvtRuntimeChild *best = NULL;
    EvtRuntimeChild *key = group->children;

    if (key != NULL) {
        do {
            if (frame >= key->frame && bestFrame < key->frame && key->p08.sb[0] == 5 &&
                key->p0C.sb[0] == channel &&
                evtViewerTestIndexedCondition(key->p10.sh[0]) ==
                    EVT_VIEWER_INDEXED_CONDITION_MATCHED) {
                bestFrame = key->frame;
                best = key;
            }
            key = key->next;
        } while (key != NULL);
    }
    return best;
}

INCLUDE_ASM(const s32, "event/evtEventViewer", evtViewerStopFrame);

/* Dispatch one of two viewer modes based on its lowest flag bit. */
void evtViewerDispatchFlagMode(dds_evtViewerInfo_t *viewer) {
    if ((viewer->flags & 1) != 0) {
        evtEventViewerApplyPlaybackMode(EVT_VIEWER_PLAYBACK_PAUSED, viewer->nowframe, viewer);
        return;
    }
    evtEventViewerApplyPlaybackMode(EVT_VIEWER_PLAYBACK_RUNNING, viewer->nowframe, viewer);
}

INCLUDE_ASM(const s32, "event/evtEventViewer", evtLeaf6);

INCLUDE_ASM(const s32, "event/evtEventViewer", evtLeaf7);

INCLUDE_ASM(const s32, "event/evtEventViewer", PushMenu);

u16 evtViewerPopHistory(dds_evtViewerInfo_t *viewer) {
    u16 historyId;
    s32 historyIndex;

    historyIndex = viewer->historyCount - 1;
    if (viewer->historyCount == 0) {
        viewer->actionMode = 0;
        return 0;
    }
    viewer->historyCount = historyIndex;
    historyId = viewer->history[historyIndex].id;
    viewer->actionMode = (u32)historyId;
    return historyId;
}

INCLUDE_ASM(const s32, "event/evtEventViewer", MessageManager);

INCLUDE_ASM(const s32, "event/evtEventViewer", sndVoiceStatus);

INCLUDE_ASM(const s32, "event/evtEventViewer", DrawMessage);

u32 evtViewerReturnZero(void) {
    return 0;
}

INCLUDE_RODATA(const s32, "event/evtEventViewer", evtStrWhiteOut);

INCLUDE_RODATA(const s32, "event/evtEventViewer", evtStrBlackOut);

INCLUDE_RODATA(const s32, "event/evtEventViewer", evtStrCurrentFrame);

INCLUDE_RODATA(const s32, "event/evtEventViewer", evtStrFirstFrame);

INCLUDE_ASM(const s32, "event/evtEventViewer", evtFRAMESETOK);

INCLUDE_ASM(const s32, "event/evtEventViewer", evtCallSetFirstFrame);

u32 evtViewerPushMode5CommandHistory(u32 unused0, u32 unused1, dds_evtViewerInfo_t *viewer) {
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

INCLUDE_ASM(const s32, "event/evtEventViewer", evtCallGetPIDfromUID);

/* Store the command value as a halfword and clear its extra halfword when tagged. */
s32 evtViewCmdSetValue(s32 unused0, s32 unused1, dds_evtViewerInfo_t *viewer) {
    s32 commandValue = viewer->value;
    EvtRuntimeChild *entry = evtEventViewerGetPendingNode(viewer);

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
u32 evtViewerStoreCommandInSelectedField(u32 unused0, u32 unused1, dds_evtViewerInfo_t *viewer) {
    EvtRuntimeChild *entry;
    u32 commandValue;
    s32 slot;
    commandValue = viewer->value;
    entry = evtEventViewerGetPendingNode(viewer);
    if (entry != 0) {
        slot = viewer->frameGroup->type - 1;
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

INCLUDE_ASM(const s32, "event/evtEventViewer", evtCallGetPIDfromUID2);

INCLUDE_ASM(const s32, "event/evtEventViewer", evtEventViewerCommitGroupSelection);

INCLUDE_ASM(const s32, "event/evtEventViewer", evtCallGetPIDfromUID3);

INCLUDE_ASM(const s32, "event/evtEventViewer", evtCallGetPIDfromUID4);

u32 evtViewCmdCancelSelection(u32 unused0, u32 unused1, dds_evtViewerInfo_t *viewer) {
    evtViewerPopHistory(viewer);
    return 0;
}

INCLUDE_ASM(const s32, "event/evtEventViewer", evtViewCmdResolveSlot);

INCLUDE_ASM(const s32, "event/evtEventViewer", evtCallGetPIDfromUID5);

INCLUDE_ASM(const s32, "event/evtEventViewer", evtCallSprintf);

INCLUDE_ASM(const s32, "event/evtEventViewer", evtViewCmdSetObjectTarget);

u32 evtViewerClearPendingNodeAndPushHistory(u32 unused0, u32 unused1, dds_evtViewerInfo_t *viewer) {
    if (evtEventViewerGetPendingNode(viewer) != 0) {
        viewer->groupFirst = 0;
        viewer->groupCursor = 0;
        evtViewerPushCommandHistory(0xa, 0x9c, 0x54, viewer);
        return 0;
    }
    return 0;
}

INCLUDE_RODATA(const s32, "event/evtEventViewer", evtStrE303);

INCLUDE_ASM(const s32, "event/evtEventViewer", EnterData);

/* Hand control back to the frame update whether or not the task still owns a viewer. */
dds3TaskUpdateResult_t evtViewerPickNextHandler(dds3ProcessID_t *task) {
    if (dds3GetProcessData(task) != 0) {
        return (s32)evtViewerUpdateFrame;
    }
    return (s32)evtViewerUpdateFrame;
}

INCLUDE_ASM(const s32, "event/evtEventViewer", evtCallEveRippleBlurSetDefTex);

INCLUDE_ASM(const s32, "event/evtEventViewer", UnitMotionChangeForWait);

INCLUDE_ASM(const s32, "event/evtEventViewer", evtValidateUnitMotionFrameForWait);

INCLUDE_ASM(const s32, "event/evtEventViewer", Process3);

INCLUDE_ASM(const s32, "event/evtEventViewer", Process2_evtViewer);

INCLUDE_ASM(const s32, "event/evtEventViewer", evtCallGetProcessData10);

INCLUDE_ASM(const s32, "event/evtEventViewer", evtProcess2_RealPlay);

INCLUDE_ASM(const s32, "event/evtEventViewer", evtCallGetProcessData11);

INCLUDE_ASM(const s32, "event/evtEventViewer", evtEndEvtViewerSub);

INCLUDE_ASM(const s32, "event/evtEventViewer", End_evtViewer);

void evtEventViewerDestroyActive(dds3ProcessID_t *task) {
    dds_evtViewerInfo_t *viewer;

    viewer = (dds_evtViewerInfo_t *)dds3GetProcessData(task);
    evtViewerRelease(viewer);
}

void evtEventViewerDestroyActiveAlternate(dds3ProcessID_t *task) {
    dds_evtViewerInfo_t *viewer;

    viewer = (dds_evtViewerInfo_t *)dds3GetProcessData(task);
    evtViewerRelease(viewer);
}

void evtEventViewerInitCampFontResource(dds_evtViewerInfo_t *viewer) {
    mnuCampInitFontResource(viewer);
}

INCLUDE_ASM(const s32, "event/evtEventViewer", mapInit);

void evtEventViewerDestroyTask(void) {
    dds3KillProcessName(evtViewerTaskName, 1);
}

INCLUDE_ASM(const s32, "event/evtEventViewer", evtEnd);

INCLUDE_ASM(const s32, "event/evtEventViewer", evtChgSeq);

INCLUDE_ASM(const s32, "event/evtEventViewer", evtIgnoreFade);

INCLUDE_ASM(const s32, "event/evtEventViewer", evtNoSkipMes);

INCLUDE_ASM(const s32, "event/evtEventViewer", evtCallGetProcessData12);

INCLUDE_ASM(const s32, "event/evtEventViewer", evtCheckProcess);

INCLUDE_ASM(const s32, "event/evtEventViewer", evtCheckLastSkipStat);

INCLUDE_ASM(const s32, "event/evtEventViewer", evtSkipStart);

INCLUDE_ASM(const s32, "event/evtEventViewer", evtSetFrame);

INCLUDE_ASM(const s32, "event/evtEventViewer", evtIgnoreAtracPlay);

INCLUDE_ASM(const s32, "event/evtEventViewer", oldEventSceneUnload);

INCLUDE_ASM(const s32, "event/evtEventViewer", evtCheckLoad);

INCLUDE_RODATA(const s32, "event/evtEventViewer", evtViewerTaskName);

INCLUDE_SDATA(const s32, "event/evtEventViewer", evtFileMajor);

INCLUDE_SDATA(const s32, "event/evtEventViewer", evt_file_minor);

INCLUDE_SDATA(const s32, "event/evtEventViewer", evtData14);

INCLUDE_SDATA(const s32, "event/evtEventViewer", Process3Data);

INCLUDE_SDATA(const s32, "event/evtEventViewer", RunFrame1Data);

INCLUDE_SDATA(const s32, "event/evtEventViewer", evtData15);

INCLUDE_SDATA(const s32, "event/evtEventViewer", evtRunFrameData);

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

