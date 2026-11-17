#include "common.h"
#include "mcManager.h"
#include "sdf_resource.h"
#include "basic.h"
#include "model.h"
#include "newdata.h"
#include "scr.h"
#include "field.h"
#include "ps2asm.h"
#include "sdfFrame.h"
#include "sdfMemory.h"
#include "fileManager.h"
#include "sdfSound.h"
#include "dds3AdminiProcess.h"
#include "slMain.h"
#include "dds3KernelMain.h"
#include "sdfTex.h"
#include "dds3WorldBasic.h"
#include "fldProcess.h"
#include "code_r1_002C8188.h"
#include "rstInit.h"
#include "code_r1_002CAFD8.h"
#include "dds3KernelDraw.h"
#include "dds3KernelDraw_0010A750.h"
#include "sdfMath.h"
#include "itfMesManager.h"
#include "EventBit.h"
#include "dds3KernelCore.h"
#include "code_r1_001D5158.h"
#include "frFont.h"


enum sl_slot_flag
{
    SL_SLOT_FLAG_CAN_READ_SAVE = 1,
    SL_SLOT_FLAG_DIRECTORY_READY = 2,
    SL_SLOT_FLAG_DELETE_BEFORE_WRITE = 8
};

extern s32 gSlWindowCnt;
extern s32 gslInfoCode;
extern s32 slChooseSaveIndex;
extern u32 mccb;
extern saddr_t slChooseData_getCardInfoNext(void);
extern void mcMakeFileName(char *path, s32 num);
saddr_t slHeadLoadProc_openFile(void);
extern s32 slMcFd;
extern sdfMemHandle_t *slLoadFileFd;
saddr_t slHeadLoadProc_readFile(void);
extern saddr_t slNoCard(void);
extern s32 slHeadLoadProc_closeFile(void);
extern void *slExitWaitNextFunc;
s32 slExitWaitProc(void);
extern s32 slExitWaitCnt;
extern s32 gslMode;
extern saddr_t slTop(void);
s32 slEndingExit(void);
extern u32 gslErrorCode;
extern u32 slQueryYesNoMsg;
extern u32 gslDrawFlagFile;
extern saddr_t slQueryYesNo(s32 (*yesFunc)(void), s32 (*noFunc)(void), s32 yesNoDefault);
saddr_t slSaveData(void);
extern saddr_t slFileSel(void);
s32 slExitToTitle(void);
extern s32 gslCheckCardWait;
extern u32 gSlNowCardCheck;
extern u32 gslDrawFlagBg;
extern sdfMemHandle_t *slBaseIconMemHandle;
extern addr_t baseIconFileHandle;
extern u32 gslErrorKeyFlag;
extern s8 dds3PadButtonState[];

/* One step of the save and load state machine, which this unit holds as an
 * address and calls through a cast. */
typedef s32 (*slProcFunc_t)(dds3ProcessID_t *);
extern u32 slHitAnyKeyNextFunc;
saddr_t slChooseData_getCardInfo2(void);
extern const char slIconSysName[];
extern u8 slDirEntries[];
s32 slChooseData_getCardInfo3(void);
saddr_t slChooseData_getCardInfo2_B(void);
s32 slChooseData_getCardInfo3_B(void);
s32 slChooseData_getCardInfoNext_B(void);
saddr_t slSaveData_chdir(void);
saddr_t slSaveProc_Write_mkdir(void);
saddr_t slSaveProc_Write_chdir(void);
extern s32 slFileSelError;
saddr_t slSaveData_deleteIconSys(void);
extern s32 slSaveData_writeFile(void);
extern const char slBaseIconName[];
extern s32 slSaveWriteEnd_baseIcon(void);
extern void *slBaseIconBuff;
extern u32 slBaseIconBuffSize;
saddr_t slSaveWriteFile(const char *filename, void **buff, u32 *size, void *endFunc, u32 *srcPtr);
extern void **slSaveWriteFile_buff;
extern u32 *slSaveWriteFile_size;
extern void *slSaveWriteFile_endFunc;
extern u32 *slSaveWriteFile_dataWaitPtr;
saddr_t slSaveProc_openFile(void);
saddr_t slSaveProc_waitDataReady(void);
saddr_t slSaveProc_writeFile(void);
saddr_t slSaveProc_closeFile(void);
s32 slCardChk(void);
saddr_t slLoadProc_readFile(void);
saddr_t slLoadProc_closeFile(void);
extern s32 gslAnimCnt;
extern s32 slDraw(s32);
extern s32 slQueryYesNoCursor;
extern s32 slQueryYesNoKeyFlag;
extern s32 (*proc_func_ans)(dds3ProcessID_t *process);

#include "sdk_libc.h"

extern saddr_t slFormatStart(void);

typedef struct slFileName_t {
    s8 ch[20];
} slFileName_t;

/* The 0x34-byte save preview: the renderer reads playTime as a word and
 * endcnt/newfile as signed halfwords. The last count is for the encoded
 * player name, not map state (nmeBuildNameFont and slCopyNonZero). */
typedef struct slFileRec_t {
    s8 verstr[3];
    s8 ver;
    s8 tmnlMode;
    s8 tmnlId;
    s8 level;
    s8 mode;
    s32 playTime;
    s16 endcnt;
    s16 newfile;
    slFileName_t name;
    s8 nameLength;
    s8 unk25[15];
} slFileRec_t;

SIZE_CHECK(slFileRecSizeCheck, slFileRec_t, 0x34);

void mcMakeFileName(char *path, s32 num);

/* Save fields used by revision conversion; the party and encyclopedia
 * records share the persistent allocation owned by dds3GlobalWorkInit. */
typedef struct {
    u8 unknown00[0x34];
    sdfMemHandle_t *memHandle;
    u8 unknown38[0xA54 - 0x38];
    datUnitWork_t unitwork[16];
    u8 unknown1914[0x11E90 - 0x1914];
    fclencyceelem_t encyclopedia[184];
} slConvertWork_t;

s32 slChkWindowOpenDone(void);
void slSetInfo(u32 id);
saddr_t slHeadLoad(void);
s32 slHeadLoadProc_closeFile(void);
s32 slEndingTop(void);
s32 slEndingUpd2(void);
saddr_t slBootTop(void);
saddr_t slBootRetry(void);
s32 slBootError(void);
s32 slBootError2(void);
saddr_t slNoCard(void);
saddr_t slTop(void);
s32 slExit(void);
saddr_t slFileSel(void);
s32 slBootProc1st(void);
s32 slErrorWait(void);
saddr_t slHitAnyKey(addr_t nextFunc);
saddr_t slChooseData_getCardInfoNext(void);
saddr_t slSaveProc_Write_deleteIconSys(void);
s32 slSaveWriteEnd_baseIcon(void);
s32 slSaveWriteEnd_saveData(void);
s32 slSaveData_writeFile(void);
saddr_t slProc_Format(void);
saddr_t slFormatStart(void);
extern f32 slFileSelPulsePhase, slFileSelPulseStep, slFileSelWork;
extern char slNewFileLabel[], slCorruptedFileLabel[], slNewCycleLabel[];
extern char slNoFileLabel[], slCycleLabel[];
extern char slNum2dFmt[], slNum3dFmt[], slNum02dFmt[], D_0054F098[], D_0054F0A0[];
extern char D_00311450[][0x20], D_00311850[][0x20];
extern const char slIconSysName[9];
extern const char slBaseIconName[9];
s32 slCalc(dds3ProcessID_t *id);
s32 slDraw(s32 id);
saddr_t slQueryYesNo(s32 (*yesFunc)(void), s32 (*noFunc)(void), s32 yesNoDefault);

#include "sdk_libkernl.h"

#define SDK_SKIP_memset

#include "include_asm.h"
#include "billboard.h"
#include "sdf_types.h"
#include "draw.h"
#include "libsdf.h"
#include "sdf_movie_types.h"
#include "fpu.h"
#include "evt_world.h"
#include "code_r1_0021CE60.h"
#include "code_r1_001CC1D0.h"

typedef s32 (*FileMenuContinuationCallback)(void);

typedef struct MenuResourceWork MenuResourceWork;

struct mnuListNode_t;

struct sprHandle_t;

enum {
    FILE_CONFIG_SET,
    FILE_CONFIG_FRAME,
    FILE_CONFIG_X,
    FILE_CONFIG_Y
};

struct sdfMemHandle_t;

typedef struct fileLoadMirror_t {
    u32 current;
    u32 previous;
} fileLoadMirror_t;

extern u32 gslErrorCode_slMain;
extern u32 gslDrawFlagFile_slMain;
extern u32 fileMemoryCardRequestIndex;
extern s32 fileResetSelection(void);
extern u32 D_003BC834;

/* Init record at fileCursorPulseState. */
typedef struct fileInit374E0_t {
    s8 mode[3];      /* 0x00 */
    u8 pad3;         /* 0x03 */
    s32 pos[3][2];   /* 0x04 */
    s32 counter[3];  /* 0x1C */
    u8 pad28[0xC];   /* 0x28 */
    s32 alpha[3];    /* 0x34 */
} fileInit374E0_t;

/* Object with loader sub-objects (+0x40...). */
typedef struct fileLoadObj_t {
    void *owner;        /* 0x00 */
    u32 color;          /* 0x04 */
    f32 scale;          /* 0x08 */
    u32 selector;       /* 0x0C: secondary job buffer operation */
    u8 pad10[0x24];
    void *deviceHandle; /* 0x34 */
    fileJobPayload_t **jobs; /* 0x38: child file-job payloads */
    struct sdfMemHandle_t *jobAllocation;          /* 0x3C */
    struct ctexlHandle_t *referenceHolder; /* 0x40 */
    bhvrHandle_t *recordWork; /* 0x44: created by fileAllocateGridRecordSlots */
    s16 unk48;          /* 0x48 */
    u16 unk4A;
} fileLoadObj_t;

#define FILE_MAIN_BLOB_SIZE 0x33600

u8 slGetPadTrig(s32 condition);
void mcdCreateFontDrawHandle(s32 x, s32 y, u32 color, const char *text);

static inline void fileDrawDialogLine(s32 line, const char *glyphSource) {
    mcdCreateFontDrawHandle(86, 114 + line * 24, 0x89FEFF80U, glyphSource);
}

