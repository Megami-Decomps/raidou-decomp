#include "shared/sdk/libipu/decls.h"

u32 sceIpuSync(s32 mode)
{
    u32 ret = 0;

    switch (mode) {
    case 0: {
        s32 ctrl;

        do {
            ctrl = IPU_CTRL;
        } while (ctrl < 0);
        ret = 0;
        break;
    }
    case 1:
        ret = IPU_CTRL >> 31;
        break;
    }
    return ret;
}
