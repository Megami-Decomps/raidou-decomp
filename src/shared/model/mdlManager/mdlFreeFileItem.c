#include "shared/model/mdlManager/decls.h"

void mdlFreeFileItem(void *aItem)
{
    sdfChipFreeDirect(aItem);
}