void fileClearAllSlotFlags(void);
s32 fileResetSelection(void);
s32 slExitToTitleSub(void);
s32 fileRestartSlotScan(void);
extern s32 D_003BC830;
extern s32 D_003BC838;
extern FileSelectionCallback D_003BD908, D_003BD90C;

/* Yes/no cursor input while the card request is idle. */
static inline void filePollPromptChoice(void) {
    if (D_003BC838 == 0 && slChkWindowOpenDone() != 0) {
        if (slGetPadTrig(dds3PadButtonState[0x27] < 0) != 0) {
            if (D_003BC830 != 1) {
                p2sdPlaySE(0, 0x7F, 0x3F);
            }
            D_003BC830 = 1;
        } else if (slGetPadTrig(dds3PadButtonState[0x26] < 0) != 0) {
            if (D_003BC830 != 0) {
                p2sdPlaySE(0, 0x7F, 0x3F);
            }
            D_003BC830 = 0;
        } else if (slGetPadTrig(dds3PadButtonState[0x21] < 0) != 0) {
            if (D_003BC830 == 0 && (D_003BC834 == 1 || D_003BC834 == 3)) {
                p2sdPlaySE(0x11, 0x7F, 0x3F);
            } else {
                p2sdPlaySE(8, 0x7F, 0x3F);
            }
            if (D_003BC830 != 0) {
                D_003BC838 = 2;
            } else {
                D_003BC838 = 1;
            }
        } else if (slGetPadTrig(dds3PadButtonState[0x23] < 0) != 0) {
            p2sdPlaySE(0xA, 0x7F, 0x3F);
            D_003BC838 = 2;
        }
    }
}

/* Lower the selected low/alpha byte, retaining the native signed, unmasked bound. */

#define FILE_JOB_LOCATION_AS_JOB_RELATIVE_OFFSET(payload) ((payload).offset)

#define FILE_JOB_LOCATION_AS_LOCKED_ADDRESS_WORD(payload) ((payload).offset)

#define FILE_JOB_LOCATION_CLEAR(payload) ((payload).offset = 0)

extern fileJob_t *fileQueueFindBySector(plistHandle_t *queue, u32 sector);

/* Makes the first job chained to owner's sector the leader and rechains the rest to it. */
static inline void fileQueueRechainSectorFollowers(plistHandle_t *queue, fileJob_t *owner, fileJob_t *leader) {
    fileJob_t *next;

    leader->sector = 0;
    leader->flags &= ~FILE_JOB_FLAG_SECTOR_FOLLOWER;
    fileQueueLinkJobToSectorLeader(queue, leader, leader);
    next = fileQueueFindBySector(queue, owner->id);
    while (next != NULL) {
        fileQueueLinkJobToSectorLeader(queue, next, leader);
        next = fileQueueFindBySector(queue, owner->id);
    }
}

/* Sixteen-byte preamble of a saved file queue. */
typedef struct fileQueueImageHeader_t {
    s32 version;
    s32 unk4;
    s32 unk8;
    f32 unkC;
} fileQueueImageHeader_t;

fileJob_t *fileQueueFindBySector(plistHandle_t *queue, u32 sector);

/* Creates a load object sized by the record's cell count, capped at 0x12C. */
typedef struct fileCellGrid_t {
    u8 pad00[0x20];
    u32 cols;
    u32 rows;
    u8 pad28[0x90];
    u32 layers;
} fileCellGrid_t;

typedef struct fileWaveMotion_t {
    f32 axis[4];             /* 0x00: xyz only */
    f32 swayDirection[4];    /* 0x10: xyz only */
    f32 phase;
    f32 previousSine;
    f32 amplitude;
    f32 amplitudeStep;
    f32 phaseStep;
    f32 speed;
    f32 scaleMultiplier;
    f32 angle;
    f32 angleMultiplier;
} fileWaveMotion_t;            /* 0x44, ordinary word alignment */

typedef struct fileSlotMotion_t {
    f32 direction[4];     /* 0x00; this callback initializes xyz only */
    f32 speed;            /* 0x10 */
    f32 scaleMultiplier;  /* 0x14 */
    f32 angle;            /* 0x18 */
    f32 angleMultiplier;  /* 0x1C */
} fileSlotMotion_t;

typedef struct fileCircularMotion_t {
    f32 origin[4];          /* 0x00: xyz only */
    f32 orientation[4];     /* 0x10: saved emitter quaternion */
    f32 height;             /* 0x20 */
    f32 radius;
    f32 radiusStep;
    f32 initialPhase;
    f32 angularSpeed;
    f32 axialSpeed;
    f32 scaleMultiplier;
    f32 angle;
    f32 angleMultiplier;
} fileCircularMotion_t;       /* 0x44, ordinary word alignment */

typedef struct fileOrientedRingMotion_t {
    f32 origin[4];          /* 0x00: xyz only */
    f32 orientationAngle;   /* 0x10 */
    f32 tiltAngle;
    f32 radius;
    f32 radiusStep;
    f32 angularSpeed;
    f32 initialPhase;
    f32 height;
    f32 scaleMultiplier;
    f32 angle;
    f32 angleMultiplier;
} fileOrientedRingMotion_t;   /* 0x38, ordinary word alignment */

INCLUDE_ASM(const s32, "mc/slMain", slKanjiDraw3);

#ifdef VERSION_RAIDOU1
void slKanjiDrawH(s32 x, s32 y, u32 rgba, const char *moji) {
    frEnableMode(1);
    slKanjiDrawH_handle2 = frSetFontString((u8 *)moji, 0, 0, 0, NULL);
    frSetFontColorNo(slKanjiDrawH_handle2, 3);
    frDisableMode(1);
    frSetFontSpace(slKanjiDrawH_handle2, -1);
    frSetDrawPosition(slKanjiDrawH_handle2, x << 4, y << 3);
    frDrawFontOt(slKanjiDrawH_handle2, 0, 0x6A);
    frDestroyFontRequestAfter(slKanjiDrawH_handle2);
    frSetFontDrawOT(0x68);
}
#else
INCLUDE_ASM(const s32, "mc/slMain", slKanjiDrawH);

INCLUDE_ASM(const s32, "mc/slMain", slCallEnableMode);
#endif

INCLUDE_ASM(const s32, "mc/slMain", slCallEnableMode2);

INCLUDE_ASM(const s32, "mc/slMain", slNextPageDraw);

INCLUDE_ASM(const s32, "mc/slMain", slMcAccessDraw);

INCLUDE_ASM(const s32, "mc/slMain", slQueryYesNoProcSub);

INCLUDE_ASM(const s32, "mc/slMain", slQueryYesNoProcSub2);

/* Raidou 2 inserts one OT layer below these. */
#ifdef VERSION_RAIDOU1
#define SL_WINDOW_OT 0x6A
#define SL_WINDOW_RESTORE_OT 0x67
#else
#define SL_WINDOW_OT 0x6B
#define SL_WINDOW_RESTORE_OT 0x68
#endif

s32 slDrawWindow(void) {
    s32 y;
    s32 h;
    s32 alpha;

    eglSetOt(SL_WINDOW_OT);
    eglLinkAlpha(0);
    eglLinkTest(1, 0, 0x80, 3, 0, 0, 1, 1);
    alpha = 100;
    y = 80;
    gSlWindowCnt++;
    if (gSlWindowCnt < 8) {
        y = 224 - gSlWindowCnt * 18;
        h = gSlWindowCnt * 36;
    } else {
        h = 288;
    }
    if (gSlWindowCnt < 8) {
        alpha = gSlWindowCnt * 100 / 8;
    }
    evtSubmitDefaultDepthGradientRect(0, y, 0x200, h, ((alpha * 80 / 100) << 24) | 0x1C1C28,
                                      ((alpha * 110 / 100) << 24) | 0xB0E19, ((alpha * 140 / 100) << 24) | 0x3030A,
                                      ((alpha * 110 / 100) << 24) | 0xB0E19);
    eglSetOt(SL_WINDOW_RESTORE_OT);
    if (gSlWindowCnt >= 14) {
        return 1;
    }
    return 0;
}

#ifdef VERSION_RAIDOU1
extern char slInsertedInMEMORYData[], slInsertedInMEMORYData2[], slInsertedInMEMORYData3[];
extern char slInsertedInMEMORYData4[], slInsertedInMEMORYData5[], slInsertedInMEMORYData6[];
extern char slInsertedInMEMORYData7[], slInsertedInMEMORYData8[], slInsertedInMEMORYData9[];
extern char slInsertedInMEMORYData10[], slInsertedInMEMORYData11[], slInsertedInMEMORYData12[];
extern char slInsertedInMEMORYData13[], slInsertedInMEMORYData14[], slInsertedInMEMORYData15[];
extern char slInsertedInMEMORYData16[], slInsertedInMEMORYData17[], slInsertedInMEMORYData18[];
extern char slInsertedInMEMORYData19[], slInsertedInMEMORYData20[], slInsertedInMEMORYData21[];
extern char slInsertedInMEMORYData22[], slInsertedInMEMORYData23[], slInsertedInMEMORYData24[];
extern char slInsertedInMEMORYData25[];

