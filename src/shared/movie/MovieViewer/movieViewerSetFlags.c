#include "shared/movie/MovieViewer/decls.h"

void movieViewerSetFlags(void)
{
    movieViewerDebugFlag.wordPending = 1;
    movieViewerDebugFlag.blockPending = 1;
}
