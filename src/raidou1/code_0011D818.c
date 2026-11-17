#include "common.h"
#include "newdata.h"
#include "libsdf.h"
#include "kernel.h"
#include "basic.h"
#include "dds3KernelCore.h"
#include "model.h"
#include "dds3ObjectBase.h"
#include "dds3EventObjectBasic.h"

/* body: matched in smt3 @ 9401f79e: src/smt3/basic/dds3EventObjectBasic.c:70, found, same function 1.00 (words 3/3, calls 1.00, shape 1.00) */
dds3ObjectBaseControlWorkData_t *dds3EventObjectBasicGetControlWorkData(dds3Basic_t *pBasic)
{
    const dds3EventObjectBasicWorkData_t *work = pBasic->workdata;

    return work->pObjectBaseControlWorkData;
}
