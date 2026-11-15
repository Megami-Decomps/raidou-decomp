#include "common.h"

#ifndef VERSION_DDS1
#define VERSION_DDS1 1
#endif

#include "fileManager.h"
#include "newdata.h"
#include "sdk_libc.h"
#include "sdk_libkernl.h"
#include "dds/btl.h"

extern void *memcpy(void *, const void *, unsigned int);

/* Apply volume and pan to a sequence track through the command queue. */
void p2sdPlaySE(s32 trackId, s32 volume, s32 pan);

/* Both entry points return one caller-owned texture reference. A nonzero
 * resourceKey reuses a live matching texture and increments its count; a new
 * texture starts at one. Release the returned reference once with
 * sdfTexReleaseReference or sdfTexFreeDelayed. */

#define SDK_SKIP_memset

#include "dds/file_load_menu_state.h"
#include "dds/ee_mmi.h"
#include "dds/file.h"
#include "dds/file_selection_api.h"
#include "dds/file_slot.h"
#include "dds/fpu.h"
#include "dds/mnu_list.h"

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

extern u32 fileClampPackedColorByte(u32, s32, s8);
extern void fileCursorStepUp(void);
extern void fileFadeStepDown(void);

struct sdfMemHandle_t;

typedef struct LoadMirror {
    u32 current;
    u32 previous;
} LoadMirror;

/* Loader context at fileLoadMenuState. */
extern FileLoadMenuState fileLoadMenuState;

/* Far scalar: incomplete array forces non-small-data addressing. */
extern s32 slNewCycleData2[];
extern void fileSetMenuValueAndInitializeFlags(u32 value);
extern void fileLoadSetMode(FileLoadMenuSlideMode mode);
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
s32 slChkWindowOpenDone(void);
extern void fileResetLoadContextSlide(void);

static inline void fileDrawDialogLine(s32 line, const char *glyphSource) {
    mcdCreateFontDrawHandle(86, 114 + line * 24, 0x89FEFF80U, glyphSource);
}

extern s8 dds3PadButtonState[];
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

void fileSetMenuValueAndInitializeFlags(u32 value);

/* Lower the selected low/alpha byte, retaining the native signed, unmasked bound. */
u32 fileClampPackedColorByte(u32 color, s32 limit, s8 alpha);
void fileCursorStepUp(void);
void fileFadeStepDown(void);
void fileLoadSetMode(FileLoadMenuSlideMode mode);
void fileResetLoadContextSlide(void);

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

void fileSetMenuValueAndInitializeFlags(u32 value) {
    slNewCycleData2[0] = value;
    fileLoadMenuState.standardRowAlpha = 0x80;
    fileLoadMenuState.transitionTickCount = 0;
}

/* Lower the selected low/alpha byte, retaining the native signed, unmasked bound. */
/* Keep the input word separate from the per-mode result; see docs/idioms.md,
 * "Trace-justified result selection: packed byte clamp". */
u32 fileClampPackedColorByte(u32 color, s32 limit, s8 alpha) {
    u32 result;
    s32 component = color & 0xFF;

    if (alpha == 0) {
        if (limit >= component) {
            result = color;
        } else {
            result = (color & 0xFFFFFF00) | limit;
        }
    } else {
        component = color >> 24;
        if (limit >= component) {
            result = color;
        } else {
            result = (color & 0xFFFFFF) | ((u32)limit << 24);
        }
    }
    return result;
}

void fileCursorStepUp(void) {
    fileLoadMenuState.transitionTickCount++;
    fileLoadMenuState.transitionTickCount = fileLoadMenuState.transitionTickCount <= 0 ? 0 : fileLoadMenuState.transitionTickCount > 12 ? 12 : fileLoadMenuState.transitionTickCount;
    if (fileLoadMenuState.transitionTickCount >= 7) {
        fileLoadMenuState.standardRowAlpha += 0x30;
        fileLoadMenuState.standardRowAlpha = fileLoadMenuState.standardRowAlpha <= 0 ? 0 : fileLoadMenuState.standardRowAlpha > 0x80 ? 0x80 : fileLoadMenuState.standardRowAlpha;
    }
}

void fileFadeStepDown(void) {
    slNewCycleData2[0] -= 0x10;
    slNewCycleData2[0] = slNewCycleData2[0] <= 0 ? 0 : slNewCycleData2[0] > 0x80 ? 0x80 : slNewCycleData2[0];
}

