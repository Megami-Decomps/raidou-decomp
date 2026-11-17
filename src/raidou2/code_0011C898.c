#include "common.h"
#include "dds3ObjectBase.h"

void dds3ObjectBaseSetControlBasicCamLen(dds3Basic_t *pObjectBasic) {
    dds3ObjectBaseControlWorkData_t *control;

    control = dds3GetBaseControlWorkData(pObjectBasic);
    control->cam_len = dds3CallMemset(pObjectBasic, ((mdlObj_t *)control->pModel)->inner);
}

f32 dds3CallGetBaseControlWorkData(dds3Basic_t *pObjectBasic) {
    return dds3GetBaseControlWorkData(pObjectBasic)->cam_len;
}
