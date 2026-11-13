#include "common.h"

extern void *func_00267550(void);
extern void func_0024F520(void *work);

#ifdef NON_MATCHING
/* Same bytes as retail, but +0x40 is a raw offset into the work block that
 * func_00267550 returns, whose struct is not defined yet. */
s32 func_002692F0(void) {
    void *work = func_00267550();

    func_0024F520((u8 *)work + 0x40);
    return 1;
}
#else
INCLUDE_ASM(const s32, "code_002692F0", func_002692F0);
#endif
