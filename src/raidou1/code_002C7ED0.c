#include "common.h"

/* Declarations from dds-decomp src/dds1/game/code_0029C530.c */
#ifndef VERSION_DDS1
#define VERSION_DDS1 1
#endif

#include "dds/sdk_ps2.h"

#include "dds/eff_block128.h"

#include "dds/eff_editor_menu.h"

#include "dds/eff_surface_grid_node.h"

#include "dds/eff_surface_node.h"

#include "dds/eff_animation_state.h"

#include "dds/eff_model_resource.h"

#include "dds/eff_gs_packet.h"

#include "dds/eff_packet_params.h"

#include "dds/eff_copied_payload.h"

#include "dds/eff_class_ops.h"

#include "dds/eff_particle_shared_resource.h"

#include "dds/eff_camera_create_request.h"

#include "dds/eff_file_job_request.h"

#include "dds/eff_emitter_row.h"

#include "dds/fld.h"

#include "dds/btl_command.h"

#include "dds/sdf_quaternion.h"

#include "dds/btl_motion_transform.h"

#include "dds/sdf_asset_state.h"

#include "dds/btl_resource_browser.h"

#include "dds/sdf_packet_list.h"

#include "dds/eff_bill.h"

#include "dds/eff_anim.h"

#include "dds/eff_bill_column_particle.h"

#include "dds/eff_ribbon_work.h"

#include "dds/eff_class_work_api.h"

#include "dds/eff_scaled_class_frame_rows.h"

#include "dds/fld_transfer_chunk.h"

#include "dds/eff_point_set.h"

#include "dds/eff_point_set_table.h"

#include "dds/eff_span_work.h"

#include "dds/common.h"

#include "dds/dds3_camera_update_flags.h"

#include "dds/sdf_vu0.h"

#include "dds/eff_emitter_type.h"

#include "dds/sdf_vu_rotation_axes.h"

#include "dds/file_pac.h"

#include "dds/eff_resource_bank_slot.h"

#include "dds/eff_matrix4.h"

#include "dds/eff_active_resource.h"

#include "dds/eff_frame_asset.h"

#include "dds/eff_actor_transition_config.h"

#include "dds/eff_object_node.h"

#include "dds/eff_random_mod.h"

#include "dds/sdf_trig.h"

#include "dds/eff_random.h"

#include "dds/dds3_fade.h"

#include "dds/btl_stage_task_cleanup.h"

#include "dds/dds3obj.h"

#include "dds/dds3_world_primary.h"

#include "dds/sdf_packet_append.h"

#include "dds/sdf_dev_state.h"

#include "dds/sdf_chip.h"

#include "dds/eff_ref_obj.h"

#include "dds/sdf_heap_api.h"

#include "dds/eff_resource_slots.h"

#include "dds/eff_resource_source_lifetime.h"

#include "dds/dds3_path.h"

#include "dds/eff_transform.h"

#include "dds/btl_state.h"

#include "dds/evt_unit.h"

#include "dds/evt_unit_color_transition.h"

#include "dds/eff_blur.h"

#include "dds/eff_event_draw.h"

#include "dds/eff_curve.h"

#include "dds/file.h"

#include "dds/file_slot.h"

#include "dds/ee_mmi.h"

#include "dds/pcp_vu0.h"

#include "dds/eff_queue.h"

#include "dds/btl_resource_name.h"

#include "dds/mdl.h"

#include "dds/sdf.h"

#include "dds/sdf_draw.h"

#include "dds/fpu.h"

#include "dds/eff.h"

#include "dds/eff_billboard_work.h"

#include "dds/eff_resource_list.h"

#include "dds/file_request_api.h"

#include "dds/sdf_texture_file.h"

#include "dds/sdf_vu_matrix.h"

#include "dds/sdf_vu_matrix_multiply_api.h"

#include "dds/dds3_fade_timing.h"

#include "dds/sdf_asset_packets.h"

#include "dds/sdf_chunk.h"

/* Declared before first use (were implicit int). */
extern void btlBeginSceneLightColorTransition(f32 *lightColor, f32 *ambientColor, s32 frames);

extern void btlInitTintTransitionDefault(u16 soundId);

extern void btlInitTintTransitionResource(u32 resource, u16 soundId);

extern void evtDestroySelectionState(void);

extern s32 evtSelStateCreate(s32 countLimit, s16 frames, s32 firstValue, s32 secondValue);

extern void fileReadRecordSecondVector(FileSlotTable *record, u128 *out);

extern void fileReadVectorPtr20(FileSlotTable *record, u128 *out);

extern void fileSetRecordSecondVector(FileSlotTable *record, const void *src);

extern void kwlnPadResetMotorLevelsAndOutput(void);

extern void mnuRecordSetVector(FileSlotTable *record, const void *src);

static inline void effSubmitSurfacePacket(sdfGrOt_t *surface, sdfDrawTag_t *list) {
    surface->regFunc(surface, list);
}

#include "dds/sdf_packet_alloc.h"

extern void *effPkt3DPrimMakePoly(sdf3DPrimPolyParm_t *);

extern u32 D_0037ECB0[];

extern sdfGrOt_t *D_0037ECF0[];

extern sdfGrOt_t *D_0037EE68[];

extern u32 D_0037EB30[];

extern sdfGrOt_t *D_0037EB78[];

extern sdfGrOt_t sdfGrOtTable[];

#include "dds/bill_object_api.h"

extern char D_003B39C8[];

extern char D_003B39E0[];

extern char D_003B3B88[];

extern char D_003B3BA0[];

extern f32 effComputeProjectedOffsetAngle(const void *position, const void *offset);

extern f32 dds3BillboardGetDirectionScale(u8 *, void *);

extern void dds3BillboardDraw(billHandle_t *);

extern u8 D_003B2B10[];

extern u32 effColorEditRgbMenuOpen;

extern u32 effColorEditSelectedKey;

extern u32 effColorEditPage;

extern u32 D_003BD158;

extern u32 D_003BD15C;

extern u32 D_003BD160;

extern u32 effColorEditSelectedAlphaKey;

extern struct BtlResourceEntryList *effResourceBankEntries;

extern struct BtlResourceDescriptor *effResourceBankDescriptor;

extern s32 effEditorCurrentMenu;

extern void *effQueuedFileObject;

extern u8 effPartsListExitRequested;

extern FileJobPayload *effTemporaryFileJob;

extern FileQueue *effAuxiliaryFileQueue;

extern FileQueue *effFileQueue;

extern void dds3PadVibrationSet(s32, u8, s32);

extern u8 *effAllocateTexturedStripWork(u32 count, u32 repeat);

extern u32 effScalyTextureHandle;

extern s32 effSharedStripReferenceCount;

extern RefObj *effSharedScalyStripResource;

extern u32 effWindTextureHandle;

extern struct sdfMemHandle_t *D_003BC984;

extern struct sdfMemHandle_t *D_003BC994;

extern u8 D_0038E000[];

extern u8 D_0038E570[];

extern u8 *D_0037EBF8[];

extern FileJobPayload *effQueuedFileHandle;

extern struct sdfDrawTag_t *effCurrentRenderPacket;

extern s32 effSharedRibbonReferenceCount;

extern u32 D_003BC990;

extern struct EffExpandedList *effCreateTextureAnimationSet(u32);

extern struct SdfTextureFileHeader *effFlashTextureHandles;

extern struct EffModelResource *effCreateModelResourceWithInlineData(u16, void *, void *, u32);

extern EffSurfaceGridNode *effCreateSurfaceGridNode(u32, u32);

extern void effFillSurfaceGridColorGradient(EffSurfaceGridNode *, u32 *);

extern void effReleaseSurfaceGridBuffers(EffSurfaceGridNode *);

extern s32 dds3BillboardGeneralNumber(void);

extern sdfMat_t *sdfMatNew(void);

extern sdf3DPrimPolyParm_t D_003DC9E0;

extern EffTrackSet *effCreateTrackSetWithSharedReferences(u32, u32, u32);

extern void effReleaseResourceRefs(EffTrackSet *);

extern void dds3EffectFilter(void *);

extern f32 mnuMeasureProjectedPerpendicularDistance(f32);

extern void effBlurStepScaleSlotsAndDraw(void *);

extern s32 effRequestResourceByMode(u32, u32, u32, void **);

extern u32 D_003BC950;

extern u32 D_003BC954;

typedef struct EffectObjectFlag {
    u32 state;
    u8 pad04[8];
    u32 flags;
} EffectObjectFlag;

extern EffectObjectNode *effFloorModelListHead;

extern u8 btlIsRuntimeAllocated(void);

void effDrawThreePointGroups(EffPointSet *, Matrix4 *);

extern u32 effPartsListScroll;

extern u32 D_0038F2FC[];

extern s32 D_003BC970[2];

extern RefObj *D_003BC978[2];

extern u32 effSharedTextureReferenceCount;

extern s32 D_003BC9B0[2];

extern s32 D_003BC9B8[2];

extern u8 D_003BC9C0[2];

extern u8 D_003BC9C8[2];

/* Native resource operations add cloning before the frame callbacks.
 * The final word is the copied payload size, not another callback.
 * Factories retain this unit's existing address-word interface. */
typedef struct EffResourceOps {
    void (*initialize)();      /* 0x00 */
    u32 (*createResource)();    /* 0x04 */
    void (*destroyResource)(); /* 0x08 */
    u32 (*cloneResource)(void *); /* 0x0C */
    void (*update)();          /* 0x10 */
    void (*draw)();            /* 0x14 */
    u32 payloadSize;           /* 0x18 */
} EffResourceOps;

/* Active frame callbacks receive the parent; destruction receives its child. */
typedef struct EffActiveInstanceOps {
    void (*initialize)(EffClassWork *); /* 0x00 */
    u32 (*createResource)(); /* 0x04 */
    void (*destroyResource)(void *); /* 0x08 */
    u32 (*cloneResource)(void *); /* 0x0C */
    void (*update)(EffClassWork *); /* 0x10 */
    void (*draw)(EffClassWork *); /* 0x14 */
    u32 payloadSize; /* 0x18 */
} EffActiveInstanceOps;

typedef char EffActiveInstanceOpsSizeCheck[(sizeof(EffActiveInstanceOps) == 0x1C) ? 1 : -1];

/* Class-resource callbacks operate on the parent class-work owner. */
typedef struct EffClassResourceOps {
    void (*initialize)(EffClassWork *); /* 0x00 */
    void *(*createResource)(EffBillPointConfig *); /* 0x04 */
    void (*destroyResource)(EffClassWork *); /* 0x08 */
    void (*update)(EffClassWork *); /* 0x0C */
    void (*draw)(EffClassWork *); /* 0x10 */
    u32 payloadSize; /* 0x14 */
} EffClassResourceOps;

typedef char EffClassResourceOps_size_must_be_0x18[
    (sizeof(EffClassResourceOps) == 0x18) ? 1 : -1];

/* Block-resource callbacks receive the owning class work, not the payload. */
typedef struct EffBlockOps {
    void (*initialize)(EffClassWork *); /* 0x00 */
    u32 (*createResource)(); /* 0x04: preserve the existing generic result ABI */
    void (*destroyResource)(EffClassWork *); /* 0x08 */
    u32 (*cloneResource)(void *); /* 0x0C: preserve the existing generic result ABI */
    void (*update)(EffClassWork *); /* 0x10 */
    void (*draw)(EffClassWork *); /* 0x14 */
    u32 payloadSize; /* 0x18 */
} EffBlockOps;

typedef char EffBlockOps_size_must_be_0x1C[
    (sizeof(EffBlockOps) == 0x1C) ? 1 : -1];

struct EffModelResource;

struct EffSpanConfig;

struct EffSpanTable;

/* This table has a single populated kind with span-resource callbacks. */
typedef struct EffModelResourceOps {
    void (*initialize)(struct EffModelResource *); /* 0x00 */
    struct EffSpanTable *(*createResource)(struct EffSpanConfig *, dds3ModelHandle_t *); /* 0x04 */
    void (*destroyResource)(struct EffSpanTable *); /* 0x08 */
    void (*update)(struct EffModelResource *); /* 0x0C */
    void (*draw)(struct EffModelResource *); /* 0x10 */
    u32 payloadSize; /* 0x14 */
} EffModelResourceOps;

