#include "shared/sdf/sdfGraph/decls.h"

void sdfGrSetDisp(s32 aEvenOdd)
{
    sdfGsMemSlice_t *buff = sdfGrFrameSet.frameHandle[aEvenOdd];
    u32 fbp;

    sceGsSetDefDispEnv((sceGsDispEnv *)EE_UNCACHED(EE_PHYSICAL(EE_ADDR(&sdfGrDispEnv))), 0,
                       sdfGrFrameSet.w, sdfGrFrameSet.buffH, 0, 0);
    fbp = (buff->addr >> 11) & 0x1FF;
    sdfGrDispEnv.dispfb = (sdfGrDispEnv.dispfb & ~0x1FF) | fbp;
    sceGsPutDispEnv(&sdfGrDispEnv);
}
