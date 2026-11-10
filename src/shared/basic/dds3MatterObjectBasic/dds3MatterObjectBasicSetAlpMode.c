#include "shared/basic/dds3MatterObjectBasic/decls.h"

void dds3MatterObjectBasicSetAlpMode(dds3Basic_t *aPtr, u32 alpmode) {
    ((dds3MatterObjectBasicWorkData_t *)aPtr->workdata)->alpmode = alpmode;
}
