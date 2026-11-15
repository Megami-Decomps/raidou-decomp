#ifndef SBTLVISUAL_H
#define SBTLVISUAL_H

#include "common.h"
#include "enums.h"

struct SBTLVISUAL_DATA;
struct V_POSITION_WORK;

s32 sbtlGetCylinderData_ID(MODELTYPE major, s32 minor, struct SBTLVISUAL_DATA *);
struct V_POSITION_WORK *sbtlGetEffCylinderData(MODELTYPE major, s32 id);

#endif
