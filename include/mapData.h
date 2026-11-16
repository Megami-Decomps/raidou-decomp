#ifndef MAPDATA_H
#define MAPDATA_H

#include "common.h"
#include "field.h"
#include "dds3Basic.h"
#include "defines.h"
#include "enums.h"
#include "newdata.h"
#include "mapFilter.h"

typedef struct fldMapAreaHead_t
{
    s32 colmOfs;
    s32 hit64Ofs;
    s32 pointOfs;
    s32 hit20Ofs;
    s32 objOfs;
    s32 eveHitOfs;
    s32 kaidanOfs;
    s32 extraOfs;
    s32 resv20;
    s16 w;
    s16 h;
    s16 originX;
    s16 originZ;
    s16 hit64Count;
    s16 pointCount;
    s16 hit20Count;
    s16 objCount;
    s16 kaidanCount;
    u8 unk36[0x40 - 0x36];
} fldMapAreaHead_t;

/* The map file: a count, then one 64 byte area head per area. */
typedef struct fldMapFileHead_t
{
    s32 count;
    u8 unk04[12];
    fldMapAreaHead_t area[64];
} fldMapFileHead_t;

/* Placed point: id, grid cell, corner offset and direction index. */
typedef struct fldMapPointRec_t
{
    s16 id;
    u16 ix;
    u16 iy;
    u8 ofs;
    u8 dir;
} fldMapPointRec_t;

typedef struct fldMapHit20Rec_t
{
    u16 w00;
    u16 w02;
    u16 w04;
    u16 w06;
    u16 w08;
    u16 w0A;
    u8 b0C;
    u8 b0D;
    u16 w0E;
    u16 w10;
    u16 w12;
} fldMapHit20Rec_t;

typedef struct fldMapHit64Rec_t
{
    u16 w00;
    u16 w02;
    u16 w04;
    u16 w06;
    u16 w08;
    u16 w0A;
    u16 w0C;
    u16 w0E;
    u16 w10;
    u8 b12;
    u8 b13;
    u16 w14[14];
    s32 word30;
    u8 unk34[12];
} fldMapHit64Rec_t;

typedef struct fldMapKaidanRec_t
{
    u16 type;
    u16 xory;
    u16 ix0;
    u16 iy0;
    u16 ix1;
    u16 iy1;
    s8 h0;
    s8 h1;
    u16 haba;
} fldMapKaidanRec_t;

/* Sub-area head: offsets of the points and 64 byte hit records, then their counts. */
typedef struct fldMapSubHead_t
{
    s32 pointOfs;
    s32 hit64Ofs;
    u8 unk08[8];
    s16 pointCount;
    s16 hit64Count;
    u8 unk14[12];
} fldMapSubHead_t;

/* Battle map data (.mpb): file offsets are from the start of the file. */
typedef struct MpbAreaHead_t {
    s32 addr_btlhit;
    s32 addr_enchead;
    s32 addr_rndenc;
    s32 addr_resv;
    s16 num_btlhit;
    s16 num_enchead;
    s16 rnd_encw;
    s16 rnd_ench;
    s16 resv[4];
} MpbAreaHead_t;

typedef struct MpbEncHead_t {
    s32 addr_encpos;
    s32 addr_btlwall;
    s32 addr_resv[2];
    s16 encount_id;
    s16 num_encpos;
    s16 num_btlwall;
    s16 resv[5];
} MpbEncHead_t;

typedef struct MpbPoint_t {
    u16 id;
    u16 x;
    u16 y;
    u8 ofs;
    u8 dir;
} MpbPoint_t;

typedef struct MpbWall_t {
    u16 id;
    u16 x;
    u16 y;
    u16 w;
    u16 h;
    u16 attr;
    s32 resv;
} MpbWall_t;

typedef struct fldMapLightHead_t
{
    s32 ofs0;
    s32 lightOfs;
    s32 count;
    s16 w;
    s16 h;
} fldMapLightHead_t;

/* Light grid cell: index into the area's light table, 0xFF for none. */
typedef struct fldMapLightCell_t
{
    u8 light;
    u8 resv;
} fldMapLightCell_t;

