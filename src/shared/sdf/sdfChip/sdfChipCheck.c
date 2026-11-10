#include "shared/sdf/sdfChip/decls.h"

s32 sdfChipCheck(s32 aPtr)
{
    s32 inRange;

    inRange = 0;
    if (aPtr >= EE_ADDR_S(sdfChipPageTop)) {
        inRange = aPtr < EE_ADDR_S(sdfChipPageEnd);
    }
    return inRange;
}
