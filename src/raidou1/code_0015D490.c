#include "common.h"
#include "include_asm.h"
#include "code_0015D490.h"
#include "code_r1_0010DF68.h"
#include "code_r1_002C8188.h"
#include "dds3KernelCore.h"
#include "dds3ObjectBase.h"
#include "dds3Randomize.h"
#include "EventBit.h"
#include "fld.h"
#include "fldProcess.h"
#include "mapData.h"
#include "model.h"
#include "motion.h"
#include "sdfSound.h"
#include "code_r1_002CA3F0.h"
#include "dds3KernelDraw.h"
#include "sdfMath.h"
#include "code_r1_001CC1D0.h"
#include "code_r1_002CAFD8.h"
#include "frFont.h"
#include "dat.h"
#include "effManager.h"
#include "field.h"
#include "mdlManager.h"
#include "newdata.h"
#include "ps2asm.h"
#include "sdfModel.h"

INCLUDE_ASM(const s32, "code_0015D490", mapLeaf);

void mapCallMemset2(void) {
    sdfVector4_t pos = { 0.0f, 0.0f, 0.0f, 1.0f };
    mdlObj_t *playerMdl;
    mdlObj_t *summonMdl;
    dds3Basic_t *player;
    dds3Basic_t *summon;
    s32 bad;
    s32 id;

    if (mapData121 != 0) {
        return;
    }
    if (fldGlobalBase.nakamaMode == 0) {
        playerMdl = playerModel;
        summonMdl = summonDvModel;
        player = playerBasic;
    } else {
        playerMdl = raidoModel;
        summonMdl = summonDvModel;
        player = raidoBasic;
    }
    summon = summonDvBasic;
    if (eMapBadEff_R != NULL && mapData118 == 0) {
        bad = sdatCalc_GetBadStatus(&DDS3_GBWK->playerData.unit);
        if ((bad == 6 || bad == 11) && !dds3ObjectBaseCheckFlag(player, 1)) {
            sdfModelGetMarkPos_sdfModel(playerMdl->inner, 2);
            SDF_VU0_STOREVECTOR(&pos);
            dds3EffectSetPosition(eMapBadEff_R, &pos);
            dds3EffectDraw_dds3Effect(eMapBadEff_R);
        }
    }
    if (eMapBadEff_N != NULL && mapData120 == 0) {
        id = datCallGetStockDevilID(DDS3_GBWK->mapSummonID);
        if (id != -1) {
            bad = sdatCalc_GetBadStatus(&DDS3_GBWK->playerData.stock[id]);
            if ((bad == 6 || bad == 11) && !dds3ObjectBaseCheckFlag(summon, 1) && (mdlGetColor(summonMdl) & 0xFF000000)) {
                sdfModelGetMarkPos_sdfModel(summonMdl->inner, 2);
                SDF_VU0_STOREVECTOR(&pos);
                dds3EffectSetPosition(eMapBadEff_N, &pos);
                dds3EffectDraw_dds3Effect(eMapBadEff_N);
            }
        }
    }
}

INCLUDE_ASM(const s32, "code_0015D490", mapEffMskMaruDraw);

INCLUDE_ASM(const s32, "code_0015D490", mapEffMskFlishDraw);

INCLUDE_ASM(const s32, "code_0015D490", mapCallSin2);

INCLUDE_ASM(const s32, "code_0015D490", mapCallSin3);

INCLUDE_ASM(const s32, "code_0015D490", mapCallSin4);

INCLUDE_ASM(const s32, "code_0015D490", mapCallSin5);

INCLUDE_ASM(const s32, "code_0015D490", mapCallSin6);

INCLUDE_ASM(const s32, "code_0015D490", mapCallSin7);

INCLUDE_ASM(const s32, "code_0015D490", mapCallSin8);

INCLUDE_ASM(const s32, "code_0015D490", mapCallSin9);

INCLUDE_ASM(const s32, "code_0015D490", mapCallSin10);

INCLUDE_ASM(const s32, "code_0015D490", fldCallSin4);

