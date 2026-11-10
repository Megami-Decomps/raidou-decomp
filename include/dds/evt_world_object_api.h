#ifndef DDS_EVT_WORLD_OBJECT_API_H

#define DDS_EVT_WORLD_OBJECT_API_H
#include "common.h"

struct dds3Basic_t;

struct sdfMemHandle_t;

typedef s32 Dds3SceneScriptAttachResult;

enum Dds3SceneScriptAttachResult {
    DDS3_SCENE_SCRIPT_ATTACH_FAILED = 0,
    DDS3_SCENE_SCRIPT_ATTACH_SUCCEEDED = 1
};

/* Load a named scene script. NULL leaves the current resource untouched. A
 * non-NULL name releases it before loading, so FAILED leaves it cleared;
 * SUCCEEDED stores the loaded handle and address on worldNode. */
Dds3SceneScriptAttachResult evtLoadSceneResourceFrom(
    struct dds3Basic_t *worldNode, const char *resourceName);

#endif /* DDS_EVT_WORLD_OBJECT_API_H */
