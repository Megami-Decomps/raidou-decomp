#include "common.h"
#include "sdfSound.h"
#include "effPCPMisc_00224D10.h"

#include "fileManager.h"
#include "newdata.h"
#include "sdf_resource.h"
#include "sdk_libc.h"
#include "sdk_libkernl.h"
#include "include_asm.h"
#include "billboard.h"
#include "sdf_types.h"
#include "draw.h"
#include "libsdf.h"
#include "sdf_movie_types.h"
#include "effect.h"
#include "ps2asm.h"
#include "basic.h"
#include "evt_world.h"
#include "fpu.h"
#include "sdf_lighting.h"
#include "model.h"
#include "motion.h"
#include "code_r1_001CC1D0.h"
#include "code_r1_0021CE60.h"
#include "code_r1_0021C768.h"
#include "sdfChip.h"
#include "slMain.h"
#include "mdlManager.h"

/* Apply volume and pan to a sequence track through the command queue. */

/* Both entry points return one caller-owned texture reference. A nonzero
 * resourceKey reuses a live matching texture and increments its count; a new
 * texture starts at one. Release the returned reference once with
 * sdfTexReleaseReference or sdfTexFreeDelayed. */

#define SDK_SKIP_memset

typedef s32 (*FileMenuContinuationCallback)(void);

struct sprHandle_t;

enum { FILE_CONFIG_SET, FILE_CONFIG_FRAME, FILE_CONFIG_X, FILE_CONFIG_Y };

typedef struct mnuWork_t {
    fileSavePreviewRecord_t header;
    s8 applyLoadResult;
    s8 hardInitSelected;
    u16 flowConditionMask;
    u16 startBranchFlag; /* 0x34: selects one of two initial menu flows */
    u16 flowRowCursor;
    u16 unk38;
    u16 unk3A;
    struct sdfMemHandle_t *unk3C;
} mnuWork_t;

typedef struct mdlFlagPair_t {
    s32 flag;
    s32 unk4;
} mdlFlagPair_t;

/* Eight-byte story-flag rows select a unit and a six-byte bonus record. */
typedef struct fileStatBonus_t {
    s32 flag;
    u16 unitId;
    u8 bonusIndex;
    u8 pad07;
} fileStatBonus_t;

struct mnuListNode_t;

struct sdfMemHandle_t;

extern u32 D_00437D18;
extern u32 D_00437D1C;
extern u32 D_00437D20;
extern s8 dds3PadButtonState[];

/* Effect parameter-set dispatch tables. Every effect kind owns one 0x28-byte
 * entry per table; the handler lives at +0x0. Slots are declared as separate
 * arrays (D_00353710/14/18/1C/20/24/28/2C/30/34 and D_00353880/84/88/90/94/
 * 98/9C/A0/A4). The family2 create table (D_00353880) additionally carries a
 * fallback selector at +0x0C: nonzero calls the entry handler directly,
 * zero falls back through D_003536A0.
 */
/* 8-byte parameter work (family1): kind id plus one data pointer. */
typedef struct effDispatchExtra_t {
    void (*func)(void *, f32 vector[4]); /* 0x00 vector handler, may be NULL */
    u8 pad04[0x24];
} effDispatchExtra_t;

typedef struct effDispatchScale_t {
    void (*func)(void *, f32); /* 0x00 handler, may be NULL */
    u8 pad04[0x24];
} effDispatchScale_t;

typedef struct effDispatchColor_t {
    void (*func)(void *, u32); /* 0x00 handler, may be NULL */
    u8 pad04[0x24];
} effDispatchColor_t;

typedef struct fileLoadMirror_t {
    u32 current;
    u32 previous;
} fileLoadMirror_t;

#define FILE_MAIN_BLOB_SIZE 0x1E840

u8 fileIsLoadedAndConditionTrue(s32 condition);
s32 slChkWindowOpenDone(void);

struct sdfMemHandle_t;

extern FileSelectionCallback D_00439024, D_00439028;

