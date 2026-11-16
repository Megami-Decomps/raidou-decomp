#include "common.h"
#include "dds3KernelDraw.h"
#include "fld.h"
#include "dds3KernelDraw_0010A750.h"
#include "EventBit.h"
#include "dds3KernelCore.h"
#include "fldDbg.h"
#include "field.h"
#include "mapFilter.h"
#include "mapLbWork.h"
#include "mapData.h"
#include "dds3KernelMain.h"

extern const char mapStrSkyBgEdit[];
extern f32 mapData93;
extern f32 mapData94;
extern f32 mapSKYBGEDITData;
extern f32 mapSKYBGEDITData2;
extern f32 mapSKYBGEDITData3;
extern u32 mapSKYBGEDITData5;
extern s32 mapWork4;
extern s32 mapBgSetScrollData17;
extern s32 mapBgSetScrollData18;
extern f32 mapZoomSetGoalData21;
extern f32 mapZoomSetGoalData22;

extern void eglLinkPoly4(f32 x0, f32 y0, f32 z0, f32 x1, f32 y1, f32 z1, f32 x2, f32 y2, f32 z2, f32 x3, f32 y3, f32 z3, u32 c0, u32 c1, u32 c2, u32 c3);
extern void dds3CallGrAllocPacketBuff5(u32 c0, u32 c1, u32 c2, u32 c3, f32 x, f32 y, f32 w, f32 h);
extern void fldCallSetOt(s32 area, s32 floor, s32 cut);
extern void fldCallLinkAlpha(u32 color, f32 x, f32 y);
extern void fldCallSetOt3(u32 color, f32 ox, f32 oy);
extern void KernelDebug(s32 camId);
extern void pipeFreeLink(void);
extern void mapLeaf22(s32 camId);

#define SKY_GRAY 0x808080

/* Floor quad at height y: x0..x1 across, zNear..zFar deep. */
#define SKY_FLOOR(x0, x1, y, zNear, zFar) \
    eglLinkPoly4((x0), (y), (zNear), (x1), (y), (zNear), (x0), (y), (zFar), (x1), (y), (zFar), \
                 SKY_GRAY, SKY_GRAY, SKY_GRAY, SKY_GRAY)

#define SKY_OT_BEGIN(ot, test2, test3, alpha) \
    eglSetOt(ot); \
    eglLinkTest(1, test2, 128, test3, 0, 0, 1, 2); \
    eglLinkAlpha(alpha)

/* Black bar over the part of the screen the map does not cover. */
#define SKY_BAR(x, y, w, h) \
    eglSetOt(98); \
    eglLinkAlpha(0); \
    eglLinkTest(1, 0, 128, 1, 0, 0, 1, 1); \
    dds3CallGrAllocPacketBuff5(0x80000000, 0x80000000, 0x80000000, 0x80000000, (x), (y), (w), (h)); \
    eglSetOt(18)

