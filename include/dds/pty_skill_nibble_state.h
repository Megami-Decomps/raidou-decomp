#ifndef DDS_PTY_SKILL_NIBBLE_STATE_H

#define DDS_PTY_SKILL_NIBBLE_STATE_H
#include "common.h"

typedef enum PtySkillNibbleState {
    PTY_SKILL_NIBBLE_STATE_CLEAR = 0,
    PTY_SKILL_NIBBLE_STATE_PRIMARY_BIT_SET = 1,
    PTY_SKILL_NIBBLE_STATE_BIT_1_SET = 2
} PtySkillNibbleState;

struct dds_datUnitWork_t;

PtySkillNibbleState ptyGetSkillNibbleState(const struct dds_datUnitWork_t *partyRecord, u16 flagId);

#endif
