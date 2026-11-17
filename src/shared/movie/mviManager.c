#include "common.h"
#include "facility.h"
#include "fpu.h"
#include "libsdf.h"
#include "newdata.h"
#include "ps2asm.h"
#include "sdfFrame.h"
#include "event.h"
#include "sdf_movie_types.h"
#include "sdk_libc.h"
#include "sdfChip.h"
#include "dds3GlobalWork.h"
#include "sdfIpu.h"
#include "sdfPrim.h"
#include "sdfDevCons.h"
#include "sdfMovie.h"
#include "mviManager.h"
#include "dds3KernelCore.h"
#include "sdfDraw.h"
#include "dds3KernelDraw.h"
#include "dds3PadManager.h"
#include "itfMesManager.h"
#include "scrCommand.h"
#include "scrTraceCode.h"


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

typedef struct
{
    /* 0x00 */ const char *path;
    /* 0x04 */ sdfMovieInitParm_t descriptor;
    /* 0x18 */ s32 unk_18; /* copied to movieData on play */
} dds3MovieEntry_t;

extern dds3MovieEntry_t moviePlayData[];

extern void moviePlayFile_27041744(const char *path, sdfMovieInitParm_t *descriptor);
extern u16 dds3VblanksPerFrame;
extern dds3ProcessID_t *PlayingNo;
extern void sdfIpuRunDecode(s32 frames);
extern void (*DrawPhaseTable[])(fclEncyc_instance_tag *work);
s32 movieDrawProc(dds3ProcessID_t *id);
extern const char movieDrawProcName[];
void moviePlayFile_27041744(const char *path, sdfMovieInitParm_t *descriptor);
void movieStop(void);
s32 movieMan_Process(dds3ProcessID_t *id);
extern const char movieManProcName[];
void *dds3StartMovieManager(void);

typedef struct
{
    /* 0x00 */ u8 unk_00[4];
    /* 0x04 */ void *chip;
    /* 0x08 */ s16 top;
    /* 0x0A */ s16 cursor;
    /* 0x0C */ u16 unk_0C;
    /* 0x0E */ s8 playing;
    /* 0x0F */ u8 pad_0F;
    /* 0x10 */ sdfDrawTag_t *packets;
} movieViewerChipList_t;

/* One entry: the next link, then the path it was scanned from. */
typedef struct movieViewerChip_t
{
    /* 0x00 */ struct movieViewerChip_t *next;
    /* 0x04 */ char path[1];
} movieViewerChip_t;

extern movieViewerChipList_t movieViewerWork;
void movieViewerFree(void);

typedef struct
{
    u32 mode;
    u8 stat[0x3C];
    char name[0x100];
    u32 privateWords[2];
} mviDirectoryEntry_t;

extern char movieExtIpu[];
extern char movieExtIps[];
void *movieViewerStep(void);


/* "%c%s", in .sdata where a literal cannot go. */
extern char movieDebugRowFmt[];
void movieDebugDrawInfo(void);
extern const char movieDrawProcName[16];
extern const char movieManProcName[16];

void moviePlayFile_27041744(const char *path, sdfMovieInitParm_t *descriptor)
{
    if (PlayingNo == NULL)
    {
        movieOpenStream(&movieCb, descriptor, path);
        PlayingNo = dds3StartProcess(movieDrawProcName, 0x2AFB, 1, 1, movieDrawProc, NULL, NULL);
    }
}

void moviePlay(s32 aNum) {
    dds3MovieEntry_t *entry = &moviePlayData[aNum];

    movieData = entry->unk_18;
    moviePlayFile_27041744(entry->path, &entry->descriptor);
}

void movieStop(void)
{
    if (PlayingNo != NULL) {
        movieCloseStream(&movieCb);
        dds3KillProcess(PlayingNo, 0);
        PlayingNo = NULL;
    }
}

/* body: matched in smt3 @ 9401f79e: src/smt3/movie/mviManager.c:106, found, same function 1.00 (words 7/7, calls 1.00, shape 1.00) */
s32 mviValidateMovieSub(void)
{
    return sdfMovieIsFinished(&movieCb);
}

s32 mviDrawMessageSub(void) {
    return movieCb.decodeCb.playbackPhase;
}

INCLUDE_ASM(const s32, "movie/mviManager", mviLeaf);

s32 movieMan_Process(dds3ProcessID_t *id)
{
    sdfIpuRunDecode(SDF_FRAMES(60) / dds3VblanksPerFrame);
    return 0;
}

void *dds3StartMovieManager(void)
{
    sdfIpuInit();
    return dds3StartProcess(movieManProcName, 0x385, 1, 0, movieMan_Process, NULL, NULL);
}

