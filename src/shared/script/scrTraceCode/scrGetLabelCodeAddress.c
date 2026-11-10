#include "shared/script/scrTraceCode/decls.h"

s32 scrGetLabelCodeAddress(s32 labelindex)
{
    return nowpwork->LabelTable[labelindex].CodeAddress;
}
