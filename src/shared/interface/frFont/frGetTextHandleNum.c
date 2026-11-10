#include "shared/interface/frFont/decls.h"

/* Return the live pooled child-glyph count. */
s32 frGetTextHandleNum(void) {
    return gFT.itemCount;
}
