#include "common.h"
#include "newdata.h"
#include "libsdf.h"
#include "kernel.h"
#include "basic.h"
#include "dds3KernelCore.h"
#include "kernel_draw.h"
#include "dds3KernelDraw.h"
#include "sdf2D.h"
#include "sdfPrim.h"
#include "sdfPacketBuff.h"
#include "sdfDraw.h"
#include "dds3KernelMain.h"
#include "code_r1_0010DF68.h"
#include "code_0010F2C0.h"
#include "code_r1_002CD5A8.h"
#include "dds3PadManager.h"
#include "sdfMemory.h"
#include "sdk_libc.h"
#include "evtMoon.h"
#include "dat.h"
#include "datCalc.h"
#include "frFont.h"
#include "sBtlSM.h"
#include "code_002A38D8.h"
#include "gohMain.h"

/* body: matched in raidou @ 61f09dc: src/raidou1/sbtlEff_00289E50.c:75, found, same function 1.00 (words 2/2, calls 1.00, shape 1.00) */
void sbtlLeaf45(dds3ProcessID_t *id) {
}

void sbtlDebugTimeGrph(s8 enabled)
{
    if (enabled == 1) {
        sbtlDebugTimeGrphWork = dds3StartProcess(sbtlStrDebugTimeGrph, 10000, 1, 1, evtDrawResourceCountDebugTask,
                                                 (dds3ProcessFunc_t)sbtlLeaf45, NULL);
    } else if (enabled == 0) {
        dds3KillProcess(sbtlDebugTimeGrphWork, 0);
    }
}

s32 sbtlFunc8(itfGridTextWidget_t *list) {
    if (dds3PadButtonState[58]) {
        if (dds3PadButtonState[54] & 2) {
            fldLmapRewindThroughWindow(list);
        }
        if (dds3PadButtonState[55] & 2) {
            fldLmapAdvanceThroughWindow(list);
        }
    } else {
        if (dds3PadButtonState[54] & 2) {
            fldLmapRewindCursor(list);
        }
        if (dds3PadButtonState[55] & 2) {
            fldLmapAdvanceCursor(list);
        }
    }
    return 0;
}

s32 sbtlLeaf46(itfGridTextWidget_t *widget) {
    if (dds3PadButtonState[49] < 0 || dds3PadButtonState[52] < 0) {
        widget->flags |= 0x20;
        return 1;
    }
    return 0;
}

s32 sbtlBtlDrawParamSub(itfGridTextWidget_t *widget) {
    u32 leaf = widget->flags & 1;
    s32 ret = sbtlLeaf46(widget);

    if (ret != 0) {
        return ret;
    }
    if (dds3PadButtonState[51] < 0 || dds3PadButtonState[53] < 0) {
        if (leaf) {
            itfActivateGridTextWidget(widget);
            return 1;
        }
        itfSetWidgetFlagsAndActivateChild(widget, 1);
        return 2;
    }
    return 0;
}

void sbtlCallGetPlayerExp(itfGridTextWidget_t *widget, s32 mode) {
    SBTL_PLAYER_DATA *player = &DDS3_GBWK->playerData;
    itfGridTextListItem_t *row;
    s32 maxhp;

    if (mode == 0) {
        player->unit.exp = datGetPlayerExp(player->unit.level);
    }
    if (mode == 1) {
        *player = datGetPlayerLevelUpUnit(player);
    }
    datCallGetPlayerMaxHP(&player->unit, player->val146);
    maxhp = player->unit.now.maxhp;
    if (mode != 2) {
        player->unit.now.hp = maxhp;
    }
    row = widget->head;
    row->number = player->unit.exp;
    row = row->next;
    row->number = player->unit.level;
    row = row->next;
    if (maxhp < player->unit.now.hp) {
        player->unit.now.hp = maxhp;
    }
    row->number = player->unit.now.hp;
    row = row->next;
    row->number = player->mag;
    row = row->next;
    row->number = player->unit.now.str;
    row = row->next;
    row->number = player->unit.now.tec;
    row = row->next;
    row->number = player->unit.now.vit;
    row = row->next;
    row->number = player->unit.now.luck;
    row = row->next;
    row->number = player->val146;
    row = row->next;
    row->number = player->swordAtk;
}

s32 sbtlRefDDS3GBWK7(itfGridTextWidget_t *widget) {
    u32 step = 1;

    if (widget->flags & 1) {
        if (dds3PadButtonState[58]) {
            step = 100;
        }
        if (dds3PadButtonState[59]) {
            step = 1000;
        }
        if (dds3PadButtonState[56]) {
            step = 10000;
        }
        if (dds3PadButtonState[54] & 2) {
            itfAdvanceSelectedGridScroll(widget, step);
        }
        if (dds3PadButtonState[55] & 2) {
            itfReverseSelectedGridScroll(widget, step);
        }
        DDS3_GBWK->playerData.unit.exp = widget->selected->number;
        if (dds3PadButtonState[57]) {
            sbtlCallGetPlayerExp(widget, 1);
        }
    }
    sbtlBtlDrawParamSub(widget);
    return 1;
}

s32 sbtlRefDDS3GBWK8(itfGridTextWidget_t *widget) {
    if (widget->flags & 1) {
        if (dds3PadButtonState[54] & 2) {
            itfAdvanceSelectedGridScroll(widget, 1);
        }
        if (dds3PadButtonState[55] & 2) {
            itfReverseSelectedGridScroll(widget, 1);
        }
        DDS3_GBWK->playerData.unit.level = widget->selected->number;
        sbtlCallGetPlayerExp(widget, 0);
    }
    sbtlBtlDrawParamSub(widget);
    return 1;
}

