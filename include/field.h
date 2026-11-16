#ifndef FIELD_H
#define FIELD_H

#include "common.h"
#include "sdf_types.h"

typedef struct {
    struct dds3Basic_t *handle;
    struct dds3Basic_t *handle2;
    s8 moveFlag;
    s8 rotFlag;
} fldUnitControl_t;

typedef struct {
    sdfVector4_t wideCameraOffset;
    sdfVector4_t wideTargetOffset;
    sdfVector4_t narrowCameraOffset;
    sdfVector4_t narrowTargetOffset;
    sdfVector4_t q0;
    sdfVector4_t q1;
    f32 rotFrame;
    f32 rotFrameMax;
    f32 t0;
    f32 t1;
    f32 lenFrame;
    f32 lenFrameMax;
    struct dds3Basic_t *handle;
} fldFollowCameraControl_t;

/* Field collision record shared by the hit and camera code. */
typedef struct collision_s collision_s;
struct collision_s {
    s16 mode2D;
    s16 numedges;
    f32 normal[4];
    f32 num;
    f32 e_normal[4][4];
    f32 e_num[4];
    f32 min[4];
    f32 point[4][4];
    f32 previousPosition[3];
    u32 elv;
    u16 blockID;
    u16 hitflag;
    s32 resID;
    struct dds3Basic_t *matter;
    struct fldCollisionFace_t *pFace;
    void *pTome;
    s32 hojiID;
    s32 npcID;
};

typedef struct fldAmXYZ_t
{
    f32 x;
    f32 y;
    f32 z;
} fldAmXYZ_t;

typedef struct fldAmIcon_t
{
    u32 type;
    fldAmXYZ_t *position;
} fldAmIcon_t;

typedef struct fldAmSBlock_t
{
    char *name;
    u32 nodeIndex;
    struct fldAmIcon_t *icons;
    u32 iconCount;
    s32 floor;
    fldAmXYZ_t *pointA;
    fldAmXYZ_t *pointB;
} fldAmSBlock_t;

typedef struct fldAmAreaData_t
{
    char *name;
    fldAmSBlock_t *items;
    u32 nSB;
    void *pMdl;
    fldAmXYZ_t *pPos;
} fldAmAreaData_t;

union fldTriggerShape_t;

typedef struct fldResPut_t
{
    u32 type;
    s32 eventIndex;
    u32 visible;
    union {
        void *parameters;
        union fldTriggerShape_t *shape;
    } data;
} fldResPut_t;

typedef struct fldResourcePoint_t
{
    sdfVector4_t pos;
    sdfVector4_t rot;
    sdfVector4_t scl;
} fldResourcePoint_t;

/* A resource point a resource header refers to. The event-hit records place
   theirs at 8 mod 16, so it is read through 4-aligned vectors. */
typedef struct
{
    sdfVector4Unaligned_t pos;
    sdfVector4Unaligned_t rot;
    sdfVector4Unaligned_t scl;
} fldResourcePointUnaligned_t;

extern fldResourcePoint_t fldPlayerPosRotWork;
extern fldResourcePoint_t ppos01Point;

/* Counted filename tables use a variable-length trailing array. The native
 * filename entry is 0x0C bytes and the file-resource row is 0x24 bytes. */
typedef struct fldFileNameEntry_t
{
    u32 word00;
    const char *name;
    u32 word08;
} fldFileNameEntry_t;

typedef struct fldFileNameTable_t
{
    u32 count;
    fldFileNameEntry_t entries[1];
} fldFileNameTable_t;

typedef struct fldResHead_t
{
    u32 id;
    u32 word04;
    const char *name;
    u32 word0C;
    fldResourcePointUnaligned_t *transform;
    u32 areaName;
    fldFileNameTable_t *names;
    u32 word1C;
    void *data;
} fldResHead_t;

typedef struct fldTexSlot_t
{
    struct sdfTexHandle_t *texture;
    struct sdfMat_t *file;
} fldTexSlot_t;

/* Shared view of fldGlobalBase through the last recovered position component.
 * Unknown gaps remain opaque; this is not the complete global allocation. */