typedef char EffModelResourceOps_size_must_be_0x18[
    (sizeof(EffModelResourceOps) == 0x18) ? 1 : -1];

extern EffActiveInstanceOps effActiveInstanceOperations[];

extern EffClassOps effClassWorkOperations[];

extern EffClassResourceOps effClassResourceWorkOperations[];

extern EffBlockOps effBlockResourceOperations[];

extern EffBlockOps effModelBlockOperations[];

extern EffResourceOps effRuntimeResourceOperations[];

extern EffModelResourceOps effModelResourceOperations[];

/* Shared work object for the effEditScalarTrack helpers (layout inferred). */
typedef struct BaObj {
    u8 pad_0x00[0x0C]; // 0x00
    u8 *x0C;           // 0x0C
} BaObj; // 0x10

extern s32 effEditScalarTrack(EffScalarTrack *, u32, void *, void *);

extern u8 D_0038F9D0[];

extern u8 D_0038FAD0[];

extern u8 D_0038F8E8[];

extern u8 D_0038FA20[];

extern u8 D_0038F938[];

extern struct BtlResourceNameRecord *effQueuedResourceNameRecord;

extern void effReleaseQueuedResourceName(void);

extern const char D_003BD088[];

extern const char D_003BD090[];

extern char D_003DF8D0[];

extern u8 D_0038F2B8[];

extern u8 effBattleOffset[];

extern u8 *D_003BD074;

extern void effDrawSurfaceNode(s32);

struct EffCameraCreateRow;

extern u32 effSelectCameraFileJobFromRows(const char *, const struct EffCameraCreateRow *, u32);

extern char D_003B3968[];

extern char D_003B3958[];

extern char D_003BCA40[];

extern char D_003BD050[];

extern char D_003B38C8[];

extern char D_003B3938[];

extern char D_003B3928[];

extern char D_003B3918[];

extern char D_003B3908[];

extern char D_003B38F8[];

extern char D_003B38E8[];

extern char D_003B38D8[];

extern char D_003B3888[];

extern char D_003BD000[];

extern u8 D_0038DE70[];

extern u8 D_0038DF50[];

extern u8 D_0038E0F0[];

extern u8 D_0038E1A0[];

extern u8 D_0038E280[];

extern u8 D_0038E240[];

extern u8 D_0038E310[];

extern u8 D_0038E450[];

extern u8 D_0038E500[];

extern u8 D_0038E620[];

extern u8 D_0038E6F0[];

extern u8 D_0038E7C0[];

extern u8 D_0038E800[];

extern u8 D_0038E9A0[];

extern u8 D_003DF840[];

extern u8 D_003DFA30[];

extern u8 D_003BD108;

extern u8 D_003BD109;

extern u8 D_003BD10A;

extern s32 effEditColorTrack(s32, s32, s32);

extern u8 D_003DE148[] __attribute__((aligned(4)));

/* Value holder with a float field and a u16 data pointer (layout inferred). */
typedef struct ValPtr44 {
    u8 pad_0x00[0x08]; // 0x00
    float f08;         // 0x08
    u8 pad_0x0C[0x38]; // 0x0C
    u16 *p44;          // 0x44
} ValPtr44; // 0x48

extern s32 effPollFileRecord(char *, s32);

struct EffKindWork;

/* Native kind operations include handle destruction and per-frame update;
 * zero-argument notifications retain the original unprototyped callback ABI. */
typedef struct EffKindDesc {
    void *(*create)(void *); // 0x00
    void (*destroy)(void *); // 0x04: release the created handle
    void (*update)();        // 0x08: advance work through its kind callback
    void (*initialize)(struct EffKindWork *, sdfTexHandle_t *); // 0x0C
    u32 payloadSize;         // 0x10: bytes copied after the work header
} EffKindDesc; // 0x14

/* Native kind work: 0x40-byte header, followed by the copied source payload.
 * Fade callbacks consume this same object: frame supplies their signed limit,
 * handle is the renderer output, and payload holds the curve configuration. */
typedef struct EffKindWork {
    f32 position[4]; /* 0x00: vector passed to projection routines */
    s32 mode;       // 0x10
    u32 color;      // 0x14
    f32 scale;      // 0x18
    s32 kind;       // 0x1C
    u32 frame;        // 0x20, incremented by the kind callback dispatcher
    void *handle;   // 0x24
    void *payload;  // 0x28
    u32 sourceKind; // 0x2C
    sdfTexHandle_t *target;     // 0x30
    u8 pad_0x34[0xC]; // 0x34
} EffKindWork; // 0x40

/* Source payload's kind selects the initial rendering mode. */
typedef struct EffKindSource {
    u8 pad_00[0x28];
    s32 modeKind;
} EffKindSource;

extern EffKindDesc D_0037E770[];

extern EffKindDesc D_0037E7E8[];

/* VU0 model helpers consume vf10 directly, matching the original macro-mode setup. */
extern void effDrawScalyTexturedStrip(EffStripWork *, void *);

extern u16 D_003BC944;

extern void effInitModelVUState(dds3ModelHandle_t *);

extern s32 effComputeLightDirectionVU(dds3ModelHandle_t *, sdf3DLightParallelPacket_t *);

extern char D_003B2AA0[];

void effDestroyModelContext(dds3ModelHandle_t *model);

dds3ModelHandle_t *effLoadViewerModelWithVUState(void *modelData, u32 modelDataSize);

typedef struct EffFadeHeaderWork {
    s32 frame;
    s32 payload[2];
} EffFadeHeaderWork;

typedef char EffFadeHeaderWorkSizeCheck[(sizeof(EffFadeHeaderWork) == 0x0C) ? 1 : -1];

EffFadeHeaderWork *effDuplicateSmallHeader(const void *source);

EffFadeHeaderWork *effCreateSmallHeaderFromFile(void *work);

void effReleaseFadeHeaderAllocation(EffFadeHeaderWork *allocation);

EffFadeHeaderWork *effCloneSmallHeaderFromWork(EffFadeHeaderWork *work);

void effFadeFrameReset(EffFadeHeaderWork *counter);

void effFadeFrameAdvance(EffFadeHeaderWork *counter);

EffFadeVectorWork *effCreateFadeVectorWork();

EffFadeVectorWork *effCreateFadeVectorFromFile(void *work);

void effFreeFadeVectorWork(EffFadeVectorWork *work);

EffFadeVectorWork *effCloneFadeVectorWork(EffFadeVectorWork *work);

void effResetFadeVectorFrame(EffFadeVectorWork *work);

void effCopyFadeWorkVector(f32 *dst, const f32 *src);

void effSetFadeVectorColor(EffFadeVectorWork *work, u32 value);

void effResetSelectionEntryBuffers(SdfFlagListWork *work);

SdfFlagListWork *effCreateSelectionFlagListFromWork(const SdfFlagListParams *work);

SdfFlagListWork *effCreateSelectionFlagListFromFile(void *file);

void effReleaseSelectionFlagList(SdfFlagListWork *work);

SdfFlagListWork *effCreateEmbeddedSelectionFlagList(SdfFlagListWork *work);

void effResetSelectionEntriesAndState(SdfFlagListWork *p);

void effUpdateSelectionEntryState(SdfFlagListWork *work);

void effDrawSelectionEntryVectors(SdfFlagListWork *work);

void effUpdateAndDrawSelectionEntries(SdfFlagListWork *work);

/* Set the selection/drop-list's packed modulation color. */
void effSetSelectionFlagListColor(SdfFlagListWork *work, u32 value);

typedef struct EffSelectionHeaderWork {
    s32 frame;
    s32 payload[4];
} EffSelectionHeaderWork;

typedef char EffSelectionHeaderWorkSizeCheck[(sizeof(EffSelectionHeaderWork) == 0x14) ? 1 : -1];

EffSelectionHeaderWork *effDuplicatePayloadHeader(const void *source);

EffSelectionHeaderWork *effCreateSelectionHeaderFromFile(void *work);

void effReleaseSelectionHeaderAllocation(EffSelectionHeaderWork *allocation);

EffSelectionHeaderWork *effCloneSelectionHeaderFromWork(EffSelectionHeaderWork *work);

void effSelectionFrameReset(EffSelectionHeaderWork *counter);

void effSelectionFrameAdvance(EffSelectionHeaderWork *counter);

typedef struct EffFadeConfig {
    /* Color and alpha tracks at 0x00/0x24, followed by three scalar
     * tracks at 0x34, 0x60 and 0x8C. The scalar extents differ. */
    SdfColorTrack blendA;   /* 0x00 */
    SdfAlphaTrack blendB2;  /* 0x24 */
    EffScalarCurve blendB;   /* 0x34 */
    u8 pad58[8];
    EffScalarTrack rateA;   /* 0x60 */
    EffScalarTrack rateB;   /* 0x8C */
    s32 duration;         /* 0xB8 */
    u8 padBC[4];
    dds3Effect_Filter_t out; /* 0xC0 */
} EffFadeConfig;

typedef struct EffRateConfig {
    /* Color and alpha tracks at 0x00/0x24, followed by three scalar
     * tracks at 0x34, 0x60 and 0x8C. The scalar extents differ. */
    SdfColorTrack blendA;   /* 0x00 */
    SdfAlphaTrack blendB2;  /* 0x24 */
    EffScalarCurve blendB;   /* 0x34 */
    u8 pad58[8];
    EffScalarTrack rateA;   /* 0x60 */
    EffScalarTrack rateB;   /* 0x8C */
    s32 duration;         /* 0xB8 */
    u8 fixedMode;         /* 0xBC */
    u8 padBD[3];
    dds3Effect_Blur_t out;       /* 0xC0 */
} EffRateConfig;

/* Draw through the configured duration; zero duration uses frame zero.
 * Blend the packed color and use percent-scaled curves for the output rates. */
void effUpdateFadeBlendA(EffKindWork *work);

void *effCreateFadeBlendWorkFromOutput(void *work);

void effReleaseFadeBlendWork(void *resourceHandle);

/* Draw the fade in fixed subpixel units, either centered or at the projected position.
 * Projected X/Y use sixteen units per pixel; output Y is doubled. */
void effUpdateProjectedBlurFadeRectangle(EffKindWork *work);

void effUpdateTarget(EffKindWork *work, sdfTexHandle_t *target);

void *effCreateFixedSlotBlurWorkFromFadeOutput(void *source);

void effReleaseFixedSlotBlurWork(EffBlurScatterWork *handle);

/* Draw the pixel-unit fade using the handle's output record and payload's curves.
 * Zero projected mode suppresses drawing; the origin and Y scaling differ from subpixels. */
void effUpdateFadeMapA(EffKindWork *work);

void effTextureReferenceRelease(EffKindWork *work, sdfTexHandle_t *target);

void *effCreateVariableSlotBlurWorkFromFadeOutput(void *source);

void effReleaseVariableSlotBlurWork(EffBlurScaleWork *work);

/* Draw the same pixel-unit fade into the wider renderer output record.
 * The native work header and duration gate remain shared with the other kind callbacks. */
void effUpdateFadeMapB(EffKindWork *work);

void effReplaceKindLinkedTarget(EffKindWork *work, sdfTexHandle_t *target);

/* Draw a framebuffer fade using raw curve rates rather than percent-scaled rates.
 * The generic kind work supplies its packed color, mode and frame limit. */
void effUpdateFadeBlendB(EffKindWork *work);

/* Draw the color-only fade from the copied payload's curves.
 * Its output uses fixed rectangle bounds and the kind work's packed color and mode. */
void effUpdateFadeBlendC(EffKindWork *work);

void *effCreateFadeColorWorkFromOutput(void *work);

void effReleaseFadeColorWork(EffResourceRectWork *work);

/* This projected fade also consumes EffKindWork: position, handle and payload. */
extern void effComputeBlurRectBounds(EffResourceRectWork *work);

/* Update the fade rectangle's projected extent, center and multiplied tint. */
void effUpdateProjectedFadeColor(EffKindWork *work);

