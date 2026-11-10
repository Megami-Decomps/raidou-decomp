#include "shared/movie/mviManager/decls.h"

void movieStop(void)
{
    if (PlayingNo != 0) {
        movieCloseStream(&movieCb);
        dds3KillProcess(PlayingNo, 0);
        PlayingNo = 0;
    }
}
