#include "shared/basic/dds3MatterObjectBasic/decls.h"

u32 dds3MatterObjectBasicGetAlpMode(const dds3Basic_t *aPtr) {
    return ((dds3MatterObjectBasicWorkData_t *)aPtr->workdata)->alpmode;
}
