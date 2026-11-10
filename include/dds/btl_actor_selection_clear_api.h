#ifndef DDS_BTL_ACTOR_SELECTION_CLEAR_API_H

#define DDS_BTL_ACTOR_SELECTION_CLEAR_API_H
#include "common.h"

struct BtlUnit;

/* True when no scene voice or masked actor status is present and the AI record kind is zero. */
u8 btlIsSelectedActorStatusAndRecordClear(struct BtlUnit *unit);

#endif /* DDS_BTL_ACTOR_SELECTION_CLEAR_API_H */
