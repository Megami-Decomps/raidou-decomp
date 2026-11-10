#include "shared/sdk/libkernl/decls.h"

/* The sign selects the table. Scaling in 32 bits drops that selector and
 * gives the native eight-byte entry offset. Once tested, command is reused
 * for the selected table's actual address; its old value is no longer live.
 */
void sceSifAddCmdHandler(s32 command, void (*handler)(void *, void *), void *argument)
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
    entry->handler = handler;
    entry->argument = argument;
}
