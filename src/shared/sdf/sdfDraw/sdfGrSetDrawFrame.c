#include "shared/sdf/sdfDraw/decls.h"

void sdfGrSetDrawFrame(sdfGrDrawFrame_t *aFrame, s32 aFrameAddr, s32 aW, s32 aH,
                           s32 aPixelFormat, s32 aZBuffAddr, s32 aZBuffFormat, s32 aContext) {
    sdfGrSetRegsPacketHeader(&aFrame->header, SDF_SCENE_DRAW_PAYLOAD_QWORDS);
    sdfGrSetDrawFramePacket(&aFrame->packet, aFrameAddr, aW, aH, aPixelFormat, aZBuffAddr, aZBuffFormat, 0, aContext);
}
