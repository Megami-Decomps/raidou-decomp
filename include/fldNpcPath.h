#ifndef FLDNPCPATH_H
#define FLDNPCPATH_H

#include "common.h"
#include "basic.h"

typedef struct { char name[12][24]; } fldNpcPathGroup1_t;
typedef struct { char name[24][24]; } fldNpcPathGroup2_t;

typedef struct mapNpcPathData_t {
    u32 nKey;
    /* Translation paths (type 0) hold points, rotation paths (type 2) quaternions. */
    union {
        sdfVector3_t *pItem;
        sdfVector4_t *pItemR;
    } item;
    u32 *pKey;
} mapNpcPathData_t;

typedef struct mapNpcPathTable_t {
    u32 type;
    mapNpcPathData_t *pData;
} mapNpcPathTable_t;

typedef struct mapNpcPathTableInfo_t {
    u32 nPathTable;
    mapNpcPathTable_t *pPathTable;
    f32 spd;
} mapNpcPathTableInfo_t;

typedef struct mapNpcPathControl_t {
    u32 mode;
    u32 flag;
    f32 frameMax;
    f32 frame;
    f32 rot;
    f32 rotGoal;
    f32 rotSpeed;
    f32 rotHokan;
    mapNpcPathData_t *pTransrate;
    mapNpcPathData_t *pRotation;
    mapNpcPathData_t *pFovy;
    mapNpcPathData_t *pLight;
} mapNpcPathControl_t;

typedef struct mapNpcPathSlot_t {
    s16 used;
    s16 unk2;
    mapNpcPathControl_t control;
    mapNpcPathTableInfo_t *info;
    dds3Basic_t *pBasic;
} mapNpcPathSlot_t;

extern mapNpcPathSlot_t DDS3_GBWK4[16];

#endif /* FLDNPCPATH_H */
