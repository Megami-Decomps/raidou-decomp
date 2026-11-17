#include "common.h"
#include "sbtlWork.h"
#include "sdk_libc.h"
#include "fileManager.h"
#include "sdf_resource.h"
#include "sdfMemory.h"
#include "sBtlUnit.h"
#include "sBtlDvltk.h"

/* NON_MATCHING: 25 of 165 words match. Retail reads msgState with both lhu and lh and lays out the first-load block differently; the logic follows the pseudocode. */
#ifdef NON_MATCHING
s32 sbtlCallBtlGetDevilDataSBtlSub4(struct SBTLUNIT_WORK *wk)
{
    s32 huuma;

    if (wk->flg & 0x800) {
        return 1;
    }
    huuma = sbtlGetDevilData_sbtlSub(wk->unit.devilId)->huumaMesNo;
    if (wk->flg2 & 1) {
        return 1;
    }
    if (sbtlCheckMsgMemType() && wk->msgState == 0) {
        wk->msgFile = NULL;
        wk->msgHandle = sdfMemAllocDirect(sbtlWork6[sbtlBtlLoadHuumaMesDataSub(huuma)]);
        wk->msgData = sdfMemLock(wk->msgHandle);
        memcpy(wk->msgData, (void *)sbtlWork5[sbtlBtlLoadHuumaMesDataSub(huuma)],
               sbtlWork6[sbtlBtlLoadHuumaMesDataSub(huuma)]);
        wk->field4E0 = 0;
        wk->msgState = 2;
        wk->cacheHandle = NULL;
        wk->flg2 |= 1;
        wk->cacheRef = NULL;
        return 1;
    }
    if (wk->msgState == 0) {
        if (!sbtlCheckMsgMemType()) {
            wk->msgFile = fileLoad(sbtlData17[sbtlBtlLoadHuumaMesDataSub(huuma)]);
        }
        wk->msgState++;
        return 0;
    }
    if (wk->msgState != 1) {
        return 0;
    }
    if (sbtlCheckMsgMemType()) {
        wk->msgHandle = sdfMemAllocDirect(sbtlWork6[sbtlBtlLoadHuumaMesDataSub(huuma)]);
        wk->msgData = sdfMemLock(wk->msgHandle);
        memcpy(wk->msgData, (void *)sbtlWork5[sbtlBtlLoadHuumaMesDataSub(huuma)],
               sbtlWork6[sbtlBtlLoadHuumaMesDataSub(huuma)]);
        wk->msgState++;
        wk->flg2 |= 1;
        return 1;
    }
    if (wk->msgData != NULL) {
        return 0;
    }
    if (!pacFileCheckEnd(wk->msgFile)) {
        return 0;
    }
    wk->msgHandle = fileGetMemHandle(wk->msgFile);
    wk->msgData = sdfMemLock(wk->msgHandle);
    fileHandleFree(wk->msgFile);
    wk->msgFile = NULL;
    wk->msgState++;
    wk->flg2 |= 1;
    return 1;
}
#else
INCLUDE_ASM(const s32, "code_r1_002A1E48", sbtlCallBtlGetDevilDataSBtlSub4);
#endif
