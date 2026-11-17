#include "common.h"
#include "sbtlWork.h"
#include "mapData.h"
#include "fldPanel.h"
#include "sBtlUnit.h"
#include "kernel_draw.h"
#include "ps2asm.h"
#include "sndAt3.h"
#include "fld.h"
#include "fldProcess.h"
#include "sbtlSub.h"
#include "dds3WorldBasic.h"
#include "sdfDelayedResrc.h"
#include "dds3SceneBasic.h"
#include "basic.h"

INCLUDE_ASM(const s32, "code_0024DF70", sbtlCallBgDrawOnOff);

INCLUDE_ASM(const s32, "code_0024DF70", sbtlMapVisiblerSet);

s32 Draw_sbtlScene(void) {
    dds3Basic_t *scene;
    struct SBTLUNIT_WORK *unit;
    s32 id;

    scene = dds3GetActiveScene();
    if (scene == NULL) {
        return 1;
    }
    dds3VisibleSceneBasic(scene, 1);
    if (!(wk_sbtl->flg & 0x200000)) {
        id = sbtlData91;
        if (sbtlData90 == 0) {
            if (sbtlCallMemset4Data4) {
                sbtlCheckZoom();
                mapBgSetScroll(sbtlCallMemset4Data5, sbtlCallMemset4Data6, sbtlCallMemset4Data7, 0);
                goto draw;
            }
            if (sbtlData86) {
                mapBgSetScroll(sbtlData87, sbtlData88, sbtlData89, 0);
                goto draw;
            }
            id = sbtlGetScrollID();
        }
        sbtlSetScrollID(id);
        unit = sbtlSearchUnit_ID(wk_sbtl->unitList, wk_sbtl->scrollID);
        if (unit != NULL) {
            sbtlCheckZoom();
            mapBgSetScroll(unit->pos.x, unit->pos.y - 100.0f, unit->pos.z, 0);
        }
    } else {
        mapBgSetScrollReset();
    }
draw:
    mapDrawSBtlSceneSub();
    return 1;
}

/* body: matched in store @ 2026-11-16: /home/wsl/.local/state/raidou-decomp-work/clusters/store/raidou1/0024E130.c:1, found */
f32 sbtlFree(f32 x, f32 z) {
    return mapGetY(x, z);
}

INCLUDE_ASM(const s32, "code_0024DF70", sbtlCallDisableMoonDraw);

s32 sbtlSetScrollID(s32 id) {
    wk_sbtl->scrollID = id;
    return 1;
}

INCLUDE_ASM(const s32, "code_0024DF70", sbtlCallBtlGetScrollID);

INCLUDE_ASM(const s32, "code_0024DF70", sbtlRefSbtl2);

/* NON_MATCHING: 3 of 37 words match.  */
#ifdef NON_MATCHING
s32 sbtlRefSbtl31(void) {
    SBTL_WORK *wk = wk_sbtl;
    s32 i;

    if (!(wk->flg & 0x100)) {
        for (i = 0; i < 3; i++) {
            if (dds3LightParam.parallel[i] != NULL) {
                SDF_VU0_LOADVECTOR(&dds3LightParam.parallel[i]->color);
                SDF_VU0_STOREVECTOR(&wk->light[i].color);
                SDF_VU0_LOADVECTOR(&dds3LightParam.parallel[i]->v);
                SDF_VU0_STOREVECTOR(&wk->light[i].v);
                SDF_VU0_LOADVECTOR(&dds3LightObjectAmbient);
                SDF_VU0_STOREVECTOR(&wk->ambient);
            }
        }
        wk->flg |= 0x100;
    }
    return 1;
}
#else
INCLUDE_ASM(const s32, "code_0024DF70", sbtlRefSbtl31);
#endif

INCLUDE_ASM(const s32, "code_0024DF70", sbtlRefSbtl3);

INCLUDE_ASM(const s32, "code_0024DF70", sbtlSetZoomLock);

INCLUDE_ASM(const s32, "code_0024DF70", sbtlResetZoomLock);

