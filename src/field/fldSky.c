#include "common.h"

/* Declarations from smt3-decomp src/smt3/field/fldSky.c */
#include "fpu.h"

#include "common.h"

#include "libsdf.h"

#include "kernel.h"

#include "ps2asm.h"

#include "sdfFrame.h"

#include "basic.h"

#include "billboard.h"

#include "draw.h"

#include "scr.h"

#include "draw3d.h"

#include "field.h"

#include "model.h"

#include "motion.h"

#include "newdata.h"

#include "event.h"

#include "effect.h"

typedef struct fldLightSet_t fldLightSet_t;

struct fldLightSet_t {
    u8 fadeArea;
    u8 pad1[3];
    s32 displayStateSlot14;
    s32 fadeMode;
    s32 swayMode;
    u8 pad10[0xC];
    s32 fogNearValue;
    s32 depthNearValue;
    s32 fogFarValue;
    s32 depthFarValue;
    s32 drawColorFixedR;
    s32 drawColorFixedG;
    s32 drawColorFixedB;
    f32 lightColorAX;
    f32 lightColorAY;
    f32 lightColorAZ;
    f32 lightDirectionAX;
    f32 lightDirectionAY;
    f32 lightDirectionAZ;
    f32 lightColorBX;
    f32 lightColorBY;
    f32 lightColorBZ;
    f32 lightDirectionBX;
    f32 lightDirectionBY;
    f32 lightDirectionBZ;
    f32 lightColorCX;
    f32 lightColorCY;
    f32 lightColorCZ;
    f32 lightDirectionCX;
    f32 lightDirectionCY;
    f32 lightDirectionCZ;
    f32 backgroundColorX;
    f32 backgroundColorY;
    f32 backgroundColorZ;
    f32 unitColorA[3];
    f32 unitLightDirection[3];
    u8 padA4[0x30];
    f32 unitColorB[3];
};

enum {
    FIELD_VALUE_RECORD_STORAGE_SIZE = 0x72000,
    FIELD_AUX_RECORD_STORAGE_SIZE = 0x4A00,
    FIELD_ROOM_RECORD_COUNT = 64,
    FIELD_ROOM_CORNER_COUNT = 8,
    FIELD_ROOM_PLANE_COUNT = 6,
    FIELD_ACTOR_SLOT_COUNT = 256
};

typedef struct devState_t devState_t;

extern void *memset(void *, int, size_t);

f32 sdfSin(f32 aRadius);

extern fldDanteGunState_t fldRunDanteGunFlag;

extern void dds3ObjectBaseOffFlag(void *pObjectBasic, s32 flag);

extern void dds3ObjectBaseOnFlag(void *object, s32 mask);

extern u32 fldDanteUnit;

extern u32 fldPlayerUnit;

extern f32 fldSkyLight[];

extern evtUnit_t *evtGetUnitHandle2(dds3Basic_t *unit);

extern void evtUnitLightOn(evtUnit_t *h);

extern void evtSetUnitLightAndAmbient(evtUnit_t *h, s32 frame, u32 lrgb, u32 argb);

extern void evtSetUnitLightDir(evtUnit_t *h, s32 frame);

extern dds3Basic_t *fldUnitControlHandle;

extern void dds3UnitObjectBasicEL_LinkFree(dds3Basic_t *unitBasic);

extern u32 fldSkyTable;

extern u32 fldSkyTableIdx;

extern u32 fldSkyLightMaTbl;

extern u32 fldSkyDisplayPointers[];

extern u32 D_002FCAA0[];

extern f32 fldSkyPlayerIdx[];

extern void *fldSkyDefaultData;

/* Filter records: a mode followed by the 8-by-9 corner-color grid (292 bytes). */
typedef struct {
    s32 mode;
    s32 colors[8][9];
} fldSkyData_t;

extern fldSkyData_t *fldSkyDataTable;

extern void *fldSkyDefault;

extern s32 fldSkyTblUpperIdx;

extern s32 fldSkyFilterAnimeSaved;

extern f32 fldSkyFilterAnimeCntFSaved;

extern s32 fldSkyFilterAnimeOfsASaved;

extern s32 fldSkyFilterPrevSaved;

extern s32 fldSkyFilterSaved;

extern f32 fldSkyFilterRateSaved;

extern f32 fldSkyFilterTimeSaved;

extern s32 fldSkyTblAnimeSaved;

extern f32 fldSkyTblAnimeCntISaved;

extern s32 fldSkyTblBaseIdxSaved;

extern s32 fldSkyTableSaved;

extern void fldSkySetPlayer(s32 intr, f32 r0, f32 g0, f32 b0,
                            f32 r3, f32 g3, f32 b3,
                            f32 x0, f32 y0, f32 z0);

extern s32 fldSkyEnable;

extern s32 fldSkyFilter;

extern s32 fldSkyFilterPrev;

extern f32 fldSkyFilterRate;

extern f32 fldSkyFilterTime;

extern void dds3SetFogColor(s32, f32 *);

extern s32 dds3SetLightColor(s32 frame, s32 no, f32 *col);

