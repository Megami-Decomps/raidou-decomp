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
extern void *sdfDevPrintf(s32 aX, s32 aY, s32 aZ, s32 aAttr, const char *aStr, ...);
extern void sdfDrawTagLink(sdfDrawTag_t *aTag, void *aPacket);
void MovieViewerDrawFrameInfo(void);
extern const char movieViewerProcName[];
extern void movieViewerFree(void);
void FinishMovieViewer(void);
void movieViewerSetFlags(void);
void movieViewerInitRegs(void);
extern void *memcpy(void *a, const void *b, size_t c);
void movieViewerDrawInfo(void);

/* "%08X", "%02X" and "**", in .sdata where a literal cannot go. */
extern char movieViewerFmtWord[];
extern char movieViewerFmtByte[];
extern char movieViewerBlankByte[];

/* The memory window: a cursor over the eight nibbles of either source address,
 * the word above and the 8x8 block below. */
extern const char movieViewerProcName[16];
extern char movieViewerFmtWord[8];
extern char movieViewerFmtByte[8];
extern char movieViewerBlankByte[8];

void MovieViewerDrawFrameInfo(void)
{
    sdfDrawTag_t *tag;

    if (dds3WaitMovieEnd() == 0) {
        tag = movieViewerWork.tag;
        sdfDrawTagLink(tag, dds3PrimFrameRect(0x8810, 0x85E8, 0xFF0080, 0x720, 0x90,
                                              0x30000000, 0x60404040));
        sdfDrawTagLink(tag, sdfDevPrintf(0x8840, 0x8600, 0xFF0080, 0, "%04d/%04d",
                                         movieCb.decodeCb.frame,
                                         movieCb.decodeCb.frameCount));
    }
}

INCLUDE_ASM(const s32, "movie/MovieViewer", MovieViewerProc);

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

/* The memory window: a cursor over the eight nibbles of either source address,
 * the word above and the 8x8 block below. */
const char movieViewerProcName[16] = "movieViewer";

INCLUDE_ASM(const s32, "movie/MovieViewer", movieViewerDebugDraw);
INCLUDE_SDATA(const s32, "movie/MovieViewer", movieViewerFmtWord);

INCLUDE_SDATA(const s32, "movie/MovieViewer", movieViewerFmtByte);

INCLUDE_SDATA(const s32, "movie/MovieViewer", movieViewerBlankByte);