/* NON_MATCHING: 84 of 5092 words match.  */
#ifdef NON_MATCHING
void fldCallSetOt(s32 area, s32 floor, s32 cut) {
    s32 i;
    s32 j;
    s32 k;
    u32 color;
    u32 color2;
    struct sdfMat_t *mat;
    f32 u0;
    f32 v0;
    f32 u1;
    f32 v1;
    f32 h;
    f32 x;
    f32 z;

    if (cut <= 0 || cut >= 113) {
        return;
    }
    if ((area == 1 || area == 21) && floor == 0 &&
        ((u32)(cut - 1) < 4 || cut == 23 || cut == 24 || cut == 25)) {
        fldCallSetOtData += 0.03f;
        fldCallSetOtData2 -= 0.0019999999f;
        if (fldCallSetOtData2 < 0.0f) {
            fldCallSetOtData2 += 1.0f;
        }
        fldCallSetOtData3 -= 0.0014f;
        if (fldCallSetOtData3 < 0.0f) {
            fldCallSetOtData3 += 1.0f;
        }
        fldCallSetOtData4 -= 0.00119999995f;
        if (fldCallSetOtData4 < 0.0f) {
            fldCallSetOtData4 += 1.0f;
        }
        fldCallSetOtData5 -= 0.0018f;
        if (fldCallSetOtData5 < 0.0f) {
            fldCallSetOtData5 += 1.0f;
        }
        fldCallSetOtData6 += 0.00099999994f;
        if (fldCallSetOtData6 > 1.0f) {
            fldCallSetOtData6 -= 1.0f;
        }
        fldCallSetOtData7 -= 0.0002f;
        if (fldCallSetOtData7 < 0.0f) {
            fldCallSetOtData7 += 1.0f;
        }
        fldCallSetOtData8 -= 0.0008f;
        if (fldCallSetOtData8 < 0.0f) {
            fldCallSetOtData8 += 1.0f;
        }
        eglSetOt(33);
        eglLinkTest(1, 0, 128, 3, 0, 0, 1, 2);
        eglLinkAlpha(0);
        fldDrwZLoad();
        for (i = 0; i < 4; i++) {
            if (i == 1) {
                mat = fldGlobalBase.texSlot[0].file;
                eglLinkAlpha(1);
                color = 0x80000000;
                eglLinkTest(1, 0, 128, 1, 0, 0, 1, 2);
            } else if (i == 2 || i == 3) {
                color = fldGlobalBase.word318 ? 0x80405060 : 0x80505050;
                mat = fldGlobalBase.texSlot[0].file;
                eglLinkAlpha(13);
                eglLinkTest(1, 0, 128, 3, 0, 0, 1, 2);
            } else {
                color = fldGlobalBase.word318 ? 0x38406070 : 0x38506060;
                mat = fldGlobalBase.texSlot[2].file;
                eglLinkAlpha(0);
                eglLinkTest(1, 0, 128, 3, 0, 0, 1, 2);
            }
            h = 0.39999998f;
            if (i == 1) {
                u0 = fldCallSetOtData7;
                v0 = fldCallSetOtData3;
                u1 = fldCallSetOtData7 + 1.0f;
            } else if (i == 2) {
                u0 = fldCallSetOtData6;
                v0 = fldCallSetOtData2;
                u1 = fldCallSetOtData6 + 1.0f;
            } else if (i == 3) {
                u0 = fldCallSetOtData8;
                v0 = fldCallSetOtData5;
                h = 0.26666666f;
                u1 = fldCallSetOtData8 + 0.5f;
            } else {
                u0 = 0.0f;
                v0 = fldCallSetOtData4;
                u1 = 1.0f;
            }
            v1 = v0 + h;
            if (v0 < 0.0f) {
                v0 += 1.0f;
                v1 += 1.0f;
            }
            if (cut == 1) {
                mapCallSin2(5, color, mat, fldCallSetOtData, 4500.0f, 4900.0f, 60.0f, 7000.0f, u0, v0, u1, v1);
            } else if (cut == 2) {
                mapCallSin2(5, color, mat, fldCallSetOtData, 4500.0f, 4900.0f, 60.0f, 5000.0f, u0, v0, u1, v1);
            } else if (cut == 3) {
                mapCallSin2(7, color, mat, fldCallSetOtData, 4500.0f, 4900.0f, 60.0f, 3500.0f, u0, v0, u1, v1);
            } else if (cut == 4) {
                mapCallSin2(5, color, mat, fldCallSetOtData, 4500.0f, 4900.0f, 60.0f, 2200.0f, u0, v0, u1, v1);
            } else if (cut == 23) {
                mapCallSin3(14, color, mat, fldCallSetOtData, -6200.0f, 200.0f, -600.0f, -1200.0f, u0, v0, u1, v1);
            } else if (cut == 24) {
                mapCallSin3(5, color, mat, fldCallSetOtData, 600.0f, 200.0f, -600.0f, -1000.0f, u0, v0, u1, v1);
            } else if (cut == 25) {
                mapCallSin3(5, color, mat, fldCallSetOtData, 2200.0f, 200.0f, -600.0f, -1000.0f, u0, v0, u1, v1);
            }
        }
        eglLinkAlpha(0);
        eglLinkTest(1, 5, 128, 3, 0, 0, 1, 2);
        if (cut == 4) {
            eglSetOt(23);
            eglLinkTest(1, 1, 128, 3, 0, 0, 1, 2);
            eglLinkAlpha(1);
            eglLinkPoly4(4500.0f, -60.0f, 1910.0f, 5100.0f, -60.0f, 1910.0f, 4500.0f, -60.0f, 1300.0f, 5100.0f, -60.0f,
                         1300.0f, 0x808080, 0x808080, 0x808080, 0x808080);
            eglLinkTest(1, 5, 128, 1, 0, 0, 1, 2);
            eglLinkAlpha(0);
        } else if (cut == 23) {
            eglSetOt(23);
            eglLinkTest(1, 1, 128, 3, 0, 0, 1, 2);
            eglLinkAlpha(1);
            eglLinkPoly4(-1678.0f, 100.0f, -600.0f, -1455.0f, 100.0f, -600.0f, -1678.0f, 100.0f, -810.0f, -1455.0f,
                         100.0f, -810.0f, 0x808080, 0x808080, 0x808080, 0x808080);
            eglLinkTest(1, 5, 128, 1, 0, 0, 1, 2);
            eglLinkAlpha(0);
        }
    }
    if ((area == 3 || area == 23) && floor == 1 && (cut == 6 || cut == 33)) {
        fldCallSetOtData9 += 0.06f;
        fldCallSetOtData10 -= 0.0029999998f;
        if (fldCallSetOtData10 < 0.0f) {
            fldCallSetOtData10 += 1.0f;
        }
        fldCallSetOtData11 -= 0.0028f;
        if (fldCallSetOtData11 < 0.0f) {
            fldCallSetOtData11 += 1.0f;
        }
        fldCallSetOtData12 -= 0.0023999999f;
        if (fldCallSetOtData12 < 0.0f) {
            fldCallSetOtData12 += 1.0f;
        }
        fldCallSetOtData13 -= 0.0026999999f;
        if (fldCallSetOtData13 < 0.0f) {
            fldCallSetOtData13 += 1.0f;
        }
        fldCallSetOtData14 += 0.0014999999f;
        if (fldCallSetOtData14 > 1.0f) {
            fldCallSetOtData14 -= 1.0f;
        }
        fldCallSetOtData15 -= 0.00029999999f;
        if (fldCallSetOtData15 < 0.0f) {
            fldCallSetOtData15 += 1.0f;
        }
        fldCallSetOtData16 -= 0.00119999995f;
        if (fldCallSetOtData16 < 0.0f) {
            fldCallSetOtData16 += 1.0f;
        }
        eglSetOt(33);
        eglLinkTest(1, 0, 128, 3, 0, 0, 1, 2);
        eglLinkAlpha(0);
        fldDrwZLoad();
        for (i = 0; i < 4; i++) {
            if (i == 1) {
                mat = fldGlobalBase.texSlot[0].file;
                eglLinkAlpha(1);
                color = 0x80000000;
                eglLinkTest(1, 0, 128, 1, 0, 0, 1, 2);
            } else if (i == 2 || i == 3) {
                color = fldGlobalBase.word318 ? 0x80406070 : 0x80606060;
                mat = fldGlobalBase.texSlot[0].file;
                eglLinkAlpha(13);
                eglLinkTest(1, 0, 128, 3, 0, 0, 1, 2);
            } else {
                color = fldGlobalBase.word318 ? 0x20505070 : 0x20806060;
                mat = fldGlobalBase.texSlot[2].file;
                eglLinkAlpha(0);
                eglLinkTest(1, 0, 128, 3, 0, 0, 1, 2);
            }
            h = 0.19999999f;
            if (i == 1) {
                u0 = fldCallSetOtData15;
                v0 = fldCallSetOtData11;
                u1 = fldCallSetOtData15 + 1.0f;
            } else if (i == 2) {
                u0 = fldCallSetOtData14;
                v0 = fldCallSetOtData10;
                u1 = fldCallSetOtData14 + 1.0f;
            } else if (i == 3) {
                u0 = fldCallSetOtData16;
                v0 = fldCallSetOtData13;
                h = 0.13333333f;
                u1 = fldCallSetOtData16 + 0.5f;
            } else {
                u0 = 0.0f;
                v0 = fldCallSetOtData12;
                u1 = 1.0f;
            }
            v1 = v0 + h;
            if (v0 < 0.0f) {
                v0 += 1.0f;
                v1 += 1.0f;
            }
            if (cut == 6) {
                mapCallSin4(9, color, mat, fldCallSetOtData9, 1300.0f, 2050.0f, 250.0f, 4000.0f, u0, v0, u1, v1);
                mapCallSin5(9, color, mat, fldCallSetOtData9, 2050.0f, 2800.0f, 250.0f, 4000.0f, u1, v0, u1 + (u1 - u0),
                            v1);
            } else if (cut == 33) {
                mapCallSin6(5, color, mat, fldCallSetOtData, 2050.0f, 3000.0f, 230.0f, -2500.0f, u1, v0, u1 + (u1 - u0),
                            v1);
            }
        }
        eglLinkAlpha(0);
        eglLinkTest(1, 5, 125, 1, 0, 0, 1, 2);
        if (cut == 6) {
            eglSetOt(23);
            eglLinkTest(1, 1, 128, 3, 0, 0, 1, 2);
            eglLinkAlpha(1);
            eglLinkPoly4(1300.0f, 0.0f, 1300.0f, 2800.0f, 0.0f, 1300.0f, 1300.0f, 0.0f, 758.0f, 2800.0f, 0.0f, 758.0f,
                         0x808080, 0x808080, 0x808080, 0x808080);
            eglLinkTest(1, 5, 128, 1, 0, 0, 1, 2);
            eglLinkAlpha(0);
        } else if (cut == 33) {
            eglSetOt(23);
            eglLinkTest(1, 1, 128, 3, 0, 0, 1, 2);
            eglLinkAlpha(1);
            eglLinkPoly4(2587.0f, 150.0f, -3095.0f, 2900.0f, 150.0f, -3095.0f, 2587.0f, 150.0f, -3400.0f, 2900.0f, 150.0f,
                         -3400.0f, 0x808080, 0x808080, 0x808080, 0x808080);
            eglLinkPoly4(2825.0f, 150.0f, -2800.0f, 2825.0f, 150.0f, -2875.0f, 2825.0f, 300.0f, -2800.0f, 2825.0f, 300.0f,
                         -2875.0f, 0x808080, 0x808080, 0x808080, 0x808080);
            eglLinkPoly4(2825.0f, 150.0f, -2875.0f, 2850.0f, 150.0f, -2875.0f, 2825.0f, 300.0f, -2875.0f, 2850.0f, 300.0f,
                         -2875.0f, 0x808080, 0x808080, 0x808080, 0x808080);
            eglLinkPoly4(2825.0f, 150.0f, -3020.0f, 2825.0f, 150.0f, -3100.0f, 2825.0f, 300.0f, -3020.0f, 2825.0f, 300.0f,
                         -3100.0f, 0x808080, 0x808080, 0x808080, 0x808080);
            eglLinkPoly4(2825.0f, 100.0f, -3400.0f, 2825.0f, 100.0f, -3700.0f, 2825.0f, 300.0f, -3400.0f, 2825.0f, 300.0f,
                         -3700.0f, 0x808080, 0x808080, 0x808080, 0x808080);
            eglLinkTest(1, 5, 128, 1, 0, 0, 1, 2);
            eglLinkAlpha(0);
        }
    }
    if ((area == 3 || area == 23) && floor == 1 &&
        (cut == 19 || cut == 22 || cut == 23 || cut == 26 || cut == 33 || cut == 34 || cut == 35 || cut == 36 ||
         cut == 37 || cut == 38)) {
        fldCallSetOtData += 0.03f;
        fldCallSetOtData2 += 0.0019999999f;
        if (fldCallSetOtData2 > 1.0f) {
            fldCallSetOtData2 -= 1.0f;
        }
        fldCallSetOtData3 += 0.0014f;
        if (fldCallSetOtData3 > 1.0f) {
            fldCallSetOtData3 -= 1.0f;
        }
        fldCallSetOtData4 += 0.00119999995f;
        if (fldCallSetOtData4 > 1.0f) {
            fldCallSetOtData4 -= 1.0f;
        }
        fldCallSetOtData5 += 0.0018f;
        if (fldCallSetOtData5 > 1.0f) {
            fldCallSetOtData5 -= 1.0f;
        }
        fldCallSetOtData6 += 0.00099999994f;
        if (fldCallSetOtData6 > 1.0f) {
            fldCallSetOtData6 -= 1.0f;
        }
        fldCallSetOtData7 -= 0.0002f;
        if (fldCallSetOtData7 < 0.0f) {
            fldCallSetOtData7 += 1.0f;
        }
        fldCallSetOtData8 -= 0.0008f;
        if (fldCallSetOtData8 < 0.0f) {
            fldCallSetOtData8 += 1.0f;
        }
        eglSetOt(33);
        eglLinkTest(1, 0, 128, 3, 0, 0, 1, 2);
        eglLinkAlpha(0);
        fldDrwZLoad();
        for (i = 0; i < 4; i++) {
            if (i == 1) {
                mat = fldGlobalBase.texSlot[0].file;
                eglLinkAlpha(1);
                color = 0x80000000;
                eglLinkTest(1, 0, 128, 1, 0, 0, 1, 2);
            } else if (i == 2 || i == 3) {
                color = fldGlobalBase.word318 ? 0x80303040 : 0x80303030;
                mat = fldGlobalBase.texSlot[0].file;
                eglLinkAlpha(13);
                eglLinkTest(1, 0, 128, 3, 0, 0, 1, 2);
            } else {
                color = fldGlobalBase.word318 ? 0x28707080 : 0x28808080;
                mat = fldGlobalBase.texSlot[2].file;
                eglLinkAlpha(0);
                eglLinkTest(1, 0, 128, 3, 0, 0, 1, 2);
            }
            h = 0.19999999f;
            if (i == 1) {
                u0 = fldCallSetOtData7;
                v0 = fldCallSetOtData3;
                u1 = fldCallSetOtData7 + 1.0f;
            } else if (i == 2) {
                u0 = fldCallSetOtData6;
                v0 = fldCallSetOtData2;
                u1 = fldCallSetOtData6 + 1.0f;
            } else if (i == 3) {
                u0 = fldCallSetOtData8;
                v0 = fldCallSetOtData5;
                h = 0.13333333f;
                u1 = fldCallSetOtData8 + 0.5f;
            } else {
                u0 = 0.0f;
                v0 = fldCallSetOtData4;
                u1 = 1.0f;
            }
            v1 = v0 + h;
            if (v0 < 0.0f) {
                v0 += 1.0f;
                v1 += 1.0f;
            }
            if (i != 0) {
                if (cut == 19) {
                    mapCallSin8(15, color, mat, fldCallSetOtData, 3700.0f, 250.0f, -2600.0f, -3050.0f, u0, v0, u1, v1);
                } else if (cut == 22) {
                    mapCallSin8(10, color, mat, fldCallSetOtData, 5200.0f, 250.0f, -2600.0f, -3050.0f, u0, v0, u1, v1);
                } else if (cut == 23) {
                    mapCallSin8(10, color, mat, fldCallSetOtData, 6500.0f, 250.0f, -2600.0f, -3050.0f, u0, v0, u1, v1);
                } else if (cut == 26) {
                    mapCallSin8(5, color, mat, fldCallSetOtData, 8300.0f, 250.0f, -2600.0f, -3050.0f, u0, v0, u1, v1);
                } else if (cut == 33) {
                    mapCallSin8(12, color, mat, fldCallSetOtData, 4100.0f, 250.0f, -2600.0f, -3050.0f, u0, v0, u1, v1);
                } else if (cut == 34) {
                    mapCallSin8(15, color, mat, fldCallSetOtData, 3500.0f, 250.0f, -2600.0f, -3050.0f, u0, v0, u1, v1);
                } else if (cut == 35) {
                    mapCallSin8(21, color, mat, fldCallSetOtData, 3500.0f, 250.0f, -2600.0f, -3090.0f, u0, v0, u1, v1);
                } else if (cut == 36) {
                    mapCallSin8(20, color, mat, fldCallSetOtData, 5200.0f, 250.0f, -2600.0f, -3090.0f, u0, v0, u1, v1);
                } else if (cut == 37) {
                    mapCallSin8(15, color, mat, fldCallSetOtData, 6600.0f, 250.0f, -2600.0f, -3090.0f, u0, v0, u1, v1);
                } else if (cut == 38) {
                    mapCallSin8(10, color, mat, fldCallSetOtData, 8000.0f, 250.0f, -2600.0f, -3090.0f, u0, v0, u1, v1);
                }
            } else if (cut == 19) {
                mapCallSin7(15, color, mat, fldCallSetOtData, 3700.0f, 250.0f, -2600.0f, -3050.0f, u0, v0, u1, v1);
            } else if (cut == 22) {
                mapCallSin7(10, color, mat, fldCallSetOtData, 5200.0f, 250.0f, -2600.0f, -3050.0f, u0, v0, u1, v1);
            } else if (cut == 23) {
                mapCallSin7(10, color, mat, fldCallSetOtData, 6500.0f, 250.0f, -2600.0f, -3050.0f, u0, v0, u1, v1);
            } else if (cut == 26) {
                mapCallSin7(5, color, mat, fldCallSetOtData, 8300.0f, 250.0f, -2600.0f, -3050.0f, u0, v0, u1, v1);
            } else if (cut == 33) {
                mapCallSin7(12, color, mat, fldCallSetOtData, 4100.0f, 250.0f, -2600.0f, -3050.0f, u0, v0, u1, v1);
            } else if (cut == 34) {
                mapCallSin7(15, color, mat, fldCallSetOtData, 3500.0f, 250.0f, -2600.0f, -3050.0f, u0, v0, u1, v1);
            } else if (cut == 35) {
                mapCallSin7(21, color, mat, fldCallSetOtData, 3500.0f, 250.0f, -2600.0f, -3090.0f, u0, v0, u1, v1);
            } else if (cut == 36) {
                mapCallSin7(20, color, mat, fldCallSetOtData, 5200.0f, 250.0f, -2600.0f, -3090.0f, u0, v0, u1, v1);
            } else if (cut == 37) {
                mapCallSin7(15, color, mat, fldCallSetOtData, 6600.0f, 250.0f, -2600.0f, -3090.0f, u0, v0, u1, v1);
            } else if (cut == 38) {
                mapCallSin7(10, color, mat, fldCallSetOtData, 8000.0f, 250.0f, -2600.0f, -3090.0f, u0, v0, u1, v1);
            }
        }
        eglLinkAlpha(0);
        eglLinkTest(1, 5, 125, 1, 0, 0, 1, 2);
        if (cut == 34) {
            eglSetOt(23);
            eglLinkTest(1, 1, 128, 3, 0, 0, 1, 2);
            eglLinkAlpha(1);
            eglLinkPoly4(3300.0f, 0.0f, -3100.0f, 6000.0f, 0.0f, -3100.0f, 3300.0f, 0.0f, -3400.0f, 6000.0f, 0.0f, -3400.0f,
                         0x808080, 0x808080, 0x808080, 0x808080);
            eglLinkTest(1, 5, 128, 1, 0, 0, 1, 2);
            eglLinkAlpha(0);
        } else if (cut == 35) {
            eglSetOt(23);
            eglLinkTest(1, 1, 128, 3, 0, 0, 1, 2);
            eglLinkAlpha(1);
            eglLinkPoly4(3300.0f, 0.0f, -3100.0f, 5000.0f, 0.0f, -3100.0f, 3300.0f, 0.0f, -3400.0f, 5000.0f, 0.0f, -3400.0f,
                         0x808080, 0x808080, 0x808080, 0x808080);
            eglLinkTest(1, 5, 128, 1, 0, 0, 1, 2);
            eglLinkAlpha(0);
            eglSetOt(38);
            eglLinkTest(1, 1, 128, 3, 0, 0, 1, 2);
            eglLinkAlpha(1);
            eglLinkPoly4(3300.0f, 225.0f, -2600.0f, 5000.0f, 225.0f, -2600.0f, 3300.0f, 225.0f, -3100.0f, 5000.0f, 225.0f,
                         -3100.0f, 0x808080, 0x808080, 0x808080, 0x808080);
            eglLinkPoly4(5000.0f, 225.0f, -2600.0f, 7000.0f, 225.0f, -2600.0f, 5000.0f, 225.0f, -3100.0f, 7000.0f, 225.0f,
                         -3100.0f, 0x808080, 0x808080, 0x808080, 0x808080);
            eglLinkPoly4(7000.0f, 225.0f, -2600.0f, 9500.0f, 225.0f, -2600.0f, 7000.0f, 225.0f, -3100.0f, 9500.0f, 225.0f,
                         -3100.0f, 0x808080, 0x808080, 0x808080, 0x808080);
            eglLinkTest(1, 5, 128, 1, 0, 0, 1, 2);
            eglLinkAlpha(0);
        }
    }
    if ((area == 4 || area == 24) && (floor == 0 || floor == 4 || floor == 5) && cut == 21) {
        eglSetOt(33);
        eglLinkTest(1, 0, 128, 3, 0, 0, 1, 2);
        eglLinkAlpha(0);
        fldDrwZLoad();
        eglLinkAlpha(1);
        eglLinkTest(1, 0, 128, 3, 0, 0, 1, 2);
        fldCallSin5();
        fldCallSetOtData4 += 0.0014f;
        if (fldCallSetOtData4 > 1.0f) {
            fldCallSetOtData4 -= 1.0f;
        }
        mat = fldGlobalBase.texSlot[2].file;
        eglLinkAlpha(0);
        color = fldGlobalBase.word318 ? 0x40606080 : 0x40808080;
        u0 = fldCallSetOtData4;
        if (u0 > 1.0f) {
            u0 -= 1.0f;
        }
        for (j = 0; j < 14; j++) {
            v0 = fldCallSetOtData4;
            if (v0 > 1.0f) {
                v0 -= 1.0f;
            }
            x = j * 50.0f + 830.0f;
            for (k = 0; k < 20; k++) {
                z = -3600.0f - k * 50.0f;
                v1 = v0 + 0.099999995f;
                dds3CallGrAllocPacketBuff8(x, fldCallSetOtWork[k + 1][j + 1] * -100.0f + 15.0f, z, x + 50.0f,
                                           fldCallSetOtWork[k + 1][j + 2] * -100.0f + 15.0f, z, x,
                                           fldCallSetOtWork[k + 2][j + 1] * -100.0f + 15.0f, z - 50.0f, x + 50.0f,
                                           fldCallSetOtWork[k + 2][j + 2] * -100.0f + 15.0f, z - 50.0f,
                                           color, color, color, color, mat, u0, v0, u0 + 0.099999995f, v1);
                v0 = v1;
            }
            u0 += 0.099999995f;
        }
        fldCallSetOtData3 += 0.00099999994f;
        if (fldCallSetOtData3 > 1.0f) {
            fldCallSetOtData3 -= 1.0f;
        }
        mat = fldGlobalBase.texSlot[0].file;
        eglLinkAlpha(1);
        eglLinkTest(1, 0, 128, 1, 0, 0, 1, 2);
        u0 = fldCallSetOtData3;
        if (u0 > 1.0f) {
            u0 -= 1.0f;
        }
        for (j = 0; j < 14; j++) {
            v0 = 0.0f;
            for (k = 0; k < 20; k++) {
                u1 = u0 + 0.099999995f;
                v1 = v0 + 0.099999995f;
                if (u0 > 1.0f) {
                    u0 -= 1.0f;
                    u1 -= 1.0f;
                }
                if (u0 < 0.0f) {
                    u0 += 1.0f;
                    u1 += 1.0f;
                }
                x = j * 50.0f + 830.0f;
                z = -3600.0f - k * 50.0f;
                dds3CallGrAllocPacketBuff8(x, fldCallSetOtWork[k + 1][j + 1] * -100.0f + 15.0f, z, x + 50.0f,
                                           fldCallSetOtWork[k + 1][j + 2] * -100.0f + 15.0f, z, x,
                                           fldCallSetOtWork[k + 2][j + 1] * -100.0f + 15.0f, z - 50.0f, x + 50.0f,
                                           fldCallSetOtWork[k + 2][j + 2] * -100.0f + 15.0f, z - 50.0f,
                                           0x80000000, 0x80000000, 0x80000000, 0x80000000, mat, u0, v0, u1, v1);
                v0 = v1;
            }
            u0 += 0.099999995f;
        }
        fldCallSetOtData2 += 0.00049999997f;
        if (fldCallSetOtData2 > 1.0f) {
            fldCallSetOtData2 -= 1.0f;
        }
        fldCallSetOtData6 += 0.0012999999f;
        if (fldCallSetOtData6 > 1.0f) {
            fldCallSetOtData6 -= 1.0f;
        }
        color = fldGlobalBase.word318 ? 0x80485048 : 0x80505040;
        mat = fldGlobalBase.texSlot[0].file;
        eglLinkAlpha(13);
        eglLinkTest(1, 0, 128, 3, 0, 0, 1, 2);
        u0 = fldCallSetOtData6;
        if (u0 > 1.0f) {
            u0 -= 1.0f;
        }
        for (j = 0; j < 14; j++) {
            v0 = fldCallSetOtData2;
            if (v0 > 1.0f) {
                v0 -= 1.0f;
            }
            for (k = 0; k < 20; k++) {
                v1 = v0 + 0.049999998f;
                u1 = u0 + 0.049999998f;
                if (u0 > 1.0f) {
                    u0 -= 1.0f;
                    u1 -= 1.0f;
                }
                if (u0 < 0.0f) {
                    u0 += 1.0f;
                    u1 += 1.0f;
                }
                if (v0 > 1.0f) {
                    v0 -= 1.0f;
                    v1 -= 1.0f;
                }
                if (v0 < 0.0f) {
                    v0 += 1.0f;
                    v1 += 1.0f;
                }
                x = j * 50.0f + 830.0f;
                z = -3600.0f - k * 50.0f;
                dds3CallGrAllocPacketBuff8(x, fldCallSetOtWork[k + 1][j + 1] * -100.0f + 15.0f, z, x + 50.0f,
                                           fldCallSetOtWork[k + 1][j + 2] * -100.0f + 15.0f, z, x,
                                           fldCallSetOtWork[k + 2][j + 1] * -100.0f + 15.0f, z - 50.0f, x + 50.0f,
                                           fldCallSetOtWork[k + 2][j + 2] * -100.0f + 15.0f, z - 50.0f,
                                           color, color, color, color, mat, u0, v0, u1, v1);
                v0 += 0.049999998f;
            }
            u0 += 0.049999998f;
        }
        fldCallSetOtData5 += 0.0014f;
        if (fldCallSetOtData2 > 1.0f) {
            fldCallSetOtData2 -= 1.0f;
        }
        color = fldGlobalBase.word318 ? 0x80485058 : 0x80505050;
        mat = fldGlobalBase.texSlot[0].file;
        eglLinkAlpha(13);
        eglLinkTest(1, 0, 128, 3, 0, 0, 1, 2);
        u0 = fldCallSetOtData5 + 1.0f;
        for (j = 0; j < 14; j++) {
            v0 = 1.0f;
            for (k = 0; k < 20; k++) {
                u1 = u0 - 0.099999995f;
                v1 = v0 - 0.099999995f;
                if (u0 > 1.0f) {
                    u0 -= 1.0f;
                    u1 -= 1.0f;
                }
                if (u0 < 0.0f) {
                    u0 += 1.0f;
                    u1 += 1.0f;
                }
                x = j * 50.0f + 830.0f;
                z = -3600.0f - k * 50.0f;
                dds3CallGrAllocPacketBuff8(x, fldCallSetOtWork[k + 1][j + 1] * -100.0f + 15.0f, z, x + 50.0f,
                                           fldCallSetOtWork[k + 1][j + 2] * -100.0f + 15.0f, z, x,
                                           fldCallSetOtWork[k + 2][j + 1] * -100.0f + 15.0f, z - 50.0f, x + 50.0f,
                                           fldCallSetOtWork[k + 2][j + 2] * -100.0f + 15.0f, z - 50.0f,
                                           color, color, color, color, mat, u0, v0, u1, v1);
                v0 = v1;
            }
            u0 -= 0.099999995f;
        }
        if (cut == 21) {
            eglSetOt(23);
            eglLinkTest(1, 1, 128, 3, 0, 0, 1, 2);
            eglLinkAlpha(1);
            eglLinkPoly4(900.0f, -500.0f, -3700.0f, 1100.0f, -500.0f, -3700.0f, 900.0f, 200.0f, -3700.0f, 1100.0f, 200.0f,
                         -3700.0f, 0x808080, 0x808080, 0x808080, 0x808080);
            eglLinkPoly4(1100.0f, -500.0f, -3700.0f, 1100.0f, -500.0f, -3600.0f, 1100.0f, 200.0f, -3700.0f, 1100.0f,
                         200.0f, -3600.0f, 0x808080, 0x808080, 0x808080, 0x808080);
            eglLinkPoly4(900.0f, -50.0f, -4500.0f, 900.0f, -50.0f, -3700.0f, 900.0f, -20.0f, -4500.0f, 900.0f, -20.0f,
                         -3700.0f, 0x808080, 0x808080, 0x808080, 0x808080);
            eglLinkTest(1, 5, 128, 1, 0, 0, 1, 2);
            eglLinkAlpha(0);
            eglSetOt(33);
            eglLinkTest(1, 0, 128, 1, 0, 0, 1, 2);
            eglLinkAlpha(0);
            eglLinkPoly4(900.0f, -20.0f, -4500.0f, 900.0f, -20.0f, -3700.0f, 900.0f, 0.0f, -4500.0f, 900.0f, 0.0f, -3700.0f,
                         0x40000000, 0x40000000, 0x10000000, 0x10000000);
        }
    }
    if ((area == 4 || area == 24) && (floor == 0 || floor == 4 || floor == 5) && cut == 5) {
        eglSetOt(33);
        eglLinkTest(1, 0, 128, 3, 0, 0, 1, 2);
        eglLinkAlpha(0);
        fldDrwZLoad();
        eglLinkAlpha(1);
        eglLinkTest(1, 0, 128, 3, 0, 0, 1, 2);
        fldCallSin6();
        fldCallSetOtData4 += 0.0014f;
        if (fldCallSetOtData4 > 1.0f) {
            fldCallSetOtData4 -= 1.0f;
        }
        mat = fldGlobalBase.texSlot[2].file;
        eglLinkAlpha(0);
        u0 = fldCallSetOtData4;
        if (u0 > 1.0f) {
            u0 -= 1.0f;
        }
        for (j = 0; j < 8; j++) {
            v0 = fldCallSetOtData4;
            if (v0 > 1.0f) {
                v0 -= 1.0f;
            }
            color = fldGlobalBase.word318 ? 0x40283028 : 0x40303020;
            for (k = 0; k < 31; k++) {
                x = j * 100.0f + 3100.0f;
                z = 4500.0f - k * 50.0f;
                v1 = v0 + 0.099999995f;
                dds3CallGrAllocPacketBuff8(x, fldCallSetOtWork[k + 1][j + 2] * -70.0f + 50.0f, z, x + 100.0f,
                                           fldCallSetOtWork[k + 1][j + 3] * -70.0f + 50.0f, z, x,
                                           fldCallSetOtWork[k + 2][j + 2] * -70.0f + 50.0f, z - 50.0f, x + 100.0f,
                                           fldCallSetOtWork[k + 2][j + 3] * -70.0f + 50.0f, z - 50.0f,
                                           color, color, color, color, mat, u0, v0, u0 + 0.099999995f, v1);
                v0 = v1;
            }
            u0 += 0.099999995f;
        }
        u0 = fldCallSetOtData4;
        if (u0 > 1.0f) {
            u0 -= 1.0f;
        }
        for (j = 0; j < 3; j++) {
            v0 = fldCallSetOtData4;
            if (v0 > 1.0f) {
                v0 -= 1.0f;
            }
            color = fldGlobalBase.word318 ? 0x30283028 : 0x30303020;
            for (k = 0; k < 3; k++) {
                x = j * 500.0f + 2600.0f;
                z = 6000.0f - k * 500.0f;
                v1 = v0 + 0.19999999f;
                dds3CallGrAllocPacketBuff8(x, 50.0f, z, x + 500.0f, 50.0f, z, x, 50.0f, z - 500.0f, x + 500.0f, 50.0f,
                                           z - 500.0f,
                                           color, color, color, color, mat, u0, v0, u0 + 0.19999999f, v1);
                v0 = v1;
            }
            u0 += 0.19999999f;
        }
        fldCallSetOtData3 += 0.00099999994f;
        if (fldCallSetOtData3 > 1.0f) {
            fldCallSetOtData3 -= 1.0f;
        }
        mat = fldGlobalBase.texSlot[0].file;
        eglLinkAlpha(1);
        eglLinkTest(1, 0, 128, 1, 0, 0, 1, 2);
        u0 = fldCallSetOtData3;
        if (u0 > 1.0f) {
            u0 -= 1.0f;
        }
        for (j = 0; j < 8; j++) {
            v0 = 0.0f;
            for (k = 0; k < 31; k++) {
                u1 = u0 + 0.099999995f;
                v1 = v0 + 0.099999995f;
                if (u0 > 1.0f) {
                    u0 -= 1.0f;
                    u1 -= 1.0f;
                }
                if (u0 < 0.0f) {
                    u0 += 1.0f;
                    u1 += 1.0f;
                }
                x = j * 100.0f + 3100.0f;
                z = 4500.0f - k * 50.0f;
                dds3CallGrAllocPacketBuff8(x, fldCallSetOtWork[k + 1][j + 2] * -70.0f + 50.0f, z, x + 100.0f,
                                           fldCallSetOtWork[k + 1][j + 3] * -70.0f + 50.0f, z, x,
                                           fldCallSetOtWork[k + 2][j + 2] * -70.0f + 50.0f, z - 50.0f, x + 100.0f,
                                           fldCallSetOtWork[k + 2][j + 3] * -70.0f + 50.0f, z - 50.0f,
                                           0x80000000, 0x80000000, 0x80000000, 0x80000000, mat, u0, v0, u1, v1);
                v0 = v1;
            }
            u0 += 0.099999995f;
        }
        u0 = fldCallSetOtData3;
        if (u0 > 1.0f) {
            u0 -= 1.0f;
        }
        for (j = 0; j < 3; j++) {
            v0 = 0.0f;
            for (k = 0; k < 3; k++) {
                u1 = u0 + 0.29999999f;
                v1 = v0 + 0.29999999f;
                if (u0 > 1.0f) {
                    u0 -= 1.0f;
                    u1 -= 1.0f;
                }
                if (u0 < 0.0f) {
                    u0 += 1.0f;
                    u1 += 1.0f;
                }
                x = j * 500.0f + 2600.0f;
                z = 6000.0f - k * 500.0f;
                dds3CallGrAllocPacketBuff8(x, 50.0f, z, x + 500.0f, 50.0f, z, x, 50.0f, z - 500.0f, x + 500.0f, 50.0f,
                                           z - 500.0f,
                                           0x60000000, 0x60000000, 0x60000000, 0x60000000, mat, u0, v0, u1, v1);
                v0 = v1;
            }
            u0 += 0.29999999f;
        }
        fldCallSetOtData2 += 0.00049999997f;
        if (fldCallSetOtData2 > 1.0f) {
            fldCallSetOtData2 -= 1.0f;
        }
        fldCallSetOtData6 += 0.0012999999f;
        if (fldCallSetOtData6 > 1.0f) {
            fldCallSetOtData6 -= 1.0f;
        }
        color = fldGlobalBase.word318 ? 0x80484048 : 0x80504040;
        mat = fldGlobalBase.texSlot[0].file;
        eglLinkAlpha(13);
        eglLinkTest(1, 0, 128, 3, 0, 0, 1, 2);
        u0 = fldCallSetOtData6;
        if (u0 > 1.0f) {
            u0 -= 1.0f;
        }
        for (j = 0; j < 8; j++) {
            v0 = fldCallSetOtData2;
            if (v0 > 1.0f) {
                v0 -= 1.0f;
            }
            for (k = 0; k < 31; k++) {
                v1 = v0 + 0.049999998f;
                u1 = u0 + 0.049999998f;
                if (u0 > 1.0f) {
                    u0 -= 1.0f;
                    u1 -= 1.0f;
                }
                if (u0 < 0.0f) {
                    u0 += 1.0f;
                    u1 += 1.0f;
                }
                if (v0 > 1.0f) {
                    v0 -= 1.0f;
                    v1 -= 1.0f;
                }
                if (v0 < 0.0f) {
                    v0 += 1.0f;
                    v1 += 1.0f;
                }
                x = j * 100.0f + 3100.0f;
                z = 4500.0f - k * 50.0f;
                dds3CallGrAllocPacketBuff8(x, fldCallSetOtWork[k + 1][j + 2] * -70.0f + 50.0f, z, x + 100.0f,
                                           fldCallSetOtWork[k + 1][j + 3] * -70.0f + 50.0f, z, x,
                                           fldCallSetOtWork[k + 2][j + 2] * -70.0f + 50.0f, z - 50.0f, x + 100.0f,
                                           fldCallSetOtWork[k + 2][j + 3] * -70.0f + 50.0f, z - 50.0f,
                                           color, color, color, color, mat, u0, v0, u1, v1);
                v0 += 0.049999998f;
            }
            u0 += 0.049999998f;
        }
        u0 = fldCallSetOtData6;
        color = fldGlobalBase.word318 ? 0x60484048 : 0x60504040;
        if (u0 > 1.0f) {
            u0 -= 1.0f;
        }
        for (j = 0; j < 3; j++) {
            v0 = fldCallSetOtData2;
            if (v0 > 1.0f) {
                v0 -= 1.0f;
            }
            for (k = 0; k < 3; k++) {
                v1 = v0 + 0.19999999f;
                u1 = u0 + 0.19999999f;
                if (u0 > 1.0f) {
                    u0 -= 1.0f;
                    u1 -= 1.0f;
                }
                if (u0 < 0.0f) {
                    u0 += 1.0f;
                    u1 += 1.0f;
                }
                if (v0 > 1.0f) {
                    v0 -= 1.0f;
                    v1 -= 1.0f;
                }
                if (v0 < 0.0f) {
                    v0 += 1.0f;
                    v1 += 1.0f;
                }
                x = j * 500.0f + 2600.0f;
                z = 6000.0f - k * 500.0f;
                dds3CallGrAllocPacketBuff8(x, 50.0f, z, x + 500.0f, 50.0f, z, x, 50.0f, z - 500.0f, x + 500.0f, 50.0f,
                                           z - 500.0f,
                                           color, color, color, color, mat, u0, v0, u1, v1);
                v0 += 0.19999999f;
            }
            u0 += 0.19999999f;
        }
        fldCallSetOtData5 += 0.0014f;
        if (fldCallSetOtData2 > 1.0f) {
            fldCallSetOtData2 -= 1.0f;
        }
        color = fldGlobalBase.word318 ? 0x80485048 : 0x80505050;
        mat = fldGlobalBase.texSlot[0].file;
        eglLinkAlpha(13);
        eglLinkTest(1, 0, 128, 3, 0, 0, 1, 2);
        u0 = fldCallSetOtData5 + 1.0f;
        for (j = 0; j < 8; j++) {
            v0 = 1.0f;
            for (k = 0; k < 31; k++) {
                u1 = u0 - 0.099999995f;
                v1 = v0 - 0.099999995f;
                if (u0 > 1.0f) {
                    u0 -= 1.0f;
                    u1 -= 1.0f;
                }
                if (u0 < 0.0f) {
                    u0 += 1.0f;
                    u1 += 1.0f;
                }
                x = j * 100.0f + 3100.0f;
                z = 4500.0f - k * 50.0f;
                dds3CallGrAllocPacketBuff8(x, fldCallSetOtWork[k + 1][j + 2] * -70.0f + 50.0f, z, x + 100.0f,
                                           fldCallSetOtWork[k + 1][j + 3] * -70.0f + 50.0f, z, x,
                                           fldCallSetOtWork[k + 2][j + 2] * -70.0f + 50.0f, z - 50.0f, x + 100.0f,
                                           fldCallSetOtWork[k + 2][j + 3] * -70.0f + 50.0f, z - 50.0f,
                                           color, color, color, color, mat, u0, v0, u1, v1);
                v0 = v1;
            }
            u0 -= 0.099999995f;
        }
        color = fldGlobalBase.word318 ? 0x60485058 : 0x60505050;
        u0 = fldCallSetOtData5 + 1.0f;
        for (j = 0; j < 3; j++) {
            v0 = 1.0f;
            for (k = 0; k < 3; k++) {
                u1 = u0 - 0.19999999f;
                v1 = v0 - 0.19999999f;
                if (u0 > 1.0f) {
                    u0 -= 1.0f;
                    u1 -= 1.0f;
                }
                if (u0 < 0.0f) {
                    u0 += 1.0f;
                    u1 += 1.0f;
                }
                x = j * 500.0f + 2600.0f;
                z = 6000.0f - k * 500.0f;
                dds3CallGrAllocPacketBuff8(x, 50.0f, z, x + 500.0f, 50.0f, z, x, 50.0f, z - 500.0f, x + 500.0f, 50.0f,
                                           z - 500.0f,
                                           color, color, color, color, mat, u0, v0, u1, v1);
                v0 = v1;
            }
            u0 -= 0.19999999f;
        }
        if (cut == 5) {
            eglSetOt(23);
            eglLinkTest(1, 1, 128, 3, 0, 0, 1, 2);
            eglLinkAlpha(1);
            eglLinkPoly4(3100.0f, -50.0f, 2650.0f, 3600.0f, -50.0f, 2650.0f, 3100.0f, -50.0f, 2100.0f, 3600.0f, -50.0f,
                         2100.0f, 0x808080, 0x808080, 0x808080, 0x808080);
            eglLinkPoly4(3100.0f, -50.0f, 2100.0f, 3600.0f, -50.0f, 2100.0f, 3100.0f, 100.0f, 2100.0f, 3600.0f, 100.0f,
                         2100.0f, 0x808080, 0x808080, 0x808080, 0x808080);
            eglLinkPoly4(3600.0f, -50.0f, 2100.0f, 3600.0f, -50.0f, 2650.0f, 3600.0f, 100.0f, 2100.0f, 3600.0f, 100.0f,
                         2650.0f, 0x808080, 0x808080, 0x808080, 0x808080);
            eglLinkPoly4(3250.0f, -130.0f, 2600.0f, 3300.0f, -130.0f, 2600.0f, 3250.0f, 0.0f, 2600.0f, 3300.0f, 0.0f,
                         2600.0f, 0x808080, 0x808080, 0x808080, 0x808080);
            eglLinkPoly4(3300.0f, -105.0f, 2600.0f, 3350.0f, -105.0f, 2600.0f, 3300.0f, 0.0f, 2600.0f, 3350.0f, 0.0f,
                         2600.0f, 0x808080, 0x808080, 0x808080, 0x808080);
            eglLinkPoly4(3350.0f, -80.0f, 2600.0f, 3400.0f, -80.0f, 2600.0f, 3350.0f, 0.0f, 2600.0f, 3400.0f, 0.0f,
                         2600.0f, 0x808080, 0x808080, 0x808080, 0x808080);
            eglLinkPoly4(2600.0f, -200.0f, 4500.0f, 3100.0f, -200.0f, 4500.0f, 2600.0f, 100.0f, 4500.0f, 3100.0f, 100.0f,
                         4500.0f, 0x808080, 0x808080, 0x808080, 0x808080);
            eglLinkTest(1, 5, 128, 1, 0, 0, 1, 2);
            eglLinkAlpha(0);
        }
    }
    if ((area == 4 || area == 24) && (floor == 0 || floor == 4 || floor == 5) && (u32)(cut - 22) < 2) {
        eglSetOt(33);
        eglLinkTest(1, 0, 128, 3, 0, 0, 1, 2);
        eglLinkAlpha(0);
        fldDrwZLoad();
        eglLinkAlpha(1);
        eglLinkTest(1, 0, 128, 3, 0, 0, 1, 2);
        if (fldGlobalBase.amSBlock == 24 && evtBitCheck(1285)) {
            fldCallSetOtData4 += 0.00013999999f;
        } else {
            fldCallSetOtData4 += 0.0014f;
        }
        if (fldCallSetOtData4 > 1.0f) {
            fldCallSetOtData4 -= 1.0f;
        }
        if (!(fldGlobalBase.amSBlock == 24 && evtBitCheck(1285))) {
            fldCallSin7();
        }
        mat = fldGlobalBase.texSlot[2].file;
        if (fldGlobalBase.amSBlock == 24 && evtBitCheck(1285)) {
            eglLinkAlpha(1);
            color = 0x30202020;
        } else {
            eglLinkAlpha(0);
            color = fldGlobalBase.word318 ? 0x50283028 : 0x50303020;
        }
        u0 = fldCallSetOtData4;
        if (u0 > 1.0f) {
            u0 -= 1.0f;
        }
        for (j = 0; j < 12; j++) {
            v0 = fldCallSetOtData4;
            if (v0 > 1.0f) {
                v0 -= 1.0f;
            }
            color2 = color;
            if (j >= 8) {
                color2 = color - 0x10000000;
            }
            x = j * 100.0f + 600.0f;
            for (k = 0; k < 17; k++) {
                z = -1700.0f - k * 100.0f;
                v1 = v0 + 0.099999995f;
                dds3CallGrAllocPacketBuff8(x, fldCallSetOtWork[k + 1][j + 1] * -50.0f + 15.0f, z, x + 100.0f,
                                           fldCallSetOtWork[k + 1][j + 2] * -50.0f + 15.0f, z, x,
                                           fldCallSetOtWork[k + 2][j + 1] * -50.0f + 15.0f, z - 100.0f, x + 100.0f,
                                           fldCallSetOtWork[k + 2][j + 2] * -50.0f + 15.0f, z - 100.0f,
                                           color, color2, color, color2, mat, u0, v0, u0 + 0.099999995f, v1);
                v0 = v1;
            }
            color = color2;
            u0 += 0.099999995f;
        }
        if (fldGlobalBase.amSBlock == 24 && evtBitCheck(1285)) {
            fldCallSetOtData3 += 0.000099999991f;
        } else {
            fldCallSetOtData3 += 0.00099999994f;
        }
        if (fldCallSetOtData3 > 1.0f) {
            fldCallSetOtData3 -= 1.0f;
        }
        mat = fldGlobalBase.texSlot[0].file;
        eglLinkAlpha(1);
        eglLinkTest(1, 0, 128, 1, 0, 0, 1, 2);
        u0 = fldCallSetOtData3;
        color = 0x80000000;
        if (u0 > 1.0f) {
            u0 -= 1.0f;
        }
        for (j = 0; j < 12; j++) {
            v0 = 0.0f;
            color2 = color;
            if (j >= 8) {
                color2 = color - 0x10000000;
            }
            for (k = 0; k < 17; k++) {
                u1 = u0 + 0.099999995f;
                v1 = v0 + 0.099999995f;
                if (u0 > 1.0f) {
                    u0 -= 1.0f;
                    u1 -= 1.0f;
                }
                if (u0 < 0.0f) {
                    u0 += 1.0f;
                    u1 += 1.0f;
                }
                x = j * 100.0f + 600.0f;
                z = -1700.0f - k * 100.0f;
                dds3CallGrAllocPacketBuff8(x, fldCallSetOtWork[k + 1][j + 1] * -50.0f + 15.0f, z, x + 100.0f,
                                           fldCallSetOtWork[k + 1][j + 2] * -50.0f + 15.0f, z, x,
                                           fldCallSetOtWork[k + 2][j + 1] * -50.0f + 15.0f, z - 100.0f, x + 100.0f,
                                           fldCallSetOtWork[k + 2][j + 2] * -50.0f + 15.0f, z - 100.0f,
                                           color, color2, color, color2, mat, u0, v0, u1, v1);
                v0 = v1;
            }
            color = color2;
            u0 += 0.099999995f;
        }
        if (fldGlobalBase.amSBlock == 24 && evtBitCheck(1285)) {
            fldCallSetOtData2 += 0.000049999996f;
        } else {
            fldCallSetOtData2 += 0.00049999997f;
        }
        if (fldCallSetOtData2 > 1.0f) {
            fldCallSetOtData2 -= 1.0f;
        }
        if (fldGlobalBase.amSBlock == 24 && evtBitCheck(1285)) {
            fldCallSetOtData6 += 0.00012999999f;
        } else {
            fldCallSetOtData6 += 0.0012999999f;
        }
        if (fldCallSetOtData6 > 1.0f) {
            fldCallSetOtData6 -= 1.0f;
        }
        mat = fldGlobalBase.texSlot[0].file;
        if (fldGlobalBase.amSBlock == 24 && evtBitCheck(1285)) {
            color = 0x80505050;
        } else {
            color = fldGlobalBase.word318 ? 0x80485048 : 0x80505040;
        }
        eglLinkAlpha(13);
        eglLinkTest(1, 0, 128, 3, 0, 0, 1, 2);
        u0 = fldCallSetOtData6;
        if (u0 > 1.0f) {
            u0 -= 1.0f;
        }
        for (j = 0; j < 12; j++) {
            v0 = fldCallSetOtData2;
            if (v0 > 1.0f) {
                v0 -= 1.0f;
            }
            for (k = 0; k < 17; k++) {
                v1 = v0 + 0.049999998f;
                u1 = u0 + 0.049999998f;
                if (u0 > 1.0f) {
                    u0 -= 1.0f;
                    u1 -= 1.0f;
                }
                if (u0 < 0.0f) {
                    u0 += 1.0f;
                    u1 += 1.0f;
                }
                if (v0 > 1.0f) {
                    v0 -= 1.0f;
                    v1 -= 1.0f;
                }
                if (v0 < 0.0f) {
                    v0 += 1.0f;
                    v1 += 1.0f;
                }
                x = j * 100.0f + 600.0f;
                z = -1700.0f - k * 100.0f;
                dds3CallGrAllocPacketBuff8(x, fldCallSetOtWork[k + 1][j + 1] * -50.0f + 15.0f, z, x + 100.0f,
                                           fldCallSetOtWork[k + 1][j + 2] * -50.0f + 15.0f, z, x,
                                           fldCallSetOtWork[k + 2][j + 1] * -50.0f + 15.0f, z - 100.0f, x + 100.0f,
                                           fldCallSetOtWork[k + 2][j + 2] * -50.0f + 15.0f, z - 100.0f,
                                           color, color, color, color, mat, u0, v0, u1, v1);
                v0 += 0.049999998f;
            }
            u0 += 0.049999998f;
        }
        if (fldGlobalBase.amSBlock == 24 && evtBitCheck(1285)) {
            fldCallSetOtData5 += 0.00013999999f;
        } else {
            fldCallSetOtData5 += 0.0014f;
        }
        if (fldCallSetOtData2 > 1.0f) {
            fldCallSetOtData2 -= 1.0f;
        }
        color = fldGlobalBase.word318 ? 0x80485058 : 0x80505050;
        mat = fldGlobalBase.texSlot[0].file;
        eglLinkAlpha(13);
        eglLinkTest(1, 0, 128, 3, 0, 0, 1, 2);
        u0 = fldCallSetOtData5 + 1.0f;
        for (j = 0; j < 12; j++) {
            v0 = 1.0f;
            for (k = 0; k < 17; k++) {
                u1 = u0 - 0.099999995f;
                v1 = v0 - 0.099999995f;
                if (u0 > 1.0f) {
                    u0 -= 1.0f;
                    u1 -= 1.0f;
                }
                if (u0 < 0.0f) {
                    u0 += 1.0f;
                    u1 += 1.0f;
                }
                x = j * 100.0f + 600.0f;
                z = -1700.0f - k * 100.0f;
                dds3CallGrAllocPacketBuff8(x, fldCallSetOtWork[k + 1][j + 1] * -50.0f + 15.0f, z, x + 100.0f,
                                           fldCallSetOtWork[k + 1][j + 2] * -50.0f + 15.0f, z, x,
                                           fldCallSetOtWork[k + 2][j + 1] * -50.0f + 15.0f, z - 100.0f, x + 100.0f,
                                           fldCallSetOtWork[k + 2][j + 2] * -50.0f + 15.0f, z - 100.0f,
                                           color, color, color, color, mat, u0, v0, u1, v1);
                v0 = v1;
            }
            u0 -= 0.099999995f;
        }
        eglLinkAlpha(0);
        eglLinkTest(1, 5, 128, 3, 0, 0, 1, 2);
        if (cut == 22) {
            eglSetOt(23);
            eglLinkTest(1, 1, 128, 3, 0, 0, 1, 2);
            eglLinkAlpha(1);
            eglLinkPoly4(600.0f, -50.0f, -1700.0f, 900.0f, -50.0f, -1700.0f, 600.0f, -50.0f, -2195.0f, 900.0f, -50.0f,
                         -2195.0f, 0x808080, 0x808080, 0x808080, 0x808080);
            eglLinkTest(1, 5, 128, 1, 0, 0, 1, 2);
            eglLinkAlpha(0);
        }
        eglSetOt(23);
        eglLinkTest(1, 1, 128, 3, 0, 0, 1, 2);
        eglLinkAlpha(1);
        eglLinkPoly4(925.0f, -25.0f, -1800.0f, 960.0f, -25.0f, -1800.0f, 925.0f, -25.0f, -2370.0f, 960.0f, -25.0f, -2370.0f,
                     0x808080, 0x808080, 0x808080, 0x808080);
        eglLinkPoly4(925.0f, -25.0f, -1800.0f, 925.0f, -25.0f, -2370.0f, 925.0f, 100.0f, -1800.0f, 925.0f, 100.0f, -2370.0f,
                     0x808080, 0x808080, 0x808080, 0x808080);
        eglLinkPoly4(925.0f, -25.0f, -2340.0f, 1800.0f, -25.0f, -2340.0f, 925.0f, -25.0f, -2370.0f, 1800.0f, -25.0f,
                     -2370.0f, 0x808080, 0x808080, 0x808080, 0x808080);
        eglLinkPoly4(925.0f, -25.0f, -2370.0f, 1800.0f, -25.0f, -2370.0f, 925.0f, 100.0f, -2370.0f, 1800.0f, 100.0f,
                     -2370.0f, 0x808080, 0x808080, 0x808080, 0x808080);
        eglLinkPoly4(1800.0f, -25.0f, -2340.0f, 925.0f, -25.0f, -2340.0f, 1800.0f, 100.0f, -2340.0f, 925.0f, 100.0f,
                     -2340.0f, 0x808080, 0x808080, 0x808080, 0x808080);
        eglLinkTest(1, 5, 128, 1, 0, 0, 1, 2);
        eglLinkAlpha(0);
    }
    if ((area == 4 || area == 24) && (floor == 0 || floor == 4 || floor == 5) &&
        (cut == 4 || cut == 6 || cut == 26 || cut == 32 || cut == 33)) {
        fldCallSetOtData9 += 0.008999999f;
        fldCallSetOtData10 -= 0.00059999992f;
        if (fldCallSetOtData10 < 0.0f) {
            fldCallSetOtData10 += 1.0f;
        }
        fldCallSetOtData11 -= 0.00041999994f;
        if (fldCallSetOtData11 < 0.0f) {
            fldCallSetOtData11 += 1.0f;
        }
        fldCallSetOtData12 -= 0.00047999996f;
        if (fldCallSetOtData12 < 0.0f) {
            fldCallSetOtData12 += 1.0f;
        }
        fldCallSetOtData13 -= 0.00053999993f;
        if (fldCallSetOtData13 < 0.0f) {
            fldCallSetOtData13 += 1.0f;
        }
        fldCallSetOtData14 += 0.0014999999f;
        if (fldCallSetOtData14 > 1.0f) {
            fldCallSetOtData14 -= 1.0f;
        }
        fldCallSetOtData15 -= 0.00029999999f;
        if (fldCallSetOtData15 < 0.0f) {
            fldCallSetOtData15 += 1.0f;
        }
        fldCallSetOtData16 -= 0.00119999995f;
        if (fldCallSetOtData16 < 0.0f) {
            fldCallSetOtData16 += 1.0f;
        }
        eglSetOt(33);
        eglLinkTest(1, 0, 128, 3, 0, 0, 1, 2);
        eglLinkAlpha(0);
        fldDrwZLoad();
        for (i = 0; i < 4; i++) {
            if (i == 1) {
                mat = fldGlobalBase.texSlot[0].file;
                eglLinkAlpha(1);
                color = 0x80000000;
                eglLinkTest(1, 0, 128, 1, 0, 0, 1, 2);
            } else if (i == 2 || i == 3) {
                color = fldGlobalBase.word318 ? 0x80506070 : 0x80606060;
                mat = fldGlobalBase.texSlot[0].file;
                eglLinkAlpha(13);
                eglLinkTest(1, 0, 128, 3, 0, 0, 1, 2);
            } else {
                color = fldGlobalBase.word318 ? 0x20706070 : 0x20806060;
                mat = fldGlobalBase.texSlot[2].file;
                eglLinkAlpha(0);
                eglLinkTest(1, 0, 128, 3, 0, 0, 1, 2);
            }
            h = 0.099999995f;
            if (i == 1) {
                u0 = fldCallSetOtData15;
                v0 = fldCallSetOtData11;
                u1 = fldCallSetOtData15 + 1.0f;
            } else if (i == 2) {
                u0 = fldCallSetOtData14;
                v0 = fldCallSetOtData10;
                u1 = fldCallSetOtData14 + 1.0f;
            } else if (i == 3) {
                u0 = fldCallSetOtData16;
                v0 = fldCallSetOtData13;
                h = 0.066666663f;
                u1 = fldCallSetOtData16 + 0.5f;
            } else {
                u0 = 0.0f;
                v0 = fldCallSetOtData12;
                u1 = 1.0f;
            }
            v1 = v0 + h;
            if (v0 < 0.0f) {
                v0 += 1.0f;
                v1 += 1.0f;
            }
            if (cut == 4) {
                mapCallSin9(15, color, mat, fldCallSetOtData9, 3400.0f, 4900.0f, 0.0f, 13000.0f, u0, v0, u1, v1);
                mapCallSin9(15, color, mat, fldCallSetOtData9, 4900.0f, 7400.0f, 0.0f, 13000.0f, u1, v0, u1 + (u1 - u0),
                            v1);
            } else if (cut == 6) {
                mapCallSin9(15, color, mat, fldCallSetOtData9, 4000.0f, 7000.0f, 0.0f, 15000.0f, u0, v0, u1, v1);
                mapCallSin9(15, color, mat, fldCallSetOtData9, 7000.0f, 10000.0f, 0.0f, 15000.0f, u1, v0, u1 + (u1 - u0),
                            v1);
            } else if (cut == 26 || cut == 32 || cut == 33) {
                mapCallSin9(16, color, mat, fldCallSetOtData9, 3200.0f, 5200.0f, 60.0f, 18000.0f, u0, v0, u1, v1);
                mapCallSin10(16, color, mat, fldCallSetOtData9, 5200.0f, 9200.0f, 60.0f, 18000.0f, u1, v0,
                             u1 + (u1 - u0), v1);
            }
        }
        eglLinkAlpha(0);
        eglLinkTest(1, 5, 125, 1, 0, 0, 1, 2);
        if (cut == 4) {
            eglSetOt(23);
            eglLinkTest(1, 1, 128, 3, 0, 0, 1, 2);
            eglLinkAlpha(1);
            eglLinkPoly4(3100.0f, -50.0f, 6100.0f, 3900.0f, -50.0f, 6100.0f, 3100.0f, -50.0f, 5600.0f, 3900.0f, -50.0f,
                         5600.0f, 0x808080, 0x808080, 0x808080, 0x808080);
            eglLinkPoly4(3100.0f, -50.0f, 5600.0f, 3900.0f, -50.0f, 5600.0f, 3100.0f, 0.0f, 5600.0f, 3900.0f, 0.0f, 5600.0f,
                         0x808080, 0x808080, 0x808080, 0x808080);
            eglLinkPoly4(3100.0f, -50.0f, 2600.0f, 3600.0f, -50.0f, 2600.0f, 3100.0f, -50.0f, 2100.0f, 3600.0f, -50.0f,
                         2100.0f, 0x808080, 0x808080, 0x808080, 0x808080);
            eglLinkPoly4(3100.0f, -50.0f, 2100.0f, 3600.0f, -50.0f, 2100.0f, 3100.0f, 0.0f, 2100.0f, 3600.0f, 0.0f, 2100.0f,
                         0x808080, 0x808080, 0x808080, 0x808080);
            eglLinkPoly4(3100.0f, -250.0f, 950.0f, 3200.0f, -250.0f, 950.0f, 3100.0f, -250.0f, 500.0f, 3200.0f, -250.0f,
                         500.0f, 0x808080, 0x808080, 0x808080, 0x808080);
            eglLinkTest(1, 5, 128, 1, 0, 0, 1, 2);
            eglLinkAlpha(0);
        } else if (cut == 26 || cut == 32 || cut == 33) {
            eglSetOt(23);
            eglLinkTest(1, 1, 128, 3, 0, 0, 1, 2);
            eglLinkAlpha(1);
            eglLinkPoly4(3100.0f, -50.0f, 6100.0f, 3910.0f, -50.0f, 6100.0f, 3100.0f, -50.0f, 5600.0f, 3910.0f, -50.0f,
                         5600.0f, 0x808080, 0x808080, 0x808080, 0x808080);
            eglLinkPoly4(3100.0f, -50.0f, 5600.0f, 3920.0f, -50.0f, 5600.0f, 3100.0f, 100.0f, 5600.0f, 3920.0f, 100.0f,
                         5600.0f, 0x808080, 0x808080, 0x808080, 0x808080);
            eglLinkPoly4(3200.0f, -115.0f, 6300.0f, 3300.0f, -115.0f, 6300.0f, 3200.0f, -115.0f, 5600.0f, 3300.0f, -115.0f,
                         5600.0f, 0x808080, 0x808080, 0x808080, 0x808080);
            eglLinkPoly4(3300.0f, -82.0f, 6300.0f, 3350.0f, -82.0f, 6300.0f, 3300.0f, -82.0f, 5600.0f, 3350.0f, -82.0f,
                         5600.0f, 0x808080, 0x808080, 0x808080, 0x808080);
            eglLinkPoly4(3350.0f, -62.0f, 6250.0f, 3405.0f, -62.0f, 6250.0f, 3350.0f, -62.0f, 5600.0f, 3405.0f, -62.0f,
                         5600.0f, 0x808080, 0x808080, 0x808080, 0x808080);
            eglLinkTest(1, 5, 128, 1, 0, 0, 1, 2);
            eglLinkAlpha(0);
        }
        if ((cut == 26 && evtBitCheck(194) && !evtBitCheck(205)) || cut == 32 || cut == 33) {
            eglSetOt(38);
            eglLinkTest(1, 1, 128, 3, 0, 0, 1, 2);
            eglLinkAlpha(1);
            eglLinkPoly4(4200.0f, -250.0f, 18000.0f, 4200.0f, -250.0f, 5000.0f, 4200.0f, 0.0f, 18000.0f, 4200.0f, 0.0f,
                         5000.0f, 0x808080, 0x808080, 0x808080, 0x808080);
            eglLinkTest(1, 5, 128, 1, 0, 0, 1, 2);
            eglLinkAlpha(0);
        }
    }
    if (area == 15) {
        fldCallSetOtData += 0.015f;
        fldCallSetOtData2 -= 0.00099999994f;
        if (fldCallSetOtData2 < 0.0f) {
            fldCallSetOtData2 += 1.0f;
        }
        fldCallSetOtData3 -= 0.0007f;
        if (fldCallSetOtData3 < 0.0f) {
            fldCallSetOtData3 += 1.0f;
        }
        fldCallSetOtData4 -= 0.00059999998f;
        if (fldCallSetOtData4 < 0.0f) {
            fldCallSetOtData4 += 1.0f;
        }
        fldCallSetOtData5 -= 0.0009f;
        if (fldCallSetOtData5 < 0.0f) {
            fldCallSetOtData5 += 1.0f;
        }
        fldCallSetOtData6 += 0.00139999984f;
        if (fldCallSetOtData6 > 1.0f) {
            fldCallSetOtData6 -= 1.0f;
        }
        fldCallSetOtData7 -= 0.00027999998f;
        if (fldCallSetOtData7 < 0.0f) {
            fldCallSetOtData7 += 1.0f;
        }
        fldCallSetOtData8 -= 0.00111999992f;
        if (fldCallSetOtData8 < 0.0f) {
            fldCallSetOtData8 += 1.0f;
        }
        eglSetOt(33);
        eglLinkTest(1, 0, 128, 3, 0, 0, 1, 2);
        eglLinkAlpha(0);
        fldDrwZLoad();
        for (i = 0; i < 4; i++) {
            if (i == 1) {
                mat = fldGlobalBase.texSlot[0].file;
                eglLinkAlpha(1);
                color = 0x50000000;
                eglLinkTest(1, 0, 128, 1, 0, 0, 1, 2);
            } else if (i == 2 || i == 3) {
                color = fldGlobalBase.word318 ? 0x30101020 : 0x30201010;
                mat = fldGlobalBase.texSlot[0].file;
                eglLinkAlpha(13);
                eglLinkTest(1, 0, 128, 3, 0, 0, 1, 2);
            } else {
                color = fldGlobalBase.word318 ? 0x08080810 : 0x08100808;
                mat = fldGlobalBase.texSlot[2].file;
                eglLinkAlpha(0);
                eglLinkTest(1, 0, 128, 3, 0, 0, 1, 2);
            }
            h = 0.39999998f;
            if (i == 1) {
                u0 = fldCallSetOtData7;
                v0 = fldCallSetOtData3;
                u1 = fldCallSetOtData7 + 1.0f;
            } else if (i == 2) {
                u0 = fldCallSetOtData6;
                v0 = fldCallSetOtData2;
                u1 = fldCallSetOtData6 + 1.0f;
            } else if (i == 3) {
                u0 = fldCallSetOtData8;
                v0 = fldCallSetOtData5;
                h = 0.26666666f;
                u1 = fldCallSetOtData8 + 0.5f;
            } else {
                u0 = 0.0f;
                v0 = fldCallSetOtData4;
                u1 = 1.0f;
            }
            v1 = v0 + h;
            if (v0 < 0.0f) {
                v0 += 1.0f;
                v1 += 1.0f;
            }
            fldCallSin4(9, color, mat, fldCallSetOtData, -4100.0f, -2300.0f, 100.0f, 2000.0f, u0, v0, u1, v1);
            fldCallSin4(9, color, mat, fldCallSetOtData, -6300.0f, -4100.0f, 100.0f, 2000.0f, u0, v0, u1, v1);
        }
        eglLinkAlpha(0);
        eglLinkTest(1, 5, 125, 1, 0, 0, 1, 2);
    }
}
#else
INCLUDE_ASM(const s32, "code_0015D490", fldCallSetOt);
#endif

