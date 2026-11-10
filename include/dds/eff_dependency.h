#ifndef DDS_EFF_DEPENDENCY_H

#define DDS_EFF_DEPENDENCY_H
#include "dds3obj.h"

struct BillConfig;

/* Effect-object dependency states drive handle update and release policy. */
typedef enum EffectDependencyStateKind {
    EFF_DEPENDENCY_STATE_EMPTY = 0,
    EFF_DEPENDENCY_STATE_BOUND_EFFECT_NODE = 1,
    EFF_DEPENDENCY_STATE_BILLBOARD_ANIMATION = 2,
    EFF_DEPENDENCY_STATE_BILLBOARD_NODE = 3,
    EFF_DEPENDENCY_STATE_BORROWED_HANDLE = 4,
    EFF_DEPENDENCY_STATE_EVENT_CUT = 5,
    EFF_DEPENDENCY_STATE_PARAMETERS_READY = 6,
    EFF_DEPENDENCY_STATE_MAGATUHI_TWO_ROWS = 7,
    EFF_DEPENDENCY_STATE_MAGATUHI_FOUR_ROWS = 8
} EffectDependencyStateKind;

/* Complete 0x50-byte kind-7 payload, allocated and cleared by its constructor.
 * `handle`, `node` and `vector` form a state-tagged dependency payload:
 * states 1, 7 and 8 own an effect node in `handle`; states 2 and 3 own a
 * billboard there; state 4 borrows `handle`; state 5 owns a BED mixer in
 * `handle`, an optional BED cut in `node`, and a copied parameter block in
 * `vector`. States 7 and 8 also own a scratch block in `node` and their
 * Magatuhi parameter rows in `vector`. States 0 and 6 have no releasable
 * dependency. Bytes 0x30..0x3B remain opaque. */
typedef struct EffectDependencyState {
    dds_dds3ObjectBaseControlWorkData_t *objectHandle;
    u32 flags;
    EffectDependencyStateKind state;
    void *handle;
    f32 ownerLocalOffset[3];
    u8 pad1C[4];
    dds3Basic_t *owner;
    u16 entryId;
    u16 ownerKind;
    void *vector;
    void *node;
    u8 pad30[0xC];
    struct BillConfig *config;
    f32 sourcePosition[4];
} EffectDependencyState;

typedef char EffectDependencyState_size_must_be_0x50[(sizeof(EffectDependencyState) == 0x50) ? 1 : -1];

#endif
