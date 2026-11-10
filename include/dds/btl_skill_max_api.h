#ifndef DDS_BTL_SKILL_MAX_API_H

#define DDS_BTL_SKILL_MAX_API_H
#include "common.h"

struct dds_datUnitWork_t;

u32 btlComputeSkillAdjustedMaxHp(struct dds_datUnitWork_t *record);

u32 btlComputeSkillAdjustedMaxMp(struct dds_datUnitWork_t *record);

#endif
