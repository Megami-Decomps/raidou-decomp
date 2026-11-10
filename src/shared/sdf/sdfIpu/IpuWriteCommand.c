#include "shared/sdf/sdfIpu/decls.h"

/* Read the FDEC result, then issue FDEC with an eight-bit advance; return the earlier result. */
s32 IpuWriteCommand(void) {
    s32 decodedBits;
    HW_WRITE(IPU_CMD, 0x40000000);
    sceIpuSync(0, 0);
    decodedBits = HW_READ(IPU_CMD);
    sceIpuSync(0, 0);
    HW_WRITE(IPU_CMD, 0x40000008);
    sceIpuSync(0, 0);
    return decodedBits;
}
