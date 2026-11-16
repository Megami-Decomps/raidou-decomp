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
#include "mapData.h"
#include "mapLbWork.h"
#include "mdlManager.h"
#include "fldDbg.h"
#include "sdfQuat.h"
#include "sdfMath.h"
#include "sdk_libm.h"
#include "sdfMemory.h"
#include "dds3KernelCore.h"
#include "sdf3D.h"
#include "EventBit.h"
#include "sdfTex.h"
#include "effManager.h"
#include "dds3KernelDraw.h"
#include "dds3CameraObjectBasic.h"
#include "sbtlWork.h"
#include "sdfVu0.h"
#include "code_r1_0010DF68.h"

/* NON_MATCHING: 4 of 34 words match.  */
#ifdef NON_MATCHING
void mapMapSetEnc(s32 encount_id) {
    MpbAreaHead_t *head;
    MpbEncHead_t *enc;
    s32 i;

    mapData112 = NULL;
    pMpbPointNow = NULL;
    mapData113 = NULL;
    if (mapData110 == NULL) {
        return;
    }
    head = mapMapSetAreaData18;
    if (head == NULL) {
        return;
    }
    enc = (MpbEncHead_t *)((u8 *)mapData111 + head->addr_enchead);
    for (i = 0; i < head->num_enchead; i++) {
        if (enc[i].encount_id == encount_id) {
            mapData112 = &enc[i];
            pMpbPointNow = (MpbPoint_t *)((u8 *)mapData111 + enc[i].addr_encpos);
            mapData113 = (MpbWall_t *)((u8 *)mapData111 + enc[i].addr_btlwall);
            return;
        }
    }
}
#else
INCLUDE_ASM(const s32, "code_00153438", mapMapSetEnc);
#endif

/* NON_MATCHING: 4 of 34 words match.  */
#ifdef NON_MATCHING
void mapMapSetForm(s32 form_id) {
    MpbAreaHead_t *head;
    MpbEncHead_t *enc;
    s32 i;

    mapData112 = NULL;
    pMpbPointNow = NULL;
    mapData113 = NULL;
    if (mapData110 == NULL) {
        return;
    }
    head = mapMapSetAreaData18;
    if (head == NULL) {
        return;
    }
    enc = (MpbEncHead_t *)((u8 *)mapData111 + head->addr_enchead);
    for (i = 0; i < head->num_enchead; i++) {
        if (enc[i].encount_id == form_id) {
            mapData112 = &enc[i];
            pMpbPointNow = (MpbPoint_t *)((u8 *)mapData111 + enc[i].addr_encpos);
            mapData113 = (MpbWall_t *)((u8 *)mapData111 + enc[i].addr_btlwall);
            return;
        }
    }
}
#else
INCLUDE_ASM(const s32, "code_00153438", mapMapSetForm);
#endif

void mapEncPointGet(s16 point_id, f32 *x, f32 *y, f32 *z, f32 *r) {
    MpbPoint_t *pt;
    s32 i;
    f32 pos[2];
    f32 py;

    *x = 0.0f;
    *y = 0.0f;
    *z = 0.0f;
    *r = 0.0f;
    if (mapData112 == NULL) {
        return;
    }
    pt = pMpbPointNow;
    for (i = 0; i < mapData112->num_encpos; i++, pt++) {
        if (pt->id == point_id) {
            PositioningGuardSystem(pt->x, pt->y, &pos[0], &pos[1]);
            switch (pt->ofs) {
            case 0:
                pos[0] += 50.0f;
                pos[1] -= 50.0f;
                break;
            case 1:
                break;
            case 2:
                pos[0] += 100.0f;
                break;
            case 3:
                pos[1] -= 100.0f;
                break;
            case 4:
                pos[0] += 100.0f;
                pos[1] -= 100.0f;
                break;
            }
            py = mapGetY(pos[0], pos[1]);
            *x = pos[0];
            *y = py;
            *z = pos[1];
            *r = -mapData20[pt->dir];
            return;
        }
    }
}

