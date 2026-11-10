#include "shared/interface/fmGslCont/decls.h"

/* Release the group's handles once and clear its active-node flag. */
s32 fmDestroyFontMemControl(void) {
    if (gFGC.root == 0) {
        return 0;
    }
    sdfMemFreeDirect(gFGC.mh);
    sdfMemFreeDirect(gFGC.ud_mh);
    gFGC.root = 0;
    return 1;
}
