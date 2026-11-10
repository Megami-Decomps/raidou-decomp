#include "shared/kernel/dds3SequenceList/decls.h"

void scrUnlinkHandle(scrProcessWork_s *work)
{
    if (scrHandleFirst == work) {
        scrHandleFirst = work->next;
    } else {
        work->prev->next = work->next;
    }
    if (scrHandleLast == work) {
        scrHandleLast = work->prev;
    } else {
        work->next->prev = work->prev;
    }
    work->prev = NULL;
    work->next = NULL;
    scrHandleCount--;
}
