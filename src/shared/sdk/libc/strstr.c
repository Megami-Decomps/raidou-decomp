#include "shared/sdk/libc/decls.h"

char *strstr(const char *haystack, const char *needle)
{
    /* Signed byte views preserve the native loads; comparisons use equality. */
    const s8 *s = (const s8 *)haystack;
    const s8 *n = (const s8 *)needle;
    s32 i;
    if (*s == 0)
        return *n == 0 ? (char *)s : 0;
    do {
        i = 0;
        for (;;) {
            if (n[i] == 0)
                return (char *)s;
            if (n[i] != s[i])
                break;
            ++i;
        }
        ++s;
    } while (*s != 0);
    return 0;
}
