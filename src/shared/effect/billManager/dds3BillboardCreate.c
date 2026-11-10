#include "shared/effect/billManager/decls.h"

billHandle_t *dds3BillboardCreate(s32 index, addr_t data) {
    billHandle_t *newobj;

    newobj = dds3BillboardFuncTable[index].create(data);
    dds3Billboard_BasicInit(newobj);
    newobj->type = index;
    newobj->callback = dds3BillboardFuncTable[index].draw;
    return newobj;
}