INCLUDE_ASM(const s32, "movie/mviManager", fldCommand_FLD_BGM_CONTINUE);

INCLUDE_ASM(const s32, "movie/mviManager", titleFreeWork);

/* NON_MATCHING: 41 of 42 words match.  */
#ifdef NON_MATCHING
s32 mnuUpdateMovieDrawShutdownCountdown(void) {
    if (mnuUpdateMovieDrawShutdownCountdownData == 0 && scrRefDDS3GBWK3() == 1 && dds3Data[0] < 0) {
        mnuUpdateMovieDrawShutdownCountdownData = 1;
        dds3SetFilter2Para(0x44);
        dds3SetFilter2RGBA(0x80000000);
        dds3Filter2On(0x1E);
        itfPanelReleaseHold();
    }
    if (mnuUpdateMovieDrawShutdownCountdownData > 0) {
        if (mnuUpdateMovieDrawShutdownCountdownData == 0x1E) {
            movieStop();
            scrSetIntReturnValue(0);
            return 1;
        }
        mnuUpdateMovieDrawShutdownCountdownData = mnuUpdateMovieDrawShutdownCountdownData + 1;
    }
    scrSetIntReturnValue(mviValidateMovieSub() == 0);
    return 1;
}
#else
INCLUDE_ASM(const s32, "movie/mviManager", mnuUpdateMovieDrawShutdownCountdown);
#endif

INCLUDE_ASM(const s32, "movie/mviManager", mviFunc);

void movieViewerFree(void)
{
    void *chip = movieViewerWork.chip;

    if (chip != NULL) {
        do {
            void *next = *(void **)chip;

            sdfChipFreeDirect(chip);
            chip = next;
        } while (chip != NULL);
        movieViewerWork.chip = NULL;
        movieViewerWork.top = 0;
        movieViewerWork.cursor = 0;
        movieViewerWork.unk_0C = 0;
    }
}

const char movieDrawProcName[16] = "movieDraw";

const char movieManProcName[16] = "movieMan";

INCLUDE_ASM(const s32, "movie/mviManager", movieViewerScanDir);

/* The entry the cursor sits on, walked from the head. */
void *movieViewerStep(void)
{
    movieViewerChip_t *chip = movieViewerWork.chip;
    s32 remaining = movieViewerWork.cursor;

    if (chip != NULL && remaining > 0) {
        do {
            chip = chip->next;
            remaining--;
        } while (chip != NULL && remaining > 0);
    }
    return chip;
}

/* One screen of the movie list, with an arrow on the selected row and a
 * marker at each end the list continues past. */
void movieDebugDrawInfo(void)
{
    movieViewerChip_t *chip;
    sdfDrawTag_t *tag;
    s32 selected;
    s32 i;

    if (movieViewerWork.chip == NULL || movieViewerWork.playing != 0) {
        return;
    }
    tag = movieViewerWork.packets;
    sdfDrawTagLink(tag, EE_ADDR(dds3PrimFrameRect(0x7150, 0x7948, 0xFF0080, 0xF60, 0x3F0, 0x30000000,
                                          0x60404040)));
    selected = movieViewerWork.cursor;
    i = movieViewerWork.top;
    chip = movieViewerWork.chip;
    selected -= i;
    for (; i > 0; i--) {
        chip = chip->next;
    }
    for (i = 0; i < 8 && chip != NULL; i++, chip = chip->next) {
        sdfDrawTagLink(tag, EE_ADDR(sdfDevPrintf(0x7240, 0x79C0 + i * 0x60, 0xFF0080, 0, movieDebugRowFmt,
                                         (i == selected) ? '>' : ' ', chip->path)));
    }
    if (movieViewerWork.top != 0) {
        sdfPrimLinkF3(tag, 0x8000A0C0, 0, 0x7900, 0x7978, 0x7840, 0x79A8, 0x79C0, 0x79A8,
                      0xFF0080, 0);
    }
    if (chip != NULL) {
        sdfPrimLinkF3(tag, 0x8000A0C0, 0, 0x7840, 0x7CD8, 0x79C0, 0x7CD8, 0x7900, 0x7D08,
                      0xFF0080, 0);
    }
}
dds3ProcessID_t *PlayingNo = NULL;

INCLUDE_SDATA(const s32, "movie/mviManager", mnuUpdateMovieDrawShutdownCountdownData);

#ifdef VERSION_RAIDOU2
INCLUDE_SDATA(const s32, "movie/mviManager", MovieViewerProcData3);

#endif
INCLUDE_SDATA(const s32, "movie/mviManager", movieExtIpu);

INCLUDE_SDATA(const s32, "movie/mviManager", movieExtIps);

INCLUDE_SDATA(const s32, "movie/mviManager", movieDebugRowFmt);

