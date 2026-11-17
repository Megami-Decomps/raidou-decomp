#include "common.h"
#include "newdata.h"
#include "libsdf.h"
#include "kernel.h"
#include "basic.h"
#include "dds3KernelCore.h"
#include "model.h"
#include "dds3ObjectBase.h"

/* body: matched in smt3 @ 9401f79e: src/smt3/code_00115D38.c:8, found, same function 1.00 (words 6/6, calls 1.00, shape 1.00) */
void dds3PathBasicFreeControl(dds3Basic_t *pControl) {
    sdfChipFreeDirect(pControl->workdata);
}
