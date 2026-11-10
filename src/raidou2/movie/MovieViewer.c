#include "common.h"

INCLUDE_ASM(const s32, "movie/MovieViewer", MovieViewerDrawFrameInfo);

INCLUDE_ASM(const s32, "movie/MovieViewer", func_001E92A0);

INCLUDE_ASM(const s32, "movie/MovieViewer", InitMovieViewer);

#include "shared/movie/MovieViewer/FinishMovieViewer.c"

#include "shared/movie/MovieViewer/movieViewerSetFlags.c"

#include "shared/movie/MovieViewer/movieViewerInitRegs.c"

#include "shared/movie/MovieViewer/movieViewerDrawInfo.c"

INCLUDE_RODATA(const s32, "movie/MovieViewer", D_004BBE88);

INCLUDE_ASM(const s32, "movie/MovieViewer", movieViewerDebugDraw);
INCLUDE_SDATA(const s32, "movie/MovieViewer", D_004E1540);

INCLUDE_SDATA(const s32, "movie/MovieViewer", D_004E1548);

INCLUDE_SDATA(const s32, "movie/MovieViewer", D_004E1550);

