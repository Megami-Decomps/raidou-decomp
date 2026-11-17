#include "common.h"
#include "billManager.h"
#include "fileManager.h"
#include "sdfMemory.h"

INCLUDE_ASM(const s32, "code_001F31B0", dds3RefDDS3GBWK2);

void dds3BillboardManagerInit(void)
{
    fileRequest_t *request;
    sdfPacDecodeResrc_t *work;
    billHandle_t **slot;
    billHandle_t *bill;
    dds3BillResource_tag *child;
    s32 i;

    dds3BillboardTag = NULL;
    request = pacLoad("/bill/billtmx.LB");
    pacSync(request);
    work = request->pac.resrcHead;
    if (work != NULL) {
        slot = hGeneralBillboard;
        i = 0;
        do {
            bill = dds3BillboardCreate(BillboardTable[i].kind, EE_ADDR(sdfMemGetPtr(work->resrc)));
            child = bill->u.child;
            child->width *= BillboardTable[i].scale;
            *slot = bill;
            slot++;
            child->height *= BillboardTable[i].scale;
            i++;
            sdfMemFreeDirect(work->resrc);
            work = work->next;
        } while (work != NULL);
    }
    pacFileHandleFree_(request);
}
