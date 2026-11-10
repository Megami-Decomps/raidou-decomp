#include "shared/sdf/sdf2D/decls.h"

void *sdf2DGetPrimPtr(sdfGrPacketHeader_t *aPacketPtr) {
    return aPacketPtr + 1;
}
