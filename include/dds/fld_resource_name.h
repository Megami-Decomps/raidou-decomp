#ifndef DDS_FLD_RESOURCE_NAME_H

#define DDS_FLD_RESOURCE_NAME_H
#include "common.h"

typedef struct FldResourceName {
    u32 unk00;
    const char *name;
} FldResourceName;

typedef char FldResourceName_size_must_be_8[
    (sizeof(FldResourceName) == 8) ? 1 : -1];

#endif
