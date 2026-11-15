#include "common.h"

#ifndef VERSION_DDS2
#define VERSION_DDS2 1
#endif

#include "fileManager.h"
#include "newdata.h"
#include "sdf_resource.h"
#include "sdk_libc.h"
#include "sdk_libkernl.h"
#include "dds/bill_object_api.h"
#include "dds/btl.h"
#include "dds/btl_stage_task_cleanup.h"
#include "dds/dds3_fade.h"
#include "dds/dds3_task_create.h"
#include "dds/dds3_task_lifecycle.h"
#include "dds/dds3_world_primary.h"
#include "dds/eff_random.h"
#include "dds/evt_world.h"
#include "dds/file_request_api.h"
#include "dds/file_request_entry.h"
#include "dds/file_slot_flags.h"
#include "dds/fr_font.h"
#include "dds/fr_font_context.h"
#include "dds/dds3_process.h"
#include "dds/mc_path_api.h"
#include "dds/mc_poll.h"
#include "dds/mc_read_api.h"
#include "dds/mc_write_api.h"
#include "dds/mdl_flag_api.h"
#include "dds/sdf.h"
#include "dds/sdf_chip.h"
#include "dds/sdf_dev_state.h"
#include "dds/sdf_heap_api.h"
#include "dds/sdf_quaternion.h"
#include "dds/sdf_texture_file.h"
#include "dds/sdf_trig.h"
#include "dds/sdf_vu_matrix.h"
#include "dds/sdf_vu_matrix_multiply_api.h"
#include "dds/sdf_vu_transform_api.h"
#include "dds/sdk_libc.h"
#include "dds/sdk_libkernl.h"
#include "dds/sdk_libm.h"
#include "dds/snd_sequence_volume_pan.h"

extern void *memcpy(void *, const void *, unsigned int);

/* Apply volume and pan to a sequence track through the command queue. */
void p2sdPlaySE(s32 trackId, s32 volume, s32 pan);

/* Both entry points return one caller-owned texture reference. A nonzero
 * resourceKey reuses a live matching texture and increments its count; a new
 * texture starts at one. Release the returned reference once with
 * sdfTexReleaseReference or sdfTexFreeDelayed. */

#define SDK_SKIP_memset

#include "dds/sdk_ps2.h"
#include "dds/file_menu_start_mode.h"
#include "dds/eff_surface_node.h"
#include "dds/common.h"
#include "dds/pty_active_roster_index_api.h"
#include "dds/file_menu_state_result.h"
#include "dds/file_config_list_node.h"
#include "dds/file_config_option_api.h"
#include "dds/sdf_vu0.h"
#include "dds/file_job_type.h"
#include "dds/file_config_task.h"
#include "dds/eff_emitter_type.h"
#include "dds/file_load_menu_state.h"
#include "dds/eff.h"
#include "dds/cam_aim_record.h"
#include "dds/eff_generated_texture_descriptor.h"
#include "dds/dds3_world_selection.h"
#include "dds/eff_scale_params.h"
#include "dds/ee_mmi.h"
#include "dds/sdf_textured_rect.h"
#include "dds/itf_mes_window.h"
#include "dds/eff_resource_slots.h"
#include "dds/eff_resource_source_lifetime.h"
#include "dds/eff_update_flags.h"
#include "dds/eff_curve.h"
#include "dds/file.h"
#include "dds/file_selection_api.h"
#include "dds/file_prompt_reveal_result.h"
#include "dds/file_slot_flags_change_api.h"
#include "dds/file_scroll_arrow_state.h"
#include "dds/file_slot.h"
#include "dds/file_save_record.h"
#include "dds/dat_state.h"
#include "dds/dat_new_cycle_state.h"
#include "dds/mnu_list.h"
#include "dds/pcp_vu0.h"
#include "dds/eff_expanded_list.h"
#include "dds/fr_font_draw_glyph.h"
#include "dds/itf.h"

typedef s32 (*FileMenuContinuationCallback)(void);

struct EffectSlotSet;

enum { FILE_CONFIG_SET, FILE_CONFIG_FRAME, FILE_CONFIG_X, FILE_CONFIG_Y };

