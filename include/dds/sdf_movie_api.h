#ifndef DDS_SDF_MOVIE_API_H

#define DDS_SDF_MOVIE_API_H
#include "sdf.h"

/* Completion requires drained input and no completed output buffers pending. */
typedef s32 SdfMovieCompletionResult;

enum {
    SDF_MOVIE_COMPLETION_INCOMPLETE = 0,
    SDF_MOVIE_COMPLETION_FINISHED = 1
};

#endif /* DDS_SDF_MOVIE_API_H */
