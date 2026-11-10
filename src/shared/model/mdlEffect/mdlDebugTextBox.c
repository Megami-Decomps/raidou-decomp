#include "shared/model/mdlEffect/decls.h"

void mdlDebugTextBox(s32 first, s32 second, s32 color, s32 variant) {
    s32 packedColor = color & 0xffffff;

    dds3PrimDebugTextBoxRGB(mdlDebugScaleWork.packetList, first, second,
                  (mdlDebugScaleWork.editing == 0) ? -1 : variant, packedColor | 0x80000000, 1);
}
