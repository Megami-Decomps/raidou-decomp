#include "common.h"
#include "dds3ObjectBase.h"

#include "basic.h"
#include "dds3obj.h"
#include "field.h"
#include "include_asm.h"
#include "kernel_draw.h"
#include "sdf_packet_list.h"
#include "sdf_resource.h"
#include "sdk_libc.h"
#include "billboard.h"
#include "sdf_types.h"
#include "sdf_lighting.h"
#include "libsdf.h"
#include "model.h"
#include "motion.h"
#include "draw.h"
#include "sdf_movie_types.h"
#include "ps2asm.h"
#include "fpu.h"
#include "kernel.h"
#include "draw3d.h"
#include "scr.h"
#include "fldFileResolver_00132080.h"
#include "effPCPMisc_00224D10.h"
#include "fldFileResolver.h"
#include "datCalc.h"
#include "newdata.h"
#include "fld.h"
#include "fldProcess.h"
#include "fldDraw.h"
#include "dds3KernelMain.h"
#include "mapData.h"
#include "mapLbWork.h"
#include "mdlManager.h"
#include "fldDbg.h"
#include "sdfQuat.h"
#include "sdfMath.h"
#include "sdk_libm.h"
#include "sdfMemory.h"
#include "fileManager.h"
#include "dds3KernelCore.h"
#include "sdf3D.h"
#include "EventBit.h"
#include "sdfTex.h"
#include "effManager.h"
#include "dds3KernelDraw.h"
#include "sdfVu0.h"

void mapBgSetScrollReset(void) {
    dds3ScreenParam.height = 448.0f;
    sdf3DScreenAdjust = 0;
    sdf3DScreenAdjustOffsetX = 0.0f;
    sdf3DScreenAdjustOffsetY = 0.0f;
    sdf3DScreenAdjustScaleX = 1.0f;
    sdf3DScreenAdjustScaleY = 1.0f;
    dds3SetScreenFovy(0.628f);
}

void map_fldPoint3To2(f32 x3, f32 y3, f32 z3, f32 *x2, f32 *y2) {
    sdfVector4_t vec = { x3, y3, z3, 1.0f };
    sdfVector4_t result;

    SDF_VU0_LOADMATRIX(&sdfViewMatrix);
    MultMatrixRev(&dds3ScreenParam.TM);
    SDF_VU0_COPYMATRIX_A_TO_B();
    SDF_VU0_LOADVECTOR_MEMOPS(*(const u128 *)&vec);
    SDF_VU0_APPLYMATRIX_XYZ1();
    SDF_VU0_PERSVECTOR();
    SDF_VU0_LOADVECTOR_B(&dds3ScreenParam.scale);
    SDF_VU0_MULVECTOR();
    SDF_VU0_LOADVECTOR_B(&dds3ScreenParam.trans);
    SDF_VU0_ADDVECTOR();
    SDF_VU0_STOREVECTOR(&result);
    *x2 = result.x;
    *y2 = result.y;
}

INCLUDE_ASM(const s32, "code_0013CDE0", map_fldPoint3To2O);

void map_fldPoint3To2O2(f32 x3, f32 y3, f32 z3, f32 *x2, f32 *y2) {
    sdfVector4_t scale = {mapData73 * 512.0f, 224.0f, 1.0f, 1.0f};
    sdfVector4_t point;
    sdfVector4_t screen;

    point.x = x3;
    point.y = y3;
    point.z = z3;
    point.w = 1.0f;
    SDF_VU0_LOADMATRIX(&sdfViewMatrix);
    MultMatrixRev(&dds3ScreenParam.TM);
    SDF_VU0_COPYMATRIX_A_TO_B();
    SDF_VU0_LOADVECTOR(&point);
    SDF_VU0_APPLYMATRIX_XYZ1();
    SDF_VU0_PERSVECTOR();
    SDF_VU0_LOADVECTOR_B(&scale);
    SDF_VU0_MULVECTOR();
    SDF_VU0_LOADVECTOR_B(&dds3ScreenParam.trans);
    SDF_VU0_ADDVECTOR();
    SDF_VU0_STOREVECTOR(&screen);
    *x2 = screen.x;
    *y2 = screen.y;
}

INCLUDE_ASM(const s32, "code_0013CDE0", map_fldPoint3To2O3);

/* NON_MATCHING: 159 of 173 words match.  */
void fldBgZoomTest(s32 mode) {
    if (mode == 2) {
        mapZoomSetGoal(fldGlobalBase.focusX, fldGlobalBase.focusY - 130.0f, fldGlobalBase.focusZ, 1.0f, 30);
    } else if (mode == 3) {
        mapZoomSetGoal(fldGlobalBase.focusX, fldGlobalBase.focusY - 130.0f, fldGlobalBase.focusZ, 2.0f, 30);
    } else if (mode == 4) {
        mapZoomClear();
    } else if (mode == 5) {
        mapZoomSetGoal(fldGlobalBase.focusX, fldGlobalBase.focusY - 130.0f, fldGlobalBase.focusZ, 2.0f, 0);
    } else if (mode == 6) {
        mapZoomSetGoal(fldGlobalBase.focusX, fldGlobalBase.focusY - 130.0f, fldGlobalBase.focusZ, 1.7f, 30);
    } else if (mode == 7) {
        mapZoomSetGoal(fldGlobalBase.focusX, fldGlobalBase.focusY - 130.0f, fldGlobalBase.focusZ, 3.0f, 30);
    } else if (mode == 8) {
        mapZoomSetGoal(fldGlobalBase.focusX, fldGlobalBase.focusY - 80.0f, fldGlobalBase.focusZ, 1.5f, 9);
    } else if (mode == 9) {
        mapZoomSetGoal(fldGlobalBase.focusX, fldGlobalBase.focusY - 130.0f, fldGlobalBase.focusZ, 1.0f, 1);
    } else if (mode == 0) {
        if (tzoom_event != 0) {
            tzoom_event = 1;
            tzoom_event_time = 30;
            tzoom_event_cnt = 30;
            mapZoomSetGoal(fldGlobalBase.focusX, fldGlobalBase.focusY - 130.0f, fldGlobalBase.focusZ, 1.0f, 30);
            FAKEMATCH_ASM_LIVE(mode);
        } else {
            mapZoomSetGoal(fldGlobalBase.focusX, fldGlobalBase.focusY - 130.0f, fldGlobalBase.focusZ, 1.0f, 30);
            FAKEMATCH_ASM_LIVE(mode);
        }
    } else {
        mapZoomSetGoal(fldGlobalBase.focusX, fldGlobalBase.focusY - 130.0f, fldGlobalBase.focusZ, 2.0f, 30);
        FAKEMATCH_ASM_LIVE(mode);
    }
}

INCLUDE_ASM(const s32, "code_0013CDE0", fldBgZoomTest2);