void slDrawError(void) {
    if (gslErrorCode == 0) {
        return;
    }
    if (gslErrorCode == 1) {
        slFileSelError = 0;
        slData2 = 0;
        fileResetLoadContextSlide();
        if (slDrawWindow() != 0) {
            slKanjiDrawH(46, 114, 0x64E6AA80, slInsertedInMEMORYData);
            slKanjiDrawH(46, 138, 0x64E6AA80, slInsertedInMEMORYData2);
            slKanjiDrawH(46, 162, 0x64E6AA80, slInsertedInMEMORYData3);
            slKanjiDrawH(46, 186, 0x64E6AA80, slInsertedInMEMORYData4);
            slKanjiDrawH(46, 210, 0x64E6AA80, slInsertedInMEMORYData5);
            slNextPageDraw();
            gslDrawFlagFile = 0;
        }
    }
    if (gslErrorCode == 2) {
        if (slDrawWindow() != 0) {
            slKanjiDrawH(70, 114, 0x64E6AA80, slInsertedInMEMORYData6);
            slKanjiDrawH(70, 138, 0x64E6AA80, slInsertedInMEMORYData7);
            slKanjiDrawH(70, 162, 0x64E6AA80, slInsertedInMEMORYData8);
            slKanjiDrawH(70, 186, 0x64E6AA80, slInsertedInMEMORYData9);
            slKanjiDrawH(70, 210, 0x64E6AA80, slInsertedInMEMORYData10);
            slNextPageDraw();
            gslDrawFlagFile = 0;
        }
    }
    if (gslErrorCode == 3) {
        if (slDrawWindow() != 0) {
            slKanjiDrawH(70, 114, 0x64E6AA80, slInsertedInMEMORYData11);
            slKanjiDrawH(70, 138, 0x64E6AA80, slInsertedInMEMORYData23);
            slKanjiDrawH(70, 162, 0x64E6AA80, slInsertedInMEMORYData24);
            slKanjiDrawH(70, 186, 0x64E6AA80, slInsertedInMEMORYData25);
            slNextPageDraw();
            gslDrawFlagFile = 0;
        }
    }
    if (gslErrorCode == 4) {
        if (slDrawWindow() != 0) {
            slKanjiDrawH(70, 114, 0x64E6AA80, slInsertedInMEMORYData12);
            slKanjiDrawH(70, 138, 0x64E6AA80, slInsertedInMEMORYData23);
            slKanjiDrawH(70, 162, 0x64E6AA80, slInsertedInMEMORYData24);
            slKanjiDrawH(70, 186, 0x64E6AA80, slInsertedInMEMORYData25);
            slNextPageDraw();
            gslDrawFlagFile = 0;
        }
    }
    if (gslErrorCode == 5) {
        if (slDrawWindow() != 0) {
            slKanjiDrawH(70, 114, 0x64E6AA80, slInsertedInMEMORYData14);
            slKanjiDrawH(70, 138, 0x64E6AA80, slInsertedInMEMORYData15);
            slKanjiDrawH(70, 162, 0x64E6AA80, slInsertedInMEMORYData16);
            slKanjiDrawH(70, 186, 0x64E6AA80, slInsertedInMEMORYData17);
            slKanjiDrawH(70, 210, 0x64E6AA80, slInsertedInMEMORYData18);
            slNextPageDraw();
            gslDrawFlagFile = 0;
        }
    }
    if (gslErrorCode == 6) {
        if (slDrawWindow() != 0) {
            slKanjiDrawH(70, 114, 0x64E6AA80, slInsertedInMEMORYData13);
            slKanjiDrawH(70, 138, 0x64E6AA80, slInsertedInMEMORYData23);
            slKanjiDrawH(70, 162, 0x64E6AA80, slInsertedInMEMORYData24);
            slKanjiDrawH(70, 186, 0x64E6AA80, slInsertedInMEMORYData25);
            slNextPageDraw();
            gslDrawFlagFile = 0;
        }
    }
    if (gslErrorCode == 7) {
        if (slDrawWindow() != 0) {
            slKanjiDrawH(70, 114, 0x64E6AA80, slInsertedInMEMORYData19);
            slKanjiDrawH(70, 138, 0x64E6AA80, slInsertedInMEMORYData20);
            slKanjiDrawH(70, 162, 0x64E6AA80, slInsertedInMEMORYData21);
            slKanjiDrawH(70, 186, 0x64E6AA80, slInsertedInMEMORYData22);
            slNextPageDraw();
            gslDrawFlagFile = 0;
        }
    }
}
#else
INCLUDE_ASM(const s32, "mc/slMain", slDrawError);
#endif

void slSetInfo(u32 id) {
    s32 previous;

    previous = gslInfoCode;
    gslInfoCode = id;
    if (previous == 0) {
        gSlWindowCnt = 0;
    }
}

#ifdef VERSION_RAIDOU1
extern char slDrawInfoData[], slDrawInfoData2[], slDrawInfoData3[], slDrawInfoData4[];
extern char slDrawInfoData5[], slDrawInfoData6[], slDrawInfoData7[], slDrawInfoData8[];
extern char slDrawInfoData9[], slDrawInfoData10[], slDrawInfoData11[], slDrawInfoData12[];
extern char slDrawInfoData13[], slDrawInfoData14[], slDrawInfoData17[];

void slDrawInfo(void) {
    if (gslInfoCode == 0) {
        return;
    }
    if (gslInfoCode == 1) {
        if (slDrawWindow() != 0) {
            slKanjiDrawH(70, 114, 0x64E6AA80, slDrawInfoData);
            slKanjiDrawH(70, 138, 0x64E6AA80, slDrawInfoData2);
            slKanjiDrawH(70, 162, 0x64E6AA80, slDrawInfoData3);
            slMcAccessDraw();
        }
    }
    if (gslInfoCode == 2) {
        if (slDrawWindow() != 0) {
            slKanjiDrawH(46, 114, 0x64E6AA80, slDrawInfoData5);
            slKanjiDrawH(46, 138, 0x64E6AA80, slDrawInfoData7);
            slKanjiDrawH(46, 162, 0x64E6AA80, slDrawInfoData8);
            slKanjiDrawH(46, 186, 0x64E6AA80, slDrawInfoData9);
            slMcAccessDraw();
        }
    }
    if (gslInfoCode == 3) {
        if (slDrawWindow() != 0) {
            slKanjiDrawH(70, 114, 0x64E6AA80, slDrawInfoData4);
            slKanjiDrawH(70, 138, 0x64E6AA80, slDrawInfoData7);
            slKanjiDrawH(70, 162, 0x64E6AA80, slDrawInfoData8);
            slKanjiDrawH(70, 186, 0x64E6AA80, slDrawInfoData9);
            slMcAccessDraw();
        }
    }
    if (gslInfoCode == 4) {
        if (slDrawWindow() != 0) {
            slKanjiDrawH(70, 114, 0x64E6AA80, slDrawInfoData11);
            slNextPageDraw();
        }
        gSlExitMode = 1;
    }
    if (gslInfoCode == 5) {
        if (slDrawWindow() != 0) {
            slKanjiDrawH(70, 114, 0x64E6AA80, slDrawInfoData6);
            slKanjiDrawH(70, 138, 0x64E6AA80, slDrawInfoData7);
            slKanjiDrawH(70, 162, 0x64E6AA80, slDrawInfoData8);
            slKanjiDrawH(70, 186, 0x64E6AA80, slDrawInfoData9);
            slMcAccessDraw();
        }
    }
    if (gslInfoCode == 6) {
        if (slDrawWindow() != 0) {
            slKanjiDrawH(70, 114, 0x64E6AA80, slDrawInfoData12);
            slNextPageDraw();
        }
    }
    if (gslInfoCode == 13) {
        if (slDrawWindow() != 0) {
            slKanjiDrawH(70, 114, 0x64E6AA80, slDrawInfoData10);
            slNextPageDraw();
        }
        gSlExitMode = 2;
    }
    if (gslInfoCode == 30) {
        if (slDrawWindow() != 0) {
            slKanjiDrawH(70, 114, 0x64E6AA80, slDrawInfoData13);
            slKanjiDrawH(70, 138, 0x64E6AA80, slDrawInfoData14);
            slKanjiDrawH(70, 162, 0x64E6AA80, slDrawInfoData17);
            slNextPageDraw();
        }
        gSlExitMode = 2;
    }
}
#else
INCLUDE_ASM(const s32, "mc/slMain", slDrawInfo);
#endif

void fileClearAllSlotFlags(void) {
    sdfMcSetCardStat(fileMemoryCardRequestIndex);
    sdfMcClearSlotFlags(fileMemoryCardRequestIndex, 0);
    sdfMcClearSlotFlags(fileMemoryCardRequestIndex, 1);
    sdfMcClearSlotFlags(fileMemoryCardRequestIndex, 2);
    sdfMcClearSlotFlags(fileMemoryCardRequestIndex, 3);
    sdfMcClearSlotFlags(fileMemoryCardRequestIndex, 4);
    sdfMcClearSlotFlags(fileMemoryCardRequestIndex, 5);
    sdfMcClearSlotFlags(fileMemoryCardRequestIndex, 6);
    sdfMcClearSlotFlags(fileMemoryCardRequestIndex, 7);
    sdfMcClearSlotFlags(fileMemoryCardRequestIndex, 8);
    sdfMcClearSlotFlags(fileMemoryCardRequestIndex, 9);
}

saddr_t slHeadLoad(void) {
    char path[0x50];
    u32 ctx;
    s32 slot;
    s32 len;

    sdfMcSetSlotIndex(mccb, slChooseSaveIndex);
    ctx = mccb;
    slot = sdfMcGetSlotIndex(ctx);
    if ((sdfMcGetSlotFlags(ctx, slot) & SL_SLOT_FLAG_CAN_READ_SAVE) == 0) {
        return slChooseData_getCardInfoNext();
    }
    path[0] = 0x2F;
    mcMakeFileName(&path[1], slot);
    len = strlen(&path[1]);
    path[len + 1] = 0x2F;
    memcpy(&path[len + 2], &path[1], len);
    path[len * 2 + 2] = 0;
    sdfMcFuncOpen(ctx, path, 1);
    return EE_ADDR_S(slHeadLoadProc_openFile);
}

saddr_t slHeadLoadProc_openFile(void) {
    McPollResult status = sdfMcFuncOpen1(&slMcFd);

    if (status == MC_POLL_PENDING) {
        return 0;
    }
    if (status == MC_POLL_SUCCESS) {
        slLoadFileFd = sdfMemAllocDirect(0x1270);
        slLoadFileHandle = sdfMemLock(slLoadFileFd);
        sdfMcFuncRead(slMcFd, slLoadFileHandle, 0x1270);
        return EE_ADDR_S(slHeadLoadProc_readFile);
    }
    return slNoCard();
}

saddr_t slHeadLoadProc_readFile(void) {
    McPollResult status = sdfMcFuncRead1();
    if (status == MC_POLL_PENDING) {
        return 0;
    }
    if (status == MC_POLL_SUCCESS) {
        sdfMcFuncClose(slMcFd);
        return EE_ADDR_S(slHeadLoadProc_closeFile);
    }
    sdfMemFreeDirect(slLoadFileFd);
    return slNoCard();
}

INCLUDE_ASM(const s32, "mc/slMain", slCallMcFuncClose1);

INCLUDE_ASM(const s32, "mc/slMain", slExitWait);

s32 slEndingTop(void) {
    gslMode = 0;
    slSetInfo(0);
    gslErrorCode = 0;
    slQueryYesNoMsg = 4;
    gslDrawFlagFile = 0;
    return slQueryYesNo(&slTop, &slEndingExit, 0);
}

