#include "shared/sdf/sdf3DPrim/decls.h"

/* vu0 routine: store the resident matrix and inverse-column-length scaled rows, then VIF ITOP/MSCAL. */
void sdf3DPrimSetTmPacket(sdf3DTmPacketData_t *aPacket) {
    VU0_STORE_MATRIX_AND_UNIT_ROWS(aPacket);
    aPacket->tag[0] = SDF_VIF_ITOP_LIGHTING;
    aPacket->tag[1] = SDF_VIF_MSCAL_COMMAND;
    aPacket->tag[2] = 0;
    aPacket->tag[3] = 0;
}