void effReplaceLinkedKindWorkTarget(EffKindWork *work, sdfTexHandle_t *target);

EffKindWork *effAllocateKindWork(u16 kind, u8 *source);

EffKindWork *effCreateKindWorkFromFile(FileJobPayload *work);

void effReleaseLinkedTarget(EffKindWork *work);

EffKindWork *effCloneKindWork(EffKindWork *work);

void effKindWorkFrameReset(EffKindWork *work);

void effAdvanceKindWorkFrame(EffKindWork *work);

void effCopyKindWorkPosition(void *dst, void *src);

void effSetKindWorkColor(EffKindWork *work, u32 value);

void effSetKindWorkMatrixComponent(EffKindWork *work, float value);

/* Allocate work for the alternate kind table, copying its payload and creating its handle. */
EffKindWork *effAllocateAlternateKindWork(u16 kind, u8 *source);

EffKindWork *effCreateKindWorkFromFileB(FileJobPayload *work);

/* Release the alternate-kind handle, owned secondary reference, and work allocation. */
void effReleaseAlternateKindWork(EffKindWork *work);

/* Reallocate the source kind and payload; this path does not copy its secondary target. */
EffKindWork *effCloneAlternateKindWork(EffKindWork *work);

/* Reset the alternate kind-table work's frame counter. */
void effAlternateKindWorkFrameReset(EffKindWork *work);

/* Invoke the alternate kind-table update callback, then advance its frame counter. */
void effAlternateKindWorkFrameUpdate(EffKindWork *work);

/* Copy the leading vector of alternate kind-table work. */
void effCopyAlternateKindVector(void *dst, void *src);

/* Set the alternate kind-table work's packed color. */
void effSetAlternateKindColor(EffKindWork *work, u32 color);

/* Set the alternate kind-table work's scalar scale. */
void effSetAlternateKindScale(EffKindWork *work, float scale);

u8 *effCreateBillboardWork(FileJobPayload *source);

void effBillboardWorkRelease(EffBillboardWork *work);

void effReplaceBillboardClone(s32 dst, s32 src);

u8 *effDuplicateBillState(const u8 *source);

void effReplaceBillboardClone(s32 dst, s32 src);

void effBillboardEntryFrameReset(EffBillboardWork *work);

/* Update a live billboard frame's projected scale, rotation, and position before its callback. */
void effUpdateScaledBillboardFrame(EffBillboardWork *work);

void effCopyBillboardPosition(void *dst, void *src);

void effCopyBillboardOrientation(void *work, void *src);

void effSetBillboardColor(s32 work, u32 color);

/* Set the billboard's scalar scale; the leading vectors are not a Matrix4. */
void effSetBillboardMatrixComponent(EffBillboardWork *work, float value);

#include "dds/eff_frame_state.h"

void effClearBillFrames(u8 *work);

u8 *effCreateBillFrameNode(EffBillFrameConfig *config, u32 resource);

/* Release the frame node's shared tracks and backing allocation. */
void effReleaseBillFrameNode(s32 work);

typedef struct EffBillFrameEntry {
    s32 age;
    f32 spin;
    f32 angle;
    f32 radius;
    f32 innerWidth;
    f32 outerWidth;
} EffBillFrameEntry;

typedef char EffBillFrameEntrySizeCheck[(sizeof(EffBillFrameEntry) == 0x18) ? 1 : -1];

typedef char EffBillFrameEntry_age_offset_check[((u32)&((EffBillFrameEntry *)0)->age == 0x0) ? 1 : -1];

typedef char EffBillFrameEntry_spin_offset_check[((u32)&((EffBillFrameEntry *)0)->spin == 0x4) ? 1 : -1];

typedef char EffBillFrameEntry_angle_offset_check[((u32)&((EffBillFrameEntry *)0)->angle == 0x8) ? 1 : -1];

typedef char EffBillFrameEntry_radius_offset_check[((u32)&((EffBillFrameEntry *)0)->radius == 0xC) ? 1 : -1];

typedef char EffBillFrameEntry_innerWidth_offset_check[((u32)&((EffBillFrameEntry *)0)->innerWidth == 0x10) ? 1 : -1];

typedef char EffBillFrameEntry_outerWidth_offset_check[((u32)&((EffBillFrameEntry *)0)->outerWidth == 0x14) ? 1 : -1];

extern EffRandState effSharedRandomState[];

extern u8 effOnesVector[];

void billAdvanceFrameInstances(EffClassWork *work);

extern u8 effOnesVector[];

extern void effDrawTrackSet(EffTrackSet *, const Matrix4 *);

#include "dds/eff_bill_cell_draw_work.h"

void billUpdateFrameDrawColorAndTransform(BillCellDrawWork *work);

void billResetCellIndices(u8 *work);

u8 *billCreateCellNode(EffBillCellConfig *config, u32 resource);

void billReleaseCellNode(s32 work);

extern f32 billRingCosines[12];

extern f32 billRingSines[12];

/* Spawn and advance the kind-2 cell instances: each live cell moves out
 * along its heading and is drawn as a 13-vertex fan (center plus two
 * six-step rings of radii A and B), faded in and out over its lifetime. */
void billAdvanceCellInstances(EffClassWork *work);

void billUpdateCellDrawColorAndTransform(BillCellDrawWork *work);

void billResetParticleIndices(u8 *work);

u8 *billCreateParticleNode(EffBillParticleConfig *config, u32 resource);

void billReleaseParticleNode(s32 work);

void billUpdateParticleDrawColorAndTransform(BillCellDrawWork *work);

void effClearAnimatedFrames(u8 *work);

u8 *billAllocateAnimatedTransformEntries(EffBillAnimatedFrameConfig *config);

void effInitializeAlternatingTransformRows(u8 *node, u8 *config);

u8 *billCreateAnimatedTransform(EffBillAnimatedFrameConfig *config, u32 resource);

extern u8 *billAllocEmitterNode(u8 *);

extern u8 *billAllocQuadNode(u8 *);

extern EffTrackSet *effDuplicateResourceRefs(const EffTrackSet *);

extern void billInitializeEmitterRows(u8 *work, u8 *descriptor);

extern void billInitializeQuadRows(u8 *work, u8 *descriptor);

u8 *billCloneAnimatedTransform(u8 *work);

void billReleaseAlternatingTransformNode(s32 work);

/* Kind-4 animated frame entry: the radius grows by radiusStep each frame
 * for the config's drawProgress frames. */
typedef struct EffBillAnimatedEntry {
    s32 age;
    f32 spin;
    f32 angle;
    f32 radius;
    f32 radiusStep;
    f32 width;
} EffBillAnimatedEntry;

/* Spawn and advance the kind-4 animated frames: each live entry spins with
 * an accelerating rate, grows its radius over drawProgress frames and is
 * drawn as a four-vertex frame, faded in and out over its lifetime. */
void billAdvanceAnimatedFrameTransforms(EffClassWork *work);

void billUpdateAlternatingDrawColorAndTransform(BillCellDrawWork *work);

void billResetEmitterIndices(u8 *work);

u8 *billAllocEmitterNode(u8 *config);

void billInitializeEmitterRows(u8 *work, u8 *descriptor);

u8 *billCreateEmitterTransform(EffBillEmitterFrameConfig *config, u32 resource);

u8 *billCloneEmitterTransform(u8 *work);

void billReleaseEmitterNode(s32 work);

extern EffRandState effSharedRandomState[];

void billAdvanceEmitterInstances(EffClassWork *work);

void billUpdateEmitterDrawColorAndTransform(u8 *work);

void effClearStripFrames(u8 *work);

u8 *billAllocStripNode(u8 *config);

void billInitializeStripRows(u8 *node, u8 *config);

extern u8 *billAllocStripNode(u8 *);

u8 *billCreateStripTransform(EffBillStripFrameConfig *config, u32 resource);

u8 *billCloneStripTransform(u8 *work);

void billReleaseStripNode(s32 work);

extern EffRandState effSharedRandomState[];

void billAdvanceStripInstances(EffClassWork *work);

void billUpdateStripDrawColorAndTransform(u8 *work);

void billResetTrailIndices(u8 *work);

u8 *billCreateTrailNode(EffBillTrailFrameConfig *config, u32 resource);

void billReleaseTrailNode(s32 work);

void billUpdateTrailDrawColorAndTransform(u8 *work);

void billResetQuadIndices(u8 *work);

u8 *billAllocQuadNode(u8 *config);

void billInitializeQuadRows(u8 *work, u8 *descriptor);

u8 *billCreateQuadTransform(EffBillQuadFrameConfig *config, u32 resource);

u8 *billCloneQuadTransform(u8 *work);

void billReleaseQuadNode(s32 work);

extern EffRandState effSharedRandomState[];

void billAdvanceQuadInstances(EffClassWork *work);

void billUpdateQuadDrawColorAndTransform(u8 *work);

/* Four-byte owner header followed by its class-work pointer array. */
typedef struct EffClassWorkList {
    EffClassWork **entries;
} EffClassWorkList;

EffClassWork *effAllocateActiveInstanceWork(u16 kind, void *source);

/* The ring source selects a minimum of three segments and repeats its three colors. */
typedef struct EffRingSource {
    u8 pad_00[0x38];
    u32 segments;       // 0x38
    u8 pad_3C[8];
    u32 firstColor;     // 0x44
    u32 middleColor;    // 0x48
    u32 lastColor;      // 0x4C
} EffRingSource;

/* Class kind 3 copies this complete 0x68-byte serialized parameter record.
 * The color/alpha prefix is the existing interpolation provider's layout;
 * alphaTrack.surfaceIndex at 0x28 also selects the point-set draw type. */
typedef struct EffRadialRingParams {
    SdfColorTrack colorTrack; /* 0x00 */
    SdfAlphaTrack alphaTrack; /* 0x24 */
    u32 duration;            /* 0x34 */
    u32 segments;            /* 0x38 */
    u8 flag;                 /* 0x3C */
    u8 pad3D[3];
    f32 baseRadius;          /* 0x40 */
    u32 firstColor;          /* 0x44 */
    u32 middleColor;         /* 0x48 */
    u32 lastColor;           /* 0x4C */
    f32 widths[3];           /* 0x50 */
    f32 speed;               /* 0x5C */
    f32 acceleration;        /* 0x60 */
    u8 reverseTime;          /* 0x64 */
    u8 pad65[3];
} EffRadialRingParams;

typedef char EffRadialRingParams_size_must_be_0x68[(sizeof(EffRadialRingParams) == 0x68) ? 1 : -1];

EffClassWork *effCreateResourceInstanceA(u16 kind, void *source, u32 option);

EffClassWork *effCreateActiveInstanceFromFile(FileJobPayload *request);

void effDestroyActiveInstanceWork(EffClassWork *work);

EffClassWork *effCreateActiveResource(EffClassWork *work);

void effResetActiveInstanceFrame(EffClassWork *work);

void effAdvanceActiveInstanceFrame(EffClassWork *work);

void effDispatchActiveInstanceDraw(EffClassWork *work);

void effUpdateAndDrawActiveInstance(EffClassWork *work);

void effCopyActiveInstancePosition(void *dst, void *src);

void effCopyActiveInstanceOrientation(void *work, void *src);

void effSetActiveInstanceColor(EffClassWork *work, u32 color);

void effSetActiveInstanceMatrixComponent(EffClassWork *work, float value);

EffTrackSet *effCreateTrackSet(s32 count, u16 kind);

extern struct SdfTextureFileHeader *D_003BC96C;

EffTrackSet *effCreateTrackSetWithSharedReferences(u32 count, u32 kind, u32 sharedRef);

/* Release the track set's retained reference, draw asset, and allocation. */
void effReleaseResourceRefs(EffTrackSet *refs);

/* Copy a track set: same size and kind, retaining the source's shared reference (or counting one more user of the built-in one). */
EffTrackSet *effDuplicateResourceRefs(const EffTrackSet *original);

extern sdfGrOt_t *D_0037EAB0[8];

extern u32 D_0037E9A0[20];

extern u32 D_0037E9F0[12];

