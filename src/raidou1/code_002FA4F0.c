#include "common.h"
#include "libsdf.h"
#include "kernel.h"
#include "basic.h"
#include "facility.h"
#include "event.h"
#include "dds3SceneBasic.h"
#include "dds3KernelCore.h"
#include "scr.h"
#include "fldNme.h"
#include "itf.h"
#include "code_r1_002C8188.h"
#include "sdf_resource.h"
#include "sdfMemory.h"
#include "cmbFont.h"
#include "nmeMain.h"
#include "Quest.h"
#include "mselGlobal.h"
#include "nmeInput0.h"
#include "scrTraceCode.h"
#include "sdk_libc.h"

extern const char nmeStr[];
extern const char nmeStrCallName[];
extern const char nmeStr2[];

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

INCLUDE_ASM(const s32, "code_002FA4F0", fldNakamaCashLoad_Decode);

s32 fldCallMemGetPtr3(dds3ProcessID_t *id, u32 *index) {
    fldNmeWork_t *work = sdfMemGetPtr(id->workdata);
    fldNmeSlotList_t *list;
    u32 i;
    u32 j;
    s32 used;

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
    return 1;
}

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

/* NON_MATCHING: 44 of 57 words match.  */
#ifdef NON_MATCHING
void fldCallMemGetPtr11(dds3ProcessID_t *id) {
    fldNmeWork_t *work = sdfMemGetPtr(id->workdata);
    u32 i;
    u32 count;

    count = work->list[1].count;
    for (i = 0; i < count; i++) {
    }
    count = work->list[0].count;
    for (i = 0; i < count; i++) {
    }
    fclCallSetFontColorNo2(&work->list[1], 0, 173.0f, 138.0f, 21.0f);
    fclCallSetFontColorNo2(&work->list[0], 0, 173.0f, 103.0f, 21.0f);
    fclFunc8(&work->draw, work->unk40);
}
#else
INCLUDE_ASM(const s32, "code_002FA4F0", fldCallMemGetPtr11);
#endif

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
    itfDrawMiscSpriteList(x, y, z, alpha, draw->hSpr, 103, (itfSprList_t *)list);
}

void SetSelect(fldNmeDraw_t *draw) {
    itfCallSetOt2(0, 0, 0xFFF, 255, draw->hSpr, 0, 13);
}

INCLUDE_ASM(const s32, "code_002FA4F0", fldLeaf30);

INCLUDE_ASM(const s32, "code_002FA4F0", fldLeaf31);

INCLUDE_ASM(const s32, "code_002FA4F0", fldFunc21);

void fldLeaf32(fldNmeDraw_t *draw, s32 x, s32 y) {
    itfCallSetOt2(x, y, 0, 255, draw->hSpr, 40, 103);
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

INCLUDE_ASM(const s32, "code_002FA4F0", fldCallSetFontString3);

INCLUDE_ASM(const s32, "code_002FA4F0", fldFunc22);

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
    itfCallSetOt2(column * 21 + 173, y, 0, 255, draw->hSpr, 41, 103);
}

INCLUDE_ASM(const s32, "code_002FA4F0", fclLeaf14);

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

INCLUDE_ASM(const s32, "code_002FA4F0", fclNakamaCashLoadDecodeSub);

INCLUDE_ASM(const s32, "code_002FA4F0", fclLeaf16);

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