/* NON_MATCHING: 54 of 1277 words match.  */
#ifdef NON_MATCHING
void mapZoomSetGoal(f32 x, f32 y, f32 z, f32 zoom, s32 time) {
    f32 px;
    f32 py;
    f32 gx;
    f32 gy;
    f32 ftime;
    f32 cx;
    f32 cy;
    f32 tx;
    f32 ty;
    f32 w;
    f32 half;
    f32 lo;
    f32 hi;
    f32 edge;
    f32 cex;
    f32 cox;
    f32 ex;
    f32 ox;
    f32 ey;
    f32 oy;
    s32 zoomIn;

    ftime = time;
    if (fldGlobalBase.camId == 1) {
        mapZoomSetGoalData = mapData59;
        mapZoomSetGoalData9 = Zoom;
        mapData73 = Zoom;
        mapZoomSetGoalData10 = zoom;
        mapZoomSetGoalData2 = mapZoomSetGoalData4;
        mapZoomSetGoalData3 = mapZoomSetGoalData5;
        mapBgSetScrollData4 = time;
        mapZoomSetGoalData6 = x;
        mapZoomSetGoalData7 = y;
        mapData74 = (mapZoomSetGoalData10 - mapData73) / ftime;
        mapZoomSetGoalData8 = z;
        map_fldPoint3To2O2(mapData59, mapZoomSetGoalData4, mapZoomSetGoalData5, &px, &py);
        mapZoomSetGoalData27 = (px - 2048.0f) / mapData73 + -mapData95 / mapData73;
        mapData87 = mapZoomSetGoalData27;
        mapZoomSetGoalData28 = (py - 2048.0f) / mapData73 * 2.0f;
        mapData88 = mapZoomSetGoalData28;
        map_fldPoint3To2O2(x, y, z, &px, &py);
        tx = (px - 2048.0f) / mapData73 + -mapData95 / mapData73;
        ty = (py - 2048.0f) / mapData73 * 2.0f;

        cx = mapZoomSetGoalData27;
        w = mapData73 * 1024.0f;
        half = -(w * 0.5f);
        edge = half - (cx * mapData73 - 256.0f);
        if (edge > 0.0f) {
            cx += edge / mapData73;
            edge = half - (cx * mapData73 - 256.0f);
        }
        if (edge + w < 512.0f) {
            cx -= (512.0f - (edge + w)) / mapData73;
        }
        mapData87 = cx;
        mapZoomSetGoalData27 = cx;
        mapBgSetScrollData9 = mapData93;
        mapZoomSetGoalData31 = mapData93;
        mapBgSetScrollData13 = mapData95;
        mapZoomSetGoalData33 = mapData95;

        w = mapZoomSetGoalData10 * 1024.0f;
        half = -(w * 0.5f);
        ox = -(tx * mapZoomSetGoalData10);
        ex = half - (tx * mapZoomSetGoalData10 - 256.0f);
        if (ex > 0.0f) {
            tx += ex / mapZoomSetGoalData10;
            ox = -(tx * mapZoomSetGoalData10);
            ex = half - (tx * mapZoomSetGoalData10 - 256.0f);
        }
        if (ex + w < 512.0f) {
            tx -= (512.0f - (ex + w)) / mapZoomSetGoalData10;
            ox = -(tx * mapZoomSetGoalData10);
            ex = half - (tx * mapZoomSetGoalData10 - 256.0f);
        }
        mapZoomSetGoalData29 = tx;
        mapData89 = ex;
        mapData91 = ox;

        cy = mapZoomSetGoalData28;
        w = mapData73 * 448.0f;
        half = -(w * 0.5f);
        edge = half - (cy * mapData73 - 224.0f);
        if (edge > 0.0f) {
            cy += edge / mapData73;
            edge = half - (cy * mapData73 - 224.0f);
        }
        if (edge + w < 448.0f) {
            cy -= (448.0f - (edge + w)) / mapData73;
        }
        mapData88 = cy;
        mapZoomSetGoalData28 = cy;
        mapBgSetScrollData10 = mapData94;
        mapZoomSetGoalData32 = mapData94;
        mapBgSetScrollData14 = mapData96;
        mapZoomSetGoalData34 = mapData96;

        w = mapZoomSetGoalData10 * 448.0f;
        half = -(w * 0.5f);
        oy = -(ty * mapZoomSetGoalData10) * 0.5f;
        ey = half - (ty * mapZoomSetGoalData10 - 224.0f);
        if (ey > 0.0f) {
            ty += ey / mapZoomSetGoalData10;
            oy = -(ty * mapZoomSetGoalData10) * 0.5f;
            ey = half - (ty * mapZoomSetGoalData10 - 224.0f);
        }
        if (ey + w < 448.0f) {
            ty -= (448.0f - (ey + w)) / mapZoomSetGoalData10;
            oy = -(ty * mapZoomSetGoalData10) * 0.5f;
            ey = half - (ty * mapZoomSetGoalData10 - 224.0f);
        }
        mapData90 = ey;
        mapZoomSetGoalData30 = ty;
        mapData92 = oy;

        mapBgSetScrollData7 = (tx - cx) / ftime;
        mapBgSetScrollData8 = (ty - cy) / ftime;
        mapBgSetScrollData11 = (ex - mapData93) / ftime;
        mapBgSetScrollData12 = (ey - mapData94) / ftime;
        mapBgSetScrollData15 = (ox - mapData95) / ftime;
        mapBgSetScrollData16 = (oy - mapData96) / ftime;
    } else if (fldGlobalBase.camId == 2) {
        mapBgSetScrollData4 = time;
        mapZoomSetGoalData9 = Zoom;
        mapData73 = Zoom;
        mapZoomSetGoalData10 = zoom;
        mapZoomSetGoalData = mapData59;
        mapZoomSetGoalData2 = mapZoomSetGoalData4;
        mapZoomSetGoalData3 = mapZoomSetGoalData5;
        mapZoomSetGoalData6 = x;
        mapZoomSetGoalData7 = y;
        mapZoomSetGoalData8 = z;
        mapData74 = (mapZoomSetGoalData10 - mapData73) / ftime;
        if (Zoom < 1.01f && zoom >= 1.01f) {
            mapZoomSetGoalData35 = mapBgSetScrollData6;
        }
        map_fldPoint3To2O2(mapData59, mapZoomSetGoalData4, mapZoomSetGoalData5, &px, &py);
        mapZoomSetGoalData28 =
            (py - 2048.0f) * 2.0f / mapData73 + mapData96 * -2.0f / mapData73 + mapData73 * 32.0f;
        mapData88 = mapZoomSetGoalData28;
        mapZoomSetGoalData27 = (px - 2048.0f) / mapData73 + -mapData95 / mapData73;
        mapData87 = mapZoomSetGoalData27;
        map_fldPoint3To2O2(x, y, z, &px, &py);
        mapZoomSetGoalData29 = (px - 2048.0f) / mapData73 + -mapData95 / mapData73;
        if (zoom < 1.01f) {
            mapZoomSetGoalData30 = mapZoomSetGoalData35;
        } else {
            mapZoomSetGoalData30 = (py - 2048.0f) * 2.0f + mapData96 * -2.0f + mapData73 * 96.0f / mapData73;
        }

        cx = mapZoomSetGoalData27;
        w = mapData73 * 512.0f;
        half = -(w * 0.5f);
        cox = -(cx * mapData73);
        cex = half - (cx * mapData73 - 256.0f);
        if (cex > 0.0f) {
            cx += cex / mapData73;
            cox = -(cx * mapData73);
            cex = half - (cx * mapData73 - 256.0f);
        }
        if (cex + w < 512.0f) {
            cx -= (512.0f - (cex + w)) / mapData73;
            cox = -(cx * mapData73);
            cex = half - (cx * mapData73 - 256.0f);
        }
        mapData87 = cx;
        mapZoomSetGoalData27 = cx;
        mapBgSetScrollData9 = cex;
        mapZoomSetGoalData31 = cex;
        mapBgSetScrollData13 = cox;
        mapZoomSetGoalData33 = cox;

        tx = mapZoomSetGoalData29;
        w = mapZoomSetGoalData10 * 512.0f;
        half = -(w * 0.5f);
        ox = -(tx * mapZoomSetGoalData10);
        ex = half - (tx * mapZoomSetGoalData10 - 256.0f);
        if (ex > 0.0f) {
            tx += ex / mapZoomSetGoalData10;
            ox = -(tx * mapZoomSetGoalData10);
            ex = half - (tx * mapZoomSetGoalData10 - 256.0f);
        }
        if (ex + w < 512.0f) {
            tx -= (512.0f - (ex + w)) / mapZoomSetGoalData10;
            ox = -(tx * mapZoomSetGoalData10);
            ex = half - (tx * mapZoomSetGoalData10 - 256.0f);
        }
        mapZoomSetGoalData29 = tx;
        mapData89 = ex;
        mapData91 = ox;

        cy = mapZoomSetGoalData28;
        w = mapData73 * 896.0f;
        half = -(w * 0.5f);
        lo = mapData73 * -64.0f;
        edge = half - (cy * mapData73 - 224.0f);
        if (lo < edge) {
            cy -= (lo - edge) / mapData73;
            edge = half - (cy * mapData73 - 224.0f);
        }
        if (edge + w < 448.0f) {
            cy -= (448.0f - (edge + w)) / mapData73;
        }
        mapData88 = cy;
        mapZoomSetGoalData28 = cy;
        mapBgSetScrollData10 = mapData94;
        mapZoomSetGoalData32 = mapData94;
        mapBgSetScrollData14 = mapData96;
        mapZoomSetGoalData34 = mapData96;

        ty = mapZoomSetGoalData30;
        w = mapZoomSetGoalData10 * 896.0f;
        half = -(w * 0.5f);
        lo = mapZoomSetGoalData10 * -64.0f;
        oy = -(ty * mapZoomSetGoalData10) * 0.5f + mapZoomSetGoalData10 * 32.0f;
        ey = half - (ty * mapZoomSetGoalData10 - 224.0f);
        if (lo < ey) {
            ty -= (lo - ey) / mapZoomSetGoalData10;
            oy = -(ty * mapZoomSetGoalData10) * 0.5f + mapZoomSetGoalData10 * 32.0f;
            ey = half - (ty * mapZoomSetGoalData10 - 224.0f);
        }
        if (ey + w < 448.0f) {
            ty -= (448.0f - (ey + w)) / mapZoomSetGoalData10;
            oy = -(ty * mapZoomSetGoalData10) * 0.5f + mapZoomSetGoalData10 * 32.0f;
            ey = half - (ty * mapZoomSetGoalData10 - 224.0f);
        }
        mapData90 = ey;
        mapZoomSetGoalData30 = ty;
        mapData92 = oy;

        mapBgSetScrollData7 = (tx - cx) / ftime;
        mapBgSetScrollData8 = (ty - cy) / ftime;
        mapBgSetScrollData11 = (ex - cex) / ftime;
        mapBgSetScrollData12 = (ey - mapData94) / ftime;
        mapBgSetScrollData15 = (ox - cox) / ftime;
        mapBgSetScrollData16 = (oy - mapData96) / ftime;
    } else if (fldGlobalBase.camId == 3) {
        mapBgSetScrollData4 = time;
        mapZoomSetGoalData9 = Zoom;
        mapData73 = Zoom;
        mapZoomSetGoalData10 = zoom;
        mapZoomSetGoalData = mapData59;
        mapZoomSetGoalData2 = mapZoomSetGoalData4;
        mapZoomSetGoalData3 = mapZoomSetGoalData5;
        mapZoomSetGoalData6 = x;
        mapZoomSetGoalData7 = y;
        mapZoomSetGoalData8 = z;
        mapData74 = (mapZoomSetGoalData10 - mapData73) / ftime;
        if (Zoom < 1.01f && zoom >= 1.01f) {
            mapZoomSetGoalData35 = mapBgSetScrollData6;
        }
        map_fldPoint3To2O2(mapData59, mapZoomSetGoalData4, mapZoomSetGoalData5, &px, &py);
        mapZoomSetGoalData28 =
            (py - 2048.0f) * 2.0f / mapData73 + mapData96 * -2.0f / mapData73 + mapData73 * 32.0f;
        mapData88 = mapZoomSetGoalData28;
        mapZoomSetGoalData27 = (px - 2048.0f) / mapData73 + -mapData95 / mapData73;
        mapData87 = mapZoomSetGoalData27;
        map_fldPoint3To2O2(x, y, z, &px, &py);
        mapZoomSetGoalData29 = (px - 2048.0f) / mapData73 + -mapData95 / mapData73;
        if (zoom < 1.01f) {
            mapZoomSetGoalData30 = mapZoomSetGoalData35;
        } else {
            mapZoomSetGoalData30 = (py - 2048.0f) * 2.0f + mapData96 * -2.0f + mapData73 * 96.0f / mapData73;
        }

        cx = mapZoomSetGoalData27;
        w = mapData73 * 1024.0f;
        half = -(w * 0.5f);
        edge = half - (cx * mapData73 - 256.0f);
        if (edge > 0.0f) {
            cx += edge / mapData73;
            edge = half - (cx * mapData73 - 256.0f);
        }
        if (edge + w < 512.0f) {
            cx -= (512.0f - (edge + w)) / mapData73;
        }
        mapData87 = cx;
        mapZoomSetGoalData27 = cx;
        mapBgSetScrollData9 = mapData93;
        mapZoomSetGoalData31 = mapData93;
        mapBgSetScrollData13 = mapData95;
        mapZoomSetGoalData33 = mapData95;

        tx = mapZoomSetGoalData29;
        w = mapZoomSetGoalData10 * 1024.0f;
        half = -(w * 0.5f);
        ox = -(tx * mapZoomSetGoalData10);
        ex = half - (tx * mapZoomSetGoalData10 - 256.0f);
        if (ex > 0.0f) {
            tx += ex / mapZoomSetGoalData10;
            ox = -(tx * mapZoomSetGoalData10);
            ex = half - (tx * mapZoomSetGoalData10 - 256.0f);
        }
        if (ex + w < 512.0f) {
            tx -= (512.0f - (ex + w)) / mapZoomSetGoalData10;
            ox = -(tx * mapZoomSetGoalData10);
            ex = half - (tx * mapZoomSetGoalData10 - 256.0f);
        }
        mapZoomSetGoalData29 = tx;
        mapData89 = ex;
        mapData91 = ox;

        cy = mapZoomSetGoalData28;
        w = mapData73 * 896.0f;
        half = -(w * 0.5f);
        lo = mapData73 * -64.0f;
        edge = half - (cy * mapData73 - 224.0f);
        if (lo < edge) {
            cy -= (lo - edge) / mapData73;
            edge = half - (cy * mapData73 - 224.0f);
        }
        if (edge + w < 448.0f) {
            cy -= (448.0f - (edge + w)) / mapData73;
        }
        mapData88 = cy;
        mapZoomSetGoalData28 = cy;
        mapBgSetScrollData10 = mapData94;
        mapZoomSetGoalData32 = mapData94;
        mapBgSetScrollData14 = mapData96;
        mapZoomSetGoalData34 = mapData96;

        ty = mapZoomSetGoalData30;
        w = mapZoomSetGoalData10 * 896.0f;
        half = -(w * 0.5f);
        lo = mapZoomSetGoalData10 * -64.0f;
        oy = -(ty * mapZoomSetGoalData10) * 0.5f + mapZoomSetGoalData10 * 32.0f;
        ey = half - (ty * mapZoomSetGoalData10 - 224.0f);
        if (lo < ey) {
            ty -= (lo - ey) / mapZoomSetGoalData10;
            oy = -(ty * mapZoomSetGoalData10) * 0.5f + mapZoomSetGoalData10 * 32.0f;
            ey = half - (ty * mapZoomSetGoalData10 - 224.0f);
        }
        if (ey + w < 448.0f) {
            ty -= (448.0f - (ey + w)) / mapZoomSetGoalData10;
            oy = -(ty * mapZoomSetGoalData10) * 0.5f + mapZoomSetGoalData10 * 32.0f;
            ey = half - (ty * mapZoomSetGoalData10 - 224.0f);
        }
        mapData90 = ey;
        mapZoomSetGoalData30 = ty;
        mapData92 = oy;

        mapBgSetScrollData7 = (tx - cx) / ftime;
        mapBgSetScrollData8 = (ty - cy) / ftime;
        mapBgSetScrollData11 = (ex - mapData93) / ftime;
        mapBgSetScrollData12 = (ey - mapData94) / ftime;
        mapBgSetScrollData15 = (ox - mapData95) / ftime;
        mapBgSetScrollData16 = (oy - mapData96) / ftime;
    } else {
        map_fldPoint3To2O(x, y, z, &gx, &gy);
        if (zoom > 0.999f && zoom < 1.001f &&
            (time == 0 || (zoom - Zoom > -0.001f && zoom - Zoom < 0.001f))) {
            mapZoomClear();
            return;
        }
        if (zoom < Zoom) {
            lo = 256.0f / Zoom;
            hi = 512.0f - lo;
            if (hi < gx) {
                gx = hi;
            }
            if (gx < lo) {
                gx = lo;
            }
            lo = 224.0f / Zoom;
            hi = 448.0f - lo;
            if (hi < gy) {
                gy = hi;
            }
            if (gy < lo) {
                gy = lo;
            }
            zoomIn = FALSE;
        } else {
            zoomIn = TRUE;
        }
        mapData78 = 1;
        mapZoomSetGoalData11 = mapZoomSetGoalData21;
        mapZoomSetGoalData12 = mapZoomSetGoalData22;
        mapZoomSetGoalData13 = mapZoomSetGoalData23;
        mapZoomSetGoalData14 = mapZoomSetGoalData24;
        mapZoomSetGoalData15 = mapZoomSetGoalData25;
        mapZoomClearData = (zoom - Zoom) / ftime;
        mapZoomSetGoalData16 = mapZoomSetGoalData26;
        mapData75 = Zoom;
        mapData76 = zoom;
        mapData77 = time;
        if (!zoomIn) {
            lo = 256.0f / zoom;
            hi = 512.0f - lo;
            if (hi < gx) {
                gx = hi;
            }
            if (gx < lo) {
                gx = lo;
            }
            lo = 224.0f / zoom;
            hi = 448.0f - lo;
            if (hi < gy) {
                gy = hi;
            }
            if (gy < lo) {
                gy = lo;
            }
            mapZoomSetGoalData17 = gx * zoom - 256.0f;
            mapZoomSetGoalData18 = gy * zoom - 224.0f;
            mapZoomSetGoalData19 = -(gx * zoom - zoom * 256.0f);
            mapZoomSetGoalData20 = -((gy * zoom - zoom * 224.0f) * 0.5f);
        } else {
            mapZoomSetGoalData19 = 256.0f - gx;
            mapZoomSetGoalData17 = gx * zoom - gx;
            mapZoomSetGoalData18 = gy * zoom - gy;
            mapZoomSetGoalData20 = (224.0f - gy) * 0.5f;
        }
        mapData73 = Zoom;
        mapData79 = zoom * 512.0f;
        mapData74 = (zoom - Zoom) / ftime;
        mapData80 = zoom * 448.0f;
        mapZoomSetGoalData22 = mapZoomSetGoalData12;
        mapZoomSetGoalData24 = mapZoomSetGoalData14;
        mapZoomSetGoalData21 = mapZoomSetGoalData11;
        mapZoomSetGoalData23 = mapZoomSetGoalData13;
        mapData83 = (zoom * 512.0f - mapZoomSetGoalData13) / ftime;
        mapZoomSetGoalData25 = mapZoomSetGoalData15;
        mapZoomSetGoalData26 = mapZoomSetGoalData16;
        mapZoomSetGoalData9 = Zoom;
        mapZoomSetGoalData10 = zoom;
        mapBgSetScrollData4 = time;
        mapData84 = (zoom * 448.0f - mapZoomSetGoalData14) / ftime;
        mapData85 = (mapZoomSetGoalData19 - mapZoomSetGoalData15) / ftime;
        mapData86 = (mapZoomSetGoalData20 - mapZoomSetGoalData16) / ftime;
        mapData81 = (mapZoomSetGoalData17 - mapZoomSetGoalData11) / ftime;
        mapData82 = (mapZoomSetGoalData18 - mapZoomSetGoalData12) / ftime;
        map_fldPoint3To2O3(mapData59, mapZoomSetGoalData4, mapZoomSetGoalData5, &px, &py);
        mapZoomSetGoalData27 = px;
        mapData87 = px;
        mapZoomSetGoalData28 = py * 2.0f;
        mapData88 = py * 2.0f;
        map_fldPoint3To2O3(x, y, z, &px, &py);
        tx = px;
        ty = py * 2.0f;

        cx = mapZoomSetGoalData27;
        w = mapData73 * 512.0f;
        half = -(w * 0.5f);
        edge = half - (cx * mapData73 - 256.0f);
        if (edge > 0.0f) {
            cx += edge / mapData73;
            edge = half - (cx * mapData73 - 256.0f);
        }
        if (edge + w < 512.0f) {
            cx -= (512.0f - (edge + w)) / mapData73;
        }
        mapData87 = cx;
        mapZoomSetGoalData27 = cx;
        mapBgSetScrollData9 = mapData93;
        mapZoomSetGoalData31 = mapData93;
        mapBgSetScrollData13 = mapData95;
        mapZoomSetGoalData33 = mapData95;

        w = mapZoomSetGoalData10 * 512.0f;
        half = -(w * 0.5f);
        ox = -(tx * mapZoomSetGoalData10);
        ex = half - (tx * mapZoomSetGoalData10 - 256.0f);
        if (ex > 0.0f) {
            tx += ex / mapZoomSetGoalData10;
            ox = -(tx * mapZoomSetGoalData10);
            ex = half - (tx * mapZoomSetGoalData10 - 256.0f);
        }
        if (ex + w < 512.0f) {
            tx -= (512.0f - (ex + w)) / mapZoomSetGoalData10;
            ox = -(tx * mapZoomSetGoalData10);
            ex = half - (tx * mapZoomSetGoalData10 - 256.0f);
        }
        mapZoomSetGoalData29 = tx;
        mapData89 = ex;
        mapData91 = ox;

        cy = mapZoomSetGoalData28;
        w = mapData73 * 448.0f;
        half = -(w * 0.5f);
        edge = half - (cy * mapData73 - 224.0f);
        if (edge > 0.0f) {
            cy += edge / mapData73;
            edge = half - (cy * mapData73 - 224.0f);
        }
        if (edge + w < 448.0f) {
            cy -= (448.0f - (edge + w)) / mapData73;
        }
        mapData88 = cy;
        mapZoomSetGoalData28 = cy;
        mapBgSetScrollData10 = mapData94;
        mapZoomSetGoalData32 = mapData94;
        mapBgSetScrollData14 = mapData96;
        mapZoomSetGoalData34 = mapData96;

        w = mapZoomSetGoalData10 * 448.0f;
        half = -(w * 0.5f);
        oy = -(ty * mapZoomSetGoalData10) * 0.5f;
        ey = half - (ty * mapZoomSetGoalData10 - 224.0f);
        if (ey > 0.0f) {
            ty += ey / mapZoomSetGoalData10;
            oy = -(ty * mapZoomSetGoalData10) * 0.5f;
            ey = half - (ty * mapZoomSetGoalData10 - 224.0f);
        }
        if (ey + w < 448.0f) {
            ty -= (448.0f - (ey + w)) / mapZoomSetGoalData10;
            oy = -(ty * mapZoomSetGoalData10) * 0.5f;
            ey = half - (ty * mapZoomSetGoalData10 - 224.0f);
        }
        mapData90 = ey;
        mapZoomSetGoalData30 = ty;
        mapData92 = oy;

        mapBgSetScrollData7 = (tx - cx) / ftime;
        mapBgSetScrollData8 = (ty - cy) / ftime;
        mapBgSetScrollData11 = (ex - mapData93) / ftime;
        mapBgSetScrollData12 = (ey - mapData94) / ftime;
        mapBgSetScrollData15 = (ox - mapData95) / ftime;
        mapBgSetScrollData16 = (oy - mapData96) / ftime;
    }
}
#else
INCLUDE_ASM(const s32, "code_0013CDE0", mapZoomSetGoal);
#endif

