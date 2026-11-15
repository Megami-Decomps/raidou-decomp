#include "common.h"
#include "dds3ObjectBase.h"
#include "EventBit.h"
#include "fld.h"
#include "sdk_libc.h"
#include "mapData.h"
#include "field.h"

/* NON_MATCHING: 145 of 1030 words match. Retail does not hoist the float constants and pointer reloads out of the loop; mine does (frame 0xB0 vs 0x70). */
#ifdef NON_MATCHING
void mapMapObjectRefresh_fld(void)
{
    f32 pos[2];
    f32 y;
    s32 i;
    s32 low;
    fldMapObjRec_t *rec;
    fldGenbaTbl_t *genba;

    for (i = 0; i < pMapAreaNow->objCount; i++)
    {
        rec = &pMapObjNow[i];
        PositioningGuardSystem(rec->ix, rec->iy, &pos[0], &pos[1]);
        if ((u16)(rec->id - 0x4000) < 0x100 || (u16)(rec->id - 0x4100) < 0x300)
        {
            continue;
        }
        switch (rec->ofs)
        {
        case 0:
            pos[0] += 50.0f;
            pos[1] -= 50.0f;
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
        y = mapGetY(pos[0], pos[1]);;
        if ((u16)(rec->id - 0x5000) < 0x40)
        {
            continue;
        }
        if ((u16)(rec->id - 0x2000) < 0x40)
        {
            mapObjLightAdd(-1, pos[0], y, pos[1], 50.0f, 150.0f, 1.6f, 1.6f, 1.9f, 1.0f, 1.0f, 1.5f);
            mapAddHukidashi(rec->id, 2, 19, 0, -1, 1, pos[0], y, pos[1], mapData20[rec->dir], 1.0f);
            mapObjEveHitSet2(rec->id, 1, 0, 150.0f, "save_label");
            continue;
        }
        if ((u16)(rec->id - 0x7000) < 0x40)
        {
            mapAddHukidashi(rec->id, 2, 11, 0, -1, 1, pos[0], y, pos[1], mapData20[rec->dir], 1.0f);
            continue;
        }
        if ((u16)(rec->id - 0x6000) < 0x40)
        {
            genba = &fldGenbaTbl[rec->id & 0xFF];
            mapNpcTblAdd(rec->id, pos[0], y + genba->yofs, pos[1]);
            mapLoadGenbaEpl();
            mapGenbaPosEffAdd(rec->id, pos[0], y + genba->yofs, pos[1], y);
            mapAddHukidashi(rec->id, 2, 18, 0, -1, 1, pos[0], y + genba->yofs, pos[1], mapData20[rec->dir], 1.0f);
            mapObjVisible(rec->id, evtBitCheck(fldGenbaTbl[rec->id & 0xFF].evtBit) != 0);
            continue;
        }
        if ((u16)(rec->id - 0x1000) >= 0x100)
        {
            if ((u16)(rec->id - 0x3000) < 0x40)
            {
                mapObjLightAdd(-1, pos[0], y, pos[1], 150.0f, 250.0f, 0.8f, 1.8f, 1.9f, 0.3f, 0.5f, 0.5f);
                mapAddHukidashi(rec->id, rec->major, rec->minor, 0, -1, 1, pos[0], y, pos[1], mapData20[rec->dir], 1.0f);
                mapObjHitSize(rec->id, 25.0f);
                mapObjFloor(rec->id, evtWorkScriptContextReturnValue2);
                mapObjEveHitSet(rec->id, 1, mapMapObjectRefresh_fldData[rec->id & 0xFF] != 0 ? "cheal_label" : "heal_label");
                continue;
            }
            mapObjectRefreshSub(rec->id, rec->major, rec->minor, rec->motion, -1, pos[0], y, pos[1], mapData20[rec->dir], 1.0f);
            continue;
        }
        low = rec->id & 0xFF;
        if (low >= 0xF0)
        {
            if (!evtBitCheck(175) || evtBitCheck(183))
            {
                continue;
            }
            printf("ep09 on takara %04x\n", rec->id);
        }
        else if ((low >= 0xF && low < 0x15) || (low >= 0x18 && low < 0x1E) || (low >= 0x24 && low < 0x29))
        {
            if (evtBitCheck(175) && !evtBitCheck(183))
            {
                printf("ep09 off takara %04x\n", rec->id);
                continue;
            }
        }
        if (!mapRefDDS3GBWK5(rec->id & 0xFF))
        {
            if (rec->minor == 18)
            {
                mapObjectRefreshSub(rec->id, rec->major, rec->minor, 0, -1, pos[0], y, pos[1], mapData20[rec->dir], 1.0f);
            }
            else
            {
                mapAddHukidashi(rec->id, rec->major, rec->minor, 0, -1, 1, pos[0], y, pos[1], mapData20[rec->dir], 1.0f);
            }
            mapObjFloor(rec->id, evtWorkScriptContextReturnValue2);
            if (rec->minor != 18)
            {
                mapObjHitSize(rec->id, 25.0f);
                dds3ObjectBaseOnFlag(mapObjGetBasic(rec->id), 1024);
                dds3GetRadiusPoseSet(mapObjGetBasic(rec->id), 50.0f);
            }
            mapObjEveHitSet(rec->id, 1, "itembox_label");
        }
        else
        {
            if (rec->minor == 18)
            {
                continue;
            }
            mapAddHukidashi(rec->id, rec->major, rec->minor, 0, -1, 1, pos[0], y, pos[1], mapData20[rec->dir], 1.0f);
            mapObjHitSize(rec->id, 25.0f);
            mapObjFloor(rec->id, evtWorkScriptContextReturnValue2);
            mapObjEveHitSet(rec->id, 1, "itembox_label");
            mapObjMotionCng(rec->id, 2, 0);
            dds3ObjectBaseOnFlag(mapObjGetBasic(rec->id), 1024);
            dds3GetRadiusPoseSet(mapObjGetBasic(rec->id), 50.0f);
        }
    }
    if (pMapAreaDat == NULL)
    {
        return;
    }
    printf("SUB OBJECT!!\n");
    for (i = 0; i < pMapAreaDat->objCount; i++)
    {
        rec = &pMapObjSub[i];
        fldWallObjTaskSub(gSubFloorID, rec->ix, rec->iy, &pos[0], &pos[1]);
        if ((u16)(rec->id - 0x4000) < 0x100 || (u16)(rec->id - 0x4100) < 0x300)
        {
            continue;
        }
        switch (rec->ofs)
        {
        case 0:
            pos[0] += 50.0f;
            pos[1] -= 50.0f;
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
        y = mapGetFloorY(gSubFloorID, pos[0], pos[1]);;
        if ((u16)(rec->id - 0x5000) < 0x40)
        {
            continue;
        }
        if ((u16)(rec->id - 0x2000) < 0x40)
        {
            mapObjectRefreshSub(rec->id, 2, 19, 0, -1, pos[0], y, pos[1], mapData20[rec->dir], 1.0f);
            mapObjEveHitSet2(rec->id, 1, 0, 150.0f, "save_label");
            continue;
        }
        if ((u16)(rec->id - 0x7000) < 0x40)
        {
            mapObjectRefreshSub(rec->id, 2, 11, 0, -1, pos[0], y, pos[1], mapData20[rec->dir], 1.0f);
            continue;
        }
        if ((u16)(rec->id - 0x6000) < 0x40)
        {
            genba = &fldGenbaTbl[rec->id & 0xFF];
            mapNpcTblAdd(rec->id, pos[0], y + genba->yofs, pos[1]);
            mapLoadGenbaEpl();
            mapGenbaPosEffAdd(rec->id, pos[0], y + genba->yofs, pos[1], y);
            mapObjectRefreshSub(rec->id, 2, 18, 0, -1, pos[0], y + genba->yofs, pos[1], mapData20[rec->dir], 1.0f);
            mapObjVisible(rec->id, evtBitCheck(fldGenbaTbl[rec->id & 0xFF].evtBit) != 0);
            continue;
        }
        if ((u16)(rec->id - 0x1000) >= 0x100)
        {
            if ((u16)(rec->id - 0x3000) < 0x40)
            {
                mapObjectRefreshSub(rec->id, rec->major, rec->minor, 0, -1, pos[0], y, pos[1], mapData20[rec->dir], 1.0f);
                mapObjHitSize(rec->id, 25.0f);
                mapObjFloor(rec->id, gSubFloorID);
                mapObjEveHitSet(rec->id, 1, mapMapObjectRefresh_fldData[rec->id & 0xFF] != 0 ? "cheal_label" : "heal_label");
                continue;
            }
            mapObjectRefreshSub(rec->id, rec->major, rec->minor, rec->motion, -1, pos[0], y, pos[1], mapData20[rec->dir], 1.0f);
            continue;
        }
        low = rec->id & 0xFF;
        if (low >= 0xF0)
        {
            if (!evtBitCheck(175) || evtBitCheck(183))
            {
                continue;
            }
            printf("ep09 on takara %04x\n", rec->id);
        }
        else if ((low >= 0xF && low < 0x15) || (low >= 0x18 && low < 0x1E) || (low >= 0x24 && low < 0x29))
        {
            if (evtBitCheck(175) && !evtBitCheck(183))
            {
                printf("ep09 off takara %04x\n", rec->id);
                continue;
            }
        }
        if (!mapRefDDS3GBWK5(rec->id & 0xFF))
        {
            mapObjectRefreshSub(rec->id, rec->major, rec->minor, 0, -1, pos[0], y, pos[1], mapData20[rec->dir], 1.0f);
            mapObjFloor(rec->id, gSubFloorID);
            if (rec->minor != 18)
            {
                mapObjHitSize(rec->id, 25.0f);
                dds3ObjectBaseOnFlag(mapObjGetBasic(rec->id), 1024);
                dds3GetRadiusPoseSet(mapObjGetBasic(rec->id), 50.0f);
            }
            mapObjEveHitSet(rec->id, 1, "itembox_label");
        }
        else
        {
            if (rec->minor == 18)
            {
                continue;
            }
            mapObjectRefreshSub(rec->id, rec->major, rec->minor, 0, -1, pos[0], y, pos[1], mapData20[rec->dir], 1.0f);
            mapObjHitSize(rec->id, 25.0f);
            mapObjFloor(rec->id, gSubFloorID);
            mapObjEveHitSet(rec->id, 1, "itembox_label");
            mapObjMotionCng(rec->id, 2, 0);
            dds3ObjectBaseOnFlag(mapObjGetBasic(rec->id), 1024);
            dds3GetRadiusPoseSet(mapObjGetBasic(rec->id), 50.0f);
        }
    }
}
#else
INCLUDE_ASM(const s32, "code_r1_0014CF58", mapMapObjectRefresh_fld);
#endif
