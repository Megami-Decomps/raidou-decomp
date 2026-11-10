#include "shared/effect/effMagatuhi/decls.h"

/* Allocate the dispatch owner before calling the selected payload constructor. */
dds3MagatuhiPar_tag *dds3MagatuhiParCreate(s32 type, void *params) {
    dds3MagatuhiPar_tag *work = sdfChipAllocDirect(sizeof(*work));
    void *payload = dds3MagatuhiCreateTable[type].handler(params);

    work->type = type;
    work->pHandle = payload;
    return work;
}
