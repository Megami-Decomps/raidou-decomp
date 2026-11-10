#include "shared/sdf/sdfGraph/decls.h"

void sdfGrSetFrameSetMode(sdfGrFrameSet_t *aFrame, s32 aMode, s32 aReqHandleClear)
{
    /* Target-specific allocation aid: retain the native row, destination and
     * depth-format homes. The read constraints order the live depth-format
     * value without adding accesses or extending it beyond its real store.
     * This is not evidence of the original source spelling. */
    FAKEMATCH_ASM_REG(sdfGrScreenDef_t *, def, "$2") = &sdfGrFrameModes[aMode];
    FAKEMATCH_ASM_REG(sdfGrFrameSet_t *, frame, "$10") = aFrame;
    s16 w;
    s16 h;
    s16 buffH;
    u16 pixelFormat;
    FAKEMATCH_ASM_REG(u16, zbuffFormat, "$9");

    if (aReqHandleClear != 0) {
        frame->frameHandle[0] = NULL;
        frame->frameHandle[1] = NULL;
        frame->zbuffHandle = NULL;
    }
    zbuffFormat = def->zbuffFormat;
    FAKEMATCH_ASM_READ(zbuffFormat);
    w = def->w;
    h = def->h;
    buffH = def->buffH;
    pixelFormat = def->pixelFormat;
    frame->w = w;
    frame->h = h;
    frame->buffH = buffH;
    FAKEMATCH_ASM_READ(zbuffFormat);
    frame->zbuffFormat = zbuffFormat;
    frame->pixelFormat = pixelFormat;
}
