#include "shared/sdf/sdf_PadManager/decls.h"

void sdfPadSetAct1(s32 pad, u8 value) {
    sdfPadCb[pad].actDataBack = value;
}
