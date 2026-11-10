#ifndef DDS_FLD_ACTION_ROW_H

#define DDS_FLD_ACTION_ROW_H
#include "fld.h"

#include "fld_trigger_shape.h"

typedef struct FldActionTransformFields {
    f32 position[4];
    f32 rotation[4];
    f32 scale[4];
} FldActionTransformFields;

typedef union FldActionTransform {
    FldActionTransformFields fields;
    struct {
        f32 position[4];
        f32 rotation[4];
        f32 scale[4];
    };
    f32 values[12];
} FldActionTransform;

typedef struct FldActionRow {
    union {
        struct {
            FldTriggerSphere shape;
            FldTaskInfo info;
        } task;
        FldTriggerShape shape;
    } prefix;
    FldActionTransform transform;
    FldFileResource childResource;
    FldFileResource displayResource;
} FldActionRow;

typedef char FldActionTransform_size_must_be_0x30[
    (sizeof(FldActionTransform) == 0x30) ? 1 : -1];

typedef char FldActionTransformFields_size_must_be_0x30[
    (sizeof(FldActionTransformFields) == 0x30) ? 1 : -1];

typedef char FldActionRow_size_must_be_0x90[
    (sizeof(FldActionRow) == 0x90) ? 1 : -1];

#endif