/* Yes/no cursor input while the card request is idle. */
static inline void filePollPromptChoice(void) {
    if (D_00437D20 == 0 && slChkWindowOpenDone() != 0) {
        if (fileIsLoadedAndConditionTrue(dds3PadButtonState[0x27] < 0) != 0) {
            if (D_00437D18 != 1) {
                p2sdPlaySE(0, 0x7F, 0x3F);
            }
            D_00437D18 = 1;
        } else if (fileIsLoadedAndConditionTrue(dds3PadButtonState[0x26] < 0) != 0) {
            if (D_00437D18 != 0) {
                p2sdPlaySE(0, 0x7F, 0x3F);
            }
            D_00437D18 = 0;
        } else if (fileIsLoadedAndConditionTrue(dds3PadButtonState[0x21] < 0) != 0) {
            if (D_00437D18 == 0 && (D_00437D1C == 1 || D_00437D1C == 3)) {
                p2sdPlaySE(0x11, 0x7F, 0x3F);
            } else {
                p2sdPlaySE(8, 0x7F, 0x3F);
            }
            if (D_00437D18 != 0) {
                D_00437D20 = 2;
            } else {
                D_00437D20 = 1;
            }
        } else if (fileIsLoadedAndConditionTrue(dds3PadButtonState[0x23] < 0) != 0) {
            p2sdPlaySE(0xA, 0x7F, 0x3F);
            D_00437D20 = 2;
        }
    }
}

/* Lower the selected low/alpha byte, retaining the native signed, unmasked bound. */

/* Init record at fileCursorPulseState. */
typedef struct fileInit374E0_t {
    s8 mode[3];      /* 0x00 */
    u8 pad3;         /* 0x03 */
    s32 pos[3][2];   /* 0x04 */
    s32 counter[3];  /* 0x1C */
    u8 pad28[0xC];   /* 0x28 */
    s32 alpha[3];    /* 0x34 */
} fileInit374E0_t;

typedef struct fileFlowEntry_t {
    u16 state;
    u16 mask;
} fileFlowEntry_t;

typedef struct fileMultiFlagBonus_t {
    s32 flags[4];
    u8 bonusIndex;
    u8 pad11[3];
} fileMultiFlagBonus_t;

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
extern void mnuRecordSetVector(void *record, const void *src);
extern void fileSetRecordSecondVector(void *record, const void *src);

extern void fileReadRecordSecondVector(void *, void *);
extern void fileReadVectorPtr20(void *, void *);

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

typedef struct fileCurveMotion_t {
    f32 firstControl[3];
    f32 secondControl[3];
    f32 rotationReference[3]; /* native live-path input; writer unresolved */
    f32 endpointOffset[3];
    f32 angularSpeed;
    f32 travelSpeed;
    f32 pathLength;
    f32 progress;
    f32 scaleMultiplier;
    f32 angle;
    f32 angleMultiplier;
} fileCurveMotion_t; /* 0x4C, naturally four-byte aligned */

void fileReadVectorPtr20(void *recordBytes, void *dst);
void mnuRecordSetVector(void *recordBytes, const void *src);
void fileReadRecordSecondVector(void *recordBytes, void *dst);
void fileSetRecordSecondVector(void *recordBytes, const void *src);

#define MDL_SKIP_TRANSFORMS 1

enum {
    SDF_PSMCT32 = 0x00,
    SDF_PSMCT24 = 0x01,
    SDF_PSMCT16 = 0x02,
    SDF_PSMCT16S = 0x0A,
    SDF_PSMT8 = 0x13,
    SDF_PSMT4 = 0x14,
    SDF_PSMT8H = 0x1B,
};

/* Model-context lifetime, per-frame update, and broadcast accessors. */
/* Destroy one context. If it leaves its resource-owning group empty, group
 * teardown also releases that group's registered assets. */
void mdlFree(mdlObj_t *ctx);

/* Allocate a distinct caller-owned context linked to the registered group.
 * Contexts share the group node and assets; release each context with mdlFree. */
mdlObj_t *mdlLoad(u32 group, u32 id);
void mdlSetColor(mdlObj_t *ctx, u32 value);
void mdlSetPosition(mdlObj_t *ctx);
void mdlSetRotate(mdlObj_t *ctx);
void mdlSetScale(mdlObj_t *ctx);

/* sdfMotionCtrlr_t-record selection and lookup. */
void mdlSingleMotion(mdlObj_t *ctx, s32 searchId, s32 motionIndex);

void Init_effModel(mdlObj_t *model);
void effDestroyModelContext(mdlObj_t *owner);
mdlObj_t *effHandleCopy(mdlObj_t *sourceModel);

/* Battle state: resource headers for texture slots 1 and 2. */
typedef struct effBattleTexHeaders_t {
    u8 pad_000[0x5B0];
    u8 *slot1; // 0x5B0
    u8 *slot2; // 0x5B4
} effBattleTexHeaders_t;

void effPCPHelperBEDGetModel(bhvrHandle_t *record) {
    sdfMemFreeDirect(record->allocation);
}

INCLUDE_ASM(const s32, "effect/effPCPMisc_00224D10", effCalc);

