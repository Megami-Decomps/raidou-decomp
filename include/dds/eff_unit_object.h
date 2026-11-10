#ifndef DDS_EFF_UNIT_OBJECT_H

#define DDS_EFF_UNIT_OBJECT_H
#include "common.h"

#include "eff.h"

struct sdfMat_t;

#define EFF_INSTANCE_BYTES 0x88

#define EFF_MATRIX_BYTES 0x40

/* An effect unit instance (EFF_INSTANCE_BYTES): each effect scene object owns
 * its matrices, a billboard and an asset reference. */
typedef struct EffUnitObject {
    f32 localMatrix[4][4]; /* 0x00: VU0 matrix initialized on clone. */
    f32 transform[4][4]; /* 0x40: VU0 matrix copied by dds3P2APolygonSetRotateMatrix. */
    billHandle_t *billboard; /* 0x80: scene-owned billboard */
    struct sdfMat_t *material; /* 0x84: owned render material */
} EffUnitObject;

typedef char EffUnitObject_size_check[(sizeof(EffUnitObject) == EFF_INSTANCE_BYTES) ? 1 : -1];

#endif
