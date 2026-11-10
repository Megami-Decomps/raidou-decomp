#ifndef DDS_DDS3_PATH_H

#define DDS_DDS3_PATH_H
#include "../basic.h"
#include "common.h"

#include "sdf_float_counter.h"

struct EffPrimitiveCurve;

/* Observed serialized path-curve kinds, distinct from runtime flags. */
enum Dds3PathCurveKind {
    DDS3_PATH_CURVE_POSITION = 0,
    DDS3_PATH_CURVE_ROTATION = 2,
    DDS3_PATH_CURVE_SCALAR = 4,
    DDS3_PATH_CURVE_WORLD_TRANSFORM = 5
};

enum Dds3PathChannelFlags {
    DDS3_PATH_POSITION_CHANNEL = 1,
    DDS3_PATH_ROTATION_CHANNEL = 2,
    DDS3_PATH_SCALAR_CHANNEL = 4,
    DDS3_PATH_WORLD_TRANSFORM_CHANNEL = 0x10
};

typedef f32 Dds3PathPositionSample[3];

typedef f32 Dds3PathRotationSample[4];

struct WorldTransformParams;

/* Each channel's sample stride is selected by its enclosing curve kind. */
typedef union Dds3PathSampleData {
    Dds3PathPositionSample *position;
    Dds3PathRotationSample *rotation;
    f32 *scalar;
    struct WorldTransformParams *worldTransform;
} Dds3PathSampleData;

/* A channel descriptor stores kind-specific samples and their frame numbers. */
typedef struct Dds3PathKeyframes {
    u32 count;
    Dds3PathSampleData data; /* Selected by the enclosing Dds3PathCurveEntry.kind. */
    u32 *frames;
} Dds3PathKeyframes;

/* The path object's variable-length curve table starts with this count. */
typedef struct Dds3PathCurveEntry {
    enum Dds3PathCurveKind kind;
    Dds3PathKeyframes *keys;
} Dds3PathCurveEntry;

typedef struct Dds3PathCurveTable {
    u32 count;
    Dds3PathCurveEntry entries[1];
} Dds3PathCurveTable;

typedef char Dds3PathKeyframes_size_must_be_0x0C[
    (sizeof(Dds3PathKeyframes) == 0x0C) ? 1 : -1];

typedef char Dds3PathSampleData_size_must_be_4[
    (sizeof(Dds3PathSampleData) == 0x04) ? 1 : -1];

typedef char Dds3PathCurveEntry_size_must_be_8[
    (sizeof(Dds3PathCurveEntry) == 0x08) ? 1 : -1];

typedef char Dds3PathCurveKind_size_must_be_4[
    (sizeof(enum Dds3PathCurveKind) == 4) ? 1 : -1];

struct dds3Basic_t;

struct dds3PoseSet_t;

struct WorldTransformParams;

/* A pathless mover callback supplies a relative pose. The mover composes it
 * into the target only for the exact apply result; all other words ignore it. */

#define DDS3_MOVER_UPDATE_NO_RELATIVE_TRANSFORM ((s32)0)

#define DDS3_MOVER_UPDATE_APPLY_RELATIVE_TRANSFORM ((s32)1)

typedef s32 (*Dds3MoverUpdate)(struct dds3PoseSet_t *, struct dds3Basic_t *);

/* Kind 3 allocates 0x10 bytes in MoverBasicConstructor. */
typedef struct Dds3SlotResource {
    struct dds3Basic_t *target; /* 0x00: object whose transform is updated. */
    dds3PathBasicControl_t *path; /* 0x04: retained curve work. */
    Dds3MoverUpdate update; /* 0x08: used when no curve work is retained. */
    struct dds3Basic_t *sourceObject; /* 0x0C: kind-16 curve source. */
} Dds3SlotResource;

typedef char Dds3SlotResource_size_must_be_0x10[
    (sizeof(Dds3SlotResource) == 0x10) ? 1 : -1];

void dds3PathBasicControlGetTransform(const dds3PathBasicControl_t *path, struct WorldTransformParams *out);

#endif
