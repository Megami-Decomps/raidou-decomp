#include "common.h"
#include "sdf_resource.h"
#include "basic.h"
#include "model.h"
#include "newdata.h"
#include "scr.h"
#include "field.h"
#include "ps2asm.h"
#include "sdfFrame.h"

typedef struct fileRequest fileRequest_t;

enum sl_mc_result
{
    SL_MC_RESULT_PENDING = 0,
    SL_MC_RESULT_SUCCESS = 1,
    SL_MC_RESULT_ERROR = -1,
    SL_MC_RESULT_SPECIAL_ERROR = -2
};

typedef enum sl_mc_result slMcResult_t;

enum sl_slot_flag
{
    SL_SLOT_FLAG_CAN_READ_SAVE = 1,
    SL_SLOT_FLAG_DIRECTORY_READY = 2,
    SL_SLOT_FLAG_DELETE_BEFORE_WRITE = 8
};

void sdfMemFreeDirect(sdfMemHandle_t *aHandle);
extern s32 gSlWindowCnt;
extern s32 gslInfoCode;
extern s32 slChooseSaveIndex;
extern u32 mccb;
void sdfMcSetSlotIndex(s32 request, s32 selectedSlot);
s32 sdfMcGetSlotIndex(s32 request);
u32 sdfMcGetSlotFlags(s32 request, s32 slot);
extern saddr_t slChooseData_getCardInfoNext(void);
extern void mcMakeFileName(char *path, s32 num);
extern u32 strlen(const char *s);
extern void *memcpy(void *, const void *, size_t);
void sdfMcFuncOpen(u32 context, const char *path, s32 openFlags);
saddr_t slHeadLoadProc_openFile(void);
extern s32 slMcFd;
slMcResult_t sdfMcFuncOpen1(s32 *resultOut);
extern sdfMemHandle_t *slLoadFileFd;
saddr_t slHeadLoadProc_readFile(void);
extern saddr_t slNoCard(void);
slMcResult_t sdfMcFuncRead1(void);
extern void sdfMcFuncClose(s32);
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
void sdfMcRegetInfo(s32 request);
extern s32 gslCheckCardWait;
extern u32 gSlNowCardCheck;
extern u32 gslDrawFlagBg;
extern sdfMemHandle_t *slBaseIconMemHandle;
extern addr_t baseIconFileHandle;
extern u32 gslErrorKeyFlag;
extern s8 dds3PadButtonState[];
void p2sdPlaySE(s32 trackId, s32 volume, s32 pan);

/* One step of the save and load state machine, which this unit holds as an
 * address and calls through a cast. */
