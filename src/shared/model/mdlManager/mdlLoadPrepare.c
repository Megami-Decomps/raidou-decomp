#include "shared/model/mdlManager/decls.h"

dds3ModelResrc_t *mdlLoadPrepare(u32 group, u32 id)
{
    return EE_ADDR_AS(dds3ModelResrc_t *, mdlPrepare(group, id, 1));
}
