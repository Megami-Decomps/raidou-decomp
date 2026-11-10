#include "common.h"

#include "shared/sdf/sdfFileIO/sdfFileCmdUnlink.c"

#include "shared/sdf/sdfFileIO/sdfFileCmdDequeue.c"

#include "shared/sdf/sdfFileIO/sdfFileCmdReleaseHandle.c"

#include "shared/sdf/sdfFileIO/sdfFileCmdFinish.c"

INCLUDE_ASM(const s32, "sdf/sdfFileIO", sdfFileCmdThread);

#include "shared/sdf/sdfFileIO/sdfFileCmdMakeRealFileName.c"

INCLUDE_ASM(const s32, "sdf/sdfFileIO", func_003E3710);

#include "shared/sdf/sdfFileIO/sdfFileCmdNewHandle.c"

#include "shared/sdf/sdfFileIO/sdfFileCmdReadOpen.c"

#include "shared/sdf/sdfFileIO/sdfFileCmdWriteOpen.c"

#include "shared/sdf/sdfFileIO/sdfFileCmdSeek.c"

#include "shared/sdf/sdfFileIO/sdfFileCmdFileSize.c"

#include "shared/sdf/sdfFileIO/sdfFileCmdRead.c"

#include "shared/sdf/sdfFileIO/sdfFileCmdWrite.c"

#include "shared/sdf/sdfFileIO/sdfFileCmdClearError.c"

#include "shared/sdf/sdfFileIO/sdfFileCmdClose.c"

#include "shared/sdf/sdfFileIO/sdfFileCmdFree.c"

#include "shared/sdf/sdfFileIO/sdfFileCmdReadFile.c"

#include "shared/sdf/sdfFileIO/sdfFileCmdWriteFile.c"

#include "shared/sdf/sdfFileIO/sdfFileCmdThreadSetPrio.c"

#include "shared/sdf/sdfFileIO/sdfFileCmdThreadSetLoPrio.c"

#include "shared/sdf/sdfFileIO/sdfFileCmdThreadSetHiPrio.c"

#include "shared/sdf/sdfFileIO/sdfFileCmdThreadCheckPrio.c"

#include "shared/sdf/sdfFileIO/sdfFileCmdStartThread.c"
INCLUDE_SDATA(const s32, "sdf/sdfFileIO", D_004E43C8);

INCLUDE_SDATA(const s32, "sdf/sdfFileIO", D_004E43CC);

INCLUDE_SDATA(const s32, "sdf/sdfFileIO", D_004E43D0);

INCLUDE_SDATA(const s32, "sdf/sdfFileIO", D_004E43D4);

INCLUDE_SDATA(const s32, "sdf/sdfFileIO", D_004E43D6);

INCLUDE_SDATA(const s32, "sdf/sdfFileIO", D_004E43D7);

INCLUDE_SDATA(const s32, "sdf/sdfFileIO", D_004E43D8);

INCLUDE_SDATA(const s32, "sdf/sdfFileIO", D_004E43DC);

INCLUDE_SDATA(const s32, "sdf/sdfFileIO", D_004E43E0);

INCLUDE_SDATA(const s32, "sdf/sdfFileIO", D_004E43E8);

INCLUDE_SDATA(const s32, "sdf/sdfFileIO", D_004E43F0);

INCLUDE_SDATA(const s32, "sdf/sdfFileIO", D_004E43F8);

INCLUDE_SDATA(const s32, "sdf/sdfFileIO", D_004E4400);

INCLUDE_SDATA(const s32, "sdf/sdfFileIO", D_004E4408);

