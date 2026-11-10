#include "shared/basic/dds3EffectObjectBasic/decls.h"

void dds3EffectObjectBasicStatOn(dds3Basic_t *basic, s32 s) {
    ((dds3EffectObjectBasicWorkData_t *)basic->workdata)->stat |= s;
}
