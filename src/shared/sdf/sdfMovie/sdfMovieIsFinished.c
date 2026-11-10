#include "shared/sdf/sdfMovie/decls.h"

s32 sdfMovieIsFinished(sdfMovieCb_t *aMov)
{
    if (aMov->decodeCb.drained == 0) {
        return 0;
    }
    return aMov->decodeCb.activeBuffCount == 0;
}
