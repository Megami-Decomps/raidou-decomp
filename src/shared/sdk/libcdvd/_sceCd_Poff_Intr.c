#include "shared/sdk/libcdvd/decls.h"

void _sceCd_Poff_Intr(void *packet, void *argument)
{
    if (cdvdPowerCallback != NULL && cdvdPowerOffActive == 0) {
        cdvdPowerCallback(cdvdPowerArgument);
    }
}
