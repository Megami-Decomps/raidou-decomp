#include "shared/sdf/sdfDraw/decls.h"

void sdfGrSetDrawFramePacket(sdfGrDrawFramePacket_t *aFrame, s32 aFrameAddr, s32 aW, s32 aH,
                  s32 aPixelFormat, s32 aZBuffAddr, s32 aZBuffFormat,
                  s32 aHalfOffset, s32 aContext) {
    s64 frameRegisterId;
    s64 depthRegisterId;
    s64 offsetRegisterId;
    s64 scissorRegisterId;
    s32 xOffset;
    s32 yOffset;

    if (aContext == 0) {
        frameRegisterId = SDF_GS_FRAME_1;
        depthRegisterId = SDF_GS_ZBUF_1;
        offsetRegisterId = SDF_GS_XYOFFSET_1;
        scissorRegisterId = SDF_GS_SCISSOR_1;
    } else {
        frameRegisterId = SDF_GS_FRAME_2;
        depthRegisterId = SDF_GS_ZBUF_2;
        offsetRegisterId = SDF_GS_XYOFFSET_2;
        scissorRegisterId = SDF_GS_SCISSOR_2;
    }
    aFrame->scissor.reg = scissorRegisterId;
    yOffset = (SDF_GS_CENTER_BIAS - aH) << 3;
    xOffset = (SDF_GS_CENTER_BIAS - aW) << 3;
    aFrame->frame.data = (s64)(aPixelFormat << 24) | (s64)(((aW + 63) >> 6) << 16) | (s64)(aFrameAddr >> 11);
    if (aHalfOffset != 0) {
        yOffset += SDF_GS_FIELD_OFFSET_STEP;
    }
    aFrame->frame.reg = frameRegisterId;
    aFrame->zbuf.data = (s64)(aZBuffFormat << 24) | (s64)(aZBuffAddr >> 11);
    aFrame->zbuf.reg = depthRegisterId;
    aFrame->xyoffset.reg = offsetRegisterId;
    aFrame->scissor.data = ((s64)(aH - 1) << 48) | ((s64)(aW - 1) << 16);
    aFrame->xyoffset.data = xOffset | ((s64)yOffset << 32);
}
