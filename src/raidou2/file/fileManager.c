#include "common.h"

#include "shared/file/fileManager/pacFileHandleFree.c"

#include "shared/file/fileManager/pacFileHandleFree_.c"

#include "shared/file/fileManager/fileHandleFree.c"

#include "shared/file/fileManager/fileLinkQueue.c"

#include "shared/file/fileManager/fileUnlinkQueue.c"

#include "shared/file/fileManager/fileLinkCloseWait.c"

#include "shared/file/fileManager/fileUnlinkCloseWait.c"

#include "shared/file/fileManager/pacLoadEx.c"

#include "shared/file/fileManager/pacLoad.c"

#include "shared/file/fileManager/pacLoadUseTail.c"

#include "shared/file/fileManager/fileLoadEx.c"

#include "shared/file/fileManager/fileLoad.c"

#include "shared/file/fileManager/fileLoadUseTail.c"

#include "shared/file/fileManager/fileGetMemHandle.c"

#include "shared/file/fileManager/fileGetLoadPtr.c"

#include "shared/file/fileManager/fileGetLoadSize.c"

#include "shared/file/fileManager/fileGetLoadRemain.c"

#include "shared/file/fileManager/pacFileCheckEnd.c"

#include "shared/file/fileManager/pacCheckEnd.c"

#include "shared/file/fileManager/fileSync.c"

#include "shared/file/fileManager/pacSync.c"

#include "shared/file/fileManager/fileSyncAll.c"

#include "shared/file/fileManager/fileSaveEx.c"

#include "shared/file/fileManager/fileSave.c"

#include "shared/file/fileManager/pacRead.c"

#include "shared/file/fileManager/pacLoadCallback.c"

#include "shared/file/fileManager/fileRead.c"

#include "shared/file/fileManager/fileLoadCallback.c"

#include "shared/file/fileManager/fileWrite.c"

#include "shared/file/fileManager/fileSaveCallback.c"

INCLUDE_ASM(const s32, "file/fileManager", fileExecQueue);

#include "shared/file/fileManager/fileExecCallback.c"

#include "shared/file/fileManager/fileManager.c"

#include "shared/file/fileManager/fileManagerProc.c"

#include "shared/file/fileManager/fileSysInit.c"
INCLUDE_SDATA(const s32, "file/fileManager", D_004E1558);

INCLUDE_SDATA(const s32, "file/fileManager", D_004E1560);

