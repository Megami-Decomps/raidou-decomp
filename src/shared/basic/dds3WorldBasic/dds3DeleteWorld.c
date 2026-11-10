#include "shared/basic/dds3WorldBasic/decls.h"

void dds3DeleteWorld(void)
{
    if (dds3TheWorld != NULL) {
        dds3DeleteBasic(dds3TheWorld);
        dds3TheWorld = NULL;
    }
}
