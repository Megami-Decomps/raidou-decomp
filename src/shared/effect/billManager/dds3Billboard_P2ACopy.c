#include "shared/effect/billManager/decls.h"

billHandle_t *dds3Billboard_P2ACopy(const billHandle_t *pCopyP2A) {
    dds3BillAnimResource_tag *data;
    s32 n;
    billHandle_t *newobj;

    data = pCopyP2A->u.animationData;
    n = data->AnimNum;
    data->count = data->count + 1;
    newobj = sdfChipAllocDirect(n * 20 + 0x6C);
    newobj->u.animationData = data;
    newobj->resolvedEntries = (dds3BillAnim_tag *)((u8 *)newobj + 0x6C);
    newobj->animationActive = 1;
    newobj->pair.packetList = 0;
    newobj->pair.next = 0;
    dds3Billboard_P2ASetNumber(newobj, 0);
    return newobj;
}
