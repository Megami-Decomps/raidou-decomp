#include "common.h"
#include "dds3KernelCore.h"
#include "fldNme.h"
#include "code_r1_002C8188.h"
#include "sdfMemory.h"
#include "cmbFont.h"
#include "nmeMain.h"
#include "Quest.h"
#include "mselGlobal.h"
#include "scrTraceCode.h"
#include "itf.h"
#include "sdk_libc.h"
#include "nmeInput0.h"

void fldCallMemGetPtr7(dds3ProcessID_t *id) {
    fldNmeWork_t *work = sdfMemGetPtr(id->workdata);
    fldNmeListInit_t init[2] = {
        { work->slot0, 8, NULL, 0 },
        { work->slot1, 8, NULL, 0 },
    };
    u32 i;

    for (i = 0; i < 2; i++) {
        fclLeaf25(&work->list[i], init[i].slot, init[i].count, 4);
        fclCallSetFontString(&work->list[i], init[i].text, init[i].length);
    }
}

INCLUDE_ASM(const s32, "code_0035E5B8", fldNakamaCashLoad_Decode);

/* NON_MATCHING: 51 of 119 words match.  */
#ifdef NON_MATCHING
s32 fldCallMemGetPtr3(dds3ProcessID_t *id, s32 *index) {
    fldNmeWork_t *work = sdfMemGetPtr(id->workdata);
    fldNmeSlotList_t *list;
    u32 i;
    u32 j;
    s32 used;
    s32 n;
    s32 match;
    s32 ids[4];

    list = work->list;
    for (i = 0; i < 2; i++) {
        used = 0;
        for (j = 0; j < list->count; j++) {
            if (list->slot[j].id != 0xFFFF) {
                used++;
            }
        }
        if (used == 0) {
            if (index != NULL) {
                *index = i;
            }
            return 0;
        }
        list++;
    }
    match = FALSE;
    n = 0;
    used = 0;
    for (j = 0; j < work->list[1].count; j++) {
        if (work->list[1].slot[j].id != 0xFFFF) {
            used++;
            if (n < 2) {
                ids[n++] = work->list[1].slot[j].id;
            }
        }
    }
    if (used == 2 && ids[0] == 597) {
        match = ids[1] == 0xBCB;
    }
    if (!match) {
        return 1;
    }
    n = 0;
    used = 0;
    for (j = 0; j < work->list[0].count; j++) {
        if (work->list[0].slot[j].id != 0xFFFF) {
            used++;
            if (n < 4) {
                ids[n++] = work->list[0].slot[j].id;
            }
        }
    }
    if (used == 4 && ids[0] == 119 && ids[1] == 82 && ids[2] == 141 && ids[3] == 83) {
        if (index != NULL) {
            *index = -1;
        }
        return 0;
    }
    return 1;
}
#else
INCLUDE_ASM(const s32, "code_0035E5B8", fldCallMemGetPtr3);
#endif

void fldCallMemGetPtr8(dds3ProcessID_t *id) {
    fldNmeWork_t *work = sdfMemGetPtr(id->workdata);
    dds3NameChar_t name[16];
    u32 length;

    fclFunc11(&work->list[0], name, &length);
    nbRefDDS3GBWK12(0, name, length, 4);
    fclFunc11(&work->list[1], name, &length);
    nbRefDDS3GBWK12(1, name, length, 4);
}

void fldCallMemGetPtr9(dds3ProcessID_t *id) {
    fldNmeWork_t *work = sdfMemGetPtr(id->workdata);

    fldFunc24(&work->draw, 0, work->unk8 == 2);
    fldFunc25(&work->draw, 0);
}

void fldCallMemGetPtr10(dds3ProcessID_t *id) {
    fldNmeWork_t *work = sdfMemGetPtr(id->workdata);

    fclCallSetFontColorNo2(&work->list[1], work->unk14 == 1, 173.0f, 138.0f, 21.0f);
    fclCallSetFontColorNo2(&work->list[0], work->unk14 == 0, 173.0f, 103.0f, 21.0f);
    fldLeaf33(&work->draw, work->unk14, work->list[work->unk14].unk2);
}

