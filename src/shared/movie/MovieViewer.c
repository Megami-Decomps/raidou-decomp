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
#include "sdk_libc.h"
#include "dds3GlobalWork.h"
#include "sdfDevCons.h"
#include "dds3KernelCore.h"
#include "sdfDraw.h"
#include "dds3PadManager.h"
#include "dds3KernelDraw.h"
#include "sdfPrim.h"
#include "kernel_draw.h"


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
void MovieViewerDrawFrameInfo(void);
extern const char movieViewerProcName[];
void FinishMovieViewer(void);
void movieViewerSetFlags(void);
void movieViewerInitRegs(void);
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
        sdfDrawTagLink(tag, EE_ADDR(dds3PrimFrameRect(0x8810, 0x85E8, 0xFF0080, 0x720, 0x90,
                                              0x30000000, 0x60404040)));
        sdfDrawTagLink(tag, EE_ADDR(sdfDevPrintf(0x8840, 0x8600, 0xFF0080, 0, "%04d/%04d",
                                         movieCb.decodeCb.frame,
                                         movieCb.decodeCb.frameCount)));
    }
}

INCLUDE_ASM(const s32, "movie/MovieViewer", MovieViewerProc);

void InitMovieViewer(void)
{
    movieViewerScanDir();
    movieViewerWork.proc = dds3StartProcess(movieViewerProcName, 0x2B00, 1, 0, MovieViewerProc, NULL, NULL);
}

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

s32 movieViewerDebugDraw(void)
{
    sdfDrawTag_t *tag;
    s32 step;
    s32 i;
    s32 x;
    s32 y;
    s32 col;

    if (movieViewerDebugFlag.started == 0) {
        movieViewerDebugFlag.started = 1;
        movieViewerInitRegs();
    }
    if (dds3PadBitTable[0x13] != 0) {
        movieViewerSetFlags();
    } else if (dds3PadBitTable[0x12] != 0) {
        movieViewerInitRegs();
    } else if (dds3PadBitTable[0x15] & 2) {
        movieViewerDebugFlag.cursor++;
        if (movieViewerDebugFlag.cursor == 0x10) {
            movieViewerDebugFlag.cursor = 0;
        }
    } else if (dds3PadBitTable[0x14] & 2) {
        if (movieViewerDebugFlag.cursor != 0) {
            movieViewerDebugFlag.cursor--;
        } else {
            movieViewerDebugFlag.cursor = 0xF;
        }
    } else {
        step = 1 << ((~movieViewerDebugFlag.cursor & 7) * 4);
        i = movieViewerDebugFlag.cursor >> 3;
        if (dds3PadBitTable[0x17] & 2) {
            if (i == 0) {
                movieViewerDebugFlag.wordPending = 0;
                movieViewerDebugFlag.regAddr += step;
            } else {
                movieViewerDebugFlag.blockPending = 0;
                movieViewerDebugFlag.buffer += step;
            }
        }
        if (dds3PadBitTable[0x16] & 2) {
            if (i == 0) {
                movieViewerDebugFlag.wordPending = 0;
                movieViewerDebugFlag.regAddr -= step;
            } else {
                movieViewerDebugFlag.blockPending = 0;
                movieViewerDebugFlag.buffer -= step;
            }
        }
    }
    movieViewerDrawInfo();
    tag = dds3FontReqAllocDrawTag();
    sdfDrawTagLink(tag, EE_ADDR(dds3PrimFrameRect(0x7150, 0x79A8, 0xFF007E, 0x1860, 0x3F0, 0x60000000,
                                          0x40806020)));
    sdfPrimLinkRect(tag, 0x80A03000, 0, (movieViewerDebugFlag.cursor & 7) * 0xC0 + 0x7180,
                    (movieViewerDebugFlag.cursor >> 3) * 0xC0 + 0x79C0,
                    (movieViewerDebugFlag.cursor & 7) * 0xC0 + 0x7240,
                    (movieViewerDebugFlag.cursor >> 3) * 0xC0 + 0x7A20, 0xFF007F, 0);
    sdfDrawTagLink(tag, EE_ADDR(sdfDevPrintf(0x7180, 0x79C0, 0xFF0080, 0, movieViewerFmtWord,
                                     movieViewerDebugFlag.regAddr)));
    if (movieViewerDebugFlag.wordPending != 0) {
        sdfDrawTagLink(tag, EE_ADDR(sdfDevPrintf(0x7840, 0x79C0, 0xFF0080, 0, movieViewerFmtWord,
                                         movieViewerDebugFlag.word)));
    } else {
        sdfDrawTagLink(tag, EE_ADDR(sdfDevPrintf(0x7840, 0x79C0, 0xFF0080, 0, "********")));
    }
    step = 0;
    y = 0x7A80;
    for (i = 0; i != 8; i++, y += 0x60) {
        sdfDrawTagLink(tag, EE_ADDR(sdfDevPrintf(0x7180, y, 0xFF0080, 0, movieViewerFmtWord,
                                         movieViewerDebugFlag.buffer + step)));
        x = 0x7840;
        for (col = 0; col != 8; col++, x += 0x240, step++) {
            if (movieViewerDebugFlag.blockPending != 0) {
                sdfDrawTagLink(tag, EE_ADDR(sdfDevPrintf(x, y, 0xFF0080, 0, movieViewerFmtByte,
                                                 movieViewerDebugFlag.block[step])));
            } else {
                sdfDrawTagLink(tag, EE_ADDR(sdfDevPrintf(x, y, 0xFF0080, 0, movieViewerBlankByte)));
            }
        }
    }
    dds3Data10.regFunc(&dds3Data10, tag);
    return 0;
}
INCLUDE_SDATA(const s32, "movie/MovieViewer", movieViewerFmtWord);

INCLUDE_SDATA(const s32, "movie/MovieViewer", movieViewerFmtByte);

INCLUDE_SDATA(const s32, "movie/MovieViewer", movieViewerBlankByte);

