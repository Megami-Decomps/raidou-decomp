#ifndef DDS3CAMERAOBJECTBASIC_H
#define DDS3CAMERAOBJECTBASIC_H

#include "common.h"
#include "basic.h"
#include "sdf_types.h"

dds3Basic_t *dds3AddCameraObjectBasicTypeFree(u32 serialID, const sdfVector4_t *pos, const sdfVector4_t *rot);
dds3ObjectBaseControlWorkData_t *dds3GetCameraObjectBaseControlWorkData(dds3Basic_t *basic);

#endif
