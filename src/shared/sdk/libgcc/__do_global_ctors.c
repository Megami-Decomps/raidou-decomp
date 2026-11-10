#include "shared/sdk/libgcc/decls.h"

void __do_global_ctors(void) {
    unsigned long count;
    unsigned int index;

    __register_frame_info(D_0052D940, &D_0059AD40);
    count = (s32)D_002F5600[0];
    if (count == (unsigned long)-1) {
        count = 0;
        while (D_002F5600[count + 1] != NULL) {
            count++;
        }
    }
    for (index = count; index != 0; index--) {
        D_002F5600[index]();
    }
    atexit(__do_global_dtors);
}