INCLUDE_ASM(const s32, "code_0035E5B8", fldCallMemGetPtr11);

INCLUDE_ASM(const s32, "code_0035E5B8", fldCallMemGetPtr4);

void fldFunc20(fldNmeDraw_t *draw) {
    if (draw->hSpr != NULL) {
        effDestroyResourceSlotSet(draw->hSpr);
        draw->hSpr = NULL;
    }
}

void fldNameNameSpr(fldNmeDraw_t *draw) {
    u32 data = 0;
    struct sdfMemHandle_t *allocation;

    fldFunc20(draw);
    allocation = sdfDevLoadFileEx(fldStrNameNameSpr, &data, 0);
    draw->hSpr = effCreateResourceSlotSetFromAllocation(allocation, 0);
    sdfMemFreeDelayedDirect(allocation);
}

void fldLeaf29(fldNmeDraw_t *draw, s32 x, s32 y, s32 z, u32 alpha, s16 *list) {
    itfDrawMiscSpriteList(x, y, z, alpha, draw->hSpr, 104, (itfSprList_t *)list);
}

void SetSelect(fldNmeDraw_t *draw) {
    itfDrawMiscSpriteAlpha(0, 0, 0xFFF, 255, draw->hSpr, 0, 13);
}

INCLUDE_ASM(const s32, "code_0035E5B8", fldLeaf30);

INCLUDE_ASM(const s32, "code_0035E5B8", fldLeaf31);

INCLUDE_ASM(const s32, "code_0035E5B8", fldFunc21);

void fldLeaf32(fldNmeDraw_t *draw, s32 x, s32 y) {
    itfDrawMiscSpriteAlpha(x, y, 0, 255, draw->hSpr, 40, 104);
}

void fldCallEnableMode(s32 x, s32 y, s32 color, u8 *text) {
    frFontHandle_t *font;

    if (text != NULL) {
        frEnableMode(1);
        font = frSetFontString(text, 0, color, 0, NULL);
        frSetFontSpace(font, 0);
        frSetDrawPosition(font, x * 16, y * 8);
        frDrawFont(font);
        frDestroyFontRequestAfter(font);
        frDisableMode(1);
    }
}

void fldCallEnableMode2(s32 x, s32 y, s32 color, u8 *text) {
    frFontHandle_t *font;

    if (text != NULL) {
        frEnableMode(1);
        font = frSetFontString(text, 0, color, 0, NULL);
        frSetFontSpace(font, 0);
        frSetDrawPosition(font, x * 16, y * 8);
        frSetFontSize(font, 18, 20);
        frDrawFont(font);
        frDestroyFontRequestAfter(font);
        frDisableMode(1);
    }
}

INCLUDE_ASM(const s32, "code_0035E5B8", fldCallSetFontString3);

INCLUDE_ASM(const s32, "code_0035E5B8", fldFunc22);

INCLUDE_RODATA(const s32, "code_0035E5B8", fldStrNameNameSpr);

void fldFunc23(s32 flag) {
    s32 pos[5][2] = { { 243, 412 }, { 333, 412 }, { 435, 412 }, { 69, 412 }, { 156, 412 } };
    s32 count;
    s32 i;

    count = (flag == 0) ? 5 : 3;
    for (i = 0; i < count; i++) {
        fldCallEnableMode2(pos[i][0], pos[i][1], 0, fldFunc23Data[i + 1]);
    }
}

void fldFunc24(fldNmeDraw_t *draw, s32 arg1, s32 arg2) {
    if (arg1 == 0) {
        fldLeaf29(draw, 0, 0, 0, 255, fldFunc24Data);
        if (arg2 == 0) {
            fldLeaf29(draw, 0, 0, 0, 255, fldFunc24Data2);
        } else {
            fldLeaf29(draw, 0, 0, 0, 255, fldFunc24Data3);
        }
    } else {
        fldLeaf29(draw, 0, 0, 0, 255, fldFunc24Data4);
        if (arg2 == 0) {
            fldLeaf29(draw, 0, 0, 0, 255, fldFunc24Data5);
        } else {
            fldLeaf29(draw, 0, 0, 0, 255, fldFunc24Data6);
        }
    }
}

