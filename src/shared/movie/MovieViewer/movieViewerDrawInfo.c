#include "shared/movie/MovieViewer/decls.h"

/* Refresh the window from its two source addresses. */
void movieViewerDrawInfo(void)
{
    if (movieViewerDebugFlag.wordPending != 0) {
        {
            movieViewerDebugFlag.word = *EE_ADDR_AS(s32 *, movieViewerDebugFlag.regAddr);
        }
    }
    if (movieViewerDebugFlag.blockPending != 0) {
        {
            memcpy(movieViewerDebugFlag.block, movieViewerDebugFlag.buffer, 0x40);
        }
    }
}
