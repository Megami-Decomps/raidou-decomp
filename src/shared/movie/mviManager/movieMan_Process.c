#include "shared/movie/mviManager/decls.h"

s32 movieMan_Process(void)
{
    sdfIpuRunDecode(SDF_FRAMES(60) / dds3VblanksPerFrame);
    return 0;
}
