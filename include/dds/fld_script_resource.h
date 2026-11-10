#ifndef DDS_FLD_SCRIPT_RESOURCE_H

#define DDS_FLD_SCRIPT_RESOURCE_H
#include "sdf_model.h"

struct MotionTable;

typedef struct FldScriptResource {
    u8 opaque00[0x10];
    sdfModelDef_t itemList;
    struct MotionTable *motionTable;
} FldScriptResource;

typedef char FldScriptResourceSizeCheck[(sizeof(FldScriptResource) == 0x24) ? 1 : -1];

#endif
