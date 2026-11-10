#include "shared/basic/dds3AreaBasic/decls.h"

u32 dds3AreaBasicTypeHasItems(u32 type)
{
    u32 ret = 0;

    if (type >= BASIC_TYPE_CAMERAOBJECT) {
        if (type >= BASIC_TYPE_EVENTOBJECT) {
            ret = type == BASIC_TYPE_EVENTOBJECT;
        }
    }
    return ret;
}
