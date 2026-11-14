#include "common.h"

extern void *sbtlRefSbtl38(void);
extern void sbtlSetCutScroll(void *work);

#ifdef NON_MATCHING
/* Same bytes as retail, but +0x40 is a raw offset into the work block that
 * sbtlRefSbtl38 returns, whose struct is not defined yet. */
s32 btlRunCameraMotionResetTask(void) {
    void *work = sbtlRefSbtl38();

    sbtlSetCutScroll((u8 *)work + 0x40);
    return 1;
}
#else
INCLUDE_ASM(const s32, "code_002692F0", btlRunCameraMotionResetTask);
#endif
