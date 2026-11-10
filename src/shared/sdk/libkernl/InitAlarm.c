#include "shared/sdk/libkernl/decls.h"

void InitAlarm(void)
{
    u32 index;
    u32 i;
    void *entry;

    if ((*(volatile u32 *)0x10001810 & 0x100) != 0) {
        return;
    }
    gcc2_compiled__17(alarmEntryPairs[0].index, alarmEntryPairs[0].entry);
    Copy_1((void *)0x80076000, alarmPayload, 0x740);
    Copy_1((void *)0x82000, alarmSmallPayload, 0x28);
    FlushCache(0);
    FlushCache(2);
    gcc2_compiled__17(alarmEntryPairs[1].index, alarmEntryPairs[1].entry);
    for (i = 2; i < 8; i++) {
        index = alarmEntryPairs[i].index;
        entry = GetEntryAddress_0(index);
        gcc2_compiled__17(alarmEntryPairs[i].index, entry);
    }
}
