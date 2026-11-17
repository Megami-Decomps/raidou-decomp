#include "common.h"
#include "fldProcess.h"
#include "mdlManager.h"

void mdlSetMotionSpeed(s32 speed) {
    mapNakamaTaskData = speed;
}

INCLUDE_ASM(const s32, "code_0016CA30", mapNakamaWachLock_fld);

void mdlChkMotionEnd(void) {
    mapNakamaTaskData2 = 0;
}

INCLUDE_ASM(const s32, "code_0016CA30", mapNakamaAlphaFade);

INCLUDE_ASM(const s32, "code_0016CA30", mapNakamaColorAnime);

INCLUDE_ASM(const s32, "code_0016CA30", mapNakamaMinAlpDown);

INCLUDE_ASM(const s32, "code_0016CA30", mapRefDvModel);

/* NON_MATCHING: 11 of 24 words match.  */
#ifdef NON_MATCHING
void mapNakamaWakeUp(s32 mode) {
    if (s_MapNakamaColorCnt != mode) {
        s_MapNakamaColorCnt = mode;
        if (mode) {
            s_MapNakamaColorNow = 0;
        } else {
            s_MapNakamaColorNow = 128;
        }
        if (summonDvModel) {
            s32 c = s_MapNakamaColorNow;
            mdlSetColor(summonDvModel, (c << 24) | (c << 16) | (c << 8) | c);
        }
    }
}
#else
INCLUDE_ASM(const s32, "code_0016CA30", mapNakamaWakeUp);
#endif

INCLUDE_ASM(const s32, "code_0016CA30", mapNakamaPosRot);

INCLUDE_ASM(const s32, "code_0016CA30", mapNakamaMotion);

INCLUDE_ASM(const s32, "code_0016CA30", mapCallSetMotionSpeed);

INCLUDE_ASM(const s32, "code_0016CA30", fldPlayerMotion_Single);

INCLUDE_ASM(const s32, "code_0016CA30", mapNakamaReturnStart);

INCLUDE_ASM(const s32, "code_0016CA30", mapNakamaSkil_ClearBit);

INCLUDE_ASM(const s32, "code_0016CA30", mapCallNakamaWakeUp3);

INCLUDE_ASM(const s32, "code_0016CA30", mapCallNakamaWakeUp);

INCLUDE_ASM(const s32, "code_0016CA30", mapNakamaZeniStart);

INCLUDE_ASM(const s32, "code_0016CA30", mapNakamaHiroStart);

INCLUDE_ASM(const s32, "code_0016CA30", mapCallNakamaWakeUp4);

INCLUDE_ASM(const s32, "code_0016CA30", mapNakamaJoinStart);

INCLUDE_ASM(const s32, "code_0016CA30", mapCallNakamaWakeUp5);

INCLUDE_ASM(const s32, "code_0016CA30", mapCallNakamaWakeUp6);

INCLUDE_ASM(const s32, "code_0016CA30", fldCallNakamaWakeUp);

INCLUDE_ASM(const s32, "code_0016CA30", fldCallNakamaWakeUp2);

INCLUDE_ASM(const s32, "code_0016CA30", fldFastTravelStart);

INCLUDE_ASM(const s32, "code_0016CA30", fldCallNakamaWakeUp3);

INCLUDE_ASM(const s32, "code_0016CA30", mapCallNakamaWakeUp7);

INCLUDE_ASM(const s32, "code_0016CA30", mapNakamaSearchTask);
