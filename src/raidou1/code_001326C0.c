#include "common.h"
#include "field.h"
#include "sdfMemory.h"
#include "sdfMat.h"
#include "fldFileResolver_00132080.h"

/* NON_MATCHING: 97 of 98 words match.  */
#ifdef NON_MATCHING
void fldCallTexListFree(void)
{
    s32 i;

    if (fldGlobalBase.amSBlock < 200) {
        fldLeaf16();
    }
    sdfTexListFree(fldData15, 1);
    fldData15 = NULL;
    for (i = 0; i < 4; i++) {
        if (fldD3pHandle[i] != NULL) {
            sdfMemFreeDelayedDirect(fldD3pHandle[i]);
            fldD3pHandle[i] = NULL;
            fldD3pData[i] = NULL;
        }
    }
    if (fldData16 != 0) {
        if (fldCallTexListFreeData != 0) {
            fldAddressResolverReset(fldCallTexListFreeData2, EE_ADDR_AS(struct fldTransferChunk_t *, fldCallTexListFreeData));
            fldCallTexListFreeData = 0;
            fldCallTexListFreeData2 = 0;
        }
        if (fldCallTexListFreeData3 != 0) {
            fldAddressResolverReset(fldCallTexListFreeData4, EE_ADDR_AS(struct fldTransferChunk_t *, fldCallTexListFreeData3));
            fldCallTexListFreeData3 = 0;
            fldCallTexListFreeData4 = 0;
        }
        if (fldCallTexListFreeData5 != 0) {
            fldAddressResolverReset(fldCallTexListFreeData6, EE_ADDR_AS(struct fldTransferChunk_t *, fldCallTexListFreeData5));
            fldCallTexListFreeData5 = 0;
            fldCallTexListFreeData6 = 0;
        }
    }
    MakeResouceBasicFL2Data = 0;
    fldCallTexListFreeData7 = 0;
    if (fldCallTexListFreeData8 != NULL) {
        sdfMemFreeDelayedDirect(fldCallTexListFreeData8);
        fldCallTexListFreeData8 = NULL;
    }
    if (fldData17 != 0) {
        sdfMemFreeDelayedDirect(EE_ADDR_AS(sdfMemHandle_t *, fldData17));
        fldData17 = 0;
    }
    if (fldCallTexListFreeData9 != NULL) {
        sdfMemFreeDelayedDirect(fldCallTexListFreeData9);
        fldCallTexListFreeData9 = NULL;
    }
    if (fldCallTexListFreeData10 != NULL) {
        sdfMemFreeDelayedDirect(fldCallTexListFreeData10);
        fldCallTexListFreeData10 = NULL;
    }
    fldGlobalBase.nameU = 0;
}
#else
INCLUDE_ASM(const s32, "code_001326C0", fldCallTexListFree);
#endif

INCLUDE_ASM(const s32, "code_001326C0", mapRegistBEFieldSub);