s32 sbtlRefDDS3GBWK9(itfGridTextWidget_t *widget) {
    u32 step = 1;

    if (widget->flags & 1) {
        if (dds3PadButtonState[58]) {
            step = 10;
        }
        if (dds3PadButtonState[59]) {
            step = 100;
        }
        if (dds3PadButtonState[54] & 2) {
            itfAdvanceSelectedGridScroll(widget, step);
        }
        if (dds3PadButtonState[55] & 2) {
            itfReverseSelectedGridScroll(widget, step);
        }
        DDS3_GBWK->playerData.unit.now.hp = widget->selected->number;
        sbtlCallGetPlayerExp(widget, 2);
    }
    sbtlBtlDrawParamSub(widget);
    return 1;
}

s32 sbtlRefDDS3GBWK10(itfGridTextWidget_t *widget) {
    u32 step = 1;

    if (widget->flags & 1) {
        if (dds3PadButtonState[58]) {
            step = 10;
        }
        if (dds3PadButtonState[59]) {
            step = 100;
        }
        if (dds3PadButtonState[54] & 2) {
            itfAdvanceSelectedGridScroll(widget, step);
        }
        if (dds3PadButtonState[55] & 2) {
            itfReverseSelectedGridScroll(widget, step);
        }
        DDS3_GBWK->playerData.mag = widget->selected->number;
        sbtlCallGetPlayerExp(widget, 2);
    }
    sbtlBtlDrawParamSub(widget);
    return 1;
}

s32 sbtlRefDDS3GBWK11(itfGridTextWidget_t *widget) {
    if (widget->flags & 1) {
        if (dds3PadButtonState[54] & 2) {
            itfAdvanceSelectedGridScroll(widget, 1);
        }
        if (dds3PadButtonState[55] & 2) {
            itfReverseSelectedGridScroll(widget, 1);
        }
        DDS3_GBWK->playerData.unit.backup.str = widget->selected->number;
        sbtlCallGetPlayerExp(widget, 0);
    }
    sbtlBtlDrawParamSub(widget);
    return 1;
}

s32 sbtlRefDDS3GBWK12(itfGridTextWidget_t *widget) {
    if (widget->flags & 1) {
        if (dds3PadButtonState[54] & 2) {
            itfAdvanceSelectedGridScroll(widget, 1);
        }
        if (dds3PadButtonState[55] & 2) {
            itfReverseSelectedGridScroll(widget, 1);
        }
        DDS3_GBWK->playerData.unit.backup.tec = widget->selected->number;
        sbtlCallGetPlayerExp(widget, 0);
    }
    sbtlBtlDrawParamSub(widget);
    return 1;
}

s32 sbtlRefDDS3GBWK13(itfGridTextWidget_t *widget) {
    if (widget->flags & 1) {
        if (dds3PadButtonState[54] & 2) {
            itfAdvanceSelectedGridScroll(widget, 1);
        }
        if (dds3PadButtonState[55] & 2) {
            itfReverseSelectedGridScroll(widget, 1);
        }
        DDS3_GBWK->playerData.unit.backup.vit = widget->selected->number;
        sbtlCallGetPlayerExp(widget, 0);
    }
    sbtlBtlDrawParamSub(widget);
    return 1;
}

s32 sbtlDrawParam(itfGridTextWidget_t *widget) {
    if (widget->flags & 1) {
        if (dds3PadButtonState[54] & 2) {
            itfAdvanceSelectedGridScroll(widget, 1);
        }
        if (dds3PadButtonState[55] & 2) {
            itfReverseSelectedGridScroll(widget, 1);
        }
        DDS3_GBWK->playerData.unit.backup.luck = widget->selected->number;
        sbtlCallGetPlayerExp(widget, 0);
    }
    sbtlBtlDrawParamSub(widget);
    return 1;
}

s32 sbtlFunc9(itfGridTextWidget_t *widget) {
    u32 step = 1;

    if (widget->flags & 1) {
        if (dds3PadButtonState[58]) {
            step = 10;
        }
        if (dds3PadButtonState[54] & 2) {
            itfAdvanceSelectedGridScroll(widget, step);
        }
        if (dds3PadButtonState[55] & 2) {
            itfReverseSelectedGridScroll(widget, step);
        }
        datRefDDS3GBWK3(widget->selected->number, 0);
        sbtlCallGetPlayerExp(widget, 2);
    }
    sbtlBtlDrawParamSub(widget);
    return 1;
}

s32 sbtlRefDDS3GBWK14(itfGridTextWidget_t *widget) {
    u32 step = 1;

    if (widget->flags & 1) {
        if (dds3PadButtonState[58]) {
            step = 10;
        }
        if (dds3PadButtonState[54] & 2) {
            itfAdvanceSelectedGridScroll(widget, step);
        }
        if (dds3PadButtonState[55] & 2) {
            itfReverseSelectedGridScroll(widget, step);
        }
        DDS3_GBWK->playerData.swordAtk = widget->selected->number;
    }
    sbtlBtlDrawParamSub(widget);
    return 1;
}

INCLUDE_RODATA(const s32, "code_0010F2C0", sbtlStrDebugTimeGrph);

