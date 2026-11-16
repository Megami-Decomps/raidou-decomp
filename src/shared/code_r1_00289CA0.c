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
        if (sbtl_effonfilecheck[i] == 0 || sbtl_effonfilecheck[i] == 2) {
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
extern const char rodatStrBattleOneplAbsorbEpl[];
extern const char rodatStrBattleOneplAgiSlipEpl[];
extern const char rodatStrBattleOneplAutorecovEpl[];
extern const char rodatStrBattleOneplBufSlipEpl[];
extern const char rodatStrBattleOneplConbAuraEpl[];
extern const char rodatStrBattleOneplDamYuka1Epl[];
extern const char rodatStrBattleOneplDamYuka2Epl[];
extern const char rodatStrBattleOneplDamYukaEpl[];
extern const char rodatStrBattleOneplDamageR21Epl[];
extern const char rodatStrBattleOneplDamageR2Epl[];
extern const char rodatStrBattleOneplDead00Epl[];
extern const char rodatStrBattleOneplDead01Epl[];
extern const char rodatStrBattleOneplDead02Epl[];
extern const char rodatStrBattleOneplDeadYuka1Epl[];
extern const char rodatStrBattleOneplDeadYuka2Epl[];
extern const char rodatStrBattleOneplDeadYukaEpl[];
extern const char rodatStrBattleOneplEncountEpl[];
extern const char rodatStrBattleOneplGuardEpl[];
extern const char rodatStrBattleOneplInvalidEpl[];
extern const char rodatStrBattleOneplJioSlipEpl[];
extern const char rodatStrBattleOneplKaitenEpl[];
extern const char rodatStrBattleOneplMikDam2Epl[];
extern const char rodatStrBattleOneplShoukanEEpl[];
extern const char rodatStrBattleOneplShoukanEpl[];
extern const char rodatStrBattleOneplStonedeadEpl[];
extern const char rodatStrBattleOneplSummonNoEpl[];
extern const char rodatStrBattleOneplZanSlipEpl[];

s8 sbtl_effonfilecheck[48] = {
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    2, 2, 2, 2, 2, 1, 2, 2, 1, 1, 1, 1, 1, 1, 2, 1,
    1, 1, 1, 1, 2, 2, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1,
};

const char *sbtl_effonfilepath[44] = {
    rodatStrBattleOneplDamYuka1Epl,
    rodatStrBattleOneplDamYuka2Epl,
    rodatStrBattleOneplShoukanEpl,
    rodatStrBattleOneplDamageR2Epl,
    rodatStrBattleOneplDeadYukaEpl,
    rodatStrBattleOneplDeadYuka1Epl,
    rodatStrBattleOneplStonedeadEpl,
    rodatStrBattleOneplDeadYuka2Epl,
    rodatStrBattleOneplKaitenEpl,
    rodatStrBattleOneplGuardEpl,
    rodatStrBattleOneplDead00Epl,
    rodatStrBattleOneplInvalidEpl,
    rodatStrBattleOneplDamageR21Epl,
    rodatStrBattleOneplDead01Epl,
    rodatStrBattleOneplDead02Epl,
    rodatStrBattleOneplAutorecovEpl,
    rodatStrBattleOneplAgiSlipEpl,
    rodatStrBattleOneplBufSlipEpl,
    rodatStrBattleOneplJioSlipEpl,
    rodatStrBattleOneplZanSlipEpl,
    rodatStrBattleOneplAbsorbEpl,
    NULL,
    rodatStrBattleOneplMikDam2Epl,
    rodatStrBattleOneplShoukanEEpl,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    rodatStrBattleOneplSummonNoEpl,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    rodatStrBattleOneplDamYukaEpl,
    rodatStrBattleOneplEncountEpl,
    NULL,
    NULL,
    NULL,
    rodatStrBattleOneplConbAuraEpl,
    NULL,
    NULL,
};

INCLUDE_SDATA(const s32, "code_r1_00289CA0", btlLoadSbPackData2);

#endif
