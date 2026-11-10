#ifndef DDS_PTY_SKILL_RECOVERY_API_H

#define DDS_PTY_SKILL_RECOVERY_API_H
#include "dat_skill_id.h"

struct dds_datUnitWork_t;

void ptyApplySkillRecovery(struct dds_datUnitWork_t *unit, DatSkillId skillId);

#endif /* DDS_PTY_SKILL_RECOVERY_API_H */