void sbtlLEVEL(itfGridTextListItem_t *parentItem, u32 reference) {
    itfGridNumericDescriptor_t descriptor[10] = {
        {0, 0.0f, 9999999.0f, 1.0f, 0.0f},
        {0, 1.0f, 99.0f, 1.0f, 1.0f},
        {0, 1.0f, 999.0f, 1.0f, 1.0f},
        {0, 0.0f, 9999.0f, 1.0f, 0.0f},
        {0, 0.0f, 40.0f, 1.0f, 0.0f},
        {0, 0.0f, 40.0f, 1.0f, 0.0f},
        {0, 0.0f, 40.0f, 1.0f, 0.0f},
        {0, 0.0f, 40.0f, 1.0f, 0.0f},
        {1, 0.0f, 80.0f, 1.0f, 0.0f},
        {0, 0.0f, 999.0f, 1.0f, 0.0f},
    };
    itfGridTextWidget_t *widget;
    itfGridTextListItem_t *item;

    widget = itfCreateGridTextWidget(nbData3, 0, 0, 0, 10, reference);
    widget->onSelect = sbtlFunc8;
    fclGohInitSkillChangeSub(parentItem, NULL, widget);
    item = itfAppendGridTextItem(widget, sbtlLEVELData, 0);
    item->select = sbtlRefDDS3GBWK7;
    itfSetGridNumericItemDescriptor(widget, item, &descriptor[0]);
    item = itfAppendGridTextItem(widget, sbtlLEVELData2, 0);
    item->select = sbtlRefDDS3GBWK8;
    itfSetGridNumericItemDescriptor(widget, item, &descriptor[1]);
    item = itfAppendGridTextItem(widget, sbtlLEVELData3, 0);
    item->select = sbtlRefDDS3GBWK9;
    itfSetGridNumericItemDescriptor(widget, item, &descriptor[2]);
    item = itfAppendGridTextItem(widget, sbtlLEVELData4, 0);
    item->select = sbtlRefDDS3GBWK10;
    itfSetGridNumericItemDescriptor(widget, item, &descriptor[3]);
    item = itfAppendGridTextItem(widget, sbtlLEVELData5, 0);
    item->select = sbtlRefDDS3GBWK11;
    itfSetGridNumericItemDescriptor(widget, item, &descriptor[4]);
    item = itfAppendGridTextItem(widget, sbtlLEVELData6, 0);
    item->select = sbtlRefDDS3GBWK12;
    itfSetGridNumericItemDescriptor(widget, item, &descriptor[5]);
    item = itfAppendGridTextItem(widget, sbtlLEVELData7, 0);
    item->select = sbtlRefDDS3GBWK13;
    itfSetGridNumericItemDescriptor(widget, item, &descriptor[6]);
    item = itfAppendGridTextItem(widget, sbtlLEVELData8, 0);
    item->select = sbtlDrawParam;
    itfSetGridNumericItemDescriptor(widget, item, &descriptor[7]);
    item = itfAppendGridTextItem(widget, sbtlLEVELData9, 0);
    item->select = sbtlFunc9;
    itfSetGridNumericItemDescriptor(widget, item, &descriptor[8]);
    item = itfAppendGridTextItem(widget, sbtlLEVELData10, 0);
    item->select = sbtlRefDDS3GBWK14;
    itfSetGridNumericItemDescriptor(widget, item, &descriptor[9]);
    sbtlCallGetPlayerExp(widget, 2);
}

s32 sbtlCallAddItem(itfGridTextWidget_t *widget) {
    itfGridTextListItem_t *item = widget->selected;
    s32 index = item->index;
    u32 step = 1;
    f32 number;

    if (index != 0 && (widget->flags & 1)) {
        if (dds3PadButtonState[58]) {
            step = 10;
        }
        if (dds3PadButtonState[54] & 2) {
            itfAdvanceSelectedGridScroll(widget, step);
        }
        if (dds3PadButtonState[55] & 2) {
            itfReverseSelectedGridScroll(widget, step);
        }
        datAddItem(index, (s32)widget->selected->number - DDS3_GBWK->playerData.item[index]);
    }
    if (dds3PadButtonState[50] & 2) {
        item = widget->head;
        if (item != NULL) {
            do {
                if (item->index != 0) {
                    number = item->number + 1.0f;
                    item->number = number;
                    if (number > 99.0f) {
                        item->number = 99.0f;
                    }
                    datAddItem(item->index, 1);
                }
                item = item->next;
            } while (item != NULL);
        }
    }
    sbtlBtlDrawParamSub(widget);
    return 1;
}

s32 sbtlCallGetSmallFontFRQ2(s32 x, s32 y, s32 z, itfGridTextWidget_t *widget, s32 ot) {
    s32 index = widget->selected->index;
    s32 drawY = y + ((widget->height + 12) << 3);
    frFontHandle_t *font;

    if (index != 0) {
        fldLmapSubmitScaledSpritePacket(x, drawY, z, 200, 28, 0x60000000, 0x80808080, ot);
        font = frwGetSmallFontFRQ2(x + 32, drawY + 32, z, 0xA09DC380, cmpSetupData4[index], NULL);
        frDrawFontOt(font, 1, ot);
        return frDestroyFontRequestAfter(font);
    }
#ifdef FIX_BUGS
    return 0;
#endif
}

/* NON_MATCHING: 86 of 93 words match. register choice of flags temp and the two function-pointer constants after itfCreateGridTextWidget, same as sbtlRefDDS3GBWK15 (retail v1/a2/v0). Logic as retail. */
#ifdef NON_MATCHING
s32 sbtlCallCheckItem(itfGridTextListItem_t *parentItem, u32 reference) {
    itfGridNumericDescriptor_t descriptor = {0, 0.0f, 99.0f, 1.0f, 0.0f};
    itfGridTextWidget_t *widget;
    itfGridTextListItem_t *item;
    s32 i = 0;
    s32 result;
    u32 flags;

    widget = itfCreateGridTextWidget(nbData3, 0, 0, 0, 8, reference);
    widget->onDraw = sbtlCallGetSmallFontFRQ2;
    widget->onSelect = sbtlFunc8;
    flags = widget->flags | 0x600;
    widget->flags = flags;
    fclGohInitSkillChangeSub(parentItem, NULL, widget);
    do {
        item = itfAppendGridTextItem(widget, nbData3, 0);
        item->select = sbtlCallAddItem;
        itfSetGridNumericItemDescriptor(widget, item, &descriptor);
        if (i < 144) {
            item->number = DDS3_GBWK->playerData.item[i];
        } else if (datCheckItem(i, 1) == 1) {
            item->number = 1.0f;
        } else {
            item->number = 0.0f;
        }
        i++;
        result = i < 194;
    } while (i < 194);
    return result;
}
#else
INCLUDE_ASM(const s32, "code_0010F2C0", sbtlCallCheckItem);
#endif