void fldFunc25(fldNmeDraw_t *draw, s32 flag) {
    if (flag == 0) {
        fldLeaf29(draw, 0, 0, 0, 255, fldFunc25Data);
    } else {
        fldLeaf29(draw, 0, 0, 0, 255, fldFunc25Data2);
    }
    fldFunc23(flag);
}

void fldLeaf33(fldNmeDraw_t *draw, s32 row, s32 column) {
    s32 y = 162;

    if (row != 1) {
        if (row != 0) {
            return;
        }
        y = 127;
    }
    itfDrawMiscSpriteAlpha(column * 21 + 173, y, 0, 255, draw->hSpr, 41, 104);
}

INCLUDE_ASM(const s32, "code_0035E5B8", fclLeaf14);

void fclFunc3(fldNmeDraw_t *draw) {
    u32 i;

    for (i = 0; i < 90; i++) {
        draw->glyph[i] = NULL;
    }
    for (i = 0; i < 44; i++) {
        draw->glyph2[i] = NULL;
    }
    fclCallSetSpriteNormal(draw, 0, 0);
    mselCallMInitSceneMjn();
    draw->hSpr = NULL;
    fldNameNameSpr(draw);
}

void fclFunc4(fldNmeDraw_t *draw) {
    frFontHandle_t **glyph;
    u32 i;

    glyph = draw->glyph2;
    for (i = 0; i < 44; i++) {
        if (*glyph != NULL) {
            frCallFlushGsFontTable(*glyph);
        }
        glyph++;
    }
    fclFunc5(draw);
    fclLeaf20(draw);
    fldFunc19();
    fldFunc20(draw);
}

void fclLeaf15(fldNmeDraw_t *draw) {
    fldLeaf29(draw, 0, 175, 0, 255, fclLeaf15Data);
}

void fclCallSetSpriteNormal(fldNmeDraw_t *draw, u32 page, s32 index) {
    dds3NameChar_t ch[2];
    s32 codePage;
    s32 codeIndex;
    u32 code;
    u32 count;
    s32 slot;
    u32 i;
    u32 j;
    u32 x;
    u32 y;

    if (page >= 3) {
        if (page == 3) {
            fclLeaf17(draw);
            ch[1].bytes[0] = 0;
            for (j = 0; j < titleStartSub2(page); j++) {
                code = titleFunc6(index + j);
                fclLeaf18(draw, code);
                titleFunc4(code, &codePage, &codeIndex);
                count = fclLeaf23(codePage) - codeIndex;
                if (titleStartSub(page) < count) {
                    count = titleStartSub(page);
                }
                slot = fclFunc7(draw, code);
                for (i = 0; i < count; i++) {
                    ch[0].code = fclSetSpriteNormal(codePage, codeIndex++);
                    draw->glyph[slot + i] = frSetFontString(ch[0].bytes, 4, 0, 0, NULL);
                }
            }
            for (j = 0; j < 44; j++) {
                ch[0].code = fclSetSpriteNormalSub(j);
                draw->glyph2[j] = frSetFontString(ch[0].bytes, 4, 0, 0, NULL);
            }
        }
    } else {
        ch[1].bytes[0] = 0;
        for (y = 0; y < titleStartSub2(page); y++) {
            for (x = 0; x < titleStartSub(page); x++) {
                if (titleFunc(page, x, y)) {
                    ch[0].code = fclSetSpriteNormalSub(nmeLeaf2(page, x, y));
                    draw->glyph[y * titleStartSub(page) + x] = frSetFontString(ch[0].bytes, 4, 0, 0, NULL);
                }
            }
        }
    }
}

INCLUDE_ASM(const s32, "code_0035E5B8", fclNakamaCashLoadDecodeSub);

INCLUDE_ASM(const s32, "code_0035E5B8", fclLeaf16);

void fclFunc5(fldNmeDraw_t *draw) {
    u32 i;

    for (i = 0; i < 90; i++) {
        if (draw->glyph[i] != NULL) {
            frCallFlushGsFontTable(draw->glyph[i]);
            draw->glyph[i] = NULL;
        }
    }
    for (i = 0; i < 44; i++) {
        frCallFlushGsFontTable(draw->glyph2[i]);
        draw->glyph2[i] = NULL;
    }
}

