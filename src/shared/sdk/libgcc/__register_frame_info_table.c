#include "shared/sdk/libgcc/decls.h"

s32 __register_frame_info_table(void *fn, void *entry) {
    object *e = entry;

    e->fde_begin = fn;
    e->fde_array = fn;
    e->pc_end = NULL;
    e->pc_begin = NULL;
    e->count = 0;
    WaitSema(D_0052C37C);
    e->next = D_0059AD60;
    D_0059AD60 = e;
    SignalSema(D_0052C37C);
    /* Dead store: blocks the SignalSema sibcall. */
    entry = NULL;
    return;
}
