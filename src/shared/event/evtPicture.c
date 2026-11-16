#include "common.h"
#include "kernel.h"
#include "libsdf.h"
#include "sdfFrame.h"
#include "evtPicture.h"
#include "dds3KernelCore.h"

/* 0x68-byte picture work block. InitPictureProcessData sets the defaults and
 * evtPictureProcess1 steps the three interpolators. */
typedef struct
{
    u32 flag;
    void *tex;
    s32 ot;
    s32 x;
    s32 y;
    s32 alpha;
    f32 zoom;
    s32 size_sw;
    s32 size_sh;
    s32 size_tw;
    s32 size_th;
    s32 pri;
    s32 pos_x0;
    s32 pos_y0;
    s32 pos_x1;
    s32 pos_y1;
    s32 pos_time;
    s32 pos_cnt;
    s32 alp_a0;
    s32 alp_a1;
    s32 alp_time;
    s32 alp_cnt;
    s32 zoom_p0;
    s32 zoom_p1;
    s32 zoom_time;
    s32 zoom_cnt;
} evtPictureProcessData_t;

/* One 0x30-byte row of the eight-entry priority table picpri_Process draws. */
typedef struct
{
    void *tex;
    s32 ot;
    f32 x0;
    f32 y0;
    f32 x1;
    f32 y1;
    s32 u0;
    s32 v0;
    s32 u1;
    s32 v1;
    s32 alpha;
    s32 pri;
} evtPicturePriTbl_t;

extern char evtPictureProcName[];

/* The four GS colour components itfMiscDrawSpritePrim packs; only alpha is set
 * per sprite. */
typedef struct
{
    u32 components[4];
} evtPictureColor_t;


extern evtPictureProcessData_t *InitPictureProcessData(void);
extern s32 evtPictureProcess1(dds3ProcessID_t *id);
void evtPictureDrawOn(dds3ProcessID_t *id);
void evtPictureDrawOff(dds3ProcessID_t *id);
void evtKillPictureProcess(dds3ProcessID_t *id);
evtPictureProcessData_t *InitPictureProcessData(void);
void LoadTexture(evtPictureProcessData_t *data, const char *texpath);
s32 evtPictureProcess1(dds3ProcessID_t *id);
s32 evtPictureShutdown(dds3ProcessID_t *id);
dds3ProcessID_t *evtStartPictureProcess2(u32 priority, const char *texdata);
extern char evtPictureProcName[8];

void evtPictureDrawOn(dds3ProcessID_t *id)
{
    evtPictureProcessData_t *work = dds3GetProcessData(id);
    work->flag |= 1;
}

void evtPictureDrawOff(dds3ProcessID_t *id)
{
    evtPictureProcessData_t *work = dds3GetProcessData(id);
    work->flag &= ~1;
}

void evtKillPictureProcess(dds3ProcessID_t *id)
{
    dds3KillProcess(id, 1);
}

INCLUDE_ASM(const s32, "event/evtPicture", InitProcessData);

INCLUDE_ASM(const s32, "event/evtPicture", evtCallMiscLoadTexture);

INCLUDE_ASM(const s32, "event/evtPicture", evtLeaf8);

INCLUDE_ASM(const s32, "event/evtPicture", evtFunc);

#ifdef VERSION_RAIDOU2
INCLUDE_ASM(const s32, "event/evtPicture", evtCutInCreateSub);

#endif
INCLUDE_ASM(const s32, "event/evtPicture", evtCallGetSmallFontFRQ2);

INCLUDE_ASM(const s32, "event/evtPicture", evtCallGetProcessData14);

INCLUDE_ASM(const s32, "event/evtPicture", evtCallGetProcessData15);

dds3ProcessID_t *evtStartPictureProcess2(u32 priority, const char *texdata)
{
    evtPictureProcessData_t *work = InitPictureProcessData();

    LoadTexture(work, texdata);
    return dds3StartProcess(evtPictureProcName, priority, 1, 1, evtPictureProcess1, evtPictureShutdown, work);
}

INCLUDE_ASM(const s32, "event/evtPicture", evtStartPictureProcess);

INCLUDE_ASM(const s32, "event/evtPicture", evtCallGetMemHandle);

INCLUDE_ASM(const s32, "event/evtPicture", evtPictureLoadStartTexFile);

INCLUDE_ASM(const s32, "event/evtPicture", evtPictureLoadCheckProcess);

INCLUDE_ASM(const s32, "event/evtPicture", evtCallGetProcessData17);

INCLUDE_ASM(const s32, "event/evtPicture", evtCallGetProcessData18);

INCLUDE_ASM(const s32, "event/evtPicture", evtPictureSetSize);

INCLUDE_ASM(const s32, "event/evtPicture", evtPictureSetEndProcess);

INCLUDE_ASM(const s32, "event/evtPicture", StartProcess_evtPicture);

INCLUDE_ASM(const s32, "event/evtPicture", evtPictureStartProcess4);
#ifdef VERSION_RAIDOU1
INCLUDE_SDATA(const s32, "event/evtPicture", evtCallGetSmallFontFRQ2Data);
#else
INCLUDE_SDATA(const s32, "event/evtPicture", evtData28);

INCLUDE_SDATA(const s32, "event/evtPicture", evtCallGetSmallFontFRQ2Data2);
#endif

char evtPictureProcName[8] = "Picture";

u16 skyanime = 0;