INCLUDE_ASM(const s32, "code_001DB8A0", cfgCallSetOt);

void fileLoadSetMode(FileLoadMenuSlideMode mode) {
    fileLoadMenuState.rowVerticalOffset = 0;
    fileLoadMenuState.slideMode = mode;
    if (mode == FILE_LOAD_MENU_SLIDE_SELECTION_DOWN) {
        fileLoadMenuState.trailingRowAlpha = 0;
        fileLoadMenuState.leadingRowAlpha = 0x74;
    } else {
        fileLoadMenuState.leadingRowAlpha = 0;
        fileLoadMenuState.trailingRowAlpha = 0x74;
    }
}

void fileResetLoadContextSlide(void) {
    fileLoadMenuState.rowVerticalOffset = 0;
    fileLoadMenuState.slideMode = FILE_LOAD_MENU_SLIDE_IDLE;
    fileLoadMenuState.transitionTickCount = 0;
}

INCLUDE_ASM(const s32, "code_001DB8A0", cfgLeaf2);

INCLUDE_ASM(const s32, "code_001DB8A0", cfgRetuenFromLoadSub);

INCLUDE_ASM(const s32, "code_001DB8A0", cfgCopyFlagGblToBak);

INCLUDE_ASM(const s32, "code_001DB8A0", cfgCopyFlagBakToGbl);

INCLUDE_ASM(const s32, "code_001DB8A0", cfgTitleConfigCalcProcessSub);

INCLUDE_ASM(const s32, "code_001DB8A0", dds3ToggleConfigFlagSub);

INCLUDE_ASM(const s32, "code_001DB8A0", dds3ToggleConfigFlag);

INCLUDE_ASM(const s32, "code_001DB8A0", dds3RefDDS3GBWK);

INCLUDE_RODATA(const s32, "code_001DB8A0", cfgCallSetOtTable);

INCLUDE_RODATA(const s32, "code_001DB8A0", cfgStrEffectSmoke02Tmx);

INCLUDE_RODATA(const s32, "code_001DB8A0", cfgStrEffectFire00Tmx);

INCLUDE_RODATA(const s32, "code_001DB8A0", cfgStrEffectFloorBloodTmx);

INCLUDE_RODATA(const s32, "code_001DB8A0", cfgStrEffectParticle00Tmx);

INCLUDE_RODATA(const s32, "code_001DB8A0", cfgStrEffectTisibukiTmx);

INCLUDE_RODATA(const s32, "code_001DB8A0", cfgStrEffectExpload04Tmx);

INCLUDE_RODATA(const s32, "code_001DB8A0", cfgStrEffectFlare07Tmx);

INCLUDE_RODATA(const s32, "code_001DB8A0", cfgStrEffectFlare06Tmx);

INCLUDE_RODATA(const s32, "code_001DB8A0", cfgStrEffectFlare05Tmx);

INCLUDE_RODATA(const s32, "code_001DB8A0", cfgStrEffectFlare04Tmx);

INCLUDE_RODATA(const s32, "code_001DB8A0", cfgStrEffectFlare03Tmx);

INCLUDE_RODATA(const s32, "code_001DB8A0", cfgStrEffectFlare02Tmx);

INCLUDE_RODATA(const s32, "code_001DB8A0", cfgStrEffectFlare01Tmx);

INCLUDE_RODATA(const s32, "code_001DB8A0", cfgStrEffectGene7Tmx);

INCLUDE_RODATA(const s32, "code_001DB8A0", cfgStrEffectGene6Tmx);

INCLUDE_RODATA(const s32, "code_001DB8A0", cfgStrEffectGene5Tmx);

INCLUDE_RODATA(const s32, "code_001DB8A0", cfgStrEffectGene4Tmx);

INCLUDE_RODATA(const s32, "code_001DB8A0", cfgStrEffectGene3Tmx);

INCLUDE_RODATA(const s32, "code_001DB8A0", cfgStrEffectGene2Tmx);

INCLUDE_RODATA(const s32, "code_001DB8A0", cfgStrEffectGene1Tmx);

INCLUDE_RODATA(const s32, "code_001DB8A0", cfgStrEffectGene0Tmx);

INCLUDE_SDATA(const s32, "code_001DB8A0", cfgData);

