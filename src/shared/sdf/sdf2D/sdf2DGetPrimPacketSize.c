#include "shared/sdf/sdf2D/decls.h"

s32 sdf2DGetPrimPacketSize(s32 aNumOfRegs, s32 aNumOfPrim) {
    return (aNumOfRegs * aNumOfPrim + 2) << SDF_DMA_QWORD_SHIFT;
}