void mapZoomClear(void) {
    mapData75 = 1.0f;
    mapData76 = 1.0f;
    mapZoomClearData = 0.0f;
    mapData77 = 0;
    mapData78 = 0;
    mapZoomSetGoalData11 = 0.0f;
    mapZoomSetGoalData12 = 0.0f;
    mapZoomSetGoalData13 = 512.0f;
    mapZoomSetGoalData14 = 448.0f;
    mapZoomSetGoalData15 = 0.0f;
    mapZoomSetGoalData16 = 0.0f;
    mapZoomSetGoalData17 = 0.0f;
    mapZoomSetGoalData18 = 0.0f;
    mapData79 = 512.0f;
    mapData80 = 448.0f;
    mapZoomSetGoalData19 = 0.0f;
    mapZoomSetGoalData20 = 0.0f;
    mapZoomSetGoalData21 = 0.0f;
    mapZoomSetGoalData22 = 0.0f;
    mapZoomSetGoalData23 = 512.0f;
    mapZoomSetGoalData24 = 448.0f;
    mapZoomSetGoalData25 = 0.0f;
    mapZoomSetGoalData26 = 0.0f;
    mapData81 = 0.0f;
    mapData82 = 0.0f;
    mapData83 = 0.0f;
    mapData84 = 0.0f;
    mapData85 = 0.0f;
    mapData86 = 0.0f;
    Zoom = 1.0f;
    mapData73 = 1.0f;
    mapZoomSetGoalData9 = 1.0f;
    mapZoomSetGoalData10 = 1.0f;
    mapData74 = 0.0f;
    mapBgSetScrollData4 = 0;
    tzoom_event = 0;
    mapZoomSetGoalData = mapData59 = mapZoomSetGoalData6 = mapZoomClearData;
    mapZoomSetGoalData2 = mapZoomSetGoalData4 = mapZoomSetGoalData7 = mapZoomClearData;
    mapZoomSetGoalData3 = mapZoomSetGoalData5 = mapZoomSetGoalData8 = mapZoomClearData;
    mapZoomSetGoalData27 = mapZoomClearData;
    mapZoomSetGoalData28 = mapZoomClearData;
    mapZoomSetGoalData29 = mapZoomClearData;
    mapZoomSetGoalData30 = mapZoomClearData;
    mapData87 = mapZoomClearData;
    mapData88 = mapZoomClearData;
    mapBgSetScrollData7 = mapZoomClearData;
    mapBgSetScrollData8 = mapZoomClearData;
    mapZoomSetGoalData31 = mapZoomClearData;
    mapZoomSetGoalData32 = mapZoomClearData;
    mapData89 = mapZoomClearData;
    mapData90 = mapZoomClearData;
    mapBgSetScrollData9 = mapZoomClearData;
    mapBgSetScrollData10 = mapZoomClearData;
    mapBgSetScrollData11 = mapZoomClearData;
    mapBgSetScrollData12 = mapZoomClearData;
    mapZoomSetGoalData33 = mapZoomClearData;
    mapZoomSetGoalData34 = mapZoomClearData;
    mapData91 = mapZoomClearData;
    mapData92 = mapZoomClearData;
    mapBgSetScrollData13 = mapZoomClearData;
    mapBgSetScrollData14 = mapZoomClearData;
    mapBgSetScrollData15 = mapZoomClearData;
    mapBgSetScrollData16 = mapZoomClearData;
    fscroll_mode = 0;
    mapData61 = 0;
    mapBgSetScrollData3 = 0;
    mapData62 = mapData65 = mapData68 = mapZoomClearData;
    mapData63 = mapData66 = mapData69 = mapZoomClearData;
    mapData64 = mapData67 = mapData70 = mapZoomClearData;
}