typedef s32 (*slProcFunc_t)(void);
extern u32 slHitAnyKeyNextFunc;
void sdfMcFuncChdir(u32 context, const char *path);
saddr_t slChooseData_getCardInfo2(void);
slMcResult_t sdfMcFuncChdir1(void);
void sdfMcSetSlotFlags(s32 request, s32 slot, s32 mask);
extern const char slIconSysName[];
extern u8 slDirEntries[];
void sdfMcFuncGetDir(u32 context, const char *path, void *directoryEntries, s32 entryLimit);
s32 slChooseData_getCardInfo3(void);
slMcResult_t sdfMcFuncGetDir1(s32 *resultOut);
saddr_t slChooseData_getCardInfo2_B(void);
s32 slChooseData_getCardInfo3_B(void);
s32 slChooseData_getCardInfoNext_B(void);
void sdfMcSetCardStat(s32 request);
saddr_t slSaveData_chdir(void);
void sdfMcFuncMkdir(u32 mccb, const char *cb);
saddr_t slSaveProc_Write_mkdir(void);
saddr_t slSaveProc_Write_chdir(void);
slMcResult_t sdfMcFuncMkdir1(void);
extern s32 slFileSelError;
saddr_t slSaveData_deleteIconSys(void);
extern s32 slSaveData_writeFile(void);
extern const char slBaseIconName[];
extern s32 slSaveWriteEnd_baseIcon(void);
extern addr_t slBaseIconBuff;
extern u32 slBaseIconBuffSize;
saddr_t slSaveWriteFile(const char *filename, u32 *buff, u32 *size, void *endFunc, u32 *srcPtr);
void sdfMcFuncDelete(u32 context, const char *path);
extern u32 *slSaveWriteFile_buff;
extern u32 *slSaveWriteFile_size;
extern void *slSaveWriteFile_endFunc;
extern u32 *slSaveWriteFile_dataWaitPtr;
saddr_t slSaveProc_openFile(void);
saddr_t slSaveProc_waitDataReady(void);
extern void sdfMcFuncWrite(s32 request, u32 first, u32 second);
saddr_t slSaveProc_writeFile(void);
slMcResult_t sdfMcFuncWrite1(void);
saddr_t slSaveProc_closeFile(void);
slMcResult_t sdfMcFuncClose1(void);
slMcResult_t sdfMcFuncFormat1(void);
extern void dds3SoftResetOn(void);
s32 slCardChk(void);
extern void dds3SoftResetOff(void);
void sdfMcFuncFormat(u32 port);
saddr_t slLoadProc_readFile(void);
saddr_t slLoadProc_closeFile(void);
extern s32 gslAnimCnt;
extern s32 slDraw(s32);
s32 pacFileCheckEnd(const fileRequest_t *pacFileHandle);
sdfMemHandle_t *fileGetMemHandle(const fileRequest_t *fileHandle);
u8 *fileGetLoadPtr(const fileRequest_t *fileHandle);
u32 fileGetLoadSize(const fileRequest_t *fileHandle);
s32 fileHandleFree(fileRequest_t *fileHandle);
extern s32 slQueryYesNoCursor;
extern s32 slQueryYesNoKeyFlag;
extern s32 (*proc_func_ans)(dds3ProcessID_t *process);

#include "sdk_libc.h"

extern void dds3AdminiCallSequence(s32 sequence, s32 *data, s32 datasize, s32 restore);
extern void sdfMcClearSlotFlags(s32 request, s32 slot);
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

#ifndef VERSION_DDS1
#define VERSION_DDS1 1
#endif

#define SMT3_DEFINES_SL_MC_RESULT_ERROR 1

#define SMT3_DEFINES_SL_MC_RESULT_PENDING 1

#define SMT3_DEFINES_SL_MC_RESULT_SPECIAL_ERROR 1

#define SMT3_DEFINES_SL_MC_RESULT_SUCCESS 1

#define SMT3_DEFINES_SL_SLOT_FLAG_CAN_READ_SAVE 1

#define SMT3_DEFINES_SL_SLOT_FLAG_DELETE_BEFORE_WRITE 1

#define SMT3_DEFINES_SL_SLOT_FLAG_DIRECTORY_READY 1

#define SMT3_DEFINES_fileRequest_t 1

#define SMT3_DEFINES_slConvertWork_t 1

#define SMT3_DEFINES_slFileName_t 1

#define SMT3_DEFINES_slFileRec_t 1

#define SMT3_DEFINES_slMcResult_t 1

#define SMT3_DEFINES_slProcFunc_t 1

#define SMT3_DEFINES_sl_mc_result 1

#define SMT3_DEFINES_sl_slot_flag 1

#include "sdk_libkernl.h"
#include "dds/bill_object_api.h"
#include "dds/btl_stage_task_cleanup.h"
#include "dds/dds3_task_lifecycle.h"
#include "dds/eff_random.h"
#include "dds/file_request_api.h"
#include "dds/sdf_chip.h"
#include "dds/sdf_dev_state.h"
#include "dds/sdf_heap_api.h"
#include "dds/sdf_trig.h"
#include "dds/sdf_vu_matrix.h"
#include "dds/sdf_vu_matrix_multiply_api.h"
#include "dds/sdf_vu_rotation_axes.h"
#include "dds/sdf_vu_transform_api.h"
#include "dds/sdk_libc.h"
#include "dds/sdk_libkernl.h"
#include "dds/sdk_libm.h"

#define SDK_SKIP_memset

