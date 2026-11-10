#include "shared/sdk/libkernl/decls.h"

fsRequestEntry_t *get_iob(u32 index)
{
    fsRequestEntry_t *entry;

    gcc2_compiled__13();
    WaitSema(fsIobSema);
    if (index >= 32) {
        SignalSema(fsIobSema);
        return NULL;
    }
    entry = &fsIobTable[index];
    SignalSema(fsIobSema);
    return entry;
}
