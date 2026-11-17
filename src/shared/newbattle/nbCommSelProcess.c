#include "common.h"
#include "dds3KernelCore.h"
#include "itf.h"

#ifdef VERSION_RAIDOU1
s32 nbGetCommSelStat(dds3ProcessID_t *PID) {
    Menu_t *wk = dds3GetProcessData(PID);

    return wk->nowHierarchy;
}
#else
INCLUDE_ASM(const s32, "newbattle/nbCommSelProcess", nbGetCommSelStat);
#endif
