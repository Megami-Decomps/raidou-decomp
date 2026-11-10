#include "shared/sdk/libcdvd/decls.h"

s32 sceCdSearchFile(sceCdlFILE *file, const char *name)
{
    return sceCdLayerSearchFile(EE_ADDR(file), (char *)name, 0);
}
