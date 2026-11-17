#include "common.h"
#include "scrTraceCode.h"
#include "fldProcess.h"
#include "evtUnitManager.h"
#include "fld.h"
#include "mapData.h"
#include "dds3KernelDraw.h"

INCLUDE_ASM(const s32, "code_00179808", nbCallGetIntPara);

INCLUDE_ASM(const s32, "code_00179808", nbMAPSETTAKARA);

INCLUDE_ASM(const s32, "code_00179808", nbCallSetIntReturnValue);

/* NON_MATCHING: 21 of 23 words match.  */
#ifdef NON_MATCHING
/* body: matched in raidou @ 61f09dc: src/shared/event/evtCommand.c:1801, near, same function 0.90 (words 23/23, calls 0.75, shape 0.96) */
s32 nbCallGetIntPara3(void)
{
    s32 map;
    s32 flor;

    map = scrGetIntPara(0);
    flor = scrGetIntPara(1);
    mapCallMap(map, flor, scrGetIntPara(2), NULL);
    return 1;
}
#else
INCLUDE_ASM(const s32, "code_00179808", nbCallGetIntPara3);
#endif

INCLUDE_ASM(const s32, "code_00179808", nbDevilInfoEncid);

INCLUDE_ASM(const s32, "code_00179808", nbLeaf55);

s32 nbCallGetIntPara4(void) {
    fldBgZoomTest(scrGetIntPara(0));
    return 1;
}

/* NON_MATCHING: 21 of 22 words match.  */
#ifdef NON_MATCHING
/* body: matched in raidou @ 61f09dc: src/shared/script/scrCommonCommand.c:489, near, same function 0.92 (words 22/22, calls 0.75, shape 1.00) */
s32 nbCallGetIntPara5(void)
{
    s32 frames;
    s32 alpha;

    frames = scrGetIntPara(0);
    alpha = scrGetIntPara(1);
    dds3SetBlur(frames, alpha, scrGetIntPara(2));
    return 1;
}
#else
INCLUDE_ASM(const s32, "code_00179808", nbCallGetIntPara5);
#endif

INCLUDE_ASM(const s32, "code_00179808", nbCallGetIntPara2);

s32 nbCallNpcTblRefreshWait(void) {
    mdlCallMemFreeDirect();
    if (mapNpcTblRefresh_Wait() == -1) {
        mapNpcTblRefresh();
    }
    if (mapNpcTblRefresh_Wait() == 1) {
        return 0;
    }
    mapMapObjectRefresh();
    mapObjOnOff();
    return 1;
}

INCLUDE_ASM(const s32, "code_00179808", nbCallGetIntPara6);

s32 nbCallGetIntPara7(void) {
    scrSetIntReturnValue(mapLeaf20(scrGetIntPara(0)));
    return 1;
}

INCLUDE_ASM(const s32, "code_00179808", nbCallSetIntReturnValue2);

INCLUDE_ASM(const s32, "code_00179808", nbCallGetIntPara8);

s32 nbCommand_AI_ACT_SKILL(void) {
    evtGetUnitHandle(scrGetIntPara(0))->unk206 = 1;
    return 1;
}

/* NON_MATCHING: 41 of 44 words match.  */
#ifdef NON_MATCHING
s32 nbCallGetIntPara9(void) {
    s32 uniq;
    s32 ofsY;
    s32 hokan;
    f32 x, y, z, r;

    uniq = scrGetIntPara(0);
    ofsY = scrGetIntPara(1);
    hokan = scrGetIntPara(2);
    if (uniq == 0) {
        sndSePlay_sndAt3(hokan);
    } else if (mapObjPointGet(uniq, &x, &y, &z, &r)) {
        mapBgFScrollOn(x, y - ofsY, z, hokan);
    }
    return 1;
}
#else
INCLUDE_ASM(const s32, "code_00179808", nbCallGetIntPara9);
#endif

INCLUDE_ASM(const s32, "code_00179808", evtCommand_TEX_SWAP);

INCLUDE_ASM(const s32, "code_00179808", evtBCheckPanelMode);

INCLUDE_ASM(const s32, "code_00179808", evtCallGetIntPara3);
