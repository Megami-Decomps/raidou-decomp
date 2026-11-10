#include "common.h"

INCLUDE_ASM(const s32, "movie/MovieViewer", MovieViewerDrawFrameInfo);

INCLUDE_ASM(const s32, "movie/MovieViewer", func_001D2F28);

INCLUDE_ASM(const s32, "movie/MovieViewer", InitMovieViewer);

#include "shared/movie/MovieViewer/FinishMovieViewer.c"

#include "shared/movie/MovieViewer/movieViewerSetFlags.c"

#include "shared/movie/MovieViewer/movieViewerInitRegs.c"

#include "shared/movie/MovieViewer/movieViewerDrawInfo.c"

INCLUDE_RODATA(const s32, "movie/MovieViewer", D_00455118);

INCLUDE_ASM(const s32, "movie/MovieViewer", movieViewerDebugDraw);
INCLUDE_SDATA(const s32, "movie/MovieViewer", D_0046CDA8);

INCLUDE_SDATA(const s32, "movie/MovieViewer", D_0046CDB0);

INCLUDE_SDATA(const s32, "movie/MovieViewer", D_0046CDB8);

