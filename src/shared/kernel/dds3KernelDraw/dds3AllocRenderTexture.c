#include "shared/kernel/dds3KernelDraw/decls.h"

/* Replacing a render texture releases the old texture first. A texture-creation
 * failure leaves the allocated framebuffer handle tracked, as in retail. */
s32 dds3AllocRenderTexture(u16 sx, u16 sy, f32 aspect)
{
    sdfTexHandle_t *texture;

    if (dds3RenderTexture != NULL) {
        dds3FreeRenderTexture();
    }
    dds3RenderTex = 0;
    dds3RenderTexWidth = sx;
    dds3RenderTexHeight = sy;
    dds3RenderTexHandle = sdfGsAllocFrameBuff(sx, sy, 0);
    if (dds3RenderTexHandle == NULL) {
        return 0;
    }
    texture = sdfTexNew(sx, sy, 0, 0, dds3RenderTexHandle, 0, 0, NULL);
    if (texture == NULL) {
        return 0;
    }
    dds3RenderTexture = texture;
    sdfTexSetWrapMode(texture, 5);
    sdfTexMakeGIFPacket(texture);
    dds3RenderScreenParam.aspect = aspect;
    dds3RenderScreenParam.width = sx;
    dds3RenderScreenParam.height = sy;
    dds3InitRenderDrawEnv();
    return 1;
}
