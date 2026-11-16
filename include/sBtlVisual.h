#ifndef SBTLVISUAL_H
#define SBTLVISUAL_H

#include "common.h"
#include "enums.h"
#include "sdf_types.h"

struct SBTLUNIT_WORK;
struct V_POSITION_WORK;

/* A unit's hit cylinder, filled by sbtlGetCylinderData. */
typedef struct SBTLVISUAL_DATA {
    sdfVector4_t center;
    sdfVector4_t rot;
    f32 hit_area_r;
    f32 width;
    f32 height;
    f32 e_width;
    f32 e_height;
    s16 dummy1;
    s16 dummy2;
} SBTLVISUAL_DATA;

s32 sbtlGetCylinderData_ID(MODELTYPE major, s32 minor, struct SBTLVISUAL_DATA *);
struct V_POSITION_WORK *sbtlGetEffCylinderData(MODELTYPE major, s32 id);
s32 sbtlGetCylinderData(struct SBTLUNIT_WORK *wk, SBTLVISUAL_DATA *data);

#endif
