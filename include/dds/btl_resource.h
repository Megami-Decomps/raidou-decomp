#ifndef DDS_BTL_RESOURCE_H

#define DDS_BTL_RESOURCE_H
#include "common.h"

struct EffectSlotSet;

struct sdfMemHandle_t;

/* Panel resource root: both native constructors allocate 0x28 bytes.
 * Preserve each game's established allocator-handle contract. */
typedef struct BtlResBlock {
#ifdef VERSION_DDS1
    s32 unk0;
#else
    struct sdfMemHandle_t *unk0;
#endif
    struct sdfMemHandle_t *resourceAllocationA; /* 0x04: /battle/panel/batle_01.spr */
    struct sdfMemHandle_t *resourceAllocationB; /* 0x08: /battle/panel/batle_02.spr */
    struct sdfMemHandle_t *resourceAllocationC; /* 0x0C: /battle/panel/battle_03.spr */
    struct EffectSlotSet *resA; /* 0x10 */
    struct EffectSlotSet *resB; /* 0x14 */
    struct EffectSlotSet *resC; /* 0x18 */
    s32 unk1C;
    u8 reserved20[8]; /* Undecoded tail of the native allocation. */
} BtlResBlock;

typedef char BtlResBlock_size_must_be_0x28[(sizeof(BtlResBlock) == 0x28) ? 1 : -1];

#endif /* DDS_BTL_RESOURCE_H */