s32 sbtlCallGetStockDevilNums(itfGridTextWidget_t *widget) {
    u32 step = 1;
    s32 result;
    s32 index;
    s32 devilId;
    s32 i;
    sdatUnitWork_t *unit;
    itfGridTextListItem_t *row;

    if (widget->flags & 1) {
        if (dds3PadButtonState[58]) {
            step = 16;
        }
        if (dds3PadButtonState[54] & 2) {
            itfAdvanceSelectedGridScroll(widget, step);
        }
        if (dds3PadButtonState[55] & 2) {
            itfReverseSelectedGridScroll(widget, step);
        }
    }
    result = sbtlBtlDrawParamSub(widget);
    if (widget->selected->index + widget->rowOffset >= datGetStockDevilNums() + 1) {
        result = 0;
        itfActivateGridTextWidget(widget);
    }
    if (result == 1) {
        devilId = widget->selected->number;
        index = widget->selected->index + widget->rowOffset;
        if (devilId != 0) {
            if (DDS3_GBWK->playerData.stock[index].devilId == 0) {
                index = datCallCreateDevilUnitEx(devilId, 0);
            } else if (DDS3_GBWK->playerData.stock[index].devilId != devilId) {
                datCreateDevilUnitEx(devilId, 0, &DDS3_GBWK->playerData.stock[index]);
            }
            if (index >= 0) {
                unit = &DDS3_GBWK->playerData.stock[index];
                unit->badStatus &= ~(BSTF_STONE | BSTF_MOUDOKU);
                if (dds3PadButtonState[50]) {
                    unit->badStatus |= BSTF_STONE;
                } else if (dds3PadButtonState[48]) {
                    unit->badStatus |= BSTF_MOUDOKU;
                }
                gohCommandDEVILOUTAFTERSub(unit, &DDS3_GBWK->unk48F0, 1);
            }
        } else {
            datCallGetStockDevilID2(datGetStockDevilID(widget->selected->index + widget->rowOffset));
            row = widget->head;
            for (i = 0; row != NULL; i++) {
                row->number = DDS3_GBWK->playerData.stock[i].devilId;
                row = row->next;
            }
        }
    }
    return 1;
}

s32 sbtlCallGetSmallFontFRQ22(s32 x, s32 y, s32 z, itfGridTextWidget_t *widget, s32 ot) {
    s32 itemNo = widget->selected->number;
    s32 drawY = y + ((widget->height + 12) << 3);
    frFontHandle_t *font;

    if (itemNo != 0) {
        fldLmapSubmitScaledSpritePacket(x, drawY, z, 200, 28, 0x60000000, 0x80808080, ot);
        font = frwGetSmallFontFRQ2(x + 32, drawY + 32, z, 0xA09DC380, g_itemNo[itemNo], NULL);
        frDrawFontOt(font, 1, ot);
        return frDestroyFontRequestAfter(font);
    }
#ifdef FIX_BUGS
    return 0;
#endif
}

void sbtlLeaf47(itfGridTextWidget_t *widget, itfGridTextListItem_t *item, char *buf, s32 size) {
    s32 value = item->number;

    if (value != 0) {
        sprintf(buf, sbtlData82, value);
    } else {
        sprintf(buf, nbDebugNoStatusLabel);
    }
}

/* NON_MATCHING: 78 of 88 words match. register choice of the two function-pointer constants and the flags temp after itfCreateGridTextWidget (retail v0/a1/v1, mine v1/v0/a1). Logic as retail. */
#ifdef NON_MATCHING
s32 sbtlRefDDS3GBWK15(itfGridTextListItem_t *parentItem, u32 reference) {
    itfGridNumericDescriptor_t descriptor = {1, 0.0f, 336.0f, 1.0f, 0.0f};
    itfGridTextWidget_t *widget;
    itfGridTextListItem_t *item;
    s32 i = 0;
    s32 result;
    u32 flags;

    widget = itfCreateGridTextWidget(nbData3, 0, 0, 0, DDS3_GBWK->playerData.partyNum, reference);
    flags = widget->flags;
    widget->onSelect = sbtlFunc8;
    widget->onDraw = sbtlCallGetSmallFontFRQ22;
    widget->flags = flags | 0x200;
    fclGohInitSkillChangeSub(parentItem, NULL, widget);
    result = DDS3_GBWK->playerData.partyNum;
    if (result > 0) {
        do {
            item = itfAppendGridTextItem(widget, nbData3, 0);
            item->select = sbtlCallGetStockDevilNums;
            item->format = sbtlLeaf47;
            itfSetGridNumericItemDescriptor(widget, item, &descriptor);
            item->number = DDS3_GBWK->playerData.stock[i].devilId;
            i++;
            result = i < DDS3_GBWK->playerData.partyNum;
        } while (i < DDS3_GBWK->playerData.partyNum);
    }
    return result;
}
#else
INCLUDE_ASM(const s32, "code_0010F2C0", sbtlRefDDS3GBWK15);
#endif