s32 slEndingUpd2(void) {
    slSetInfo(0);
    gslErrorCode = 0;
    slQueryYesNoMsg = 9;
    return slQueryYesNo(&slSaveData, &slFileSel, 1);
}

s32 slEndingExit(void) {
    slSetInfo(0);
    gslErrorCode = 0;
    slQueryYesNoMsg = 8;
    gslDrawFlagFile = 0;
    return slCallMcRegetInfo((s32 (*)(void))&slDefaultFileFlowProc1st, &slEndingTop, 1);
}

saddr_t slBootTop(void) {
    slSetInfo(0);
    gslDrawFlagFile = 0;
    return EE_ADDR_S(slBootProc_CheckCard);
}

saddr_t slBootRetry(void) {
    sdfMcRegetInfo(0);
    gslCheckCardWait = SDF_FRAMES(60);
    gSlNowCardCheck = 1;
    slSetInfo(1);
    gslDrawFlagBg = 1;
    gslDrawFlagFile = 1;
    return EE_ADDR_S(slBootProc_CheckCardR);
}

s32 slBootError(void) {
    gslDrawFlagBg = 1;
    slSetInfo(0);
    slQueryYesNoMsg = 5;
    return slQueryYesNo(&slExitToTitle, &slBootRetry, 1);
}

s32 slBootError2(void) {
    gslDrawFlagBg = 1;
    slSetInfo(0);
    slQueryYesNoMsg = 6;
    return slQueryYesNo(&slExitToTitle, &slBootRetry, 1);
}

INCLUDE_ASM(const s32, "mc/slMain", slBootRetry_slMain);

INCLUDE_ASM(const s32, "mc/slMain", slTop);

s32 slExit(void) {
    slSetInfo(0);
    gslDrawFlagFile = 0;
    gslDrawFlagBg = 0;
    return 0xffffffff;
}

INCLUDE_ASM(const s32, "mc/slMain", slCallSetInfo2);

#ifdef VERSION_RAIDOU1
s32 slExitToTitleSub(void) {
    u32 mode[4];

    mode[0] = 0;
    dds3AdminiCallSequence(2, mode, 4, 0);
    return 0;
}
#else
INCLUDE_ASM(const s32, "mc/slMain", slExitToTitleSub);
#endif

s32 slExitToTitle(void) {
    return slExitToTitleSub();
}

#ifdef VERSION_RAIDOU1
s32 slCallAdminiCallSequence(void) {
    u32 v = 2;

    slData3 = 1;
    dds3AdminiCallSequence(2, &v, 4, 0);
    return 0;
}
#else
INCLUDE_ASM(const s32, "mc/slMain", slCallAdminiCallSequence);
#endif

/* body: matched in store @ 2026-11-16: /home/wsl/.local/state/raidou-decomp-work/clusters/store/raidou1/001D65F8.c:1, match */
dds3ProcessFunc_t slDefaultFileFlowProc1st(void) {
    return EE_ADDR_AS(dds3ProcessFunc_t, slExitToTitleSub());
}

s32 fileRestartSlotScan(void) {
    fileClearAllSlotFlags();
    slSetInfo(1);
    gslErrorCode_slMain = 0;
    gslDrawFlagFile_slMain = 0;
    return fileResetSelection();
}

INCLUDE_ASM(const s32, "mc/slMain", slFileSel);

INCLUDE_ASM(const s32, "mc/slMain", slCancel);

INCLUDE_ASM(const s32, "mc/slMain", slSaveProc1st);

INCLUDE_ASM(const s32, "mc/slMain", slCallSetInfo3);

INCLUDE_ASM(const s32, "mc/slMain", slRefSystem);

/* body: matched in store @ 2026-11-16: /home/wsl/.local/state/raidou-decomp-work/clusters/store/raidou1/001D6808.c:1, match */
void slLeaf5(void) {
    slTop();
}

INCLUDE_ASM(const s32, "mc/slMain", slLeaf6);

s32 slCallSetInfo4(void) {
    slSetInfo(0);
    gslDrawFlagFile_slMain = 0;
    slQueryYesNoMsg = 12;
    return slCallMcRegetInfo(&slCallSetInfo5, &slLeaf7, 1);
}

INCLUDE_ASM(const s32, "mc/slMain", slCallSetInfo5);

INCLUDE_ASM(const s32, "mc/slMain", slLeaf7);

s32 slCallSetInfo6(void) {
    slSetInfo(0);
    gslErrorCode = 0;
    slQueryYesNoMsg = 12;
    gslDrawFlagFile = 0;
    return slCallMcRegetInfo(&slCallSetInfo5, &slLeaf7, 1);
}

dds3ProcessFunc_t slLoadProc1st(void)
{
    return EE_ADDR_AS(dds3ProcessFunc_t, slTop());
}

INCLUDE_ASM(const s32, "mc/slMain", slLeaf9);

s32 slEndingProc1st(void) {
    slBaseIconMemHandle = NULL;
    baseIconFileHandle = EE_ADDR(fileLoad(slIconPath));
    return slEndingTop();
}

s32 slBootProc1st(void) {
    sdfMcRegetInfo(0);
    gSlNowCardCheck = 1;
    gslCheckCardWait = 0;
    slSetInfo(0);
    gslDrawFlagFile = 0;
    return slBootTop();
}

saddr_t slCallSetInfo7(void) {
    slSetInfo(0);
    slCallPlaySE2Data = 0;
    gslDrawFlagFile_slMain = 0;
    return EE_ADDR_S(slCallPlaySE2);
}

INCLUDE_ASM(const s32, "mc/slMain", slCallPlaySE2);

INCLUDE_ASM(const s32, "mc/slMain", slCallBitOn);

INCLUDE_ASM(const s32, "mc/slMain", slHardSelect2N);

INCLUDE_ASM(const s32, "mc/slMain", slHardSelect3);

INCLUDE_ASM(const s32, "mc/slMain", slHardSelect4);

INCLUDE_ASM(const s32, "mc/slMain", slSaveProc_CheckCard);

INCLUDE_ASM(const s32, "mc/slMain", sl1Ken);

#ifdef VERSION_RAIDOU1
#define SL_SAVE_DATA_SIZE 0x19000
#else
#define SL_SAVE_DATA_SIZE 0x1E400
#endif

s32 slSizeChk(void) {
    return sdfMcGetFreeBytes(mccb) >= SL_SAVE_DATA_SIZE;
}

s32 slBootProc_CheckCard(void) {
    if (sdfMcFuncGetInfo1() == 0) {
        return 0;
    }
    if (gslCheckCardWait > 0) {
        gslCheckCardWait--;
        sdfMcRegetInfo(mccb);
        return 0;
    }
    switch (sdfMcGetFileInfoReady(mccb)) {
    case 0:
    case 3:
        return slBootError();
    case 2:
        return slExitToTitle();
    case 1:
        if (slSizeChk() != 0) {
            return slExitToTitle();
        }
        return slBootRetry();
    default:
        return 0;
    }
}

INCLUDE_ASM(const s32, "mc/slMain", slBootProc_CheckCardR);

INCLUDE_ASM(const s32, "mc/slMain", slErrorWaitMc);

s32 slErrorWaitToFileMc(void) {
    if (gslErrorKeyFlag == 0) {
        if (slGetPadTrig(dds3PadButtonState[0x23] < 0) ||
            slGetPadTrig(dds3PadButtonState[0x21] < 0)) {
            p2sdPlaySE(0, 0x7F, 0x3F);
            gslErrorKeyFlag = 1;
        }
    }
    if (sdfMcFuncGetInfo1() != 0) {
        if (gslErrorKeyFlag == 1) {
            gslErrorCode = 0;
            return slFileSel();
        }
        if (sdfMcGetFileInfoReady(mccb) != 1) {
            gslErrorCode = 0;
            gslDrawFlagFile = 0;
            return slTop();
        }
        sdfMcRegetInfo(mccb);
    }
    return 0;
}

INCLUDE_ASM(const s32, "mc/slMain", slCallPlaySE3);

s32 slExitWaitProc(void) {
    s32 remaining = slExitWaitCnt - 1;
    slExitWaitCnt = remaining;
    if (remaining <= 0) {
        if (slHitAnyKeyNextFunc == -1) {
            return -1;
        }
        return ((s32 (*)())slExitWaitNextFunc)();
    }
    return 0;
}

INCLUDE_ASM(const s32, "mc/slMain", slLeaf10);

INCLUDE_ASM(const s32, "mc/slMain", slHitAnyKeyNoChkProc);

saddr_t slHitAnyKey(addr_t nextFunc) {
    slHitAnyKeyNextFunc = nextFunc;
    gslErrorKeyFlag = 0;
    sdfMcRegetInfo(mccb);
    return EE_ADDR_S(slHitAnyKeyProc);
}

INCLUDE_ASM(const s32, "mc/slMain", slHitAnyKeyProc);

/* body: matched in smt3 @ 9401f79e: src/smt3/mc/slMain.c:1818, near, same function 1.00 (words 15/15, calls 1.00, shape 1.00) */
saddr_t slChooseData_getCardInfo_B(void) {
    u8 buf[0x50];

    buf[0] = 0x2F;
    mcMakeFileName(&buf[1], slChooseSaveIndex);
    sdfMcFuncChdir(mccb, (const char *)buf);
    return EE_ADDR_S(slChooseData_getCardInfo2);
}

saddr_t slChooseData_getCardInfo2(void) {
    McPollResult t = sdfMcFuncChdir1();

    if (t == MC_POLL_PENDING) {
        return 0;
    }
    if (t == MC_POLL_SUCCESS) {
        sdfMcSetSlotFlags(mccb, slChooseSaveIndex, SL_SLOT_FLAG_DIRECTORY_READY);
        sdfMcFuncGetDir(mccb, slIconSysName, slDirEntries, 1);
        return EE_ADDR_S(slChooseData_getCardInfo3);
    }
    if (t == MC_POLL_ERROR) {
        return slNoCard();
    }
    return slHeadLoad();
}

s32 slChooseData_getCardInfo3(void) {
    s32 value;
    McPollResult status = sdfMcFuncGetDir1(&value);
    if (status == MC_POLL_PENDING) {
        return 0;
    }
    if (status == MC_POLL_SUCCESS) {
        if (value == 1) {
            sdfMcSetSlotFlags(mccb, slChooseSaveIndex,
                                SL_SLOT_FLAG_CAN_READ_SAVE | SL_SLOT_FLAG_DELETE_BEFORE_WRITE);
        }
        return slHeadLoad();
    }
    if (status == MC_POLL_ERROR) {
        return slNoCard();
    }
    return slHeadLoad();
}

