#include "shared/sdk/libkernl/decls.h"

fsRequestEntry_t *new_iob(void)
{
    fsRequestEntry_t *entry;

    gcc2_compiled__13();
    WaitSema(fsIobSema);
    for (entry = fsIobTable; entry < &fsIobTable[32]; entry++) {
        if (entry->active == 0) {
            entry->active = 0x10000000;
            SignalSema(fsIobSema);
            return entry;
        }
    }
    SignalSema(fsIobSema);
    return NULL;
}