INCLUDE_ASM(const s32, "code_0035E5B8", fclCallSetFontColorNo);

void fclFunc6(fldNmeDraw_t *draw, s32 mode, s32 index) {
    fldLeaf29(draw, 400, 256, 0, 255, fldLeaf30Data);
    if (mode == 1) {
        fldLeaf30(draw, index);
    }
}

void fclCallSetSelect(fldNmeDraw_t *draw, s32 flag) {
    SetSelect(draw);
    Start_nmeMain();
    if (flag == 0) {
        fldLeaf29(draw, 0, 0, 0, 255, fclCallSetSelectData);
    } else {
        fldLeaf29(draw, 0, 0, 0, 255, fclCallSetSelectData2);
    }
}

INCLUDE_ASM(const s32, "code_0035E5B8", fclLeaf17);

INCLUDE_ASM(const s32, "code_0035E5B8", fclLeaf18);

INCLUDE_ASM(const s32, "code_0035E5B8", fclFunc7);

void fclLeaf19(fldNmeDraw_t *draw) {
}

void fclLeaf20(fldNmeDraw_t *draw) {
}

void fclFunc8(fldNmeDraw_t *draw, s32 row) {
    fldCallEnableMode(60, 239, 0, fclFunc8Data2[1]);
    fldCallEnableMode(117, 271, 0, fclFunc8Data2[2]);
    fldCallEnableMode(117, 297, 0, fclFunc8Data2[3]);
    fldLeaf29(draw, 113, row * 26 + 270, 0, 255, fclFunc8Data);
}

INCLUDE_ASM(const s32, "code_0035E5B8", fclLoadFileAsync);

void fclFirstInitSub(void) {
    u32 first = 0;
    u32 i;

    for (i = 0; i < 44; i++) {
        fclWork[i] = first;
        first += fclFirstInitSubData[i];
    }
    fclWork[i] = first;
}

u16 fclSetSpriteNormalSub(s32 index) {
    return fclData[index];
}

INCLUDE_ASM(const s32, "code_0035E5B8", fclLeaf21);

void fclLeaf22(frFontHandle_t *font, u32 index, f32 x, f32 y) {
    if (index < 241) {
        x += fclLeaf22Data[index][0];
        y += fclLeaf22Data[index][1];
    }
    frSetDrawPosition(font, (s32)(x * 16.0f), (s32)(y * 8.0f));
}

INCLUDE_ASM(const s32, "code_0035E5B8", fclSetSpriteNormal);

INCLUDE_ASM(const s32, "code_0035E5B8", fclLeaf23);

INCLUDE_ASM(const s32, "code_0035E5B8", fclLeaf24);

void fclLeaf25(fldNmeSlotList_t *list, fldNmeSlot_t *slot, s32 count, s32 unk4) {
    u32 i;

    list->count = count;
    list->unk4 = unk4;
    list->unk2 = 0;
    list->slot = slot;
    for (i = 0; i < list->count; i++) {
        slot[i].id = -1;
    }
}

void fclCallSetFontString(fldNmeSlotList_t *list, u16 *text, u32 n) {
    fldNmeSlot_t *slot;
    dds3NameChar_t ch[2];
    u32 count;
    u32 i;

    count = list->count;
    for (i = 0; i < n; i++) {
        slot = &list->slot[i];
        slot->id = fclLeaf21(text[i]);
        ch[0].code = fclSetSpriteNormalSub(slot->id);
        ch[1].bytes[0] = 0;
        slot->font = frSetFontString(ch[0].bytes, (s8)list->unk4, 0, 0, NULL);
    }
    for (i = n; i < count; i++) {
        list->slot[i].id = -1;
    }
}

void fclCallSetFontString2(fldNmeSlotList_t *list, s32 id) {
    fldNmeSlot_t *slot;
    dds3NameChar_t text[2];

    slot = &list->slot[list->unk2];
    if (slot->id != 0xFFFF) {
        frCallFlushGsFontTable(slot->font);
    }
    slot->id = id;
    text[0].code = fclSetSpriteNormalSub(slot->id);
    text[1].bytes[0] = 0;
    slot->font = frSetFontString(text[0].bytes, (s8)list->unk4, 0, 0, NULL);
}