void fldCallGetRandA3(void) {
    s32 i;
    s32 j;

    for (i = 0; i < 32; i++) {
        for (j = 0; j < 32; j++) {
            fldWork7[i][j] = ((f32)((dds3GetRandA(NULL) >> 4) & 0xFF) - 128.0f) / 10000.0f;
            fldCallSetOtWork[i][j] = ((f32)((dds3GetRandA(NULL) >> 4) & 0xFF) - 128.0f) / 10000.0f;
        }
    }
}

INCLUDE_ASM(const s32, "code_0015D490", fldLeaf23);

INCLUDE_ASM(const s32, "code_0015D490", fldCallSin5);

INCLUDE_ASM(const s32, "code_0015D490", fldCallSin6);

INCLUDE_ASM(const s32, "code_0015D490", fldCallSin7);

void fldCallGetRandFloatA(mefMistParticle_t *particle) {
    mefMistSystem_t *mist = pMapMoya;
    s32 i;

    particle->fExist = 1;
    switch (mist->scroll) {
    case 0:
        particle->pos[0] = dds3GetRandFloatA(NULL) * 672.0f - 160.0f;
        particle->pos[1] = dds3GetRandFloatA(NULL) * 608.0f - 160.0f;
        break;
    case 1:
        particle->pos[0] = dds3GetRandFloatA(NULL) * 1184.0f - 160.0f;
        particle->pos[1] = dds3GetRandFloatA(NULL) * 608.0f - 160.0f;
        break;
    case 2:
        particle->pos[0] = dds3GetRandFloatA(NULL) * 672.0f - 160.0f;
        particle->pos[1] = dds3GetRandFloatA(NULL) * 1056.0f - 160.0f;
        break;
    case 3:
        particle->pos[0] = dds3GetRandFloatA(NULL) * 1184.0f - 160.0f;
        particle->pos[1] = dds3GetRandFloatA(NULL) * 1056.0f - 160.0f;
        break;
    }
    particle->velocity[0] = (dds3GetRandFloatA(NULL) - 0.5f) * 0.5f;
    particle->velocity[1] = (dds3GetRandFloatA(NULL) - 0.5f) * 0.5f;
    particle->lifeCnt = 0;
    particle->deathCnt = dds3GetRandIntA(NULL, 988) + 512;
    for (i = 0; i < 4; i++) {
        particle->rgba[i][3] = dds3GetRandIntA(NULL, 255);
        particle->rgba[i][2] = dds3GetRandIntA(NULL, 255);
        particle->rgba[i][1] = dds3GetRandIntA(NULL, 255);
        particle->rgba[i][0] = (particle->rgba[i][3] + particle->rgba[i][2] + particle->rgba[i][1]) / 3;
    }
    mist->num++;
}

