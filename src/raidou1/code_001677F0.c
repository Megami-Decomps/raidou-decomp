#include "common.h"
#include "field.h"
#include "fldProcess.h"
#include "code_0016A558.h"
#include "code_00195698.h"
#include "mapMenuWin.h"
#include "dds3PadManager.h"
#include "EventBit.h"
#include "sdfSound.h"
#include "fld.h"
#include "dat.h"
#include "sdatUnit.h"
#include "newdata.h"
#include "itf.h"
#include "dds3WorldBasic.h"
#include "dds3SceneBasic.h"

INCLUDE_ASM(const s32, "code_001677F0", mapIfDrawActionMasterSub);

INCLUDE_ASM(const s32, "code_001677F0", mapCallSetOt9);

INCLUDE_ASM(const s32, "code_001677F0", mapCallLinkAlpha);

INCLUDE_ASM(const s32, "code_001677F0", mapCallSetOt10);

INCLUDE_ASM(const s32, "code_001677F0", mapCallSetOt11);

INCLUDE_ASM(const s32, "code_001677F0", mapIfDrawActionNum);

INCLUDE_ASM(const s32, "code_001677F0", mapRefDDS3GBWK6);

INCLUDE_ASM(const s32, "code_001677F0", mapIfDrawActionMaster);

INCLUDE_ASM(const s32, "code_001677F0", mapIgnite);

INCLUDE_ASM(const s32, "code_001677F0", mapCallPreCalcStrWidth);

INCLUDE_ASM(const s32, "code_001677F0", mapCallPreCalcStrWidth2);