/* Placed light: cell position, radius, colors; ofs 0x30 total. */
typedef struct fldMapLightRec_t
{
    f32 x;
    f32 z;
    f32 inR;
    f32 difR;
    f32 difG;
    f32 difB;
    f32 outR;
    f32 ambR;
    f32 ambG;
    f32 ambB;
    u8 unk28[8];
} fldMapLightRec_t;

typedef struct fldMapSub2File_t
{
    s32 count;
    u8 unk04[12];
    fldMapLightHead_t entry[64];
} fldMapSub2File_t;

typedef struct fldMapSub3File_t
{
    s32 count;
    u8 unk04[12];
    fldMapSubHead_t entry[64];
} fldMapSub3File_t;

typedef struct fldMapSub4File_t
{
    s32 count;
    u8 unk04[12];
    MpbAreaHead_t entry[64];
} fldMapSub4File_t;

/* Event hit grid head: one type per bit of a 32 or 64 bit mask per cell. */
typedef struct fldMapEveHitHead_t
{
    s32 wide;
    s16 w;
    s16 h;
    s32 maskOfs;
    u8 unk0C[4];
    u8 type[64];
} fldMapEveHitHead_t;

typedef struct fldMapFlagRec_t
{
    u8 field;
    u8 floor;
    u16 w02;
    u16 w04;
    u8 b06;
    u8 b07;
    s16 s08;
    s16 s0A;
    s16 s0C;
    s16 s0E;
} fldMapFlagRec_t;

/* Placed flag record: field, floor, kind and parameters for mapCallBitCheck2. */
typedef struct fldMapBitRec_t
{
    u8 field;
    u8 floor;
    u16 kind;
    u16 w04;
    u16 w06;
    u8 b08;
    u8 b09;
    s16 s0A;
    s16 s0C;
    s16 s0E;
} fldMapBitRec_t;

/* One placed map object: id class in the high bits, grid cell, corner offset
 * and direction index, then the model and motion to load. */
typedef struct fldMapObjRec_t
{
    u16 id;
    u16 ix;
    u16 iy;
    u8 ofs;
    u8 dir;
    u16 major;
    u16 minor;
    u16 motion;
    u16 pad;
} fldMapObjRec_t;

typedef struct fldGenbaTbl_t
{
    u8 unk00[4];
    s16 yofs;
    u8 unk06[8];
    u16 evtBit;
    u8 unk10[0x20];
} fldGenbaTbl_t;

typedef struct MAP_OBJ_t
{
    dds3Basic_t *basic;
    s16 use;
    s16 uniq;
    s16 visible;
    s16 act;
    f32 x;
    f32 y;
    f32 z;
    f32 r;
    f32 s;
    u32 cut_bit;
    char scr_name[32];
    f32 hitsize;
    s16 evehitdir;
    s16 evehitsize;
    s16 spcial;
    s16 noclip;
    s16 floor;
    s16 npc_idx;
    s16 npc_cng;
    s16 model_id1;
    s16 model_id2;
    s16 user_param[4];
    s32 npc_kuse_wait;
    s32 npc_kuse_time;
    s32 motion_pause_flag;
    f32 motion_pause_spd;
} MAP_OBJ_t;

typedef struct MAP_GRIDEVEHIT_t
{
    s16 use;
    s16 on;
    s16 id;
    s16 ix;
    s16 iy;
    s16 iw;
    s16 ih;
    s8 act;
    s8 dir;
    f32 x;
    f32 y;
    f32 z;
    u16 flag0;
    u16 flag1;
    u8 flag0j;
    u8 flag1j;
    char scr_name[28];
    u16 msk;
} MAP_GRIDEVEHIT_t;

typedef struct MAP_GRIDHIT_t
{
    s16 use;
    s16 flag;
    s32 floor;
    s16 ix;
    s16 iy;
    s16 iw;
    s16 ih;
    s32 attr;
    s16 mode;
    u16 flag0;
    u16 flag1;
    u8 flag0j;
    u8 flag1j;
} MAP_GRIDHIT_t;

