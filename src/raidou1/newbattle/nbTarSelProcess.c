#include "common.h"

extern u8 *dds3GetProcessData(void);

#ifdef NON_MATCHING
/* Same bytes as retail, but 0xB66 is a raw offset into process data whose
 * struct is not defined yet. The callee takes no argument here, unlike the
 * dds3GetProcessData(task) other units declare. */
u8 func_00190648(void) {
    return dds3GetProcessData()[0xB66];
}
#else
INCLUDE_ASM(const s32, "newbattle/nbTarSelProcess", func_00190648);
#endif