extern u32 D_0037EA20[20];

extern u32 D_0037EA70[16];

extern sdfTexHandle_t *sdfGetOrCreateTextureForReference(sdfGrOt_t *, RefObj *);

struct sdfMat_t;

/* Draw track-set positions/colors/UVs; a null matrix selects the identity transform. */
void effDrawTrackSet(EffTrackSet *track, const Matrix4 *matrix);

extern struct sdfMemHandle_t *D_003BC960[2];

void effLoadFlashTextures(void);

u32 effGetFlashTextureHandle(s32 flashTextureIndex);

typedef struct EffCounterHeader {
    u32 unk00;
    u32 frame;
} EffCounterHeader;

/* Class draw modes own either a counter header or per-segment scales. */
typedef struct EffClassDrawState {
    union {
        EffCounterHeader *ring;
        f32 *scales;
    };
    EffClassWork *effect;
    EffTrackSet *trackSet;
    struct sdfMemHandle_t *allocation;
} EffClassDrawState;

void effResetRingResourceFrame(s32 work);

/* Create a point-set reference and initialize four colors per segment. */
u32 *effSegmentPointerSet(u8 *work);

extern void effAssetQueueRelease(EffPointSet *);

void effReleaseRingResourceHandle(u32 *handle);

/* Class 1 ring parameters have a distinct 0x6C-byte copied layout. */
typedef struct EffExpandingRingParams {
    SdfColorTrack colorTrack;
    SdfAlphaTrack alphaTrack;
    s32 duration;
    u32 segments;
    u8 flag;
    u8 pad3D[3];
    f32 baseRadius;
    u32 colors[3];
    f32 widths[3];
    u32 unk5C;
    f32 speed;
    f32 acceleration;
    u8 reverseTime;
    u8 pad69[3];
} EffExpandingRingParams;

typedef char EffExpandingRingParams_size[(sizeof(EffExpandingRingParams) == 0x6C) ? 1 : -1];

/* Generate the timed ring's four radial point bands; reverse-time mode is retained. */
void effUpdateExpandingRingPoints(EffClassWork *work);

extern void effDrawFourPointGroups(EffPointSet *, const Matrix4 *);

void billDrawCellBlendA(BillCellDrawWork *work);

void effResetClassFrameAndFlags(s32 work);

extern EffRandState effSharedRandomState[];

/* The cached class payload is copied from the 0x5C-byte source prefix. */
typedef struct EffRingClassConfig {
    EffRingSource ring;
    u8 pad50[0xC];
    u8 classConfig[0x5C];
    u8 padB8[0x10];
    f32 scale;
    f32 scaleRand;
} EffRingClassConfig;

EffClassDrawState *effCreateScaledClassDrawState(EffRingClassConfig *source);

void effReleaseClassDrawResources(s32 work);

extern void effAdvanceClassFrame(EffClassWork *);

/* Advance frame geometry and rebuild scaled row offsets with predecessor wrapping. */
void effUpdateScaledClassFrameRows(BillCellDrawWork *work);

extern void effRunClassPostFrame(EffClassWork *);

void billDrawClassUpdatedCellBlend(BillCellDrawWork *work);

void effResetClassRingFrame(EffClassWork *work);

/* Create a point-set reference and initialize four colors per segment. */
EffRingResource *effCreateRingHandle(EffRadialRingParams *work);

void effReleaseRingHandle(EffRingResource *handle);

void effUpdateCenteredRadialRingVertices(EffClassWork *work);

void billDrawCellBlendB(EffClassWork *work);

EffClassWork *effCreateClassWork(u16 kind, void *source);

EffClassWork *effCreateClassWorkFromFile(FileJobPayload *request);

void effDestroyClassWork(EffClassWork *work);

EffClassWork *effCloneClassWork(EffClassWork *work);

void effInitializeClassFrame(EffClassWork *work);

void effAdvanceClassFrame(EffClassWork *work);

void effRunClassPostFrame(EffClassWork *work);

void effUpdateClassFrame(EffClassWork *work);

void effSetClassWorkPrimaryTransformVector(s128 *dst, const s128 *src);

void effSetClassWorkSecondaryTransformVector(EffClassWork *work, const s128 *src);

void effSetClassWorkColor(EffClassWork *work, u32 color);

void effSetClassWorkScale(EffClassWork *work, float value);

extern sdf3DPrimPolyParm_t D_003DCA10[];

EffPointSet *effCreatePointSet4(u32 count);

/* Queue the draw asset for release and return the backing allocation. */
void effAssetQueueRelease(EffPointSet *set);

/* vu0 routine: SDK loads the supplied transform or constructs identity. */
void effDrawFourPointGroups(EffPointSet *set, const Matrix4 *matrix);

/* Create an empty surface node for handleCount grid cells; no handles are allocated yet. */
EffectSurfaceNode *effCreateSurfaceNode(u32 handleCount);

EffectSurfaceNode *effCreateSurfaceNodeForGrid(FileKeyBlock *grid);

extern void effRebuildSurfaceHandles(EffectSurfaceNode *, u16, void *);

void effReplaceResourceRef(EffectSurfaceNode *node, u32 entryId, const void *resource);

EffectSurfaceNode *effResourceReferenceReplaceFromFile(FileJobPayload *work);

extern void effRebuildSurfaceJobs(EffectSurfaceNode *, void *);

extern void effReplaceSurfaceResourceHolder(EffectSurfaceNode *node, u32 resource);

void effReplaceSurfacePrimaryBillboard(EffectSurfaceNode *node, u32 resourceId);

void effReplaceSurfaceFlaggedBillboard(EffectSurfaceNode *node, u32 resourceId);

void effSetSurfaceRetainedResource(EffectSurfaceNode *node, u32 resourceId);

EffectSurfaceNode *effInitializeSurfaceForKind(FileJobPayload *work);

void effDestroySurfaceNode(EffectSurfaceNode *node);

EffectSurfaceNode *effRecreateSurfaceNodeFromWork(u8 *work);

extern void effCloneSurfaceGridResources(EffectSurfaceNode *, u8 *);

EffectSurfaceNode *effCreateSurfaceGridWithConfiguration(u8 *work);

/* Replace the destination's kind-selected resources with clones or retained references. */
void effCloneSurfaceGridResources(EffectSurfaceNode *dst, u8 *work);

/* Replace the per-cell grid handles from the copied source header. kind is unused here. */
void effRebuildSurfaceHandles(EffectSurfaceNode *node, u16 kind, void *source);

void effReplaceResourceRef(EffectSurfaceNode *node, u32 entryId, const void *resource);

void effSetSurfaceRetainedResource(EffectSurfaceNode *node, u32 resourceId);

void effReplaceSurfacePrimaryBillboard(EffectSurfaceNode *node, u32 resourceId);

void effReplaceSurfaceFlaggedBillboard(EffectSurfaceNode *node, u32 resourceId);

void effRebuildSurfaceJobs(EffectSurfaceNode *node, void *source);

void effReplaceSurfaceResourceHolder(EffectSurfaceNode *node, u32 resource);

void effClearSurfaceNodeRecordReferences(s32 node);

void effAcquireSurfaceRecord(s32 node);

void effUpdateSurfaceRecordAndDraw(s32 node);

void effMenuRecordVectorSet(s32 node, const u128 *value);

void effSetSurfaceRecordSecondaryVector(s32 node, const u128 *value);

void effSetSurfaceNodeColor(s32 node, u32 value);

void effIndexedFloatCallbackDispatch(ValPtr44 *p, float v);

extern sdf3DPrimPolyParm_t D_003DCA40;

extern sdf3DPrimPolyParm_t D_003DCA70;

extern u8 D_0037EB90[];

extern u8 D_0037EBE0[];

/* vu0 routine: grid surface node with rows of three vertex columns */
EffSurfaceGridNode *effCreateSurfaceGridNode(u32 count, u32 columns);

/* vu0 routine: fade-blended vertex colours written into the surface node's tail */
void effFillSurfaceGridColorGradient(EffSurfaceGridNode *node, u32 *colors);

void effReleaseSurfaceGridBuffers(EffSurfaceGridNode *node);

void effResetSurfaceGridFrame(EffSurfaceGridNode *node);

/* Three-vector sampler reads the primary surface ring counters. */
void effCopyRingFrameVectors(EffSurfaceGridNode *node, u128 *destination, s32 frame);

/* data holds two vector addresses: the ring head, then the input frame.
 * ring_mirror_pointer_epochs_probe (hooked==unhooked): GCSE preserves the
 * head-to-mirror copy; separate append/mirror lifetimes keep native allocation. */
void effAppendSurfaceGridVectors(EffSurfaceGridNode *work, const f32 (*source)[4]);

extern void vuCatmullRomPoint(f32 (*)[4], f32);

/* vu0 routine: interpolate three-vector ring frames with SDK spline and lerp operations. */
void effInterpolateSurfaceGridVectors(EffSurfaceGridNode *work, const f32 (*vectors)[4]);

void effBeginMatrixVuDrawPacket(const Matrix4 *matrix);

/* Point history of a grid-surface trail: a ring of 16-byte points with packed colors,
 * drawn as triangle strips into the current render packet, then a fixed end cap. */
typedef struct EffSurfaceTrailHistory {
    u8 pad00[0x10];
    s32 pointCapacity;        /* 0x10 */
    s32 activePointCount;     /* 0x14 */
    s32 nextPointWriteIndex;  /* 0x18 */
    u8 pad1C[0x08];
    u128 *points;             /* 0x24 */
    u32 *colors;              /* 0x28 */
    s32 resourceHandle;       /* 0x2C */
    u128 *endPoints;          /* 0x30 */
    u32 *endColors;           /* 0x34 */
} EffSurfaceTrailHistory;

void effDrawSurfaceTrailHistory(EffSurfaceTrailHistory *history, u32 color);

typedef struct EffDrawEntry {
    u32 handle;
    s32 frame;
    u8 pad08[8];
    void (*draw)(u8 *, struct sdfDrawTag_t *);
} EffDrawEntry;

void effSubmitIndexedRenderPacket(u32 index);

extern EffQuadWork *effCreateQuadWork(FileJobPayload *job);

void effDuplicateRenderResourceOwner(EffQuadWork *work, const EffQuadWork *source);

extern sdf3DPrimPolyParm_t effQuadPacketParams;

extern u32 D_0037EC10[];

extern u32 D_0037EC20[];

EffQuadWork *effCreateQuadWork(FileJobPayload *job);

void effReleaseRenderResources(EffQuadWork *work);

EffQuadWork *effCloneRenderResourceWork(const EffQuadWork *source);

void effDuplicateRenderResourceOwner(EffQuadWork *work, const EffQuadWork *source);

void effResetRenderResourceKind(EffQuadWork *work);

void effCopyRenderResourcePosition(EffQuadWork *work, const f32 *source);

void effCopyRenderResourceOrientation(EffQuadWork *work, const f32 *source);

void effSetRenderResourceColor(EffQuadWork *work, u32 color);

void effSetRenderResourceMatrixComponent(EffQuadWork *work, f32 value);

void effRandomizeParticleFields(EffClassWork *work);

EffPointSetTable *effCreateAlphaRampPointSetRows(EffBillPointConfig *src);

void effFreeIndexedEntries(EffClassWork *work);

extern f32 effWorldUpVector[4];

extern f32 aViewEye[4];

extern f32 aViewTarget[4];

/* Advance radial five-point wave strips from the class orientation. */
void effUpdateRadialWavePointRows(EffClassWork *work);

typedef struct Matrix4 Matrix4;

void effDrawFivePointGroups(EffPointSet *set, Matrix4 *matrix);

/* Per-frame update of a five-point-group class: tint every active set and draw it under the class transform. */
void effDrawTintedFivePointGroupRows(EffClassWork *work);

extern void effResetDispatchCounter(EffClassWork *);

void effResetRadialClassResourceFrames(EffClassWork *work);

extern EffClassWork *effCreateClassResourceWork(u16, void *);

EffClassWorkList *effCreateRadialClassResourceEntries(EffBillPointConfig *config);

extern void effDestroyClassResourceWork(EffClassWork *);