typedef struct MAP_WALLOBJ_t
{
    s16 use;
    s16 uniq;
    s16 type;
    s16 dir;
    s16 flag;
    s16 encountid;
    s16 ix;
    s16 iy;
    s16 iw;
    s16 ih;
    s16 vix0;
    s16 viy0;
    s16 vix1;
    s16 viy1;
    s16 eveix0;
    s16 eveiy0;
    s16 eveix1;
    s16 eveiy1;
    s16 alpha;
    s16 haba;
    s16 view;
    s16 link_idx;
    s32 floor;
} MAP_WALLOBJ_t;

typedef struct MAP_STOPOBJ_t
{
    s32 use;
    s16 uniq;
    s16 dir;
    s16 ix;
    s16 iy;
    s16 iw;
    s16 ih;
    u16 flag0;
    u16 flag0j;
    u16 flag1;
    u16 flag1j;
} MAP_STOPOBJ_t;

typedef struct MAP_LIGHTOBJ_t
{
    s16 use;
    s16 uniq;
    f32 x;
    f32 y;
    f32 z;
    f32 in_r;
    f32 out_r;
    s32 dif_rgba;
    s32 amb_rgba;
} MAP_LIGHTOBJ_t;

typedef struct MAP_WORK_GENBA_t
{
    s16 use;
    s16 uniq;
    f32 x;
    f32 y;
    f32 z;
} MAP_WORK_GENBA_t;

typedef struct MAP_GRIDWARPHIT_t
{
    s16 use;
    s16 id;
    s16 ix;
    s16 iy;
    s16 iw;
    s16 ih;
    s16 act;
    s16 dir;
    s16 se;
    s16 fieldID;
    s32 areaID;
    s32 posID;
    s32 w_ix;
    s32 w_iy;
    f32 x;
    f32 y;
    f32 z;
} MAP_GRIDWARPHIT_t;

typedef struct MAP_KAIDAN_t
{
    s16 use;
    s16 uniq;
    s16 type;
    s16 xory;
    f32 x0;
    f32 z0;
    f32 x1;
    f32 z1;
    f32 h0;
    f32 h1;
    f32 haba;
    f32 allhaba;
    f32 alltaka;
    f32 alldan;
    f32 oneh;
    f32 addh;
    s32 basyo;
    s32 resv;
} MAP_KAIDAN_t;

typedef struct MAP_CHIT_t
{
    s16 use;
    s16 uniq;
    f32 x;
    f32 z;
    f32 r;
} MAP_CHIT_t;

typedef struct MAP_WHIT_t
{
    s16 use;
    s16 uniq;
    f32 x0;
    f32 z0;
    f32 x1;
    f32 z1;
    f32 vx;
    f32 vz;
    s32 attr;
} MAP_WHIT_t;

typedef struct MAP_SAKA4_t
{
    s16 use;
    s16 uniq;
    s32 resv[3];
    f32 x0;
    f32 y0;
    f32 z0;
    f32 x1;
    f32 y1;
    f32 z1;
    f32 x2;
    f32 y2;
    f32 z2;
    f32 x3;
    f32 y3;
    f32 z3;
} MAP_SAKA4_t;

extern MAP_OBJ_t mapObjTaskData2[128];
extern MAP_GRIDEVEHIT_t mapScriptGet_PosData2[128];
extern MAP_GRIDHIT_t mapData23[128];
extern MAP_GRIDWARPHIT_t mapData24[80];
extern MAP_WALLOBJ_t mapWallObjTaskData[64];
extern MAP_STOPOBJ_t mapData32[32];
extern MAP_LIGHTOBJ_t mapData25[16];
extern MAP_KAIDAN_t mapData27[64];
extern MAP_CHIT_t mapData28[256];
extern MAP_WHIT_t mapData29[128];
extern MAP_SAKA4_t mapData30[32];
extern MAP_WORK_GENBA_t mapData31[32];

typedef struct MAP_POINT_t
{
    fldResourcePoint_t resource;
    dds3Basic_t *basic;
    u32 serialID;
    s16 use;
    s16 uniq;
    f32 x;
    f32 y;
    f32 z;
    f32 r;
    s16 floor;
    s16 pad;
} MAP_POINT_t;

