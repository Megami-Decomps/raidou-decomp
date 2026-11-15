#ifndef EVT_EVENT_VIEWER_H
#define EVT_EVENT_VIEWER_H

#include "common.h"

/* File formats used to split event-viewer tracks during serialization. */
typedef s32 EvtViewerTrackFileFormat;

enum {
    EVT_VIEWER_TRACK_FILE_FORMAT_PM2 = 2,
    EVT_VIEWER_TRACK_FILE_FORMAT_PM3 = 3
};

/* Sign-extended 32-bit view of the signed timeline-key parameter halfword:
 * bits 12-15 select a script integer, and bits 0-11 hold its required value.
 * Selector zero disables the condition. */
typedef s32 EvtViewerIndexedCondition;

/* The predicate returns only these two outcomes. */
typedef enum EvtViewerIndexedConditionResult {
    EVT_VIEWER_INDEXED_CONDITION_NOT_MATCHED = 0,
    EVT_VIEWER_INDEXED_CONDITION_MATCHED = 1
} EvtViewerIndexedConditionResult;

/* A zero selector matches unconditionally; otherwise the integer at
 * script.ints[199 + selector] must equal the encoded 12-bit value. */
EvtViewerIndexedConditionResult evtViewerTestIndexedCondition(
    EvtViewerIndexedCondition condition);

/* Callback result: 0 retains the callback, -1 finishes the task, and
 * other words select a replacement callback. */
typedef s32 dds3TaskUpdateResult_t;

#endif
