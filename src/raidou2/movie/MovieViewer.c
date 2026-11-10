#include "common.h"

/* Declarations from smt3-decomp src/smt3/movie/MovieViewer.c */
#include "common.h"

#include "eeregs.h"

#include "mviManager.h"

#include "facility.h"

#include "fpu.h"

#include "libsdf.h"

#include "newdata.h"

#include "ps2asm.h"

#include "event.h"

#include "kernel.h"

#include "sdf_movie_types.h"

extern dds3ProcessID_t *dds3StartProcess(char *name, u32 priority, s32 startTimer, s32 shutdownTimer,
                                      dds3ProcessFunc_t update, dds3ProcessFunc_t shutdown, void *work);

extern void *dds3SearchProcessName(const char *namestr);

typedef struct
{
    /* 0x0000 */ u8 flags;
    /* 0x0001 */ u8 unk_0001[0x13];
    /* 0x0014 */ void *allocation;
    /* 0x0018 */ u8 unk_0018[0x44B0];
    /* 0x44C8 */ void *unk_44C8;
    /* 0x44CC */ s32 unk_44CC;
    /* 0x44D0 */ s32 unk_44D0;
} rcvWork_t;

extern rcvWork_t *shutdownfunc;

extern sdfMovieCb_t movieCb;

extern void movieStop(void);

/* The viewer's memory window: one word and one 0x40-byte block, each copied
 * from its source address when its pending flag is set. */
typedef struct
{
    /* 0x00 */ u8 started;
    /* 0x01 */ u8 cursor;
    /* 0x02 */ u8 wordPending;
    /* 0x03 */ u8 blockPending;
    /* 0x04 */ u32 regAddr;
    /* 0x08 */ u8 *buffer;
    /* 0x0C */ s32 word;
    /* 0x10 */ u8 block[0x40];
} movieViewerWork_t;

extern movieViewerWork_t movieViewerDebugFlag;

extern s32 dds3KillProcess(dds3ProcessID_t *id, s32 shutdowntimer);

extern void (*DrawPhaseTable[])(fclEncyc_instance_tag *work);

extern s32 dds3WaitMovieEnd(void);

extern void *dds3PrimFrameRect(s32 aX, s32 aY, s32 aZ, s32 aW, s32 aH, u32 aBgCol, u32 aFrameCol);

/* The viewer process' own state; the draw tag is at 0x10. */
typedef struct
{
    /* 0x00 */ void *proc;
    /* 0x04 */ s32 unk_04;
    /* 0x08 */ s16 scrollTop;
    /* 0x0A */ s16 cursor;
    /* 0x0C */ s16 count;
    /* 0x0E */ s8 playing;
    /* 0x0F */ s8 unk_0F;
    /* 0x10 */ sdfDrawTag_t *tag;
} movieViewerProcWork_t;

extern movieViewerProcWork_t movieViewerWork;

extern s8 dds3PadButtonState[];

extern sdfMovieInitParm_t movieViewerPlayParm;

extern u16 sdfPadButtons[2];

extern u16 movieViewerPrevButtons;

extern s32 movieViewerPhase;

extern sdfGrOt_t sdfGrOtTable[];

SYMBOL_AT(sdfGrOt_t, D_002F67F8, sdfGrOtTable, 0x8E0);

extern void *movieViewerStep(void);

extern void moviePlayFile_27041744(const char *path, sdfMovieInitParm_t *descriptor);

extern void dds3PlayMovieByIndex(s32 index);

extern sdfDrawTag_t *sdfPrimAllocDrawTag(void *aAllocator);

extern void movieDebugDrawInfo(void);

extern char *strcpy(char *dst, const char *src);

extern char *strcat(char *dst, const char *src);

extern void *sdfDevPrintf(s32 aX, s32 aY, s32 aZ, s32 aAttr, const char *aStr, ...);

extern void sdfDrawTagLink(sdfDrawTag_t *aTag, void *aPacket);

void MovieViewerDrawFrameInfo(void);

s32 MovieViewerProc(dds3ProcessID_t *process);

extern void movieViewerScanDir(void);

extern s32 MovieViewerProc(dds3ProcessID_t *process);

extern char movieViewerProcName[];

void InitMovieViewer(void);

extern char movieViewerProcName[];

extern void movieViewerFree(void);

void FinishMovieViewer(void);

void movieViewerSetFlags(void);

void movieViewerInitRegs(void);

extern void *memcpy(void *a, const void *b, size_t c);

/* Refresh the window from its two source addresses. */
void movieViewerDrawInfo(void);

extern s8 dds3PadBitTable[];

extern sdfDrawTag_t *dds3FontReqAllocDrawTag(void);

extern void sdfPrimLinkRect(sdfDrawTag_t *aTag, s32 aColor, s32 aPrim, s32 aX0, s32 aY0,
                            s32 aX1, s32 aY1, s32 aZ, s32 (*aAllocator)(s32));

SYMBOL_AT(sdfGrOt_t, D_002F6938, sdfGrOtTable, 0xA20);

/* "%08X", "%02X" and "**", in .sdata where a literal cannot go. */
extern char movieViewerFmtWord[];

extern char movieViewerFmtByte[];

extern char movieViewerBlankByte[];

s32 movieViewerDebugDraw(void);
/* End of declarations from src/smt3/movie/MovieViewer.c */

INCLUDE_ASM(const s32, "movie/MovieViewer", MovieViewerDrawFrameInfo);

INCLUDE_ASM(const s32, "movie/MovieViewer", func_001E92A0);

INCLUDE_ASM(const s32, "movie/MovieViewer", InitMovieViewer);

void FinishMovieViewer(void)
{
    void *proc = dds3SearchProcessName(movieViewerProcName);

    if (proc != NULL) {
        dds3KillProcess(proc, 0);
        movieViewerWork.proc = NULL;
        movieStop();
    }
    movieViewerFree();
}

void movieViewerSetFlags(void)
{
    movieViewerDebugFlag.wordPending = 1;
    movieViewerDebugFlag.blockPending = 1;
}

void movieViewerInitRegs(void)
{
    movieViewerDebugFlag.regAddr = EE_ADDR(IPU_CTRL);
    movieViewerDebugFlag.buffer = (u8 *)&movieCb;
    movieViewerSetFlags();
}

/* Refresh the window from its two source addresses. */
void movieViewerDrawInfo(void)
{
    if (movieViewerDebugFlag.wordPending != 0) {
        {
            movieViewerDebugFlag.word = *EE_ADDR_AS(s32 *, movieViewerDebugFlag.regAddr);
        }
    }
    if (movieViewerDebugFlag.blockPending != 0) {
        {
            memcpy(movieViewerDebugFlag.block, movieViewerDebugFlag.buffer, 0x40);
        }
    }
}

INCLUDE_RODATA(const s32, "movie/MovieViewer", D_004BBE88);

INCLUDE_ASM(const s32, "movie/MovieViewer", movieViewerDebugDraw);
INCLUDE_SDATA(const s32, "movie/MovieViewer", D_004E1540);

INCLUDE_SDATA(const s32, "movie/MovieViewer", D_004E1548);

INCLUDE_SDATA(const s32, "movie/MovieViewer", D_004E1550);

