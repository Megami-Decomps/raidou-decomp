#ifndef SDFMOVIE_H
#define SDFMOVIE_H

#include "common.h"
#include "sdf_movie_types.h"

void movieCloseStream(sdfMovieCb_t *aMov);
s32 sdfMovieIsFinished(sdfMovieCb_t *aMov);
void release_movieobj2(sdfMovieCb_t *obj, struct sdfGrOt_t *ot);
void movieOpenStream(sdfMovieCb_t *stream, sdfMovieInitParm_t *desc, const char *path);
void sdfSetStreamFrameDrawMode(u32 value);
void sdfSetGridScaledDrawBounds(s32 column, s32 row, s32 width, s32 height, u32 mode);

#endif