s32 sbtlCheckZoom(void) {
    struct SBTLUNIT_WORK *unit;
    sdfVector4_t pos;

    if (wk_sbtl->startFlg & 3) {
        mapZoomSetGoal(wk_sbtl->zoom_x, wk_sbtl->zoom_y, wk_sbtl->zoom_z, wk_sbtl->zoom_bairitu, wk_sbtl->zoom_time);
        if (wk_sbtl->startFlg & 1) {
            sbtlSetZoomLock(sbtlSearchUnit_ID(wk_sbtl->unitList, wk_sbtl->targetUnitID), wk_sbtl->zoom_bairitu);
        }
        wk_sbtl->startFlg &= ~3;
        return 1;
    }
    if (wk_sbtl->startFlg & 4) {
        wk_sbtl->zoomlocktime++;
        unit = sbtlSearchUnit_ID(wk_sbtl->unitList, wk_sbtl->zoomlockID);
        if (wk_sbtl->zoomlocktime >= 60 || !wk_sbtl->zoomlockflg) {
            wk_sbtl->zoomlockflg = 1;
            wk_sbtl->zoomlocktime = 0;
            sbtlGetUnitHelperPos(unit, SBTLUNIT_MARK__1SHOT);
            SDF_VU0_STOREVECTOR(&pos);
            pos.y = unit->pos.y - 90.0f;
            mapZoomSetGoal(pos.x, pos.y, pos.z, wk_sbtl->zoom_bairitu, 1);
        }
    }
    return 1;
}

s32 sbtlGetScrollID(void) {
    struct SBTLUNIT_WORK *unit;

    if ((wk_sbtl->flg & 0x80000) && wk_sbtl->list2CC != NULL && wk_sbtl->list2CC->count > 0) {
        unit = sbtlSearchUnit_ID(wk_sbtl->list2CC, wk_sbtl->tgt_csr);
        if (unit != NULL && sbtlCheckDamageTarget(unit)) {
            return unit->field1A0;
        }
    }
    if (wk_sbtl->flg & 0x4000000) {
        return wk_sbtl->unitPadID;
    }
    return wk_sbtl->targetUnitID;
}

s32 DataLoadProc(dds3ProcessID_t *id) {
    s32 status;

    switch (loadstep) {
    case 0:
        sbtlCallMemset4(wk_sbtl->mapNo[0], wk_sbtl->mapNo[1]);
        if (encLoad()) {
            dds3FadeIn2(0);
            encMode(1);
        } else {
            dds3FadeIn2(15);
        }
        loadstep = 1;
        if (sndBgmStatus_XF() == 3) {
            sbtlSetLoadPanelData();
            loadstep = 2;
        }
        break;
    case 1:
        status = sndBgmStatus_XF();
        if (status == 2) {
            sndBgmPlay();
            sbtlSetLoadPanelData();
            loadstep = status;
        }
        break;
    case 2:
        if (sbtlCallCheckEnd()) {
            sbtlCallBitCheck(wk_sbtl->stageNo);
            loadstep = 0;
            return -1;
        }
        break;
    }
    return 0;
}

/* body: matched in store @ 2026-11-16: /home/wsl/.local/state/raidou-decomp-work/clusters/store/raidou1/0024E6A0.c:1, found */
s32 sbtlLeaf54(dds3ProcessID_t *id) {
    return sbtlOnDataLoadInit();
}

s32 BasicSceneFree(void) {
    dds3Basic_t *scene;
    dds3SceneBasic_t *work;

    scene = dds3GetActiveScene();
    if (scene == NULL) {
        return 0;
    }
    work = scene->workdata;
    while (work->basictable[BASIC_TYPE_UNITOBJECT].firstbasic != NULL) {
        dds3DeleteSceneBasicData(work->basictable[BASIC_TYPE_UNITOBJECT].firstbasic);
    }
    while (sdfDelayedResrcCheck()) {
    }
    dds3DeleteScene(scene);
    return 1;
}
