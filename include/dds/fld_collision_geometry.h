#ifndef DDS_FLD_COLLISION_GEOMETRY_H

#define DDS_FLD_COLLISION_GEOMETRY_H
#include "field.h"

struct FldStopEntry;

typedef s32 FldCollisionAxis;

enum FldCollisionAxis {
    FLD_COLLISION_AXIS_X = 0,
    FLD_COLLISION_AXIS_Y = 1,
    FLD_COLLISION_AXIS_Z = 2
};

/* Six-word runtime view used by both file-loaded and generated geometry. */
typedef struct FldCollisionDescriptor {
    s32 vertexCount;
    s32 faceCount;
    s32 triangleCount;
    f32 (*vertices)[4];
    fldCollisionFace_t *faces;
    struct FldStopEntry *stopData;
} FldCollisionDescriptor;

/* Temporary axis-aligned walls own their vertex/face arrays inline. */
typedef struct FldWallMeshRecord {
    f32 vertices[8][4];
    fldCollisionFace_t faces[4];
    FldCollisionDescriptor geometry;
} FldWallMeshRecord;

typedef char FldCollisionDescriptor_size_must_be_0x18[
    (sizeof(FldCollisionDescriptor) == 0x18) ? 1 : -1];

typedef char FldWallMeshRecord_size_must_be_0x128[
    (sizeof(FldWallMeshRecord) == 0x128) ? 1 : -1];

#endif /* DDS_FLD_COLLISION_GEOMETRY_H */
