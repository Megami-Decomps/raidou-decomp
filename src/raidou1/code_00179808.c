#include "common.h"
#include "scrTraceCode.h"
#include "fldProcess.h"
#include "evtUnitManager.h"
#include "fld.h"
#include "mapData.h"
#include "dds3KernelDraw.h"
#include "code_r1_0010DF68.h"
#include "EventBit.h"
#include "dds3ObjectBase.h"
#include "fldFileResolver.h"
#include "dds3Randomize.h"
#include "sbtlWork.h"

s32 nbCallGetIntPara(void) {
    s32 idx = nbCallGetIntPara2Data[1];
    s32 para = scrGetIntPara(0);
    s32 row;
    s32 sum;
    s32 roll;
    s32 i;

    switch (para) {
    case 0:
        scrSetIntReturnValue(nbCallGetIntParaData[idx].mode);
        break;
    case 1:
        if (nbCallGetIntParaData[idx].mode == 1) {
            row = nbData[gohData7[DDS3_GBWK->mapSummonID].field68];
            sum = 0;
            roll = dds3GetRandIntA(NULL, 100);
            for (i = 0; i < 6; i++) {
                sum += nbCallGetIntParaData4[row][i].rate;
                if (roll < sum) {
                    nbCallGetIntParaData5 = nbCallGetIntParaData4[row][i].sub;
                    scrSetIntReturnValue(nbCallGetIntParaData4[row][i].id);
                    return 1;
                }
            }
            nbCallGetIntParaData5 = nbCallGetIntParaData4[row][0].sub;
            scrSetIntReturnValue(nbCallGetIntParaData4[row][0].id);
        } else {
            scrSetIntReturnValue(nbCallGetIntParaData[idx].value1);
        }
        break;
    case 2:
        if (nbCallGetIntParaData[idx].mode == 1) {
            scrSetIntReturnValue(nbCallGetIntParaData5);
        } else {
            scrSetIntReturnValue(nbCallGetIntParaData[idx].value2);
        }
        break;
    case 3:
        scrSetIntReturnValue(nbCallGetIntParaData[idx].value3);
        break;
    case 4:
        scrSetIntReturnValue(nbCallGetIntParaData[idx].value4);
        break;
    }
    return 1;
}

s32 nbMAPSETTAKARA(void) {
    datDebugPrintf(nbStrMapSetTakaraOpen02, fldGlobalBase.nowTakaraTblIdx);
    mapTakaraOpen_fld(fldGlobalBase.nowTakaraTblIdx);
    return 1;
}

s32 nbCallSetIntReturnValue(void) {
    if (mapRefDDS3GBWK5(fldGlobalBase.nowTakaraTblIdx)) {
        scrSetIntReturnValue(1);
    } else {
        scrSetIntReturnValue(0);
    }
    return 1;
}

s32 nbCallGetIntPara3(void)
{
    s32 map;
    s32 flor;

    map = scrGetIntPara(0);
    flor = scrGetIntPara(1);
    mapTitleStart_fld(map, flor, scrGetIntPara(2), 1);
    return 1;
}

INCLUDE_RODATA(const s32, "code_00179808", nbStrMapSetTakaraOpen02);

INCLUDE_ASM(const s32, "code_00179808", nbDevilInfoEncid);

s32 nbLeaf55(void) {
    return 1;
}

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

s32 nbCallGetIntPara2(void) {
    s32 uniq = scrGetIntPara(0);

    if (uniq == 0) {
        scrSetIntReturnValue(fldGlobalBase.nowTalkModelID);
    } else {
        scrSetIntReturnValue(mapObjGetBasic(uniq)->serialID);
    }
    return 1;
}

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

