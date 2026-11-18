#include "common.h"
#include "basic.h"

dds3ObjectBaseControlWorkData_t *dds3MatterObjectBasicGetControlWorkData(const dds3Basic_t *pBasic) {
    const dds3MatterObjectBasicWorkData_t *work = pBasic->workdata;
    return work->pObjectBaseControlWorkData;
}
