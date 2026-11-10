#include "shared/sdk/libkernl/decls.h"

fsPowerOffCallback_t scePowerOffHandler(fsPowerOffCallback_t callback, void *argument)
{
    fsPowerOffState_t *state = &fsPowerOffState;
    fsPowerOffCallback_t previous;
    s32 interrupts;

    _sceFsWaitS(0x1B);
    if (fsInitialized == 0) {
        sceFsInit();
    }
    interrupts = DIntr();
    previous = fsPowerOffState.callback;
    state->argument = argument;
    fsPowerOffState.callback = callback;
    if (interrupts != 0) {
        EIntr();
    }
    _sceFsSigSema();
    return previous;
}
