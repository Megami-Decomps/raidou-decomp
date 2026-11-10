#include "shared/basic/dds3EffectObjectBasic/decls.h"

s32 dds3EffectObjectPresetMG12(dds3Basic_t *eff) {
    if (((u8 *)&eff->type)[3] == EFF_OBJ_KIND) {
        dds3EffectObjectBasicWorkData_t *data = eff->workdata;

        switch (data->type) {
        case EFF_OBJ_STATE_MAGATUHI_TWO_ROWS:
            break;
        case EFF_OBJ_STATE_MAGATUHI_FOUR_ROWS:
            effMagaP4Randomize(data->handle);
            break;
        case EFF_OBJ_STATE_BOUND_BILL:
            effMagaRandomize(data->handle);
            break;
        }
    }
}
