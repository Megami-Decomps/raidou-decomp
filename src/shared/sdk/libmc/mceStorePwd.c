#include "shared/sdk/libmc/decls.h"

void mceStorePwd(char *dest)
{
    if (dest != NULL) {
        const char *source = (const char *)(EE_ADDR(mcPwdBuf) | 0x20000000);
        u32 count;

        /* Native evaluates the length again on the short-string path. */
        if (strlen(source) < 0x400) {
            count = strlen(source);
        } else {
            count = 0x3FF;
        }
        memcpy(dest, (const char *)(EE_ADDR(mcPwdBuf) | 0x20000000), count);
        dest[count] = 0;
    }
}
