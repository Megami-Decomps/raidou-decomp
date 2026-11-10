#include "shared/sdk/libkernl/decls.h"

void _sceFs_Poff_Intr(void *packet, fsPowerOffState_t *state)
{
    if (state->callback != NULL) {
        state->callback(state->argument);
    }
    EE_SYNC_EI();
}