void effDestroyRadialClassResourceEntries(EffClassWork *work);

extern void effAdvanceClassResourceFrame(EffClassWork *);

void effAdvanceRadialClassResourceFrames(EffClassWork *work);

extern void effCopyClassResourcePosition(void *, void *);

extern void effCopyClassResourceOrientation(void *, void *);

extern void effSetClassResourceMatrixComponent(EffClassWork *, f32);

extern void effSetClassResourceColor(EffClassWork *, u32);

extern void effDrawClassResourceWork(EffClassWork *);

/* vu0 routine: packed color blend and SDK vector copies. */
void effUpdateRadialClassInstances(EffClassWork *work);

typedef struct EffScaleRange {
    EffScaleRangeEntry *entries;
    f32 start;
    f32 delta;
    struct sdfMemHandle_t *allocation;
} EffScaleRange;

void effSeedBillScaleRange(EffClassWork *work);

EffScaleRange *effCreateRetainedPointSetColorRows(EffBillPointConfig *src);

void effReleaseBillPointEntries(EffClassWork *work);

/* Actual same-title provider contracts; full four-float VU buffers remain. */
extern f32 aViewEye[4];

extern f32 aViewTarget[4];

/* Advance orbiting entries and rebuild their camera-facing, wavy point spans. */
void effUpdateOrbitalPointSpanWaves(EffClassWork *work);

/* vu0 routine: tint point sets and draw under the class quaternion transform.
 * Retail color homes +0x50/+0x60/+0x70/+0x80 are 16-byte SDK-sized locals. */
void effDrawRetainedPointSetRows(EffClassWork *work);

EffClassWork *effCreateClassResourceWork(u16 kind, void *source);

EffClassWork *effCreateClassResourceFromFile(FileJobPayload *request);

void effDestroyClassResourceWork(EffClassWork *work);

EffClassWork *effCloneClassResourceWork(EffClassWork *work);

void effResetDispatchCounter(EffClassWork *work);

void effAdvanceClassResourceFrame(EffClassWork *work);

void effDrawClassResourceWork(EffClassWork *work);

void effUpdateAndDrawClassResource(EffClassWork *work);

void effCopyClassResourcePosition(void *dst, void *src);

void effCopyClassResourceOrientation(void *work, void *src);

void effSetClassResourceColor(EffClassWork *work, u32 color);

void effSetClassResourceMatrixComponent(EffClassWork *work, float value);

extern sdf3DPrimPolyParm_t D_003DCAD0[];

EffPointSet *effCreatePointSet5(s32 count);

/* Queue the draw asset for release and return the backing allocation. */
void effReleasePointSetAsset(EffPointSet *set);

void effDrawFivePointGroups(EffPointSet *set, Matrix4 *matrix);

EffectStripNode *effCreateStripNode(u32 percent);

EffectStripNode *effCreateStripNodeFromGrid(FileKeyBlock *grid);

extern void effReplaceFileResourceRef(EffectStripNode *, u32, const void *);

EffectStripNode *effFileResourceReferenceReplace(FileJobPayload *work);

void effReleaseModelResources(EffectStripNode *work);

EffectStripNode *effCloneStripResourceFromOwner(EffectStripNode *work);

void effReplaceFileResourceRef(EffectStripNode *node, u32 entryId, const void *resource);

void effClearStripRecordReferences(EffectStripNode *node);

void effAcquireStripRecord(EffectStripNode *node);

extern f32 aViewDown[4];

void effDrawEffectStrip(EffectStripNode *node);

/* Advance the active strip record, rebuild its geometry, and submit its tracks. */
void effUpdateAndDrawStripRecord(EffectStripNode *node);

void effSetStripRecordVector(EffectStripNode *node, const u128 *value);

void effSetStripRecordSecondaryVector(EffectStripNode *node, const u128 *value);

void effSetStripRecordColor(EffectStripNode *node, u32 value);

/* Store opacity and notify the strip's active-record callback. */
void effSetStripOpacity(EffectStripNode *p, f32 v);

void effResetBillTable(u8 *work);

u8 *effAllocateRingFadeEntries(EffBillVortexConfig *config);

/* Ring fade tables of the ring effects: `segments + 1` entries of colors
 * (4 words each) and radii (8 words each). The alpha fades in over the first
 * fadeIn fraction of the entries and out from the fadeOut fraction; the table
 * is then copied for every remaining layer. */
void effFillRingFadeGradient(u8 *node, EffBillEmitterCommon *config);

u8 *effCreateRingFadeTable(EffBillVortexConfig *config, u32 resource);

u8 *effAssetPointerSet(u8 *work);

void effReleaseRingFadeTable(s32 work);

extern void effDrawRibbon(EffRibbonWork *, const Matrix4 *);

void effBlendBillboardInstanceColorsAndTransforms(u8 *work);

void effResetBillboardFrameInstanceCounters(u8 *work);

u8 *effAllocateBillFadeFrameEntries(EffBillColumnConfig *config);

void effFillBillFadeGradient(u8 *node, EffBillEmitterCommon *config);

u8 *effCreateBillFadeTable(EffBillColumnConfig *config, u32 resource);

u8 *effCloneBillFadeTable(u8 *work);

void effReleaseBillFadeTable(s32 work);

/* Update the column's particle records and the ribbon vertices for this frame. */
void effUpdateColumnParticlesAndRibbons(EffClassWork *work);

void effBillBlendCellColorAndUpdateTransform(BillCellDrawWork *work);

void effResetParticleBillFrameCounters(u8 *work);

u8 *effAllocateCompactRingFadeEntries(EffBillSpiralConfig *config);

void effFillCompactRingFadeGradient(u8 *node, EffBillEmitterCommon *config);

u8 *effCreateCompactRingFadeTable(EffBillSpiralConfig *config, u32 resource);

u8 *effCloneBillboardFrameAsset(u8 *work);

void effReleaseCompactRingFadeTable(s32 work);

typedef struct EffSpiralParticle {
    s32 timer;      /* -1 free, -2 retired */
    u8 pad04[4];
    f32 tiltX;      /* 0x08 */
    f32 tiltY;      /* 0x0C */
    f32 radius;     /* 0x10 */
    f32 radiusStep; /* 0x14 */
    f32 spin;       /* 0x18 */
    f32 phase;      /* 0x1C */
    f32 width;      /* 0x20 */
    f32 sweep;      /* 0x24 */
    f32 height;     /* 0x28 */
} EffSpiralParticle;

/* vu0 routine: native matrix, cross/normalize and ribbon extrusion. */
void effUpdateBillSpiral(EffClassWork *work);

void effUpdateCompactRingDrawColorAndTransform(u8 *work);

EffClassWork *effAllocateBlock(u16 kind, void *source);

extern EffClassWork *effAllocateBlock(u16, void *);

EffClassWork *effCreateResourceInstanceB(u16 kind, void *source, u32 option);

EffClassWork *effCreateBlockResourceFromFile(FileJobPayload *request);

void effDestroyBlockResourceWork(EffClassWork *work);

EffClassWork *effDuplicateActiveResourceB(EffClassWork *work);

void effResetBlockResourceFrame(EffClassWork *work);

void effAdvanceBlockResourceFrame(EffClassWork *work);

void effDrawBlockResourceWork(EffClassWork *work);

void effUpdateAndDrawBlockResource(EffClassWork *work);

void effCopyBlockResourcePosition(void *dst, void *src);

void effCopyBlockResourceOrientation(void *work, void *src);

void effSetBlockResourceColor(EffClassWork *work, u32 color);

void effSetBlockResourceMatrixComponent(EffClassWork *work, float value);

extern sdf3DPrimPolyParm_t D_003DCB00;

EffRibbonWork *effCreateRibbonWork(u32 count, u32 repeat);

EffRibbonWork *effCreateRibbonWithSharedResource(u32 count, u32 repeat, struct SdfTextureFileHeader *resource);

void effSharedAssetReferenceRelease(EffRibbonWork *work);

extern sdfGrOt_t *D_0037ED78[];

EffRibbonWork *effCloneRibbonWithSharedResource(EffRibbonWork *source);

static inline void unpackNormalizedColor(u32 color) {
    u32 rgba[4];
    rgba[0] = color;
    EE_MMI_RGBA_UNPACK(rgba, 1.0f / 128.0f);
}

static inline u32 packNormalizedColor(void) {
    u32 rgba[4];
    EE_MMI_RGBA_PACK_UNIT(rgba[0], 128.0f);
    return rgba[0];
}

void effDrawRibbon(EffRibbonWork *work, const Matrix4 *matrix);

void effLoadWindTexture(void);

u32 effGetWindTextureHandle(void);

void effResetAnimationFrameEntries(u8 *work);

u32 *effAllocateAnimationBuffer(EffBillFlameConfig *work);

void effFillFadeColorRows(u8 *work, u8 *config);

extern u32 effCreateTexturedStripWithSharedTexture(u32, u32);

u32 *effPrepareTextureAnimation(EffBillFlameConfig *work);

extern u32 effAllocateStripFromWorkAndRetainTexture(u8 *);

u32 *effPrepareOwnedTextureAnimation(u8 *work);

void effReleaseScalyStripResources(u8 *work);

void effReleaseTextureAnimationWork(s32 work);

/* vu0 routine: fade-blended colour and scaled transform of a mesh draw record */
void effUpdateTintedScaledMesh(BillCellDrawWork *work);

void effInitializeAnimationPositions(EffClassWork *work);

u32 effClampSlotCount(FileKeyBlock *grid);

extern u32 effRetainScalyTextureReference(void);

EffAnimationState *effCreateAnimationState(u32 unused, u32 count);

EffAnimationState *effActivateAnimationState(s32 work);

extern void effReleaseScalyTextureReference(s32);

void effReleaseAnimationFrameResources(u8 *work);

void effSynchronizeFileTransform(u8 *work);

extern u32 effClampSlotCount(FileKeyBlock *);

EffAnimationState *effCreatePrimarySlotAnimationState(u8 *work);

EffAnimationState *effCreateAlternateSlotAnimationState(u8 *work);

void effResetSlotAnimationRecord(s32 work);

u32 *effAllocateQuantizedBuffer(EffBillQuantizedConfig *work);

void effBuildQuantizedTexturedStrip(u32 *buffer, EffBillQuantizedConfig *config);

extern void effBuildQuantizedTexturedStrip(u32 *, EffBillQuantizedConfig *);

u32 *effPrepareQuantizedTexture(EffBillQuantizedConfig *work);

u32 *effPrepareOwnedQuantizedTexture(u8 *work);

void effReleaseBillboardFrameAsset(s32 work);

void effOffsetNodeRowsVU(u8 *work);

/* vu0 routine: fade-blended colour and scaled transform of a mesh draw record */
void effUpdateFadedMeshTransform(BillCellDrawWork *work);

EffClassWork *effAllocateBlockWithModel(u16 kind, void *source);

extern EffClassWork *effAllocateBlockWithModel(u16, void *);

EffClassWork *effCreateResourceInstanceC(u16 kind, void *source);

EffClassWork *effCreateModelBlockFromFile(FileJobPayload *request);

void effDestroyModelBlockWork(EffClassWork *work);

EffClassWork *effRecreateActiveByClass(EffClassWork *work);

void effResetModelBlockFrame(EffClassWork *work);

void effAdvanceDispatchCounter(EffClassWork *work);

void effDrawModelBlock(EffClassWork *work);

void effUpdateAndDrawModelBlock(EffClassWork *work);

void effCopyModelBlockPosition(void *dst, void *src);

void effCopyModelBlockOrientation(void *work, void *src);

void effSetModelBlockColor(EffClassWork *work, u32 color);

void effSetModelBlockMatrixComponent(EffClassWork *work, float value);

/* Retain and return the lazy shared reference to /effect/scaly00.tmx. */
u32 effRetainScalyTextureReference(void);

/* Drop a scaly texture reference; unused is ignored and the last owner releases the clone. */
void effReleaseScalyTextureReference(s32 unused);

extern sdf3DPrimPolyParm_t D_003DCB30;

