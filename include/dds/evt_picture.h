#ifndef DDS_EVT_PICTURE_H

#define DDS_EVT_PICTURE_H
#include "common.h"

#include "kwln.h"

#include "dds3_task_runner.h"

#include "evt_world.h"

enum {
    EVT_PICTURE_FLAG_DRAW_ENABLED = 1
};

extern char evtPictureTaskName[];

EvtPictureWork *evtAllocateContext(void);

void evtSubmitPictureDrawPacket(const struct sdfTexHandle_t *borrowedTexture);

dds3TaskUpdateResult_t evtUpdatePictureWhenFlagged(dds3ProcessID_t *task);

#endif /* DDS_EVT_PICTURE_H */
