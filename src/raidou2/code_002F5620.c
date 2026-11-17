#include "common.h"
#include "sbtlDMG.h"
#include "sbattle.h"

#define SBTL_LIMIT_F(v, min, max) (((v) <= (min)) ? (min) : (((max) <= (v)) ? (max) : (v)))
#define SBTL_LIMIT_I(v, min, max) (((v) <= (min)) ? (min) : (((v) > (max)) ? (max) : (v)))

s32 sbtlSetMDLHitPara(SBTLSKILLHIT_TBL *hwk, SBDEBUG_CSRWORK *wk, s32 flg)
{
    f32 fadd = 0.0f;
    s32 add = 0;
    s32 num;
    SBTLSKILLHIT_POSWORK *start;
    /* BUG: copied from the effect hit editor without its ef_stendp switch, so
     * the move fields (ef_stendp 1 and 2) are written through NULL. */
    SBTLSKILLIDOU_WORK *idou = NULL;

    if (flg == 0) {
        fadd = 0.1f;
        add = 1;
    } else if (flg == 1) {
        fadd = -0.1f;
        add = -1;
    } else if (flg == 2) {
        fadd = 0.5f;
        add = 5;
    } else if (flg == 3) {
        fadd = -0.5f;
        add = -5;
    } else if (flg == 4) {
        fadd = 5.0f;
        add = 10;
    } else if (flg == 5) {
        fadd = -5.0f;
        add = -10;
    }

    start = &hwk->start;
#ifdef FIX_BUGS
    /* The effect hit editor (sbtlSetEffHitPara_sbtl) edits apos1 for ef_stendp 1, apos2 for 2. */
    idou = (evtStageTestState.ef_stendp == 1) ? &hwk->apos1 : &hwk->apos2;
#endif

    if (evtStageTestState.ef_mode == 3) {
        switch (evtStageTestState.nef_poseture) {
        case 0:
            hwk->x += fadd;
            if (hwk->x <= -2000.0f) {
                hwk->x = -2000.0f;
            }
            if (2000.0f <= hwk->x) {
                hwk->x = 2000.0f;
            }
            break;
        case 1:
            hwk->y += fadd;
            if (hwk->y <= -2000.0f) {
                hwk->y = -2000.0f;
            }
            if (2000.0f <= hwk->y) {
                hwk->y = 2000.0f;
            }
            break;
        case 2:
            hwk->z += fadd;
            if (hwk->z <= -2000.0f) {
                hwk->z = -2000.0f;
            }
            if (2000.0f <= hwk->z) {
                hwk->z = 2000.0f;
            }
            break;
        }
        return 1;
    }

    switch (hwk->type) {
    case 0:
        switch (wk->point) {
        case 0:
            evtStageTestState.ef_mode = 1;
            break;
        case 1:
            hwk->area_r += fadd;
            hwk->area_r = SBTL_LIMIT_F(hwk->area_r, 0.0f, 4000.0f);
            break;
        case 2:
            hwk->cy.height += fadd;
            hwk->cy.height = SBTL_LIMIT_F(hwk->cy.height, 0.0f, 4000.0f);
            break;
        case 3:
            hwk->rad += fadd;
            if (1.0f <= hwk->rad) {
                hwk->rad = 1.0f;
            }
            if (hwk->rad <= -1.0f) {
                hwk->rad = -1.0f;
            }
            break;
        case 4:
            hwk->lifetime += add;
            hwk->lifetime = SBTL_LIMIT_I(hwk->lifetime, 0, 300);
            break;
        case 5:
            evtStageTestState.ef_mode = 3;
            break;
        case 6:
            hwk->dir += fadd;
            if (360.0f <= hwk->dir) {
                hwk->dir = 0.0f;
            }
            if (hwk->dir < 0.0f) {
                hwk->dir = 360.0f;
            }
            break;
        case 7:
            hwk->cy.xrad += fadd;
            if (360.0f <= hwk->cy.xrad) {
                hwk->cy.xrad = 0.0f;
            }
            if (hwk->cy.xrad < 0.0f) {
                hwk->cy.xrad = 360.0f;
            }
            break;
        case 8:
            break;
        case 9:
            if (start->iti == 1) {
                start->iti = 10;
                hwk->flg |= 0x8000;
            } else {
                start->iti = 1;
                hwk->flg &= ~0x8000;
            }
            break;
        case 10:
            if (evtStageTestState.ef_stendp == 0 || evtStageTestState.ef_stendp == 3) {
                start->iti_no += add;
                start->iti_no = SBTL_LIMIT_I(start->iti_no, 0, 999);
            } else {
                idou->w += fadd;
                if (1000.0f <= idou->w) {
                    idou->w = 1000.0f;
                }
                if (idou->w <= -1000.0f) {
                    idou->w = -1000.0f;
                }
            }
            break;
        case 11:
            if (evtStageTestState.ef_stendp == 0 || evtStageTestState.ef_stendp == 3) {
                start->takasa += add;
                start->takasa = SBTL_LIMIT_I(start->takasa, 0, 5);
            } else {
                idou->h += fadd;
                if (1000.0f <= idou->h) {
                    idou->h = 1000.0f;
                }
                if (idou->h <= -1000.0f) {
                    idou->h = -1000.0f;
                }
            }
            break;
        case 12:
            if (evtStageTestState.ef_stendp == 0 || evtStageTestState.ef_stendp == 3) {
                start->muki += add;
                start->muki = SBTL_LIMIT_I(start->muki, 0, 8);
            }
            break;
        case 13:
            hwk->motionno += add;
            num = sbtlGetMotionNum(evtStageTestState.acterID);
            if (hwk->motionno >= num) {
                hwk->motionno = 0;
            }
            if (hwk->motionno < 0) {
                hwk->motionno = num - 1;
            }
            break;
        }
        break;
    case 1:
        switch (wk->point) {
        case 0:
            evtStageTestState.ef_mode = 1;
            break;
        case 1:
            hwk->area_r += fadd;
            hwk->area_r = SBTL_LIMIT_F(hwk->area_r, 0.0f, 4000.0f);
            hwk->cy.area_r += fadd;
            hwk->cy.area_r = SBTL_LIMIT_F(hwk->cy.area_r, 0.0f, 4000.0f);
            break;
        case 2:
            hwk->cy.height += fadd;
            hwk->cy.height = SBTL_LIMIT_F(hwk->cy.height, 0.0f, 4000.0f);
            break;
        case 3:
            hwk->rad += fadd;
            if (1.0f <= hwk->rad) {
                hwk->rad = 1.0f;
            }
            if (hwk->rad <= -1.0f) {
                hwk->rad = -1.0f;
            }
            break;
        case 4:
            hwk->lifetime += add;
            hwk->lifetime = SBTL_LIMIT_I(hwk->lifetime, 0, 300);
            break;
        case 5:
            evtStageTestState.ef_mode = 3;
            break;
        case 6:
            hwk->dir += fadd;
            if (360.0f <= hwk->dir) {
                hwk->dir = 0.0f;
            }
            if (hwk->dir < 0.0f) {
                hwk->dir = 360.0f;
            }
            break;
        case 7:
            hwk->cy.xrad += fadd;
            if (360.0f <= hwk->cy.xrad) {
                hwk->cy.xrad = 0.0f;
            }
            if (hwk->cy.xrad < 0.0f) {
                hwk->cy.xrad = 360.0f;
            }
            break;
        case 8:
            break;
        case 9:
            if (start->iti == 1) {
                start->iti = 10;
                hwk->flg |= 0x8000;
            } else {
                start->iti = 1;
                hwk->flg &= ~0x8000;
            }
            break;
        case 10:
            if (evtStageTestState.ef_stendp == 0 || evtStageTestState.ef_stendp == 3) {
                start->iti_no += add;
                start->iti_no = SBTL_LIMIT_I(start->iti_no, 0, 999);
            } else {
                idou->w += fadd;
                if (1000.0f <= idou->w) {
                    idou->w = 1000.0f;
                }
                if (idou->w <= -1000.0f) {
                    idou->w = -1000.0f;
                }
            }
            break;
        case 11:
            if (evtStageTestState.ef_stendp == 0 || evtStageTestState.ef_stendp == 3) {
                start->takasa += add;
                start->takasa = SBTL_LIMIT_I(start->takasa, 0, 5);
            } else {
                idou->h += fadd;
                if (1000.0f <= idou->h) {
                    idou->h = 1000.0f;
                }
                if (idou->h <= -1000.0f) {
                    idou->h = -1000.0f;
                }
            }
            break;
        case 12:
            if (evtStageTestState.ef_stendp == 0 || evtStageTestState.ef_stendp == 3) {
                start->muki += add;
                start->muki = SBTL_LIMIT_I(start->muki, 0, 8);
            }
            break;
        case 13:
            hwk->motionno += add;
            num = sbtlGetMotionNum(evtStageTestState.acterID);
            if (hwk->motionno >= num) {
                hwk->motionno = 0;
            }
            if (hwk->motionno < 0) {
                hwk->motionno = num - 1;
            }
            break;
        }
        break;
    case 2:
        switch (wk->point) {
        case 0:
            evtStageTestState.ef_mode = 1;
            break;
        case 1:
            hwk->area_r += fadd;
            hwk->area_r = SBTL_LIMIT_F(hwk->area_r, 0.0f, 4000.0f);
            hwk->cy.area_r += fadd;
            hwk->cy.area_r = SBTL_LIMIT_F(hwk->cy.area_r, 0.0f, 4000.0f);
            break;
        case 2:
            hwk->area_r2 += fadd;
            hwk->area_r2 = SBTL_LIMIT_F(hwk->area_r2, 0.0f, 4000.0f);
            break;
        case 3:
            hwk->cy.height += fadd;
            hwk->cy.height = SBTL_LIMIT_F(hwk->cy.height, 0.0f, 4000.0f);
            break;
        case 4:
            hwk->rad += fadd;
            if (1.0f <= hwk->rad) {
                hwk->rad = 1.0f;
            }
            if (hwk->rad <= -1.0f) {
                hwk->rad = -1.0f;
            }
            break;
        case 5:
            hwk->lifetime += add;
            hwk->lifetime = SBTL_LIMIT_I(hwk->lifetime, 0, 1000);
            break;
        case 6:
            evtStageTestState.ef_mode = 3;
            break;
        case 7:
            hwk->dir += fadd;
            if (360.0f <= hwk->dir) {
                hwk->dir = 0.0f;
            }
            if (hwk->dir < 0.0f) {
                hwk->dir = 360.0f;
            }
            break;
        case 8:
            hwk->cy.xrad += fadd;
            if (360.0f <= hwk->cy.xrad) {
                hwk->cy.xrad = 0.0f;
            }
            if (hwk->cy.xrad < 0.0f) {
                hwk->cy.xrad = 360.0f;
            }
            break;
        case 9:
            break;
        case 10:
            if (start->iti == 1) {
                start->iti = 10;
                hwk->flg |= 0x8000;
            } else {
                start->iti = 1;
                hwk->flg &= ~0x8000;
            }
            break;
        case 11:
            if (evtStageTestState.ef_stendp == 0 || evtStageTestState.ef_stendp == 3) {
                start->iti_no += add;
                start->iti_no = SBTL_LIMIT_I(start->iti_no, 0, 999);
            } else {
                idou->w += fadd;
                if (1000.0f <= idou->w) {
                    idou->w = 1000.0f;
                }
                if (idou->w <= -1000.0f) {
                    idou->w = -1000.0f;
                }
            }
            break;
        case 12:
            if (evtStageTestState.ef_stendp == 0 || evtStageTestState.ef_stendp == 3) {
                start->takasa += add;
                start->takasa = SBTL_LIMIT_I(start->takasa, 0, 5);
            } else {
                idou->h += fadd;
                if (1000.0f <= idou->h) {
                    idou->h = 1000.0f;
                }
                if (idou->h <= -1000.0f) {
                    idou->h = -1000.0f;
                }
            }
            break;
        case 13:
            if (evtStageTestState.ef_stendp == 0 || evtStageTestState.ef_stendp == 3) {
                start->muki += add;
                start->muki = SBTL_LIMIT_I(start->muki, 0, 8);
            }
            break;
        case 14:
            hwk->motionno += add;
            num = sbtlGetMotionNum(evtStageTestState.acterID);
            if (hwk->motionno >= num) {
                hwk->motionno = 0;
            }
            if (hwk->motionno < 0) {
                hwk->motionno = num - 1;
            }
            break;
        }
        break;

    }
    return 1;
}