/* body: matched in raidou @ 9ab2342: src/shared/code_r1_0019D3A8.c:827, found, same function 1.00 (words 9/9, calls 1.00, shape 1.00) */
s32 fldIsMapTitleActive(void) {
    return dds3SearchProcessName(fldTitleTaskName) != 0;
}

INCLUDE_ASM(const s32, "code_0015D490", fldCallLinkSprite);

INCLUDE_ASM(const s32, "code_0015D490", mapCallLinkSprite);

void mapTitleSkip(void) {
    if (fldCallSetOt4Data2 == 2) {
        fldCallSetOt4Data = 0;
        fldCallSetOt4Data2 = 3;
    }
}

INCLUDE_ASM(const s32, "code_0015D490", mapProcSequenceSub9);

INCLUDE_ASM(const s32, "code_0015D490", fldCallSetOt4);

INCLUDE_ASM(const s32, "code_0015D490", fldReleaseTitleTextures);

INCLUDE_ASM(const s32, "code_0015D490", mapTitleStart);

INCLUDE_ASM(const s32, "code_0015D490", mapTitleStart_fld);

/* body: matched in dds @ 18d1fa88f: src/dds1/game/code_001411F0.c:4799, found, same function 0.67 (words 15/15, calls 0.00, shape 1.00) */
void fldDestroyTitleTask(void) {
    if (fldIsMapTitleActive() != 0) {
        dds3KillProcessName(fldTitleTaskName, 1);
    }
}