typedef struct fldAreaWork_t
{
    u32 flags;                  /* 0x000 */
    s32 area;                   /* 0x004 */
    s32 floor;                  /* 0x008: zero-based */
    s32 blockID;                /* 0x00C */
    s32 word10;                 /* 0x010 */
    u32 consumedFlags;          /* 0x014 */
    s32 mode;                   /* 0x018 */
    s32 word1C;                 /* 0x01C */
    s32 word20;                 /* 0x020 */
    s32 word24;                 /* 0x024 */
    s32 word28;                 /* 0x028 */
    s32 word2C;                 /* 0x02C */
    struct sdfTexHandle_t *word30; /* 0x030: bg texture, released by mapBgFree2 */
    s32 cutIdx;                 /* 0x034: index into the fldWork6 cut table */
    s32 cutIdxPrev;             /* 0x038: cutIdx before the last change */
    s32 cutState;               /* 0x03C: 1 requests a cut reload, 2 once done */
    s32 playerSeTable;          /* 0x040 */
    s32 word44;                 /* 0x044 */
    s32 hitMatterIndex;         /* 0x048 */
    s32 word4C;                 /* 0x04C */
    struct dds3Basic_t *fastArea; /* 0x050: the first area object built from the FL2 block table */
    s32 mapCallMode;            /* 0x054: set to 1 by mapCallMap, tested by mapReturnFromShop */
    u8 unk58[4];
    s32 word5C;                 /* 0x05C */
    s32 amSBlock;               /* 0x060 */
    s32 amFloor;                /* 0x064 */
    s32 nameU;                  /* 0x068 */
    s32 nameD;                  /* 0x06C */
    s32 menuOpenFlag;           /* 0x070 */
    s32 encountState;           /* 0x074 */
    s32 word78;                 /* 0x078 */
    s32 previousFloor;          /* 0x07C: last floor fldFreeFile recorded */
    s32 word80;                 /* 0x080 */
    s32 resourceFlag;           /* 0x084 */
    s32 resourceArea;           /* 0x088 */
    s32 resourceFloor;          /* 0x08C */
    s32 word90;                 /* 0x090 */
    s32 noEncountCnt;           /* 0x094 */
    s32 noInpPlCnt;             /* 0x098 */
    f32 focusX;                 /* 0x09C: camera focus point */
    f32 focusY;                 /* 0x0A0 */
    f32 focusZ;                 /* 0x0A4 */
    f32 floatA8;                /* 0x0A8: set by fldPlayerPosRot */
    f32 floatAC;                /* 0x0AC */
    f32 floatB0;                /* 0x0B0 */
    f32 floatB4;                /* 0x0B4 */
    s32 lightMaIdx;             /* 0x0B8 */
    s32 defaultEncounterId;     /* 0x0BC: the floor's encounter pack, fldReadyScene's */
    f32 floatC0;                /* 0x0C0: read and written with lwc1/swc1 in both games */
    f32 floatC4;               /* 0x0C4 */
    union { s32 wordC8; f32 PlayerMotionFrame; }; /* 0x0C8: the player motion frame of the last PlayerMotionSe2 */
    s32 wordCC;                 /* 0x0CC */
    s32 wordD0;                 /* 0x0D0 */
    s32 wordD4;                 /* 0x0D4 */
    s32 wordD8;                 /* 0x0D8 */
    s32 wordDC;                 /* 0x0DC */
    s32 wordE0;                 /* 0x0E0 */
    f32 floatE4;                /* 0x0E4 */
    s32 camDir;                 /* 0x0E8: 0-3, mapCutGetCam's fixed-camera side */
    s32 camZoom;                /* 0x0EC: 0-7, index into the camera height table */
    s32 camId;                  /* 0x0F0: first word of the fldWork6 cut entry */
    u8 unkF4[4];
    s32 wordF8;                 /* 0x0F8 */
    s32 wordFC;                 /* 0x0FC */
#ifdef VERSION_RAIDOU1
    f32 x;                      /* 0x100 */
    f32 y;                      /* 0x104 */
    f32 z;                      /* 0x108 */
    f32 prevX;                  /* 0x10C */
    s32 btlReturnMap;           /* 0x110: nonzero after a battle, the field reloads from gMapBtlReturnParam (HD btl_return_map) */
    s32 bgZoomSet;              /* 0x114: set to 1 after fldBgZoomTest(3); never a float */
    f32 hitX;                   /* 0x118 */
    f32 hitY;
    f32 hitZ;
    f32 camAngle;               /* 0x124 */
    f32 camAngle2;              /* 0x128 */
    union { f32 float12C; s32 word12C; }; /* 0x12C: Raidou stores -1 */
    s32 mskIndex;               /* 0x130: MSK index from mapNpcTableGetMSK */
    s32 hitValid;               /* 0x134 */
    s32 word138;                /* 0x138 */
    f32 float13C;               /* 0x13C */
    f32 float140;               /* 0x140 */
    f32 float144;               /* 0x144 */
    union { s32 word148; s16 half148; };  /* 0x148 */
    s32 word14C;                /* 0x14C */
    s32 word150;                /* 0x150 */
    s32 word154;                /* 0x154 */
    s32 word158;                /* 0x158 */
    s32 word15C;                /* 0x15C */
    s32 word160;                /* 0x160 */
    s32 word164;                /* 0x164 */
    s32 summonStop;             /* 0x168 */
    s32 word16C;                /* 0x16C */
    s32 word170;                /* 0x170 */
    s32 word174;                /* 0x174 */
    s32 word178;                /* 0x178 */
    s32 word17C;                /* 0x17C */
    s32 word180;                /* 0x180 */
    f32 float184;               /* 0x184 */
    f32 float188;               /* 0x188 */
    f32 float18C;               /* 0x18C */
    s32 word190;                /* 0x190 */
    s32 word194;                /* 0x194 */
    f32 float198;               /* 0x198 */
    f32 float19C;               /* 0x19C */
    f32 float1A0;               /* 0x1A0 */
    f32 float1A4;               /* 0x1A4 */
    s32 word1A8;                /* 0x1A8 */
    s32 word1AC;                /* 0x1AC */
    f32 float1B0;               /* 0x1B0 */
    f32 float1B4;               /* 0x1B4 */
    f32 float1B8;               /* 0x1B8 */
    f32 float1BC;               /* 0x1BC */
    s32 word1C0;                /* 0x1C0 */
    s32 word1C4;                /* 0x1C4 */
    f32 float1C8;               /* 0x1C8 */
    s32 word1CC;                /* 0x1CC */
    s32 word1D0;                /* 0x1D0 */
    s32 word1D4;                /* 0x1D4 */
    s32 word1D8;                /* 0x1D8 */
    s32 word1DC;                /* 0x1DC */
    s32 word1E0;                /* 0x1E0 */
    s32 word1E4;                /* 0x1E4 */
    s32 rndEncMode;             /* 0x1E8 */
    s32 rndEncFieldID;          /* 0x1EC */
    s32 rndEncFloorID;          /* 0x1F0 */
    s32 rndEncNo;               /* 0x1F4 */
    f32 rndEncX;                /* 0x1F8 */
    f32 rndEncY;                /* 0x1FC */
    f32 rndEncZ;                /* 0x200 */
    s32 word204;                /* 0x204 */
    f32 float208;               /* 0x208 */
    f32 float20C;               /* 0x20C */
    f32 float210;               /* 0x210 */
    f32 float214;               /* 0x214 */
    s32 word218;                /* 0x218 */
    s32 word21C;                /* 0x21C */
    struct sdfTexHandle_t *texture220; /* 0x220 */
    fldTexSlot_t texSlot[3];    /* 0x224 */
    u8 unk23C[0x4];
    s32 word240;                /* 0x240 */
    s32 automapMaskCurrentIdx;  /* 0x244 */
    s32 word248;                /* 0x248 */
    s32 word24C;                /* 0x24C */
    s32 ifDrawOn;               /* 0x250 */
    char scriptName[0x40];      /* 0x254 */
    s32 word294;                /* 0x294 */
    s32 word298;                /* 0x298 */
    s32 word29C;                /* 0x29C */
    s32 word2A0;                /* 0x2A0 */
    s32 word2A4;                /* 0x2A4 */
    s32 word2A8;                /* 0x2A8 */
    s32 word2AC;                /* 0x2AC */
    struct sdfTexHandle_t *texture2B0; /* 0x2B0 */
    f32 float2B4;               /* 0x2B4 */
    f32 float2B8;               /* 0x2B8 */
    s32 word2BC;                /* 0x2BC */
    s32 word2C0;                /* 0x2C0 */
    s32 word2C4;                /* 0x2C4 */
    char *initScript;           /* 0x2C8: script name pointers set by mapCallBitCheck */
    char *startScript;          /* 0x2CC */
    s32 word2D0;                /* 0x2D0 */
    s32 word2D4;                /* 0x2D4 */
    s32 word2D8;                /* 0x2D8 */
    s32 word2DC;                /* 0x2DC */
    s32 word2E0;                /* 0x2E0 */
    s32 word2E4;                /* 0x2E4 */
    s32 word2E8;                /* 0x2E8 */
    s32 word2EC;                /* 0x2EC */
    s32 word2F0;                /* 0x2F0 */
    s32 word2F4;                /* 0x2F4 */
    s32 nakamaTaskOff;          /* 0x2F8 */
    s32 aiboRunStop;            /* 0x2FC */
    s32 word300;                /* 0x300 */
    s32 word304;                /* 0x304 */
    s32 word308;                /* 0x308 */
    s32 word30C;                /* 0x30C */
    s32 word310;                /* 0x310 */
    s32 word314;                /* 0x314 */
    s32 word318;                /* 0x318 */
    u8 unk31C[4];
    s32 word320;                /* 0x320 */
    u8 unk324[8];
    s32 word32C;                /* 0x32C */
    u8 unk330[4];
    s32 word334;                /* 0x334 */
    s32 bgDrawOn;               /* 0x338: mapBgDrawOnOff (HD fldDbg->BgDrawOn) */
    s32 word33C;                /* 0x33C */
    u8 unk340[4];
    s32 word344;                /* 0x344 */
    u8 unk348[0x14];
    s32 word35C;                /* 0x35C */
#else
    s32 word100;                /* 0x100 */
    s32 btlReturnMap;           /* 0x104: Raidou 1 has it at 0x110 */
    s32 bgZoomSet;              /* 0x108 */
    s32 word10C;                /* 0x10C */
    s32 word110;                /* 0x110 */
    s32 word114;                /* 0x114 */
    f32 camAngle;               /* 0x118: nbMAPSETTAKARA reads it, Raidou 1 at 0x124 */
    s32 word11C;                /* 0x11C */
    s32 word120;                /* 0x120 */
    s32 word124;                /* 0x124 */
    s32 word128;                /* 0x128 */
    s32 word12C;                /* 0x12C */
    s32 word130;                /* 0x130 */
    s32 word134;                /* 0x134 */
    s32 word138;                /* 0x138 */
    s32 word13C;                /* 0x13C */
    s32 word140;                /* 0x140 */
    s32 word144;                /* 0x144 */
    f32 float148;               /* 0x148 */
    f32 float14C;               /* 0x14C */
    f32 float150;               /* 0x150 */
    union { s32 word154; s16 half154; };  /* 0x154 */
    s32 word158;                /* 0x158 */
    s32 word15C;                /* 0x15C */
    s32 word160;                /* 0x160 */
    s32 word164;                /* 0x164 */
    s32 word168;                /* 0x168 */
    s32 word16C;                /* 0x16C */
    s32 word170;                /* 0x170 */
    union { s32 word174; s16 half174; u8 byte174; };  /* 0x174 */
    s32 word178;                /* 0x178 */
    s32 word17C;                /* 0x17C */
    s32 word180;                /* 0x180 */
    s32 word184;                /* 0x184 */
    s32 word188;                /* 0x188 */
    s32 word18C;                /* 0x18C */
    s32 word190;                /* 0x190 */
    s32 summonStop;             /* 0x194 */
    s32 word198;                /* 0x198 */
    s32 word19C;                /* 0x19C */
    s32 word1A0;                /* 0x1A0 */
    s32 word1A4;                /* 0x1A4 */
    s32 word1A8;                /* 0x1A8 */
    s32 word1AC;                /* 0x1AC */
    s32 word1B0;                /* 0x1B0 */
    s32 word1B4;                /* 0x1B4 */
    s32 word1B8;                /* 0x1B8 */
    f32 float1BC;               /* 0x1BC */
    f32 float1C0;               /* 0x1C0 */
    f32 float1C4;               /* 0x1C4 */
    s32 word1C8;                /* 0x1C8 */
    s32 word1CC;                /* 0x1CC */
    f32 float1D0;               /* 0x1D0 */
    f32 float1D4;               /* 0x1D4 */
    f32 float1D8;               /* 0x1D8 */
    f32 float1DC;               /* 0x1DC */
    s32 word1E0;                /* 0x1E0 */
    s32 word1E4;                /* 0x1E4 */
    f32 float1E8;               /* 0x1E8 */
    f32 float1EC;               /* 0x1EC */
    f32 float1F0;               /* 0x1F0 */
    f32 float1F4;               /* 0x1F4 */
    s32 word1F8;                /* 0x1F8 */
    s32 word1FC;                /* 0x1FC */
    f32 float200;               /* 0x200 */
    s32 word204;                /* 0x204 */
    s32 word208;                /* 0x208 */
    s32 word20C;                /* 0x20C */
    s32 word210;                /* 0x210 */
    s32 word214;                /* 0x214 */
    s32 word218;                /* 0x218 */
    s32 word21C;                /* 0x21C */
    s32 word220;                /* 0x220 */
    s32 word224;                /* 0x224 */
    s32 word228;                /* 0x228 */
    s32 rndEncMode;             /* 0x22C */
    s32 rndEncFieldID;          /* 0x230 */
    s32 rndEncFloorID;          /* 0x234 */
    s32 rndEncNo;               /* 0x238 */
    f32 rndEncX;                /* 0x23C */
    f32 rndEncY;                /* 0x240 */
    f32 rndEncZ;                /* 0x244 */
    s32 word248;                /* 0x248 */
    f32 float24C;               /* 0x24C */
    f32 float250;               /* 0x250 */
    f32 float254;               /* 0x254 */
    f32 float258;               /* 0x258 */
    s32 word25C;                /* 0x25C */
    s32 word260;                /* 0x260 */
    s32 word264;                /* 0x264 */
    fldTexSlot_t texSlot[3];    /* 0x268 */
    u8 unk280[0x4];
    s32 word284;                /* 0x284 */
    s32 automapMaskCurrentIdx;  /* 0x288 */
    s32 word28C;                /* 0x28C */
    s32 word290;                /* 0x290 */
    s32 ifDrawOn;               /* 0x294 */
    u8 bytes298[0x10];          /* 0x298: two unaligned 8-byte copies */
    s32 word2A8;                /* 0x2A8 */
    u8 byte2AC;                 /* 0x2AC */
    u8 unk2AD[0x2B];
    s32 word2D8;                /* 0x2D8 */
    s32 word2DC;                /* 0x2DC */
    s32 word2E0;                /* 0x2E0 */
    s32 word2E4;                /* 0x2E4 */
    s32 word2E8;                /* 0x2E8 */
    s32 word2EC;                /* 0x2EC */
    s32 word2F0;                /* 0x2F0 */
    struct sdfTexHandle_t *texture2B0; /* 0x2F4 */
    f32 float2F8;               /* 0x2F8 */
    f32 float2FC;               /* 0x2FC */
    s32 word300;                /* 0x300 */
    s32 word304;                /* 0x304 */
    s32 word308;                /* 0x308 */
    s32 word30C;                /* 0x30C */
    s32 word310;                /* 0x310 */
    s32 word314;                /* 0x314 */
    s32 word318;                /* 0x318 */
    s32 word31C;                /* 0x31C */
    s32 word320;                /* 0x320 */
    s32 word324;                /* 0x324 */
    s32 word328;                /* 0x328 */
    s32 word32C;                /* 0x32C */
    s32 word330;                /* 0x330 */
    s32 word334;                /* 0x334 */
    s32 word338;                /* 0x338 */
    s32 nakamaTaskOff;          /* 0x33C */
    s32 aiboRunStop;            /* 0x340 */
    s32 word344;                /* 0x344 */
    s32 word348;                /* 0x348 */
    s32 word34C;                /* 0x34C */
    s32 word350;                /* 0x350 */
    s32 word354;                /* 0x354 */
    s32 word358;                /* 0x358 */
    s32 word35C;                /* 0x35C */
    s32 word360;                /* 0x360 */
    s32 word364;                /* 0x364 */
    u8 unk368[0x1];
    u8 byte369;                 /* 0x369 */
    s16 half36A;                /* 0x36A */
    f32 float36C;               /* 0x36C */
    s32 word370;                /* 0x370 */
    s32 word374;                /* 0x374 */
    u8 bytes378[4];             /* 0x378 */
    f32 float37C;               /* 0x37C */
    f32 float380;               /* 0x380 */
    f32 float384;               /* 0x384 */
    f32 float388;               /* 0x388 */
    f32 float38C;               /* 0x38C */
    f32 float390;               /* 0x390 */
    s32 word394;                /* 0x394 */
#endif
} fldAreaWork_t;

