#include "common.h"
#include "newdata.h"
#include "libsdf.h"
#include "kernel.h"
#include "basic.h"
#include "dds3KernelCore.h"
#include "dds3Basic.h"
#include "dds3SceneBasic.h"
#include "model.h"
#include "dds3ObjectBase.h"

/* body: matched in smt3 @ 9401f79e: src/smt3/basic/dds3MatterObjectBasic.c:40, found, same function 1.00 (words 3/3, calls 1.00, shape 1.00) */
dds3ObjectBaseControlWorkData_t *dds3MatterObjectBasicGetControlWorkData(const dds3Basic_t *pBasic) {
    const dds3MatterObjectBasicWorkData_t *work = pBasic->workdata;
    return work->pObjectBaseControlWorkData;
}
