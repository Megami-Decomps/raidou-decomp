#include "shared/sdk/libkernl/decls.h"

void sceSifRemoveCmdHandler(s32 command)
{
    u32 address = (u32)command << 3;
    sifCommandHandler_t *entry;

    if (command < 0) {
        command = EE_ADDR_S(sifCommandContext.systemHandlers);
    } else {
        command = EE_ADDR_S(sifCommandContext.handlers);
    }
    address += (u32)command;
    entry = (sifCommandHandler_t *)address;
    entry->handler = NULL;
}
