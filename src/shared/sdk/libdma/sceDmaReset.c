#include "shared/sdk/libdma/decls.h"

int sceDmaReset(int enable)
{
    sceDmaEnv env;
    u32 dmae = *VD_CTRL & 1;
    int i;

    for (i = 0; i < 10; i++) {
        if (sceDmaChanResetFlag[i]) {
            sceDmaChan *chan = sceDmaChanRegs[i];

            chan->chcr = 0;
            chan->tadr = 0;
            chan->madr = 0;
            chan->asr1 = 0;
            chan->asr0 = 0;
            chan->sadr = 0;
        }
    }
    {
        volatile u32 *stat = VD_STAT;

        *stat = 0xFF1F;
        *(u32 *)stat = *stat & 0xFF1F0000;
    }
    memclr(&env, sizeof(env));
    sceDmaPutEnv(&env);
    if (enable == 1) {
        *VD_CTRL = *VD_CTRL | 1;
    }
    return dmae;
}