void fldSetPlayerTargetData(s32 *ix, s32 *iz, f32 x, f32 z) {
    f32 dz = -(z - mapData104) / 100.0f;

    *ix = (x - mapData103) / 100.0f;
    *iz = dz;
}

INCLUDE_ASM(const s32, "code_00153438", fldGetNowCutID);

void fldProcSequenceSub4(f32 x, f32 z, f32 *outX, f32 *outZ) {
    f32 dz = -(z - mapData104) / 100.0f;

    *outX = (x - mapData103) / 100.0f;
    *outZ = dz;
}

INCLUDE_ASM(const s32, "code_00153438", fldProcSequenceSub5);

void PositioningGuardSystem(s32 ix, s32 iy, f32 *x, f32 *z) {
    *x = mapData103 + (f32)(ix * 100);
    *z = mapData104 - (f32)(iy * 100);
}

INCLUDE_ASM(const s32, "code_00153438", fldWallObjTaskSub);

INCLUDE_ASM(const s32, "code_00153438", mapLeaf21);

INCLUDE_ASM(const s32, "code_00153438", mapFileLoadSub6);

void mapHitWorkCalcSetupSub(f32 *outx, f32 *outz, f32 x, f32 z) {
    s32 ix = (x - mapData103) / 100.0f;
    s32 iz = (z - mapData104) / 100.0f;

    *outx = (f32)(ix * 100) + mapData103;
    *outz = (f32)(iz * 100) + mapData104;
}

s32 mapGetLight(f32 x, f32 z) {
    s32 ix;
    s32 iz;
    s32 w;
    s32 light;

    if (mapData106 == NULL) {
        return 0;
    }
    if (mapData108 == NULL) {
        return 0;
    }
    fldSetPlayerTargetData(&ix, &iz, x, z);
    if (iz < 0) {
        iz = 0;
    }
    if (ix < 0) {
        ix = 0;
    }
    w = mapData106->w;
    if (ix >= w) {
        ix = w - 1;
    }
    if (iz >= mapData106->h) {
        iz = mapData106->h - 1;
    }
    light = mapData108[w * iz + ix].light;
    if (light == 0xFF) {
        light = 0;
    }
    return light;
}

INCLUDE_ASM(const s32, "code_00153438", mapGetFloorLight);

/* NON_MATCHING: 20 of 24 words match.  */
#ifdef NON_MATCHING
s32 mapGetHit(f32 x, f32 z) {
    s32 ix;
    s32 iz;

    fldSetPlayerTargetData(&ix, &iz, x, z);
    if (pMapAreaNow == NULL) {
        return 0;
    }
    if (pMapColmNow == NULL) {
        return 0;
    }
    return (pMapColmNow[pMapAreaNow->w * iz + ix].hit & 7) == 1;
}
#else
INCLUDE_ASM(const s32, "code_00153438", mapGetHit);
#endif

/* NON_MATCHING: 128 of 132 words match.  */
#ifdef NON_MATCHING
s32 mapGetAttr(f32 x, f32 z) {
    s32 ix;
    s32 iz;
    s32 attr;
    s32 i;
    s32 result;

    fldSetPlayerTargetData(&ix, &iz, x, z);
    result = 0;
    if (pMapAreaNow != NULL && pMapColmNow != NULL) {
        attr = pMapColmNow[pMapAreaNow->w * iz + ix].attr;
        for (i = 0; i < 128; i++) {
            if (mapData23[i].use == 0) {
                continue;
            }
            if (fldGlobalBase.summonStop != 0 && mapData23[i].mode == 1) {
                continue;
            }
            if (mapData23[i].floor != -1 && mapData23[i].floor != fldGlobalBase.nameU) {
                continue;
            }
            if (mapData23[i].flag0 != 0) {
                if (mapData23[i].flag0j == 0) {
                    if (evtBitCheck(mapData23[i].flag0) != 0) {
                        continue;
                    }
                } else if (evtBitCheck(mapData23[i].flag0) == 0) {
                    continue;
                }
            }
            if (mapData23[i].flag1 != 0) {
                if (mapData23[i].flag1j == 0) {
                    if (evtBitCheck(mapData23[i].flag1) != 0) {
                        continue;
                    }
                } else if (evtBitCheck(mapData23[i].flag1) == 0) {
                    continue;
                }
            }
            if (ix >= mapData23[i].ix && ix < mapData23[i].ix + mapData23[i].iw && iz >= mapData23[i].iy && iz < mapData23[i].iy + mapData23[i].ih && (mapData23[i].attr & 0xF) != 0) {
                if (mapData23[i].flag != 0) {
                    if (evtBitCheck(mapData23[i].flag) == 0) {
                        attr |= mapData23[i].attr & 0xF;
                    }
                } else {
                    attr |= mapData23[i].attr & 0xF;
                }
            }
        }
        result = attr;
    }
    return result;
}
#else
INCLUDE_ASM(const s32, "code_00153438", mapGetAttr);
#endif

