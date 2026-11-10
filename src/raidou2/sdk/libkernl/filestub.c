#include "common.h"

#include "shared/sdk/libkernl/new_iob.c"

#include "shared/sdk/libkernl/get_iob.c"

INCLUDE_ASM(const s32, "sdk/libkernl/filestub", _sceFs_Rcv_Intr);

#include "shared/sdk/libkernl/_sceFsSemInit.c"

#include "shared/sdk/libkernl/_sceFsWaitS.c"

#include "shared/sdk/libkernl/_sceFsSigSema.c"

#include "shared/sdk/libkernl/scePowerOffHandler.c"

#include "shared/sdk/libkernl/_sceFs_Poff_Intr.c"

INCLUDE_ASM(const s32, "sdk/libkernl/filestub", sceFsInit);

#include "shared/sdk/libkernl/_fs_version.c"

#include "shared/sdk/libkernl/sceFsReset.c"

INCLUDE_ASM(const s32, "sdk/libkernl/filestub", sceOpen);

#include "shared/sdk/libkernl/sceClose.c"

INCLUDE_ASM(const s32, "sdk/libkernl/filestub", sceLseek);

INCLUDE_ASM(const s32, "sdk/libkernl/filestub", sceRead);

INCLUDE_ASM(const s32, "sdk/libkernl/filestub", sceWrite);

INCLUDE_ASM(const s32, "sdk/libkernl/filestub", sceIoctl);

#include "shared/sdk/libkernl/sceIoctl2.c"

#include "shared/sdk/libkernl/_sceCallCode.c"

#include "shared/sdk/libkernl/sceRemove.c"

#include "shared/sdk/libkernl/sceMkdir.c"

#include "shared/sdk/libkernl/sceRmdir.c"

#include "shared/sdk/libkernl/sceFormat.c"

#include "shared/sdk/libkernl/sceAddDrv.c"

#include "shared/sdk/libkernl/sceDelDrv.c"

#include "shared/sdk/libkernl/sceDopen.c"

#include "shared/sdk/libkernl/sceDclose.c"

#include "shared/sdk/libkernl/sceDread.c"

#include "shared/sdk/libkernl/sceGetstat.c"

#include "shared/sdk/libkernl/sceChstat.c"

#include "shared/sdk/libkernl/sceRename.c"

#include "shared/sdk/libkernl/sceChdir.c"

#include "shared/sdk/libkernl/sceSync.c"

#include "shared/sdk/libkernl/sceMount.c"

#include "shared/sdk/libkernl/sceUmount.c"

INCLUDE_ASM(const s32, "sdk/libkernl/filestub", sceLseek64);

INCLUDE_ASM(const s32, "sdk/libkernl/filestub", func_00413030);

#include "shared/sdk/libkernl/sceSymlink.c"

#include "shared/sdk/libkernl/sceReadlink.c"

INCLUDE_RODATA(const s32, "sdk/libkernl/filestub", D_004DBC08);

INCLUDE_RODATA(const s32, "sdk/libkernl/filestub", D_004DBC10);