#include "dds/sdk_ps2.h"
#include "dds/common.h"
#include "dds/file_menu_start_mode.h"
#include "dds/file_menu_state_result.h"
#include "dds/file_config_list_node.h"
#include "dds/file_config_option_api.h"
#include "dds/sdf_vu0.h"
#include "dds/file_job_type.h"
#include "dds/file_config_task.h"
#include "dds/file_load_menu_state.h"
#include "dds/eff.h"
#include "dds/cam_aim_record.h"
#include "dds/eff_generated_texture_descriptor.h"
#include "dds/dds3_world_selection.h"
#include "dds/mnu_title_stream.h"
#include "dds/snd_sequence_volume_pan.h"
#include "dds/eff_scale_params.h"
#include "dds/ee_mmi.h"
#include "dds/sdf_textured_rect.h"
#include "dds/itf_mes_window.h"
#include "dds/evt_world.h"
#include "dds/fr_font.h"
#include "dds/fr_font_context.h"
#include "dds/mc_poll.h"
#include "dds/mc_read_api.h"
#include "dds/mc_write_api.h"
#include "dds/mc_path_api.h"
#include "dds/eff_resource_slots.h"
#include "dds/eff_resource_source_lifetime.h"
#include "dds/eff_curve.h"
#include "dds/file.h"
#include "dds/file_selection_api.h"
#include "dds/file_prompt_reveal_result.h"
#include "dds/file_request_entry.h"
#include "dds/file_slot_flags.h"
#include "dds/file_slot_flags_change_api.h"
#include "dds/file_scroll_arrow_state.h"
#include "dds/file_slot.h"
#include "dds/file_save_record.h"
#include "dds/dat_state.h"
#include "dds/dat_new_cycle_state.h"
#include "dds/pcp_vu0.h"
#include "dds/dds3_process.h"
#include "dds/dds3_task_create.h"
#include "dds/fpu.h"
#include "dds/mnu_list.h"
#include "dds/eff_expanded_list.h"
#include "dds/mdl_flag_api.h"
#include "dds/sdf.h"
#include "dds/dds3_fade.h"
#include "dds/file_pac.h"
#include "dds/sdf_pac_work.h"
#include "dds/sdf_texture_file.h"
#include "dds/btl.h"
#include "dds/fr_font_draw_glyph.h"
#include "dds/itf.h"

typedef s32 (*FileMenuContinuationCallback)(void);

typedef struct MenuResourceWork MenuResourceWork;

struct MenuListNode;

struct EffectSlotSet;

enum {
    FILE_CONFIG_SET,
    FILE_CONFIG_FRAME,
    FILE_CONFIG_X,
    FILE_CONFIG_Y
};

#include "dds/eff_emitter_type.h"

struct sdfMemHandle_t;

typedef struct LoadMirror {
    u32 current;
    u32 previous;
} LoadMirror;

#include "dds/sdf_debug_api.h"

extern u32 gslErrorCode_slMain;
extern u32 gslDrawFlagFile_slMain;
extern u32 fileMemoryCardRequestIndex;
extern s32 fileResetSelection(void);
extern u32 D_003BC834;

/* Init record at fileCursorPulseState. */
typedef struct Init374E0 {
    s8 mode[3];      /* 0x00 */
    u8 pad3;         /* 0x03 */
    s32 pos[3][2];   /* 0x04 */
    s32 counter[3];  /* 0x1C */
    u8 pad28[0xC];   /* 0x28 */
    s32 alpha[3];    /* 0x34 */
} Init374E0;

/* Object with loader sub-objects (+0x40...). */
typedef struct LoadObj {
    void *owner;        /* 0x00 */
    u32 color;          /* 0x04 */
    f32 scale;          /* 0x08 */
    u32 selector;       /* 0x0C: secondary job buffer operation */
    u8 pad10[0x24];
    void *deviceHandle; /* 0x34 */
    FileJobPayload **jobs; /* 0x38: child file-job payloads */
    struct sdfMemHandle_t *jobAllocation;          /* 0x3C */
    struct EffExpandedList *referenceHolder; /* 0x40 */
    FileSlotTable *recordWork; /* 0x44: created by fileAllocateGridRecordSlots */
    s16 unk48;          /* 0x48 */
    u16 unk4A;
} LoadObj;

#define FILE_MAIN_BLOB_SIZE 0x33600

u8 fileIsLoadedAndConditionTrue(s32 condition);
void mcdCreateFontDrawHandle(s32 x, s32 y, u32 color, const char *text);

