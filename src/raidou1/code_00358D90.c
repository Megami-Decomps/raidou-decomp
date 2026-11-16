#include "common.h"
#include "libsdf.h"
#include "sdf_resource.h"
#include "sdfTex.h"

/* body: matched in smt3 @ 9401f79e: src/smt3/sdf/sdfTex.c:55, found, same function 1.00 (words 2/2, calls 1.00, shape 1.00) */
sdfTexPacket_t *sdfTexGetPacket(sdfTexHandle_t *aHandle)
{
    return aHandle->packet;
}
