#ifndef DDS_FLD_AREA_RESOURCE_NAME_CHANGE_API_H

#define DDS_FLD_AREA_RESOURCE_NAME_CHANGE_API_H
#include "common.h"

typedef u8 FldAreaResourceNameChangeResult;

enum {
    FLD_AREA_RESOURCE_NAME_UNCHANGED = 0,
    FLD_AREA_RESOURCE_NAME_CHANGED = 1
};

FldAreaResourceNameChangeResult fldHasAreaResourceNameChanged(void);

#endif /* DDS_FLD_AREA_RESOURCE_NAME_CHANGE_API_H */
