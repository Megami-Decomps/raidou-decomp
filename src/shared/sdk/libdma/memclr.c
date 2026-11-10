#include "shared/sdk/libdma/decls.h"

void memclr(void *p, int n)
{
    u8 *q = p;

    while (n-- != 0) {
        *q++ = 0;
    }
}