/* NON_MATCHING: 83 of 149 words match. args of a call inside 'if (mode == 0)' loaded before the branch in retail: read unit->devilId and unit->level into locals ahead of the mode tests (head matched, 103 to 66 words). Logic as retail. */
#ifdef NON_MATCHING
void sbtlCallGetDevilLevelUpUnit(itfGridTextWidget_t *widget, s32 mode) {
    sdatUnitWork_t *unit = &DDS3_GBWK->playerData.stock[widget->parent->selected->index + widget->parent->rowOffset];
    itfGridTextListItem_t *row;
    s32 maxhp;
    s32 maxmp;
    s32 devilId;
    s32 level;

    devilId = unit->devilId;
    level = unit->level;
    if (mode == 0) {
        unit->exp = datGetDevilLevelUpNumsSub(devilId, level);
    }
    if (mode == 1) {
        *unit = datGetDevilLevelUpUnit(unit);
    }
    datCallGetPlayerMaxHP(unit, 0);
    maxhp = unit->now.maxhp;
    maxmp = unit->now.maxmp;
    if (mode != 2) {
        unit->now.hp = maxhp;
        unit->now.mp = maxmp;
    }
    row = widget->head;
    row->number = unit->exp;
    row = row->next;
    row->number = unit->level;
    row = row->next;
    if (maxhp < unit->now.hp) {
        unit->now.hp = maxhp;
    }
    row->number = unit->now.hp;
    row = row->next;
    if (maxmp < unit->now.mp) {
        unit->now.mp = maxmp;
    }
    row->number = unit->now.mp;
    row = row->next;
    row->number = unit->chuusei;
    row = row->next;
    row->number = unit->now.str;
    row = row->next;
    row->number = unit->now.tec;
    row = row->next;
    row->number = unit->now.vit;
    row = row->next;
    row->number = unit->now.luck;
}
#else
INCLUDE_ASM(const s32, "code_0010F2C0", sbtlCallGetDevilLevelUpUnit);
#endif

s32 sbtlRefDDS3GBWK(itfGridTextWidget_t *widget) {
    if (DDS3_GBWK->playerData.stock[widget->selected->index + widget->rowOffset].devilId != 0) {
        if (sbtlBtlDrawParamSub(widget) == 2) {
            sbtlCallGetDevilLevelUpUnit(widget->selected->child, 2);
        }
    } else {
        sbtlLeaf46(widget);
    }
    return 1;
}

s32 sbtlRefDDS3GBWK16(itfGridTextWidget_t *widget) {
    u32 step = 1;
    itfGridTextWidget_t *parent = widget->parent;

    if (widget->flags & 1) {
        if (dds3PadButtonState[58]) {
            step = 100;
        }
        if (dds3PadButtonState[59]) {
            step = 1000;
        }
        if (dds3PadButtonState[56]) {
            step = 10000;
        }
        if (dds3PadButtonState[54] & 2) {
            itfAdvanceSelectedGridScroll(widget, step);
        }
        if (dds3PadButtonState[55] & 2) {
            itfReverseSelectedGridScroll(widget, step);
        }
        DDS3_GBWK->playerData.stock[parent->selected->index + parent->rowOffset].exp = widget->selected->number;
        if (dds3PadButtonState[57]) {
            sbtlCallGetDevilLevelUpUnit(widget, 1);
        }
    }
    sbtlBtlDrawParamSub(widget);
    return 1;
}

/* NON_MATCHING: 0 of 51 words differ once sbtlCallGetDevilLevelUpUnit is C in this unit; with it as INCLUDE_ASM, the delay slot fill after the reverse-scroll call differs. Logic as retail. */
#ifdef NON_MATCHING
s32 sbtlRefDDS3GBWK17(itfGridTextWidget_t *widget) {
    itfGridTextWidget_t *parent = widget->parent;

    if (widget->flags & 1) {
        if (dds3PadButtonState[54] & 2) {
            itfAdvanceSelectedGridScroll(widget, 1);
        }
        if (dds3PadButtonState[55] & 2) {
            itfReverseSelectedGridScroll(widget, 1);
        }
        DDS3_GBWK->playerData.stock[parent->selected->index + parent->rowOffset].level = widget->selected->number;
        sbtlCallGetDevilLevelUpUnit(widget, 0);
    }
    sbtlBtlDrawParamSub(widget);
    return 1;
}
#else
INCLUDE_ASM(const s32, "code_0010F2C0", sbtlRefDDS3GBWK17);
#endif

s32 sbtlRefDDS3GBWK18(itfGridTextWidget_t *widget) {
    u32 step = 1;
    itfGridTextWidget_t *parent = widget->parent;
    s32 number;
    sdatUnitWork_t *unit;

    if (widget->flags & 1) {
        if (dds3PadButtonState[58]) {
            step = 10;
        }
        if (dds3PadButtonState[59]) {
            step = 100;
        }
        if (dds3PadButtonState[54] & 2) {
            itfAdvanceSelectedGridScroll(widget, step);
        }
        if (dds3PadButtonState[55] & 2) {
            itfReverseSelectedGridScroll(widget, step);
        }
        number = widget->selected->number;
        unit = &DDS3_GBWK->playerData.stock[parent->selected->index + parent->rowOffset];
        unit->now.hp = number;
        if (number > 0) {
            unit->badStatus &= ~BSTF_KANSEN;
        } else {
            unit->badStatus |= BSTF_KANSEN;
        }
        sbtlCallGetDevilLevelUpUnit(widget, 2);
    }
    sbtlBtlDrawParamSub(widget);
    return 1;
}

