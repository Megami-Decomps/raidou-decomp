#ifndef DDS_EVT_ENCOUNTER_REQUEST_API_H

#define DDS_EVT_ENCOUNTER_REQUEST_API_H
#include "evt_script_event_id.h"

#include "fld_encounter_runtime_api.h"

/* The request selects a battle-scene record; queued follow-up selects an event script. */
void evtSubmitEventRequest(FldBattleSceneRecordIndex sceneRecordIndex,
                           EvtScriptEventId followupEventId);

void evtSubmitEventRequestImmediate(FldBattleSceneRecordIndex sceneRecordIndex);

#endif /* DDS_EVT_ENCOUNTER_REQUEST_API_H */