s32 mapBadMafuHajiki(void)
{
    s32 data;
    s32 id;
    s32 pos;
    f32 step;
    fldSequenceController_t *controller = &fldAutoMapSeqFlags;
    s32 idx;

    if (mapBadMafuHajikiData2 == 4) {
        if (fldEncountTestFlag(0x40) == 0) {
            return 0;
        }
        if (dds3PadButtonState[0x2A] < 0 && fldGlobalBase.noInpPlCnt == 0 && fldGlobalBase.amSBlock != 16) {
            fldReleaseUnitPad();
            p2sdPlaySE(0, 127, 63);
            mapMenuGetSelectUNIQ();
            mapProcShutDownSub2(-1);
            mapBadMafuHajikiData2 = 3;
            fldGlobalBase.word2E4 = 1;
        }
        return 0;
    } else if (mapBadMafuHajikiData2 == 3) {
        if (nmeCalc()) {
            data = mapCallGetProcessData();
            switch (data) {
            case -1:
                mapBadMafuHajikiData2 = 4;
                mapProcShutDownSub2(1);
                fldGlobalBase.word2E4 = 0;
                break;
            case 0x20000:
                evtBitOff(2993);
                mapBadMafuHajikiData2 = 4;
                fldGlobalBase.word178 = 0;
                mapProcShutDownSub2(1);
                fldGlobalBase.word2E4 = 0;
                return 3;
            case 0x30000:
                if (DDS3_GBWK->mapSummonID > 0) {
                    mapBadMafuHajikiData2 = 4;
                    mapProcShutDownSub2(1);
                    fldGlobalBase.word2E4 = 0;
                    fldGlobalBase.word16C = DDS3_GBWK->mapSummonID;
                    fldGlobalBase.word170 = 5;
                    fldGlobalBase.word174 = 5;
                    return 5;
                }
                break;
            case 0x40000:
                if (evtBitCheck(2274) || evtBitCheck(2275) || evtBitCheck(2291) || evtBitCheck(2292)) {
                    id = datCallGetStockDevilID(DDS3_GBWK->mapSummonID);
                    if (id != -1 && sdatCalc_GetBadStatus(&DDS3_GBWK->playerData.stock[id]) == 11) {
                        fldReleaseUnitPad();
                        fldPlayerMotReset();
                        dds3SceneBasicStartScript(dds3GetActiveScene(), "bad_mafu_hajiki_label");
                        fldPlayerPosRot();
                        mapBadMafuHajikiData2 = 4;
                        mapProcShutDownSub2(1);
                        fldGlobalBase.word2E4 = 0;
                        return 6;
                    }
                }
                if (evtBitCheck(2274)) {
                    fldReleaseUnitPad();
                    controller->stage = 43;
                    mapCallNakamaWakeUp();
                } else if (evtBitCheck(2275)) {
                    fldReleaseUnitPad();
                    controller->stage = 42;
                    mapCallNakamaWakeUp3();
                } else if (evtBitCheck(2291)) {
                    fldReleaseUnitPad();
                    controller->stage = 44;
                    mapNakamaZeniStart();
                } else if (evtBitCheck(2292)) {
                    fldReleaseUnitPad();
                    controller->stage = 45;
                    mapNakamaHiroStart();
                }
                mapBadMafuHajikiData2 = 4;
                mapProcShutDownSub2(1);
                fldGlobalBase.word2E4 = 0;
                break;
            case 0x50000:
                if ((fldGlobalBase.amSBlock == 21 && fldGlobalBase.nameU == 0)
                    || (fldGlobalBase.amSBlock == 22 && fldGlobalBase.nameU == 0)
                    || (fldGlobalBase.amSBlock == 22 && fldGlobalBase.nameU == 1)
                    || (fldGlobalBase.amSBlock == 22 && fldGlobalBase.nameU == 2)
                    || (fldGlobalBase.amSBlock == 23 && fldGlobalBase.nameU == 1)
                    || (fldGlobalBase.amSBlock == 24 && fldGlobalBase.nameU == 0)
                    || (fldGlobalBase.amSBlock == 24 && fldGlobalBase.nameU == 1)
                    || (fldGlobalBase.amSBlock == 25 && fldGlobalBase.nameU == 0)
                    || (fldGlobalBase.amSBlock == 25 && fldGlobalBase.nameU == 1)
                    || (fldGlobalBase.amSBlock == 25 && fldGlobalBase.nameU == 2)
                    || (fldGlobalBase.amSBlock == 25 && fldGlobalBase.nameU == 3)
                    || (fldGlobalBase.amSBlock == 27 && fldGlobalBase.nameU == 0)
                    || (fldGlobalBase.amSBlock == 28 && fldGlobalBase.nameU == 0)
                    || (fldGlobalBase.amSBlock == 8 && fldGlobalBase.nameU == 2)
                    || (fldGlobalBase.amSBlock == 9 && fldGlobalBase.nameU == 1)
                    || (fldGlobalBase.amSBlock == 10 && fldGlobalBase.nameU == 0)
                    || (fldGlobalBase.amSBlock == 10 && fldGlobalBase.nameU == 1)
                    || (fldGlobalBase.amSBlock == 10 && fldGlobalBase.nameU == 2)
                    || (fldGlobalBase.amSBlock == 10 && fldGlobalBase.nameU == 3)
                    || (fldGlobalBase.amSBlock == 10 && fldGlobalBase.nameU == 5)
                    || fldGlobalBase.amSBlock == 13
                    || fldGlobalBase.amSBlock == 14
                    || fldGlobalBase.amSBlock == 17
                    || (fldGlobalBase.amSBlock == 31 && fldGlobalBase.nameU == 0)
                    || (fldGlobalBase.amSBlock == 31 && fldGlobalBase.nameU == 1)
                    || (fldGlobalBase.amSBlock == 31 && fldGlobalBase.nameU == 2)
                    || (fldGlobalBase.amSBlock == 31 && fldGlobalBase.nameU == 3)) {
                    mapBadMafuHajikiData2 = 4;
                    mapProcShutDownSub2(1);
                    fldGlobalBase.word2E4 = 0;
                    fldReleaseUnitPad();
                    fldPlayerMotReset();
                    dds3SceneBasicStartScript(dds3GetActiveScene(), "jimusyo_hajiki1_label");
                    return 4;
                }
                if ((evtBitCheck(1481) && !evtBitCheck(218)) || (evtBitCheck(55) && !evtBitCheck(57))
                    || (evtBitCheck(90) && !evtBitCheck(1152)) || (evtBitCheck(133) && !evtBitCheck(134))
                    || (evtBitCheck(146) && !evtBitCheck(147)) || (evtBitCheck(175) && !evtBitCheck(176))) {
                    mapBadMafuHajikiData2 = 4;
                    mapProcShutDownSub2(1);
                    fldGlobalBase.word2E4 = 0;
                    fldReleaseUnitPad();
                    fldPlayerMotReset();
                    dds3SceneBasicStartScript(dds3GetActiveScene(), "jimusyo_hajiki2_label");
                    return 4;
                }
                controller->stage = 25;
                fldGlobalBase.word80 = fldGlobalBase.amSBlock;
                fldGlobalBase.resourceFlag = fldGlobalBase.nameU;
                break;
            default:
                fldGlobalBase.word178 = data;
                mapBadMafuHajikiData2 = 4;
                mapProcShutDownSub2(1);
                fldGlobalBase.word2E4 = 0;
                return 2;
            }
        }
    } else if (mapBadMafuHajikiData2 == 1) {
        /* mapWork11[1] written directly folds the base into each access: 238 words differ. */
        idx = 1;
        switch (mapWork11[idx].state) {
        case 1:
            break;
        case 2:
            pos = mapWork11[idx].scroll;
            if (pos > 1.0f) {
                step = pos * 0.125f;
                if (step > 48.0f) {
                    step = 48.0f;
                }
                if (step < 4.0f) {
                    step = 4.0f;
                }
                pos -= step;
                if (pos < 0.0f) {
                    pos = 0;
                }
            } else {
                mapWork11[idx].state = 0;
                pos = 0;
            }
            mapWork11[idx].scroll = pos;
            break;
        case 3:
            pos = mapWork11[idx].scroll;
            if (pos < 160.0f) {
                step = (160 - pos) * 0.125f;
                if (step > 48.0f) {
                    step = 48.0f;
                }
                if (step < 4.0f) {
                    step = 4.0f;
                }
                pos += step;
            } else {
                mapWork11[idx].state = 1;
                pos = 160;
                mapBadMafuHajikiData2 = 4;
            }
            mapWork11[idx].scroll = pos;
            break;
        case 0:
            mapWork11[idx].phase += 0.1f;
            if (dds3PadButtonState[0x26] & 2) {
                p2sdPlaySE(3, 127, 63);
                if (--mapWork11[idx].sel < 0) {
                    mapWork11[idx].sel = 0;
                    mapWork11[idx].devilBase = (mapWork11[idx].devilBase - 1 < 0) ? 0 : mapWork11[idx].devilBase - 1;
                }
            } else if (dds3PadButtonState[0x27] & 2) {
                p2sdPlaySE(3, 127, 63);
                if (++mapWork11[idx].sel >= 7) {
                    if (++mapWork11[idx].devilBase >= 330) {
                        mapWork11[idx].devilBase = 329;
                    }
                    mapWork11[idx].sel = 6;
                }
            } else if (dds3PadButtonState[0x24] & 2) {
                p2sdPlaySE(3, 127, 63);
                mapWork11[idx].devilBase -= 7;
                if (mapWork11[idx].devilBase < 0) {
                    mapWork11[idx].devilBase = 0;
                }
            } else if (dds3PadButtonState[0x25] & 2) {
                p2sdPlaySE(3, 127, 63);
                mapWork11[idx].devilBase += 7;
                if (mapWork11[idx].devilBase >= 330) {
                    mapWork11[idx].devilBase = 329;
                }
            }
            mapWork11[idx].sel += mapWork11[idx].rows;
            mapWork11[idx].sel %= mapWork11[idx].rows;
            if (dds3PadButtonState[0x23] < 0) {
                if (mapWork11[idx].devilBase + mapWork11[idx].sel != 0) {
                    mapWork11[idx].state = 3;
                    p2sdPlaySE(0, 127, 63);
                    fldGlobalBase.word178 = mapWork11[idx].devilBase + mapWork11[idx].sel;
                    return 2;
                }
            } else if (dds3PadButtonState[0x29] < 0) {
                mapWork11[idx].state = 3;
            } else if (dds3PadButtonState[0x21] < 0) {
                mapWork11[idx].state = 3;
            }
            break;
        }
    }
    return 1;
}