/* NON_MATCHING: 10 of 32 words match.  */
#ifdef NON_MATCHING
void mapBgFScrollOn(f32 x, f32 y, f32 z, s32 hokan) {
    if (hokan == 0) {
        mapData62 = x;
        fscroll_mode = 1;
        mapData64 = z;
        mapData61 = 0;
        mapBgSetScrollData3 = 0;
        mapData68 = x;
        mapData65 = x;
        mapData69 = y;
        mapData66 = y;
        mapData70 = z;
        mapData67 = z;
        mapData63 = y;
    } else {
        fscroll_mode = 1;
        mapData62 = mapData59;
        mapData63 = mapZoomSetGoalData4;
        mapData64 = mapZoomSetGoalData5;
        mapData68 = x;
        mapData69 = y;
        mapData70 = z;
        mapData61 = hokan;
        mapData65 = x;
        mapData66 = y;
        mapData67 = z;
        mapBgSetScrollData3 = hokan;
    }
}
#else
INCLUDE_ASM(const s32, "code_0013CDE0", mapBgFScrollOn);
#endif

void sndSePlay_sndAt3(s32 hokan) {
    f32 zero;

    if (hokan == 0) {
        fscroll_mode = 0;
        zero = 0.0f;
        mapData61 = 0;
        mapBgSetScrollData3 = 0;
        mapData68 = zero;
        mapData65 = zero;
        mapData62 = zero;
        mapData69 = zero;
        mapData66 = zero;
        mapData63 = zero;
        mapData70 = zero;
        mapData67 = zero;
        mapData64 = zero;
    } else {
        fscroll_mode = 2;
        mapData61 = hokan;
        mapData65 = mapData59;
        mapData66 = mapZoomSetGoalData4;
        mapData67 = mapZoomSetGoalData5;
        mapBgSetScrollData3 = hokan;
        mapData62 = mapData59;
        mapData63 = mapZoomSetGoalData4;
        mapData64 = mapZoomSetGoalData5;
        mapData68 = 0.0f;
        mapData69 = 0.0f;
        mapData70 = 0.0f;
    }
}