void fileAcquireRecord(bhvrHandle_t *record) {
    effEmitterTypeDescriptors[record->type].updateSlots(record);
    record->references++;
}

void fileReadVectorPtr20(void *recordBytes, void *dst) {
    bhvrHandle_t *record = (bhvrHandle_t *)recordBytes;
    SDF_VU0_COPYVECTOR(dst, record->data0);
}

void mnuRecordSetVector(void *recordBytes, const void *src) {
    bhvrHandle_t *record = (bhvrHandle_t *)recordBytes;
    SDF_VU0_COPYVECTOR(record->data0, src);
}

void fileReadRecordSecondVector(void *recordBytes, void *dst) {
    bhvrHandle_t *record = (bhvrHandle_t *)recordBytes;
    SDF_VU0_COPYVECTOR(dst, record->data0 + 0x10);
}

void fileSetRecordSecondVector(void *recordBytes, const void *src) {
    bhvrHandle_t *record = (bhvrHandle_t *)recordBytes;
    SDF_VU0_COPYVECTOR(record->data0 + 0x10, src);
}

/* Persona 4 func_00492e30 @ 00492E30 (src/promoted/code1_0049.c), recompiled unchanged */
void dds3DispatchIndexedCallback(u16 *record, f32 scale) {
    effEmitterTypeDescriptors[*record].scaleParameters(record, scale);
}

/* Initialize the VU transforms and the first node's float slot, if present. */
void Init_effModel(mdlObj_t *model) {
    SDF_VU0_LOADIDENTITYVECTOR();
    mdlSetPosition(model);
    SDF_VU0_LOADIDENTITYVECTOR();
    mdlSetRotate(model);
    SDF_VU0_SETSCALE1VECTOR();
    mdlSetScale(model);
    mdlSetColor(model, 0x80808080);
    if (model->first != NULL) {
        mdlSingleMotion(model, 0, 0);
        model->first->speed = 1.0f;
    }
    model->flags &= ~MDL_SKIP_TRANSFORMS;
}

mdlObj_t *effHandleCreate(const u8 *data, s32 size) {
    mdlObj_t *model;

    while (mdlSearchResrc(5, effHandleCreateData) != NULL) {
        effHandleCreateData++;
    }
    mdlPackDecode(5, effHandleCreateData, 0x101, data, size);
    model = mdlLoad(5, effHandleCreateData);
    Init_effModel(model);
    effHandleCreateData++;
    return model;
}

/* Clear the lighting attachment before destroying its model context. */
void effDestroyModelContext(mdlObj_t *owner) {
    owner->inner->light = NULL;
    mdlFree(owner);
}

mdlObj_t *effHandleCopy(mdlObj_t *sourceModel) {
    s32 group;
    s32 id;
    mdlObj_t *model;

    group = mdlGetMajorNum(sourceModel);
    id = mdlGetMinorNum(sourceModel);
    model = mdlLoad(group, id);
    Init_effModel(model);
    return model;
}

effModelOwner_t *effCreateModelOwner(fileJobPayload_t *source) {
    effModelOwner_t *owner = sdfChipAllocZeroDirect(sizeof(effModelOwner_t));

    owner->ownedBuffer = sdfChipAllocZeroDirect(0xE0);
    if (source != NULL) {
        u32 scaleBits;
        void *data;

        scaleBits = *(u32 *)fileResolvePrimaryBuffer(source);
        memcpy(&owner->scale, &scaleBits, sizeof(owner->scale));
        data = fileResolveSecondaryBuffer(source);
        if (data != NULL) {
            owner->model = effHandleCreate(data, source->secondary.size);
            SDF_VU0_SETSCALE1VECTOR();
            VU0_SCALE_VF_MFC1(vf10, owner->scale);
            mdlSetScale(owner->model);
        }
    }
    owner->unk10 = 0;
    return owner;
}

void Delete(effModelOwner_t *owner) {
    sdf3DLightParallelPacket_t *buffer = owner->ownedBuffer;
    if (buffer != NULL) {
        sdfChipFreeDirect(buffer);
    }
    if (owner->model != 0) {
        effDestroyModelContext(owner->model);
    }
    sdfChipFreeDirect(owner);
}

effModelOwner_t *effPCPAnalyzeBEDCopy(effModelOwner_t *source) {
    effModelOwner_t *owner = effCreateModelOwner(NULL);

    memcpy(&owner->scale, &source->scale, sizeof(owner->scale));
    effRecreateModelFromSource(owner, source);
    return owner;
}

INCLUDE_SDATA(const s32, "effect/effPCPMisc_00224D10", effHandleCreateData);