/* Allocate position/UV/color rows and a draw handle using count and repeat. */
u8 *effAllocateTexturedStripWork();

/* Create a scaly strip and retain its shared texture reference. */
u32 effCreateTexturedStripWithSharedTexture(u32 count, u32 repeat);

void effReleaseScalyStripResources(u8 *work);

/* Recreate the source strip's dimensions and retain another shared texture reference. */
u32 effAllocateStripFromWorkAndRetainTexture(u8 *work);

#include "dds/dds3_draw_buffer.h"

#include "dds/dds3_frame_draw_buffer.h"

#include "dds/sdf_gs_header.h"

extern sdfGrOt_t *D_0037EE20[];

extern vu8 sdfFrameId;

extern dds3FrameDrawBuffer_t dds3FrameDrawPacketRecords[];

extern void sdfGrSetFrameMaskRGB(const u8 *, SdfDmaReferenceChainPacket *, s32);

extern void sdfGrSetFrameMaskReset(const u8 *, SdfDmaReferenceChainPacket *, s32);

void effDrawScalyTexturedStrip(EffStripWork *work, void *matrix);

void effLoadScalyTexture(void);

u32 effGetScalyTextureHandle(void);

typedef char EffModelResource_size_must_be_0x40[
    (sizeof(EffModelResource) == 0x40) ? 1 : -1];

void effSeedParticleSpanParameters(EffModelResource *work);

extern EffPointSet *effCreatePointSet3(s32 count);

/* Build the point and reference sets for each model map-position record. */
EffSpanTable *effCreateParticleSpanTable(EffSpanConfig *config, dds3ModelHandle_t *model);

extern void effReleaseModelPointSetAsset(EffPointSet *);

void effReleaseParticleList(EffSpanTable *list);

/* vu0 routine: quaternion matrix and SDK RGBA modulation. */
void effDrawParticleSpanPointsAndReferences(EffModelResource *work);

EffModelResource *effCreateModelResourceWithInlineData(u16 kind, void *source, void *secondary, u32 param);

u32 effCreateModelResourceFromFile(FileJobPayload *work);

typedef struct EffModelCreateRequest {
    u8 pad0[0x2C];
    u16 kind;
    u8 pad2E[2];
    dds3ModelHandle_t *assetId;
    u32 attributes;
    u8 pad38[4];
    void *source;
} EffModelCreateRequest;

void effDestroyModelResource(EffModelResource *effect);

EffModelResource *effCreateModelResource(EffModelCreateRequest *work);

void effResetModelResourceUpdateCount(EffModelResource *effect);

void effAdvanceModelResourceUpdateCount(EffModelResource *effect);

void effDispatchModelResourceCallback(EffModelResource *effect);

void effStepModelResourceCallbacks(EffModelResource *effect);

void effSetModelResourcePrimaryTransformVector(EffModelResource *effect, const f32 *position);

void effSetModelResourceSecondaryTransformVector(EffModelResource *effect, const f32 *orientation);

void effSetModelResourceColor(EffModelResource *effect, u32 color);

void effSetModelResourceScale(EffModelResource *effect, f32 scale);

extern sdf3DPrimPolyParm_t D_003DCB60[];

EffPointSet *effCreatePointSet3(s32 count);

/* Queue the draw asset for release and return the backing allocation. */
void effReleaseModelPointSetAsset(EffPointSet *set);

void effDrawThreePointGroups(EffPointSet *set, Matrix4 *matrix);

extern u32 effBTLSkillGetInvokeFormation(void);

extern u32 effBTLFieldColorGetVariantSelector(void);

extern u32 effBTLFieldColorGetOverrideSelector(void);

extern u32 effBTLFieldColorGetFinalSelector(void);

extern void effBTLPartGetPositionPtr(BtlUnit *owner, EffectVectorRequest *request, u128 *out);

void effGetWorldVector(u32 which);

#define EFF_COLLECT_ACTORS_VARIANT_SELECTOR 0

#define EFF_COLLECT_ACTORS_ORIGINAL_SELECTOR 1

#define EFF_COLLECT_ACTORS_VARIANT_SELECTOR_ALIAS 2

#define EFF_COLLECT_ACTORS_ORIGINAL_GROUP 3

#define EFF_COLLECT_ACTORS_VARIANT_GROUP 4

#define EFF_COLLECT_ACTORS_EITHER_GROUP 5

#define EFF_COLLECT_ACTORS_OVERRIDE_SELECTOR 6

#define EFF_COLLECT_ACTORS_FINAL_SELECTOR 7

s32 effCollectModelEffectActors(BtlUnit **out, u32 kind);

extern void evtUnitLightOff(evtUnitHandle_t *, s32);

/* Restore actor light endpoints and directions when the field-light effect ends. */
void effRestoreBattleActorLighting(u32 unusedResource);

#include "dds/btl_effect_position.h"

extern void effBattleMiscDirectionTo(BtlUnit *, EffectVectorRequest *, f32 *);

extern u32 btlCameraVectorHasNaN(void);

extern u32 btlBlendColorVec(f32 *, f32 *, f32);

/* Apply timed actor light transitions, then fade back to the original colors/directions. */
void effUpdateBattleActorLighting(EffActiveResource *work);

void effSyncLinkedActorChildParameter(void);

/* Apply a temporary tint to eligible actors, then restore their original RGB. */
void effTransitionSelectedActorRgbColors(EffActiveResource *work);

extern u128 D_003DCBD0[];

extern s8 D_003BC9AC;

extern u128 D_003DCBE0[];

extern u128 D_003DCBA0[];

extern sdf3DLight_t *D_0037EE80[3];

s32 effComputeLightDirectionVU(dds3ModelHandle_t *vector, sdf3DLightParallelPacket_t *target);

extern u128 *D_00324770[];

extern u128 D_003DCB90[];

extern u128 kwlnDefaultColorVector[];

void effResetDefaultColorTables(void);

/* Payload of resource kind 2: two colours plus the position request that feeds the shared light vector. */
typedef struct EffFieldColorConfig {
    u32 firstColor;   /* 0x00 */
    u32 secondColor;  /* 0x04 */
    EffectVectorRequest direction; /* 0x08 */
} EffFieldColorConfig;

void effInitializeFieldLightColors(EffActiveResource *work);

void effResetCopiedPayloadTargets(EffCopiedPayloadWork *work);

EffCopiedPayload *effAllocateCopiedEffectPayload(u32 owner, const void *source, s32 size);

typedef struct EffCopiedSpawnList {
    u32 count;
    FldSpawnBatch *groups;
} EffCopiedSpawnList;

extern dds3Basic_t *dds3AddPathBasic(s32 key, Dds3PathCurveTable *work, const char *name);

/* Capture constructor arguments before consuming the entry; see
 * docs/idioms.md, "Trace-justified cursor consumption: copied payload slots". */
void effInitializeCopiedPayloadSlots(EffCopiedPayload *payload);

EffCopiedPayload *effCreateAndInitializeCopiedPayload(u32 type, const void *source, s32 size);

EffCopiedPayload *effCloneEffectPayloadFromOwner(EffCopiedPayloadWork *work);

void effDestroyCopiedEffectPayload(EffCopiedPayload *payload);

#include "dds/eff_camera_path_parameters.h"

extern void btlSetRuntimeFlag2000(void);

extern u32 btlIsRuntimeFlag2000Clear(void);

extern void btlClearAllUnitDefeatCandidates(void);

extern void btlFlagMatchingUnitsDefeatCandidate(s32);

extern void btlFlagAllUnitsDefeatCandidate(void);

extern void btlFlagUnitDefeatCandidate(BtlUnit *);

extern void btlCopyUnitRotationQuaternion(BtlUnit *, f32 dst[4]);

extern void dds3CameraObjectBasicSetFovy(dds3Basic_t *, f32);

extern f32 D_003BC9A8;

extern u128 D_0037EE90, D_0037EEA0, D_0037EEB0, D_0037EEC0, D_0037EED0;

extern u128 D_0037E110;

/* vu0 routine: follow the selected actor with the active camera path. */
s32 func_002B3698(EffActiveResource *owner);

extern void btlApplyScaledUnitEffectParameter(BtlUnit *, s32, s32, f32);

extern void btlStartMoveOtherUnitsTask(BtlUnit *, s32);

typedef struct EffAnimInfo {
    u16 id;
    u16 flags;
    u8 actorSelection;
    u8 unk05;
    u16 loop;
} EffAnimInfo;

void effApplyOverlaySpecs(u8 *work);

extern void sndLoadAndPlayStationedSe(u32);

void effReportResourceStatus(EffActiveResource *work);

/* Apply the selected battle state's stored tint vectors immediately when its mask is set. */
void effApplyBattleStateTint(void);

/* Start the payload's packed-color tint transition on frame zero. */
void effStartTintTransitionFromColors(EffActiveResource *work);

void effResetSlots(void);

void effTickSlotVolumeFade(void);

void effSetDormantSlot(u32 index, u8 color, u32 value);

void effSetActiveSlot(u32 index, u8 color, u32 value);

void effResetActiveEffectSlots(void);

void effUpdateEffectSlotTransitions(EffActiveResource *work);

void effBattleTintTransitionInitialize(void);

/* The final timer word also supplies the default tint's halfword ID. */
typedef struct EffTintTransitionParams {
    u32 duration;
    u32 resourceArgument;
    u16 resourceKind;
    u16 unk0A;
    union {
        u32 delay;
        struct {
            u16 defaultKind;
            u16 unk0E;
        } ids;
    } tail;
} EffTintTransitionParams;

void effUpdateSlotTimerPair(EffActiveResource *work);

extern f32 D_003BC9A8;

/* Interpolates camera field of view in degrees and stores it in radians. */
void effApplyKeyframeAngle(EffActiveResource *work);

EffActiveResource *effAllocateResourcePayload(u16 kind, void *source);

EffActiveResource *effCreateResourceInstance(u16 kind, void *source, void *secondary, u32 param);

EffActiveResource *effCreateActiveResourceFromFile(FileJobPayload *request);

void effDestroyResourceInstance(EffActiveResource *work);

EffActiveResource *effDuplicateActiveResource(EffActiveResource *source);

void effClearCallbackFrame(EffActiveResource *work);

void effDispatchIndexedCallback(EffActiveResource *work);

void effDispatchEnabledCallback(EffActiveResource *work);

void effAdvanceActiveResourceCallbacks(EffActiveResource *work);

void effCopyActiveResourceVector(EffActiveResource *dst, const void *src);

void effCopyActiveResourceSecondaryVector(EffActiveResource *work, const void *src);

void effSetActiveResourceColor(EffActiveResource *work, u32 color);

void effSetActiveResourceScale(EffActiveResource *work, f32 value);

/* Alternate particle handles share a header but occupy distinct slots. */
EffParticleSharedResource *effCreateParticleSharedResourceFromFile(FileJobPayload *source);

void effReleaseParticleResources(EffParticleSharedResource *work);

void effReplaceSharedResource(EffParticleSharedResource *, EffParticleSharedResource *);

EffParticleSharedResource *effCloneParticleSharedResource(EffParticleSharedResource *source);

void effReplaceSharedResource(EffParticleSharedResource *work, EffParticleSharedResource *source);

void effResetParticleStateWord(EffParticleSharedResource *work);

extern sdfGrOt_t D_003255A8;

extern void sdfPrimLinkGt4(sdfDrawTag_t *list, s32 primitive, s32 x0, s32 y0, s32 u0, s32 v0, s32 color0, s32 x1, s32 y1, s32 u1, s32 v1, s32 color1, s32 x2, s32 y2, s32 u2, s32 v2, s32 color2, s32 x3, s32 y3, s32 u3, s32 v3, s32 color3, s32 depth, s32 (*alloc)(s32));

/* Submit frames below the exclusive limit; zero leaves the cursor unbounded. */
void effDrawAndAdvanceSharedParticleFrame(EffParticleSharedResource *work);

/* The shared particle header keeps its packed color in its first word. */
void effSetParticleSharedColor(u32 *dst, u32 value);

void btlInitializeEffectWork(void);

void effResetFileResourceStores(void);

s32 btlUpdateEffectWork(void);

