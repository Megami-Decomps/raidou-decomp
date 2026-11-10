#include "shared/kernel/dds3SequenceList/decls.h"

s32 ExitBootMc(void)
{
    s32 done;
    s32 ret;

    func_00157050();
    func_00157058();
    done = fmShutDown2();
    ret = -1;
    if (done != 0) {
        ret = 0;
    }
    return ret;
}
