#include "common.h"
#include "fld.h"
#include "dds3KernelDraw.h"
#include "EventBit.h"

typedef struct mapFuncEntry_t {
    u8 scene;
    u8 id;
    u8 flag;
    u8 num;
    s8 x;
    s8 y;
    s16 type;
} mapFuncEntry_t;

extern mapFuncEntry_t mapFunc10Data[19];
extern s8 mapWork13[16];
extern const char *mapFunc2Data[];
extern void mapCallEnableMode2(s32 x, s32 y, u32 color, s32 priority, const char *str);

typedef struct mapOt2Block_t {
    u8 unk00[0x10];
    s16 w;
    s16 h;
    u8 unk14[4];
    s16 spriteNum;
} mapOt2Block_t;

typedef struct mapOt2Marker_t {
    u8 area;
    u8 floor;
    u16 type;
    u16 x;
    u16 y;
    u8 num;
    u8 dir;
    u16 evBit;
    u8 unk0C[4];
} mapOt2Marker_t;

typedef struct mapOt2Uv_t {
    s16 u;
    s16 v;
    s16 w;
    s16 h;
    s16 center;     /* 1: the position is the sprite centre */
} mapOt2Uv_t;

typedef struct mapOt2Sprite_t {
    u16 x;
    u16 y;
    u8 anchor;
    u8 unk05;
    u8 tile;
    u8 uvIdx;       /* low 5 bits: uv entry, top 2 bits: flip */
} mapOt2Sprite_t;

extern mapOt2Block_t *mapCallSetOt2Data2[];
extern u8 *mapData35[];
extern mapOt2Sprite_t *mapData36[];
extern mapOt2Marker_t mapCallSetOt2Data[128];
extern s16 mapCallSetOt2Data3[];
extern mapOt2Uv_t mapCallSetOt2Data4[];
extern s32 mapCallSetOt2Data5;
extern s32 mapCallSetOt2Data6;
extern f32 mapCallSetOt2Data7;
extern f32 mapCallSetOt2Data8;
extern f32 mapCallSetOt2Data9;
extern s32 mapCallSetOt2Data10;
extern s32 mapCallSetOt2Data11;
extern s32 fldData32;
extern s32 fldData33;
extern f32 fldData34;
extern f32 fldData35;
extern s32 fldData36;
extern void *effQueuedResourceNameRecord4;

extern s32 fldLeaf24(void);
extern void eglLinkSprite(s32 x, s32 y, s32 w, s32 h, s32 u, s32 v, s32 uw, s32 vh, u32 c0, u32 c1, u32 c2, u32 c3, void *th);
extern void mapProcSequenceSub(s32 area);
extern void mapFunc2(s32 num, s32 x, s32 y);
extern void fldProcSequenceSub4(f32 x, f32 z, f32 *outX, f32 *outY);
extern void mapCallCutGetCam(s32 x0, s32 y0, s32 x1, s32 y1);
extern void mapCallSin11(s32 x, s32 y);
extern void mapCallSin12(s32 x, s32 y);
extern void mapAmDrawBasyoName(s32 area, s32 floor);

/* NON_MATCHING: 18 of 110 words match. Retail reads the type through a second symbol and allocates registers differently. */
#ifdef NON_MATCHING
void mapFunc2(s32 num, s32 x, s32 y) {
    s32 i;

    for (i = 0; i < 16; i++) {
        s32 idx = mapWork13[i];
        mapFuncEntry_t *entry;
        s32 px;
        s32 py;
        s32 t;
        s32 nx;
        const char *text;

        if (idx < 0) {
            continue;
        }
        entry = &mapFunc10Data[idx];
        if (entry->num != num) {
            continue;
        }
        px = x + entry->x;
        py = y + entry->y;
        t = entry->type;
        if (t == 8) {
            nx = px - 0x34;
            text = mapFunc2Data[8];
        } else if (t >= 9) {
            if (t != 14) {
                goto plain;
            }
            mapCallEnableMode2(px, py, 0xA09DC380, 0x67, mapFunc2Data[14]);
            nx = px;
            text = mapFunc2Data[16];
        } else {
            if (t != 1) {
plain:
                mapCallEnableMode2(px, py, 0xA09DC380, 0x67, mapFunc2Data[entry->type]);
                continue;
            }
            mapCallEnableMode2(px, py, 0xA09DC380, 0x67, mapFunc2Data[1]);
            nx = px;
            text = mapFunc2Data[15];
        }
        mapCallEnableMode2(nx, py + 0x18, 0xA09DC380, 0x67, text);
    }
}
#else
INCLUDE_ASM(const s32, "code_r1_0016B498", mapFunc2);
#endif