void setCngCameraDir(void) {
    mapBgSetScrollData20 = 1;
}

/* NON_MATCHING: 164 of 1249 words match.  */
#ifdef NON_MATCHING
void mapBgSetScroll(f32 x, f32 y, f32 z, s32 mode) {
    f32 x2;
    f32 y2;
    f32 t;
    s32 cam;
    f32 zoom;
    f32 w;
    f32 half;
    f32 edge;
    f32 d;
    f32 lo;
    f32 hi;
    f32 oldX;
    f32 oldY;
    f32 sx;
    f32 sy;
    f32 ox;
    f32 oy;

    bg_scroll_last_x = x;
    mapData71 = y;
    mapData72 = z;
    if (dds3BgSetScrollSub()) {
        return;
    }
    if (tzoom_event) {
        t = (f32)tzoom_event_cnt / (f32)tzoom_event_time;
        z = mapZoomSetGoalData5 = mapZoomSetGoalData3 * t + mapZoomSetGoalData8 * (1.0f - t);
        x = mapData59 = mapZoomSetGoalData * t + mapZoomSetGoalData6 * (1.0f - t);
        y = mapZoomSetGoalData4 = mapZoomSetGoalData2 * t + mapZoomSetGoalData7 * (1.0f - t);
        if (tzoom_event_cnt > 0) {
            tzoom_event_cnt--;
        } else {
            tzoom_event_cnt = 0;
        }
    }
    if (fscroll_mode == 1) {
        t = (f32)mapBgSetScrollData3 / (f32)mapData61;
        z = mapData67 = mapData64 * t + mapData70 * (1.0f - t);
        x = mapData65 = mapData62 * t + mapData68 * (1.0f - t);
        y = mapData66 = mapData63 * t + mapData69 * (1.0f - t);
        if (mapBgSetScrollData3 > 0) {
            mapBgSetScrollData3--;
        } else {
            mapBgSetScrollData3 = 0;
        }
    }
    if (fscroll_mode == 2) {
        t = (f32)mapBgSetScrollData3 / (f32)mapData61;
        z = mapData67 = mapData64 * t + z * (1.0f - t);
        x = mapData65 = mapData62 * t + x * (1.0f - t);
        y = mapData66 = mapData63 * t + y * (1.0f - t);
        if (mapBgSetScrollData3 > 0) {
            mapBgSetScrollData3--;
        } else {
            mapBgSetScrollData3 = 0;
            fscroll_mode = 0;
        }
    }
    if (!mapWork4 && !fldDbg.hitViewOn) {
        return;
    }
    mapData59 = x;
    mapZoomSetGoalData4 = y;
    mapZoomSetGoalData5 = z;
    mapCutSet_From_IdxData = x;
    mapCutSet_From_IdxData2 = y;
    mapCutSet_From_IdxData3 = z;

    switch (fldDbg.camOn) {
    case 0:
        cam = fldGlobalBase.camId;
        break;
    case 2:
        cam = 1;
        break;
    case 3:
        cam = 2;
        break;
    case 4:
        cam = 3;
        break;
    case 1:
    default:
        cam = 0;
        break;
    }

    switch (cam) {
    case 0:
        dds3ScreenParam.height = 448.0f;
        sdf3DScreenAdjust = 1;
        sdf3DScreenAdjustScaleY = 1.0f;
        mapSKYBGEDITData = 0.0f;
        mapBgSetScrollData18 = 0;
        sdf3DScreenAdjustScaleX = 1.0f;
        oldX = mapData87;
        oldY = mapData88;
        if (mapBgSetScrollData4 > 0) {
            zoom = mapData73 += mapData74;
            sx = mapBgSetScrollData9 += mapBgSetScrollData11;
            sy = mapBgSetScrollData10 += mapBgSetScrollData12;
            ox = mapBgSetScrollData13 += mapBgSetScrollData15;
            mapBgSetScrollData4--;
            oy = mapBgSetScrollData14 += mapBgSetScrollData16;
            mapData87 += mapBgSetScrollData7;
            mapData88 += mapBgSetScrollData8;
        } else {
            zoom = mapZoomSetGoalData10;
            sx = mapData89;
            sy = mapData90;
            ox = mapData91;
            oy = mapData92;
            mapData87 = mapZoomSetGoalData29;
            mapData88 = mapZoomSetGoalData30;
            mapData73 = zoom;
            mapBgSetScrollData4 = 0;
            mapBgSetScrollData9 = sx;
            mapBgSetScrollData10 = sy;
            mapBgSetScrollData13 = ox;
            mapBgSetScrollData14 = oy;
            if (tzoom_event && zoom < 1.001f) {
                tzoom_event = 0;
            }
        }
        if (zoom < 0.501f) {
            mapBgSetScrollData5 = 0.0f;
        } else {
            mapBgSetScrollData5 = oldX;
        }
        if (zoom < 1.001f) {
            mapBgSetScrollData6 = 0.0f;
        } else {
            mapBgSetScrollData6 = oldY;
        }
        mapBgSetScrollData17 = sx;
        mapData94 = sy;
        sdf3DScreenAdjustOffsetX = ox;
        sdf3DScreenAdjustOffsetY = oy;
        Zoom = zoom;
        sdf3DScreenAdjustScaleX *= zoom;
        sdf3DScreenAdjustScaleY *= zoom;
        mapData93 = sx;
        mapData95 = ox;
        mapData96 = oy;
        mapBgSetScrollData19 = sx;
        break;

    case 1:
        if (mapBgSetScrollData4 > 0) {
            mapData73 += mapData74;
            mapBgSetScrollData4--;
            mapData87 += mapBgSetScrollData7;
            mapData88 += mapBgSetScrollData8;
            mapBgSetScrollData9 += mapBgSetScrollData11;
            mapBgSetScrollData10 += mapBgSetScrollData12;
            mapBgSetScrollData13 += mapBgSetScrollData15;
            mapBgSetScrollData14 += mapBgSetScrollData16;
        } else {
            mapData87 = mapZoomSetGoalData29;
            mapData88 = mapZoomSetGoalData30;
            mapBgSetScrollData9 = mapData89;
            mapBgSetScrollData10 = mapData90;
            mapBgSetScrollData13 = mapData91;
            mapBgSetScrollData14 = mapData92;
            mapData73 = mapZoomSetGoalData10;
            mapBgSetScrollData4 = 0;
            if (tzoom_event && mapZoomSetGoalData10 < 1.001f) {
                tzoom_event = 0;
            }
        }
        map_fldPoint3To2O2(x, y, z, &x2, &y2);
        if (mapData73 >= 1.001f) {
            mapBgSetScrollData5 = mapData87;
            mapBgSetScrollData6 = mapData88;
        } else if (!mapBgSetScrollData20) {
            d = x2 - 2048.0f;
            if (d < -32.0f) {
                mapBgSetScrollData5 += (d + 32.0f) / 2.0f;
            } else if (d > 32.0f) {
                mapBgSetScrollData5 += (d - 32.0f) / 2.0f;
            }
            d = (y2 - 2048.0f) + (y2 - 2048.0f);
            if (d < -32.0f) {
                mapBgSetScrollData6 += (d + 32.0f) / 2.0f;
            } else if (d > 32.0f) {
                mapBgSetScrollData6 += (d - 32.0f) / 2.0f;
            }
        } else {
            d = x2 - 2048.0f;
            if (d < -32.0f) {
                mapBgSetScrollData5 += d + 32.0f;
            } else if (d > 32.0f) {
                mapBgSetScrollData5 += d - 32.0f;
            }
            d = (y2 - 2048.0f) + (y2 - 2048.0f);
            if (d < -32.0f) {
                mapBgSetScrollData6 += d + 32.0f;
            } else if (d > 32.0f) {
                mapBgSetScrollData6 += d - 32.0f;
            }
        }
        dds3ScreenParam.height = 448.0f;
        sdf3DScreenAdjust = 1;
        sdf3DScreenAdjustScaleY = 2.0f;
        mapBgSetScrollData20 = 0;
        mapSKYBGEDITData = 0.0f;
        mapBgSetScrollData18 = 0;
        sdf3DScreenAdjustScaleX = 2.0f;
        if (mapData73 < 0.501f) {
            mapBgSetScrollData5 = 0.0f;
        }
        if (mapData73 < 1.001f) {
            mapBgSetScrollData6 = 0.0f;
            w = mapData73 * 1024.0f;
            half = -(w * 0.5f);
            edge = half - (mapBgSetScrollData5 * mapData73 - 256.0f);
            if (mapData73 <= 0.5f) {
                mapData93 = edge;
                mapData95 = -(mapBgSetScrollData5 * mapData73);
            } else {
                lo = 0.0f;
                if (edge + w < 512.0f) {
                    lo = 512.0f - (edge + w);
                }
                hi = 0.0f;
                if (edge > 0.0f) {
                    hi = edge;
                }
                mapBgSetScrollData5 = (mapBgSetScrollData5 - lo / mapData73) + hi / mapData73;
                mapData95 = -(mapBgSetScrollData5 * mapData73);
                mapData93 = half - (mapBgSetScrollData5 * mapData73 - 256.0f);
            }
            w = mapData73 * 448.0f;
            half = -(w * 0.5f);
            edge = half - (mapBgSetScrollData6 * mapData73 - 224.0f);
            if (mapData73 <= 1.0f) {
                mapData94 = edge;
                mapData96 = -(mapBgSetScrollData6 * mapData73) * 0.5f;
            } else {
                lo = 0.0f;
                if (edge + w < 448.0f) {
                    lo = 448.0f - (edge + w);
                }
                hi = 0.0f;
                if (edge > 0.0f) {
                    hi = edge;
                }
                mapBgSetScrollData6 = (mapBgSetScrollData6 - lo / mapData73) + hi / mapData73;
                mapData94 = half - (mapBgSetScrollData6 * mapData73 - 224.0f);
                mapData96 = -(mapBgSetScrollData6 * mapData73) * 0.5f;
            }
        } else {
            mapData93 = mapBgSetScrollData9;
            mapData95 = mapBgSetScrollData13;
            mapData94 = mapBgSetScrollData10;
            mapData96 = mapBgSetScrollData14;
        }
        mapBgSetScrollData17 = mapData93;
        sdf3DScreenAdjustOffsetX = mapData95;
        sdf3DScreenAdjustOffsetY = mapData96;
        sdf3DScreenAdjustScaleX *= mapData73;
        sdf3DScreenAdjustScaleY *= mapData73;
        Zoom = mapData73;
        mapBgSetScrollData19 = mapData93;
        break;

    case 2:
        if (mapBgSetScrollData4 > 0) {
            mapData73 += mapData74;
            mapBgSetScrollData4--;
            mapData87 += mapBgSetScrollData7;
            mapData88 += mapBgSetScrollData8;
            mapBgSetScrollData9 += mapBgSetScrollData11;
            mapBgSetScrollData10 += mapBgSetScrollData12;
            mapBgSetScrollData13 += mapBgSetScrollData15;
            mapBgSetScrollData14 += mapBgSetScrollData16;
        } else {
            mapData87 = mapZoomSetGoalData29;
            mapData88 = mapZoomSetGoalData30;
            mapBgSetScrollData9 = mapData89;
            mapBgSetScrollData10 = mapData90;
            mapBgSetScrollData13 = mapData91;
            mapBgSetScrollData14 = mapData92;
            mapData73 = mapZoomSetGoalData10;
            mapBgSetScrollData4 = 0;
            if (tzoom_event && mapZoomSetGoalData10 < 1.001f) {
                tzoom_event = 0;
            }
        }
        mapBgSetScrollData19 = 0.0f;
        mapBgSetScrollData17 = 0;
        fldPoint3To2(&x2, &y2, x, y, z);
        zoom = mapData73;
        if (zoom < 1.001f) {
            if (!mapBgSetScrollData20) {
                d = x2 - 2048.0f;
                if (d < -32.0f) {
                    mapBgSetScrollData5 += (d + 32.0f) / 2.0f;
                } else if (d > 32.0f) {
                    mapBgSetScrollData5 += (d - 32.0f) / 2.0f;
                }
                d = (y2 - 2048.0f) + (y2 - 2048.0f);
                if (d < -32.0f) {
                    mapBgSetScrollData6 += (d + 32.0f) / 2.0f;
                } else if (d > 32.0f) {
                    mapBgSetScrollData6 += (d - 32.0f) / 2.0f;
                }
            } else {
                d = x2 - 2048.0f;
                if (d < -32.0f) {
                    mapBgSetScrollData5 += d + 32.0f;
                } else if (d > 32.0f) {
                    mapBgSetScrollData5 += d - 32.0f;
                }
                d = (y2 - 2048.0f) + (y2 - 2048.0f);
                if (d < -32.0f) {
                    mapBgSetScrollData6 += d + 32.0f;
                } else if (d > 32.0f) {
                    mapBgSetScrollData6 += d - 32.0f;
                }
            }
        }
        mapBgSetScrollData20 = 0;
        if (mapData73 < 1.001f) {
            w = mapData73 * 512.0f;
            half = -(w * 0.5f);
            edge = half - (mapBgSetScrollData5 * mapData73 - 256.0f);
            if (mapData73 <= 0.5f) {
                mapData93 = edge;
                mapData95 = -(mapBgSetScrollData5 * mapData73);
            } else {
                lo = 0.0f;
                if (edge + w < 512.0f) {
                    lo = 512.0f - (edge + w);
                }
                hi = 0.0f;
                if (edge > 0.0f) {
                    hi = edge;
                }
                mapBgSetScrollData5 = (mapBgSetScrollData5 - lo / mapData73) + hi / mapData73;
                mapData95 = -(mapBgSetScrollData5 * mapData73);
                mapData93 = half - (mapBgSetScrollData5 * mapData73 - 256.0f);
            }
            half = -(mapData73 * 896.0f * 0.5f);
            edge = half - (mapBgSetScrollData6 * mapData73 - 224.0f);
            if (mapData73 <= 0.5f) {
                mapData94 = edge;
                mapData96 = -(mapBgSetScrollData6 * mapData73) * 0.5f + mapData73 * 32.0f;
            } else {
                lo = 0.0f;
                if (edge + mapData73 * 896.0f < 448.0f) {
                    lo = 448.0f - (edge + mapData73 * 896.0f);
                }
                hi = 0.0f;
                if (mapData73 * -64.0f < edge) {
                    hi = mapData73 * -64.0f - edge;
                }
                mapBgSetScrollData6 = (mapBgSetScrollData6 - lo / mapData73) - hi / mapData73;
                mapData94 = half - (mapBgSetScrollData6 * mapData73 - 224.0f);
                mapData96 = -(mapBgSetScrollData6 * mapData73) * 0.5f + mapData73 * 32.0f;
            }
        } else {
            mapData93 = mapBgSetScrollData9;
            mapData95 = mapBgSetScrollData13;
            mapData94 = mapBgSetScrollData10;
            mapData96 = mapBgSetScrollData14;
        }
        mapBgSetScrollData17 = mapData93;
        dds3ScreenParam.height = 512.0f;
        sdf3DScreenAdjust = 1;
        sdf3DScreenAdjustOffsetX = mapData95;
        sdf3DScreenAdjustOffsetY = mapData96;
        sdf3DScreenAdjustScaleY = mapData73 + mapData73;
        Zoom = zoom;
        mapBgSetScrollData19 = mapData93;
        mapSKYBGEDITData = 0.0f;
        mapBgSetScrollData18 = 0;
        sdf3DScreenAdjustScaleX = mapData73 + mapData73;
        break;

    case 3:
        if (mapBgSetScrollData4 > 0) {
            mapData73 += mapData74;
            mapBgSetScrollData4--;
            mapData87 += mapBgSetScrollData7;
            mapData88 += mapBgSetScrollData8;
            mapBgSetScrollData9 += mapBgSetScrollData11;
            mapBgSetScrollData10 += mapBgSetScrollData12;
            mapBgSetScrollData13 += mapBgSetScrollData15;
            mapBgSetScrollData14 += mapBgSetScrollData16;
        } else {
            mapData87 = mapZoomSetGoalData29;
            mapData88 = mapZoomSetGoalData30;
            mapBgSetScrollData9 = mapData89;
            mapBgSetScrollData10 = mapData90;
            mapBgSetScrollData13 = mapData91;
            mapBgSetScrollData14 = mapData92;
            mapData73 = mapZoomSetGoalData10;
            mapBgSetScrollData4 = 0;
            if (tzoom_event && mapZoomSetGoalData10 < 1.001f) {
                tzoom_event = 0;
            }
        }
        mapBgSetScrollData19 = 0.0f;
        mapBgSetScrollData17 = 0;
        fldPoint3To2(&x2, &y2, x, y, z);
        zoom = mapData73;
        if (zoom >= 1.001f) {
            mapBgSetScrollData5 = mapData87;
            mapBgSetScrollData6 = mapData88;
        } else if (!mapBgSetScrollData20) {
            d = x2 - 2048.0f;
            if (d < -32.0f) {
                mapBgSetScrollData5 += (d + 32.0f) / 2.0f;
            } else if (d > 32.0f) {
                mapBgSetScrollData5 += (d - 32.0f) / 2.0f;
            }
            d = (y2 - 2048.0f) + (y2 - 2048.0f);
            if (d < -32.0f) {
                mapBgSetScrollData6 += (d + 32.0f) / 2.0f;
            } else if (d > 32.0f) {
                mapBgSetScrollData6 += (d - 32.0f) / 2.0f;
            }
        } else {
            d = x2 - 2048.0f;
            if (d < -32.0f) {
                mapBgSetScrollData5 += d + 32.0f;
            } else if (d > 32.0f) {
                mapBgSetScrollData5 += d - 32.0f;
            }
            d = (y2 - 2048.0f) + (y2 - 2048.0f);
            if (d < -32.0f) {
                mapBgSetScrollData6 += d + 32.0f;
            } else if (d > 32.0f) {
                mapBgSetScrollData6 += d - 32.0f;
            }
        }
        mapBgSetScrollData20 = 0;
        if (mapData73 < 1.001f) {
            w = mapData73 * 1024.0f;
            half = -(w * 0.5f);
            edge = half - (mapBgSetScrollData5 * mapData73 - 256.0f);
            if (mapData73 <= 0.5f) {
                mapData93 = edge;
                mapData95 = -(mapBgSetScrollData5 * mapData73);
            } else {
                lo = 0.0f;
                if (edge + w < 512.0f) {
                    lo = 512.0f - (edge + w);
                }
                hi = 0.0f;
                if (edge > 0.0f) {
                    hi = edge;
                }
                mapBgSetScrollData5 = (mapBgSetScrollData5 - lo / mapData73) + hi / mapData73;
                mapData95 = -(mapBgSetScrollData5 * mapData73);
                mapData93 = half - (mapBgSetScrollData5 * mapData73 - 256.0f);
            }
            half = -(mapData73 * 896.0f * 0.5f);
            edge = half - (mapBgSetScrollData6 * mapData73 - 224.0f);
            if (mapData73 <= 0.5f) {
                mapData94 = edge;
                mapData96 = -(mapBgSetScrollData6 * mapData73) * 0.5f + mapData73 * 32.0f;
            } else {
                lo = 0.0f;
                if (edge + mapData73 * 896.0f < 448.0f) {
                    lo = 448.0f - (edge + mapData73 * 896.0f);
                }
                hi = 0.0f;
                if (mapData73 * -64.0f < edge) {
                    hi = mapData73 * -64.0f - edge;
                }
                mapBgSetScrollData6 = (mapBgSetScrollData6 - lo / mapData73) - hi / mapData73;
                mapData94 = half - (mapBgSetScrollData6 * mapData73 - 224.0f);
                mapData96 = -(mapBgSetScrollData6 * mapData73) * 0.5f + mapData73 * 32.0f;
            }
        } else {
            mapData93 = mapBgSetScrollData9;
            mapData95 = mapBgSetScrollData13;
            mapData94 = mapBgSetScrollData10;
            mapData96 = mapBgSetScrollData14;
        }
        mapBgSetScrollData17 = mapData93;
        dds3ScreenParam.height = 512.0f;
        sdf3DScreenAdjust = 1;
        sdf3DScreenAdjustOffsetX = mapData95;
        sdf3DScreenAdjustOffsetY = mapData96;
        sdf3DScreenAdjustScaleY = mapData73 + mapData73;
        Zoom = zoom;
        mapBgSetScrollData19 = mapData93;
        mapSKYBGEDITData = 0.0f;
        mapBgSetScrollData18 = 0;
        sdf3DScreenAdjustScaleX = mapData73 + mapData73;
        break;
    }

    mapSKYBGEDITData2 = 0.0f;
    if (dds3QuakeFlag) {
        sdf3DScreenAdjust = 1;
        sdf3DScreenAdjustOffsetX += gMapScrollReset[0];
        mapSKYBGEDITData3 = gMapScrollReset[1] + mapSKYBGEDITData2;
        mapSKYBGEDITData2 = gMapScrollReset[0] + mapSKYBGEDITData2;
        sdf3DScreenAdjustOffsetY += gMapScrollReset[1] * 0.5f;
    } else {
        mapSKYBGEDITData3 = mapSKYBGEDITData2;
    }
}
#else
INCLUDE_ASM(const s32, "code_0013CDE0", mapBgSetScroll);
#endif