INCLUDE_ASM(const s32, "mc/slMain", slCallMcClearCardStat);

INCLUDE_ASM(const s32, "mc/slMain", slChooseData);

/* body: matched in smt3 @ 9401f79e: src/smt3/mc/slMain.c:1724, near, same function 1.00 (words 15/15, calls 1.00, shape 1.00) */
saddr_t slChooseData_getCardInfo(void) {
    u8 buf[0x50];

    buf[0] = 0x2F;
    mcMakeFileName(&buf[1], slChooseSaveIndex);
    sdfMcFuncChdir(mccb, (const char *)buf);
    return EE_ADDR_S(slChooseData_getCardInfo2_B);
}

saddr_t slChooseData_getCardInfo2_B(void) {
    McPollResult t = sdfMcFuncChdir1();

    if (t == MC_POLL_PENDING) {
        return 0;
    }
    if (t == MC_POLL_SUCCESS) {
        sdfMcSetSlotFlags(mccb, slChooseSaveIndex, SL_SLOT_FLAG_DIRECTORY_READY);
        sdfMcFuncGetDir(mccb, slIconSysName, slDirEntries, 1);
        return EE_ADDR_S(slChooseData_getCardInfo3_B);
    }
    if (t == MC_POLL_ERROR) {
        slSetInfo(0);
        gslDrawFlagFile = 0;
        gslErrorCode = 0;
        return slBootError();
    }
    return slChooseData_getCardInfoNext_B();
}

s32 slChooseData_getCardInfo3_B(void) {
    s32 value;
    McPollResult status = sdfMcFuncGetDir1(&value);
    if (status == MC_POLL_PENDING) {
        return 0;
    }
    if (status == MC_POLL_SUCCESS) {
        if (value == 1) {
            sdfMcSetSlotFlags(mccb, slChooseSaveIndex,
                                SL_SLOT_FLAG_CAN_READ_SAVE | SL_SLOT_FLAG_DELETE_BEFORE_WRITE);
        }
        return slChooseData_getCardInfoNext_B();
    }
    if (status == MC_POLL_ERROR) {
        slSetInfo(0);
        gslDrawFlagFile = 0;
        gslErrorCode = 0;
        return slBootError();
    }
    return slChooseData_getCardInfoNext_B();
}

/* body: matched in smt3 @ 9401f79e: src/smt3/mc/slMain.c:1869, near, same function 1.00 (words 40/40, calls 1.00, shape 1.00) */
s32 slChooseData_getCardInfoNext_B(void) {
    s32 slot = slChooseSaveIndex;
    s32 buttons = sdfMcGetSlotFlags(mccb, slot);

    slot++;
    if (buttons & SL_SLOT_FLAG_CAN_READ_SAVE) {
        if (buttons & SL_SLOT_FLAG_DIRECTORY_READY) {
            if (buttons & SL_SLOT_FLAG_DELETE_BEFORE_WRITE) {
                slSetInfo(0);
                gslDrawFlagFile = 0;
                gslErrorCode = 0;
                return slExitToTitle();
            }
        }
    }
    slChooseSaveIndex = slot;
    if (slot == 10) {
        slSetInfo(0);
        gslDrawFlagFile = 0;
        gslErrorCode = 0;
        return slBootError2();
    }
    return slChooseData_getCardInfo();
}

INCLUDE_ASM(const s32, "mc/slMain", slChooseData_B);

saddr_t slSaveData(void) {
    u8 name[0x50];
    u32 entry = mccb;
    s32 slot = sdfMcGetSlotIndex(entry);
    sdfMcGetSlotFlags(entry, slot);
    slSetInfo(2);
    sdfMcSetCardStat(entry);
    if (sdfMcGetSlotFlags(entry, slot) & SL_SLOT_FLAG_DIRECTORY_READY) {
        return slSaveData_chdir();
    }
    name[0] = '/';
    mcMakeFileName(name + 1, slot);
    sdfMcFuncMkdir(entry, (const char *)name);
    return EE_ADDR_S(slSaveProc_Write_mkdir);
}

saddr_t slSaveData_chdir(void) {
    u8 buf[0x50];
    u32 entry = mccb;
    s32 slot = sdfMcGetSlotIndex(entry);

    buf[0] = 0x2F;
    mcMakeFileName(&buf[1], slot);
    sdfMcFuncChdir(entry, (const char *)buf);
    return EE_ADDR_S(slSaveProc_Write_chdir);
}

saddr_t slSaveProc_Write_mkdir(void) {
    McPollResult status = sdfMcFuncMkdir1();
    if (status == MC_POLL_PENDING) {
        return 0;
    }
    if (status == MC_POLL_SUCCESS) {
        sdfMcSetSlotFlags(mccb, slFileSelError, SL_SLOT_FLAG_DIRECTORY_READY);
        return slSaveData_chdir();
    }
    slSetInfo(0);
    gslErrorCode = 4;
    return EE_ADDR_S(slErrorWait);
}

saddr_t slCallMcFuncChdir1(void) {
    McPollResult t = sdfMcFuncChdir1();

    if (t == MC_POLL_PENDING) {
        return 0;
    }
    if (t == MC_POLL_SUCCESS) {
        return slSaveData_deleteIconSys();
    }
    if (t == MC_POLL_ERROR) {
        slSetInfo(0);
        gslErrorCode = 4;
        return EE_ADDR_S(slErrorWait);
    }
    return 0;
}

INCLUDE_ASM(const s32, "mc/slMain", slLoadData_slMain);

s32 slSaveProc_doNothing(void) {
    sdfMcSetSlotFlags(mccb, slFileSelError, SL_SLOT_FLAG_CAN_READ_SAVE);
    sdfMcSetSlotFlags(mccb, slFileSelError, SL_SLOT_FLAG_DELETE_BEFORE_WRITE);
    p2sdPlaySE(26, 127, 63);
    slSetInfo(4);
    return slHitAnyKey(EE_ADDR(slFileSel));
}

INCLUDE_ASM(const s32, "mc/slMain", slVersionConvert_Step1);

INCLUDE_ASM(const s32, "mc/slMain", slSaveWriteEnd_baseIcon_slMain);

s32 slSaveWriteEnd_saveData(void) {
    return slSaveWriteFile(slBaseIconName, &slBaseIconBuff, &slBaseIconBuffSize,
                          slSaveWriteEnd_baseIcon, &baseIconFileHandle);
}

INCLUDE_ASM(const s32, "mc/slMain", slSystemDataProc);

saddr_t slSaveData_deleteIconSys(void) {
    u32 entry = mccb;
    s32 slot = sdfMcGetSlotIndex(entry);
    u32 flags = sdfMcGetSlotFlags(entry, slot);
    if (!(flags & SL_SLOT_FLAG_DELETE_BEFORE_WRITE)) {
        return slSaveData_writeFile();
    }
    sdfMcFuncDelete(entry, slIconSysName);
    return EE_ADDR_S(slSaveProc_Write_deleteIconSys);
}

saddr_t slSaveWriteFile(const char *filename, void **buff, u32 *size, void *endFunc, u32 *srcPtr) {
    slSaveWriteFile_buff = buff;
    slSaveWriteFile_size = size;
    slSaveWriteFile_endFunc = endFunc;
    slSaveWriteFile_dataWaitPtr = srcPtr;
    sdfMcFuncOpen(mccb, filename, 0x203);
    return EE_ADDR_S(slSaveProc_openFile);
}

saddr_t slSaveProc_openFile(void) {
    McPollResult status = sdfMcFuncOpen1(&slMcFd);

    if (status == MC_POLL_PENDING) {
        return 0;
    }
    if (status == MC_POLL_SUCCESS) {
        if (slSaveWriteFile_dataWaitPtr != NULL && *slSaveWriteFile_dataWaitPtr != 0) {
            return EE_ADDR_S(slSaveProc_waitDataReady);
        }
        sdfMcFuncWrite(slMcFd, *slSaveWriteFile_buff, *slSaveWriteFile_size);
        return EE_ADDR_S(slSaveProc_writeFile);
    }
    if (status == MC_POLL_ERROR) {
        slSetInfo(0);
        gslErrorCode = 4;
        return EE_ADDR_S(slErrorWait);
    }
    return 0;
}

saddr_t slSaveProc_writeFile(void) {
    McPollResult status = sdfMcFuncWrite1();
    if (status == MC_POLL_PENDING) {
        return 0;
    }
    if (status == MC_POLL_SUCCESS) {
        sdfMcFuncClose(slMcFd);
        return EE_ADDR_S(slSaveProc_closeFile);
    }
    if (status == MC_POLL_ERROR) {
        slSetInfo(0);
        gslErrorCode = 4;
        return EE_ADDR_S(slErrorWait);
    }
    return 0;
}

saddr_t slSaveProc_closeFile(void) {
    McPollResult status = sdfMcFuncClose1();
    if (status == MC_POLL_PENDING) {
        return 0;
    }
    if (status == MC_POLL_SUCCESS) {
        return ((s32 (*)())slSaveWriteFile_endFunc)();
    }
    if (status == MC_POLL_ERROR) {
        slSetInfo(0);
        gslErrorCode = 4;
        return EE_ADDR_S(slErrorWait);
    }
    return 0;
}

saddr_t slSaveProc_waitDataReady(void) {
    if (baseIconFileHandle != 0) {
        return 0;
    }
    sdfMcFuncWrite(slMcFd, *slSaveWriteFile_buff, *slSaveWriteFile_size);
    return EE_ADDR_S(slSaveProc_writeFile);
}

saddr_t slProc_Format(void) {
    McPollResult status = sdfMcFuncFormat1();
    if (status == MC_POLL_PENDING) {
        return 0;
    }
    dds3SoftResetOn();
    if (status < 0) {
        slSetInfo(0);
        gslErrorCode = 6;
        return EE_ADDR_S(slErrorWait);
    }
    slSetInfo(6);
    return slHitAnyKey(EE_ADDR(slCardChk));
}

saddr_t slFormatStart(void) {
    slSetInfo(5);
    dds3SoftResetOff();
    sdfMcFuncFormat(mccb);
    return EE_ADDR_S(slProc_Format);
}

INCLUDE_ASM(const s32, "mc/slMain", slFormatCancel);

