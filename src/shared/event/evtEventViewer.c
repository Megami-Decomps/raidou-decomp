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

void evtSetGameData2and3(evtPolygonMovieWork_t *wdata, evtViewerInfo_t *pInfo, s32 mode);
extern s16 winoff_x;
extern s8 dds3PadButtonState[];
extern const char evtStrE303[];
void evtProcess3Sub(evtViewerInfo_t *viewer);
void evtProcess3Sub2(evtViewerInfo_t *viewer);
void dds3FontReqListAdd(evtViewerInfo_t *viewer);
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
void evtEventViewerProcessPending(evtViewerInfo_t *viewer) {
    while (evtEventViewerGetPendingNode(viewer) != 0) {
        evtEventViewerReleasePendingNode(viewer);
    }
}

INCLUDE_ASM(const s32, "event/evtEventViewer", LinkViewerObject);

INCLUDE_ASM(const s32, "event/evtEventViewer", UnlinkViewerObject);

INCLUDE_ASM(const s32, "event/evtEventViewer", evtAddViewerObject);

INCLUDE_ASM(const s32, "event/evtEventViewer", evtGetViewerObjectTypeCount);

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

INCLUDE_ASM(const s32, "event/evtEventViewer", evtUnitMotionChange);

INCLUDE_ASM(const s32, "event/evtEventViewer", ValidateUnitMotionFrame);

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

INCLUDE_ASM(const s32, "event/evtEventViewer", evtMessageDelete);

INCLUDE_ASM(const s32, "event/evtEventViewer", rcvIsHealPhase);

INCLUDE_ASM(const s32, "event/evtEventViewer", CalcNextMesSet);

INCLUDE_ASM(const s32, "event/evtEventViewer", EventSkipStart);

u8 evtViewerHasUpdateFlag(evtViewerInfo_t *viewer) {
    s32 flags = viewer->flags;
    return (flags & 0x10) > 0;
}

INCLUDE_ASM(const s32, "event/evtEventViewer", evtCallGetFrameID);

INCLUDE_ASM(const s32, "event/evtEventViewer", evtCallVoiceStatus);

INCLUDE_ASM(const s32, "event/evtEventViewer", dds3FontReqListAdd);

INCLUDE_ASM(const s32, "event/evtEventViewer", ValidateMovie);

INCLUDE_ASM(const s32, "event/evtEventViewer", ValidateRenderTex);

void evtProcess3Sub(evtViewerInfo_t *viewer) {
}

INCLUDE_ASM(const s32, "event/evtEventViewer", evtCallDrawFont);

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
void evtViewerDispatchFlagMode(evtViewerInfo_t *viewer) {
    if ((viewer->flags & 1) != 0) {
        evtEventViewerApplyPlaybackMode(EVT_VIEWER_PLAYBACK_PAUSED, viewer->nowframe, viewer);
        return;
    }
    evtEventViewerApplyPlaybackMode(EVT_VIEWER_PLAYBACK_RUNNING, viewer->nowframe, viewer);
}

INCLUDE_ASM(const s32, "event/evtEventViewer", evtLeaf6);

INCLUDE_ASM(const s32, "event/evtEventViewer", evtLeaf7);

INCLUDE_ASM(const s32, "event/evtEventViewer", PushMenu);

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

INCLUDE_ASM(const s32, "event/evtEventViewer", evtCallGetPIDfromUID);

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

INCLUDE_ASM(const s32, "event/evtEventViewer", evtCallGetPIDfromUID2);

INCLUDE_ASM(const s32, "event/evtEventViewer", evtEventViewerCommitGroupSelection);

INCLUDE_ASM(const s32, "event/evtEventViewer", evtCallGetPIDfromUID3);

INCLUDE_ASM(const s32, "event/evtEventViewer", evtCallGetPIDfromUID4);

u32 evtViewCmdCancelSelection(u32 unused0, u32 unused1, evtViewerInfo_t *viewer) {
    evtViewerPopHistory(viewer);
    return 0;
}

INCLUDE_ASM(const s32, "event/evtEventViewer", evtViewCmdResolveSlot);

INCLUDE_ASM(const s32, "event/evtEventViewer", evtCallGetPIDfromUID5);

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
            itfMesMngRequestMessage_itf(viewer->workdata->unk018, viewer->voiceMessageNo, 0);
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
            if (viewer->fontRequestMode == 1) {
                dds3FontReqListAdd(viewer);
                viewer->fontRequestReset = -1;
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

INCLUDE_ASM(const s32, "event/evtEventViewer", Process2_evtViewer);

INCLUDE_ASM(const s32, "event/evtEventViewer", evtCallGetProcessData10);

INCLUDE_ASM(const s32, "event/evtEventViewer", evtProcess2_RealPlay);

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

INCLUDE_ASM(const s32, "event/evtEventViewer", evtEndEvtViewerSub);

INCLUDE_ASM(const s32, "event/evtEventViewer", End_evtViewer);

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

