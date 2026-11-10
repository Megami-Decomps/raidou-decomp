#include "shared/kernel/dds3KernelDraw/decls.h"

sdfDrawTag_t *dds3LinkZClearDrawTag(s32 sw)
{
    sdfDrawTag_t *tag = dds3FontReqAllocDrawTag();

    sdfDrawTagLink(tag, &dds3ZClearDrawTag[sw][dds3GetFrameID()]);
    return tag;
}