INCLUDE_ASM(const s32, "code_0035E5B8", fclFunc9);

void fclCallSetFontColorNo2(fldNmeSlotList_t *list, s32 color, f32 x, f32 y, f32 dx) {
    u32 i;
    u16 id;

    for (i = 0; i < list->count; i++) {
        id = list->slot[i].id;
        if (id != 0xFFFF) {
            fclLeaf22(list->slot[i].font, id, x + i * dx, y);
            frSetFontColorNo(list->slot[i].font, color);
            frDrawFont(list->slot[i].font);
        }
    }
}

INCLUDE_ASM(const s32, "code_0035E5B8", fclCallSetFontColorNo3);

void fclFunc10(fldNmeSlotList_t *list) {
    u32 i;

    for (i = 0; i < list->count; i++) {
        if (list->slot[i].id != 0xFFFF) {
            frCallFlushGsFontTable(list->slot[i].font);
        }
    }
}

INCLUDE_ASM(const s32, "code_0035E5B8", fclCallMemset);

void fclFunc11(fldNmeSlotList_t *list, dds3NameChar_t *name, u32 *length) {
    u32 count = list->count;
    u32 i = count - 1;
    u32 n;

    while (list->slot[i].id == 0xFFFF) {
        i--;
    }
    count = i + 1;
    n = 0;
    for (i = 0; i < count; i++) {
        if (list->slot[i].id == 0xFFFF) {
            name[n].code = 0x8080;
        } else {
            name[n].code = fclSetSpriteNormalSub(list->slot[i].id);
        }
        n++;
    }
    *length = n;
}

INCLUDE_ASM(const s32, "code_0035E5B8", fclNmeDebugProcess);

INCLUDE_ASM(const s32, "code_0035E5B8", fclNameNmeDebug);

INCLUDE_ASM(const s32, "code_0035E5B8", fclFunc12);

INCLUDE_ASM(const s32, "code_0035E5B8", fclCallMemGetPtr);

INCLUDE_ASM(const s32, "code_0035E5B8", effNimProcess);

INCLUDE_ASM(const s32, "code_0035E5B8", effCallMemGetPtr);

INCLUDE_ASM(const s32, "code_0035E5B8", effFunc21);

INCLUDE_ASM(const s32, "code_0035E5B8", effFunc22);

INCLUDE_ASM(const s32, "code_0035E5B8", effCallMemGetPtr2);

INCLUDE_ASM(const s32, "code_0035E5B8", effCallMemGetPtr3);

INCLUDE_ASM(const s32, "code_0035E5B8", effCallMemGetPtr4);

INCLUDE_ASM(const s32, "code_0035E5B8", effCallMemGetPtr5);

INCLUDE_ASM(const s32, "code_0035E5B8", effLeaf402);

INCLUDE_ASM(const s32, "code_0035E5B8", effMesMngSetVarItfSub);

INCLUDE_ASM(const s32, "code_0035E5B8", effLeaf403);

INCLUDE_ASM(const s32, "code_0035E5B8", effCallMemGetPtr6);

INCLUDE_ASM(const s32, "code_0035E5B8", effCallMemGetPtr7);

INCLUDE_ASM(const s32, "code_0035E5B8", effCallMemGetPtr8);

INCLUDE_ASM(const s32, "code_0035E5B8", effCallMemGetPtr9);

INCLUDE_ASM(const s32, "code_0035E5B8", effPCP_HBED2_SetRotateMatrix);

INCLUDE_ASM(const s32, "code_0035E5B8", effCallMemGetPtr10);

INCLUDE_ASM(const s32, "code_0035E5B8", effCallMemGetPtr11);

INCLUDE_ASM(const s32, "code_0035E5B8", effCallMemGetPtr12);

INCLUDE_ASM(const s32, "code_0035E5B8", effBTLSkyFilterOFF);

INCLUDE_ASM(const s32, "code_0035E5B8", effCallMemGetPtr13);

