#include "common.h"

#include "shared/sdf/sdfFile/sdfFileSetCdFilename.c"

#include "shared/sdf/sdfFile/sdfFileSetHostFilename.c"

#include "shared/sdf/sdfFile/sdfFileSetHddFilename.c"

#include "shared/sdf/sdfFile/sdfFileChkCdFile.c"

#include "shared/sdf/sdfFile/sdfFileChkHostFile.c"

#include "shared/sdf/sdfFile/sdfFileChkHddFile.c"

#include "shared/sdf/sdfFile/sdfFileMakeFilename.c"

#include "shared/sdf/sdfFile/sdfFileCallback.c"

INCLUDE_ASM(const s32, "sdf/sdfFile", sdfFileOpen);

INCLUDE_ASM(const s32, "sdf/sdfFile", func_003E28D0);

#include "shared/sdf/sdfFile/sdfFileClose.c"

#include "shared/sdf/sdfFile/sdfFileRead.c"

#include "shared/sdf/sdfFile/sdfFileLength.c"

#include "shared/sdf/sdfFile/sdfFileSeek.c"

#include "shared/sdf/sdfFile/sdfFileSetHostRoot.c"

#include "shared/sdf/sdfFile/sdfFileGetHostRoot.c"

INCLUDE_ASM(const s32, "sdf/sdfFile", sdfFileSetDefaultSrc);

INCLUDE_ASM(const s32, "sdf/sdfFile", func_003E2AB8);

#include "shared/sdf/sdfFile/sdfFileCmdAllocHandle.c"
INCLUDE_SDATA(const s32, "sdf/sdfFile", D_004E4398);

INCLUDE_SDATA(const s32, "sdf/sdfFile", D_004E439C);

INCLUDE_SDATA(const s32, "sdf/sdfFile", D_004E43A0);

INCLUDE_SDATA(const s32, "sdf/sdfFile", D_004E43A8);

INCLUDE_SDATA(const s32, "sdf/sdfFile", D_004E43B0);

INCLUDE_SDATA(const s32, "sdf/sdfFile", D_004E43B8);

INCLUDE_SDATA(const s32, "sdf/sdfFile", D_004E43C0);

INCLUDE_SDATA(const s32, "sdf/sdfFile", D_004E43C4);