/* NON_MATCHING: 70 of 440 words match.  */
#ifdef NON_MATCHING
void fclCallSetFontColorNo(fldNmeDraw_t *draw, u32 page, s32 index, s32 mode, u32 col, s32 row) {
    s32 codePage;
    s32 codeIndex;
    frFontHandle_t **glyph;
    u32 height;
    u32 width;
    u32 count;
    u32 code;
    u32 x;
    u32 y;
    s32 last;
    u8 color;
    s32 top;
    s32 posX;
    s32 posY;

    switch (page) {
        case 3:
        fldLeaf29(draw, 0, 175, 0, 255, fclCallSetFontColorNoData);
        height = titleStartSub2(3);
        width = titleStartSub(3);
        last = -1;
        for (y = 0; y < height; y++) {
            code = titleFunc6(index + y);
            titleFunc4(code, &codePage, &codeIndex);
            if (last != codePage) {
                color = codeIndex != 0 ? 0 : 5;
                last = codePage;
                posY = y * 27 + 205;
                fldLeaf31(draw, 64, posY);
                fldFunc22(codePage, 64, posY, color);
            }
        }
        for (y = 0; y < height; y++) {
            code = titleFunc6(index + y);
            titleFunc4(code, &codePage, &codeIndex);
            glyph = &draw->glyph[fclFunc7(draw, code)];
            count = fclLeaf23(codePage) - codeIndex;
            if (width < count) {
                count = width;
            }
            for (x = 0; x < count; x++) {
                code = fclLeaf24(codePage, codeIndex);
                if (mode != 0) {
                    color = 0;
                } else if (y == row - index) {
                    color = 1;
                    if (x != col) {
                        color = 3;
                    }
                } else {
                    color = 0;
                }
                fclLeaf22(*glyph, code, x * 21 + 64, y * 27 + 205);
                frSetFontColorNo(*glyph, color);
                frDrawFont(*glyph++);
                codeIndex++;
            }
        }
            break;
        case 1:
        case 2:
            glyph = draw->glyph;
            height = titleStartSub2(page);
            width = titleStartSub(page);
            for (y = 0; y < height; y++) {
                for (x = 0; x < width; x++, glyph++) {
                    if (titleFunc(page, x, y)) {
                        code = nmeLeaf2(page, x, y);
                        if (mode == 0 && y == row && x == col) {
                            color = 1;
                        } else {
                            color = 0;
                        }
                        fclLeaf22(*glyph, code, x * 21 + (x / 5) * 10 + 37, y * 27 + 205);
                        frSetFontColorNo(*glyph, color);
                        frDrawFont(*glyph);
                    }
                }
            }
            break;
        case 0:
            glyph = draw->glyph;
            height = titleStartSub2(0);
            width = titleStartSub(0);
            for (y = 0; y < height; y++) {
                for (x = 0; x < width; x++, glyph++) {
                    if (titleFunc(0, x, y)) {
                        code = nmeLeaf2(0, x, y);
                        if (mode == 0 && y == row && x == col) {
                            color = 1;
                        } else {
                            color = 0;
                        }
                        fclLeaf22(*glyph, code, x * 21 + 37, y * 27 + 205);
                        frSetFontColorNo(*glyph, color);
                        frDrawFont(*glyph);
                    }
                }
            }
            break;
    }
    if (mode == 0) {
        posX = -1;
        posY = -1;
        if (page < 3) {
            if (page == 0) {
                posX = col * 21 + 37;
                posY = (row - index) * 27 + 205;
            } else {
                posX = col * 21 + (col / 5) * 10 + 37;
                posY = (row - index) * 27 + 205;
            }
        } else if (page == 3) {
            posX = col * 21 + 64;
            posY = (row - index) * 27 + 205;
        }
        fldLeaf32(draw, posX, posY);
    } else if (mode == 2) {
        titleFunc4(row, &codePage, &codeIndex);
        top = titleFunc5(codePage) - index;
        if (top < 0) {
            top = 0;
        }
        fldFunc21(draw, 64, top * 27 + 205);
    }
}
#else
INCLUDE_ASM(const s32, "code_002FA4F0", fclCallSetFontColorNo);
#endif

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

INCLUDE_ASM(const s32, "code_002FA4F0", fclLeaf17);

INCLUDE_ASM(const s32, "code_002FA4F0", fclLeaf18);

INCLUDE_ASM(const s32, "code_002FA4F0", fclFunc7);

/* body: matched in raidou @ 112d69c: src/shared/code_r1_002CD5A8.c:95, found, same function 1.00 (words 2/2, calls 1.00, shape 1.00) */
void fclLeaf19(void) {
}

void fclLeaf20(fldNmeDraw_t *draw) {
}

void fclFunc8(fldNmeDraw_t *draw, s32 row) {
    fldCallEnableMode(107, 239, 0, fclFunc8Data2[1]);
    fldCallEnableMode(117, 271, 0, fclFunc8Data2[2]);
    fldCallEnableMode(117, 297, 0, fclFunc8Data2[3]);
    fldLeaf29(draw, 113, row * 26 + 270, 0, 255, fclFunc8Data);
}

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

INCLUDE_ASM(const s32, "code_002FA4F0", fclLeaf21);

void fclLeaf22(frFontHandle_t *font, u32 index, f32 x, f32 y) {
    if (index < 241) {
        x += fclLeaf22Data[index][0];
        y += fclLeaf22Data[index][1];
    }
    frSetDrawPosition(font, (s32)(x * 16.0f), (s32)(y * 8.0f));
}

INCLUDE_ASM(const s32, "code_002FA4F0", fclSetSpriteNormal);

INCLUDE_ASM(const s32, "code_002FA4F0", fclLeaf23);

INCLUDE_ASM(const s32, "code_002FA4F0", fclLeaf24);

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

INCLUDE_ASM(const s32, "code_002FA4F0", fclFunc9);

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

INCLUDE_ASM(const s32, "code_002FA4F0", fclCallSetFontColorNo3);

void fclFunc10(fldNmeSlotList_t *list) {
    u32 i;

    for (i = 0; i < list->count; i++) {
        if (list->slot[i].id != 0xFFFF) {
            frCallFlushGsFontTable(list->slot[i].font);
        }
    }
}

INCLUDE_ASM(const s32, "code_002FA4F0", fclCallMemset);

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

INCLUDE_ASM(const s32, "code_002FA4F0", fclNmeDebugProcess);

INCLUDE_ASM(const s32, "code_002FA4F0", fclNameNmeDebug);

INCLUDE_ASM(const s32, "code_002FA4F0", fclFunc12);

INCLUDE_ASM(const s32, "code_002FA4F0", fclCallMemGetPtr);

INCLUDE_ASM(const s32, "code_002FA4F0", effNimProcess);

