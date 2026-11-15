#include "common.h"
#include "fld.h"
#include "fldNpcPath.h"
#include "dds3obj.h"
#include "dds3Basic.h"
#include "ps2asm.h"

/* NON_MATCHING: 4 of 44 words match. Same base+4 pattern, and retail loads pBasic early. */
#ifdef NON_MATCHING
void mapNpcPathTask(void) {
    mapNpcPathSlot_t *slot;
    sdfVector4_t vec;
    s32 i;

    for (i = 0, slot = DDS3_GBWK4; i < 16; i++, slot++) {
        if (slot->used == 0) {
            continue;
        }
        if (slot->control.flag & 1) {
            mapNpcPathControlGetPos(&slot->control);
            VU0_STORE_VF($vf10, &vec);
            dds3SetPosPoseSet_dds3Basic(slot->pBasic, &vec);
        }
        if (slot->control.flag & 2) {
            mapNpcPathControlGetRot(&slot->control);
            VU0_STORE_VF($vf10, &vec);
            dds3SetRotPoseSet(slot->pBasic, &vec);
        }
        mapNpcPathTaskSub(&slot->control);
    }
}
#else
INCLUDE_ASM(const s32, "code_r1_00175858", mapNpcPathTask);
#endif
