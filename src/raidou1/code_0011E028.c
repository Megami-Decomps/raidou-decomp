#include "common.h"
#include "newdata.h"
#include "libsdf.h"
#include "kernel.h"
#include "basic.h"
#include "dds3KernelCore.h"
#include "model.h"
#include "dds3ObjectBase.h"

/* body: matched in smt3 @ 9401f79e: src/smt3/basic/dds3EventBasic.c:15, found, same function 1.00 (words 2/2, calls 1.00, shape 1.00) */
void *dds3EventBasicGetResource(dds3Basic_t *pBasic) {
    return pBasic->workdata;
}