INCLUDE_ASM(const s32, "code_00153438", mapGetAttr_DevilWall);

/* NON_MATCHING: 102 of 104 words match.  */
#ifdef NON_MATCHING
s32 mapGetAttr_Btl(f32 x, f32 z) {
    s32 ix;
    s32 iz;
    s32 attr;
    s32 i;
    s32 result;
    MpbWall_t *wall;

    fldSetPlayerTargetData(&ix, &iz, x, z);
    result = 0;
    if (pMapAreaNow != NULL && pMapColmNow != NULL) {
        attr = pMapColmNow[pMapAreaNow->w * iz + ix].attr;
        if (mapData112 != NULL) {
            wall = mapData113;
            for (i = 0; i < mapData112->num_btlwall; i++, wall++) {
                if (ix >= wall->x && ix < wall->x + wall->w && iz >= wall->y && iz < wall->y + wall->h) {
                    attr |= wall->attr & 0xF;
                }
            }
        }
        for (i = 0; i < 128; i++) {
            if (mapData23[i].use == 0) {
                continue;
            }
            if (ix >= mapData23[i].ix && ix < mapData23[i].ix + mapData23[i].iw && iz >= mapData23[i].iy && iz < mapData23[i].iy + mapData23[i].ih && (mapData23[i].attr & 0xF) != 0) {
                if (mapData23[i].flag == 0) {
                    attr |= mapData23[i].attr & 0xF;
                } else if (evtBitCheck(mapData23[i].flag) == 0) {
                    attr |= mapData23[i].attr & 0xF;
                }
            }
        }
        result = attr;
    }
    return result;
}
#else
INCLUDE_ASM(const s32, "code_00153438", mapGetAttr_Btl);
#endif

s32 fldGetFldBgNameList(f32 x, f32 z) {
    MpbAreaHead_t *head;
    s32 ix;
    s32 iz;

    head = mapMapSetAreaData18;
    if (pMapAreaNow == NULL) {
        return 0;
    }
    if (head == NULL) {
        return 0;
    }
    fldSetPlayerTargetData(&ix, &iz, x, z);
    return mapMapSetAreaData22[head->rnd_encw * iz + ix];
}

INCLUDE_ASM(const s32, "code_00153438", fldRefMapAreaNow);

INCLUDE_ASM(const s32, "code_00153438", mapRefMapAreaNow);

s32 mapGetCut_FXY(f32 x, f32 z) {
    s32 ix;
    s32 iz;
    s32 cut;

    fldSetPlayerTargetData(&ix, &iz, x, z);
    cut = pMapColmNow[pMapAreaNow->w * iz + ix].cut & 0x7F;
    if (fldGlobalBase.word1DC == 1 && cut == 27) {
        cut = 33;
    }
    return cut;
}

s32 mapProcSequenceSub5(f32 x, f32 z) {
    s32 i;

    for (i = 0; i < 64; i++) {
        if (mapData27[i].use != 0 && mapData27[i].x0 < x && x < mapData27[i].x1 && z < mapData27[i].z0 &&
            mapData27[i].z1 < z && mapData27[i].type == 2) {
            return mapData27[i].basyo;
        }
    }
    return 0;
}