INCLUDE_ASM(const s32, "code_0015D490", fldProcSequenceSub6);

void fldSe_Init(void) {
    s32 i;

    for (i = 0; i < 64; i++) {
        mapSoundKankyoSeSetWork[i].active = 0;
        mapSoundKankyoSeSetWork[i].se = 0;
        mapSoundKankyoSeSetWork[i].vol = 0;
        mapSoundKankyoSeSetWork[i].update = 0;
        mapSoundKankyoSeSetWork[i].state = 0;
    }
    mapData131 = 0;
    mapSoundKankyoTaskSubData = 100;
}

INCLUDE_ASM(const s32, "code_0015D490", fldCallSoundKankyoSeSet);

INCLUDE_ASM(const s32, "code_0015D490", mapBgTexLoad);

/* NON_MATCHING: 10 of 23 words match.  */
#ifdef NON_MATCHING
void mapSoundKankyoTaskSub(void) {
    s32 vol;

    switch (mapData131) {
    case -1:
        vol = mapSoundKankyoTaskSubData - 2;
        if (vol < 0) {
            vol = 0;
        }
        mapSoundKankyoTaskSubData = vol;
        break;
    case 1:
        vol = mapSoundKankyoTaskSubData + 2;
        if (vol > 100) {
            vol = 100;
        }
        mapSoundKankyoTaskSubData = vol;
        mapData131 = vol != 100;
        break;
    }
}
#else
INCLUDE_ASM(const s32, "code_0015D490", mapSoundKankyoTaskSub);
#endif

