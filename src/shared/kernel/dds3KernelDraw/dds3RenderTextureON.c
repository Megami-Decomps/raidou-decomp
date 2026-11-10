#include "shared/kernel/dds3KernelDraw/decls.h"

s32 dds3RenderTextureON(void)
{
    s32 result = 0;

    if (dds3RenderTexture == 0) {
        return result;
    }
    dds3RenderTex = 1;
    return 1;
}
