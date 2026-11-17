#include "common.h"
#include "newdata.h"
#include "libsdf.h"
#include "kernel.h"
#include "basic.h"
#include "dds3KernelCore.h"
#include "model.h"
#include "dds3ObjectBase.h"
#include "field.h"

/* body: matched in smt3 @ 9401f79e: src/smt3/basic/dds3PointBasic.c:23, found, same function 1.00 (words 2/2, calls 1.00, shape 1.00) */
fldResourcePoint_t *dds3PointBasicGetResource(dds3Basic_t *pPointBasic)
{
    return pPointBasic->workdata;
}

/* body: matched in dds @ 18d1fa88f: src/dds1/basic/dds3PointBasic.c:47, found, same function 1.00 (words 2/2, calls 1.00, shape 1.00) */
void *dds3GetWorldNodeData(dds3Basic_t *node) {
    return node->workdata;
}
