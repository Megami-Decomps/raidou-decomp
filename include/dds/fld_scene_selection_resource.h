#ifndef DDS_FLD_SCENE_SELECTION_RESOURCE_H

#define DDS_FLD_SCENE_SELECTION_RESOURCE_H
#include "common.h"

/* Resource key selected by the current scene-hit row's secondary slot. */
typedef s32 FldSceneSelectionResourceId;

FldSceneSelectionResourceId fldGetCurrentSceneSelectionResource(void);

#endif /* DDS_FLD_SCENE_SELECTION_RESOURCE_H */