/* NON_MATCHING: 242 of 1850 words match. Functionally checked against the asm: call multiset and float literal pool equal retail. Retail duplicates the camId 3 block, reloads constants per eglLinkPoly4 call and orders the draws differently. */
#ifdef NON_MATCHING
void mapSKYBGEDIT(void) {
    f32 sizeX = 512.0f;
    f32 sizeY = 448.0f;
    f32 offX = 0.0f;
    f32 offY = 0.0f;
    f32 zero;
    s32 camId;
    u32 color;
    s32 i;

    if (fldDbg.skyEnable == 0) {
        return;
    }
    camId = fldGlobalBase.camId;

    if (fldGlobalBase.texture2B0 != NULL && fldGlobalBase.cutIdx < 64) {
        f32 posX = 0.0f;
        f32 posY = 0.0f;
        f32 scale;
        f32 pivot;
        f32 wrap;
        f32 uA;
        f32 uB;

        if (camId >= 0 && camId < 4) {
            posY = mapData94 + mapSKYBGEDITData;
            posX = posX - mapData93 / Zoom;
        }
        scale = mapData33[fldGlobalBase.cutIdx].skyScale;
        pivot = mapData33[fldGlobalBase.cutIdx].skyOffset;
        if (dds3SearchProcessName(mapStrSkyBgEdit) == NULL) {
            fldDbg.skyOffset = pivot;
            fldDbg.skyScale = scale;
        } else {
            pivot = fldDbg.skyOffset;
            scale = fldDbg.skyScale;
        }

        wrap = fldGlobalBase.float2B8 + fldGlobalBase.float2B4;
        if (512.0f <= wrap) wrap -= 512.0f;
        if (wrap < 0.0f) wrap += 512.0f;
        if (512.0f <= wrap) wrap -= 512.0f;
        if (wrap < 0.0f) wrap += 512.0f;

        uB = posX + 256.0f;
        posX = posX / scale;
        uB = uB / scale;
        if (512.0f <= uB) uB -= 512.0f;
        posX += wrap;
        uB += wrap;
        if (512.0f <= posX) posX -= 512.0f;
        if (posX < 0.0f) posX += 512.0f;
        if (512.0f <= uB) uB -= 512.0f;
        if (uB < 0.0f) uB += 512.0f;
        uA = posX;

        eglSetOt(18);
        eglLinkAlpha(4);
        eglLinkTest(1, 0, 128, 1, 0, 0, 1, 1);
        eglCallDLinkTexPacket2(Zoom * 0.0f + mapSKYBGEDITData2, posY + pivot * Zoom + mapSKYBGEDITData3,
                               Zoom * 256.0f, scale * 255.0f * Zoom, uA, 1.0f, 256.0f / scale, 254.0f,
                               mapSKYBGEDITData5, mapSKYBGEDITData5, mapSKYBGEDITData5, mapSKYBGEDITData5,
                               fldGlobalBase.texture2B0);
        eglCallDLinkTexPacket2(Zoom * 256.0f + mapSKYBGEDITData2, posY + pivot * Zoom + mapSKYBGEDITData3,
                               Zoom * 256.0f, scale * 255.0f * Zoom, uB, 1.0f, 256.0f / scale, 254.0f,
                               mapSKYBGEDITData5, mapSKYBGEDITData5, mapSKYBGEDITData5, mapSKYBGEDITData5,
                               fldGlobalBase.texture2B0);
    }
    if (fldData22 == 4) {
        return;
    }

    fldCallSetOt(fldGlobalBase.amSBlock, fldGlobalBase.nameU, fldGlobalBase.cutIdx);
    eglSetOt(18);
    eglLinkAlpha(1);
    eglLinkTest(1, 0, 128, 2, 0, 0, 1, 1);
    eglLinkBoxZ(0, 0, 512, 448, 0, 0, 0, 0, 0);
    if (mapWork4 == 0) {
        return;
    }
    eglLinkAlpha(4);
    eglLinkTest(1, 0, 128, 3, 0, 0, 1, 1);
    color = mapSKYBGEDITData5;
    KernelDebug(camId);

    if (camId >= 0 && camId < 4) {
        f32 zoomX = Zoom;

        switch (camId) {
        case 0:
            sizeY = zoomX * 448.0f;
            sizeX = zoomX * 512.0f;
            break;
        case 1:
            sizeY = zoomX * 448.0f;
            sizeX = zoomX * 1024.0f;
            break;
        case 2:
            sizeY = zoomX * 896.0f;
            sizeX = zoomX * 512.0f;
            break;
        default:
            sizeY = zoomX * 896.0f;
            sizeX = zoomX * 1024.0f;
            break;
        }
        offY = -((mapData94 + mapSKYBGEDITData) + mapSKYBGEDITData3);
        offX = -(mapData93 + mapSKYBGEDITData2);
        eglLinkAlpha(4);
        eglLinkTest(1, 0, 128, 1, 0, 0, 1, 1);
        fldCallLinkAlpha(color, mapData93 + mapSKYBGEDITData2, (mapData94 + mapSKYBGEDITData) + mapSKYBGEDITData3);
        eglLinkAlpha(0);
        fldCallSetOt3(color, mapData93 + mapSKYBGEDITData2, (mapData94 + mapSKYBGEDITData) + mapSKYBGEDITData3);
    }

    zero = 0.0f;
    if (zero < mapData93 + mapSKYBGEDITData2) {
        SKY_BAR(zero, zero, mapData93 + mapSKYBGEDITData2, 448.0f);
    }
    if (mapData93 + mapSKYBGEDITData2 + sizeX < 512.0f) {
        SKY_BAR(mapData93 + mapSKYBGEDITData2 + sizeX, zero, 512.0f - (mapData93 + mapSKYBGEDITData2 + sizeX), 448.0f);
    }
    if (zero < (mapData94 + mapSKYBGEDITData) + mapSKYBGEDITData3) {
        SKY_BAR(zero, zero, 512.0f, (mapData94 + mapSKYBGEDITData) + mapSKYBGEDITData3);
    }
    if ((mapData94 + mapSKYBGEDITData) + mapSKYBGEDITData3 + sizeY < 448.0f) {
        SKY_BAR(zero, (mapData94 + mapSKYBGEDITData) + mapSKYBGEDITData3 + sizeY, 512.0f,
                448.0f - ((mapData94 + mapSKYBGEDITData) + mapSKYBGEDITData3 + sizeY));
    }
    eglLinkAlpha(0);

    if (fldGlobalBase.menuOpenFlag != 0) {
        s32 area = fldGlobalBase.amSBlock;
        s32 floor = fldGlobalBase.nameU;
        s32 cut = fldGlobalBase.cutIdx;

        SKY_OT_BEGIN(23, 1, 3, 1);
        if ((u32)(area - 36) < 6) {
            SKY_FLOOR(-1500.0f, 1500.0f, 3.0f, 1500.0f, -1500.0f);
        } else if (area == 3 && floor == 2) {
            SKY_FLOOR(-500.0f, 500.0f, 3.0f, 1000.0f, 0.0f);
        } else if (area == 4 && floor == 2) {
            SKY_FLOOR(-2000.0f, 0.0f, 3.0f, 3600.0f, 300.0f);
            SKY_FLOOR(0.0f, 2000.0f, 3.0f, 3600.0f, 300.0f);
        } else if (area == 9 && floor == 0 && cut == 6) {
            SKY_FLOOR(-700.0f, 700.0f, 3.0f, 4000.0f, 2000.0f);
        } else if (area == 10 && (floor == 3 || floor == 5)) {
            SKY_FLOOR(-1000.0f, 1000.0f, -97.0f, 4900.0f, 2200.0f);
            SKY_FLOOR(-1000.0f, -400.0f, -117.0f, 2600.0f, 2000.0f);
            SKY_FLOOR(-200.0f, 200.0f, -117.0f, 2600.0f, 2000.0f);
            SKY_FLOOR(400.0f, 1000.0f, -117.0f, 2600.0f, 2000.0f);
        } else if (area == 12 && floor == 0 && cut == 2) {
            SKY_FLOOR(-1500.0f, 0.0f, 3.0f, 4000.0f, 2000.0f);
            SKY_FLOOR(0.0f, 1500.0f, 3.0f, 4000.0f, 2000.0f);
            SKY_FLOOR(-1500.0f, 0.0f, 3.0f, 2000.0f, 500.0f);
            SKY_FLOOR(0.0f, 1500.0f, 3.0f, 2000.0f, 500.0f);
        } else if (area == 22 && floor == 0 && cut == 14) {
            SKY_FLOOR(4000.0f, 5000.0f, 3.0f, -1900.0f, -4500.0f);
            SKY_FLOOR(5000.0f, 6000.0f, 3.0f, -1900.0f, -4500.0f);
        } else if (area == 22 && floor == 0 && cut == 16) {
            SKY_FLOOR(4000.0f, 6000.0f, -7.0f, -4500.0f, -6500.0f);
        } else if (area == 23 && floor == 1 && cut == 14) {
            SKY_FLOOR(6000.0f, 9000.0f, 3.0f, 3500.0f, 2000.0f);
        } else if (area == 23 && floor == 1 && cut == 32) {
            SKY_FLOOR(6000.0f, 8000.0f, 3.0f, 500.0f, -2000.0f);
        } else if (area == 28 && floor == 0) {
            SKY_FLOOR(-1500.0f, 1500.0f, 3.0f, 2000.0f, -1000.0f);
        } else if (area == 31 && floor == 3 && cut == 13) {
            SKY_FLOOR(-1200.0f, 1200.0f, 3.0f, 1500.0f, -800.0f);
        }
        eglLinkTest(1, 5, 128, 1, 0, 0, 1, 2);
        eglLinkAlpha(0);
    }

    if (fldGlobalBase.amSBlock == 13 && (u32)(fldGlobalBase.nameU - 3) < 2) {
        SKY_OT_BEGIN(23, 1, 3, 1);
        SKY_FLOOR(-1700.0f, -1200.0f, -495.0f, 4300.0f, 1400.0f);
        eglLinkTest(1, 5, 128, 1, 0, 0, 1, 2);
        eglLinkAlpha(0);
    } else if (fldGlobalBase.amSBlock == 8 && (fldGlobalBase.nameU == 0 || fldGlobalBase.nameU == 1)
               && ((u32)(fldGlobalBase.cutIdx - 21) < 5 || fldGlobalBase.cutIdx == 40
                   || fldGlobalBase.cutIdx == 41 || fldGlobalBase.cutIdx == 44)) {
        SKY_OT_BEGIN(23, 1, 3, 1);
        eglLinkPoly4(-1000.0f, -500.0f, 1700.0f, 0.0f, -500.0f, 1700.0f, -1000.0f, 0.0f, 1700.0f, 0.0f, 0.0f, 1700.0f,
                     SKY_GRAY, SKY_GRAY, SKY_GRAY, SKY_GRAY);
        eglLinkPoly4(0.0f, -500.0f, 1700.0f, 1000.0f, -500.0f, 1700.0f, 0.0f, 0.0f, 1700.0f, 1000.0f, 0.0f, 1700.0f,
                     SKY_GRAY, SKY_GRAY, SKY_GRAY, SKY_GRAY);
        eglLinkTest(1, 5, 128, 1, 0, 0, 1, 2);
        eglLinkAlpha(0);
    } else if (fldGlobalBase.amSBlock == 9 && fldGlobalBase.nameU == 0
               && (fldGlobalBase.cutIdx == 3 || fldGlobalBase.cutIdx == 7 || fldGlobalBase.cutIdx == 2
                   || fldGlobalBase.cutIdx == 1)
               && evtBitCheck(1558)) {
        SKY_OT_BEGIN(23, 1, 3, 1);
        eglLinkPoly4(-500.0f, -500.0f, 2305.0f, 500.0f, -500.0f, 2305.0f, -500.0f, 0.0f, 2305.0f, 500.0f, 0.0f, 2305.0f,
                     SKY_GRAY, SKY_GRAY, SKY_GRAY, SKY_GRAY);
        eglLinkTest(1, 5, 128, 1, 0, 0, 1, 2);
        eglLinkAlpha(0);
    }

    if (fldGlobalBase.amFloor == 4) {
        SKY_OT_BEGIN(23, 1, 3, 1);
        SKY_FLOOR(-1500.0f, 1500.0f, 5.0f, 1500.0f, -1500.0f);
        if (fldGlobalBase.cutIdx == 3) {
            SKY_FLOOR(-1500.0f, 1500.0f, 5.0f, 0.0f, -3000.0f);
        }
        eglLinkTest(1, 5, 128, 1, 0, 0, 1, 2);
        eglLinkAlpha(0);
    }

    if (fldJmsQuestMapLoad_fldData != NULL && fldData22 == 0) {
        u32 boxColor = fldDbg.spriteTint != 0 ? 0x2080F080 : SKY_GRAY;

        eglSetOt(18);
        eglLinkAlpha(0);
        eglLinkTest(1, 1, 128, 3, 0, 0, 1, 3);
        for (i = 0; i < fldData21->count; i++) {
            mapLbSprite_t *sp = &mapSKYBGEDITData4[i];
            f32 gx;
            f32 gy;
            f32 zoom;
            f32 x0, y0, x1, y1, x2, y2, x3, y3;
            s32 ix0, iy0, ix1, iy1, ix2, iy2, ix3, iy3;

            if (sp->tex == 0) {
                continue;
            }
            gx = (f32)mapBgSetScrollData17;
            gy = (f32)mapBgSetScrollData18;
            zoom = Zoom;
            if (camId >= 0 && camId < 3) {
                gx = mapData93 + mapSKYBGEDITData2;
                gy = (mapData94 + mapSKYBGEDITData) + mapSKYBGEDITData3;
            }
            if (fldGlobalBase.camId == 0) {
                switch (dds3LinkBoxZSub()) {
                case 0:
                    zoom += zoom;
                    break;
                case 1:
                    gx -= 512.0f;
                    zoom += zoom;
                    break;
                case 2:
                    gy -= 448.0f;
                    zoom += zoom;
                    break;
                case 3:
                    gx -= 512.0f;
                    gy -= 448.0f;
                    zoom += zoom;
                    break;
                }
            } else {
                switch (dds3LinkBoxZSub()) {
                case 0:
                    gx += gx;
                    gy += gy;
                    zoom += zoom;
                    break;
                case 1:
                    gx = (gx + gx) - 512.0f;
                    gy += gy;
                    zoom += zoom;
                    break;
                case 2:
                    gx += gx;
                    gy = (gy + gy) - 448.0f;
                    zoom += zoom;
                    break;
                case 3:
                    gx = (gx + gx) - 512.0f;
                    gy = (gy + gy) - 448.0f;
                    zoom += zoom;
                    break;
                }
            }

            x0 = sp->pos[0] * zoom;
            y0 = sp->pos[1] * zoom;
            x1 = sp->pos[2] * zoom;
            y1 = sp->pos[3] * zoom;
            x2 = sp->pos[4] * zoom;
            y2 = sp->pos[5] * zoom;
            x3 = sp->pos[6] * zoom;
            y3 = sp->pos[7] * zoom;
            if (camId == 3) {
                f32 baseY = mapData94 + mapSKYBGEDITData;

                eglLinkBox((s32)(mapData93 + (s32)x0), (s32)(baseY + (s32)y0), (s32)(mapData93 + (s32)x1),
                           (s32)(baseY + (s32)y1), (s32)(mapData93 + (s32)x2), (s32)(baseY + (s32)y2),
                           (s32)(mapData93 + (s32)x3), (s32)(baseY + (s32)y3), sp->tex, boxColor, boxColor,
                           boxColor, boxColor);
                continue;
            }
            if (camId < 0 || camId >= 3) {
                x0 -= mapZoomSetGoalData21;
                x1 -= mapZoomSetGoalData21;
                x2 -= mapZoomSetGoalData21;
                x3 -= mapZoomSetGoalData21;
                y0 -= mapZoomSetGoalData22;
                y1 -= mapZoomSetGoalData22;
                y2 -= mapZoomSetGoalData22;
                y3 -= mapZoomSetGoalData22;
            }
            ix0 = (s32)x0;
            iy0 = (s32)y0;
            ix1 = (s32)x1;
            iy1 = (s32)y1;
            ix2 = (s32)x2;
            iy2 = (s32)y2;
            ix3 = (s32)x3;
            iy3 = (s32)y3;
            eglLinkBox((s32)(gx + ix0), (s32)(gy + iy0), (s32)(gx + ix1), (s32)(gy + iy1), (s32)(gx + ix2),
                       (s32)(gy + iy2), (s32)(gx + ix3), (s32)(gy + iy3), sp->tex, boxColor, boxColor, boxColor,
                       boxColor);
        }
        eglLinkAlpha(0);
        eglLinkTest(1, 1, 128, 3, 0, 0, 1, 1);
    }

    if (mapFileLoadSub7() != 0) {
        pipeFreeLink();
        mefMistDraw(offX, offY, 0);
        mapLeaf22(camId);
    }
    eglSetOt(28);
    eglLinkTest(1, 0, 128, 2, 0, 0, 1, 2);
}
#else
INCLUDE_ASM(const s32, "code_r1_0013B0F8", mapSKYBGEDIT);
#endif