void mapSoundKankyoSeSet(s32 map, s32 floor, s32 cut) {
    s32 i;
    s32 j;
    s32 idx;
    s32 se;
    s32 vol;

    for (i = 0; i < 64; i++) {
        idx = -1;
        vol = 0;
        se = 0;
        if (mapData4[i].map == map) {
            for (j = 0; j < 32; j++) {
                if (mapData4[i].cutvol[j].cut == cut) {
                    idx = j;
                    vol = mapData4[i].cutvol[j].vol;
                    se = mapData4[i].se;
                    break;
                }
            }
        }
        if (mapSoundKankyoSeSetWork[i].active != 0) {
            if (idx != -1 && se != 0 && vol != 0) {
                p2sdVolume2(se, vol * mapSoundKankyoTaskSubData / 100);
                continue;
            }
            p2sdPlaySE_Vol_Loop(mapSoundKankyoSeSetWork[i].se);
            mapSoundKankyoSeSetWork[i].active = 0;
            continue;
        }
        if (idx == -1 || se == 0 || vol == 0) {
            continue;
        }
        if (evtBitCheck(184) || evtBitCheck(206)) {
            if (map == 1) {
                if (mapData4[i].se == 51) {
                    continue;
                }
                if (mapData4[i].se == 52) {
                    continue;
                }
                if (mapData4[i].se == 53) {
                    continue;
                }
            }
            if (map == 2) {
                if (mapData4[i].se == 51) {
                    continue;
                }
                if (mapData4[i].se == 52) {
                    continue;
                }
            }
            if (map == 4) {
                if (mapData4[i].se == 50) {
                    continue;
                }
                if (mapData4[i].se == 51) {
                    continue;
                }
                if (mapData4[i].se == 52) {
                    continue;
                }
                if (mapData4[i].se == 53) {
                    continue;
                }
                if (mapData4[i].se == 54) {
                    continue;
                }
            }
            if (map == 5) {
                if (mapData4[i].se == 48) {
                    continue;
                }
                if (mapData4[i].se == 49) {
                    continue;
                }
            }
        }
        if ((evtBitCheck(65) && !evtBitCheck(96)) || evtBitCheck(184)) {
            if (map == 3) {
                if (mapData4[i].se == 49) {
                    continue;
                }
                if (mapData4[i].se == 51) {
                    continue;
                }
                if (mapData4[i].se == 52) {
                    continue;
                }
            }
        }
        if (evtBitCheck(184) && map == 3 && floor == 0 && mapData4[i].se == 50) {
            continue;
        }
        if ((evtBitCheck(131) && !evtBitCheck(145)) || evtBitCheck(184)) {
            if (map == 7) {
                if (mapData4[i].se == 48) {
                    continue;
                }
                if (mapData4[i].se == 49) {
                    continue;
                }
                if (mapData4[i].se == 50) {
                    continue;
                }
            }
        }
        mapSoundKankyoSeSetWork[i].active = 1;
        mapSoundKankyoSeSetWork[i].se = mapData4[i].se;
        mapSoundKankyoSeSetWork[i].vol = vol;
        mapSoundKankyoSeSetWork[i].update = 0;
        mapSoundKankyoSeSetWork[i].state = mapData4[i].waittime;
        if (mapSoundKankyoSeSetWork[i].state == 0) {
            p2sdPlaySE_Vol(se, vol * mapSoundKankyoTaskSubData / 100);
        }
    }
}

