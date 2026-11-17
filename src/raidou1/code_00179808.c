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

s32 nbCallGetIntPara3(void)
{
    s32 map;
    s32 flor;

    map = scrGetIntPara(0);
    flor = scrGetIntPara(1);
    mapTitleStart_fld(map, flor, scrGetIntPara(2), 1);
    return 1;
}

INCLUDE_ASM(const s32, "code_00179808", nbDevilInfoEncid);

INCLUDE_ASM(const s32, "code_00179808", nbLeaf55);

s32 nbCallGetIntPara4(void) {
    fldBgZoomTest(scrGetIntPara(0));
    return 1;
}

s32 nbCallGetIntPara5(void)
{
    s32 mode;
    s32 point;

    mode = scrGetIntPara(0);
    point = scrGetIntPara(1);
    fldBgZoomTest2(mode, point, scrGetIntPara(2));
    return 1;
}

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
