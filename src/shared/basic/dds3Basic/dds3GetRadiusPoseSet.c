#include "shared/basic/dds3Basic/decls.h"

void dds3GetRadiusPoseSet(dds3Basic_t *basic, f32 r)
{
    basic->poseset->r = r;
}