/* NON_MATCHING: 0 of 57 words differ once sbtlCallGetDevilLevelUpUnit is C in this unit; with it as INCLUDE_ASM, the delay slot fill after the reverse-scroll call differs. Logic as retail. */
#ifdef NON_MATCHING
s32 sbtlRefDDS3GBWK19(itfGridTextWidget_t *widget) {
    u32 step = 1;
    itfGridTextWidget_t *parent = widget->parent;

    if (widget->flags & 1) {
        if (dds3PadButtonState[58]) {
            step = 10;
        }
        if (dds3PadButtonState[59]) {
            step = 100;
        }
        if (dds3PadButtonState[54] & 2) {
            itfAdvanceSelectedGridScroll(widget, step);
        }
        if (dds3PadButtonState[55] & 2) {
            itfReverseSelectedGridScroll(widget, step);
        }
        DDS3_GBWK->playerData.stock[parent->selected->index + parent->rowOffset].now.mp = widget->selected->number;
        sbtlCallGetDevilLevelUpUnit(widget, 2);
    }
    sbtlBtlDrawParamSub(widget);
    return 1;
}
#else
INCLUDE_ASM(const s32, "code_0010F2C0", sbtlRefDDS3GBWK19);
#endif

s32 nbRefDDS3GBWK4(itfGridTextWidget_t *widget) {
    u32 step = 1;
    itfGridTextWidget_t *parent = widget->parent;

    if (widget->flags & 1) {
        if (dds3PadButtonState[58]) {
            step = 10;
        }
        if (dds3PadButtonState[59]) {
            step = 100;
        }
        if (dds3PadButtonState[54] & 2) {
            itfAdvanceSelectedGridScroll(widget, step);
        }
        if (dds3PadButtonState[55] & 2) {
            itfReverseSelectedGridScroll(widget, step);
        }
        DDS3_GBWK->playerData.stock[parent->selected->index + parent->rowOffset].chuusei = widget->selected->number;
    }
    sbtlBtlDrawParamSub(widget);
    return 1;
}

/* NON_MATCHING: 0 of 51 words differ once sbtlCallGetDevilLevelUpUnit is C in this unit; with it as INCLUDE_ASM, the delay slot fill after the reverse-scroll call differs. Logic as retail. */
#ifdef NON_MATCHING
s32 nbRefDDS3GBWK5(itfGridTextWidget_t *widget) {
    itfGridTextWidget_t *parent = widget->parent;

    if (widget->flags & 1) {
        if (dds3PadButtonState[54] & 2) {
            itfAdvanceSelectedGridScroll(widget, 1);
        }
        if (dds3PadButtonState[55] & 2) {
            itfReverseSelectedGridScroll(widget, 1);
        }
        DDS3_GBWK->playerData.stock[parent->selected->index + parent->rowOffset].backup.str = widget->selected->number;
        sbtlCallGetDevilLevelUpUnit(widget, 0);
    }
    sbtlBtlDrawParamSub(widget);
    return 1;
}
#else
INCLUDE_ASM(const s32, "code_0010F2C0", nbRefDDS3GBWK5);
#endif

/* NON_MATCHING: 0 of 51 words differ once sbtlCallGetDevilLevelUpUnit is C in this unit; with it as INCLUDE_ASM, the delay slot fill after the reverse-scroll call differs. Logic as retail. */
#ifdef NON_MATCHING
s32 nbRefDDS3GBWK6(itfGridTextWidget_t *widget) {
    itfGridTextWidget_t *parent = widget->parent;

    if (widget->flags & 1) {
        if (dds3PadButtonState[54] & 2) {
            itfAdvanceSelectedGridScroll(widget, 1);
        }
        if (dds3PadButtonState[55] & 2) {
            itfReverseSelectedGridScroll(widget, 1);
        }
        DDS3_GBWK->playerData.stock[parent->selected->index + parent->rowOffset].backup.tec = widget->selected->number;
        sbtlCallGetDevilLevelUpUnit(widget, 0);
    }
    sbtlBtlDrawParamSub(widget);
    return 1;
}
#else
INCLUDE_ASM(const s32, "code_0010F2C0", nbRefDDS3GBWK6);
#endif

/* NON_MATCHING: 0 of 51 words differ once sbtlCallGetDevilLevelUpUnit is C in this unit; with it as INCLUDE_ASM, the delay slot fill after the reverse-scroll call differs. Logic as retail. */
#ifdef NON_MATCHING
s32 nbRefDDS3GBWK7(itfGridTextWidget_t *widget) {
    itfGridTextWidget_t *parent = widget->parent;

    if (widget->flags & 1) {
        if (dds3PadButtonState[54] & 2) {
            itfAdvanceSelectedGridScroll(widget, 1);
        }
        if (dds3PadButtonState[55] & 2) {
            itfReverseSelectedGridScroll(widget, 1);
        }
        DDS3_GBWK->playerData.stock[parent->selected->index + parent->rowOffset].backup.vit = widget->selected->number;
        sbtlCallGetDevilLevelUpUnit(widget, 0);
    }
    sbtlBtlDrawParamSub(widget);
    return 1;
}
#else
INCLUDE_ASM(const s32, "code_0010F2C0", nbRefDDS3GBWK7);
#endif

/* NON_MATCHING: 0 of 51 words differ once sbtlCallGetDevilLevelUpUnit is C in this unit; with it as INCLUDE_ASM, the delay slot fill after the reverse-scroll call differs. Logic as retail. */
#ifdef NON_MATCHING
s32 nbRefDDS3GBWK8(itfGridTextWidget_t *widget) {
    itfGridTextWidget_t *parent = widget->parent;

    if (widget->flags & 1) {
        if (dds3PadButtonState[54] & 2) {
            itfAdvanceSelectedGridScroll(widget, 1);
        }
        if (dds3PadButtonState[55] & 2) {
            itfReverseSelectedGridScroll(widget, 1);
        }
        DDS3_GBWK->playerData.stock[parent->selected->index + parent->rowOffset].backup.luck = widget->selected->number;
        sbtlCallGetDevilLevelUpUnit(widget, 0);
    }
    sbtlBtlDrawParamSub(widget);
    return 1;
}
#else
INCLUDE_ASM(const s32, "code_0010F2C0", nbRefDDS3GBWK8);
#endif