INCLUDE_ASM(const s32, "code_0013CDE0", mapLeaf11);

void mapBgFree2(void) {
    fldCallMemFreeDelayedDirect4();
    mapCallMemFreeDirect();
    if (fldGlobalBase.word30 != NULL) {
        sdfTexFreeDelayed(fldGlobalBase.word30);
        fldGlobalBase.word44 = -1;
        fldGlobalBase.word30 = NULL;
    }
    mapBgSetScrollReset();
    dds3FilterClear();
    fldGlobalBase.hitMatterIndex = 0;
    fldGlobalBase.cutIdxPrev = 0;
    fldGlobalBase.word4C = 0;
}

void mapAutomapFldClr(s32 field) {
    if (fldGlobalBase.word4C == 1) {
        fldGlobalBase.word4C = 2;
    }
    fldJmsQuestMapLoad_fld(fldGlobalBase.amSBlock, field);
    mapCallFilterSet(field);
    if (fldGlobalBase.word30 != NULL) {
        sdfTexFreeDelayed(fldGlobalBase.word30);
        fldGlobalBase.word44 = -1;
        fldGlobalBase.word30 = NULL;
    }
    fldGlobalBase.word44 = field;
    fldGlobalBase.word30 = NULL;
}

void mapCutSetTblID(s32 cut) {
    if (fldGlobalBase.word1DC == 1) {
        if (cut == 27 || cut == 33) {
            fldGlobalBase.cutIdx = 33;
            return;
        }
        fldGlobalBase.word1DC = 0;
    }
    fldGlobalBase.cutIdx = cut;
}