extern fldAreaWork_t fldGlobalBase;

s32 mapCheckDying(s32 mode);

typedef struct {
    s32 type;
    s16 id;
    s16 num;
    s32 trap;
    s32 param;
} fldTakaraTbl_t;

extern fldTakaraTbl_t fldTakaraTable[];

typedef struct {
    u8 x;
    u8 y;
    u8 z;
    u8 w;
    u8 unk04[4];
    u32 routeOffset; /* byte offset from the map header */
    u32 routeCount;
} fldMapCell_t;

typedef struct {
    u32 firstOffset;
    u32 cellCount;
    u32 secondOffset;
} fldRunMapHeader_t;

typedef struct {
    u8 x;
    u8 z;
    u16 angle;
} fldRoutePoint_t;

typedef struct {
    s32 gunFlag;
    s32 word04;
    s32 word08;
    s32 word0C;
    s32 word10;
    s32 word14;
    s32 word18;
    s32 word1C;
    s32 word20;
    s32 word24;
    s32 word28;
    s32 skyTable;
    s32 word30;
    s32 filter;
    s32 word38;
    s32 word3C;
    s32 word40;
    s32 enabled;
    u32 unk48;
    s32 counter;
    s32 word50;
    s32 word54;
    u32 unk58;
    s32 word5C;
} fldDanteGunState_t;

