#include "shared/movie/MovieViewer/decls.h"

void FinishMovieViewer(void)
{
    void *proc = dds3SearchProcessName(movieViewerProcName);

    if (proc != NULL) {
        dds3KillProcess(proc, 0);
        movieViewerWork.proc = NULL;
        movieStop();
    }
    movieViewerFree();
}
