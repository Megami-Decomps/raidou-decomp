#include "shared/sdf/sdf_PadManager/decls.h"

void sdfPadSetAct0(s32 pad, u16 value) {
    sdfPadCb[pad].actData = value;
}