typedef struct MenuWork {
    FileSavePreviewRecord header;
    s8 applyLoadResult;
    s8 hardInitSelected;
    u16 flowConditionMask;
    u16 startBranchFlag; /* 0x34: selects one of two initial menu flows */
    u16 flowRowCursor;
    u16 unk38;
    u16 unk3A;
    struct sdfMemHandle_t *unk3C;
} MenuWork;

typedef struct MdlFlagPair {
    s32 flag;
    s32 unk4;
} MdlFlagPair;

/* Eight-byte story-flag rows select a unit and a six-byte bonus record. */
typedef struct FileStatBonus {
    s32 flag;
    u16 unitId;
    u8 bonusIndex;
    u8 pad07;
} FileStatBonus;

#include "dds/fpu.h"

struct MenuListNode;

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
typedef struct EffDispatchExtra {
    void (*func)(void *, f32 vector[4]); /* 0x00 vector handler, may be NULL */
    u8 pad04[0x24];
} EffDispatchExtra;

typedef struct EffDispatchScale {
    void (*func)(void *, f32); /* 0x00 handler, may be NULL */
    u8 pad04[0x24];
} EffDispatchScale;

typedef struct EffDispatchColor {
    void (*func)(void *, u32); /* 0x00 handler, may be NULL */
    u8 pad04[0x24];
} EffDispatchColor;

typedef struct LoadMirror {
    u32 current;
    u32 previous;
} LoadMirror;

#define FILE_MAIN_BLOB_SIZE 0x1E840

u8 fileIsLoadedAndConditionTrue(s32 condition);
s32 slChkWindowOpenDone(void);

#include "dds/file_pac.h"

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
typedef struct Init374E0 {
    s8 mode[3];      /* 0x00 */
    u8 pad3;         /* 0x03 */
    s32 pos[3][2];   /* 0x04 */
    s32 counter[3];  /* 0x1C */
    u8 pad28[0xC];   /* 0x28 */
    s32 alpha[3];    /* 0x34 */
} Init374E0;

#include "dds/sdf_debug_api.h"

typedef struct FileFlowEntry {
    u16 state;
    u16 mask;
} FileFlowEntry;

typedef struct FileMultiFlagBonus {
    s32 flags[4];
    u8 bonusIndex;
    u8 pad11[3];
} FileMultiFlagBonus;

#define FILE_JOB_LOCATION_AS_JOB_RELATIVE_OFFSET(payload) ((payload).offset)

#define FILE_JOB_LOCATION_AS_LOCKED_ADDRESS_WORD(payload) ((payload).offset)

#define FILE_JOB_LOCATION_CLEAR(payload) ((payload).offset = 0)

struct CamAim;

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
extern void mnuRecordSetVector(void *record, const void *src);
extern void fileSetRecordSecondVector(void *record, const void *src);

#include "dds/eff_anim.h"

extern void fileReadRecordSecondVector(u8 *, void *);
extern void fileReadVectorPtr20(u8 *, void *);

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

typedef struct FileCurveMotion {
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
} FileCurveMotion; /* 0x4C, naturally four-byte aligned */

void fileReadVectorPtr20(u8 *recordBytes, void *dst);
void mnuRecordSetVector(void *recordBytes, const void *src);
void fileReadRecordSecondVector(u8 *recordBytes, void *dst);
void fileSetRecordSecondVector(void *recordBytes, const void *src);

#ifndef VERSION_DDS2
#define VERSION_DDS2 1
#endif

void dds3DispatchIndexedCallback(u16 *record, f32 scale);

#ifndef VERSION_DDS2
#define VERSION_DDS2 1
#endif

#include "dds/btl_grouped_entity_api.h"
#include "dds/mdl.h"
#include "dds/sdf_chunk.h"
#include "dds/sdf_map_position_api.h"
#include "dds/sdf_motion.h"
#include "dds/sdf_packet_alloc.h"
#include "dds/sdf_packet_builders.h"
#include "dds/sdf_packet_list.h"
#include "dds/sdf_projection.h"
#include "dds/sdf_tex_create.h"
#include "dds/sdf_tex_packet.h"
#include "dds/sdf_texture_format.h"
#include "dds/sdf_texture_submit.h"

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
void mdlFree(dds3ModelHandle_t *ctx);
u16 mdlGetMajorNum(const dds3ModelHandle_t *ctx);
u16 mdlGetMinorNum(const dds3ModelHandle_t *ctx);