static inline void fileDrawDialogLine(s32 line, const char *glyphSource) {
    mcdCreateFontDrawHandle(86, 114 + line * 24, 0x89FEFF80U, glyphSource);
}

void fileClearAllSlotFlags(void);
s32 fileResetSelection(void);
s32 mcdEnterDefaultFileFlow(void);
s32 fileRestartSlotScan(void);
extern s32 D_003BC830;
extern s32 D_003BC838;
extern FileSelectionCallback D_003BD908, D_003BD90C;

/* Yes/no cursor input while the card request is idle. */
static inline void filePollPromptChoice(void) {
    if (D_003BC838 == 0 && slChkWindowOpenDone() != 0) {
        if (fileIsLoadedAndConditionTrue(dds3PadButtonState[0x27] < 0) != 0) {
            if (D_003BC830 != 1) {
                p2sdPlaySE(0, 0x7F, 0x3F);
            }
            D_003BC830 = 1;
        } else if (fileIsLoadedAndConditionTrue(dds3PadButtonState[0x26] < 0) != 0) {
            if (D_003BC830 != 0) {
                p2sdPlaySE(0, 0x7F, 0x3F);
            }
            D_003BC830 = 0;
        } else if (fileIsLoadedAndConditionTrue(dds3PadButtonState[0x21] < 0) != 0) {
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
        } else if (fileIsLoadedAndConditionTrue(dds3PadButtonState[0x23] < 0) != 0) {
            p2sdPlaySE(0xA, 0x7F, 0x3F);
            D_003BC838 = 2;
        }
    }
}

/* Lower the selected low/alpha byte, retaining the native signed, unmasked bound. */

#define FILE_JOB_LOCATION_AS_JOB_RELATIVE_OFFSET(payload) ((payload).offset)

#define FILE_JOB_LOCATION_AS_LOCKED_ADDRESS_WORD(payload) ((payload).offset)

#define FILE_JOB_LOCATION_CLEAR(payload) ((payload).offset = 0)

extern FileJob *fileQueueFindBySector(FileQueue *queue, u32 sector);

/* Makes the first job chained to owner's sector the leader and rechains the rest to it. */
static inline void fileQueueRechainSectorFollowers(FileQueue *queue, FileJob *owner, FileJob *leader) {
    FileJob *next;

    leader->sector = 0;
    leader->flags &= ~FILE_JOB_FLAG_SECTOR_FOLLOWER;
    fileQueueLinkJobToSectorLeader(queue, leader, leader);
    next = fileQueueFindBySector(queue, owner->id);
    while (next != NULL) {
        fileQueueLinkJobToSectorLeader(queue, next, leader);
        next = fileQueueFindBySector(queue, owner->id);
    }
}

void fileQueueLinkJobToSectorLeader(FileQueue *queue, FileJob *job, FileJob *ref);

/* Sixteen-byte preamble of a saved file queue. */
typedef struct FileQueueImageHeader {
    s32 version;
    s32 unk4;
    s32 unk8;
    f32 unkC;
} FileQueueImageHeader;

FileJob *fileQueueFindBySector(FileQueue *queue, u32 sector);

/* Creates a load object sized by the record's cell count, capped at 0x12C. */
typedef struct FileCellGrid {
    u8 pad00[0x20];
    u32 cols;
    u32 rows;
    u8 pad28[0x90];
    u32 layers;
} FileCellGrid;

#include "dds/eff_anim.h"

typedef struct FileWaveMotion {
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
} FileWaveMotion;            /* 0x44, ordinary word alignment */

typedef struct FileSlotMotion {
    f32 direction[4];     /* 0x00; this callback initializes xyz only */
    f32 speed;            /* 0x10 */
    f32 scaleMultiplier;  /* 0x14 */
    f32 angle;            /* 0x18 */
    f32 angleMultiplier;  /* 0x1C */
} FileSlotMotion;

typedef struct FileCircularMotion {
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
} FileCircularMotion;       /* 0x44, ordinary word alignment */

typedef struct FileOrientedRingMotion {
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
} FileOrientedRingMotion;   /* 0x38, ordinary word alignment */

INCLUDE_ASM(const s32, "mc/slMain", slKanjiDraw3);

