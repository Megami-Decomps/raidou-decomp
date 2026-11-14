#include "common.h"

/* Declarations from smt3-decomp src/smt3/event/evtPicture.c */
#include "common.h"

#include "kernel.h"

#include "libsdf.h"

#include "sdfFrame.h"

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

extern s32 evtPictureSpritePos[];

extern s32 evtPictureSpriteUv[];

extern evtPictureColor_t evtPictureSpriteColor;

extern sdfGrOt_t sdfGrOtTable[];

extern void eglSetOt(s32 ot);

extern void eglLinkTest(s32 ate, s32 atst, s32 aref, s32 afail, s32 date, s32 datm, s32 zte, s32 ztst);

extern void eglLinkAlpha(s32 mode);

extern void *sdfGrAllocPacketBuff(s32 aSize);

extern void sdfDrawTagInit(sdfDrawTag_t *a);

extern void itfMiscDrawAlphaPrim(sdfDrawTag_t *pdrawtag, s32 tp, s32 context);

extern void itfMiscDrawSpritePrim(void *pos, void *uv, void *color, s32 z,
                                  void *texhandle, s32 context, sdfDrawTag_t *pdrawtag);

extern void *sdfChipAllocDirect(u32 aSize);

extern void sdfChipFreeDirect(void *aPtr);

extern void sdfTexFreeDelayed(void *aHandle);

extern void *itfMiscLoadTexture(const char *filename);

extern dds3ProcessID_t *dds3StartProcess(char *v1, u32 dDS3_PRIORITY_DRAW_PB, s32 v2, s32 v3,
                                         dds3ProcessFunc_t drawExit, dds3ProcessFunc_t deleteExit,
                                         void *p);

extern void *dds3GetProcessData(dds3ProcessID_t *id);

extern evtPictureProcessData_t *InitPictureProcessData(void);

extern s32 evtPictureProcess1(dds3ProcessID_t *id);

extern void *evtPicturePriList;

extern void evtPicturePriClear(void);

extern s32 dds3KillProcess(dds3ProcessID_t *id, s32 shutdowntimer);

extern void dds3SetProcessData(dds3ProcessID_t *id, void *data);

extern char evtPicturePriProcName[];

void evtPicturePriClear(void);

void evtPicturePriAdd(evtPicturePriTbl_t *indata);

void evtPicturePriDraw(void);

void evtPictureDrawOn(dds3ProcessID_t *id);

void evtPictureDrawOff(dds3ProcessID_t *id);

void evtKillPictureProcess(dds3ProcessID_t *id);

void evtPictureSetOt(dds3ProcessID_t *id, s32 ot);

void evtPictureSetPri(dds3ProcessID_t *id, s32 pri);

void evtPictureSetSize(dds3ProcessID_t *id, s32 sw, s32 sh, s32 tw, s32 th);

void evtPictureSetPos(dds3ProcessID_t *id, s32 x0, s32 y0, s32 x1, s32 y1, s32 time);

void evtPictureSetAlpha(dds3ProcessID_t *id, s32 alp0, s32 alp1, s32 time);

void evtPictureSetZoom(dds3ProcessID_t *id, s32 zoom0, s32 zoom1, s32 time);

void evtPictureSwap(void);

s32 evtPictureStat(dds3ProcessID_t *id);

evtPictureProcessData_t *InitPictureProcessData(void);

void LoadTexture(evtPictureProcessData_t *data, const char *texpath);

s32 evtPictureProcess1(dds3ProcessID_t *id);

s32 evtPictureShutdown(dds3ProcessID_t *id);

dds3ProcessID_t *evtStartPictureProcess2(u32 priority, const char *texdata);

dds3ProcessID_t *evtStartPictureProcess(u32 priority, void *texpath);

s32 picpri_Process(dds3ProcessID_t *id);

void picpri_Shutdown(dds3ProcessID_t *id);

void evtPicturePriProcessInit(void);

extern void *evtPicturePriList;

extern char evtPictureProcName[8];

extern char evtPicturePriProcName[7];
/* End of declarations from src/smt3/event/evtPicture.c */

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

INCLUDE_ASM(const s32, "event/evtPicture", func_r1_001BACE8);

INCLUDE_ASM(const s32, "event/evtPicture", func_r1_001BAD70);

INCLUDE_ASM(const s32, "event/evtPicture", func_r1_001BADA0);

INCLUDE_ASM(const s32, "event/evtPicture", func_r1_001BADC0);

INCLUDE_ASM(const s32, "event/evtPicture", func_001BAF98);

INCLUDE_ASM(const s32, "event/evtPicture", func_r1_001BB130);

INCLUDE_ASM(const s32, "event/evtPicture", func_r1_001BB240);

dds3ProcessID_t *evtStartPictureProcess2(u32 priority, const char *texdata)
{
    evtPictureProcessData_t *work = InitPictureProcessData();

    LoadTexture(work, texdata);
    return dds3StartProcess(evtPictureProcName, priority, 1, 1, evtPictureProcess1, evtPictureShutdown, work);
}

INCLUDE_ASM(const s32, "event/evtPicture", evtStartPictureProcess);

INCLUDE_ASM(const s32, "event/evtPicture", func_r1_001BB368);

INCLUDE_ASM(const s32, "event/evtPicture", evtPictureLoadStartTexFile);

INCLUDE_ASM(const s32, "event/evtPicture", func_r1_001BB430);

INCLUDE_ASM(const s32, "event/evtPicture", func_r1_001BB458);

INCLUDE_ASM(const s32, "event/evtPicture", func_r1_001BB4C8);

INCLUDE_ASM(const s32, "event/evtPicture", func_r1_001BB4F8);

INCLUDE_ASM(const s32, "event/evtPicture", func_r1_001BB558);

INCLUDE_ASM(const s32, "event/evtPicture", func_r1_001BB588);

INCLUDE_ASM(const s32, "event/evtPicture", func_r1_001BB5F8);
INCLUDE_SDATA(const s32, "event/evtPicture", D_0046C7F8);

char evtPictureProcName[8] = "Picture";

u16 skyanime = 0;

