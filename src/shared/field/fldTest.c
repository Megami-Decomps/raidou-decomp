#include "common.h"
#include "sdk_libgcc.h"
#include "basic.h"
#include "dds3obj.h"
#include "kernel.h"
#include "libsdf.h"
#include "ps2asm.h"
#include "newdata.h"
#include "field.h"
#include "event.h"
#include "model.h"
#include "motion.h"
#include "sdfFrame.h"
#include "dds3KernelCore.h"
#include "effManager.h"

typedef struct {
    u16 sizeX;
    u16 sizeZ;
    u32 cellsOffset;
} fldRunMapGrid_t;

#include "sdk_libc.h"
#include "effPart.h"

/* One loadable field effect: the resource handle trio the loader fills, then
 * the live effect or part handle and the placement the task applies. */
typedef struct {
    sdfMemHandle_t *mhFile;
    void *ptr;
    s32 size;
    void *pcf;
    void *d3p;
    s32 cnt;
    f32 x;
    f32 y;
    f32 z;
    f32 r;
} fldTest_FLDRUN_EFF_t;

typedef struct {
    fldTest_FLDRUN_EFF_t *pEff;
    s32 time;
    s32 effmode;
    s32 rotmode;
    f32 off_cam_range_min;
    f32 off_cam_range_max;
} fldTest_FLDRUN_EFFTBL_t;

extern const char fldTama1UnitName[], fldTama2UnitName[], fldTama3UnitName[];
extern const char fldDsparkPcfPath[], fldDsmoke2Path[], fldDspark02Path[], fldGun02aPath[];
extern const char fldGun01a0Path[], fldMuzzleB010Path[], fldDsmoke1Path[], fldDflash1Path[];

typedef struct {
    f32 f[10];
} fldTamaSeFrame_t;

typedef struct {
    s16 v[10];
} fldTamaSeVol_t;

typedef struct {
    char name[8];
    s16 ix;
    s16 iz;
    f32 angle;
} fldDanteDoor_t;

/* Dante's chase work block. Only the fields the chase code reaches are known. */
typedef struct
{
    s32 status;
    f32 x;
    f32 y;
    f32 z;
    f32 turn;
    f32 oldframe;
    f32 nowframe;
    s16 ix;
    s16 iz;
    u16 targetIx;
    u16 targetIz;
    u16 tikamiti;
    s16 wkmode;
    fldMapCell_t *routeCell;
    void *RouteDataTop;
    s32 now;
    s32 goal;
    s32 nextframe;
    f32 angleToPlayer;
    f32 speed;
    s32 cnt_ex;
    s32 gunlight_anime;
    u32 flag;
    s32 update_flag;
    s32 bgm_mode;
    f32 d_p_len;
    f32 d_p_dir;
    f32 d_c_dir;
    s32 stance;
    fldDanteDoor_t *pDoor;
    u16 mapIxOld;
    u16 mapIzOld;
    s32 old_status;
} fldTest_FLDRUN_DANTE_t;

extern const char fldTestDrawName[];


extern s32 ooyTestDrawProc(dds3ProcessID_t *aProcId);
s32 ooyTestDrawProc(dds3ProcessID_t *aProcId);
void InitOoyTest(void);

/* NON_MATCHING. Behaviour checked against retail: 6 PCSX2 call vectors give the same return and memory,
 * field underground facility, field unknown realm savestates, covering only the paths those calls took.
 * (sdfport bc29478, tools/funcvec, tests/funcvec.sh). */
extern const char fldTestDrawName[16];
extern const char fldTama1UnitName[16];
extern const char fldTama2UnitName[16];
extern const char fldDsparkPcfPath[24];
extern const char fldDsmoke2Path[24];
extern const char fldDspark02Path[24];
extern const char fldGun01a0Path[24];
extern const char fldMuzzleB010Path[32];
extern const char fldDsmoke1Path[24];

void InitOoyTest(void)
{
    dds3StartProcess(fldTestDrawName, DDS3_USERDRAW_PRIORITY, 0, 0, ooyTestDrawProc, NULL, NULL);
}

INCLUDE_RODATA(const s32, "field/fldTest", fldTestDrawName);

