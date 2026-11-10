#include "shared/basic/dds3MatterObjectBasic/decls.h"

void dds3MatterObjectBasicSetOt(dds3Basic_t *aPtr, s32 idx) {
    ((dds3MatterObjectBasicWorkData_t *)aPtr->workdata)->OtLinkIdx = idx;
}
