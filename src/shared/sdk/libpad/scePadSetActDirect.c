#include "shared/sdk/libpad/decls.h"

int scePadSetActDirect(int port, int slot, const u8 *data)
{
    padCmd *c;
    int i;
    int ret;

    if (scePadGetDmaStr(port, slot)->ok != 1) {
        return 0;
    }
    c = padPortState[port][slot].cmd;
    for (i = 0; i < 6; i++) {
        c->data[i] = data[i];
    }
    c->cmd = 1;
    c->len = 6;
    ret = 1;
    if (_send_to_iop(port, slot) != 1) {
        ret = 0;
    }
    return ret;
}