void mapSoundKankyoTask(s32 map, s32 floor, s32 cut) {
    s32 i;
    s32 j;
    s32 idx;
    s32 se;
    s32 vol;

    mapSoundKankyoTaskSub();
    for (i = 0; i < 64; i++) {
        idx = -1;
        vol = 0;
        se = 0;
        if (mapData4[i].map == map) {
            for (j = 0; j < 32; j++) {
                if (mapData4[i].cutvol[j].cut == cut) {
                    idx = j;
                    vol = mapData4[i].cutvol[j].vol;
                    se = mapData4[i].se;
                    break;
                }
            }
        }
        if (mapSoundKankyoSeSetWork[i].active == 0) {
            continue;
        }
        if (evtBitCheck(184)) {
            if (map == 1) {
                if (se == 51) {
                    continue;
                }
                if (se == 52) {
                    continue;
                }
                if (se == 53) {
                    continue;
                }
            }
            if (map == 2) {
                if (se == 51) {
                    continue;
                }
                if (se == 52) {
                    continue;
                }
            }
            if (map == 4) {
                if (se == 50) {
                    continue;
                }
                if (se == 51) {
                    continue;
                }
                if (se == 52) {
                    continue;
                }
                if (se == 53) {
                    continue;
                }
                if (se == 54) {
                    continue;
                }
            }
            if (map == 5) {
                if (se == 48) {
                    continue;
                }
                if (se == 49) {
                    continue;
                }
            }
        }
        if ((evtBitCheck(65) && !evtBitCheck(96)) || evtBitCheck(184)) {
            if (map == 3) {
                if (se == 49) {
                    continue;
                }
                if (se == 51) {
                    continue;
                }
                if (se == 52) {
                    continue;
                }
            }
        }
        if ((evtBitCheck(131) && !evtBitCheck(145)) || evtBitCheck(184)) {
            if (map == 7) {
                if (se == 48) {
                    continue;
                }
                if (se == 49) {
                    continue;
                }
                if (se == 50) {
                    continue;
                }
            }
        }
        if (idx == -1 || se == 0 || vol == 0) {
            continue;
        }
        if (mapSoundKankyoSeSetWork[i].state > 0) {
            if (--mapSoundKankyoSeSetWork[i].state > 0) {
                continue;
            }
            mapSoundKankyoSeSetWork[i].state = mapData4[i].waittime;
            p2sdPlaySE_Vol(se, vol * mapSoundKankyoTaskSubData / 100);
        } else if (mapSoundKankyoSeSetWork[i].state == 0 && mapData131 != 0) {
            p2sdVolume2(se, vol * mapSoundKankyoTaskSubData / 100);
        }
    }
}

void mapCallPlaySEVolLoop(void) {
    s32 i;

    for (i = 0; i < 64; i++) {
        if (mapSoundKankyoSeSetWork[i].active != 0) {
            p2sdPlaySE_Vol_Loop(mapSoundKankyoSeSetWork[i].se);
        }
    }
    fldSe_Init();
}

