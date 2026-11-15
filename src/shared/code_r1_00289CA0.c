#include "common.h"
#include "sbtlWork.h"
#include "libsdf.h"
#include "fileManager.h"
#include "sdfMemory.h"
#include "sdk_libc.h"

s32 btlLoadSbPack(void)
{
    SBTL_PACK_SLOT *slot = btlLoadSbPackData;
    fileRequest_t *pac;
    sdfPacDecodeResrc_t *resrc;
    s32 i = SBTL_PACK_SLOT_COUNT - 1;

    do {
        i--;
        slot->data = NULL;
        slot->unk8 = NULL;
        slot->handle = NULL;
        slot->unkC = NULL;
        slot++;
    } while (i >= 0);
    pac = pacLoad("/battle/SB_PACK4.LB");
    pacSync(pac);
    for (resrc = pac->pac.resrcHead; resrc != NULL; resrc = resrc->next) {
        i = ((sdfPacStdBlockHeader_t *)resrc->header)->kind;
        if (OnDataLoadProcData[i] == 0 || OnDataLoadProcData[i] == 2) {
            void *srcPtr;
            s32 size;
            void *dst;
            sdfMemHandle_t *dstHandle;
            sdfMemHandle_t *src = resrc->resrc;

            srcPtr = sdfMemGetPtr(src);
            size = sdfMemGetSize(src);
            dstHandle = sdfMemAllocFromTailDirect(size);
            dst = sdfMemLock(dstHandle);

            memcpy(dst, srcPtr, size);
            sdfMemFreeDirect(resrc->resrc);
            btlLoadSbPackData[i].data = dst;
            btlLoadSbPackData[i].handle = dstHandle;
            btlLoadSbPackData2 += size;
        } else {
            sdfMemFreeDirect(resrc->resrc);
        }
    }
    pacFileHandleFree_(pac);
    return 1;
}
#ifdef VERSION_RAIDOU1

INCLUDE_SDATA(const s32, "code_r1_00289CA0", btlLoadSbPackData2);

#endif
