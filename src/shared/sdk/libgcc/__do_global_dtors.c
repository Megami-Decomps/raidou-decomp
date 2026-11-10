#include "shared/sdk/libgcc/decls.h"

void __do_global_dtors(void) {
    void (**p)(void);

    while (*D_0052BB28 != NULL) {
        p = D_0052BB28;
        D_0052BB28 = p + 1;
        (*p)();
    }
    if (D_0052BB2C == 0) {
        D_0052BB2C = 1;
        __deregister_frame_info(D_0052D940);
    }
}
