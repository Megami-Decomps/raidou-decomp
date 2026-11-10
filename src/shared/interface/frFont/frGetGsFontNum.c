#include "shared/interface/frFont/decls.h"

/* Return the live count of retained font-cache records. */
s32 frGetGsFontNum(void) {
    return gFT.cachedItemCount;
}
