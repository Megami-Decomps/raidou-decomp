#include "shared/sdf/sdf_PadManager/decls.h"

void sdfPadChangeMode(s32 aPadNum, u8 mode) {
    sdfPadCb[aPadNum].reqMode = mode;
}