INCLUDE_ASM(const s32, "mc/slMain", slKanjiDrawH);

INCLUDE_ASM(const s32, "mc/slMain", slCallEnableMode2);

INCLUDE_ASM(const s32, "mc/slMain", slCallSin2);

INCLUDE_ASM(const s32, "mc/slMain", effDebugPrintf);

INCLUDE_ASM(const s32, "mc/slMain", slQueryYesNoProcSub);

INCLUDE_ASM(const s32, "mc/slMain", slQueryYesNoProcSub2);

INCLUDE_ASM(const s32, "mc/slMain", slCallSetOt5);

INCLUDE_ASM(const s32, "mc/slMain", slInsertedInMEMORY);

void slSetInfo(u32 id) {
    s32 previous;

    previous = gslInfoCode;
    gslInfoCode = id;
    if (previous == 0) {
        gSlWindowCnt = 0;
    }
}

INCLUDE_ASM(const s32, "mc/slMain", slDrawInfo);

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

INCLUDE_ASM(const s32, "mc/slMain", slHeadLoadProc_openFile);

saddr_t slHeadLoadProc_readFile(void) {
    slMcResult_t status = sdfMcFuncRead1();
    if (status == SL_MC_RESULT_PENDING) {
        return 0;
    }
    if (status == SL_MC_RESULT_SUCCESS) {
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

INCLUDE_ASM(const s32, "mc/slMain", slEndingExit);

INCLUDE_ASM(const s32, "mc/slMain", slCallSetInfo);

INCLUDE_ASM(const s32, "mc/slMain", slBootTop_slMain);

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

INCLUDE_ASM(const s32, "mc/slMain", slHardSelect2);

s32 slExit(void) {
    slSetInfo(0);
    gslDrawFlagFile = 0;
    gslDrawFlagBg = 0;
    return 0xffffffff;
}

INCLUDE_ASM(const s32, "mc/slMain", slCallSetInfo2);

s32 mcdEnterDefaultFileFlow(void) {
    u32 mode[4];

    mode[0] = 0;
    dds3AdminiCallSequence(2, mode, 4, 0);
    return 0;
}

INCLUDE_ASM(const s32, "mc/slMain", slLeaf4);

INCLUDE_ASM(const s32, "mc/slMain", slCallAdminiCallSequence);

INCLUDE_ASM(const s32, "mc/slMain", slLoadProc1st);

s32 fileRestartSlotScan(void) {
    fileClearAllSlotFlags();
    slSetInfo(1);
    gslErrorCode_slMain = 0;
    gslDrawFlagFile_slMain = 0;
    return fileResetSelection();
}

INCLUDE_ASM(const s32, "mc/slMain", slCallMcGetSlotFlags);

INCLUDE_ASM(const s32, "mc/slMain", slCancel);

INCLUDE_ASM(const s32, "mc/slMain", slMcSumIco);

INCLUDE_ASM(const s32, "mc/slMain", slCallSetInfo3);

INCLUDE_ASM(const s32, "mc/slMain", slRefSystem);

INCLUDE_ASM(const s32, "mc/slMain", slLeaf5);

INCLUDE_ASM(const s32, "mc/slMain", slLeaf6);

INCLUDE_ASM(const s32, "mc/slMain", slCallSetInfo4);

INCLUDE_ASM(const s32, "mc/slMain", slCallSetInfo5);

INCLUDE_ASM(const s32, "mc/slMain", slLeaf7);

INCLUDE_ASM(const s32, "mc/slMain", slCallSetInfo6);

INCLUDE_ASM(const s32, "mc/slMain", slLeaf8);

INCLUDE_ASM(const s32, "mc/slMain", slLeaf9);

INCLUDE_ASM(const s32, "mc/slMain", slEndingProc1st);

s32 slBootProc1st(void) {
    sdfMcRegetInfo(0);
    gSlNowCardCheck = 1;
    gslCheckCardWait = 0;
    slSetInfo(0);
    gslDrawFlagFile = 0;
    return slBootTop();
}

INCLUDE_ASM(const s32, "mc/slMain", slCallSetInfo7);

INCLUDE_ASM(const s32, "mc/slMain", slCallPlaySE2);

INCLUDE_ASM(const s32, "mc/slMain", slCallBitOn);

INCLUDE_ASM(const s32, "mc/slMain", slHardSelect2N);

INCLUDE_ASM(const s32, "mc/slMain", slHardSelect3);

INCLUDE_ASM(const s32, "mc/slMain", slHardSelect4);

INCLUDE_ASM(const s32, "mc/slMain", slSaveProc_CheckCard);

INCLUDE_ASM(const s32, "mc/slMain", sl1Ken);

INCLUDE_ASM(const s32, "mc/slMain", slSizeChk);

INCLUDE_ASM(const s32, "mc/slMain", slBootProc_CheckCardR);

INCLUDE_ASM(const s32, "mc/slMain", slBootProc_CheckCard);

INCLUDE_ASM(const s32, "mc/slMain", slErrorWaitMc);

INCLUDE_ASM(const s32, "mc/slMain", slErrorWaitToFileMc);

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

INCLUDE_ASM(const s32, "mc/slMain", slHitAnyKey);

INCLUDE_ASM(const s32, "mc/slMain", slHitAnyKeyProc);

INCLUDE_ASM(const s32, "mc/slMain", slChooseData_getCardInfo_B);

saddr_t slChooseData_getCardInfo2(void) {
    slMcResult_t t = sdfMcFuncChdir1();

    if (t == SL_MC_RESULT_PENDING) {
        return 0;
    }
    if (t == SL_MC_RESULT_SUCCESS) {
        sdfMcSetSlotFlags(mccb, slChooseSaveIndex, SL_SLOT_FLAG_DIRECTORY_READY);
        sdfMcFuncGetDir(mccb, slIconSysName, slDirEntries, 1);
        return EE_ADDR_S(slChooseData_getCardInfo3);
    }
    if (t == SL_MC_RESULT_ERROR) {
        return slNoCard();
    }
    return slHeadLoad();
}

s32 slChooseData_getCardInfo3(void) {
    s32 value;
    slMcResult_t status = sdfMcFuncGetDir1(&value);
    if (status == SL_MC_RESULT_PENDING) {
        return 0;
    }
    if (status == SL_MC_RESULT_SUCCESS) {
        if (value == 1) {
            sdfMcSetSlotFlags(mccb, slChooseSaveIndex,
                                SL_SLOT_FLAG_CAN_READ_SAVE | SL_SLOT_FLAG_DELETE_BEFORE_WRITE);
        }
        return slHeadLoad();
    }
    if (status == SL_MC_RESULT_ERROR) {
        return slNoCard();
    }
    return slHeadLoad();
}

INCLUDE_ASM(const s32, "mc/slMain", slCallMcClearCardStat);

INCLUDE_ASM(const s32, "mc/slMain", slChooseData);

INCLUDE_ASM(const s32, "mc/slMain", slChooseData_getCardInfo);

saddr_t slChooseData_getCardInfo2_B(void) {
    slMcResult_t t = sdfMcFuncChdir1();

    if (t == SL_MC_RESULT_PENDING) {
        return 0;
    }
    if (t == SL_MC_RESULT_SUCCESS) {
        sdfMcSetSlotFlags(mccb, slChooseSaveIndex, SL_SLOT_FLAG_DIRECTORY_READY);
        sdfMcFuncGetDir(mccb, slIconSysName, slDirEntries, 1);
        return EE_ADDR_S(slChooseData_getCardInfo3_B);
    }
    if (t == SL_MC_RESULT_ERROR) {
        slSetInfo(0);
        gslDrawFlagFile = 0;
        gslErrorCode = 0;
        return slBootError();
    }
    return slChooseData_getCardInfoNext_B();
}

s32 slChooseData_getCardInfo3_B(void) {
    s32 value;
    slMcResult_t status = sdfMcFuncGetDir1(&value);
    if (status == SL_MC_RESULT_PENDING) {
        return 0;
    }
    if (status == SL_MC_RESULT_SUCCESS) {
        if (value == 1) {
            sdfMcSetSlotFlags(mccb, slChooseSaveIndex,
                                SL_SLOT_FLAG_CAN_READ_SAVE | SL_SLOT_FLAG_DELETE_BEFORE_WRITE);
        }
        return slChooseData_getCardInfoNext_B();
    }
    if (status == SL_MC_RESULT_ERROR) {
        slSetInfo(0);
        gslDrawFlagFile = 0;
        gslErrorCode = 0;
        return slBootError();
    }
    return slChooseData_getCardInfoNext_B();
}

INCLUDE_ASM(const s32, "mc/slMain", slChooseData_getCardInfoNext_B);

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
    slMcResult_t status = sdfMcFuncMkdir1();
    if (status == SL_MC_RESULT_PENDING) {
        return 0;
    }
    if (status == SL_MC_RESULT_SUCCESS) {
        sdfMcSetSlotFlags(mccb, slFileSelError, SL_SLOT_FLAG_DIRECTORY_READY);
        return slSaveData_chdir();
    }
    slSetInfo(0);
    gslErrorCode = 4;
    return EE_ADDR_S(slErrorWait);
}

INCLUDE_ASM(const s32, "mc/slMain", slCallMcFuncChdir1);

INCLUDE_ASM(const s32, "mc/slMain", slLoadData_slMain);

INCLUDE_ASM(const s32, "mc/slMain", slSaveProc_doNothing);

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

saddr_t slSaveWriteFile(const char *filename, u32 *buff, u32 *size, void *endFunc, u32 *srcPtr) {
    slSaveWriteFile_buff = buff;
    slSaveWriteFile_size = size;
    slSaveWriteFile_endFunc = endFunc;
    slSaveWriteFile_dataWaitPtr = srcPtr;
    sdfMcFuncOpen(mccb, filename, 0x203);
    return EE_ADDR_S(slSaveProc_openFile);
}

saddr_t slSaveProc_openFile(void) {
    slMcResult_t status = sdfMcFuncOpen1(&slMcFd);

    if (status == SL_MC_RESULT_PENDING) {
        return 0;
    }
    if (status == SL_MC_RESULT_SUCCESS) {
        if (slSaveWriteFile_dataWaitPtr != NULL && *slSaveWriteFile_dataWaitPtr != 0) {
            return EE_ADDR_S(slSaveProc_waitDataReady);
        }
        sdfMcFuncWrite(slMcFd, *slSaveWriteFile_buff, *slSaveWriteFile_size);
        return EE_ADDR_S(slSaveProc_writeFile);
    }
    if (status == SL_MC_RESULT_ERROR) {
        slSetInfo(0);
        gslErrorCode = 4;
        return EE_ADDR_S(slErrorWait);
    }
    return 0;
}

saddr_t slSaveProc_writeFile(void) {
    slMcResult_t status = sdfMcFuncWrite1();
    if (status == SL_MC_RESULT_PENDING) {
        return 0;
    }
    if (status == SL_MC_RESULT_SUCCESS) {
        sdfMcFuncClose(slMcFd);
        return EE_ADDR_S(slSaveProc_closeFile);
    }
    if (status == SL_MC_RESULT_ERROR) {
        slSetInfo(0);
        gslErrorCode = 4;
        return EE_ADDR_S(slErrorWait);
    }
    return 0;
}

saddr_t slSaveProc_closeFile(void) {
    slMcResult_t status = sdfMcFuncClose1();
    if (status == SL_MC_RESULT_PENDING) {
        return 0;
    }
    if (status == SL_MC_RESULT_SUCCESS) {
        return ((s32 (*)())slSaveWriteFile_endFunc)();
    }
    if (status == SL_MC_RESULT_ERROR) {
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
    slMcResult_t status = sdfMcFuncFormat1();
    if (status == SL_MC_RESULT_PENDING) {
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

INCLUDE_ASM(const s32, "mc/slMain", slLoadData);

INCLUDE_ASM(const s32, "mc/slMain", slLoadProc_openFile);

saddr_t slLoadProc_readFile(void) {
    slMcResult_t status = sdfMcFuncRead1();
    if (status == SL_MC_RESULT_PENDING) {
        return 0;
    }
    if (status == SL_MC_RESULT_SUCCESS) {
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
        slBaseIconBuff = EE_ADDR(fileGetLoadPtr((fileRequest_t *)EE_ADDR_PTR(job)));
        slBaseIconBuffSize = fileGetLoadSize((fileRequest_t *)EE_ADDR_PTR(job));
        fileHandleFree((fileRequest_t *)EE_ADDR_PTR(job));
    }
    return 0;
}

INCLUDE_ASM(const s32, "mc/slMain", slDraw_slMain);

INCLUDE_RODATA(const s32, "mc/slMain", fmStrSaveloadSpr);

INCLUDE_ASM(const s32, "mc/slMain", fmProcTrigSequence_slMain);

INCLUDE_ASM(const s32, "mc/slMain", slShutDownProcess);

void fmShutDownFade(void) {
}

INCLUDE_ASM(const s32, "mc/slMain", fmShutDown);

INCLUDE_ASM(const s32, "mc/slMain", fmShutDown2_slMain);

INCLUDE_ASM(const s32, "mc/slMain", fmProcessChk_slMain);

INCLUDE_ASM(const s32, "mc/slMain", slRetuenFromLoadSub);

INCLUDE_ASM(const s32, "mc/slMain", slQueryYesNoProc);

INCLUDE_ASM(const s32, "mc/slMain", slCallMcRegetInfo);

INCLUDE_ASM(const s32, "mc/slMain", slQueryYesNoDraw);

addr_t baseIconFileHandle = 0;

INCLUDE_SDATA(const s32, "mc/slMain", fmShutDownData);

INCLUDE_SDATA(const s32, "mc/slMain", slNewCycleData16);

INCLUDE_SDATA(const s32, "mc/slMain", fmShutDownData2);

s32 slExitWaitCnt = 0;

u32 gSlNowCardCheck = 0;

s32 gslCheckCardWait = 0;

u32 gslDrawFlagBg = 0;

u32 gslDrawFlagFile = 0;

s32 gslAnimCnt = 0;

INCLUDE_SDATA(const s32, "mc/slMain", slNewCycleData17);

INCLUDE_SDATA(const s32, "mc/slMain", slDrawInfoData15);

INCLUDE_SDATA(const s32, "mc/slMain", slFileSelCalcData);

INCLUDE_SDATA(const s32, "mc/slMain", fmProcTrigSequence_slMainData);

INCLUDE_SDATA(const s32, "mc/slMain", fmProcTrigSequence_slMainData2);

s32 slChooseSaveIndex = 0;

INCLUDE_SDATA(const s32, "mc/slMain", slFileSelCalcData2);

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

INCLUDE_SDATA(const s32, "mc/slMain", effDebugPrintfData);

s32 gSlWindowCnt = 0;

INCLUDE_RODATA(const s32, "mc/slMain", fmStrFileMentCalc);

INCLUDE_RODATA(const s32, "mc/slMain", jtbl_00455280);

INCLUDE_RODATA(const s32, "mc/slMain", slQueryYesNoDrawTable);

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

INCLUDE_SDATA(const s32, "mc/slMain", slErrorWaitMcData);

INCLUDE_SDATA(const s32, "mc/slMain", slSaveWriteEnd_baseIcon_slMainData2);

INCLUDE_SDATA(const s32, "mc/slMain", slSaveWriteEnd_baseIcon_slMainData3);

INCLUDE_SDATA(const s32, "mc/slMain", slNewCycleData20);

INCLUDE_SDATA(const s32, "mc/slMain", slNewCycleData21);

INCLUDE_SDATA(const s32, "mc/slMain", slNewCycleData22);

INCLUDE_SDATA(const s32, "mc/slMain", slNewCycleData23);

INCLUDE_SDATA(const s32, "mc/slMain", fmProcTrigSequence_slMainData3);

INCLUDE_SDATA(const s32, "mc/slMain", slQueryYesNoDrawData39);

INCLUDE_SDATA(const s32, "mc/slMain", slQueryYesNoDrawData40);

INCLUDE_SDATA(const s32, "mc/slMain", slDrawInfoData17);

INCLUDE_SDATA(const s32, "mc/slMain", slQueryYesNoDrawData41);

INCLUDE_SDATA(const s32, "mc/slMain", slNewCycleData24);

INCLUDE_SDATA(const s32, "mc/slMain", slNewCycleData25);

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