void func_002B51A8(void);

void effResetFileResources(void);

extern void effResetFileResourceManager(void);

void effResetFileResourceStores(void);

void effSetBattleOffset(void *src);

extern void camFollowOffsetVec(u8 *, void *);

void effComputeBattleCameraPositionVU(u8 *effect);

extern void camAimRotation(void *, void *);

void effMultiplyQuatWithFlag(u8 *effect);

extern u8 effSelectedJobHeader[];

extern void effComputeBattleCameraPositionVU(u8 *);

void effApplyBattleCameraToObject();

u32 effCreateBattleCameraJob(u8 *request);

FileJobPayload *effLoadFileJobPayload(EffFileJobRequest *request, u32 existingJob);

void effInvokeFileJobWithBattleCamera(u32 job);

void effFileJobQueueRelease(u32 job);

extern char D_003BD080[];

extern const char D_003BD078[];

/* Payload option (+0x0C) selects the asset identifier at +0x08. */
typedef struct EffAssetIdentifier {
    u8 pad_00[4];
    u16 type;       // 0x04
    u8 pad_06[2];
    u16 id;         // 0x08
} EffAssetIdentifier;

u32 effPollPrimaryFile(void);

void effQueueNamedResourceRequest(void);

u32 effPollNamedFile(void);

void effQueueAttachedResourceRequest(void);

u32 effPollAttachedFile(void);

#include "dds/effect_asset_link.h"

extern EffectAssetLink *D_0038E9D8[22];

u8 *effFindAssetData(FileJob *entry);

EffQueuedFileObject *effFindAssetObject(FileJob *entry);

/* Return the selected queue row plus the offset of its visible window. */
u32 effGetSelectedFileQueueIndex(void);

extern EffectBlock128 effEditedQueueHeader;

extern EffQueuedFileObject D_0038F428;

extern s8 dds3PadBitTable[];

extern const char D_003BD0E8[];

extern u32 btlDebugMenuSetupNoOp(void *context);

extern void btlDebugMenuBeforeRecordNoOp(void);

extern void btlDebugMenuRecordNoOp(void *context);

extern void btlDebugMenuAfterRecordNoOp(void);

typedef struct EffCameraCreateRow {
    EffCameraCreateRequest request;
    u32 sourceTail;
} EffCameraCreateRow;

typedef char EffCameraCreateRowSizeCheck[(sizeof(EffCameraCreateRow) == 0x1C) ? 1 : -1];

extern void fileQueueInitTransform(void *);

extern FileJob *fileAppendJob(FileQueue *, u32);

extern s32 fileQueueCountLinkedJobs(FileQueue *);

extern FileJob *effCurrentFileQueueEntry;

extern u32 D_003BD064;

extern EffQueuedFileObject effPartsListMenu;

u32 effAppendPositionedCameraFileJob(const EffCameraCreateRequest *request);

/* Load the selected job's payload and camera snapshots; return the refresh status. */
u32 effLoadSelectedFileJob(void);

extern u32 D_0038EA6C[];

u32 effReinitializeFileQueue(void);

extern u32 D_0038EB3C[];

u32 effResetFileQueueState(void);

extern u32 D_0038EAD4[];

u32 effFinalizeQueuedFile(void);

extern FileJob *fileDuplicateAndAppendJob(FileQueue *, FileJobPayload *);

extern FileJob *fileQueueFindById(FileQueue *, u32);

/* Clone the selected job, preserve its sector leader, and refresh the selected asset. */
u32 effCloneSelectedFileJob(void);

extern u32 effSelectLinkedFileState();

u32 effFlagFileQueueActive(void);

typedef struct EffectBlock90 {
    u32 word[36];
} EffectBlock90;

typedef struct EffectAlignedBlock128 {
    u64 word[16];
} EffectAlignedBlock128;

extern EffQueuedFileObject effBattleCameraSnapshotSelection;

extern EffEditorRow D_0038EC38[];

extern EffEditorRow D_0038EC80[];

extern u8 effPartsPreviewReload;

u32 effLoadBattleCameraSnapshot(u32 value);

u32 effStoreBattleCameraSnapshot(u32 value);

/* Present named camera/file requests, maintaining a preview job and returning selection status. */
u32 effSelectCameraFileJobFromRows(const char *nameKey, const EffCameraCreateRow *requests, u32 sourceCount);

s32 effCreateParticleTask(void);

s32 effCreatePolyTrackControlTask(void);

s32 effCreatePolyTrackTask(void);

s32 effCreateBlurTask(void);

s32 effCreateFilterTask(void);

s32 effCreateEnvironmentTask(void);

s32 effCreatePolyTextureTask(void);

s32 effCreatePolyFlashTask(void);

s32 effCreatePolyRingTask(void);

s32 effCreatePolyThunderTask(void);

s32 effCreatePolyTwinkleTask(void);

s32 effCreatePolyWindTask(void);

s32 effCreatePolyScalyTask(void);

s32 effCreatePolyCrackTask(void);

s32 effCreateBattleOnlyTask(void);

s32 effCreateTwoDimensionalTask(void);

extern void effPollResourceBankSlot(char *, u32, EffResourceBankSlot *);

u32 effPollPartResource(void);

u32 effPollNamedFileJob(void);

extern void btlBossDebugPrintfN(s32, s32, s32, const char *, ...);

extern void SetRotateMatrixY(f32);

extern char D_003B3A90[], D_003B3AA8[];

extern s8 dds3PadBitTable[];

extern f32 D_0038F530[4];

extern f32 D_003DFA20[4];

s32 effHandleDebugOrbitInput(void);

s32 effResetStaticState(void);

typedef struct EffectStateSnapshot {
    s128 vectors[4];
    f32 position[4]; /* 0x40: copied to the debug camera position and follow offset. */
    s128 unused50;
    u8 pad60[8];
    u32 flags; /* 0x68: transform flags tested by the camera diagnostic. */
    u8 pad6C[0x14];
} EffectStateSnapshot;

typedef char EffectStateSnapshotSizeCheck[(sizeof(EffectStateSnapshot) == 0x80) ? 1 : -1];

extern s32 effUpdateCameraDiagnostics(EffectStateSnapshot *, s8);

extern u8 D_0038F4F0[];

extern s8 D_0039862B[];

extern void btlDebugDrawPoint(f32 *position);

extern void btlDebugDrawRadiusGuide(f32 *center, f32 radius, u32 color, s32 flags);

extern void btlDebugDrawSegment(f32 *from, f32 *to, u32 color);

extern void btlDebugMenuBeforeRecordNoOp(void);

extern void btlDebugMenuAfterRecordNoOp(void);

extern u32 btlDebugMenuSetupNoOp(void *camera);

extern void btlDebugMenuRecordNoOp(void *camera);

extern void camFollowOffsetVec(u8 *camera, void *destination);

extern void effComputeBattleCameraPositionVU(u8 *snapshot);

/* Synchronize the diagnostic camera position and calculate its positional guides.
 * Return input flags; the native diagnostic drawing providers are retail stubs. */
s32 effUpdateCameraDiagnostics(EffectStateSnapshot *snapshot, s8 mode);

void effCopyCameraSnapshotSelection(void);

#include "dds/sdf_vu_matrix.h"

extern u8 D_003BD10B;

s32 func_002B8108(void);

s32 effResetCameraSnapshotSelection(void);

/* The camera snapshot payload is the 0x90-byte record consumed by this updater. */
typedef union EffectCameraSnapshotView {
    u32 word[36];
    struct {
        u8 pad00[0x40];
        f32 offset[4];
        f32 quat[4];
        u8 pad60[0x30];
    } camera;
} EffectCameraSnapshotView;

typedef struct EffectCameraDebugState {
    u32 unknown00[4];
    u32 flags;
    u32 unknown14[9];
} EffectCameraDebugState;

extern EffectCameraDebugState D_0038F588;

extern s8 D_0039862B[];

extern f32 effWorldUpVector[4];

extern void btlDebugDrawOrientedExtentGuide(f32 *, f32, f32, u32, f32 (*)[4]);

extern void btlDebugDrawPoint(f32 *);

typedef struct EffectCameraDebugScratch {
    s128 basePosition;
    s128 segmentStart;
    s128 segmentEnd;
    s128 rotationMatrix[4];
} EffectCameraDebugScratch;

u32 effUpdateCameraSnapshotGuides(EffectCameraSnapshotView *state, s8 useBattlePosition);

u32 effLoadFileSlotAndPoll(void);

s32 effRunWithStateBackup(void);

extern u32 effUpdateCameraSnapshotGuides(EffectCameraSnapshotView *, s8);

u32 effProcessQueuedFileRecordAndPersistChanges(void);

extern u8 effPartsPreviewReload;

void effResetFileResources(void);

void effBattleResourceDescriptorRelease(void);

void effPollResourceBankSlot(char *path, u32 flags, EffResourceBankSlot *selection);

extern struct BtlResourceNameRecord *effFileQueueNameRecord;

extern const char D_003BCF30[];

void effInitializeResourceQueue(void);

u32 effPollResourceQueue(void);

void effReleaseQueuedResourceName(void);

void effQueueResource(const char *extension, const char *resourceName);

void effUpdateResourceQueue(const char *directoryPath, const char *extension, EffQueueRecord *record);

void effReleaseResourceBankDescriptors(void);

typedef struct EffectPollRecord {
    char fullName[0xC8]; /* Formatted path or selected built-in name. */
    s32 type;   // 0xC8
    s32 selectionStatus;  // 0xCC
    s32 pathVariant; // 0xD0
} EffectPollRecord;

void effPollResourceBank(s32 flags, EffectPollRecord *record);

extern EffQueuedFileObject effMappingState;

void effResetMappingFlags(void);

extern EffEditorRow D_0038F7D8[];

extern EffEditorRow D_0038F718[];

extern EffEditorRow D_0038F658[];

/* The effect mapping dispatcher stores its data pointer at +0x0C. */
typedef struct EffMappingRequest {
    u8 pad_00[0x0C];
    s32 object;          // 0x0C
} EffMappingRequest;

/* Parameters passed to the mapping dispatcher from distinct object channels. */
typedef struct EffMappingObject {
    u8 pad_00[0x34];
    s32 value34;       // 0x34
    u32 value38;       // 0x38
    s32 value3C;
    u8 pad40[0xC];
    u32 value4C;
    u8 pad50[0x20];
    u32 value70;       // 0x70
    s32 value74;
    u8 pad78[8];
    u32 value80;       // 0x80
    u32 value84;       // 0x84
    u8 pad_88[4];
    s32 value8C;       // 0x8C
    u8 pad_90[0x28];
    u32 valueB8;       // 0xB8
    u8 pad_BC[0x18];
    s32 valueD4;       // 0xD4
    s32 valueD8;       // 0xD8
    u8 pad_DC[0x18];
    s32 valueF4;       // 0xF4
} EffMappingObject;

s32 effMapObjectWithTemporaryMappingTable(s32 request);

/* Edit the request's G-/K-family COLOR tracks; return the native editor status. */
s32 effEditGkColorTrack(s32 request);

/* B-/F-family COLOR callback: edit the request's color and adjacent alpha tracks. */
s32 effEditBfColorTrack(s32 request);

/* HELPER's COLOR callback edits the shared default color/alpha data. */
s32 effEditHlpColorTrack(void);

/* Edit T-BOARD color/alpha tracks using the alternate property-option table. */
s32 effEditTBoardColorTrack(s32 request);

/* Pass the E-RAIN request's color/alpha tracks and frame bound to the editor. */
s32 effEditRainColorTrack(s32 request);

/* Pass the E-FLARE request's color/alpha tracks and frame bound to the editor. */
s32 effEditFlareColorTrack(s32 request);

/* COLOR property callback shared by the native S-family descriptors. */
s32 effEditSColorTrack(s32 request);

/* Edit R-NOMAL/R-CRCLE COLOR tracks, then restore the default option table. */
s32 effEditRColorTrack(s32 request);

/* COLOR property callback for the native R-TRACK descriptor. */
s32 effEditRTrackColorTrack(s32 request);

