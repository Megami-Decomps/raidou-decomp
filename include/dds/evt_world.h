#ifndef DDS_EVT_WORLD_H

#define DDS_EVT_WORLD_H
#include "../evt_world.h"
#include "common.h"

#include "eff_transform.h"

#include "dds3_area_room_key.h"

/* Result of querying a path object's movement state. */
typedef enum EvtPathStatus {
    EVT_PATH_STATUS_INVALID = -1,
    EVT_PATH_STATUS_AT_ENDPOINT = 0,
    EVT_PATH_STATUS_IN_PROGRESS = 1
} EvtPathStatus;

typedef s32 EvtPathLinkResult;

enum EvtPathLinkResult {
    EVT_PATH_LINK_FAILED = 0,
    EVT_PATH_LINK_SUCCEEDED = 1
};

/* Native 0x0A-byte frame editor row: count followed by eight signed column kinds. */
typedef struct EvtFrameTableEntry {
    s16 columns;
    s8 columnTypes[8];
} EvtFrameTableEntry;

typedef char EvtFrameTableEntry_size_must_be_0x0A[(sizeof(EvtFrameTableEntry) == 0x0A) ? 1 : -1];

#ifdef VERSION_DDS1

extern EvtFrameTableEntry D_00368768[];

#elif VERSION_DDS2

extern EvtFrameTableEntry D_003C9538[];

#endif

struct sdfTexHandle_t;

struct sdfMemHandle_t;

struct ItfMesSub;

/* The picture-task allocator creates this complete 0x08-byte state. */
typedef struct EvtPictureWork {
    u32 drawEnabled; /* Only EVT_PICTURE_FLAG_DRAW_ENABLED is set or tested. */
    struct sdfTexHandle_t *texture;
} EvtPictureWork;

typedef char EvtPictureWork_size_must_be_0x08[(sizeof(EvtPictureWork) == 0x08) ? 1 : -1];

/* Native resource handle and the relocatable message data it owns. */
typedef struct EvtResourcePair {
    struct sdfMemHandle_t *handle;
    struct ItfMesSub *messageData;
} EvtResourcePair;

typedef char EvtResourcePair_size_must_be_0x08[(sizeof(EvtResourcePair) == 0x08) ? 1 : -1];

/* Loaded resource table header: the resource handle, the origin of a
 * 32-byte-stride record array, and its record count. */
typedef struct EvtLoadedRecord {
    struct sdfMemHandle_t *handle; /* 0x0: released by sdfMemFreeDirect */
    u8 *records; /* 0x4: origin of a 32-byte-stride lookup */
    u32 recordCount; /* 0x8: bound used by the coordinate search */
} EvtLoadedRecord;

typedef char EvtLoadedRecord_size_must_be_0x0C[(sizeof(EvtLoadedRecord) == 0x0C) ? 1 : -1];

struct scrHeader_t;

struct dds3ProcessID_t;

/* Scene resource scripts are selected by name on a world node. The start
 * result is the scheduler task created for the script, or NULL on failure. */
struct dds3ProcessID_t *evtStartSceneResourceTask(const dds3Basic_t *worldNode, const char *taskName);

#endif /* DDS_EVT_WORLD_H */