void mapProcSequenceSub10(void) {
    s32 i;

    for (i = 0; i < 8; i++) {
        mapFileLoadSub8Work[i].active = 0;
        mapFileLoadSub8Work[i].se = 0;
        mapFileLoadSub8Work[i].vol = 0;
        mapFileLoadSub8Work[i].update = 0;
        mapFileLoadSub8Work[i].state = 0;
    }
}

INCLUDE_ASM(const s32, "code_0015D490", mapFileLoadSub8);

void mapCallPlaySEVolLoop2(void) {
    s32 i;

    for (i = 0; i < 8; i++) {
        if (mapFileLoadSub8Work[i].active != 0) {
            p2sdPlaySE_Vol_Loop(mapFileLoadSub8Work[i].se);
        }
    }
    mapProcSequenceSub10();
}

INCLUDE_ASM(const s32, "code_0015D490", mapSoundMoveSeVol);

void mapSoundMoveSeTask(void) {
    s32 i;
    s32 vol;

    for (i = 0; i < 8; i++) {
        if (mapFileLoadSub8Work[i].active != 0 && mapFileLoadSub8Work[i].update != 0) {
            mapFileLoadSub8Work[i].update = 0;
            if (mapFileLoadSub8Work[i].vol > 0) {
                vol = mapFileLoadSub8Work[i].vol * mapSoundKankyoTaskSubData / 100;
                if (mapFileLoadSub8Work[i].state != 0) {
                    p2sdVolume2(mapFileLoadSub8Work[i].se, vol);
                } else {
                    p2sdPlaySE_Vol(mapFileLoadSub8Work[i].se, vol);
                    mapFileLoadSub8Work[i].state = 1;
                }
            } else {
                p2sdPlaySE_Vol_Loop(mapFileLoadSub8Work[i].se);
                mapFileLoadSub8Work[i].state = 0;
            }
        }
    }
}

INCLUDE_ASM(const s32, "code_0015D490", mapSoundMoveSeFade0);

INCLUDE_ASM(const s32, "code_0015D490", mapLeaf23);

INCLUDE_ASM(const s32, "code_0015D490", mapLeaf24);

s32 p2sdCallBitCheck(s32 map, s32 floor) {
    s32 bgm;

    if (map >= 35) {
        return 8;
    }
    if (map == 13 && floor >= 16) {
        bgm = p2sdCallBitCheckData[floor];
    } else {
        bgm = p2sdData[map][floor];
    }
    if (evtBitCheck(184) && !evtBitCheck(208) && bgm == 8) {
        bgm = 15;
    }
    return bgm;
}

s32 p2sdSoundSmgLoadChkSub(s32 map) {
    return p2sdSoundSmgLoadChkSubData[map];
}

void p2sdPlaySE_Vol(s32 se, s32 vol) {
    p2sdPlaySE(p2sdData2 + se, vol, 63);
}

INCLUDE_ASM(const s32, "code_0015D490", p2sdVolume2);

INCLUDE_ASM(const s32, "code_0015D490", p2sdPlaySE_Vol_Loop);

void mapSoundSmgLoadChk(s32 map) {
    s32 addr = p2sdSoundSmgLoadChkSub(map);

    fldBgmSmgAddr = addr;
    p2sdData2 = addr;
    fldBgmLoadStep = 0;
}

/* body: matched in smt3 @ 9401f79eb: src/smt3/field/fldSe.c:734, found, same function 1.00 (words 32/32, calls 1.00, shape 1.00) */
s32 mapSoundSmgLoadChk_fld(void) {
    switch (fldBgmLoadStep) {
    case 0:
        if (p2sdLoadCheckMidi(fldBgmSmgAddr) == 0) {
            p2sdLoadSMGFile(fldBgmSmgAddr);
            fldBgmLoadStep = fldBgmLoadStep + 1;
        } else {
            fldBgmLoadStep = fldBgmLoadStep + 2;
        }
        break;
    case 1:
        if (p2sdLoadCheckMidi(fldBgmSmgAddr) == 1) {
            fldBgmLoadStep = fldBgmLoadStep + 1;
        }
        break;
    default:
        fldBgmLoadStep = -1;
        return 1;
    }
    return 0;
}

INCLUDE_ASM(const s32, "code_0015D490", mapNakamaCashLoadEndCheckSub);

INCLUDE_ASM(const s32, "code_0015D490", sndCallLoadCheckMidi);

INCLUDE_ASM(const s32, "code_0015D490", sndSeLoadId);

INCLUDE_ASM(const s32, "code_0015D490", sndNakamaJoinTaskSub);

void datInitBossPress(void) {
    s32 i;

    for (i = 0; i < 16; i++) {
        datWork7[i].timer = -1;
        datWork7[i].vol = 127;
    }
}

INCLUDE_ASM(const s32, "code_0015D490", datNakamaMotionSub);

/* NON_MATCHING: 19 of 29 words match.  */
#ifdef NON_MATCHING
void mapSeNakamaDelayTask(void) {
    mapSeDelay_t *delay = datWork7;
    s32 i;

    for (i = 0; i < 16; i++, delay++) {
        if (delay->timer >= 0) {
            delay->timer--;
            if (delay->timer < 0) {
                p2sdPlaySE(mapData129 + i, delay->vol, 63);
            }
        }
    }
}
#else
INCLUDE_ASM(const s32, "code_0015D490", mapSeNakamaDelayTask);
#endif

void mapSeWallHit(s32 hit) {
    if (mapSeWallHitData == 1 && hit == 0) {
        mapSeWallHitData = 0;
    } else if (mapSeWallHitData == 0 && hit == 1) {
        /* The loop keeps ee-gcc from making the call a sibling call (calls.c tries none inside a loop), as in SMT3's dds3SceneBasicStartScript. */
        do {
            mapSeWallHitData = 1;
            p2sdPlaySE_Vol(33, 127);
        } while (0);
    }
}

void mapTakaraOpen(void) {
    takara_openanm_objid = -1;
}

INCLUDE_ASM(const s32, "code_0015D490", mapTakaraOpen_fld);

INCLUDE_ASM(const s32, "code_0015D490", mapRefDDS3GBWK5);

void mapTakaraTask(void) {
    mdlObj_t *model;

    if (takara_openanm_objid != -1) {
        datDebugPrintf(mapStrTakaraOpenanmObjid, takara_openanm_objid);
        model = dds3ObjectBaseGetModelHandle(mapObjGetBasic(takara_openanm_objid));
        if (model->first->stat == SDF_MOTION_STAT_NOCUT) {
            mapObjMotionCng(takara_openanm_objid, 2, 0);
            takara_openanm_objid = -1;
        }
    }
}

void DecisionFeedIn(void) {
    fldDrwZBuf2 = effLoadIndexedResource(DecisionFeedInStrMapAll, DecisionFeedInStrMapMeiSpr, 0);
}

void mapProcShutDownSub(void) {
    if (fldDrwZBuf2 != NULL) {
        effReleaseTextureHandlesAndResetSlots(fldDrwZBuf2);
        effDestroyResourceSlotSet(fldDrwZBuf2);
        fldDrwZBuf2 = NULL;
    }
}

/* NON_MATCHING: 152 of 207 words match.  */
#ifdef NON_MATCHING
void mapIfDrawES(void) {
    u32 palette[4];
    s32 pct;
    s32 cnt;
    s32 max;
    s32 area;
    s32 level;
    s32 r;
    s32 b;
    s32 g;
    f32 t;
    u32 color;

    if (fldDrwZBuf2 == NULL) {
        return;
    }
    pct = 0;
    eglSetOt(103);
    eglLinkAlpha(1);
    eglLinkTest(1, 0, 128, 3, 0, 0, 1, 1);
    area = fldGlobalBase.nameD;
    cnt = sbtlGetRndEncCnt();
    max = sbtlGetRndEncMax(area);
    if (max < cnt) {
        cnt = max;
    }
    if (max > 0) {
        pct = cnt * 100 / max;
        level = 0;
        if (pct > 50) {
            level = 1;
        }
        if (pct > 90) {
            level = 2;
        }
    } else {
        level = 4;
    }
    r = 128;
    eglLinkAlpha(0);
    itfDrawGridWithResolvedSlot(6864, 2736, 0, 32, fldDrwZBuf2, 3, 103);
    b = 128;
    g = 128;
    if (level == 4) {
        r = 0;
        g = 50;
        b = 170;
        mapIfDrawESData2 += 0.02f;
    }
    if (level == 0) {
        t = pct * 0.02f;
        if (t > 1.0f) {
            t = 1.0f;
        }
        mapIfDrawESData2 += 0.03f;
        r = (1.0f - t) * 190.0f + t * 200.0f;
        b = (1.0f - t) * 60.0f + t * 20.0f;
        g = (1.0f - t) * 160.0f + t * 90.0f;
    }
    if (level == 1) {
        t = (pct - 50) * 0.025f;
        if (t > 1.0f) {
            t = 1.0f;
        }
        mapIfDrawESData2 += 0.05f;
        r = (1.0f - t) * 200.0f + t * 160.0f;
        b = (1.0f - t) * 20.0f + t * 40.0f;
        g = (1.0f - t) * 90.0f + t * 35.0f;
    }
    if (level == 2) {
        r = 160;
        g = 35;
        b = 40;
        mapIfDrawESData2 += 0.07f;
    }
    color = ((s32)(sdfSin(mapIfDrawESData2) * 25.0f) + 65) | (r << 24) | (g << 16 | b << 8);
    palette[0] = color;
    palette[1] = color;
    palette[2] = color;
    palette[3] = color;
    effDrawSpriteWithCornerColors(7184, 2816, 0, palette, 32, fldDrwZBuf2, 4, 103);
    eglLinkAlpha(0);
}
#else
INCLUDE_ASM(const s32, "code_0015D490", mapIfDrawES);
#endif

INCLUDE_ASM(const s32, "code_0015D490", mapIfDrawActionWin2);

/* NON_MATCHING: 89 of 167 words match.  */
#ifdef NON_MATCHING
void mapCallEnableMode(s32 map, s32 floor, s32 cut) {
    frFontHandle_t *font;
    u32 width;
    u32 color;
    s32 i;
    s32 right = 496;

    if (fldGlobalBase.previousFloor != -1) {
        frEnableMode(1);
        font = frSetFontString(mapCallEnableModeData2[fldGlobalBase.previousFloor], 0, 0, 0, NULL);
        frDisableMode(1);
        frSetFontSpace(font, 1);
        width = frGetStrWidth2(font);
        frSetDrawPosition(font, (right - width) * 16, 3232);
        frSetFontColor(font, 0xF0DC6480);
        fldDrwZBuf2->descriptions[2].rect[3] = width + 39;
        itfDrawGridWithResolvedSlot((right - width - 23) * 16, 3184, 0, 32, fldDrwZBuf2, 2, 103);
        frDrawFontOt(font, 0, 103);
        frDestroyFontRequestAfter(font);
    } else {
        for (i = 0; i < 160; i++) {
            if (mapData[i].map == map && mapData[i].floor == floor && mapData[i].cut == cut) {
                color = 0xA09DC380;
                if (mapData[i].visited != 0) {
                    color = 0xF0DC6480;
                }
                frEnableMode(1);
                font = frSetFontString(mapData[i].name, 0, 0, 0, NULL);
                frDisableMode(1);
                frSetFontSpace(font, -1);
                width = frGetStrWidth2(font);
                frSetDrawPosition(font, (right - width) * 16, 3232);
                frSetFontColor(font, color);
                fldDrwZBuf2->descriptions[2].rect[3] = width + 39;
                itfDrawGridWithResolvedSlot((right - width - 23) * 16, 3184, 0, 32, fldDrwZBuf2, 2, 103);
                frDrawFontOt(font, 0, 103);
                frDestroyFontRequestAfter(font);
            }
        }
    }
}
#else
INCLUDE_ASM(const s32, "code_0015D490", mapCallEnableMode);
#endif

INCLUDE_ASM(const s32, "code_0015D490", mapIfDrawMskSub);