/* body: matched in smt3 @ 9401f79e: src/smt3/mc/slMain.c:2175, adapt, same function 0.94 (words 59/57, calls 0.89, shape 0.97) */
saddr_t slLoadData(void) {
    char path[0x50];
    u32 ctx = mccb;
    s32 slot = sdfMcGetSlotIndex(ctx);
    u32 attr = sdfMcGetSlotFlags(ctx, slot);
    u32 len;

    slData5 = 1;
    slSetInfo(3);
    if ((attr & SL_SLOT_FLAG_CAN_READ_SAVE) == 0) {
        return slFileSel();
    }
    setCallMcGetSlotIndex();
    path[0] = '/';
    mcMakeFileName(&path[1], slot);
    len = strlen(&path[1]);
    path[len + 1] = '/';
    memcpy(&path[len + 2], &path[1], len);
    path[len * 2 + 2] = 0;
    sdfMcFuncOpen(ctx, path, 1);
    return EE_ADDR(slLoadProc_openFile);
}

/* NON_MATCHING: 40 of 41 words match.  */
#ifdef NON_MATCHING
saddr_t slLoadProc_openFile(void) {
    McPollResult status = sdfMcFuncOpen1(&slMcFd);
    u32 size;

    if (status == MC_POLL_PENDING) {
        return 0;
    }
    size = slVersionConvert_GetSize();
    slLoadFileFd = sdfMemAllocDirect(size);
    slLoadFileHandle = sdfMemLock(slLoadFileFd);
    if (status == MC_POLL_SUCCESS) {
        sdfMcFuncRead(slMcFd, slLoadFileHandle, size);
        return EE_ADDR_S(slLoadProc_readFile);
    }
    slSetInfo(0);
    gslErrorCode = 3;
    return EE_ADDR_S(slErrorWait);
}
#else
INCLUDE_ASM(const s32, "mc/slMain", slLoadProc_openFile);
#endif

saddr_t slLoadProc_readFile(void) {
    McPollResult status = sdfMcFuncRead1();
    if (status == MC_POLL_PENDING) {
        return 0;
    }
    if (status == MC_POLL_SUCCESS) {
        sdfMcFuncClose(slMcFd);
        return EE_ADDR_S(slLoadProc_closeFile);
    }
    sdfMemFreeDirect(slLoadFileFd);
    slSetInfo(0);
    gslErrorCode = 3;
    return EE_ADDR_S(slErrorWait);
}

INCLUDE_ASM(const s32, "mc/slMain", slCallMcFuncClose12);

INCLUDE_ASM(const s32, "mc/slMain", slCallPlaySE4);

INCLUDE_ASM(const s32, "mc/slMain", slFileSelCalc);

INCLUDE_ASM(const s32, "mc/slMain", slNewCycle);

s32 slCalc(dds3ProcessID_t *id) {
    s32 next;
    u32 job;
    s32 (*cur)(dds3ProcessID_t *);
    gslAnimCnt++;
    slDraw(EE_ADDR_S(id));
    next = proc_func_ans(id);
    if (next == -1) {
        return next;
    }
    job = baseIconFileHandle;
    cur = proc_func_ans;
    if (next != 0) {
        cur = EE_ADDR_AS(slProcFunc_t, next);
    }
    proc_func_ans = cur;
    if (job != 0 && pacFileCheckEnd(EE_ADDR_AS(fileRequest_t *, job)) != 0) {
        baseIconFileHandle = 0;
        slBaseIconMemHandle = fileGetMemHandle((fileRequest_t *)EE_ADDR_PTR(job));
        slBaseIconBuff = fileGetLoadPtr((fileRequest_t *)EE_ADDR_PTR(job));
        slBaseIconBuffSize = fileGetLoadSize((fileRequest_t *)EE_ADDR_PTR(job));
        fileHandleFree((fileRequest_t *)EE_ADDR_PTR(job));
    }
    return 0;
}

dds3ProcessFunc_t slDraw_slMain(dds3ProcessID_t *id) {
    if (0 < slData5 && slData5 < 600) {
        slData5++;
    }
#ifdef VERSION_RAIDOU1
    eglSetOt(102);
#else
    eglSetOt(103);
#endif
    eglLinkAlpha(0);
    eglLinkTest(1, 0, 0x80, 3, 0, 0, 1, 1);
    if (gslDrawFlagBg != 0) {
#ifdef VERSION_RAIDOU1
        rstCallCalcAlphaRate(fmShutDownData, 0x100, 102);
        itfDrawGridWithResolvedSlot(0, 0, 0, 32, slNewCycleData16, 15, 102);
        itfDrawGridWithResolvedSlot(7456, 0, 0, 32, slNewCycleData16, 15, 102);
        itfDrawGridWithResolvedSlot(0, 0, 0, 32, slNewCycleData16, 16, 103);
        itfDrawGridWithResolvedSlot(7120, 0, 0, 32, slNewCycleData16, 17, 103);
#else
        rstCallCalcAlphaRate(fmShutDownData, 0x100, 103);
        itfDrawGridWithResolvedSlot(0, 0, 0, 32, slNewCycleData16, 15, 103);
        itfDrawGridWithResolvedSlot(7456, 0, 0, 32, slNewCycleData16, 15, 103);
        itfDrawGridWithResolvedSlot(0, 0, 0, 32, slNewCycleData16, 16, 104);
        itfDrawGridWithResolvedSlot(7120, 0, 0, 32, slNewCycleData16, 17, 104);
#endif
    }
    if (gslDrawFlagFile != 0) {
        slNewCycle(id);
    }
    slDrawInfo();
    slQueryYesNoDraw();
    slDrawError();
    eglLinkAlpha(0);
    eglLinkTest(1, 5, 0x80, 3, 0, 0, 1, 2);
    return 0;
}

#ifdef VERSION_RAIDOU1
/* NON_MATCHING: 124 of 192 words match.  */
#ifdef NON_MATCHING
void fmProcTrigSequence_slMain(s32 mode) {
    s32 i = 0;
    fileRequest_t *pac;
    sdfPacDecodeResrc_t *resrc;
    dds3Basic_t *scene;
    s32 on;

    dds3SoftResetOff();
    fmShutDownData = InitBG();
    slNewCycleData16 = effLoadIndexedResource(fmProcTrigSequence_slMainData3, "saveload.spr", 0);
    pac = pacLoad("/mc/MCPACK.LB");
    pacSync(pac);
    for (resrc = pac->pac.resrcHead; resrc != NULL; resrc = resrc->next) {
        switch (i) {
            case 0:
                fmData2 = sdfTexSet(sdfMemGetPtr(resrc->resrc));
                break;
            case 1:
                fmData3 = sdfTexSet(sdfMemGetPtr(resrc->resrc));
                break;
            case 2:
                fmShutDownData3 = sdfTexSet(sdfMemGetPtr(resrc->resrc));
                break;
            case 3:
                fmShutDownData4 = sdfTexSet(sdfMemGetPtr(resrc->resrc));
                break;
            case 4:
                fmShutDownData5 = sdfTexSet(sdfMemGetPtr(resrc->resrc));
                break;
            case 5:
                slNewCycleData19 = sdfTexSet(sdfMemGetPtr(resrc->resrc));
                break;
        }
        i++;
        sdfMemFreeDirect(resrc->resrc);
    }
    pacFileHandleFree_(pac);
    on = 1;
    slData5++;
    if (mode == 0) {
        gslDrawFlagBg = on;
        fmProcTrigSequence_slMainData2 = 0;
        proc_func_ans = (s32 (*)(dds3ProcessID_t *))slSaveProc1st;
        gSlEndingMode = 0;
    } else if (mode == 1) {
        gSlEndingMode = 0;
        gslDrawFlagBg = on;
        proc_func_ans = (s32 (*)(dds3ProcessID_t *))slLoadProc1st;
        fmProcTrigSequence_slMainData2 = on;
    } else {
        fmProcTrigSequence_slMainData2 = 0;
        if (mode == 2) {
            gslDrawFlagBg = on;
            gSlEndingMode = on;
            evtBitOn(0xB90);
            DDS3_GBWK->half0C++;
            DDS3_GBWK->transition = on;
            proc_func_ans = (s32 (*)(dds3ProcessID_t *))slEndingProc1st;
        } else {
            gSlEndingMode = 0;
            if (mode == 4) {
                gslDrawFlagBg = 1;
                mode = 1;
                proc_func_ans = (s32 (*)(dds3ProcessID_t *))slLeaf9;
            } else {
                gslDrawFlagBg = 0;
                proc_func_ans = (s32 (*)(dds3ProcessID_t *))slBootProc1st;
            }
        }
    }
    dds3StartProcess("FileMentCalc", 0x2B08, 0, 0, slCalc, (s32 (*)(dds3ProcessID_t *))slShutDownProcess, NULL);
    slProcShutDownFlg = 1;
    dds3FadeIn(0, 0, 0, 8);
    slSetInfo(0);
    gSlExitMode = 0;
    gslMode = mode;
    slQueryYesNoCursor = -1;
    slExitWaitCnt = 0;
    slData3 = 0;
    gslErrorCode = 0;
    gslErrorKeyFlag = 0;
    gslDrawFlagFile = 0;
    gSlSaveMode = 0;
    gslCheckCardWait = 0;
    gslAnimCnt = 0;
    gSlNowCardCheck = 0;
    gSlFileErroeFlg = 0;
    slQueryYesNoMsg = 0;
    slQueryYesNoKeyFlag = 0;
    gSlMcAccessCnt = 0;
    gSlWindowCnt = 0;
    fileClearAllSlotFlags();
    scene = dds3GetActiveScene();
    if (scene != NULL) {
        dds3VisibleSceneBasic(scene, 0);
    }
}
#else
INCLUDE_RODATA(const s32, "mc/slMain", fmStrSaveloadSpr);

INCLUDE_ASM(const s32, "mc/slMain", fmProcTrigSequence_slMain);
#endif
#else
INCLUDE_ASM(const s32, "mc/slMain", fmProcTrigSequence_slMain);
#endif

void slShutDownProcess(dds3ProcessID_t *id) {
    fmShutDown();
}

void fmShutDownFade(void) {
}
#ifdef VERSION_RAIDOU2

extern struct sdfTexHandle_t *mcFormatSaveFilenameData;
#endif

