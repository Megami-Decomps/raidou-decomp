#include "shared/sdk/libc/decls.h"

s32 strcasecmp(const char *left, const char *right)
{
    const s8 *s = (const s8 *)left;
    const s8 *t = (const s8 *)right;

    /* Retail signed-byte classification can reach the adjacent mapped CFI
     * table, and 0xE0 versus NUL can advance beyond the right terminator.
     * Use EE integer addresses for these native mapped-memory operations. */
    while (*s &&
           ({ s32 c = *s;
              (*EE_ADDR_AS(const u8 *, EE_ADDR(_ctype_) + 1 + c) & 1)
                  ? c + 32 : c; }) ==
           ({ s32 c = *t;
              (*EE_ADDR_AS(const u8 *, EE_ADDR(_ctype_) + 1 + c) & 1)
                  ? c + 32 : c; })) {
        s = EE_ADDR_AS(const s8 *, EE_ADDR(s) + 1);
        t = EE_ADDR_AS(const s8 *, EE_ADDR(t) + 1);
    }
    return ({ s32 c = *(const u8 *)s;
              ((_ctype_ + 1)[c] & 1) ? c + 32 : c; }) -
           ({ s32 c = *(const u8 *)t;
              ((_ctype_ + 1)[c] & 1) ? c + 32 : c; });
}