/* gfldSceneParam: the pending scene request, 0x38 bytes (HD SceneParam_t). */
typedef struct fldSceneParam_t {
    char FoceScript[0x20];
    u32 flag;                   /* 0x20 */
    s32 mode;                   /* 0x24 */
    s32 loadField;              /* 0x28 */
    s32 loadFloor;              /* 0x2C */
    s32 loadPoint;              /* 0x30 */
    u32 loadMode;               /* 0x34 */
} fldSceneParam_t;

extern fldSceneParam_t gfldSceneParam;
/* The scene to return to after a battle (HD gMapBtlReturnParam). */
extern fldSceneParam_t gMapBtlReturnParam;

typedef struct slFileWork_t {
    s32 w[5];
} slFileWork_t;

typedef struct GameFileInfo_t {
    u8 verstr[3];
    u8 ver;
    u8 TmnlMode;
    u8 TmnlId;
    u8 Lv;
    u8 mode;
    s32 PlayTime;
    union { struct { u16 endcnt; u16 newfile; }; s32 endCounterWord; };
    slFileWork_t work;
    u8 autoMapOpen;
    u8 autoMapOpenEX[15];
} GameFileInfo_t;

/* Native 0x24-byte resource collision face; attributes occupy the first word. */
typedef struct fldCollisionFace_t fldCollisionFace_t;
struct fldCollisionFace_t {
    u32 attributes;
    u8 moveFloor;
    u8 sound;
    u16 stop;
    u16 place;
    u8 automap[2];
    u32 vertexIndices[4];
    s16 encounterType;
    s16 encounter;
    s16 special[2];
};

/* Field encounter control and the native placement table. */
#include "fldNpc_tag.h"
extern u32 fldEncountFlags;
extern fldNpcSlot_t fldModelSlotTable[32];
extern s32 fldNpcModelTable;

#include "mapLight.h"

#endif /* FIELD_H */