extern MAP_POINT_t mapData26[MAP_POINT_MAX];
extern const sdfVector4Unaligned_t mapObjPointAdd2Table;
extern const sdfVector4Unaligned_t mapObjFloorAdd_CashTable;
extern const sdfVector4Unaligned_t mapObjFloorAdd_CashTable2;

typedef struct MAP_WORK_HIT_LIST_t
{
    f32 line_x0;
    f32 line_z0;
    f32 line_x1;
    f32 line_z1;
    f32 line_c_len;
    f32 line_vx;
    f32 line_vz;
    s32 type;
} MAP_WORK_HIT_LIST_t;

extern MAP_WORK_HIT_LIST_t mapData34[];
extern s32 map_work_hit_list_num;
extern s32 mapHamidasiWHitSubSub(f32 *outx, f32 *outz, f32 newx, f32 newz, f32 r, f32 x0, f32 z0, f32 x1, f32 z1, f32 vx, f32 vz);
extern s32 mapHamidasiMoveBoxHitSub(f32 *outx, f32 *outz, f32 newx, f32 newz, f32 r, f32 x0, f32 z0, f32 x1, f32 z1, f32 vx, f32 vz);

/* The loaded map file and the pointers into it. The words between the named
 * pointers are zeroed together when the map is freed. */
extern sdfMemHandle_t *fldFreeMapDataData;
extern fldMapFileHead_t *mapFileLoadData7;
extern void *pFldMap;
extern fldMapAreaHead_t *pMapAreaNow;
extern fldMapAreaHead_t *pMapAreaDat;
/* One collision grid cell (HD MapColm_t). */
typedef struct MapColm_t {
    u8 hit;
    u8 cut;
    u8 attr;
    s8 hight;
} MapColm_t;

extern MapColm_t *pMapColmNow;
extern MapColm_t *pMapColmDat;
extern fldMapHit64Rec_t *mapMapSetAreaRefreshData;
extern void *fldData25;
extern fldMapPointRec_t *mapMapSetAreaRefreshData2;
extern void *fldData26;
extern fldMapHit20Rec_t *mapMapSetAreaRefreshData3;
extern void *fldData27;
extern fldMapObjRec_t *pMapObjNow;
extern fldMapObjRec_t *pMapObjSub;
extern fldMapKaidanRec_t *mapMapSetAreaRefreshData4;
extern void *fldData28;
extern fldMapEveHitHead_t *pMapEveHitNow;
extern void *fldData29;
/* Per-area sprite block at fldMapAreaHead_t.extraOfs; the first two words are file offsets. */
typedef struct mapOt2Block_t {
    s32 ofs00;   /* to the mapData35 cell grid */
    s32 ofs04;   /* to the mapData36 block */
    u8 unk08[8];
    s16 w;
    s16 h;
    u8 unk14[4];
    s16 spriteNum;
} mapOt2Block_t;

typedef struct mapOt2Cell_t {
    u8 id;
    u8 unk01;
} mapOt2Cell_t;

extern mapOt2Block_t *mapCallSetOt2Data2[64];
extern fldMapBitRec_t mapCallSetOt2Data[128];
extern fldMapLightRec_t *mapFileLoadData5[64];
extern void *mapFileLoadData6[64];
extern mapOt2Cell_t *mapData35[64];
extern void *mapData36[64];
extern u8 mapFileLoadData2[];
extern u8 mapFileLoadData3[];
extern u8 mapFileLoadData4[];
extern u8 mapData6[][32];
extern u32 mapFileLoadData;
extern s32 mapData18;
extern fldMapFlagRec_t mapData7[64];

#ifdef VERSION_RAIDOU1
/* Map script table: the script of (map, floor) when both event bits agree.
 * kind 0 sets initScript, otherwise startScript. */
typedef struct mapCallBitCheckData_t {
    s16 map;
    s16 floor;
    s16 kind;
    s16 once;
    u16 bit;
    u16 bitOn;
    u16 bit2;
    u16 bit2On;
    char name[32];
} mapCallBitCheckData_t;

extern mapCallBitCheckData_t mapCallBitCheckData[96];

