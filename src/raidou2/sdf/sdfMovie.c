#include "common.h"

#include "shared/sdf/sdfMovie/movieWaitSema.c"

INCLUDE_ASM(const s32, "sdf/sdfMovie", func_003E9F90);

INCLUDE_ASM(const s32, "sdf/sdfMovie", func_003E9F98);

INCLUDE_ASM(const s32, "sdf/sdfMovie", func_003E9FD0);

INCLUDE_ASM(const s32, "sdf/sdfMovie", func_003EA248);

#include "shared/sdf/sdfMovie/movieReadNextChunk.c"

#include "shared/sdf/sdfMovie/movieGetStreamSize.c"

#include "shared/sdf/sdfMovie/movieReadHeaderChunk.c"

INCLUDE_ASM(const s32, "sdf/sdfMovie", movieReadChunk);

#include "shared/sdf/sdfMovie/movieCopyChunk.c"

INCLUDE_ASM(const s32, "sdf/sdfMovie", movieMemcpy);

INCLUDE_ASM(const s32, "sdf/sdfMovie", movieParseHeader);

#include "shared/sdf/sdfMovie/movieOpenStream.c"

#include "shared/sdf/sdfMovie/movieCloseStream.c"

#include "shared/sdf/sdfMovie/sdfMovieIsFinished.c"
INCLUDE_SDATA(const s32, "sdf/sdfMovie", D_004E45C8);

INCLUDE_SDATA(const s32, "sdf/sdfMovie", D_004E45CC);

INCLUDE_SDATA(const s32, "sdf/sdfMovie", D_004E45D0);

INCLUDE_SDATA(const s32, "sdf/sdfMovie", D_004E45D4);

INCLUDE_SDATA(const s32, "sdf/sdfMovie", D_004E45D8);

INCLUDE_SDATA(const s32, "sdf/sdfMovie", D_004E45DC);

INCLUDE_SDATA(const s32, "sdf/sdfMovie", D_004E45E0);

INCLUDE_SDATA(const s32, "sdf/sdfMovie", D_004E45E3);

INCLUDE_SDATA(const s32, "sdf/sdfMovie", D_004E45E8);