INCLUDE_ASM(const s32, "code_002FA4F0", effCallMemGetPtr);

INCLUDE_ASM(const s32, "code_002FA4F0", effFunc21);

INCLUDE_ASM(const s32, "code_002FA4F0", effFunc22);

INCLUDE_ASM(const s32, "code_002FA4F0", effCallMemGetPtr2);

INCLUDE_ASM(const s32, "code_002FA4F0", effCallMemGetPtr3);

INCLUDE_ASM(const s32, "code_002FA4F0", effCallMemGetPtr4);

INCLUDE_ASM(const s32, "code_002FA4F0", effCallMemGetPtr5);

INCLUDE_ASM(const s32, "code_002FA4F0", effLeaf402);

INCLUDE_ASM(const s32, "code_002FA4F0", effMesMngSetVarItfSub);

INCLUDE_ASM(const s32, "code_002FA4F0", effLeaf403);

INCLUDE_ASM(const s32, "code_002FA4F0", effCallMemGetPtr6);

INCLUDE_ASM(const s32, "code_002FA4F0", effCallMemGetPtr7);

INCLUDE_ASM(const s32, "code_002FA4F0", effCallMemGetPtr8);

INCLUDE_ASM(const s32, "code_002FA4F0", effCallMemGetPtr9);

INCLUDE_ASM(const s32, "code_002FA4F0", effPCP_HBED2_SetRotateMatrix);

INCLUDE_ASM(const s32, "code_002FA4F0", effCallMemGetPtr10);

INCLUDE_ASM(const s32, "code_002FA4F0", effCallMemGetPtr11);

INCLUDE_ASM(const s32, "code_002FA4F0", effCallMemGetPtr12);

INCLUDE_ASM(const s32, "code_002FA4F0", effBTLSkyFilterOFF);

INCLUDE_ASM(const s32, "code_002FA4F0", effCallMemGetPtr13);

INCLUDE_ASM(const s32, "code_002FA4F0", effCallMemGetPtr14);

INCLUDE_ASM(const s32, "code_002FA4F0", effCallMemGetPtr15);

void effFirstInitSub(void) {
    effData45 = 3;
}

INCLUDE_ASM(const s32, "code_002FA4F0", effLeaf404);

INCLUDE_ASM(const s32, "code_002FA4F0", effCallGetIntPara);

INCLUDE_ASM(const s32, "code_002FA4F0", fldCallGetIntPara5);

INCLUDE_ASM(const s32, "code_002FA4F0", fldNmeRootProcess);

INCLUDE_ASM(const s32, "code_002FA4F0", fldCallMemGetPtr12);

INCLUDE_ASM(const s32, "code_002FA4F0", fldFunc26);

INCLUDE_ASM(const s32, "code_002FA4F0", fldCallMemGetPtr13);

INCLUDE_ASM(const s32, "code_002FA4F0", fldCommand_SET_ALLPANEL);

INCLUDE_ASM(const s32, "code_002FA4F0", fldFunc27);

INCLUDE_ASM(const s32, "code_002FA4F0", nmeCallExistProcess);

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

/* body: matched in raidou @ 112d69c: src/shared/script/scrCommonCommand.c:302, found, same function 1.00 (words 10/10, calls 1.00, shape 1.00) */
s32 nmeScrCmd_CALL_NAME_WAIT(void)
{
    return dds3ExistProcess(EE_ADDR_AS(dds3ProcessID_t *, scrGetIntPara(0))) == 0;
}

INCLUDE_ASM(const s32, "code_002FA4F0", ScrCmd_GET_DREAM_COMPARE);

INCLUDE_ASM(const s32, "code_002FA4F0", nmeStart);

INCLUDE_ASM(const s32, "code_002FA4F0", nmeLeaf);

INCLUDE_ASM(const s32, "code_002FA4F0", nmeCallMemGetPtr);

INCLUDE_ASM(const s32, "code_002FA4F0", nmeCallMemGetPtr2);

INCLUDE_ASM(const s32, "code_002FA4F0", nmeCallMemGetPtr3);

INCLUDE_ASM(const s32, "code_002FA4F0", nmeCallMemGetPtr4);

INCLUDE_ASM(const s32, "code_002FA4F0", nmeCallMemGetPtr5);

INCLUDE_ASM(const s32, "code_002FA4F0", nmeCallMemGetPtr6);

INCLUDE_ASM(const s32, "code_002FA4F0", nmeCallMemGetPtr7);

INCLUDE_ASM(const s32, "code_002FA4F0", nmeCallMemGetPtr8);

INCLUDE_ASM(const s32, "code_002FA4F0", nmeCallMemGetPtr9);

INCLUDE_ASM(const s32, "code_002FA4F0", nmeCallMemGetPtr10);

INCLUDE_ASM(const s32, "code_002FA4F0", nmeCallMemGetPtr11);

INCLUDE_ASM(const s32, "code_002FA4F0", nmeCallMemGetPtr12);

INCLUDE_ASM(const s32, "code_002FA4F0", nmeScrCmdGETDREAMCOMPARESub);

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