/* Allocate a distinct caller-owned context linked to the registered group.
 * Contexts share the group node and assets; release each context with mdlFree. */
dds3ModelHandle_t *mdlLoad(u32 group, u32 id);
void mdlSetColor(dds3ModelHandle_t *ctx, u32 value);
void mdlSetPosition(dds3ModelHandle_t *ctx);
void mdlSetRotate(dds3ModelHandle_t *ctx);
void mdlSetScale(dds3ModelHandle_t *ctx);

/* sdfMotionCtrlr_t-record selection and lookup. */
void mdlSingleMotion(dds3ModelHandle_t *ctx, s32 searchId, s32 motionIndex);

#include "dds/eff_texture_table.h"
#include "dds/eff_object_node.h"
#include "dds/eff_model_owner.h"
#include "dds/eff_ref_obj.h"
#include "dds/mdl_broadcast_callback.h"
#include "dds/eff_expanded_source.h"
#include "dds/btl_runtime.h"

void Init_effModel(dds3ModelHandle_t *model);
void effDestroyModelContext(dds3ModelHandle_t *owner);
dds3ModelHandle_t *effHandleCopy(dds3ModelHandle_t *sourceModel);

/* Battle state: resource headers for texture slots 1 and 2. */
typedef struct EffBattleTexHeaders {
    u8 pad_000[0x5B0];
    u8 *slot1; // 0x5B0
    u8 *slot2; // 0x5B4
} EffBattleTexHeaders;

#include "dds/eff_resource_owner.h"
#include "dds/sdf_vu_rotation_axes.h"

INCLUDE_ASM(const s32, "effect/effPCPMisc_00224D10", effPCPHelperBEDGetModel);

INCLUDE_ASM(const s32, "effect/effPCPMisc_00224D10", effCalc);

INCLUDE_ASM(const s32, "effect/effPCPMisc_00224D10", effCalcSub);

void fileReadVectorPtr20(u8 *recordBytes, void *dst) {
    FileSlotTable *record = (FileSlotTable *)recordBytes;
    SDF_VU0_COPYVECTOR(dst, record->data0);
}

void mnuRecordSetVector(void *recordBytes, const void *src) {
    FileSlotTable *record = (FileSlotTable *)recordBytes;
    SDF_VU0_COPYVECTOR(record->data0, src);
}

void fileReadRecordSecondVector(u8 *recordBytes, void *dst) {
    FileSlotTable *record = (FileSlotTable *)recordBytes;
    SDF_VU0_COPYVECTOR(dst, record->data0 + 0x10);
}

void fileSetRecordSecondVector(void *recordBytes, const void *src) {
    FileSlotTable *record = (FileSlotTable *)recordBytes;
    SDF_VU0_COPYVECTOR(record->data0 + 0x10, src);
}

/* Persona 4 func_00492e30 @ 00492E30 (src/promoted/code1_0049.c), recompiled unchanged */
void dds3DispatchIndexedCallback(u16 *record, f32 scale) {
    effEmitterTypeDescriptors[*record].scaleParameters(record, scale);
}

/* Initialize the VU transforms and the first node's float slot, if present. */
void Init_effModel(dds3ModelHandle_t *model) {
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

INCLUDE_ASM(const s32, "effect/effPCPMisc_00224D10", effHandleCreate);

/* Clear the lighting attachment before destroying its model context. */
void effDestroyModelContext(dds3ModelHandle_t *owner) {
    owner->inner->lighting = NULL;
    mdlFree(owner);
}

dds3ModelHandle_t *effHandleCopy(dds3ModelHandle_t *sourceModel) {
    s32 group;
    s32 id;
    dds3ModelHandle_t *model;

    group = mdlGetMajorNum(sourceModel);
    id = mdlGetMinorNum(sourceModel);
    model = mdlLoad(group, id);
    Init_effModel(model);
    return model;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc_00224D10", effCallChipAllocZeroDirect3);

INCLUDE_ASM(const s32, "effect/effPCPMisc_00224D10", Delete);

INCLUDE_ASM(const s32, "effect/effPCPMisc_00224D10", effPCPAnalyzeBEDCopy);

INCLUDE_SDATA(const s32, "effect/effPCPMisc_00224D10", effHandleCreateData);

