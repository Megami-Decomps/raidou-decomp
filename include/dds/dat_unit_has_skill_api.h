#ifndef DDS_DAT_UNIT_HAS_SKILL_API_H

#define DDS_DAT_UNIT_HAS_SKILL_API_H
#include "common.h"

#include "dat_skill_id.h"

struct dds_datUnitWork_t;

typedef u8 DatUnitHasSkillResult;

#define DAT_UNIT_SKILL_RESULT_ABSENT ((DatUnitHasSkillResult)0)

#define DAT_UNIT_SKILL_RESULT_PRESENT ((DatUnitHasSkillResult)1)

DatUnitHasSkillResult datUnitHasSkill(
    const struct dds_datUnitWork_t *unit, DatSkillId skillId);

#endif /* DDS_DAT_UNIT_HAS_SKILL_API_H */
