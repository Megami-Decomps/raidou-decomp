#include "shared/sdk/libcdvd/decls.h"

s32 PowerOffCB(void)
{
    s32 intr;

    cdvdPowerOffActive = 1;
    intr = DIntr();
    sceSifAddCmdHandler(0x80000012, _sceCd_Poff_Intr, NULL);
    if (intr) {
        EIntr();
    }
    cdvdPowerOffActive = 0;
    cdvdScmdState[0] = 1;
    return 1;
}
