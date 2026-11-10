#include "shared/sdk/libcdvd/decls.h"

void cmd_sem_init(void)
{
    /* Retain the native partial descriptor initialization. The opaque kernel
     * service's treatment of the untouched fields is not inferred here. */
    sdfSemaParam_t param;

    if (cdvdNcmdSema == -1 || cdvdScmdLockSema == -1) {
        param.option = 0;
        param.initCount = 1;
        param.maxCount = 1;
        {
            s32 sema = CreateSema(&param);
            /* Target-specific aid on the actual second-call argument: prepare
             * it before publishing the first result. Its lifetime begins after
             * the first call and ends at the second, with no intervening call. */
            FAKEMATCH_ASM_REG(sdfSemaParam_t *, nextParam, "a0") = &param;
            FAKEMATCH_ASM_LIVE(nextParam);
            cdvdNcmdSema = sema;
            cdvdScmdLockSema = CreateSema(nextParam);
        }
        param.initCount = 0;
        cdvdScmdSema = CreateSema(&param);
        /* Keep the real published semaphore in storage before the following
         * clear-address setup. This empty read-only operand emits no access
         * and is not a memory barrier or an original-source claim. */
        FAKEMATCH_ASM_READ_MEM(cdvdScmdSema);
        cdvdNcmdState = 0;
    }
}