s32 nbCallGetSmallFontFRQ2(s32 x, s32 y, s32 z, itfGridTextWidget_t *widget, s32 ot) {
    s32 drawY = y + ((widget->height + 12) << 3);
    s32 devilId = DDS3_GBWK->playerData.stock[widget->selected->index].devilId;
    frFontHandle_t *font;

    if (devilId != 0) {
        fldLmapSubmitScaledSpritePacket(x, drawY, z, 200, 28, 0x60000000, 0x80808080, ot);
        font = frwGetSmallFontFRQ2(x + 32, drawY + 32, z, 0xA09DC380, g_itemNo[devilId], NULL);
        frDrawFontOt(font, 1, ot);
        return frDestroyFontRequestAfter(font);
    }
#ifdef FIX_BUGS
    return 0;
#endif
}

void nbRefDDS3GBWK(itfGridTextWidget_t *widget, itfGridTextListItem_t *item, char *buf, s32 size) {
    s16 devilId = DDS3_GBWK->playerData.stock[item->index].devilId;

    if (devilId != 0) {
        sprintf(buf, sbtlData82, devilId);
    } else {
        sprintf(buf, nbDebugNoStatusLabel);
    }
}

INCLUDE_RODATA(const s32, "code_0010F2C0", sbtlCallCheckItemTable);

INCLUDE_RODATA(const s32, "code_0010F2C0", sbtlRefDDS3GBWK15Table);

void nbLEVEL(itfGridTextListItem_t *parentItem, u32 reference) {
    itfGridNumericDescriptor_t descriptor[9] = {
        {0, 0.0f, 99999992.0f, 1.0f, 0.0f},
        {0, 1.0f, 99.0f, 1.0f, 1.0f},
        {0, 0.0f, 999.0f, 1.0f, 1.0f},
        {0, 0.0f, 999.0f, 1.0f, 1.0f},
        {0, 0.0f, 100.0f, 1.0f, 0.0f},
        {0, 0.0f, 40.0f, 1.0f, 0.0f},
        {0, 0.0f, 40.0f, 1.0f, 0.0f},
        {0, 0.0f, 40.0f, 1.0f, 0.0f},
        {0, 0.0f, 40.0f, 1.0f, 0.0f},
    };
    s32 i = 0;
    itfGridTextWidget_t *list;
    itfGridTextWidget_t *widget;
    itfGridTextListItem_t *item;

    list = itfCreateGridTextWidget(nbData3, 0, 0, 0, DDS3_GBWK->playerData.partyNum, reference);
    list->onSelect = sbtlFunc8;
    list->onDraw = nbCallGetSmallFontFRQ2;
    list->flags |= 0x200;
    fclGohInitSkillChangeSub(parentItem, NULL, list);
    if (DDS3_GBWK->playerData.partyNum > 0) {
        do {
            item = itfAppendGridTextItem(list, nbLEVELData, 0);
            item->select = sbtlRefDDS3GBWK;
            item->format = nbRefDDS3GBWK;
            widget = itfCreateGridTextWidget(nbData3, 0, 0, 0, 9, reference);
            widget->onSelect = sbtlFunc8;
            i++;
            fclGohInitSkillChangeSub(item, list, widget);
            item = itfAppendGridTextItem(widget, nbLEVELData2, 0);
            item->select = sbtlRefDDS3GBWK16;
            itfSetGridNumericItemDescriptor(widget, item, &descriptor[0]);
            item = itfAppendGridTextItem(widget, nbLEVELData3, 0);
            item->select = sbtlRefDDS3GBWK17;
            itfSetGridNumericItemDescriptor(widget, item, &descriptor[1]);
            item = itfAppendGridTextItem(widget, nbLEVELData4, 0);
            item->select = sbtlRefDDS3GBWK18;
            itfSetGridNumericItemDescriptor(widget, item, &descriptor[2]);
            item = itfAppendGridTextItem(widget, nbLEVELData5, 0);
            item->select = sbtlRefDDS3GBWK19;
            itfSetGridNumericItemDescriptor(widget, item, &descriptor[3]);
            item = itfAppendGridTextItem(widget, nbLEVELData6, 0);
            item->select = nbRefDDS3GBWK4;
            itfSetGridNumericItemDescriptor(widget, item, &descriptor[4]);
            item = itfAppendGridTextItem(widget, nbLEVELData7, 0);
            item->select = nbRefDDS3GBWK5;
            itfSetGridNumericItemDescriptor(widget, item, &descriptor[5]);
            item = itfAppendGridTextItem(widget, nbLEVELData8, 0);
            item->select = nbRefDDS3GBWK6;
            itfSetGridNumericItemDescriptor(widget, item, &descriptor[6]);
            item = itfAppendGridTextItem(widget, nbLEVELData9, 0);
            item->select = nbRefDDS3GBWK7;
            itfSetGridNumericItemDescriptor(widget, item, &descriptor[7]);
            item = itfAppendGridTextItem(widget, nbLEVELData10, 0);
            item->select = nbRefDDS3GBWK8;
            itfSetGridNumericItemDescriptor(widget, item, &descriptor[8]);
            sbtlCallGetDevilLevelUpUnit(widget, 2);
        } while (i < DDS3_GBWK->playerData.partyNum);
    }
}

s32 nbRefDDS3GBWK9(itfGridTextWidget_t *widget) {
    u32 step = 1;

    if (widget->flags & 1) {
        if (dds3PadButtonState[58]) {
            step = 1000;
        }
        if (dds3PadButtonState[59]) {
            step = 10000;
        }
        if (dds3PadButtonState[56]) {
            step = 100000;
        }
        if (dds3PadButtonState[54] & 2) {
            itfAdvanceSelectedGridScroll(widget, step);
        }
        if (dds3PadButtonState[55] & 2) {
            itfReverseSelectedGridScroll(widget, step);
        }
        DDS3_GBWK->playerData.maka = widget->selected->number;
    }
    sbtlBtlDrawParamSub(widget);
    return 1;
}

