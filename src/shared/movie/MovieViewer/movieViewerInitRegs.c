#include "shared/movie/MovieViewer/decls.h"

void movieViewerInitRegs(void)
{
    movieViewerDebugFlag.regAddr = EE_ADDR(IPU_CTRL);
    movieViewerDebugFlag.buffer = (u8 *)&movieCb;
    movieViewerSetFlags();
}
