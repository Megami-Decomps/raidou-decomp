#ifndef SDFSYSTEM_H
#define SDFSYSTEM_H

#include "common.h"

typedef struct sdfHandlerNode_t {
    struct sdfHandlerNode_t *next;
    s32 handlerKind;
    s32 channel;
    s32 handlerId;
} sdfHandlerNode_t;

s32 CreateThreadSimple(void *entryAddress, void *workspace, s32 stackBytes, s32 priority);
void FatalError(const char *format, ...) __attribute__((noreturn));
void *sdfAllocAuto(s32 size);
sdfHandlerNode_t *DmacAddHandler(s32 kind, s32 channel, void *handler, s32 next, void *arg);
void sdfIpuRemoveHandler(sdfHandlerNode_t *target);
s32 sdfCreateSema(s32 initCount, s32 maxCount, u32 option);
s32 sdfCreateThreadWithStack(void (*entryAddress)(), s32 stackBytes, s32 priority);
void sdfDelayThread(u32 delay);
s32 sdfFreeAuto(void *allocation);
s32 sdfFreeAutoDelayed(void *allocation);
s32 sdfGetRunTimeCount(s32 aBaseTime);
s32 sdfGetRunTimeCountBase(void);
void sdfWaitVblank(s32 count);

#endif
