#include "common.h"

extern void *func_r1_00267550(void);
extern void func_r1_0024F520(void *work);

#ifdef NON_MATCHING
/* Same bytes as retail, but +0x40 is a raw offset into the work block that
 * func_r1_00267550 returns, whose struct is not defined yet. */
s32 btlRunCameraMotionResetTask(void) {
    void *work = func_r1_00267550();

    func_r1_0024F520((u8 *)work + 0x40);
    return 1;
}
#else
INCLUDE_ASM(const s32, "code_002692F0", btlRunCameraMotionResetTask);
#endif