/* NON_MATCHING: 120 of 285 words match.  */
#ifdef NON_MATCHING
f32 mapGetFloorY(s32 floor, f32 fx, f32 fz) {
    f32 bx[4];
    f32 by[4];
    f32 bz[4];
    s32 ix;
    s32 iz;
    s32 i;
    s32 fl;
    f32 h;
    f32 y;
    fldMapAreaHead_t *area;
    MapColm_t *colm;

    y = -99999.0f;
    if (floor == -1 || fldGlobalBase.nameU == floor) {
        for (i = 0; i < 32; i++) {
            if (mapData30[i].use != 0) {
                bx[0] = mapData30[i].x0;
                by[0] = mapData30[i].y0;
                bz[0] = mapData30[i].z0;
                bx[1] = mapData30[i].x1;
                by[1] = mapData30[i].y1;
                bz[1] = mapData30[i].z1;
                bx[2] = mapData30[i].x2;
                by[2] = mapData30[i].y2;
                bz[2] = mapData30[i].z2;
                bx[3] = mapData30[i].x3;
                by[3] = mapData30[i].y3;
                bz[3] = mapData30[i].z3;
                h = mapGet4Y(fx, fz, bx, by, bz);
                if (h > -99998.0f) {
                    return h;
                }
            }
        }
    }
    if (floor == -1 || fldGlobalBase.nameU == floor) {
        for (i = 0; i < 64; i++) {
            if (mapData27[i].use == 0) {
                continue;
            }
            if (mapData27[i].x0 < fx && fx < mapData27[i].x1 && fz < mapData27[i].z0 && mapData27[i].z1 < fz) {
                if (mapData27[i].type == 0) {
                    if (mapData27[i].xory == 0) {
                        return mapData27[i].h0 + (fx - mapData27[i].x0) * (mapData27[i].alltaka / mapData27[i].allhaba);
                    }
                    return mapData27[i].h0 + -(fz - mapData27[i].z0) * (mapData27[i].alltaka / mapData27[i].allhaba);
                }
                if (mapData27[i].type == 1) {
                    if (mapData27[i].xory == 0) {
                        return mapData27[i].addh + mapData27[i].oneh * (s32)((fx - mapData27[i].x0) / mapData27[i].haba);
                    }
                    return mapData27[i].addh + mapData27[i].oneh * (s32)(-(fz - mapData27[i].z0) / mapData27[i].haba);
                }
            }
        }
    }
    if (y > -99998.0f) {
        return y;
    }
    fl = fldGlobalBase.nameU;
    if (floor >= 0) {
        fl = floor;
    }
    if (fldGlobalBase.amSBlock == 2 && fl == 0 && fz < -6500.0f) {
        return -110.0f;
    }
    if (floor >= 0 && gSubFloorID == floor) {
        if (pMapAreaDat == NULL) {
            return 0.0f;
        }
        fldSetPlayerTargetData(&ix, &iz, fx, fz);
        return pMapColmDat[pMapAreaDat->w * iz + ix].hight * -5.0f;
    }
    if (pMapAreaNow == NULL) {
        return 0.0f;
    }
    if (floor == -1 || fldGlobalBase.nameU == floor) {
        fldSetPlayerTargetData(&ix, &iz, fx, fz);
        return pMapColmNow[pMapAreaNow->w * iz + ix].hight * -5.0f;
    }
    area = mapMapSetAreaData[floor];
    colm = mapMapSetAreaData2[floor];
    if (area == NULL) {
        return 0.0f;
    }
    fldGetNowCutID(floor, &ix, &iz, fx, fz);
    return colm[area->w * iz + ix].hight * -5.0f;
}
#else
INCLUDE_ASM(const s32, "code_00153438", mapGetFloorY);
#endif

f32 mapGetY(f32 x, f32 z) {
    return mapGetFloorY(-1, x, z);
}

