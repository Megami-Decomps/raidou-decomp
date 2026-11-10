#include "shared/basic/dds3Basic/decls.h"

void dds3DeleteBasic(dds3Basic_t *basic)
{
    dds3BasicFunction_t *function;

    if (basic == NULL) {
        return;
    }
    function = basic->basicfunction;
    if (function != NULL && function->destructor != NULL) {
        ((void (*)(dds3Basic_t *))function->destructor)(basic);
    }
    if (basic->prev != NULL) {
        basic->prev->next = basic->next;
    }
    if (basic->next != NULL) {
        basic->next->prev = basic->prev;
    }
    sdfChipFreeDirect(basic);
}
