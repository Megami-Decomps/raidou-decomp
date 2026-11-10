#ifndef DDS_FLD_SCENE_REQUEST_H

#define DDS_FLD_SCENE_REQUEST_H
#include "fld.h"

#include "fld_light_set_index.h"

/* File-batch records select the initial scene light set at +0x04 and carry
 * optional resource path suffixes. */
typedef struct FldFileResolverRecord {
    u32 unk00;
    FldLightSetIndex initialLightSetIndex;
    /* Optional path suffixes appended by the field resource resolver. */
    const char *resourceA;
    const char *resourceB;
    const char *resourceC;
    const char *resourceD;
} FldFileResolverRecord;

/* Scene-placement chunks use the same wire size but store a fallback script
 * name at +0x04 rather than a file-resolver index. */
typedef struct FldSceneResourceRecord {
    u32 unk00;
    const char *fallbackResourceName;
    const char *resourceA;
    const char *resourceB;
    const char *resourceC;
    const char *resourceD;
} FldSceneResourceRecord;

/* Object-spawn records carry a name pointer and signed world-node key. */
typedef struct FldActionSpawn {
    u32 unk00;
    const char *objectName;
    s32 key;
} FldActionSpawn;

typedef char FldFileResolverRecord_size_check[
    (sizeof(FldFileResolverRecord) == 0x18) ? 1 : -1];

typedef char FldSceneResourceRecord_size_check[
    (sizeof(FldSceneResourceRecord) == 0x18) ? 1 : -1];

typedef char FldActionSpawn_size_check[
    (sizeof(FldActionSpawn) == 0x0C) ? 1 : -1];

/* Observed prefix of the transfer payload passed to the resource-batch loader. */
typedef struct FldLoadRequest {
    u32 spawnBatchCount;
    FldSpawnBatch *spawnBatches;
    const FldFileResolverRecord *record;
} FldLoadRequest;

/* Counted placement-batch input consumed by both field request paths. */
typedef struct FldSceneRequest {
    u32 spawnBatchCount;
    FldSpawnBatch *spawnBatches;
    const FldSceneResourceRecord *record;
    u32 spawnCount;
    struct FldActionSpawn *spawnList;
} FldSceneRequest;

#endif /* DDS_FLD_SCENE_REQUEST_H */