INCLUDE_ASM(const s32, "code_0035E5B8", effCallMemGetPtr14);

INCLUDE_ASM(const s32, "code_0035E5B8", effCallMemGetPtr15);

void effFirstInitSub(void) {
    effData45 = 3;
}

INCLUDE_ASM(const s32, "code_0035E5B8", effLeaf404);

INCLUDE_ASM(const s32, "code_0035E5B8", effCallGetIntPara);

INCLUDE_ASM(const s32, "code_0035E5B8", fldCallGetIntPara5);

INCLUDE_ASM(const s32, "code_0035E5B8", fldNmeRootProcess);

INCLUDE_ASM(const s32, "code_0035E5B8", fldCallMemGetPtr12);

INCLUDE_ASM(const s32, "code_0035E5B8", fldFunc26);

INCLUDE_ASM(const s32, "code_0035E5B8", fldCallMemGetPtr13);

INCLUDE_ASM(const s32, "code_0035E5B8", fldCommand_SET_ALLPANEL);

INCLUDE_ASM(const s32, "code_0035E5B8", fldFunc27);

INCLUDE_ASM(const s32, "code_0035E5B8", nmeCallExistProcess);

s32 nmeScrCmd_CALL_NAME(void) {
    scrProcessWork_s *work;
    dds3ProcessID_t *proc;

    work = scrGetScriptHandle();
    if (work == NULL) {
        printf(nmeStr);
        return 1;
    }
    if (work->ProcessID == NULL) {
        printf(nmeStrCallName);
        return 1;
    }
    switch ((u32)scrGetIntPara(0)) {
    default:
        printf(nmeStr2);
        return 1;
    case 0:
        proc = fldNmeProcess();
        break;
    case 1:
        proc = nmeStart(0);
        break;
    case 2:
        proc = nmeStart(1);
        break;
    }
    dds3LinkProcess(work->ProcessID, proc);
    scrSetIntReturnValue((s32)proc);
    return 1;
}

s32 nmeScrCmd_CALL_NAME_WAIT(void)
{
    return dds3ExistProcess(EE_ADDR_AS(dds3ProcessID_t *, scrGetIntPara(0))) == 0;
}

INCLUDE_ASM(const s32, "code_0035E5B8", ScrCmd_GET_DREAM_COMPARE);

INCLUDE_ASM(const s32, "code_0035E5B8", nmeStart);

INCLUDE_ASM(const s32, "code_0035E5B8", nmeFinish);

INCLUDE_ASM(const s32, "code_0035E5B8", nmeCallMemGetPtr);

INCLUDE_ASM(const s32, "code_0035E5B8", nmeCallMemGetPtr2);

INCLUDE_ASM(const s32, "code_0035E5B8", nmeCallMemGetPtr3);

INCLUDE_ASM(const s32, "code_0035E5B8", nmeCallMemGetPtr4);

INCLUDE_ASM(const s32, "code_0035E5B8", nmeCallMemGetPtr5);

INCLUDE_ASM(const s32, "code_0035E5B8", nmeCallMemGetPtr6);

INCLUDE_ASM(const s32, "code_0035E5B8", nmeCallMemGetPtr7);

INCLUDE_ASM(const s32, "code_0035E5B8", nmeCallMemGetPtr8);

INCLUDE_ASM(const s32, "code_0035E5B8", nmeCallMemGetPtr9);

INCLUDE_ASM(const s32, "code_0035E5B8", nmeCallMemGetPtr10);

INCLUDE_ASM(const s32, "code_0035E5B8", nmeCallMemGetPtr11);

INCLUDE_ASM(const s32, "code_0035E5B8", nmeCallMemGetPtr12);

INCLUDE_ASM(const s32, "code_0035E5B8", nmeScrCmdGETDREAMCOMPARESub);

u16 nmeLeaf2(s32 page, s32 x, s32 y) {
    switch (page) {
        case 2:
            return nmeLeaf2Data[y][x];
        case 1:
            return nmeLeaf2Data2[y][x];
        case 0:
            return nmeLeaf2Data3[y][x];
    }
    return 0;
}
