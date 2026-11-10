#include "common.h"

#include "shared/sdf/sdfMovie/movieWaitSema.c"

INCLUDE_ASM(const s32, "sdf/sdfMovie", func_00374350);

INCLUDE_ASM(const s32, "sdf/sdfMovie", func_00374358);

INCLUDE_ASM(const s32, "sdf/sdfMovie", func_00374390);

INCLUDE_ASM(const s32, "sdf/sdfMovie", func_00374608);

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
INCLUDE_SDATA(const s32, "sdf/sdfMovie", D_0046F5B0);

INCLUDE_SDATA(const s32, "sdf/sdfMovie", D_0046F5B4);

INCLUDE_SDATA(const s32, "sdf/sdfMovie", D_0046F5B8);

INCLUDE_SDATA(const s32, "sdf/sdfMovie", D_0046F5BC);

INCLUDE_SDATA(const s32, "sdf/sdfMovie", D_0046F5C0);

INCLUDE_SDATA(const s32, "sdf/sdfMovie", D_0046F5C4);

INCLUDE_SDATA(const s32, "sdf/sdfMovie", D_0046F5C8);

INCLUDE_SDATA(const s32, "sdf/sdfMovie", D_0046F5CB);

INCLUDE_SDATA(const s32, "sdf/sdfMovie", D_0046F5D0);