s32 nbTOTAL(itfGridTextListItem_t *parentItem, u32 reference) {
    itfGridNumericDescriptor_t descriptor = {0, 0.0f, 9999999.0f, 1.0f, 0.0f};
    itfGridTextWidget_t *widget;
    itfGridTextListItem_t *item;
    s32 result;

    widget = itfCreateGridTextWidget(nbData3, 0, 0, 0, 1, reference);
    widget->onSelect = sbtlFunc8;
    fclGohInitSkillChangeSub(parentItem, NULL, widget);
    item = itfAppendGridTextItem(widget, nbTOTALData, 0);
    item->select = nbRefDDS3GBWK9;
    result = itfSetGridNumericItemDescriptor(widget, item, &descriptor);
    item->number = DDS3_GBWK->playerData.maka;
    return result;
}

s32 nbCallSetAgeOfMoon(itfGridTextWidget_t *widget) {
    if (widget->flags & 1) {
        if (dds3PadButtonState[54] & 2) {
            itfAdvanceSelectedGridScroll(widget, 1);
        }
        if (dds3PadButtonState[55] & 2) {
            itfReverseSelectedGridScroll(widget, 1);
        }
        evtSetAgeOfMoon(widget->selected->number);
    }
    sbtlBtlDrawParamSub(widget);
    return 1;
}

void GohInitSkillChange(itfGridTextListItem_t *parentItem, u32 reference) {
    itfGridNumericDescriptor_t descriptor = {0, 0.0f, 15.0f, 1.0f, 0.0f};
    itfGridTextWidget_t *widget;
    itfGridTextListItem_t *item;

    widget = itfCreateGridTextWidget(nbData3, 0, 0, 0, 1, reference);
    widget->onSelect = sbtlFunc8;
    fclGohInitSkillChangeSub(parentItem, NULL, widget);
    item = itfAppendGridTextItem(widget, GohInitSkillChangeData, 0);
    item->select = nbCallSetAgeOfMoon;
    itfSetGridNumericItemDescriptor(widget, item, &descriptor);
    item->number = evtGetAgeOfMoon();
}

s32 nbRefDDS3GBWK10(itfGridTextWidget_t *widget) {
    if (widget->flags & 1) {
        if (dds3PadButtonState[54] & 2) {
            itfAdvanceSelectedGridScroll(widget, 1);
        }
        if (dds3PadButtonState[55] & 2) {
            itfReverseSelectedGridScroll(widget, 1);
        }
        DDS3_GBWK->playerData.partyNum = widget->selected->number;
    }
    sbtlBtlDrawParamSub(widget);
    return 1;
}

s32 nbNUMS2(itfGridTextListItem_t *parentItem, u32 reference) {
    itfGridNumericDescriptor_t descriptor = {0, 6.0f, 12.0f, 1.0f, 6.0f};
    itfGridTextWidget_t *widget;
    itfGridTextListItem_t *item;
    s32 result;

    widget = itfCreateGridTextWidget(nbData3, 0, 0, 0, 1, reference);
    widget->onSelect = sbtlFunc8;
    fclGohInitSkillChangeSub(parentItem, NULL, widget);
    item = itfAppendGridTextItem(widget, nbNUMS2Data, 0);
    item->select = nbRefDDS3GBWK10;
    itfSetGridNumericItemDescriptor(widget, item, &descriptor);
    result = DDS3_GBWK->playerData.partyNum;
    item->number = result;
    return result;
}

nbUnitEditWork_t *nbCallMemAllocDirect(s32 mode) {
    u32 i = 0;
    sdfMemHandle_t *handle = sdfMemAllocDirect(16);
    nbUnitEditWork_t *work = sdfMemLock(handle);
    itfGridTextWidget_t *widget;
    itfGridTextListItem_t *item;

    memset(work, 0, 16);
    work->handle = handle;
    work->mode = mode;
    widget = itfCreateGridTextWidget(nbData3, 0, 0, 0, 7, 0);
    widget->onSelect = sbtlFunc8;
    work->widget = widget;
    widget->flags |= 4;
    for (;;) {
        item = itfAppendGridTextItem(work->widget, nbCallMemAllocDirectData[i].label, 0);
        item->select = sbtlBtlDrawParamSub;
        if (nbCallMemAllocDirectData[i].create) {
            nbCallMemAllocDirectData[i].create(item, mode);
        }
        if (++i >= 7) {
            break;
        }
    }
    return work;
}

void nbCallGetProcessData31(dds3ProcessID_t *id) {
    nbUnitEditWork_t *work = dds3GetProcessData(id);

    if (work != NULL) {
        itfDestroyGridTextWidgetTree(work->widget);
        sdfMemFreeDirect(work->handle);
    }
}

s32 nbGetEventForm(dds3ProcessID_t *id) {
    nbUnitEditWork_t *work = dds3GetProcessData(id);

    fldLmapDrawListTree(512, 256, 0, work->widget, 112);
    return 0;
}

s32 nbUnitEdit(void) {
    if (nbData2 == NULL) {
        nbData2 = dds3StartProcess(nbStrUnitEdit, 10000, 1, 1, nbGetEventForm,
                                   (dds3ProcessFunc_t)nbCallGetProcessData31, nbCallMemAllocDirect(0));
    }
    return 0;
}

void nbCallKillProcess(void) {
    dds3KillProcess(nbData2, 0);
    nbData2 = NULL;
}

INCLUDE_RODATA(const s32, "code_0010F2C0", nbStrUnitEdit);

