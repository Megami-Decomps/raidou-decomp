#ifndef DDS_EVT_SCRIPT_EVENT_ID_H

#define DDS_EVT_SCRIPT_EVENT_ID_H
#include "common.h"

/* Script-pack event ID queued for follow-up dispatch; nonpositive means none. */
typedef s32 EvtScriptEventId;

extern EvtScriptEventId evtPendingEventSelection;

#endif /* DDS_EVT_SCRIPT_EVENT_ID_H */