/* D-family COLOR callback; its DDS2 twin also serves O-family descriptors. */
s32 effEditDoColorTrack(s32 request);

s32 effResolveMappingWithTemporaryTable(s32 request);

/* COLOR callback shared by the native N-SPIRL/N-CYLND/N-SPHRE descriptors. */
s32 effEditNColorTrack(s32 request);

/* Edit COLOR in the N-family descriptors' nested POLYGON data. */
s32 effEditNPolygonColorTrack(s32 request);

/* Edit the root U-CYLND color and alpha tracks selected by the request. */
s32 effEditUCylinderColorTrack(s32 request);

/* Edit COLOR in U-CYLND's nested POLYGON data. */
s32 effEditUCylinderPolygonColorTrack(s32 request);

/* U-SMOKE/U-SPIRL COLOR uses its own property options; forward editor status. */
s32 effEditUSmokeSpiralColorTrack(s32 request);

/* Edit U-SPHRE's root COLOR tracks using its frame bound. */
s32 effEditUSphereColorTrack(s32 request);

/* COLOR callback for the CRACK polygon descriptor. */
s32 effEditCrackPolygonColorTrack(s32 request);

/* COLOR callback for the FLASH polygon descriptor. */
s32 effEditFlashPolygonColorTrack(s32 request);

/* 2D's COLOR callback restores its property options and forwards editor status. */
s32 effEditTwoDimensionalColorTrack(s32 request);

void btlResetEffectWork(void);

extern s8 dds3PadBitTable[];

extern EffScalarTrack D_003DFA78;

extern f32 D_003BD164;

extern f32 D_003BD168;

extern s32 D_003BD16C;

extern char D_003BD190[];

extern void btlDebugDrawRectangle(s32, s32, s32, s32);

extern void btlDebugDrawTwoColorRectangle(s32 x, s32 y, s32 width, s32 height, u32 color0, u32 color1);

extern void btlDebugDrawMappingColorSpan(s32 x, s32 y, s32 width, s32 height, u32 color0, u32 color1);

extern void btlDebugDrawLine2D(s32 x0, s32 y0, s32 x1, s32 y1, u32 color);

extern void btlDebugDrawPoint2DNoOp();

s32 effEditScalarTrack(EffScalarTrack *track, u32 frames, void *panelWork, void *subPanelWork);

/* Edit the requested P-family SCALE track; return the native editor status. */
s32 effEditPScaleTrack(BaObj *p);

/* Edit the requested P-family ROTATE track with its independent editor state. */
s32 effEditPRotationTrack(BaObj *p);

/* SCALE property callback shared by native G-/K-family descriptors. */
s32 effEditGkScaleTrack(BaObj *p);

/* ROTATE property callback for native K-family descriptors. */
s32 effEditKRotationTrack(BaObj *p);

/* SCALE property callback for native B-family descriptors. */
s32 effEditBScaleTrack(BaObj *p);

/* B-family ROTATE callback; identical forwarding code does not imply P-family SCALE. */
s32 effEditBRotationTrack(BaObj *p);

/* AREA property callback shared by B-/F-family descriptors. */
s32 effEditBfAreaTrack(BaObj *p);

/* Pass the requested T-BOARD SCALE track and its frame bound to the editor. */
s32 effEditTBoardScaleTrack(BaObj *p);

/* Pass the requested T-BOARD ROTATE track and its frame bound to the editor. */
s32 effEditTBoardRotationTrack(BaObj *p);

/* SCALE property callback for native W-family descriptors. */
s32 effEditWScaleTrack(BaObj *p);

/* ROTATE property callback for native W-family descriptors. */
s32 effEditWRotationTrack(BaObj *p);

/* Edit the SCALE_UV curve in U-CYLND's POLYGON child; forward editor status. */
s32 effEditUCylinderPolygonUvScaleTrack(BaObj *p);

/* Edit the shared U-SMOKE/U-SPIRL root SCALE curve; forward editor status. */
s32 effEditUSmokeSpiralScaleTrack(BaObj *p);

/* Edit the shared U-SMOKE/U-SPIRL root ROTATE curve; forward editor status. */
s32 effEditUSmokeSpiralRotationTrack(BaObj *p);

/* Edit SCALE_UV in their POLYGON children; forward the numeric editor's status. */
s32 effEditUSmokeSpiralPolygonUvScaleTrack(BaObj *p);

/* Edit the native U-SPHRE root SCALE curve; forward editor status. */
s32 effEditUSphereScaleTrack(BaObj *p);

/* Edit the native 2D root SCALE curve; forward editor status. */
s32 effEditTwoDimensionalScaleTrack(BaObj *p);

/* Edit the native 2D root ROTATE curve; forward editor status. */
s32 effEditTwoDimensionalRotationTrack(BaObj *p);

/* Reset the color/alpha editor's selected curve key to one, not a pending flag. */
void effResetCurveEditorSelection(void);

extern SdfAlphaTrack effColorEditAlphaTrack;

extern u8 D_0038FB38[];

extern s32 effColorEditFadeInFrame;

extern s32 effColorEditFadeOutFrame;

extern char effColorEditFrameCountFormat[];

extern void btlDebugDrawMappingColorSpan(s32, s32, s32, s32, u32, u32);

/* Edit the alpha fade interval and its four-breakpoint graph. */
s32 func_002BB188(SdfAlphaTrack *range, s32 frames);

/* ALPHA callback for POLYGON children of the native S-family descriptors. */
s32 effEditSPolygonAlphaTrack(s32 work);

/* D-family POLYGON FADE callback, shared with O in DDS2; retain bound plus one. */
s32 effEditDoPolygonFadeTrack(s32 work);

/* FADE callback for N-family POLYGON children; preserve bound plus one. */
s32 effEditNPolygonFadeTrack(s32 work);

/* FADE callback for the native U-CYLND POLYGON child. */
s32 effEditUCylinderPolygonFadeTrack(s32 work);

/* FADE callback for the native U-SPHRE POLYGON child. */
s32 effEditUSpherePolygonFadeTrack(s32 work);

/* Poll record from the resource-bank queue; the path variant selects its entry. */
extern void effPollResourceBank(s32, EffectPollRecord *);

s32 effPollFileQueueRecord(s32 request);

/* Poll queued TMX/P2A/general resources with the TLP format enabled. */
s32 effPollResourceQueueWithTlp(void);

/* Separate native descriptor callback with the same TLP-enabled queue filter. */
s32 effPollAlternateResourceQueueWithTlp(void);

/* Poll the queue without adding the TLP flag to its format mask. */
s32 effPollResourceQueueWithoutTlp(void);

extern u16 effClassifyResourceMask(s32);

typedef struct EffFileResourceRecord {
    char *name;
    u8 pad4[4];
    u16 mode;
    u8 padA[2];
    u8 *buffer;
    u32 size;
    struct sdfMemHandle_t *allocation;
} EffFileResourceRecord;

s32 effPollFileRecord(char *path, s32 flags);

/* Poll the TEXTURE property chooser in /tool/effect/mat/ and return its status. */
s32 effPollTextureResourceFile(void);

/* Alternate descriptor family's TEXTURE callback; retain its separate entrypoint. */
s32 effPollAlternateTextureResourceFile(void);

/* Poll EP resources from /tool/effect/ep/ and return the chooser status. */
s32 effPollEpResourceFile(void);

/* Poll EPL resources from /tool/effect/ and return the chooser status. */
s32 effPollEplResourceFile(void);

/* Separate native EP descriptor callback using the same directory and format. */
s32 effPollAlternateEpResourceFile(void);

/* TMX-only TEXTURE chooser for B-/F-family MATERIAL groups. */
s32 effPollBfMaterialTmxFile(void);

/* TMX-only TEXTURE chooser for N-family MATERIAL groups. */
s32 effPollNMaterialTmxFile(void);

/* TMX-only TEXTURE chooser for S-RAD_T/SPK_T/RNG_T/SCT_T MATERIAL groups. */
s32 effPollSTMaterialTmxFile(void);

u32 effFileJobSecondaryDataSet(void);

/* Poll PB resources from /tool/effect/hlp/ and return the chooser status. */
s32 effPollHlpPbResourceFile(void);

typedef struct EffectFileHeader {
    u8 unk_00[8];
    u16 mode;
    u8 unk_0A[2];
    u32 start;
    u32 length;
} EffectFileHeader;

extern void effResetFileResourceManager(void);

extern EffectFileHeader D_0038DC08;

extern EffectFileHeader D_00383F18;

extern EffectFileHeader D_0038C758;

u32 fileLoadEffectSlotA(void);

extern u8 D_00384A08[] __attribute__((aligned(4)));

u32 fileLoadEffectSlotHelp(void);

u32 fileLoadEffectSlotB(void);

extern EffectFileHeader D_0038D470;

u32 effLoadFileSlotF2(void);

u32 effLoadMaterialFile(void);

void effResetFileResourceManager(void);

u16 effClassifyResourceMask(s32 flags);

EffectList *mnuAllocateValueRecord(u32 mode);

void effDestroyEffectList(EffectList *list);

u32 mnuGetValueRecordOwner(const EffectList *list);

s32 effAppendListEntry(EffectList *list, u32 value, u32 length,
                          u32 kind, u32 reference);

s32 effRemoveListEntry(EffectList *list);

/* Drives the head request of the list: mode 0 asks for a resource by name, modes 1 and 2 stream a package and hand each
 * finished job's buffer to the entry's destination. Returns the remaining entry count. */
s32 effPollResourceList(EffectList *list);
/* End of declarations from src/dds1/game/code_0029C530.c */

s32 effAppendListEntry(EffectList *list, u32 value, u32 length,
                          u32 kind, u32 reference) {
    EffectListNode *node = sdfChipAllocDirect(sizeof(EffectListNode));
    memset(node, 0, sizeof(EffectListNode));
    node->next = NULL;
    node->kind = kind;
    node->value = value;
    node->length = length;
    node->reference = (void **)reference;
    if (list->last == NULL) {
        list->first = node;
        list->last = node;
    } else {
        list->last->next = node;
        list->last = node;
    }
    return ++list->count;
}

s32 effRemoveListEntry(EffectList *list) {
    EffectListNode *node = list->first;
    EffectListNode *next = node->next;
    sdfChipFreeDirect(node);
    list->first = next;
    if (--list->count == 0) {
        list->last = NULL;
    }
    return list->count;
}

/* Drives the head request of the list: mode 0 asks for a resource by name, modes 1 and 2 stream a package and hand each
 * finished job's buffer to the entry's destination. Returns the remaining entry count. */
s32 effPollResourceList(EffectList *list) {
    EffectListNode *node = list->first;
    PacWork *item;
    struct sdfMemHandle_t *allocation;

    if (node != NULL) {
        switch (list->mode) {
        case 0:
            if (node->state == 0) {
                effRequestResourceByMode(node->value, node->length, node->kind, node->reference);
                node->state = 1;
            } else if (*node->reference != NULL) {
                effRemoveListEntry(list);
            }
            break;
        case 1:
        case 2:
            if (node->state == 0 && node->length != 0) {
                if (list->request != NULL) {
                    pacFileHandleFree_(list->request);
                }
                list->request = (FilePacRequest *)pacLoad((const char *)node->length);
                if (list->mode == 2) {
                    pacSync((struct FileRequest *)list->request);
                }
                node->state = 1;
            } else if (pacCheckEnd((struct FileRequest *)list->request) != 0) {
                for (item = list->request->packet.queueHead; item != NULL; item = item->next) {
                    if (((SdfPacStreamPacketHeader *)item->packet)->command == 1) {
                        node = list->first;
                        allocation = PAC_WORK_ALLOCATION(item);
                        *node->reference = effCreateResourceSlotSetFromAllocation(allocation, node->kind);
                        if (node->kind == EFF_RESOURCE_SOURCE_TRANSIENT) {
                            sdfMemFreeDirect(allocation);
                        }
                        effRemoveListEntry(list);
                    }
                }
            }
            if (list->count == 0) {
                pacFileHandleFree_(list->request);
                list->request = NULL;
            }
            break;
        }
    }
    return list->count;
}