/* NON_MATCHING: 19 of 23 words match.  */
#ifdef NON_MATCHING
s32 mapCallSetPlayerTargetData5(s32 se, mdlObj_t *model, f32 x, f32 z) {
    s32 ix;
    s32 iz;

    fldSetPlayerTargetData(&ix, &iz, x, z);
    if (pMapAreaNow == NULL) {
        return 0;
    }
    if (pMapColmNow == NULL) {
        return 0;
    }
    return (pMapColmNow[pMapAreaNow->w * iz + ix].hit & 0x30) >> 4;
}
#else
INCLUDE_ASM(const s32, "code_00153438", mapCallSetPlayerTargetData5);
#endif

/* NON_MATCHING: 16 of 28 words match.  */
#ifdef NON_MATCHING
s32 mapCallSetPlayerTargetData6(f32 x, f32 z) {
    mapOt2Block_t *block;
    mapOt2Cell_t *cells;
    s32 ix;
    s32 iz;

    fldSetPlayerTargetData(&ix, &iz, x, z);
    block = mapCallSetOt2Data2[fldGlobalBase.nameU];
    if (block != NULL) {
        cells = mapData35[fldGlobalBase.nameU];
        if (cells != NULL) {
            return cells[ix + block->w * iz].id;
        }
    }
    return 0;
}
#else
INCLUDE_ASM(const s32, "code_00153438", mapCallSetPlayerTargetData6);
#endif

/* NON_MATCHING: 128 of 129 words match.  */
#ifdef NON_MATCHING
void mapCutSet_From_Pos(f32 x, f32 y, f32 z) {
    fldMapAreaHead_t *area;
    MapColm_t *colm;
    s32 cut;
    s32 i;
    volatile f32 target[4] QWORD_ALIGNED;
    volatile f32 eye[4] QWORD_ALIGNED;

    datDebugPrintf(mapStrMapCutSetFromPos, (s32)x, (s32)y, (s32)z);
    area = pMapAreaNow;
    colm = pMapColmNow;
    if (area == NULL || colm == NULL) {
        return;
    }
    cut = mapGetCut_FXY(x, z);
    if (bg_tex_load_last != cut) {
        mapCutSetSub(cut);
        mapCutBgLoad();
        bg_tex_load_last = bg_tex_load_rec;
    }
    if (cameraBasic != NULL) {
        mapCutSetCam(cameraBasic, 0);
        dds3CameraObjectBasicFourceCalc(cameraBasic);
    } else {
        mapCutSetCam(mapData97, 0);
        dds3CameraObjectBasicFourceCalc(mapData97);
    }
    sdf3MakeScreenParam(&dds3ScreenParam);
    SDF_VU0_LOADVECTOR(&aViewTarget);
    SDF_VU0_LOADVECTOR_B(&aViewEye);
    SDF_VU0_SUBVECTOR();
    SDF_VU0_NORMALIZE();
    SDF_VU0_SCALEVECTOR(600.0f);
    SDF_VU0_ADDVECTOR();
    SDF_VU0_LOADVECTOR_B(gMapScrollReset);
    SDF_VU0_ADDVECTOR();
    SDF_VU0_STOREVECTOR(target);
    SDF_VU0_LOADVECTOR(&aViewEye);
    SDF_VU0_ADDVECTOR();
    SDF_VU0_STOREVECTOR_NOMEM(eye);
    LookAtMatrix((const void *)target, (const void *)eye, &aViewDown);
    SDF_VU0_STOREMATRIX_NOMEM(&sdfViewMatrix);
    sdf3DSetCamera(&dds3ScreenParam, &sdfViewMatrix);
    for (i = 0; i < 13; i++) {
        sdf3MakeScreenParam(&dds3ScreenParam);
        setCngCameraDir();
        mapBgSetScroll(x, y, z, 0);
    }
    fldGlobalBase.cutState = 0;
    fldGlobalBase.cutIdxPrev = fldGlobalBase.cutIdx;
    mapObjOnOff();
    setCngCameraDir();
}
#else
INCLUDE_ASM(const s32, "code_00153438", mapCutSet_From_Pos);
#endif

