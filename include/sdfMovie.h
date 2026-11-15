#ifndef SDFMOVIE_H
#define SDFMOVIE_H

#include "common.h"
#include "sdf_movie_types.h"

void movieCloseStream(sdfMovieCb_t *aMov);
void movieOpenStream(sdfMovieCb_t *stream, sdfMovieInitParm_t *desc, const char *path);

#endif