INCLUDE_ASM(const s32, "code_0013CDE0", mapCutSetTblID2);

/* NON_MATCHING: 4 of 34 words match.  */
void mapCutBgLoad(void) {
    s32 cut;

    cut = fldGlobalBase.cutIdx;
    if (cut < 127 && cut != 0 && bg_tex_load_last != cut) {
        bg_tex_load_rec = -1;
        bg_lay_load_rec = -1;
        if (mapData5[fldGlobalBase.amSBlock][cut] != 0) {
            mapAutomapFldClr(cut);
            bg_tex_load_last = fldGlobalBase.cutIdx;
        } else {
            mapBgFree2();
            FAKEMATCH_ASM_LIVE(cut);
        }
    }
}

INCLUDE_ASM(const s32, "code_0013CDE0", mapCutBgLoad_NB);

void fldAllTblPacLoad(void) {
    fileRequest_t *pac;
    sdfPacDecodeResrc_t *node;
    s32 i;
    sdfMemHandle_t *handle;
    void *top;

    i = 0;
    pac = pacLoad(fldStrMapAllAlltblLb);
    pacSync(pac);
    for (node = pac->pac.resrcHead; node != NULL; node = node->next, i++) {
        switch (i) {
        case 2:
            handle = node->resrc;
            top = sdfMemGetPtr(handle);
            memcpy(fldWork6, top, sdfMemGetSize(handle));
            sdfMemFreeDirect(node->resrc);
            break;
        case 3:
            handle = node->resrc;
            top = sdfMemGetPtr(handle);
            memcpy(LightDataTbl, top, sdfMemGetSize(handle));
            sdfMemFreeDirect(node->resrc);
            break;
        case 0:
            handle = node->resrc;
            top = sdfMemGetPtr(handle);
            memcpy(mapEXFloorData, top, sdfMemGetSize(handle));
            sdfMemFreeDirect(node->resrc);
            break;
        case 1:
            handle = node->resrc;
            top = sdfMemGetPtr(handle);
            memcpy(fldAllTblPacLoadData, top, sdfMemGetSize(handle));
            sdfMemFreeDirect(node->resrc);
            break;
        default:
            sdfMemFreeDirect(node->resrc);
            break;
        }
    }
    pacFileHandleFree_(pac);
}

INCLUDE_RODATA(const s32, "code_0013CDE0", fldStrMapAllAlltblLb);

