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

/* Effect cylinder of one model id (HD V_POSITION_WORK). */
typedef struct V_POSITION_WORK {
    f32 x;
    f32 y;
    f32 z;
    f32 cylinder;
    f32 height;
} V_POSITION_WORK;

/* One motion-move window: inside [startframe, endframe] of motion motionno the unit moves at sokudo. */
typedef struct MTIONMV_PARA {
#ifdef VERSION_RAIDOU2
    s16 motionno;
    s16 startframe;
    s16 endframe;
    s16 sokudo;
#else
    s8 motionno;
    u8 pad01[3];
    f32 startframe;
    f32 endframe;
    f32 sokudo;
#endif
} MTIONMV_PARA;

/* A model id's motion-move windows (HD MOTIONMOVE_WORK, inline on the PS2). */
typedef struct MOTIONMOVE_WORK {
#ifdef VERSION_RAIDOU2
    MTIONMV_PARA para[8];
#else
    MTIONMV_PARA para[10];
#endif
} MOTIONMOVE_WORK;

#ifdef VERSION_RAIDOU2
SIZE_CHECK(MOTIONMOVE_WORKSizeCheck, MOTIONMOVE_WORK, 0x40);
#else
SIZE_CHECK(MOTIONMOVE_WORKSizeCheck, MOTIONMOVE_WORK, 0xA0);
#endif

/* Per-model tables for devils (major nonzero) and for the player (major zero). */
extern V_POSITION_WORK *sbtleff_cylindertbl;
extern V_POSITION_WORK *sbtleff_cylindertbl_p;
extern MOTIONMOVE_WORK *sbtlmotionmv_tbldata;
extern MOTIONMOVE_WORK *sbtlmotionmove_tbldata_player;

MOTIONMOVE_WORK *sbtlGetMotionMoveData(MODELTYPE major, s32 minor);
s32 sbtlGetCylinderData_ID(MODELTYPE major, s32 minor, struct SBTLVISUAL_DATA *);
s32 sbtlCallBtlCheckDamageTarget8(struct SBTLVISUAL_DATA *data);
s32 sbtlCalcAnim_DmgNumPos(struct SBTLVISUAL_DATA *data, struct SBTLUNIT_WORK *wk);
struct V_POSITION_WORK *sbtlGetEffCylinderData(MODELTYPE major, s32 id);
s32 sbtlGetCylinderData(struct SBTLUNIT_WORK *wk, SBTLVISUAL_DATA *data);

#endif