void fmShutDown(void) {
    dds3Basic_t *scene;

    if (slProcShutDownFlg == 0) {
        return;
    }

    slProcShutDownFlg = 0;
#ifdef VERSION_RAIDOU2
    if (mcFormatSaveFilenameData != NULL) {
        sdfTexFreeDelayed(mcFormatSaveFilenameData);
        mcFormatSaveFilenameData = NULL;
    }
#endif
    if (slNewCycleData19 != NULL) {
        sdfTexFreeDelayed(slNewCycleData19);
        slNewCycleData19 = NULL;
    }
    if (fmShutDownData5 != NULL) {
        sdfTexFreeDelayed(fmShutDownData5);
        fmShutDownData5 = NULL;
    }
    if (fmShutDownData4 != NULL) {
        sdfTexFreeDelayed(fmShutDownData4);
        fmShutDownData4 = NULL;
    }
    if (fmShutDownData3 != NULL) {
        sdfTexFreeDelayed(fmShutDownData3);
        fmShutDownData3 = NULL;
    }
    if (fmData3 != NULL) {
        sdfTexFreeDelayed(fmData3);
        fmData3 = NULL;
    }
    if (fmData2 != NULL) {
        sdfTexFreeDelayed(fmData2);
        fmData2 = NULL;
    }
    effDestroyResourceSlotSet(slNewCycleData16);
    if (fmShutDownData != NULL) {
        rstDestroyBG(fmShutDownData);
        fmShutDownData = NULL;
    }
    scene = dds3GetActiveScene();
    if (scene != NULL) {
        dds3VisibleSceneBasic(scene, 1);
    }
    if (baseIconFileHandle != 0) {
        fileSync(EE_ADDR_AS(fileRequest_t *, baseIconFileHandle));
        slBaseIconMemHandle = fileGetMemHandle(EE_ADDR_AS(fileRequest_t *, baseIconFileHandle));
        fileHandleFree(EE_ADDR_AS(fileRequest_t *, baseIconFileHandle));
        baseIconFileHandle = 0;
    }
    sdfMemFreeHandleDirect(&slBaseIconMemHandle);
    dds3KillProcessName(slCalcName, 1);
    dds3SoftResetOn();
}

s32 fmShutDown2_slMain(void) {
    return 1;
}

s32 fmProcessChk_slMain(void) {
    return dds3SearchProcessName(slCalcName) != 0;
}

INCLUDE_ASM(const s32, "mc/slMain", slRetuenFromLoadSub);

INCLUDE_ASM(const s32, "mc/slMain", slQueryYesNoProc);

saddr_t slCallMcRegetInfo(s32 (*yesFunc)(void), s32 (*noFunc)(void), s32 yesNoDefault) {
    slQueryYesFunc = yesFunc;
    slQueryNoFunc = noFunc;
    slQueryYesNoCursor = yesNoDefault;
    slQueryYesNoKeyFlag = 0;
    if (slQueryYesNoMsg != 4 && slQueryYesNoMsg != 8 && slQueryYesNoMsg != 10 && slQueryYesNoMsg != 11 &&
        slQueryYesNoMsg != 12) {
        sdfMcRegetInfo(mccb);
    }
    return EE_ADDR_S(slQueryYesNoProc);
}

#ifdef VERSION_RAIDOU1
/* NON_MATCHING: 600 of 600 words match; its string table lands in this unit's .rodata, which then differs from retail. */
#ifdef NON_MATCHING
extern char slQueryYesNoDrawData6[], slOverwriteMsg1[], slQueryYesNoDrawData7[], slQueryYesNoDrawData[];
extern char slQueryYesNoDrawData2[], slQueryYesNoDrawData3[], slQueryYesNoDrawData4[], slQueryYesNoDrawData9[];
extern char slQueryYesNoDrawData10[], slQueryYesNoDrawData11[], slQueryYesNoDrawData12[], slQueryYesNoDrawData22[];
extern char slQueryYesNoDrawData23[], slQueryYesNoDrawData24[], slQueryYesNoDrawData25[], slQueryYesNoDrawData26[];
extern char slQueryYesNoDrawData27[], slQueryYesNoDrawData28[], slQueryYesNoDrawData29[], slQueryYesNoDrawData30[];
extern char slQueryYesNoDrawData31[], slQueryYesNoDrawData32[], slQueryYesNoDrawData33[], slQueryYesNoDrawData34[];
extern char slQueryYesNoDrawData35[], slQueryYesNoDrawData36[], slQueryYesNoDrawData37[], slQueryYesNoDrawData38[];
extern char slQueryYesNoDrawData5[], slReturnTitleMsg[], slQueryYesNoDrawData17[], slQueryYesNoDrawData18[];
extern char slQueryYesNoDrawData19[], slQueryYesNoDrawData13[], slQueryYesNoDrawData14[], slQueryYesNoDrawData15[];
extern char slQueryYesNoDrawData16[], slQueryYesNoDrawData20[], slQueryYesNoDrawData21[], slQueryYesNoDrawData41[];
extern char slQueryYesNoDrawData8[], slQueryYesNoDrawData39[], slQueryYesNoDrawData40[];

void slQueryYesNoDraw(void) {
    const char *loadingFileMsg[20] = {
        "Loading file 1.", "Loading file 2.", "Loading file 3.", "Loading file 4.", "Loading file 5.",
        "Loading file 6.", "Loading file 7.", "Loading file 8.", "Loading file 9.", "Loading file 10.",
        "Loading file 11.", "Loading file 12.", "Loading file 13.", "Loading file 14.", "Loading file 15.",
        "Loading file 16.", "Loading file 17.", "Loading file 18.", "Loading file 19.", "Loading file 20.",
    };
    s32 y;

    if ((slQueryYesNoCursor != -1) && (slQueryYesNoMsg != 0)) {
        eglSetOt(103);
        eglLinkAlpha(0);
        eglLinkTest(1, 0, 128, 3, 0, 0, 1, 1);
        if (slQueryYesNoMsg == 1) {
            if (slDrawWindow() != 0) {
                slKanjiDrawH(70, 114, 0x64E6AA80, slQueryYesNoDrawData6);
                slKanjiDrawH(70, 138, 0x64E6AA80, slOverwriteMsg1);
                slKanjiDrawH(70, 162, 0x64E6AA80, slQueryYesNoDrawData7);
            }
        }
        if (slQueryYesNoMsg == 2) {
            if (slDrawWindow() != 0) {
                slKanjiDrawH(46, 114, 0x64E6AA80, slQueryYesNoDrawData);
                slKanjiDrawH(46, 138, 0x64E6AA80, slQueryYesNoDrawData2);
                slKanjiDrawH(46, 162, 0x64E6AA80, slQueryYesNoDrawData3);
                slKanjiDrawH(46, 186, 0x64E6AA80, slQueryYesNoDrawData4);
            }
        }
        if (slQueryYesNoMsg == 3) {
            if (slDrawWindow() != 0) {
                slKanjiDrawH(70, 114, 0x64E6AA80, loadingFileMsg[slFileSelError]);
                slKanjiDrawH(70, 138, 0x64E6AA80, slQueryYesNoDrawData8);
            }
        }
        if (slQueryYesNoMsg == 4) {
            if (slDrawWindow() != 0) {
                slKanjiDrawH(70, 114, 0x64E6AA80, slQueryYesNoDrawData9);
                slKanjiDrawH(70, 138, 0x64E6AA80, slQueryYesNoDrawData10);
                slKanjiDrawH(70, 162, 0x64E6AA80, slQueryYesNoDrawData11);
                slKanjiDrawH(70, 186, 0x64E6AA80, slQueryYesNoDrawData12);
            }
        }
        if (slQueryYesNoMsg == 5) {
            if (slDrawWindow() != 0) {
                slKanjiDrawH(46, 90, 0x64E6AA80, slQueryYesNoDrawData22);
                slKanjiDrawH(46, 114, 0x64E6AA80, slQueryYesNoDrawData23);
                slKanjiDrawH(46, 138, 0x64E6AA80, slQueryYesNoDrawData24);
                slKanjiDrawH(46, 162, 0x64E6AA80, slQueryYesNoDrawData25);
                slKanjiDrawH(46, 186, 0x64E6AA80, slQueryYesNoDrawData26);
                slKanjiDrawH(46, 210, 0x64E6AA80, slQueryYesNoDrawData27);
                slKanjiDrawH(46, 234, 0x64E6AA80, slQueryYesNoDrawData28);
                slKanjiDrawH(46, 258, 0x64E6AA80, slQueryYesNoDrawData29);
            }
        }
        if (slQueryYesNoMsg == 6) {
            if (slDrawWindow() != 0) {
                slKanjiDrawH(46, 90, 0x64E6AA80, slQueryYesNoDrawData30);
                slKanjiDrawH(46, 114, 0x64E6AA80, slQueryYesNoDrawData31);
                slKanjiDrawH(46, 138, 0x64E6AA80, slQueryYesNoDrawData32);
                slKanjiDrawH(46, 162, 0x64E6AA80, slQueryYesNoDrawData33);
                slKanjiDrawH(46, 186, 0x64E6AA80, slQueryYesNoDrawData34);
                slKanjiDrawH(46, 210, 0x64E6AA80, slQueryYesNoDrawData35);
                slKanjiDrawH(46, 234, 0x64E6AA80, slQueryYesNoDrawData36);
                slKanjiDrawH(46, 258, 0x64E6AA80, slQueryYesNoDrawData37);
                slKanjiDrawH(46, 282, 0x64E6AA80, slQueryYesNoDrawData38);
            }
        }
        if (slQueryYesNoMsg == 7) {
            if (slDrawWindow() != 0) {
                slKanjiDrawH(70, 114, 0x64E6AA80, slQueryYesNoDrawData5);
            }
        }
        if (slQueryYesNoMsg == 8) {
            if (slDrawWindow() != 0) {
                slKanjiDrawH(70, 114, 0x64E6AA80, slReturnTitleMsg);
            }
        }
        if (slQueryYesNoMsg == 9) {
            if (slDrawWindow() != 0) {
                slKanjiDrawH(70, 114, 0x64E6AA80, slQueryYesNoDrawData17);
                slKanjiDrawH(70, 138, 0x64E6AA80, slQueryYesNoDrawData18);
                slKanjiDrawH(70, 162, 0x64E6AA80, slQueryYesNoDrawData19);
            }
        }
        if (slQueryYesNoMsg == 10) {
            if (slDrawWindow() != 0) {
                slKanjiDrawH(70, 114, 0x64E6AA80, slQueryYesNoDrawData13);
                slKanjiDrawH(70, 138, 0x64E6AA80, slQueryYesNoDrawData14);
                slKanjiDrawH(70, 162, 0x64E6AA80, slQueryYesNoDrawData15);
                slKanjiDrawH(70, 186, 0x64E6AA80, slQueryYesNoDrawData16);
            }
        }
        if (slQueryYesNoMsg == 11) {
            if (slDrawWindow() != 0) {
                slKanjiDrawH(70, 114, 0x64E6AA80, slQueryYesNoDrawData20);
            }
        }
        if (slQueryYesNoMsg == 12) {
            if (slDrawWindow() != 0) {
                slKanjiDrawH(70, 114, 0x64E6AA80, slQueryYesNoDrawData21);
                slKanjiDrawH(70, 138, 0x64E6AA80, slQueryYesNoDrawData41);
            }
        }
        if (slQueryYesNoProcSub() != 0) {
            y = 306;
            if (slQueryYesNoMsg != 5) {
                if (slQueryYesNoMsg == 6) {
                    y = 306;
                } else {
                    y = 258;
                }
            }
            eglSetOt(106);
            eglLinkAlpha(1);
            eglLinkTest(1, 0, 128, 3, 0, 0, 1, 1);
            itfMesGetWindowTexture();
            slNewCycleData2[2] = (slNewCycleData2[2] + 8) % 360;
            slNewCycleData2[3] = ((sdfSin((f32)((slNewCycleData2[2] + 90) % 360) / 180.0f * 3.14159f) + 1.0f) * 0.5f) * 80.0f + 112.0f;
            if (slQueryYesNoCursor == 0) {
                evtSubmitDefaultDepthGradientRect(61, y, 48, 24, 0x151BA6, 0x40151BA6, 0x151BA6, 0x40151BA6);
                evtSubmitDefaultDepthGradientRect(109, y, 48, 24, 0x40151BA6, 0x151BA6, 0x40151BA6, 0x151BA6);
            } else {
                evtSubmitDefaultDepthGradientRect(61, y + 24, 48, 24, 0x151BA6, 0x40151BA6, 0x151BA6, 0x40151BA6);
                evtSubmitDefaultDepthGradientRect(109, y + 24, 48, 24, 0x40151BA6, 0x151BA6, 0x40151BA6, 0x151BA6);
            }
            slKanjiDrawH(80, y, 0xA09DC380, slQueryYesNoDrawData39);
            slKanjiDrawH(80, y + 24, 0xA09DC380, slQueryYesNoDrawData40);
        }
        eglSetOt(103);
        eglLinkAlpha(0);
        eglLinkTest(1, 0, 128, 3, 0, 0, 1, 1);
    }
}
#else
INCLUDE_ASM(const s32, "mc/slMain", slQueryYesNoDraw);
#endif
#else
INCLUDE_ASM(const s32, "mc/slMain", slQueryYesNoDraw);
#endif