/* The other MAPALL.TBL tables, loaded by fldAllTblLoad. */
extern u8 mapData[6400];
extern u8 mapData2[3456];
extern u8 nbCallGetIntParaData[4096];
extern u8 mapData3[1024];
extern u8 p2sdData[1536];
extern u8 mapData4[5120];
extern s8 mapData5[48][64];
extern u8 fldData2[48];
extern u8 mapMapObjectRefreshData[512];
extern u8 nbCallGetIntParaData4[192];
extern u8 nbData[16];
extern u8 mapData8[1024];
extern u8 mapData9[1024];
extern u8 mapData10[1024];
extern u8 mapData11[1024];
/* Event hit mask row, 20 bytes (HD MskEveHitTbl_t plus padding). */
typedef struct MskEveHitTbl_t {
    s16 _map;
    s16 _floor;
    s16 _EveHitId;
    s16 _EffPosId;
    s16 _EffPosY;
    s16 _PlayerPosId;
    s16 _NekoPosId;
    s16 _NakamaPosId;
    s16 _OverCutId;
    s16 pad12;
} MskEveHitTbl_t;
extern MskEveHitTbl_t mapMskEveHitTbl[16];
#endif
extern s32 mapBtlReLoadMapSBtlSceneSub2Data;
void mapBtlReLoadMapSBtlSceneSub2(void);
extern s32 mapNpcTblRefresh_WaitData;
extern s32 mapData114;
extern s32 mapData115;

extern sdfMemHandle_t *mapData99;
extern fldMapSub3File_t *mapFileLoadData8;
extern fldMapSub3File_t *mapData100;
extern fldMapSubHead_t *mapMapSetAreaRefreshData5;
extern fldMapSubHead_t *mapData101;
extern fldMapPointRec_t *mapMapSetAreaRefreshData6;
extern fldMapPointRec_t *mapMapSetAreaRefreshData7;
extern fldMapHit64Rec_t *mapData102;
extern void *mapMapSetAreaData17;

extern sdfMemHandle_t *mapData105;
extern fldMapSub2File_t *mapFileLoadData9;
extern fldMapSub2File_t *mapFileLoadData10;
extern fldMapLightHead_t *mapData106;
extern void *mapData107;
extern fldMapLightCell_t *mapData108;
extern void *mapData109;

extern sdfMemHandle_t *mapData110;
extern fldMapSub4File_t *mapFileLoadData11;
extern fldMapSub4File_t *mapData111;
extern MpbAreaHead_t *mapMapSetAreaData18;
extern void *mapMapSetAreaData19;
extern void *mapMapSetAreaData20;
extern void *mapMapSetAreaData21;
extern MpbEncHead_t *mapData112;
extern s32 fldFreeMapDataData2;
extern MpbPoint_t *pMpbPointNow;
/* ALLTBL.LB resources 0 and 3, layouts unknown. */
extern u8 mapEXFloorData[];
extern u8 mapWork5[];
extern s32 fldFreeMapDataData3;
extern MpbWall_t *mapData113;
extern s32 fldFreeMapDataData4;
extern u8 *mapMapSetAreaData22;
extern void *mapMapSetAreaData23;

extern fldMapAreaHead_t *mapMapSetAreaData[64];
extern MapColm_t *mapMapSetAreaData2[64];
extern void *mapMapSetAreaData3[64];
extern void *mapMapSetAreaData4[64];
extern void *mapMapSetAreaData5[64];
extern void *mapMapSetAreaData6[64];
extern void *mapMapSetAreaData7[64];
extern void *mapMapSetAreaData8[64];
extern void *mapMapSetAreaData9[64];
extern void *mapMapSetAreaData10[64];
extern void *mapMapSetAreaData11[64];
extern void *mapMapSetAreaData12[64];
extern void *mapMapSetAreaData13[64];
extern MpbAreaHead_t *mapMapSetAreaData14[64];
extern void *mapMapSetAreaData15[64];
extern void *mapMapSetAreaData16[64];
extern f32 mapData103;
extern f32 mapData104;

extern fldGenbaTbl_t fldGenbaTbl[];
extern f32 mapData20[16];
extern s32 mapMapObjectRefresh_fldData[];
extern s32 evtWorkScriptContextReturnValue;
extern s32 evtWorkScriptContextReturnValue2;