extern void dds3SetAmbientColor(s32, f32 *);

extern s32 dds3SetLightDir(s32 frame, s32 no, f32 *dir);

extern void dds3SetFogPara(s32 frame, f32 max, f32 min, f32 near, f32 far);

sdfMemHandle_t *sdfMemAllocDirect(s32 aSize);

u32 sdfMemLock(sdfMemHandle_t *aHandle);

extern addr_t fldItemBoxObjBuf;

extern addr_t fldMatterTable;

extern addr_t fldItemBoxObjBufHandle;

extern addr_t fldMatterTableHandle;

void sdfMemUnlock(sdfMemHandle_t *aHandle);

void sdfMemFreeDelayedDirect(sdfMemHandle_t *aHandle);

void fldMakeFilePath(char *pathstr, s32 field, s32 area);

devState_t *sdfFileOpen(const char *aFileName);

s32 sdfFileRead(devState_t *aHandle, void *aPtr, s32 aLeng);

void sdfFileClose(devState_t *aHandle);

#include "sdk_libc.h"

extern void *memcpy(void *, const void *, size_t);

s32 evtBitCheck(s32 no);

extern s32 gSkyTblAnime;

extern f32 gSkyTblAnimeCntI;

extern s32 gSkyTblBaseIdx;

extern void fldSkySetTbl(s32 idx, s32 hokan);

extern s32 gFilterAlpha;

extern s32 gFilterAnime;

extern f32 gFilterAnimeCntF;

extern s32 gFilterAnimeOfsA;

void fldSkyDefaultLoad(void);

void fldSkyFieldLoad(s32 field);

void fldGetSkyEntry(const void *src);

void fldSkyCalcFilter(void);

void fldSkySetFilter(s32 idx, s32 mode, s32 hokan);

void fldSetFilterAnime(s32 anime);

void fldSkySetAlpha(s32 alp);

s32 fldSkyGetAlpha(void);

void fldSkySetAlphaOfs(s32 alp);

s32 fldSkyGetAlphaOfs(void);

void fldSkySetTableAnime(s32 type);

/* The dispatch runs ahead of all three bodies, which a switch or an if chain
 * over the same three values does not reproduce. */
void fldSkyCalcTable(void);

void fld_136800(s32 duration, f32 lightR, f32 lightG, f32 lightB,
                f32 ambientR, f32 ambientG, f32 ambientB);

void fldSkySetPlayer(s32 intr, f32 r0, f32 g0, f32 b0,
                     f32 r3, f32 g3, f32 b3,
                     f32 x0, f32 y0, f32 z0);

void fldSkySetTbl(s32 idx, s32 hokan);

void fldSkySetTblModosu(s32 hokan);

void fldSkySetTblUpper(s32 idx, s32 hokan);

void fldSkySetTblLightMaDZ(s32 idx);

void fldSkySetTblLightMa(s32 idx, s32 hokan);

void fldSetFileResolverIdx(u32 state);

void fld_1374F0(void);

void fldSkyRefreshTbl(void);

void fldSkyBattle(void);

void fldAllocPlaneTable(void);

void fldFreeMatterTable(void);

/* The collision records' planes sit at 4 mod 16, after two s16 fields. */
f32 fldVecDot(const sdfVector4Unaligned_t *a, const sdfVector4Unaligned_t *b);

extern void *fldSkyDefault;

extern void *fldSkyDefaultData;

extern fldSkyData_t *fldSkyDataTable;

extern s32 gFilterAlpha;

extern s32 gFilterAnime;

extern f32 gFilterAnimeCntF;

extern s32 gFilterAnimeOfsA;

extern s32 fldSkyFilterPrev;

extern s32 fldSkyFilter;

extern f32 fldSkyFilterRate;

extern f32 fldSkyFilterTime;

extern u32 fldSkyTable;

extern s32 gSkyTblAnime;

extern f32 gSkyTblAnimeCntI;

extern s32 gSkyTblBaseIdx;

extern s32 fldSkyTblUpperIdx;

extern s32 fldSkyFilterAnimeSaved;

extern f32 fldSkyFilterAnimeCntFSaved;

extern s32 fldSkyFilterAnimeOfsASaved;

extern s32 fldSkyFilterPrevSaved;

extern s32 fldSkyFilterSaved;

extern f32 fldSkyFilterRateSaved;

extern f32 fldSkyFilterTimeSaved;

extern u32 fldSkyTableIdx;

extern s32 fldSkyTblAnimeSaved;

extern f32 fldSkyTblAnimeCntISaved;

extern s32 fldSkyTblBaseIdxSaved;

extern u32 fldSkyLightMaTbl;

extern s32 fldSkyTableSaved;

extern s32 fldHitMatterIndex;

extern addr_t fldItemBoxObjBuf;
/* End of declarations from src/smt3/field/fldSky.c */

void fld_1374F0(void) {
    u32 *displayPointers = fldSkyDisplayPointers;

    memset(displayPointers, 0, 0x14);
    displayPointers[0] = EE_ADDR(D_002FCAA0);
    displayPointers[1] = EE_ADDR(fldSkyPlayerIdx);
}
