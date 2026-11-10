#include "common.h"

#include "shared/sdk/libcdvd/sceCdDelayThread.c"

#include "shared/sdk/libcdvd/sceCdCallback.c"

INCLUDE_ASM(const s32, "sdk/libcdvd/cdvd000", _sceCd_cd_callback);

INCLUDE_ASM(const s32, "sdk/libcdvd/cdvd000", _Cdvd_cbLoop);

#include "shared/sdk/libcdvd/_sceCd_cd_read_intr.c"

#include "shared/sdk/libcdvd/cmd_sem_init.c"

#include "shared/sdk/libcdvd/cdvd_exit.c"

#include "shared/sdk/libcdvd/sceCdPOffCallback.c"

#include "shared/sdk/libcdvd/_sceCd_Poff_Intr.c"

#include "shared/sdk/libcdvd/PowerOffCB.c"

INCLUDE_ASM(const s32, "sdk/libcdvd/cdvd000", sceCdLayerSearchFile);

#include "shared/sdk/libcdvd/sceCdSearchFile.c"

INCLUDE_ASM(const s32, "sdk/libcdvd/cdvd000", _sceCd_ncmd_prechk);

#include "shared/sdk/libcdvd/sceCdNcmdDiskReady.c"

INCLUDE_ASM(const s32, "sdk/libcdvd/cdvd000", sceCdSync);

INCLUDE_ASM(const s32, "sdk/libcdvd/cdvd000", sceCdSyncS);

INCLUDE_ASM(const s32, "sdk/libcdvd/cdvd000", _sceCd_scmd_prechk);

INCLUDE_ASM(const s32, "sdk/libcdvd/cdvd000", sceCdInit);

INCLUDE_ASM(const s32, "sdk/libcdvd/cdvd000", sceCdDiskReady);