/* NON_MATCHING: 23 of 47 words match. state in a0 not v1 and the 0x3BF17C store goes through s0 not a fresh lui. Logic as retail. */
#ifdef NON_MATCHING
s32 nbCallGetIntPara6(void) {
    s32 prev;
    s32 para;

    if (fldGlobalBase.word4C == 0) {
        prev = fldGlobalBase.hitMatterIndex;
        para = scrGetIntPara(0);
        fldGlobalBase.cutIdxPrev = 0;
        fldGlobalBase.hitMatterIndex = para;
        if (para == 0) {
            if (prev == 0) {
                return 1;
            }
            if (fldGlobalBase.cutIdx == prev) {
                return 1;
            }
        } else {
            if (fldGlobalBase.cutIdx == para) {
                return 1;
            }
        }
        fldGlobalBase.word4C = 1;
        return 0;
    }
    if (fldGlobalBase.word4C == 1) {
        return 0;
    }
    if (fldGlobalBase.word4C != 2) {
        return 0;
    }
    fldGlobalBase.word4C = 0;
    return 1;
}
#else
INCLUDE_ASM(const s32, "code_00179808", nbCallGetIntPara6);
#endif

s32 nbCallGetIntPara7(void) {
    scrSetIntReturnValue(mapObjPointGetSerializeID(scrGetIntPara(0)));
    return 1;
}

s32 nbCallSetIntReturnValue2(void) {
    scrSetIntReturnValue(DDS3_GBWK->mapSummonID);
    return 1;
}

s32 nbCallGetIntPara8(void) {
    if (scrGetIntPara(0) == 0) {
        if (fldGlobalBase.nakamaMode != 0) {
            fldGlobalBase.nakamaMode = 0;
            evtBitOff(2993);
            fldCallGetRotPoseSet();
            mdlSetMotionSpeed(1);
            if (summonDvBasic != NULL) {
                mapSummonDvFoceSet(summonDvBasic->poseset->pos.x, summonDvBasic->poseset->pos.y, summonDvBasic->poseset->pos.z);
            }
            fldAutoMapSeqFlags.stage = 14;
            if (aiboBasic != NULL) {
                dds3ObjectBaseOffFlag(aiboBasic, 1);
            }
        }
    } else {
        if (fldGlobalBase.nakamaMode == 0) {
            fldGlobalBase.word1C0 = fldGlobalBase.amSBlock;
            fldGlobalBase.word1C4 = fldGlobalBase.nameU;
            fldGlobalBase.nakamaMode = 1;
            fldGlobalBase.word16C = DDS3_GBWK->mapSummonID;
            fldGlobalBase.float1B4 = fldGlobalBase.focusX;
            fldGlobalBase.float1B8 = fldGlobalBase.focusY;
            fldGlobalBase.float1BC = fldGlobalBase.focusZ;
            fldGlobalBase.float1C8 = fldGlobalBase.floatC4;
            fldGlobalBase.word170 = 5;
            fldGlobalBase.word174 = 5;
            evtBitOn(2993);
            fldPlayerToNakama(2);
            mdlSetMotionSpeed(5);
            fldAutoMapSeqFlags.stage = 15;
        }
    }
    return 1;
}

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

s32 evtCommand_TEX_SWAP(void) {
    s32 map = scrGetIntPara(0);
    s32 floor = scrGetIntPara(1);
    s32 arg = scrGetIntPara(2);
    s32 open = scrGetIntPara(3);

    mapAM_MaskSetCurrent(map, floor);
    if (open == 1) {
        mapAM_MaskOpen_ID(map, floor, arg);
    } else {
        mapRefDDS3GBWK3(map, floor, arg);
    }
    return 1;
}

s32 evtBCheckPanelMode(void) {
    s32 check = scrGetIntPara(0);
    s32 mode = scrGetIntPara(1);

    if (check == 0) {
        if (mode == 0) {
            mapBCheckPanelModeSub2(0);
        } else {
            mapBCheckPanelModeSub2(1);
        }
    } else {
        if (mode == 0) {
            mapBCheckPanelModeSub(0);
        } else {
            mapBCheckPanelModeSub(1);
        }
    }
    return 1;
}

s32 evtCallGetIntPara3(void) {
    s32 se = scrGetIntPara(0);
    s32 on = scrGetIntPara(1);

    if (se == 0) {
        fldGlobalBase.word314 = 1;
        if (on == 0) {
            mapBgTexLoad(0);
        } else {
            mapBgTexLoad(1);
        }
    } else {
        if (on == 0) {
            fldCallSoundKankyoSeSet(0);
        } else {
            fldCallSoundKankyoSeSet(1);
        }
    }
    return 1;
}
