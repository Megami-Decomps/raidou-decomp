#include "shared/sdf/sdfSystem/decls.h"

void sdfStartThreadAndWait(sdfThCB_t *node, void (*entry)(void), void *stack, s64 stackSize, s32 priority, s32 threadArgument) {
    node->thId = CreateThreadSimple((void *)entry, stack, (s32)stackSize, priority);
    WaitSema(sdfThreadSema);
    node->next = sdfVblankThreadId;
    sdfVblankThreadId = node;
    SignalSema(sdfThreadSema);
    _StartThread(node->thId, EE_ADDR_AS(void *, threadArgument));
}