/* NON_MATCHING: 145 of 809 words match. Same instruction count, frame size and register allocation differ. */
#ifdef NON_MATCHING
void mapCallSetOt2(void) {
    mapOt2Block_t *block;
    u8 *tiles;
    mapOt2Sprite_t *sprites;
    mapOt2Marker_t *mk;
    mapOt2Uv_t *uv;
    s32 w;
    s32 h;
    s32 cols;
    s32 tileNum;
    s32 i;
    s32 j;
    s32 k;
    s32 n;
    s32 px;
    s32 py;
    s32 x;
    s32 y;
    s32 sw;
    s32 sh;
    s32 id;
    s32 spr;
    s32 flip;
    s32 uvType;
    s32 valid;
    s32 tile;
    s32 tile2;
    s32 idx;
    s32 ax;
    s32 ay;
    s32 bx;
    s32 by;
    u32 color;
    f32 outX;
    f32 outY;

    cols = 0;
    if (mapCallSetOt2Data2[mapCallSetOt2Data6] == NULL) {
        return;
    }
    if (fldLeaf24() == 0) {
        return;
    }
    px = 0;
    eglSetOt(0x66);
    eglLinkAlpha(0);
    eglLinkTest(1, 0, 0x80, 3, 0, 0, 1, 1);
    sprites = mapData36[mapCallSetOt2Data6];
    tiles = mapData35[mapCallSetOt2Data6];
    block = mapCallSetOt2Data2[mapCallSetOt2Data6];
    h = block->h;
    w = block->w;
    mapProcSequenceSub(mapCallSetOt2Data5);
    for (i = fldData32; i < w; i++) {
        if (i >= 0) {
            if (cols >= 64) {
                break;
            }
            py = 0;
            tileNum = 0;
            cols++;
            for (j = fldData33; j < h; j++) {
                if (j < 0) {
                    py += 8;
                    continue;
                }
                if (tileNum >= 56) {
                    break;
                }
                tileNum++;
                color = 0x80C0C0C0;
                id = tiles[(i + w * j) * 2];
                spr = tiles[(i + w * j) * 2 + 1] & 0x1F;
                flip = tiles[(i + w * j) * 2 + 1] & 0xC0;
                if (mapAM_MaskChk_ID(mapCallSetOt2Data5, mapCallSetOt2Data6, id) == 0) {
                    color = 0x80808080;
                    if (mapCallSetOt2Data11 != 0) {
                        spr = 0;
                        flip = 0;
                    }
                }
                if (spr != 0) {
                    if (flip == 0x80) {
                        eglLinkSprite(px, py + 8, 8, -8, mapCallSetOt2Data3[spr * 2], mapCallSetOt2Data3[spr * 2 + 1], 8, 8, color, color, color, color, effQueuedResourceNameRecord4);
                    } else if (flip == 0xC0) {
                        eglLinkSprite(px + 8, py + 8, -8, -8, mapCallSetOt2Data3[spr * 2], mapCallSetOt2Data3[spr * 2 + 1], 8, 8, color, color, color, color, effQueuedResourceNameRecord4);
                    } else if (flip == 0x40) {
                        eglLinkSprite(px + 8, py, -8, 8, mapCallSetOt2Data3[spr * 2], mapCallSetOt2Data3[spr * 2 + 1], 8, 8, color, color, color, color, effQueuedResourceNameRecord4);
                    } else {
                        eglLinkSprite(px, py, 8, 8, mapCallSetOt2Data3[spr * 2], mapCallSetOt2Data3[spr * 2 + 1], 8, 8, color, color, color, color, effQueuedResourceNameRecord4);
                    }
                }
                mapFunc2(id, px, py);
                py += 8;
            }
        }
        px += 8;
    }
    for (n = 0; n < 128; n++) {
        mk = &mapCallSetOt2Data[n];
        if (mk->area != mapCallSetOt2Data5 || mk->floor != mapCallSetOt2Data6) {
            continue;
        }
        if (evtBitCheck(mk->evBit) != 0) {
            continue;
        }
        valid = 0;
        uvType = 0;
        if (mk->type == 1) {
            valid = 1;
            uvType = 3;
        } else if (mk->type < 2) {
            valid = 1;
            uvType = 12;
        } else if (mk->type < 8) {
            valid = 1;
            uvType = 3;
        }
        color = 0x80808080;
        if (valid == 0) {
            continue;
        }
        idx = mk->y * w + mk->x;
        tile = tiles[idx * 2];
        if (mapAM_MaskChk_ID(mapCallSetOt2Data5, mapCallSetOt2Data6, tile) == 0) {
            tile2 = tile;
            if (mk->dir == 4) {
                tile2 = tiles[idx * 2 - 2];
            } else if (mk->dir == 0) {
                tile2 = tiles[(mk->y + 1) * w * 2 + mk->x * 2];
            } else if (mk->dir == 8) {
                tile2 = tiles[(mk->y - 1) * w * 2 + mk->x * 2];
            } else if (mk->dir == 12) {
                tile2 = tiles[idx * 2 + 2];
            }
            if (mapAM_MaskChk_ID(mapCallSetOt2Data5, mapCallSetOt2Data6, tile2) == 0) {
                continue;
            }
        }
        x = (mk->x - fldData32) * 8;
        y = (mk->y - fldData33) * 8;
        uv = &mapCallSetOt2Data4[uvType];
        if (mk->dir == 4) {
            if (mk->num == 0) {
                continue;
            }
            for (k = 0; k < mk->num; k++) {
                eglLinkSprite(x + 8, y + k * 8, -8, 8, uv[1].u, uv[1].v, 8, 8, color, color, color, color, effQueuedResourceNameRecord4);
            }
        } else if (mk->dir == 8) {
            if (mk->num == 0) {
                continue;
            }
            for (k = 0; k < mk->num; k++) {
                eglLinkSprite(x + k * 8, y, 8, 8, uv[0].u, uv[0].v, 8, 8, color, color, color, color, effQueuedResourceNameRecord4);
            }
        } else if (mk->dir == 12) {
            if (mk->num == 0) {
                continue;
            }
            for (k = 0; k < mk->num; k++) {
                eglLinkSprite(x, y + k * 8, 8, 8, uv[1].u, uv[1].v, 8, 8, color, color, color, color, effQueuedResourceNameRecord4);
            }
        } else if (mk->dir == 0) {
            if (mk->num == 0) {
                continue;
            }
            for (k = 0; k < mk->num; k++) {
                eglLinkSprite(x + k * 8, y + 8, 8, -8, uv[0].u, uv[0].v, 8, 8, color, color, color, color, effQueuedResourceNameRecord4);
            }
        }
    }
    for (k = 0; k < mapCallSetOt2Data2[mapCallSetOt2Data6]->spriteNum; k++) {
        mapOt2Sprite_t *sp = &sprites[k];

        color = 0x80C0C0C0;
        spr = sp->uvIdx & 0x1F;
        x = (sp->x - fldData32) * 8;
        y = (sp->y - fldData33) * 8;
        uv = &mapCallSetOt2Data4[spr];
        flip = sp->uvIdx & 0xC0;
        sw = uv->w;
        sh = uv->h;
        if (mapAM_MaskChk_ID(mapCallSetOt2Data5, mapCallSetOt2Data6, sp->tile) == 0) {
            color = 0x80808080;
            if (mapCallSetOt2Data11 != 0) {
                continue;
            }
        }
        if (uv->center == 1) {
            switch (sp->anchor) {
            case 0:
                x += 4;
                y += 4;
                break;
            case 1:
                break;
            case 2:
                x += 8;
                break;
            case 3:
                y += 8;
                break;
            case 4:
                x += 8;
                y += 8;
                break;
            }
            x -= sw / 2;
            y -= sh / 2;
        }
        if (flip == 0x40 || flip == 0xC0) {
            x += sw;
            sw = -sw;
        }
        if (flip == 0x80 || flip == 0xC0) {
            y += sh;
            sh = -sh;
        }
        eglLinkSprite(x, y, sw, sh, uv->u, uv->v, uv->w, uv->h, color, color, color, color, effQueuedResourceNameRecord4);
    }
    if (mapCallSetOt2Data6 == fldData36) {
        fldProcSequenceSub4(mapObjTaskData.x, mapObjTaskData.z, &outX, &outY);
        ax = ((s32)outX - fldData32) * 8;
        ay = ((s32)outY - fldData33) * 8;
        fldProcSequenceSub4(mapCutSetCamData.x, mapCutSetCamData.z, &outX, &outY);
        bx = ((s32)outX - fldData32) * 8;
        by = ((s32)outY - fldData33) * 8;
        mapCallCutGetCam(ax, ay, bx, by);
        if (mapCallSetOt2Data9 != 0.0f) {
            mapCallSin12(((s32)fldData34 - fldData32) * 8, ((s32)fldData35 - fldData33) * 8);
            if (mapCallSetOt2Data6 == mapCallSetOt2Data10) {
                mapCallSin11(((s32)mapCallSetOt2Data7 - fldData32) * 8, ((s32)mapCallSetOt2Data8 - fldData33) * 8);
            }
        } else {
            mapCallSin11(((s32)mapCallSetOt2Data7 - fldData32) * 8, ((s32)mapCallSetOt2Data8 - fldData33) * 8);
        }
    } else if (mapCallSetOt2Data9 != 0.0f && mapCallSetOt2Data6 == mapCallSetOt2Data10) {
        mapCallSin11(((s32)mapCallSetOt2Data7 - fldData32) * 8, ((s32)mapCallSetOt2Data8 - fldData33) * 8);
    }
    mapAmDrawBasyoName(mapCallSetOt2Data5, mapCallSetOt2Data6);
}
#else
INCLUDE_ASM(const s32, "code_r1_0016B498", mapCallSetOt2);
#endif

INCLUDE_SDATA(const s32, "code_r1_0016B498", mapCallSetOt2Data11);