INCLUDE_ASM(const s32, "code_00153438", mapCatChkMain);

INCLUDE_ASM(const s32, "code_00153438", mapCatGetNakama);

INCLUDE_ASM(const s32, "code_00153438", mapCallGetNowCutID);

void mapCallSetPlayerTargetData4(void) {
    fldMapAreaHead_t *area;
    MapColm_t *colm;
    s32 ix;
    s32 iz;
    s32 cut;
    volatile f32 target[4] QWORD_ALIGNED;
    volatile f32 eye[4] QWORD_ALIGNED;

    area = pMapAreaNow;
    colm = pMapColmNow;
    if (area == NULL || colm == NULL) {
        return;
    }
    fldSetPlayerTargetData(&ix, &iz, fldGlobalBase.focusX, fldGlobalBase.focusZ);
    if (fldGlobalBase.cutState == 1) {
        fldGlobalBase.cutIdx = colm[area->w * iz + ix].cut & 0x7F;
        if (fldGlobalBase.hitMatterIndex > 0) {
            fldGlobalBase.cutIdx = fldGlobalBase.hitMatterIndex;
        }
        fldGlobalBase.cutState = 2;
        fldGlobalBase.cutIdxPrev = fldGlobalBase.cutIdx;
        mapCutSetSub(fldGlobalBase.cutIdx);
        mapCutBgLoad_NB();
        mapCutSetCam(cameraBasic, 0);
        dds3CameraObjectBasicFourceCalc(cameraBasic);
        sdf3MakeScreenParam(&dds3ScreenParam);
        SDF_VU0_LOADVECTOR(&aViewTarget);
        SDF_VU0_LOADVECTOR_B(&aViewEye);
        SDF_VU0_SUBVECTOR();
        SDF_VU0_NORMALIZE();
        SDF_VU0_SCALEVECTOR(600.0f);
        SDF_VU0_ADDVECTOR();
        SDF_VU0_LOADVECTOR_B(gMapScrollReset);
        SDF_VU0_ADDVECTOR();
        SDF_VU0_STOREVECTOR(target);
        SDF_VU0_LOADVECTOR(&aViewEye);
        SDF_VU0_ADDVECTOR();
        SDF_VU0_STOREVECTOR_NOMEM(eye);
        LookAtMatrix((const void *)target, (const void *)eye, &aViewDown);
        SDF_VU0_STOREMATRIX_NOMEM(&sdfViewMatrix);
        sdf3DSetCamera(&dds3ScreenParam, &sdfViewMatrix);
        sdf3MakeScreenParam(&dds3ScreenParam);
        setCngCameraDir();
        mapBgSetScroll(fldGlobalBase.focusX, fldGlobalBase.focusY, fldGlobalBase.focusZ, 0);
        sdf3MakeScreenParam(&dds3ScreenParam);
        setCngCameraDir();
        mapBgSetScroll(fldGlobalBase.focusX, fldGlobalBase.focusY, fldGlobalBase.focusZ, 0);
        sdf3MakeScreenParam(&dds3ScreenParam);
        setCngCameraDir();
        mapBgSetScroll(fldGlobalBase.focusX, fldGlobalBase.focusY, fldGlobalBase.focusZ, 0);
        sdf3MakeScreenParam(&dds3ScreenParam);
        setCngCameraDir();
        mapBgSetScroll(fldGlobalBase.focusX, fldGlobalBase.focusY, fldGlobalBase.focusZ, 0);
        sdf3MakeScreenParam(&dds3ScreenParam);
        setCngCameraDir();
        mapBgSetScroll(fldGlobalBase.focusX, fldGlobalBase.focusY, fldGlobalBase.focusZ, 0);
        mapObjOnOff();
        setCngCameraDir();
    } else {
        if ((colm[area->w * iz + ix].cut & 0x80) && fldGlobalBase.hitMatterIndex <= 0) {
            return;
        }
        cut = colm[area->w * iz + ix].cut;
        if (fldGlobalBase.word1DC == 1 && cut == 27) {
            cut = 33;
        }
        if (fldGlobalBase.cutIdxPrev != cut && cut != 0) {
            fldGlobalBase.cutState = 1;
        }
        if (fldGlobalBase.hitMatterIndex > 0 && fldGlobalBase.cutIdx == fldGlobalBase.hitMatterIndex) {
            fldGlobalBase.cutState = 0;
        }
    }
}

