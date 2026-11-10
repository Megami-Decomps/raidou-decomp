#include "shared/sdk/libgcc/decls.h"

void *__deregister_frame_info(void *fn) {
    object **link;
    object *entry;

    WaitSema(D_0052C37C);
    for (link = &D_0059AD60; *link != NULL; link = &(*link)->next) {
        entry = *link;
        if (entry->fde_begin == fn) {
            *link = entry->next;
            if (entry->fde_array != NULL && entry->fde_array != fn) {
                free(entry->fde_array);
            }
            SignalSema(D_0052C37C);
            return entry;
        }
    }
    SignalSema(D_0052C37C);
    abort();
}
