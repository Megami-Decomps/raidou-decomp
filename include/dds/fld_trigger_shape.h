#ifndef DDS_FLD_TRIGGER_SHAPE_H

#define DDS_FLD_TRIGGER_SHAPE_H
#include "common.h"

typedef u32 FldTriggerShapeKind;

/* Room probes recognize these variants; any other tag matches no probe. */
enum {
    FLD_TRIGGER_SHAPE_KIND_SPHERE = 0,
    FLD_TRIGGER_SHAPE_KIND_PLANE = 1,
    FLD_TRIGGER_SHAPE_KIND_BOX = 2
};

typedef struct FldTriggerSphere {
    FldTriggerShapeKind kind;
    f32 radius;
} FldTriggerSphere;

typedef struct FldTriggerPlane {
    FldTriggerShapeKind kind;
    f32 width;
    f32 height;
} FldTriggerPlane;

typedef struct FldTriggerBox {
    FldTriggerShapeKind kind;
    f32 size[3];
} FldTriggerBox;

/* Trigger data is selected by kind: the serialized variants are 8, 12, and
 * 16 bytes, with one, two, or three float words after kind. Runtime probes
 * read kind through this union before using it. */
typedef union FldTriggerShape {
    FldTriggerShapeKind kind;
    FldTriggerSphere sphere;
    FldTriggerPlane plane;
    FldTriggerBox box;
} FldTriggerShape;

/* The task callback data retains two opaque words around its slot and shape. */
typedef struct FldTaskInfo {
    s32 unk0;
    s32 slot;
    u32 unk08;
    FldTriggerShape *shape;
} FldTaskInfo;

typedef char FldTriggerSphereSizeCheck[(sizeof(FldTriggerSphere) == 0x08) ? 1 : -1];

typedef char FldTriggerPlaneSizeCheck[(sizeof(FldTriggerPlane) == 0x0C) ? 1 : -1];

typedef char FldTriggerBoxSizeCheck[(sizeof(FldTriggerBox) == 0x10) ? 1 : -1];

typedef char FldTriggerShapeSizeCheck[(sizeof(FldTriggerShape) == 0x10) ? 1 : -1];

typedef char FldTaskInfoSizeCheck[(sizeof(FldTaskInfo) == 0x10) ? 1 : -1];

#endif /* DDS_FLD_TRIGGER_SHAPE_H */
