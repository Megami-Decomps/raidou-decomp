#include "shared/sdk/libkernl/decls.h"

/* Internal notification: callers discard the SignalSema status. */
void _sceFsSigSema(void)
{
    SignalSema(fsCommandSema);
}
