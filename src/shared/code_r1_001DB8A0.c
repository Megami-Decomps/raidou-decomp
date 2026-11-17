#include "common.h"
#include "sdfSound.h"

#include "fileManager.h"
#include "newdata.h"
#include "sdk_libc.h"
#include "sdk_libkernl.h"
#include "include_asm.h"
#include "ps2asm.h"
#include "sdf_resource.h"
#include "billboard.h"
#include "sdf_types.h"
#include "draw.h"
#include "libsdf.h"
#include "sdf_movie_types.h"
#include "fpu.h"
#include "basic.h"
#include "evt_world.h"
#include "code_r1_0021CE60.h"
#include "slMain.h"
#include "code_r1_001CC1D0.h"
#include "cfg.h"

/* Apply volume and pan to a sequence track through the command queue. */

/* Both entry points return one caller-owned texture reference. A nonzero
 * resourceKey reuses a live matching texture and increments its count; a new
 * texture starts at one. Release the returned reference once with
 * sdfTexReleaseReference or sdfTexFreeDelayed. */

#define SDK_SKIP_memset

/* Save-slot selection motion: a one-tick-at-a-time scroll advances or retreats
 * the selected slot while the visible rows slide in the opposite direction. */
typedef s8 FileLoadMenuSlideMode;

enum {
    FILE_LOAD_MENU_SLIDE_IDLE = 0,
    FILE_LOAD_MENU_SLIDE_SELECTION_DOWN = 1,
    FILE_LOAD_MENU_SLIDE_SELECTION_UP = 2
};

typedef struct fileLoadMenuState_t {
    u8 unk00[4];              /* 0x00: opaque */
    s32 transitionTickCount;  /* 0x04 */
    s32 inputRepeatTimer;     /* 0x08 */
    s32 standardRowAlpha;     /* 0x0C: clamped to 0..0x80 for ordinary rows */
    FileLoadMenuSlideMode slideMode; /* 0x10: idle or selection-scroll direction */
    u8 pad11[3];              /* 0x11 */
    s32 rowVerticalOffset;    /* 0x14 */
    s32 leadingRowAlpha;      /* 0x18 */
    s32 trailingRowAlpha;     /* 0x1C */
} fileLoadMenuState_t;

typedef char fileLoadMenuState_size_must_be_0x20[
    (sizeof(fileLoadMenuState_t) == 0x20) ? 1 : -1];

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

extern u32 fileClampPackedColorByte(u32, s32, s8);
extern void fileCursorStepUp(void);
extern void fileFadeStepDown(void);

struct sdfMemHandle_t;

typedef struct fileLoadMirror_t {
    u32 current;
    u32 previous;
} fileLoadMirror_t;

/* Loader context at fileLoadMenuState. */
extern fileLoadMenuState_t fileLoadMenuState;

/* Far scalar: incomplete array forces non-small-data addressing. */
#ifdef VERSION_RAIDOU1
extern s32 slNewCycleData2[];
#else
extern s32 slNewCycleWork3[];
#endif
extern void fileSetMenuValueAndInitializeFlags(u32 value);
extern void fileLoadSetMode(FileLoadMenuSlideMode mode);
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

void fileSetMenuValueAndInitializeFlags(u32 value) {
#ifdef VERSION_RAIDOU1
    slNewCycleData2[0] = value;
#else
    slNewCycleWork3[0] = value;
#endif
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
#ifdef VERSION_RAIDOU1
    slNewCycleData2[0] -= 0x10;
    slNewCycleData2[0] = slNewCycleData2[0] <= 0 ? 0 : slNewCycleData2[0] > 0x80 ? 0x80 : slNewCycleData2[0];
#else
    slNewCycleWork3[0] -= 0x10;
    slNewCycleWork3[0] = slNewCycleWork3[0] <= 0 ? 0 : slNewCycleWork3[0] > 0x80 ? 0x80 : slNewCycleWork3[0];
#endif
}

