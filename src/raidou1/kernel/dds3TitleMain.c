#include "common.h"
#include "dds3KernelDraw.h"
#include "mselGlobal.h"

void mselDrawBackground(void)
{
    eglSetOt(0x67);
    eglLinkTest(1, 0, 0x80, 3, 0, 0, 1, 1);
    eglLinkAlpha(0);
    eglLinkSprite(0, 0, 0x200, 0x1C0, 0, 0, 0x200, 0x1C0, 0x80808080, 0x80808080, 0x80808080, 0x80808080,
                  gGlobal->bgTex);
}
