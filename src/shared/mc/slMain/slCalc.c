#include "shared/mc/slMain/decls.h"

s32 slCalc(dds3ProcessID_t *id) {
    s32 next;
    u32 job;
    s32 (*cur)(dds3ProcessID_t *);
    gslAnimCnt++;
    slDraw(EE_ADDR_S(id));
    next = proc_func_ans(id);
    if (next == -1) {
        return next;
    }
    job = baseIconFileHandle;
    cur = proc_func_ans;
    if (next != 0) {
        cur = EE_ADDR_AS(slProcFunc_t, next);
    }
    proc_func_ans = cur;
    if (job != 0 && pacFileCheckEnd(EE_ADDR_AS(fileRequest_t *, job)) != 0) {
        baseIconFileHandle = 0;
        slBaseIconMemHandle = fileGetMemHandle((fileRequest_t *)EE_ADDR_PTR(job));
        slBaseIconBuff = EE_ADDR(fileGetLoadPtr((fileRequest_t *)EE_ADDR_PTR(job)));
        slBaseIconBuffSize = fileGetLoadSize((fileRequest_t *)EE_ADDR_PTR(job));
        fileHandleFree((fileRequest_t *)EE_ADDR_PTR(job));
    }
    return 0;
}