void mapMapObjectRefresh_fld(void);
s32 mapObjFloorAdd_Cash(s32 floor, s16 uniq, MODELTYPE major, s32 minor, s32 motion, u32 cut_bit, s32 cash, f32 x, f32 y, f32 z, f32 r, f32 s);
s32 mapAddHukidashi(s32 uniq, s32 major, s32 minor, s32 motion, u32 cut_bit, s32 cash, f32 x, f32 y, f32 z, f32 r, f32 s);
s32 mapObjectRefreshSub(s32 uniq, s32 major, s32 minor, s32 motion, u32 cut_bit, f32 x, f32 y, f32 z, f32 r, f32 s);
void mapObjLightAdd(s16 uniq, f32 x, f32 y, f32 z, f32 in_r, f32 out_r, f32 dif_r, f32 dif_g, f32 dif_b, f32 amb_r, f32 amb_g, f32 amb_b);
void mapObjEveHitSet2(s16 uniq, s32 act, s32 dir, f32 size, char *scr_name);
void mapNpcTblAdd(s32 uniq, f32 x, f32 y, f32 z);
s32 mapLoadGenbaEpl(void);
f32 mapGetY(f32 x, f32 z);
void mapCHitAdd(s16 uniq, f32 x, f32 z, f32 r);
void mapWHitAdd(s16 uniq, s32 attr, f32 x0, f32 z0, f32 x1, f32 z1, f32 vx, f32 vz);
void mapSetAreaRefreshSub(s32 ix, s32 iy, s32 type, s32 bit);
void mapSetAreaRefreshSub2(s32 floor, s32 uniq, s32 a2, s32 a3, s32 a4, s32 a5, s32 a6, s32 a7, s32 a8, s32 a9);
void mapCallBitCheck2(s32 floor, s32 uniq, s32 kind, s32 a3, s32 a4, s32 a5, s32 a6, s32 a7, s32 a8);
void mapCallPositioningGuardSystem2(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5, s32 a6, s32 a7, s32 a8, s32 a9, s32 a10, s32 a11, void *a12);
void mapCallPositioningGuardSystem3(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5, s32 a6, s32 a7, s32 a8, s32 a9, s32 a10);
/* Run-map refresh grid: w * h 64-bit cells, one bit per refresh type. */
extern sdfMemHandle_t *fldRunMapData;
extern u64 *fldRunMapCellData;
extern s32 pFroMapHead;
extern s32 fldRunMapScale;
extern s32 fldData24;
void mapCallMemAllocDirect(s32 w, s32 h);
void fldCallMemFreeDirect(void);
void mapCallMemFreeDirect(void);
void mapNpcTblRefresh(void);
s32 mapFileLoad(s32 mapid);
void mapFileLoadSub2(void);
void mapFileLoadSub3(s16 uniq, s32 a1);
s32 mapFileLoadSub4(s32 floor, s32 kind);
void mapFileLoadSub5(s32 a0, f32 a1, f32 a2, f32 a3, f32 a4, f32 a5, f32 a6, f32 a7, f32 a8, f32 a9, f32 a10, f32 a11, f32 a12);
void mapFileLoadSub6(f32 *outx, f32 *outz, f32 x, f32 z);
s32 mefMistTexOnDataCheck(void);
void mapFileLoadSub8(s32 a0);
void mapFileLoadSub9(s32 a0, f32 a1);
void mapFileLoadSub10(s32 a0);
s32 mdlCallMemGetSize(sdfMemHandle_t *handle, s32 major, s32 minor);
void mdlCallMemGetSize2(void);
struct sdfMat_t *dds3CallMatNew2(struct sdfTexHandle_t *tex);
void pipeFileLoadSub(s32 a0);
void pipeFileLoadSub2(s32 a0);
void mapLoadMistTexture(void);
void mapCallTexAllocFromHead(void);
void mapEXFloor(s32 a0);
void mapObjTaskSub(s32 a0, s32 a1);
void mapNpcPathCalc_RotGoal(s32 a0, struct dds3Basic_t *basic);
void fldSe_Init(void);
s32 mapRefDDS3GBWK5(s32 id);
s32 mapObjPointGet2(s16 uniq, f32 *x, f32 *y, f32 *z, f32 *r);
void mapFreeMapDataSub(void);
void workRelease(void);
void nbMoveFormation(dds3Basic_t *basic);
void fldTitleMini_Shutdown(void);
extern struct sdfMemHandle_t *fldTitleMiniWork4;
extern struct effHandle_t *fldTitleMiniTex3;
void fldWallObjTaskSub(s32 floor, s32 ix, s32 iy, f32 *x, f32 *z);