addr_t baseIconFileHandle = 0;

INCLUDE_SDATA(const s32, "mc/slMain", fmShutDownData);

INCLUDE_SDATA(const s32, "mc/slMain", slNewCycleData16);

INCLUDE_SDATA(const s32, "mc/slMain", slProcShutDownFlg);

s32 slExitWaitCnt = 0;

u32 gSlNowCardCheck = 0;

s32 gslCheckCardWait = 0;

u32 gslDrawFlagBg = 0;

u32 gslDrawFlagFile = 0;

s32 gslAnimCnt = 0;

INCLUDE_SDATA(const s32, "mc/slMain", slNewCycleData17);

INCLUDE_SDATA(const s32, "mc/slMain", gSlExitMode);

INCLUDE_SDATA(const s32, "mc/slMain", gSlSaveMode);

INCLUDE_SDATA(const s32, "mc/slMain", gSlEndingMode);

INCLUDE_SDATA(const s32, "mc/slMain", fmProcTrigSequence_slMainData2);

s32 slChooseSaveIndex = 0;

INCLUDE_SDATA(const s32, "mc/slMain", gSlFileErroeFlg);

INCLUDE_SDATA(const s32, "mc/slMain", slQueryYesNoCursor);

u32 slQueryYesNoMsg = 0;

INCLUDE_SDATA(const s32, "mc/slMain", slQueryYesNoKeyFlag);

INCLUDE_SDATA(const s32, "mc/slMain", slNewCycleData18);

INCLUDE_SDATA(const s32, "mc/slMain", slData2);

s32 slFileSelError = 0;

s32 gslMode = 0;

INCLUDE_SDATA(const s32, "mc/slMain", slData3);

s32 gslInfoCode = 0;

u32 gslErrorCode = 0;

INCLUDE_SDATA(const s32, "mc/slMain", gslErrorKeyFlag);

INCLUDE_SDATA(const s32, "mc/slMain", gSlMcAccessCnt);

s32 gSlWindowCnt = 0;

INCLUDE_RODATA(const s32, "mc/slMain", slCalcName);

#ifdef VERSION_RAIDOU1
INCLUDE_RODATA(const s32, "mc/slMain", jtbl_00455280);
#else
INCLUDE_RODATA(const s32, "mc/slMain", jtbl_004BC050);
#endif

#ifdef VERSION_RAIDOU1
INCLUDE_RODATA(const s32, "mc/slMain", slQueryYesNoDrawTable);
#else
INCLUDE_RODATA(const s32, "mc/slMain", slQueryYesNoDrawTable2);
#endif

INCLUDE_SDATA(const s32, "mc/slMain", setCallMcGetSlotIndexData);

INCLUDE_SDATA(const s32, "mc/slMain", slData5);

INCLUDE_SDATA(const s32, "mc/slMain", fmData2);

INCLUDE_SDATA(const s32, "mc/slMain", fmData3);

INCLUDE_SDATA(const s32, "mc/slMain", fmShutDownData3);

INCLUDE_SDATA(const s32, "mc/slMain", fmShutDownData4);

INCLUDE_SDATA(const s32, "mc/slMain", fmShutDownData5);

INCLUDE_SDATA(const s32, "mc/slMain", slNewCycleData19);

INCLUDE_SDATA(const s32, "mc/slMain", mcFormatSaveFilenameData);

INCLUDE_SDATA(const s32, "mc/slMain", slCallSin2Data);

INCLUDE_SDATA(const s32, "mc/slMain", slCallPlaySE2Data);

#ifdef VERSION_RAIDOU1
INCLUDE_SDATA(const s32, "mc/slMain", slErrorWaitMcData);

#endif
INCLUDE_SDATA(const s32, "mc/slMain", slSaveWriteEnd_baseIcon_slMainData2);

INCLUDE_SDATA(const s32, "mc/slMain", slSaveWriteEnd_baseIcon_slMainData3);

#ifdef VERSION_RAIDOU1
INCLUDE_SDATA(const s32, "mc/slMain", slNewCycleData20);
#else
INCLUDE_SDATA(const s32, "mc/slMain", slNewCycleData100);
#endif

#ifdef VERSION_RAIDOU1
INCLUDE_SDATA(const s32, "mc/slMain", slNewCycleData21);
#else
INCLUDE_SDATA(const s32, "mc/slMain", slNewCycleData101);
#endif

#ifdef VERSION_RAIDOU1
INCLUDE_SDATA(const s32, "mc/slMain", slNewCycleData22);

INCLUDE_SDATA(const s32, "mc/slMain", slNewCycleData23);
#else
INCLUDE_SDATA(const s32, "mc/slMain", slNewCycleData102);
#endif

INCLUDE_SDATA(const s32, "mc/slMain", fmProcTrigSequence_slMainData3);

INCLUDE_SDATA(const s32, "mc/slMain", slQueryYesNoDrawData39);

#ifdef VERSION_RAIDOU1
INCLUDE_SDATA(const s32, "mc/slMain", slQueryYesNoDrawData40);
#else
INCLUDE_SDATA(const s32, "mc/slMain", slQueryYesNoDrawData59);
#endif

#ifdef VERSION_RAIDOU1
INCLUDE_SDATA(const s32, "mc/slMain", slDrawInfoData17);
#else
INCLUDE_SDATA(const s32, "mc/slMain", slDrawErrorData54);

INCLUDE_SDATA(const s32, "mc/slMain", slQueryYesNoDrawData60);

INCLUDE_SDATA(const s32, "mc/slMain", slQueryYesNoDrawData61);

INCLUDE_SDATA(const s32, "mc/slMain", slDrawInfoData18);

INCLUDE_SDATA(const s32, "mc/slMain", slQueryYesNoDrawData62);

INCLUDE_SDATA(const s32, "mc/slMain", slQueryYesNoDrawData63);
#endif

INCLUDE_SDATA(const s32, "mc/slMain", slQueryYesNoDrawData41);

#ifdef VERSION_RAIDOU1
INCLUDE_SDATA(const s32, "mc/slMain", slNewCycleData24);
#else
INCLUDE_SDATA(const s32, "mc/slMain", slNewCycleData103);
#endif

#ifdef VERSION_RAIDOU1
INCLUDE_SDATA(const s32, "mc/slMain", slNewCycleData25);
#else
INCLUDE_SDATA(const s32, "mc/slMain", slNewCycleData104);
#endif

#ifdef VERSION_RAIDOU1
INCLUDE_SDATA(const s32, "mc/slMain", slNewCycleData26);

INCLUDE_SDATA(const s32, "mc/slMain", slNewCycleData27);

INCLUDE_SDATA(const s32, "mc/slMain", slNewCycleData28);

INCLUDE_SDATA(const s32, "mc/slMain", slNewCycleData29);

INCLUDE_SDATA(const s32, "mc/slMain", slNewCycleData30);

INCLUDE_SDATA(const s32, "mc/slMain", slNewCycleData31);

INCLUDE_SDATA(const s32, "mc/slMain", slNewCycleData32);

INCLUDE_SDATA(const s32, "mc/slMain", slNewCycleData33);

INCLUDE_SDATA(const s32, "mc/slMain", slNewCycleData34);

INCLUDE_SDATA(const s32, "mc/slMain", slNewCycleData35);

INCLUDE_SDATA(const s32, "mc/slMain", slNewCycleData36);

INCLUDE_SDATA(const s32, "mc/slMain", slNewCycleData37);

#endif