INCLUDE_ASM(const s32, "code_r1_001DB8A0", cfgCallSetOt);

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

INCLUDE_ASM(const s32, "code_r1_001DB8A0", cfgLeaf2);

INCLUDE_ASM(const s32, "code_r1_001DB8A0", cfgRetuenFromLoadSub);

void cfgCopyFlagGblToBak(void) {
    cfgWork = DDS3_GBWK->configFlag;
    cfgData = 0;
}

void cfgCopyFlagBakToGbl(void) {
    DDS3_GBWK->configFlag = cfgWork;
    cfgData = 0;
}

INCLUDE_ASM(const s32, "code_r1_001DB8A0", cfgTitleConfigCalcProcessSub);

s32 dds3ToggleConfigFlagSub(s32 type, u32 *flag) {
    switch (type) {
    case 0:
        return (*flag ^ 1) & 1;
    }
    return -1;
}

INCLUDE_ASM(const s32, "code_r1_001DB8A0", dds3ToggleConfigFlag);

s32 dds3RefDDS3GBWK(s32 type) {
    return dds3ToggleConfigFlagSub(type, &DDS3_GBWK->configFlag);
}

INCLUDE_RODATA(const s32, "code_r1_001DB8A0", cfgCallSetOtTable);

INCLUDE_RODATA(const s32, "code_r1_001DB8A0", cfgStrEffectSmoke02Tmx);

INCLUDE_RODATA(const s32, "code_r1_001DB8A0", cfgStrEffectFire00Tmx);

INCLUDE_RODATA(const s32, "code_r1_001DB8A0", cfgStrEffectFloorBloodTmx);

INCLUDE_RODATA(const s32, "code_r1_001DB8A0", cfgStrEffectParticle00Tmx);

INCLUDE_RODATA(const s32, "code_r1_001DB8A0", cfgStrEffectTisibukiTmx);

INCLUDE_RODATA(const s32, "code_r1_001DB8A0", cfgStrEffectExpload04Tmx);

INCLUDE_RODATA(const s32, "code_r1_001DB8A0", cfgStrEffectFlare07Tmx);

INCLUDE_RODATA(const s32, "code_r1_001DB8A0", cfgStrEffectFlare06Tmx);

INCLUDE_RODATA(const s32, "code_r1_001DB8A0", cfgStrEffectFlare05Tmx);

INCLUDE_RODATA(const s32, "code_r1_001DB8A0", cfgStrEffectFlare04Tmx);

INCLUDE_RODATA(const s32, "code_r1_001DB8A0", cfgStrEffectFlare03Tmx);

INCLUDE_RODATA(const s32, "code_r1_001DB8A0", cfgStrEffectFlare02Tmx);

INCLUDE_RODATA(const s32, "code_r1_001DB8A0", cfgStrEffectFlare01Tmx);

INCLUDE_RODATA(const s32, "code_r1_001DB8A0", cfgStrEffectGene7Tmx);

INCLUDE_RODATA(const s32, "code_r1_001DB8A0", cfgStrEffectGene6Tmx);

INCLUDE_RODATA(const s32, "code_r1_001DB8A0", cfgStrEffectGene5Tmx);

INCLUDE_RODATA(const s32, "code_r1_001DB8A0", cfgStrEffectGene4Tmx);

INCLUDE_RODATA(const s32, "code_r1_001DB8A0", cfgStrEffectGene3Tmx);

INCLUDE_RODATA(const s32, "code_r1_001DB8A0", cfgStrEffectGene2Tmx);

INCLUDE_RODATA(const s32, "code_r1_001DB8A0", cfgStrEffectGene1Tmx);

INCLUDE_RODATA(const s32, "code_r1_001DB8A0", cfgStrEffectGene0Tmx);

INCLUDE_SDATA(const s32, "code_r1_001DB8A0", cfgData);