extern const char mapStrMapCutSetFromPos[];
void mapCutSet_From_Pos(f32 x, f32 y, f32 z);

/* Mist particle system (HD mefMistSystem_t), 0x11040 bytes from sdfMemAllocDirect. */
typedef struct mefMistParticle_t {
    f32 pos[2];
    u8 unk08[8];
    f32 velocity[2];
    u8 unk18[0x58 - 0x18];
    u32 lifeCnt;
    u32 deathCnt;
    u32 fExist;
    u8 unk64[0x70 - 0x64];
} mefMistParticle_t;

typedef struct mefMistSystem_t {
    mefMistParticle_t particle[512];
    u8 unkE000[0x11000 - 0xE000]; /* HD rgbaArray, posArray */
    sdfMemHandle_t *hMem;
    u32 num;
    struct sdfTexHandle_t *texHandle;
    u32 cnt;
    s32 mode;
    s32 ot;
    s32 scroll;
    s32 alpha;
    s32 saturation;
    s32 brightness;
    u32 frequency;
    u32 fRun;
    u32 fDebugMenu;
    u32 debugMenuIndex;
    u8 fDebugTimeGraph;
    s32 alphaMax;
} mefMistSystem_t;

extern mefMistSystem_t *pMapMoya;
void mefMistUpdate(void);
void mefMistEmitParticle(void);
void mefMistUpdateParticle(void);
void fldCallGetRandFloatA(mefMistParticle_t *particle);

extern u32 mapSKYBGEDITData5;
extern s32 mapBgSetScrollData20;
extern sdfMemHandle_t *hWorkArea2;
extern s32 sdfDiscRequestSemaphore2;
void setCngCameraDir(void);
void mapEffEplEnd(s32 idx);
void mapEffEplAllEnd(void);

/* Effect slot of the EPL table, 33 entries (HD MAP_EPL_EFF_WORK_t). */
typedef struct MAP_EPL_EFF_WORK_t {
    sdfVector4_t pos;
    sdfMemHandle_t *hMem;
    void *pMem;
    struct effHandle_t *eHdl;
    s32 view;
    f32 yukay;
    s32 startwait;
    s32 time;
    s32 resv;
} MAP_EPL_EFF_WORK_t;

extern MAP_EPL_EFF_WORK_t mapEffEplStartWork[33];
void mapEffEplAllDrawTask(void);
void mapCutSetSub(s32 cut);

void mapEveHitMsk_Find(s32 evehitid);

void fldJmsQuestMapLoad_fld(s32 area, s32 sub);
void mapCallFilterSet(s32 no);

/* Bg scroll state (HD tzoom_player_n_*, fscroll_*). */
extern s32 tzoom_event;
extern s32 tzoom_event_time;
extern s32 tzoom_event_cnt;
extern f32 mapZoomSetGoalData;
extern f32 mapZoomSetGoalData2;
extern f32 mapZoomSetGoalData3;
extern f32 mapData59;
extern f32 mapZoomSetGoalData4;
extern f32 mapZoomSetGoalData5;
extern f32 mapZoomSetGoalData6;
extern f32 mapZoomSetGoalData7;
extern f32 mapZoomSetGoalData8;
extern s32 fscroll_mode;
extern s32 mapData61;
extern s32 mapBgSetScrollData3;
extern f32 mapData62;
extern f32 mapData63;
extern f32 mapData64;
extern f32 mapData65;
extern f32 mapData66;
extern f32 mapData67;
extern f32 mapData68;
extern f32 mapData69;
extern f32 mapData70;
void mapBgFScrollOn(f32 x, f32 y, f32 z, s32 hokan);
void sndSePlay_sndAt3(s32 hokan);

#endif
