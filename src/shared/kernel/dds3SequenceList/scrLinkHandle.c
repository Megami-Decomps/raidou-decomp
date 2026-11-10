#include "shared/kernel/dds3SequenceList/decls.h"

void scrLinkHandle(scrProcessWork_s *work)
{
    scrProcessWork_s *last = scrHandleLast;

    if (last == NULL) {
        scrHandleFirst = work;
        scrHandleLast = work;
        work->prev = NULL;
        work->next = NULL;
    } else {
        work->prev = last;
        last->next = work;
        work->next = NULL;
        scrHandleLast = work;
    }
    scrHandleCount++;
}
