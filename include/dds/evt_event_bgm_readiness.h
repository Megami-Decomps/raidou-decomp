#ifndef DDS_EVT_EVENT_BGM_READINESS_H

#define DDS_EVT_EVENT_BGM_READINESS_H
#include "common.h"

/* A BGM wait is complete when the track is resident or the event has no BGM. */
typedef s32 EvtEventBgmReadiness;

enum {
    EVT_EVENT_BGM_NOT_READY = 0,
    EVT_EVENT_BGM_READY = 1
};

#endif /* DDS_EVT_EVENT_BGM_READINESS_H */
