#include "shared/basic/dds3EffectObjectBasic/decls.h"

s32 GetESPR(dds3Basic_t *b) {
    dds3PoseSet_t *parameters;

    if (CheckESP(b) == NULL) {
        return 0;
    }
    parameters = b->poseset;
    return (s32)parameters->scl.x;
}
