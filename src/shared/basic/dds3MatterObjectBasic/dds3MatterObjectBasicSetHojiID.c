#include "shared/basic/dds3MatterObjectBasic/decls.h"

void dds3MatterObjectBasicSetHojiID(dds3Basic_t *aPtr, u32 hojiID) {
    ((dds3MatterObjectBasicWorkData_t *)aPtr->workdata)->hojiID = hojiID;
}
