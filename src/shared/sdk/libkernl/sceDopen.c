#include "shared/sdk/libkernl/decls.h"

s32 sceDopen(const char *path)
{
    fsRequestEntry_t *entry;
    s32 result;
    s32 sema;

    _sceFsWaitS(9);
    if (fsInitialized == 0) {
        sceFsInit();
    }
    _sceFsSigSema();
    entry = new_iob();
    if (entry == NULL) {
        return -19;
    }
    result = _sceCallCode(path, 9);
    if (result < 0) {
        WaitSema(fsIobSema);
        entry->active = 0;
        sema = fsIobSema;
    } else {
        WaitSema(fsIobSema);
        entry->word0 = result;
        sema = fsIobSema;
        result = entry - fsIobTable;
    }
    SignalSema(sema);
    return result;
}
