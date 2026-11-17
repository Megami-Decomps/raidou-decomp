#ifndef DDS3EVENTOBJECTBASIC_H
#define DDS3EVENTOBJECTBASIC_H

#include "common.h"
#include "basic.h"
#include "sdf_types.h"

typedef struct dds3EventObjectBasicWorkData_t
{
    dds3ObjectBaseControlWorkData_t *pObjectBaseControlWorkData;
    u8 unk04[12];
} dds3EventObjectBasicWorkData_t;

dds3Basic_t *dds3AddEventObjectBasic(s32 serialID, const sdfVector4_t *pos, const sdfVector4_t *rot);

#endif