void mapCutSet_From_Idx(void) {
    fldMapAreaHead_t *area;
    MapColm_t *colm;
    s32 ix;
    s32 iz;
    volatile f32 target[4] QWORD_ALIGNED;
    volatile f32 eye[4] QWORD_ALIGNED;

    area = pMapAreaNow;
    colm = pMapColmNow;
    if (area == NULL || colm == NULL) {
        return;
    }
    fldSetPlayerTargetData(&ix, &iz, mapCutSet_From_IdxData, mapCutSet_From_IdxData3);
    if (fldGlobalBase.cutState != 1) {
        return;
    }
    if (sbtlSbattle4() == 0) {
        return;
    }
    fldGlobalBase.cutIdx = colm[area->w * iz + ix].cut & 0x7F;
    if (fldGlobalBase.hitMatterIndex > 0) {
        fldGlobalBase.cutIdx = fldGlobalBase.hitMatterIndex;
    }
    fldGlobalBase.cutState = 2;
    fldGlobalBase.cutIdxPrev = fldGlobalBase.cutIdx;
    mapCutSetSub(fldGlobalBase.cutIdx);
    mapCutBgLoad_NB();
    mapCutSetCam(mapData97, 0);
    dds3CameraObjectBasicFourceCalc(mapData97);
    sdf3MakeScreenParam(&dds3ScreenParam);
    SDF_VU0_LOADVECTOR(&aViewTarget);
    SDF_VU0_LOADVECTOR_B(&aViewEye);
    SDF_VU0_SUBVECTOR();
    SDF_VU0_NORMALIZE();
    SDF_VU0_SCALEVECTOR(600.0f);
    SDF_VU0_ADDVECTOR();
    SDF_VU0_LOADVECTOR_B(gMapScrollReset);
    SDF_VU0_ADDVECTOR();
    SDF_VU0_STOREVECTOR(target);
    SDF_VU0_LOADVECTOR(&aViewEye);
    SDF_VU0_ADDVECTOR();
    SDF_VU0_STOREVECTOR_NOMEM(eye);
    LookAtMatrix((const void *)target, (const void *)eye, &aViewDown);
    SDF_VU0_STOREMATRIX_NOMEM(&sdfViewMatrix);
    sdf3DSetCamera(&dds3ScreenParam, &sdfViewMatrix);
    sdf3MakeScreenParam(&dds3ScreenParam);
    mapBgSetScroll(mapCutSet_From_IdxData, mapCutSet_From_IdxData2, mapCutSet_From_IdxData3, 0);
    sdf3MakeScreenParam(&dds3ScreenParam);
    mapBgSetScroll(mapCutSet_From_IdxData, mapCutSet_From_IdxData2, mapCutSet_From_IdxData3, 0);
    RestartBattle();
}

INCLUDE_ASM(const s32, "code_00153438", mapCallWarpGetPos);

INCLUDE_ASM(const s32, "code_00153438", mapProcSequenceSub6);

INCLUDE_ASM(const s32, "code_00153438", mapProcSequenceSub7);

INCLUDE_ASM(const s32, "code_00153438", mapCallSetOt7);

INCLUDE_ASM(const s32, "code_00153438", mapCallSetOt8);

void mapCallTexAllocFromHead(void) {
    fldTitleMiniTex2 = sdfTexAllocFromHead(128, 128, 19, 0, 0, 1);
    sdfTexMakeGIFPacket(fldTitleMiniTex2);
    fldTitleMiniWork2 = sdfTexAllocFromHead(128, 128, 20, 0, 0, 1);
    sdfTexMakeGIFPacket(fldTitleMiniWork2);
}
