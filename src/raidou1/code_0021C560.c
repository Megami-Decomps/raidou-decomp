#include "common.h"

#ifndef VERSION_DDS1
#define VERSION_DDS1 1
#endif

#include "fileManager.h"
#include "newdata.h"
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
#include "dds/sdf_texture_file.h"
#include "dds/sdf_trig.h"
#include "dds/sdf_vu_matrix.h"
#include "dds/sdf_vu_matrix_multiply_api.h"
#include "dds/sdf_vu_rotation_axes.h"
#include "dds/sdf_vu_transform_api.h"
#include "dds/sdk_libc.h"
#include "dds/sdk_libkernl.h"
#include "dds/sdk_libm.h"
#include "dds/snd_sequence_volume_pan.h"

/* Load matrix into the VU0 scratch bank and postmultiply the primary bank. */
void MultMatrixRev(const struct Matrix4 *matrix);
extern void *memcpy(void *, const void *, unsigned int);

/* Apply volume and pan to a sequence track through the command queue. */
void p2sdPlaySE(s32 trackId, s32 volume, s32 pan);

/* Both entry points return one caller-owned texture reference. A nonzero
 * resourceKey reuses a live matching texture and increments its count; a new
 * texture starts at one. Release the returned reference once with
 * sdfTexReleaseReference or sdfTexFreeDelayed. */

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
#include "dds/eff_scale_params.h"
#include "dds/ee_mmi.h"
#include "dds/sdf_textured_rect.h"
#include "dds/itf_mes_window.h"
#include "dds/eff_resource_slots.h"
#include "dds/eff_resource_source_lifetime.h"
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
#include "dds/pcp_vu0.h"
#include "dds/fpu.h"
#include "dds/mnu_list.h"
#include "dds/eff_expanded_list.h"
#include "dds/file_pac.h"
#include "dds/sdf_pac_work.h"
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

extern sdfGrOt_t *effPacketSubmissionSurfaces[6];
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

/* Lower the selected low/alpha byte, retaining the native signed, unmasked bound. */

sdfGrOt_t *effGetPacketSubmissionSurface(s32 index);
extern u8 sdfViewMatrix[];
extern u8 sdfProjectionMatrix[];
extern u8 dds3BillboardGetDirectionRotateData2[];
void mnuProjectViewPoint(void);

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

sdfGrOt_t *effGetPacketSubmissionSurface(s32 index) {
    return effPacketSubmissionSurfaces[index];
}

/* vu0 routine: project the point in vf10 through view/projection and screen scale/bias. */
void mnuProjectViewPoint(void) {
    u8 *matrix;
    VU0_LOAD_MATRIX(sdfViewMatrix);
    matrix = sdfProjectionMatrix;
    MultMatrixRev((const struct Matrix4 *)matrix);
    SDF_VU0_APPLYMATRIX_XYZ1();
    SDF_VU0_PERSVECTOR();
    matrix += 0x40;
    SDF_VU0_LOADVECTOR_B(matrix);
    SDF_VU0_MULVECTOR();
    SDF_VU0_LOADVECTOR_B(dds3BillboardGetDirectionRotateData2);
    SDF_VU0_ADDVECTOR();
}
