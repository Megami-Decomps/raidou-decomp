#include "common.h"

/* Declarations from dds-decomp src/dds1/game/code_0029C530.c */
#ifndef VERSION_DDS1
#define VERSION_DDS1 1
#endif

#include "dds3obj.h"

#include "kernel_draw.h"

#include "libsdf.h"

#include "sdf_resource.h"

#include "sdk_libc.h"

#include "dds/bill_object_api.h"

extern void *memcpy(void *, const void *, size_t);
#include "dds/btl.h"

#include "dds/btl_effect_position.h"

#include "dds/btl_stage_task_cleanup.h"

#include "dds/dds3_draw_buffer.h"

#include "dds/dds3_fade_timing.h"

#include "dds/dds3_path.h"

#include "dds/dds3_world_primary.h"

#include "dds/dds3obj.h"

#include "dds/eff.h"

#include "dds/eff_blur.h"

#include "dds/eff_random.h"

#include "dds/eff_random_mod.h"

#include "dds/evt_unit.h"

#include "dds/evt_unit_color_transition.h"

#include "dds/file_pac_completion_api.h"

#include "dds/file_request_api.h"

#include "dds/file_slot.h"

#include "dds/fld_transfer_chunk.h"

#include "dds/mdl.h"

#include "dds/sdf.h"

#include "dds/sdf_asset_packets.h"

#include "dds/sdf_asset_state.h"

#include "dds/sdf_chip.h"

#include "dds/sdf_chunk.h"

#include "dds/sdf_dev_state.h"

#include "dds/sdf_heap_api.h"

#include "dds/sdf_packet_alloc.h"

#include "dds/sdf_packet_append.h"

#include "dds/sdf_packet_list.h"

#include "dds/sdf_quaternion.h"

#include "dds/sdf_texture_draw_packet.h"

#include "dds/sdf_texture_file.h"

#include "dds/sdf_trig.h"

#include "dds/sdf_vu_matrix.h"

#include "dds/sdf_vu_matrix_multiply_api.h"

#include "dds/sdf_vu_rotation_axes.h"

#include "dds/sdk_libc.h"

#include "dds/sdk_libm.h"

/* Convert three Euler angle components to the VU0 quaternion output.
 * Providers: DDS1 game/code_002E7C20.c and DDS2 game/code_00340AC8.c. */
void FromEuler(f32 x, f32 y, f32 z);

/* Multiply the quaternions in vf10 and vf11, writing the product to vf10. */
void Mult(void);

/* Set vf28-vf31 to vf28-vf31 * vf24-vf27 in place. */
void MultMatrixRevNoArg(void);

/* Load axis-angle rotation into VU0 vf28-vf31. The angle-first interface
 * follows the native Flash caller at DDS2 00171FF8..00172000; EE's separate
 * f12/a0 argument registers alone do not distinguish the two source orders. */
void SetRotateMatrixAxis(f32 angle, const RwV3d *axis);

/* Load the X-axis rotation matrix into the primary VU0 matrix registers. */
void SetRotateMatrixX(f32 angle);

/* Load the secondary Y-axis rotation matrix into the primary VU0 matrix registers. */
void SetRotateMatrixY_B(f32 angle);

/* Convert the quaternion in vf10 into the primary matrix bank vf28-vf31.
 * Providers: DDS1 game/code_002E7C20.c and DDS2 game/code_00340AC8.c. */
void ToMatrix(void);

f32 acosf(f32 x);

f32 atan2f(f32 y, f32 x);

f32 atanf(f32 x);

dds3PathBasicControl_t *dds3AllocPathBasicWorkData(struct dds3Basic_t *object);

struct billHandle_t *dds3BillboardCopy(const struct billHandle_t *source);

/* The selected kind interprets the opaque payload word. */
struct billHandle_t *dds3BillboardCreate(s32 kind, u32 data);

void dds3BillboardDelete(struct billHandle_t *obj);

struct billHandle_t *dds3BillboardGeneral(s32 index);

s32 dds3BillboardGetAnimFrame(const struct billHandle_t *billboard);

struct sdfTexHandle_t *dds3BillboardGetGeneralTex(s32 index);

void dds3BillboardSetAnimColorAdvMode(struct billHandle_t *billboard);

void dds3BillboardSetAnimKey(struct billHandle_t *billboard, u32 frame);

void dds3BillboardSetAnimKey2(struct billHandle_t *billboard, u32 frame);

void dds3BillboardSetMode(struct billHandle_t *billboard, s32 mode);

void dds3BillboardSetRGBA(struct billHandle_t *billboard, u32 parameter);

/* Radian rotation of the billboard quad in its plane. */
void dds3BillboardSetRotate(struct billHandle_t *billboard, f32 angle);

void dds3BillboardSetScale(struct billHandle_t *billboard, f32 x, f32 y);

void dds3DeletePathControl(dds3PathBasicControl_t *path);

void dds3DeleteSceneBasicData(dds3Basic_t *node);

/* Factories accept the serialized parameter prefix used by effect callbacks. */
EffBlurScatterWork *dds3EffectDistortionBlurCreate(dds3Effect_DistortionBlurParam_t *params);

void dds3EffectDistortionBlurDelete(EffBlurScatterWork *work);

EffBlurTemplate *dds3EffectPartialMultiBlurCreate(const dds3Effect_PartialMultiBlurParam_t *body);

void dds3EffectPartialMultiBlurDelete(EffBlurTemplate *owner);

EffResourceRectWork *dds3EffectPartialMultiFilterCreate(dds3Effect_PartialMultiFilterParam_t *params);

void dds3EffectPartialMultiFilterDelete(EffResourceRectWork *work);

EffBlurScaleWork *dds3EffectRippleBlurCreate(dds3Effect_RippleBlurParam_t *params);

void dds3EffectRippleBlurDelete(EffBlurScaleWork *work);

dds3Basic_t *dds3GetActiveCameraSceneBasic(dds3Basic_t *world);

/* Return the borrowed primary world node, or NULL when no world is active. */
struct dds3Basic_t *dds3GetActiveScene(void);

/* Return the current frame draw-buffer slot (0 or 1). */
u32 dds3GetFrameID(void);

/* Advance caller-owned state, returning its next 32-bit value. NULL selects
 * the shared effect RNG state. */
u32 dds3GetRandA(EffRandState *state);

/* Returns 24 random bits scaled to [0, 1); NULL selects the shared state. */
f32 dds3GetRandFloatA(EffRandState *state);

/* Return the next caller-owned or shared state value reduced modulo `modulus`. */
u32 dds3GetRandIntA(EffRandState *state, u32 modulus);

void dds3PathBasicControlGetPos(dds3PathBasicControl_t *path);

void dds3PathBasicControlGetRot(dds3PathBasicControl_t *path);

/* Configure the foreground fade timing mode and frame duration. */
void dds3SetQuake(s32 mode, s32 frames);

/* Disable configured timing while retaining the stored parameters. */
void dds3StopQuake(void);

/* Leaves the unit's transformed muzzle position in vf10. */
void effBTLModelGetSphereCenter(struct BtlUnit *unit);

void effPCPHelperBEDGetModel(FileSlotTable *record);

void evtSetUnitColorRGB(evtUnitHandle_t *unit, s32 duration, u32 color);

/* Initialize an event unit's color transition for the requested duration. */
void evtSetUnitLightAndAmbient(evtUnitHandle_t *unit, s32 duration, u32 firstColor, u32 secondColor);

void evtSetUnitLightDir(evtUnitHandle_t *unit, EvtUnitDirectionTransitionFrames durationFrames);

void evtUnitLightOn(evtUnitHandle_t *unit);

/* Relocate resource words in buffer using this read-only transfer descriptor. */
void fldAddressResolver(u8 *buffer, const FldTransferChunk *chunk);

u16 mdlGetMajorNum(const dds3ModelHandle_t *ctx);

u16 mdlGetMinorNum(const dds3ModelHandle_t *ctx);

/* The referenced halfword is promoted to a word-sized SDK result. */
s32 mdlGetNumOfCut(const dds3ModelHandle_t *ctx, s32 searchId);

/* VU helpers transfer vectors through vf10 and basis rows through vf28-vf30. */
void mdlGetPosition(dds3ModelHandle_t *ctx);

/* Allocate a distinct caller-owned context linked to the registered group.
 * Contexts share the group node and assets; release each context with mdlFree. */
dds3ModelHandle_t *mdlLoad(u32 group, u32 id);

/* sdfMotionCtrlr_t-record selection and lookup. */
void mdlSingleMotion(dds3ModelHandle_t *ctx, s32 searchId, s32 motionIndex);

extern void *memcpy(void *, const void *, unsigned int);

void *memset(void *dst, s32 value, u32 size);

FilePacRequestCompletionResult pacCheckEnd(const struct FileRequest *request);

/* These title-specific wrappers forward an opaque stage-task owner and its
 * handled/not-handled result. */
s32 pacFileHandleFree_(void *request);

struct FileRequest *pacLoad(const char *requestName);

void pacSync(struct FileRequest *request);

SdfTextureDrawPacket *sdf2DLinkTexPacket(sdfDrawTag_t *packetList, sdfTexHandle_t *texture, s32 context);

void sdf3DPrimLinkMat(sdfDrawTag_t *packetList, sdfMat_t *asset,
                              s32 (*allocatePacket)(s32));

void sdf3DPrimLinkTm(sdfDrawTag_t *packetList,
                           s32 (*allocatePacket)(s32));

void *sdfChipAllocDirect(s32 size);

void *sdfChipAllocZeroDirect(s32 size);

void sdfChipFreeDirect(void *memory);

f32 sdfCos(f32 angle);

void sdfDrawTagInit(struct sdfDrawTag_t *list);

void sdfDrawTagLink(struct sdfDrawTag_t *list, u32 packetAddress);

void sdfDrawTagLinkRef(struct sdfDrawTag_t *list, u32 packetAddress);

void sdfDrawTagLinkRefGif(struct sdfDrawTag_t *list, u32 packetAddress);

void sdfFileClose(sdfFileCmdHandle_t *state);

u32 sdfFileLength(sdfFileCmdHandle_t *state);

/* Stable state APIs. Callback factories keep caller-specific callback types. */
sdfFileCmdHandle_t *sdfFileOpen(const char *path);

void sdfFileRead(sdfFileCmdHandle_t *state, void *buffer, s32 byteCount);

/* Reserve size bytes from the active packet buffer, rounded up to 16 bytes.
 * Returns the prior cursor as a 32-bit address; the buffer owns this storage. */
s32 sdfGrAllocPacketBuff(s32 size);

/* Queue an asset owner for the deferred sdfMat_t release callback. */
void sdfMatFreeDelayed(struct sdfMat_t *asset);

/* Write the reference header for one buffered asset draw entry and return
 * the packet payload immediately after the header. */
struct SdfDmaTagHeader *sdfMatMakePacket(struct sdfMat_t *asset,
                                                           struct SdfDmaTagHeader *tag,
                                                           s32 frame);

void sdfMatSetConstant(struct sdfMat_t *asset, f32 value);

void sdfMatSetMultiMethod(struct sdfMat_t *asset, u32 packetMode);

void sdfMatSetMultiTex(struct sdfMat_t *asset, struct sdfTexHandle_t *texture);

/* Store the primary or secondary texture owner and mark its draw state dirty. */
void sdfMatSetTex(struct sdfMat_t *asset, struct sdfTexHandle_t *texture);

/* Refresh one of the two draw entries and retain the other entry's dirty bits. */
void sdfMatUpdatePacket(struct sdfMat_t *asset, s32 entryIndex);

struct sdfMemHandle_t *sdfMemAllocDirect(s32 requestedBytes);

void sdfMemFreeDirect(struct sdfMemHandle_t *allocation);

void *sdfMemLock(struct sdfMemHandle_t *allocation);

u32 sdfModelGetMarkPosCount(const struct dds_sdfModelRootNode_t *model);

f32 sdfSin(f32 angle);

void sdfTexFreeDelayed(sdfTexHandle_t *texture);

/* Both entry points return one caller-owned texture reference. A nonzero
 * resourceKey reuses a live matching texture and increments its count; a new
 * texture starts at one. Release the returned reference once with
 * sdfTexReleaseReference or sdfTexFreeDelayed. */
/* For a new texture, copy payload bytes into queue-owned storage before return. */
struct sdfTexHandle_t *sdfTexSet(const SdfTextureFileHeader *header);

char *strcpy(char *dst, const char *src);

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

extern u32 D_r1_004035D0[];

extern sdfGrOt_t *D_r1_00403610[];

extern sdfGrOt_t *D_r1_004037A8[];

extern u32 D_0037EB30[];

extern sdfGrOt_t *D_0037EB78[];

#include "dds/bill_object_api.h"

extern char D_r1_00456E58[];

extern char D_003B39E0[];

extern char D_003B3B88[];

extern char D_003B3BA0[];
extern char D_r1_00456E70[];
extern char D_r1_00457028[];
extern char D_r1_00457040[];

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

extern struct sdfMemHandle_t *D_r1_0046D14C;

extern u8 D_r1_00415880[];

extern u8 D_r1_00415E10[];

extern u8 *D_r1_00403508[];

extern FileJobPayload *effQueuedFileHandle;

extern struct sdfDrawTag_t *effCurrentRenderPacket;

extern s32 effSharedRibbonReferenceCount;

extern u32 D_r1_0046D148;

extern struct EffExpandedList *effCreateTextureAnimationSet(u32);

extern struct SdfTextureFileHeader *effFlashTextureHandles;

extern struct EffModelResource *effCreateModelResourceWithInlineData(u16, void *, void *, u32);

extern EffSurfaceGridNode *effCreateSurfaceGridNode(u32, u32);

extern void effFillSurfaceGridColorGradient(EffSurfaceGridNode *, u32 *);

extern void effReleaseSurfaceGridBuffers(EffSurfaceGridNode *);

extern s32 dds3BillboardGeneralNumber(void);

extern sdfMat_t *sdfMatNew(void);

extern sdf3DPrimPolyParm_t D_r1_004D1BC0;

extern EffTrackSet *effCreateTrackSetWithSharedReferences(u32, u32, u32);

extern void effReleaseResourceRefs(EffTrackSet *);

extern void dds3EffectFilter(void *);

extern f32 mnuMeasureProjectedPerpendicularDistance(f32);

extern void effBlurStepScaleSlotsAndDraw(void *);

extern s32 effRequestResourceByMode(u32, u32, u32, void **);

extern u32 D_r1_0046D108;

extern u32 D_r1_0046D10C;

typedef struct EffectObjectFlag {
    u32 state;
    u8 pad04[8];
    u32 flags;
} EffectObjectFlag;

extern EffectObjectNode *effFloorModelListHead;

extern u8 btlIsRuntimeAllocated(void);

void effDrawThreePointGroups(EffPointSet *, Matrix4 *);

extern u32 effPartsListScroll;

extern u32 D_r1_00416D44[];

extern s32 D_r1_0046D128[2];

extern RefObj *D_r1_0046D130[2];

extern u32 effSharedTextureReferenceCount;

extern s32 D_r1_0046D170[2];

extern s32 D_r1_0046D178[2];

extern u8 D_r1_0046D180[2];

extern u8 D_r1_0046D188[2];

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

extern u8 D_r1_00417378[];

extern struct BtlResourceNameRecord *effQueuedResourceNameRecord;

extern void effReleaseQueuedResourceName(void);

extern const char D_003BD088[];

extern const char D_003BD090[];

extern char D_r1_004D54F0[];

extern u8 D_0038F2B8[];

extern u8 effBattleOffset[];

extern u8 *D_003BD074;

extern void effDrawSurfaceNode(s32);

struct EffCameraCreateRow;

extern u32 effSelectCameraFileJobFromRows(const char *, const struct EffCameraCreateRow *, u32);

extern char D_003B3968[];

extern char D_r1_00456DD8[];

extern char D_003BCA40[];

extern char D_003BD050[];

extern char D_r1_00456D48[];

extern char D_003B3938[];

extern char D_r1_00456DA8[];

extern char D_r1_00456D98[];

extern char D_r1_00456D88[];

extern char D_r1_00456DE8[];

extern char D_r1_00456D68[];

extern char D_r1_00456D58[];

extern char D_003B3888[];

extern char D_003BD000[];

extern u8 D_0038DE70[];

extern u8 D_r1_004157D0[];

extern u8 D_r1_00415970[];

extern u8 D_r1_00415A20[];

extern u8 D_r1_00415B20[];

extern u8 D_0038E240[];

extern u8 D_r1_00415BB0[];

extern u8 D_r1_00415CF0[];

extern u8 D_r1_00415DA0[];

extern u8 D_r1_00416320[];

extern u8 D_r1_00415FA0[];

extern u8 D_r1_00416070[];

extern u8 D_0038E800[];

extern u8 D_r1_004162E0[];

extern u8 D_003DF840[];

extern u8 D_r1_004D5650[];

extern u8 D_003BD108;

extern u8 D_003BD109;

extern u8 D_003BD10A;

extern s32 effEditColorTrack(s32, s32, s32);

extern u8 D_r1_004D3418[] __attribute__((aligned(4)));

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

extern EffKindDesc D_r1_00403080[];

extern EffKindDesc D_r1_004030F8[];

/* VU0 model helpers consume vf10 directly, matching the original macro-mode setup. */
extern void effDrawScalyTexturedStrip(EffStripWork *, void *);

extern u16 D_003BC944;

extern void effInitModelVUState(dds3ModelHandle_t *);

extern s32 effComputeLightDirectionVU(dds3ModelHandle_t *, sdf3DLightParallelPacket_t *);

extern char D_r1_00455D00[];

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

void effSetFadeVectorColor(EffFadeVectorWork *work, u32 color);

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
void effSetSelectionFlagListColor(SdfFlagListWork *work, u32 color);

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

void effSetKindWorkColor(EffKindWork *work, u32 color);

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

void effSetBillboardColor(EffBillboardWork *work, u32 color);

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

extern struct SdfTextureFileHeader *D_r1_0046D124;

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

extern struct sdfMemHandle_t *D_r1_0046D118[2];

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

void effSetClassWorkScale(EffClassWork *work, f32 scale);

extern sdf3DPrimPolyParm_t D_r1_004D1BF0[];

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

void effSetSurfaceNodeColor(EffectSurfaceNode *work, u32 color);

void effIndexedFloatCallbackDispatch(ValPtr44 *p, float v);

extern sdf3DPrimPolyParm_t D_r1_004D1C20;

extern sdf3DPrimPolyParm_t D_r1_004D1C50;

extern u8 D_r1_004034A0[];

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

void Delete_effPolygonTex(EffQuadWork *work);

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

extern sdf3DPrimPolyParm_t D_r1_004D1CB0[];

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

void effSetStripRecordColor(EffectStripNode *work, u32 color);

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

extern sdf3DPrimPolyParm_t D_r1_004D1CE0;

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

void effSetModelResourceColor(EffModelResource *work, u32 color);

void effSetModelResourceScale(EffModelResource *work, f32 scale);

extern sdf3DPrimPolyParm_t D_r1_004D1D40[];

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

extern u128 D_r1_004D1DB0[];

extern s8 D_r1_0046D169;

extern u128 D_r1_004D1DC0[];

extern u128 D_r1_004D1D80[];

extern sdf3DLight_t *D_r1_004037C0[3];

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
void effSetParticleSharedColor(EffParticleSharedResource *work, u32 color);

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

extern u32 D_r1_00416484[];

u32 effReinitializeFileQueue(void);

extern u32 D_r1_00416554[];

u32 effResetFileQueueState(void);

extern u32 D_r1_004164EC[];

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

extern EffEditorRow D_r1_00416650[];

extern EffEditorRow D_r1_00416698[];

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

extern u8 D_r1_00416F38[];

extern s8 D_r1_0043EED9[];

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

extern EffectCameraDebugState D_r1_00416FC8;

extern s8 D_r1_0043EED9[];

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

extern EffEditorRow D_r1_00417158[];

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
    u8 pad_F8[0x10];
    s32 value108;      // 0x108
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

extern EffectFileHeader D_r1_00414EA8;

extern EffectFileHeader D_r1_00408D60;

extern EffectFileHeader D_r1_00411E28;

u32 fileLoadEffectSlotA(void);

extern u8 D_r1_00409908[] __attribute__((aligned(4)));

u32 fileLoadEffectSlotHelp(void);

u32 fileLoadEffectSlotB(void);

extern EffectFileHeader D_r1_00414710;

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

/* Declarations from dds-decomp src/dds1/game/code_002CC750.c */
#ifndef VERSION_DDS1
#define VERSION_DDS1 1
#endif

#include "newdata.h"

#include "dds/fr_font.h"

#include "dds/mdl_flag_api.h"

#include "dds/sdf_heap_api.h"

#include "dds/sdf_line_pair_packet_api.h"

#include "dds/sdf_projection.h"

#include "dds/sdk_libc.h"

extern SdfProjectionRecord dds3ScreenParam;

struct SdfPacket *effDebugDrawWireModel(const f32 vertices[][4], const u32 *colors,
                                                 s32 vertexCount, u32 primitiveFlags);

/* Return whether the selected flag bit is set, as EvtBitTestResult. */
EvtBitTestResult evtBitCheck(s32 flag);

/* Set and clear one flag using the providers' signed-index adjustment. */
void evtBitOn(s32 flag);

/* Link a glyph chain into the current draw-buffer slot. */
s32 frDestroyFontRequestAfter(struct _FRQ *glyph);

/* Replace each child render word along the borrowed glyph chain. */
void frSetFontColor(struct _FRQ *parentNode, u32 colorWord);

/* Return the descriptor's payload base as a byte address, not the descriptor. */
u8 *sdfMemGetPtr(const struct sdfMemHandle_t *block);

u32 strlen(const char *text);

#include "dds/sdk_ps2.h"

#include "dds/sdf_line_pair_packet_api.h"

#include "dds/dsp_name.h"

#include "dds/sdf_packet_list.h"

#include "dds/common.h"

#include "dds/sdf_vu0.h"

#include "dds/sdf_kernel_thread_param.h"

#include "dds/sdf_thread_workspace_api.h"

#include "dds/eff_random.h"

#include "dds/sdf_heap_api.h"

#include "dds/sdf_vu_matrix.h"

#include "dds/sdf.h"

#include "dds/sdf_semaphore_api.h"

#include "dds/eff_curve.h"

#include "dds/sdf_projection.h"

#include "dds/pcp_vu0.h"

#include "dds/itf.h"

#include "dds/fr_font.h"

#include "dds/dat_state.h"

#include "dds/pty_profile_record_value.h"

#include "dds/pty_skill_nibble_state.h"

#include "dds/file.h"

#include "dds/prf_profile_cap.h"

#include "dds/prf_requirement.h"

#include "dds/prf_requirement_fallback.h"

#include "dds/prf_fallback_item_requirement.h"

#include "dds/mdl_flag_api.h"

#include "dds/script_flag_slot.h"

#include "dds/pty_preset_record.h"

#include "dds/prf_profile_parameters.h"

#include "dds/prf_current_record_view.h"

#include "dds/sdf_packet_alloc.h"

/* Declared before first use (were implicit int). */
extern u16 prfGetSkillAtIndex(u16 profileId, u32 skillIndex);

extern s32 scrSetFlag(dds_datUnitWork_t *work, u16 flagId);

#define SCR_FLAG_ID_MASK 0xFFFF

#define SCR_FLAG_SLOT_INDEX_MASK 7

#define SCR_FLAG_WORD_INDEX_SHIFT 3

#define SCR_FLAG_SLOT_BIT_SHIFT 2

#define SCR_FLAG_PRIMARY_BIT 1U

#define SCR_SKILL_STATE_TWO_BIT 2U

#define SCR_FLAG_SECONDARY_BIT 4U

#define SCR_SECONDARY_FLAG_COUNT 0x260

#define SCR_GLOBAL_FLAG_FIRST_ID 0x1AB

#define SCR_GLOBAL_FLAG_END_ID 0x200

#define SCR_GLOBAL_FLAG_WRAP_BIAS 0xFE55

#define SCR_GLOBAL_FLAG_WORD_SHIFT 5

#define SCR_GLOBAL_FLAG_BIT_MASK 31

#define PTY_SKILL_SLOT_COUNT 24

#define PRF_SKILL_LIST_FLAGGED 4

#define PTY_PRESET_SKILL_SLOT_COUNT 8

#define PTY_PRESET_POOL_SKILL_COUNT 40

#define PTY_PRESET_POOL_EXTRA_GATE 0xB90

#define PTY_PROFILE_UNIT_ID_COUNT 0x10

#define PTY_PROFILE_UNIT_OCCUPIED_BIT 1

#define PRF_REQUIRED_PROFILE_COUNT 8

#define PRF_FALLBACK_GROUP_COUNT 4

#define PRF_FALLBACK_FLAG_COUNT 2

#define PRF_REQUIREMENT_RULES_BIT 4

#define PRF_REQUIREMENT_EXCLUSION_BIT 1

#define SCR_PAIRED_OUTPUT_BYTES 8

#define SDF_FLAG_LIST_VALUE_SHIFT 3

#define SDF_FLAG_LIST_RESET_MARK 0xFFFFFFFF

#define SDF_FLAG_LIST_ENTRY_VERTEX_BYTES 32

#define SDF_FLAG_LIST_VERTICES_PER_ENTRY 2

#define SDF_FLAG_LIST_PACKET_BYTES 0x20

#define SDF_FLAG_SLOT_TABLE_SHIFT 7

#define SDF_FLAG_SLOT_COUNT 16

extern PtyPresetRecord D_00393A80[];

extern void ptyMergeStockSkills(dds_datUnitWork_t *);

extern void (*sdfTickCallback)(void);

extern u32 D_003BD2C8;

extern u32 prfIsRequirementExcluded(u16);

extern DatProfileRecord *ptyGetProfileRecord(const dds_datUnitWork_t *, u16);

extern FrFontTextBank frFontColoredGlyphResource;

typedef struct Entry24W {
    u32 v0;            // 0x00
    u8 pad_0x04[0x14]; // 0x04
} Entry24W; 

extern PrfFallbackItemRequirement D_00393220[];

extern Entry24W D_00393234[];

extern SdkFileStream *fopen(const char *, const char *);

extern s32 fprintf(SdkFileStream *, const char *, ...);

extern void fclose(SdkFileStream *);

extern char sdfDebugLogAppendMode[];

extern char sdfDebugLogPairFormat[];

extern _FRQ *itfDrawBankTextWithLayoutFlags(s32, s32, s32, u16, FrFontTextBank *, s32);

extern s32 frFontDrawGlyphChain(_FRQ *, s8, u32);

extern s32 ptyTestPrimaryProfileFlag(dds_datUnitWork_t *, u16);

s32 ptyGetCurrentProfileId(const dds_datUnitWork_t *);

extern void ptyRecomputeMaxVitals(dds_datUnitWork_t *, const s32 *);

void sdfAppendFormattedDebugLogPair(s32 left, s32 right);

/* Clear the complete per-unit profile-record table in game state. */
void ptyClearProfileRecords(void);

s8 scrSelectOperandIndex(dds_datUnitWork_t *unit, s32 profileId);

extern void ptySetSecondaryProfileFlag(dds_datUnitWork_t *, u16);

PtyProfileRecordValue ptyAddProfilePoints(dds_datUnitWork_t *unit, s32 increment);

/* Pick the highest preset stage whose required profiles all sit at their cap, then apply that stage's script. */
void ptySelectProfileStage(dds_datUnitWork_t *unit);

extern void ptyApplyProfile(dds_datUnitWork_t *, u16);

/* Choose the least-conflicting profile group and install its nonzero IDs. */
void ptyApplyProfilePreset(s32 useCurrentProfile, dds_datUnitWork_t *unit);

/* Load nonzero preset IDs into their original slots; zero IDs leave slots unchanged. */
void ptyLoadPresetSkillSlots(dds_datUnitWork_t *unit);

/* Mark nonzero IDs in the preset pool, then perform the native flag-gated extra call. */
void ptyMarkPresetSkillPool(dds_datUnitWork_t *unit);

extern void ptyRecomputeMaxHpMp(dds_datUnitWork_t *);

extern s32 prfBuildRawSkillList(u16, PrfSkillList *);

extern s32 scrSetFlag(dds_datUnitWork_t *, u16);

extern void scrSetSecondaryScriptFlag(dds_datUnitWork_t *, u16);

extern void scrClearAllSecondaryScriptFlags(dds_datUnitWork_t *);

/* Rebuild skill flags from the preset (no current profile) or from the current profile. */
void ptyRebuildProfileSkills(s32 useCurrentProfile, dds_datUnitWork_t *unit);

/* Rebuild skill lists for the five occupied party slots. */
void ptyRebuildAllProfiles(void);

extern u32 datComputeSkillBoostedMaxHp(dds_datUnitWork_t *unit);

extern u32 datComputeSkillBoostedMaxMp(dds_datUnitWork_t *unit);

/* Recompute effective maxima from the profile plus optional stat gains, then
 * preserve each current resource's proportion of its former maximum. */
void ptyRecomputeMaxVitals(dds_datUnitWork_t *unit, const s32 *statDeltas);

void ptyRecomputeMaxHpMp(dds_datUnitWork_t *unit);

extern DspMantraName *D_003BAA78;

void prfCopyMantraNamePart(u16 scriptId, s32 namePartMode, char *destination);

extern u8 *D_0037CDF8 __attribute__((section(".data")));

s32 prfResolveMantraNameOrFallback(u16 profileId, u8 **out);

/* Read the configured capacity for an unchecked profile ID. */
u32 prfGetCapValue(u16 profileId);

/* Store the configured capacity in an unchecked profile record. */
void ptySetProfileRecordToCap(dds_datUnitWork_t *unitAddress, u16 profileId);

/* Add to the selected record and cap the unsigned result; no selection returns zero. */
PtyProfileRecordValue ptyAddProfilePoints(dds_datUnitWork_t *unit, s32 increment);

void prfDecodeFlagPair(u32 v, u32 *a, u32 *b);

extern ScriptFlagSlot D_00393280[];

/* Search the selected 16-entry table for an exact nonzero byte ID.
 * Set and return its global flag ID; return zero if no entry matches. */
u32 ptySetMappedGlobalFlagForSlot(dds_datUnitWork_t *unit, u32 slotId);

/* Test the first bit of the decoded profile flag pair for this unit. */
s32 ptyTestPrimaryProfileFlag(dds_datUnitWork_t *partyRecord, u16 id);

/* Require the primary profile flag for every row not marked as excluded. */
s32 scrCheckStateBits(dds_datUnitWork_t *unit);

/* Mark the second bit of the decoded mantra pair for this unit. */
void ptySetSecondaryProfileFlag(dds_datUnitWork_t *unit, u16 id);

/* Test the second bit of the decoded profile flag pair for this unit. */
s32 ptyTestSecondaryProfileFlag(dds_datUnitWork_t *partyRecord, u16 id);

s8 scrGetSelectedOperandIndex(dds_datUnitWork_t *op);

/* The party record supplies only the bank index; the returned profile record is writable. */
DatProfileRecord *ptyGetProfileRecord(const dds_datUnitWork_t *unit, u16 profileId);

u32 ptyGetProfileRecordValue(const dds_datUnitWork_t *unit, u16 profileId);

/* Resolve the selected ID using the native low-16-bit conversion. */
const DatProfileRecord *ptyGetCurrentProfileRecord(dds_datUnitWork_t *unit);

s32 ptyGetCurrentProfileId(const dds_datUnitWork_t *op);

/* Store the narrowed selection, mark its paired profile flag, and return the stored ID. */
s8 scrSelectOperandIndex(dds_datUnitWork_t *unit, s32 profileId);

u32 func_002CD7F0(u32 unused, u32 value);

/* Retail mode-1 item predicate: never blocks an entry; itemId is unused. */
u32 mnuShopIsMode1PurchaseBlocked(u32 itemId);

/* Retail mode-1 purchase hook: unused itemId, no mutation, ignored return one. */
u32 mnuShopCommitMode1PurchaseNoOp(u32 itemId);

/* Decode the low 16 bits: eight four-bit flag slots per word.
 * The unused first argument remains part of the existing calling convention. */
void scrDecodePackedFlagIndex(const dds_datUnitWork_t *unused, u32 flagId, u32 *wordIndex, u32 *bitShift);

/* Set bit 0 of the selected nibble; the return value is always 1. */
s32 scrSetFlag(dds_datUnitWork_t *work, u16 flagId);

/* Set the global bit for an ID in the supported half-open range. */
void scrSetGlobalSeenBit(u32 flagId);

/* Return the global bit's raw word mask, not a normalized boolean. */
u32 scrTestGlobalSeenBit(u32 flagId);

/* Set only bit 2 of the selected flag nibble. */
void scrSetSecondaryScriptFlag(dds_datUnitWork_t *work, u16 flagId);

/* Clear only bit 2 of the selected flag nibble. */
void scrClearSecondaryScriptFlag(dds_datUnitWork_t *work, u16 flagId);

/* Clear secondary bits over the fixed ID range, preserving other nibble bits. */
void scrClearAllSecondaryScriptFlags(dds_datUnitWork_t *work);

/* Return bit 2's raw word mask, not a normalized boolean. */
u32 scrGetSecondaryScriptFlag(dds_datUnitWork_t *work, u16 flagId);

/* Bit 1 takes precedence and returns state 2; otherwise return boolean bit 0.
 * The secondary bit does not affect this state. */
PtySkillNibbleState ptyGetSkillNibbleState(const dds_datUnitWork_t *partyRecord, u16 flagId);

/* Search all slots for the low 16-bit ID; zero can match an empty slot. */
s32 ptyHasSkill(dds_datUnitWork_t *unit, s32 skillId);

extern s32 scrFindSlot(dds_datUnitWork_t *unit, u16 skillId);

extern s32 scrRemoveSlot(dds_datUnitWork_t *unit, u16 skillId);

void ptyRemoveProfileSkills(dds_datUnitWork_t *unit, u16 profileId);

/* Return the first exact-ID slot, including zero IDs, or -1 when absent. */
s32 scrFindSlot(dds_datUnitWork_t *unit, u16 skillId);

/* Read a slot's ID, returning zero for an out-of-range unsigned index. */
u16 scrGetSlot(dds_datUnitWork_t *unit, u32 slotIndex);

/* Count nonzero skill IDs across every slot. */
u32 scrCountSlots(dds_datUnitWork_t *unit);

/* Replace an unchecked skill-slot index and return its previous identifier. */
u16 scrSetSlot(dds_datUnitWork_t *unit, s32 slotIndex, u16 skillId);

/* Clear the first exact-ID match and report whether a slot was found. */
s32 scrRemoveSlot(dds_datUnitWork_t *unit, u16 skillId);

/* Read the unchecked profile's label-display byte; DDS1 exposes it as a word. */
s32 prfGetMantraDisplayParameter(u16 profileId);

/* Read the threshold used by the profile-level check. */
u16 prfGetRequiredProfileLevel(u16 profileId);

/* Read byte 5 from the unchecked profile parameter row. */
u8 prfGetProfileParameterByte5(u16 profileId);

u32 prfGetIndexedProfileByte(u16 profileId, s32 statIndex);

/* DDS2 reads an indexed profile byte here; DDS1's native callee returns zero.
 * Retain both selection reads and the no-selection fall-through. */
u32 scrCallIfOperandReady(dds_datUnitWork_t *operand, u32 byteIndex);

/* Read one of the profile row's eight skills; an unsigned out-of-range slot returns zero. */
u16 prfGetSkillAtIndex(u16 profileId, u32 skillIndex);

u32 func_002CDE60(void);

/* Test whether a unit meets the profile's configured level requirement. */
u8 ptyMeetsProfileLevelRequirement(const dds_datUnitWork_t *unit, u16 profileId);

/* Clear the paired output words; the first two arguments are unused. */
void scrClearPairedEntryOutput(u32 unused0, u32 unused1, u32 output);

u8 ptyIsCurrentProfileId(dds_datUnitWork_t *operand, u32 profileId);

s32 prfBuildRawSkillList(u16 profile, PrfSkillList *output);

/* Compact the selected profile's nonzero skills, optionally including nonzero
 * nibble states and marking them in the output. The profile argument is unused;
 * output may be NULL when only the resulting count is needed. */
s32 prfBuildSkillList(dds_datUnitWork_t *unit, const DatProfileRecord *unusedProfile, PrfSkillList *output, s32 includeFlagged);

/* Build the stored selection's list without flagged skills; the second argument is unused. */
u32 prfBuildSkillListState0(dds_datUnitWork_t *unit, const DatProfileRecord *unusedProfile, PrfSkillList *output);

/* Count leading IDs in a DDS1 requirement slot up to the first zero or bound. */
u32 prfCountProfileList(PrfDds1RequirementSlot *operand);

/* Require each listed profile's primary flag; an empty list succeeds. */
s32 ptyHasAllReqProfiles(dds_datUnitWork_t *unit, PrfDds1RequirementSlot *operand);

/* Compare the number of flagged IDs in the list with its minimum count. */
s32 ptyReqProfileCountAtLeast(dds_datUnitWork_t *unit, PrfDds1RequirementSlot *operand);

/* Count flagged profile rows whose byte-five value reaches the operand's threshold. */
s32 ptyProfileCountAtLeast(dds_datUnitWork_t *unit, PrfDds1RequirementSlot *operand);

/* Each listed profile must be flagged in at least one occupied party slot. */
s32 ptyAreReqProfilesInParty(PrfDds1RequirementSlot *operand);

/* Compare the unit's level word with the prerequisite's byte threshold. */
u8 prfReqCheckUnitLevel(dds_datUnitWork_t *unit, PrfDds1RequirementSlot *operand);

/* Test the global currency counter against a prerequisite threshold. */
u8 prfReqCheckGlobalCounter(PrfDds1RequirementSlot *operand);

s32 prfReqEvaluateRules(u32 state, dds_datUnitWork_t *unit, u16 requirementId, u32 *mismatchCount);

/* Evaluate the selected rule set for a party record without mismatch output. */
s32 prfCheckRequirementRules(u32 state, dds_datUnitWork_t *unit, u16 requirementId);

/* Read requirement flags for an unchecked profile ID. */
u32 prfIsRequirementExcluded(u16 requirementId);

/* Read the rule-state word for an unchecked requirement ID. */
u32 prfReq54GetWord1230(u16 requirementId);

typedef struct PrfFallbackGroup {
    u8 id;
    u8 pad01[3];
    u32 flags[2];
} PrfFallbackGroup;

extern PrfFallbackGroup D_003931B0[];

/* A zero evaluator return selects its result; otherwise test the fallback flags.
 * Missing fallback IDs succeed, as do entries with no nonzero flag requirements. */
s32 prfReqCheckWithFallback(dds_datUnitWork_t *operand, u16 requirementId);

/* Select the first active, satisfied prerequisite group whose flag is unset. */
s32 prfSelectSatisfiedUnsetRequirementGroup(u32 start);

/* Read the byte in an unchecked prerequisite-table entry. */
u8 prfReq18GetWord3220(s32 entryIndex);

s32 prfReqGetPair(s32 entryIndex, s32 requirementIndex, s32 *minimum);

/* Read the flag ID in an unchecked prerequisite-table entry. */
u32 prfReq18GetWord3234(s32 entryIndex);

/* Set flags returned by the native selector until it yields a negative index. */
void sdfSetAllFlagsFromTable(void);

PrfDds1RequirementRecord *prfReqGetEntryRecord(u16 profileId);

void frFontQueueColoredGlyph(s32 x, s32 y, u32 first, u16 width, u32 second, s32 option);

FrFontTextBank *frFontGetColoredGlyphResource(void);

/* Reset each entry's timer to free, then clear both colour words per entry. */
void sdfResetFlagListEntries(SdfFlagListWork *work);

SdfFlagListWork *sdfCreateFlagListWork(const SdfFlagListParams *source);

SdfFlagListWork *sdfInitializeFlagListFromResource(void *file);

void sdfReleaseFlagListResource(SdfFlagListWork *work);

extern f32 sdfAtan(f32 ratio);

/* Advance the drop list: respawn a random number of free drops around the camera, move live drops along their
 * own direction, and fade them over their last ten frames. */
void sdfUpdateFlagListEntries(SdfFlagListWork *work);

extern sdfGrOt_t *D_r1_004039D0[];

/* Copy vertex pairs, optionally add the view target, and submit the packet list.
 * count is reused first as an entry count and then as a vertex count. */
void sdfDrawFlagListVertices(SdfFlagListWork *work);

/* Return the drop list's movement speed; this is not a script operand. */
float sdfGetFlagListSpeed(SdfFlagListWork *work);

/* Replace the drop list's movement speed without validation. */
void sdfSetFlagListSpeed(SdfFlagListWork *work, float speed);

/* Sample color/alpha at frame zero with duration zero; discard the return value. */
void sdfSampleFlagListStartColor(SdfFlagListWork *work);

void itfSetPackedRgbAlpha(SdfFlagListWork *p, u32 color);

u32 sdfGetFlagListAlphaSurfaceIndex(SdfFlagListWork *work);

void sdfSetFlagListAlphaSurfaceIndex(SdfFlagListWork *work, u32 surfaceIndex);

void itfCopyColorFields(SdfFlagListWork *dst, const SdfFlagListParams *src);

void sdfEnableFlagListViewTargetRelativeMode(void);

void sdfDisableFlagListViewTargetRelativeMode(void);
/* End of declarations from src/dds1/game/code_002CC750.c */

void func_r1_00242308(void) {
    effResetSlots();
}

INCLUDE_ASM(const s32, "code_00242308", func_r1_00242320);

void func_r1_002423E0(void) {
}

void func_r1_002423E8(void) {
}

void func_r1_002423F0(void) {
}

EffActiveResource *effAllocateResourcePayload(u16 kind, void *source) {
    u32 headerSize = 0x40;
    u32 size = effRuntimeResourceOperations[kind].payloadSize;
    EffActiveResource *effect = sdfChipAllocDirect(size + headerSize);
    effect->payload = (u8 *)effect + headerSize;
    effect->color = 0x80808080;
    effect->scale = 1.0f;
    effect->kind.index = kind;
    effect->frame = 0;
    VU0_STORE_VF($vf0, effect);
    VU0_STORE_VF($vf0, (u8 *)effect + 0x10);
    memcpy(effect->payload, source, size);
    return effect;
}

EffActiveResource *effCreateResourceInstance(u16 kind, void *source, void *secondary, u32 param) {
    EffActiveResource *effect = effAllocateResourcePayload(kind, source);

    if (btlIsRuntimeAllocated() != 0) {
        if (effRuntimeResourceOperations[kind].createResource != NULL) {
            effect->resource = (void *)effRuntimeResourceOperations[kind].createResource(source, secondary, param);
        }
        if (effRuntimeResourceOperations[kind].initialize != NULL) {
            effRuntimeResourceOperations[kind].initialize(effect);
        }
    }
    return effect;
}

EffActiveResource *effCreateActiveResourceFromFile(FileJobPayload *request) {
    void *first = fileResolvePrimaryBuffer(request);
    void *second = fileResolveSecondaryBuffer(request);
    return effCreateResourceInstance(request->option, first, second, request->secondary.size);
}

void effDestroyResourceInstance(EffActiveResource *work) {
    if (btlIsRuntimeAllocated() != 0) {
        void (*callback)(void *) = effRuntimeResourceOperations[work->kind.signedIndex].destroyResource;
        if (callback != NULL) {
            callback(work->resource);
        }
    }
    sdfChipFreeDirect(work);
}

EffActiveResource *effDuplicateActiveResource(EffActiveResource *source) {
    EffActiveResource *effect;
    u32 kind = source->kind.index;

    if (effRuntimeResourceOperations[kind].cloneResource == NULL) {
        effect = effCreateResourceInstance(source->kind.shortIndex, source->payload, 0, 0);
    } else {
        void *resource;
        u32 activeKind;
        effect = effAllocateResourcePayload(source->kind.shortIndex, source->payload);
        resource = (void *)effRuntimeResourceOperations[source->kind.signedIndex].cloneResource(source);
        activeKind = source->kind.index;
        effect->resource = resource;
        if (effRuntimeResourceOperations[activeKind].initialize != NULL) {
            effRuntimeResourceOperations[activeKind].initialize(effect);
        }
    }
    return effect;
}

void effClearCallbackFrame(EffActiveResource *work) {
    if (btlIsRuntimeAllocated() != 0) {
        void (*callback)(void *) = effRuntimeResourceOperations[work->kind.signedIndex].initialize;
        if (callback != NULL) {
            callback(work);
        }
        work->frame = 0;
    }
}

INCLUDE_ASM(const s32, "code_00242308", func_r1_00242748);

void effDispatchEnabledCallback(EffActiveResource *work) {
    if (btlIsRuntimeAllocated() != 0) {
        void (*callback)(void *) = effRuntimeResourceOperations[work->kind.signedIndex].draw;
        if (callback != NULL) {
            callback(work);
        }
    }
}

void func_r1_00242748(EffActiveResource *work);

void func_r1_00242820(EffActiveResource *work) {
    func_r1_00242748(work);
    effDispatchEnabledCallback(work);
}

INCLUDE_ASM(const s32, "code_00242308", func_r1_00242848);

INCLUDE_ASM(const s32, "code_00242308", func_r1_00242858);

INCLUDE_ASM(const s32, "code_00242308", func_r1_00242870);

INCLUDE_ASM(const s32, "code_00242308", func_r1_00242878);

/* Reset each entry's timer to free, then clear both colour words per entry. */
void sdfResetFlagListEntries(SdfFlagListWork *work) {
    u32 entryCount;
    SdfFlagListMark *markCursor;
    u32 entryIndex;

    entryIndex = 0;
    entryCount = work->params.count;
    markCursor = work->marks;
    if (entryCount != 0) {
        do {
            entryIndex = entryIndex + 1;
            markCursor->timer = SDF_FLAG_LIST_RESET_MARK;
            markCursor++;
        } while (entryIndex < entryCount);
    }
    memset(work->colors, 0, entryCount << SDF_FLAG_LIST_VALUE_SHIFT);
}

SdfFlagListWork *sdfCreateFlagListWork(const SdfFlagListParams *source) {
    u32 count;
    u32 arrayBytes;
    sdfMemHandle_t *allocation;
    u8 *buffer;
    SdfFlagListWork *work;

    count = source->count;
    arrayBytes = count * (sizeof(f32[2][4]) + sizeof(u32[2]) + sizeof(SdfFlagListMark));
    allocation = sdfMemAllocDirect(arrayBytes + sizeof(SdfFlagListWork));
    buffer = (u8 *)sdfMemLock(allocation);
    work = (SdfFlagListWork *)(buffer + arrayBytes);
    work->vertices = (f32 (*)[4])buffer;
    buffer += count * sizeof(f32[2][4]);
    work->colors = (u32 *)buffer;
    buffer += count * sizeof(u32[2]);
    work->marks = (SdfFlagListMark *)buffer;
    work->unk04 = 0x80808080;
    work->allocation = allocation;
    work->frame = 0;
    memcpy(&work->params, source, sizeof(work->params));
    sdfResetFlagListEntries(work);
    return work;
}

SdfFlagListWork *sdfInitializeFlagListFromResource(void *file) {
    return effCreateSelectionFlagListFromWork(fileResolvePrimaryBuffer(file));
}

void sdfReleaseFlagListResource(SdfFlagListWork *work) {
    sdfMemFreeDirect(work->allocation);
}

INCLUDE_ASM(const s32, "code_00242308", sdfUpdateFlagListEntries);

/* Copy vertex pairs, optionally add the view target, and submit the packet list.
 * count is reused first as an entry count and then as a vertex count. */
void sdfDrawFlagListVertices(SdfFlagListWork *work) {
    f32 (*sourceVertices)[4];
    f32 (*copiedVertices)[4];
    u32 count;
    u32 vertexIndex;
    struct sdfMemHandle_t *vertexAllocation;
    sdfDrawTag_t *packetList;
    sdfGrOt_t *surface;

    if (work->params.maxFrames != 0 && work->frame >= work->params.maxFrames) {
        return;
    }
    count = work->params.count;
    sourceVertices = work->vertices;
    vertexAllocation = sdfMemAllocDirect(count * SDF_FLAG_LIST_ENTRY_VERTEX_BYTES);
    count *= SDF_FLAG_LIST_VERTICES_PER_ENTRY;
    copiedVertices = (f32 (*)[4])sdfMemGetPtr(vertexAllocation);
    for (vertexIndex = 0; vertexIndex < count; vertexIndex++) {
        SDF_VU0_LOADVECTOR(&sourceVertices[vertexIndex]);
        if (D_003BD2C8 != 0) {
            SDF_VU0_LOADVECTOR_B(aViewEye);
            SDF_VU0_ADDVECTOR();
        }
        SDF_VU0_STOREVECTOR_NOMEM(&copiedVertices[vertexIndex]);
    }
    packetList = (sdfDrawTag_t *)sdfGrAllocPacketBuff(SDF_FLAG_LIST_PACKET_BYTES);
    sdfDrawTagInit(packetList);
    sdfDrawTagLink(packetList, (u32)effDebugDrawWireModel(copiedVertices, work->colors, work->params.count * SDF_FLAG_LIST_VERTICES_PER_ENTRY, 0x40));
    surface = D_r1_004039D0[work->params.alpha.surfaceIndex];
    surface->regFunc(surface, packetList);
    sdfMemFreeDirect(vertexAllocation);
}

INCLUDE_ASM(const s32, "code_00242308", func_r1_00243180);

INCLUDE_ASM(const s32, "code_00242308", func_r1_00243188);

/* Sample color/alpha at frame zero with duration zero; discard the return value. */
void sdfSampleFlagListStartColor(SdfFlagListWork *work) {
    effSampleColorAlphaTracks(&work->params.color, &work->params.alpha, 0, 0);
}

INCLUDE_ASM(const s32, "code_00242308", func_r1_002431B8);

INCLUDE_ASM(const s32, "code_00242308", func_r1_002431D8);

INCLUDE_ASM(const s32, "code_00242308", func_r1_002431E0);

INCLUDE_ASM(const s32, "code_00242308", func_r1_002431E8);

extern s32 D_r1_0046D18C;

void func_r1_00243210(void) {
    D_r1_0046D18C = 1;
}

extern s32 D_r1_0046D18C;

void func_r1_00243220(void) {
    D_r1_0046D18C = 0;
}

INCLUDE_ASM(const s32, "code_00242308", func_r1_00243228);

s32 effUpdatePartsListEditor(s32 mode);

s32 btlUpdateEffectWork(void) {
    if (D_003BD074 != NULL) {
        if ((effUpdatePartsListEditor(0) & 1) == 0) {
            effResetFileResourceStores();
            return 0;
        }
    }
    return 1;
}

void func_r1_002432B0(void) {
}

void func_r1_0021BB88(void);

void func_r1_002432B8(void) {
    effResetFileResources();
    effResetFileResourceManager();
    func_r1_0021BB88();
}

void effSetBattleOffset(void *src) {
    SDF_VU0_COPYVECTOR(effBattleOffset, src);
}

void effComputeBattleCameraPositionVU(u8 *effect) {
    u128 direction;
    u32 flags = ((FileJob *)effect)->xformFlags;

    if ((flags & 0x18) != 0) {
        camFollowOffsetVec(effect, &direction);
        VU0_LOAD_VF($vf10, &direction);
        flags = ((FileJob *)effect)->xformFlags;
    } else {
        VU0_LOAD_VF($vf10, ((FileJob *)effect)->offset);
    }
    if ((flags & 0x80) != 0) {
        VU0_SCALAR_OP_CLOBBER(effFileQueue->scaleMultiplier, "vmulx.xyzw vf10, vf10, vf2x");
    }
    VU0_LOAD_VF($vf11, effBattleOffset);
    VU0_ADD($vf10, $vf10, $vf11);
    VU0_LOAD_VF($vf11, effFileQueue);
    VU0_ADD($vf10, $vf10, $vf11);
    if ((flags & 4) != 0) {
        VU0_SCALAR_OP_CLOBBER(-5.0f, "vaddx.y vf10, vf0, vf2x");
    }
}

void effMultiplyQuatWithFlag(u8 *effect) {
    if ((((FileJob *)effect)->xformFlags & 0x60) != 0) {
        u128 quaternion;
        camAimRotation(effect, &quaternion);
        VU0_LOAD_VF($vf10, effFileQueue->quat);
        VU0_LOAD_VF($vf11, &quaternion);
        Mult();
    } else {
        VU0_LOAD_VF($vf10, effFileQueue->quat);
        VU0_LOAD_VF($vf11, ((FileJob *)effect)->quat);
        Mult();
    }
}

INCLUDE_ASM(const s32, "code_00242308", effApplyBattleCameraToObject);

u32 effCreateBattleCameraJob(u8 *request) {
    u8 *outputBuffer = ((EffFileJobRequest *)request)->outputBuffer;
    u32 job;

    if (outputBuffer != NULL) {
        if (((EffFileJobRequest *)request)->fileKind == 6) {
            memcpy(outputBuffer, D_r1_004D3418, 0x3C);
            outputBuffer = ((EffFileJobRequest *)request)->outputBuffer;
        }
        fileJobSetPrimaryData(effQueuedFileHandle, outputBuffer, ((EffFileJobRequest *)request)->size, ((EffFileJobRequest *)request)->transferMode);
    }
    job = (u32)fileJobCreateFromJob(effQueuedFileHandle);
    effApplyBattleCameraToObject(job);
    return job;
}

FileJobPayload *effLoadFileJobPayload(EffFileJobRequest *request, u32 existingJob) {
    FileJobPayload *job;
    if (existingJob != 0) {
        void *source;
        job = fileJobCreateFromCommandState((const char *)existingJob);
        source = fileResolvePrimaryBuffer(job);
        memcpy(request->outputBuffer, source, request->size);
    } else {
        job = fileCreateJob(request->fileKind);
        if (request->outputBuffer != NULL) {
            fileJobSetPrimaryData(job, request->outputBuffer, request->size, request->transferMode);
        }
        if (request->secondaryCommandPath != 0) {
            fileJobCopyCommandIntoSecondaryData(job, request->secondaryCommandPath, request->resourceMode);
        } else {
            u32 value = 0;
            fileJobSetSecondaryData(job, &value, 4, 4);
        }
    }
    effResetFileResourceManager();
    return job;
}

void effInvokeFileJobWithBattleCamera(u32 job) {
    if (effAuxiliaryFileQueue == 0) {
        effApplyBattleCameraToObject((void *)job);
        fileJobInvokeTypeCallback((FileJobPayload *)job);
        return;
    }
}

void effFileJobQueueRelease(u32 job) {
    if (effAuxiliaryFileQueue != 0) {
        fileQueueDestroy(effAuxiliaryFileQueue);
        effAuxiliaryFileQueue = 0;
    }
    if (effTemporaryFileJob != 0) {
        fileJobDestroy(effTemporaryFileJob);
        effTemporaryFileJob = 0;
    }
    fileJobDestroy((FileJobPayload *)job);
}

u32 effPollPrimaryFile(void) {
    EffQueueRecord record;
    char path[0x70];
    u32 state;
    u32 result = 0x400001;

    effUpdateResourceQueue(D_r1_00456E58, D_003BD078, &record);
    state = record.completion.state;
    if (state == BTL_RESOURCE_SELECTION_CANCELED) {
        result = 0x400000;
    } else if (state == BTL_RESOURCE_SELECTION_ACCEPTED) {
        if (effQueuedFileHandle != 0) {
            sprintf(path, D_003BD080, D_r1_00456E58, record.nameWithExtension);
            fileWriteToPfs(effQueuedFileHandle, path);
            result = 0x400002;
        }
    }
    return result;
}

void effQueueNamedResourceRequest(void) {
    effReleaseQueuedResourceName();
    effQueueResource(D_003BD088, D_r1_004D54F0);
}

u32 effPollNamedFile(void) {
    EffQueueRecord record;
    char path[0x70];
    u32 state;
    u32 result = 0x400001;

    effUpdateResourceQueue(D_003B39E0, D_003BD088, &record);
    state = record.completion.state;
    if (state == BTL_RESOURCE_SELECTION_CANCELED) {
        result = 0x400000;
    } else if (state == BTL_RESOURCE_SELECTION_ACCEPTED) {
        if (effFileQueue != 0) {
            strcpy(D_r1_004D54F0, record.name);
            sprintf(path, D_003BD080, D_003B39E0, record.nameWithExtension);
            fileQueueSaveImage(effFileQueue, path);
            result = 0x400002;
        }
    }
    return result;
}

void effQueueAttachedResourceRequest(void) {
    effReleaseQueuedResourceName();
    effQueueResource(D_003BD090, D_r1_004D54F0);
}

u32 effPollAttachedFile(void) {
    EffQueueRecord record;
    char path[0x70];
    u32 state;
    u32 result = 0x400001;

    effUpdateResourceQueue(D_003B39E0, D_003BD090, &record);
    state = record.completion.state;
    if (state == BTL_RESOURCE_SELECTION_CANCELED) {
        result = 0x400000;
    } else if (state == BTL_RESOURCE_SELECTION_ACCEPTED) {
        if (effFileQueue != 0) {
            strcpy(D_r1_004D54F0, record.name);
            sprintf(path, D_003BD080, D_003B39E0, record.nameWithExtension);
            fileQueueSaveVersionedImage(effFileQueue, path);
            result = 0x400002;
        }
    }
    return result;
}

extern EffectAssetLink *D_r1_004163E8[24];

u8 * effFindAssetData(FileJob *entry) {
    FileJobPayload *requested = (FileJobPayload *)entry->id;
    u16 type = requested->type;
    u16 option = requested->option;
    u16 i;
    for (i = 0; i < 24; i++) {
        EffectAssetLink *links = D_r1_004163E8[i];
        if (links != NULL) {
            EffectAssetLink *current = links;
            if (current->asset != NULL) {
                do {
                    u8 *asset = current->asset;
                    if (((EffAssetIdentifier *)asset)->type == type && ((EffAssetIdentifier *)asset)->id == option) {
                        return asset;
                    }
                    current++;
                } while (current->asset != NULL);
            }
        }
    }
    return NULL;
}

extern EffectAssetLink *D_r1_004163E8[24];

EffQueuedFileObject * effFindAssetObject(FileJob *entry) {
    FileJobPayload *requested = (FileJobPayload *)entry->id;
    u16 type = requested->type;
    u16 option = requested->option;
    u16 i;
    for (i = 0; i < 24; i++) {
        EffectAssetLink *links = D_r1_004163E8[i];
        if (links != NULL) {
            EffectAssetLink *current = links;
            if (current->asset != NULL) {
                do {
                    u8 *asset = current->asset;
                    if (((EffAssetIdentifier *)asset)->type == type && ((EffAssetIdentifier *)asset)->id == option) {
                        return current->object;
                    }
                    current++;
                } while (current->asset != NULL);
            }
        }
    }
    return NULL;
}

/* Return the selected queue row plus the offset of its visible window. */
u32 effGetSelectedFileQueueIndex(void) {
    return D_r1_00416D44[0] + effPartsListScroll;
}

INCLUDE_RODATA(const s32, "code_00242308", D_r1_00456E70);

INCLUDE_RODATA(const s32, "code_00242308", D_r1_00456E80);

INCLUDE_RODATA(const s32, "code_00242308", D_r1_00456E90);

INCLUDE_ASM(const s32, "code_00242308", effUpdatePartsListEditor);

u32 effAppendPositionedCameraFileJob(const EffCameraCreateRequest *request) {
    FileJob *entry;
    u32 count;

    fileQueueInitTransform(effSelectedJobHeader);
    entry = fileAppendJob(effFileQueue, (u32)effLoadFileJobPayload(request->resource, request->existingJob));
    effCurrentFileQueueEntry = entry;
    strcpy(entry->name, request->resource->name);
    entry->offset[0] = request->position[0];
    entry->offset[1] = request->position[1];
    entry->offset[2] = request->position[2];
    entry->unk88[0] = 8;
    entry->unk88[1] = 0;
    entry->unk88[2] = 0;
    memcpy(effSelectedJobHeader, entry, 0x80);
    effQueuedFileHandle = (FileJobPayload *)entry->id;
    D_003BD064 = effCreateBattleCameraJob((u8 *)request->resource);
    effQueuedFileObject = request->object;
    count = fileQueueCountLinkedJobs(effFileQueue);
    if (count > 21) {
        effPartsListMenu.cursor = 20;
        effPartsListScroll = count - 21;
    } else {
        effPartsListScroll = 0;
        effPartsListMenu.cursor = count - 1;
    }
    request->object->linkedState = (u8 *)&effPartsListMenu;
    effResetFileResourceManager();
    return 0x800000;
}

/* Load the selected job's payload and camera snapshots; return the refresh status. */
u32 effLoadSelectedFileJob(void) {
    FileJob *entry = fileQueueGetAt(effFileQueue, effGetSelectedFileQueueIndex());
    EffFileJobRequest *request;
    u8 *fileData;
    u32 job;

    effCurrentFileQueueEntry = entry;

    /* These independent snapshots cover the queue header and camera transform. */
    memcpy(D_003DF840, entry, 0x90);
    memcpy(effSelectedJobHeader, entry, 0x80);

    effQueuedFileHandle = (FileJobPayload *)entry->id;
    request = (EffFileJobRequest *)effFindAssetData(entry);
    if (request->fileKind != 6) {
        fileData = (u8 *)fileResolvePrimaryBuffer(effQueuedFileHandle);
        memcpy(request->outputBuffer, fileData, request->size);
    } else {
        FileJobPayload *payload;
        u32 transferMode;
        u32 payloadSize;

        if (request->allocation != NULL) {
            sdfMemFreeDirect(request->allocation);
        }
        request->allocation = sdfMemAllocDirect(
            effQueuedFileHandle->primary.size);
        {
            void *output = sdfMemLock(request->allocation);
            payload = effQueuedFileHandle;
            /* Publish the retained pointer through its object representation. */
            memcpy(&request->outputBuffer, &output, sizeof(request->outputBuffer));
        }
        payloadSize = payload->primary.size;
        transferMode = payload->option;
        request->transferMode = transferMode;
        request->size = payloadSize;
        fileData = (u8 *)fileResolvePrimaryBuffer(payload);
        memcpy(request->outputBuffer, fileData,
               effQueuedFileHandle->primary.size);
        memcpy(D_r1_004D3418, fileData, 0x3C);
    }

    job = effCreateBattleCameraJob((u8 *)request);
    D_003BD064 = job;
    effQueuedFileObject = effFindAssetObject(entry);
    ((EffQueuedFileObject *)effQueuedFileObject)->linkedState = (u8 *)&effPartsListMenu;
    effResetFileResourceManager();
    if (effTemporaryFileJob != 0) {
        fileJobDestroy(effTemporaryFileJob);
        effTemporaryFileJob = 0;
    }
    return 0x800000;
}

u32 effReinitializeFileQueue(void) {
    if (effTemporaryFileJob != 0) {
        fileQueueDestroy(effAuxiliaryFileQueue);
        effAuxiliaryFileQueue = 0;
    }
    if (effTemporaryFileJob != 0) {
        fileJobDestroy(effTemporaryFileJob);
        effTemporaryFileJob = 0;
    }
    if (effFileQueue != 0) {
        fileQueueDestroy(effFileQueue);
    }
    effFileQueue = fileQueueCreate();
    D_r1_00416484[0] = 0;
    D_r1_004D54F0[0] = 0;
    effPartsListScroll = 0;
    effPartsListMenu.cursor = 0;
    effQueuedFileObject = &effPartsListMenu;
    return 0;
}

u32 effResetFileQueueState(void) {
    fileQueueRemoveAndDestroyJob(effFileQueue, fileQueueGetAt(effFileQueue, effGetSelectedFileQueueIndex()));
    D_r1_00416554[0] = 0;
    effPartsListScroll = 0;
    effPartsListMenu.cursor = 0;
    effQueuedFileObject = &effPartsListMenu;
    return 0;
}

u32 effFinalizeQueuedFile(void) {
    s32 index = effGetSelectedFileQueueIndex();
    FileJob *record = fileQueueGetAt(effFileQueue, index);
    FileJob *copy = fileJobDuplicateAfter(effFileQueue, record);
    D_r1_004164EC[0] = 0;
    fileJobCopyHeader(copy, record);
    effQueuedFileObject = &effPartsListMenu;
    return 0;
}

/* Clone the selected job, preserve its sector leader, and refresh the selected asset. */
u32 effCloneSelectedFileJob(void) {
    FileJob *source;
    FileJob *entry;
    FileJob *leader;
    EffFileJobRequest *resource;
    void *fileData;
    s32 linkedCount;

    source = fileQueueGetAt(effFileQueue, effGetSelectedFileQueueIndex());
    entry = fileDuplicateAndAppendJob(effFileQueue,
                                      (FileJobPayload *)source->id);
    effCurrentFileQueueEntry = entry;
    fileJobCopyHeader(entry, source);

    if ((source->flags & 2) != 0) {
        leader = fileQueueFindById(effFileQueue, source->sector);
    } else {
        leader = source;
    }
    fileQueueLinkJobToSectorLeader(effFileQueue, entry, leader);

    memcpy(D_003DF840, entry, 0x90);
    memcpy(effSelectedJobHeader, entry, 0x80);

    effQueuedFileHandle = (FileJobPayload *)entry->id;
    resource = (EffFileJobRequest *)effFindAssetData(entry);
    strcpy(entry->name, resource->name);

    if (resource->fileKind != 6) {
        fileData = fileResolvePrimaryBuffer(effQueuedFileHandle);
        memcpy(resource->outputBuffer, fileData, resource->size);
    } else {
        if (resource->allocation != 0) {
            sdfMemFreeDirect(resource->allocation);
        }
        resource->allocation = sdfMemAllocDirect(
            effQueuedFileHandle->primary.size);
        {
            void *buffer = sdfMemLock(resource->allocation);
            FileJobPayload *payload = effQueuedFileHandle;
            s32 option;
            u32 primarySize;
            /* Publish the retained pointer through its object representation. */
            memcpy(&resource->outputBuffer, &buffer, sizeof(resource->outputBuffer));
            primarySize = payload->primary.size;
            option = payload->option;
            resource->transferMode = option;
            resource->size = primarySize;
            fileData = fileResolvePrimaryBuffer(payload);
        }
        memcpy(resource->outputBuffer, fileData,
               effQueuedFileHandle->primary.size);
        memcpy(D_r1_004D3418, fileData, 0x3C);
    }

    D_003BD064 = effCreateBattleCameraJob((u8 *)resource);
    effQueuedFileObject = effFindAssetObject(entry);
    ((EffQueuedFileObject *)effQueuedFileObject)->linkedState = (u8 *)&effPartsListMenu;

    linkedCount = fileQueueCountLinkedJobs(effFileQueue);
    if ((u32)linkedCount >= 22) {
        effPartsListMenu.cursor = 20;
        effPartsListScroll = linkedCount - 21;
    } else {
        effPartsListScroll = 0;
        effPartsListMenu.cursor = linkedCount - 1;
    }

    effResetFileResourceManager();
    if (effTemporaryFileJob != 0) {
        fileJobDestroy(effTemporaryFileJob);
        effTemporaryFileJob = 0;
    }
    return 0x800000;
}

u32 effFlagFileQueueActive(void) {
    effPartsListExitRequested = 1;
    effSelectLinkedFileState(0);
    return 0;
}

/* Select the queued file object's linked state when present; return zero in either case. */
u32 effSelectLinkedFileState(void) {
    if (((EffQueuedFileObject *)effEditorCurrentMenu)->linkedState != NULL) {
        effQueuedFileObject = ((EffQueuedFileObject *)effEditorCurrentMenu)->linkedState;
    }
    return 0;
}

u32 effLoadBattleCameraSnapshot(u32 value) {
    FileJob *record = fileQueueGetAt(effFileQueue, effGetSelectedFileQueueIndex());

    *(EffectBlock90 *)D_003DF840 = *(EffectBlock90 *)record;
    *(EffectAlignedBlock128 *)D_003DF840 = *(EffectAlignedBlock128 *)effSelectedJobHeader;
    if (((EffectBlock90 *)D_003DF840)->word[26] & 2) {
        effBattleCameraSnapshotSelection.rows = D_r1_00416650;
        effBattleCameraSnapshotSelection.count = 3;
        if (effBattleCameraSnapshotSelection.cursor > effBattleCameraSnapshotSelection.count) {
            effBattleCameraSnapshotSelection.cursor = 0;
        }
    } else {
        effBattleCameraSnapshotSelection.rows = D_r1_00416698;
        effBattleCameraSnapshotSelection.count = 9;
        if (effBattleCameraSnapshotSelection.cursor == 0) {
            effBattleCameraSnapshotSelection.cursor = 1;
        }
    }
    return value;
}

u32 effStoreBattleCameraSnapshot(u32 value) {
    FileJob *record = fileQueueGetAt(effFileQueue, effGetSelectedFileQueueIndex());

    *(EffectBlock90 *)record = *(EffectBlock90 *)D_003DF840;
    *(EffectAlignedBlock128 *)effSelectedJobHeader = *(EffectAlignedBlock128 *)D_003DF840;
    effPartsPreviewReload = 1;
    if (((EffectBlock90 *)D_003DF840)->word[26] & 2) {
        effBattleCameraSnapshotSelection.rows = D_r1_00416650;
        effBattleCameraSnapshotSelection.count = 3;
        if (effBattleCameraSnapshotSelection.cursor > effBattleCameraSnapshotSelection.count) {
            effBattleCameraSnapshotSelection.cursor = 0;
        }
    } else {
        effBattleCameraSnapshotSelection.rows = D_r1_00416698;
        effBattleCameraSnapshotSelection.count = 9;
        if (effBattleCameraSnapshotSelection.cursor == 0) {
            effBattleCameraSnapshotSelection.cursor = 1;
        }
    }
    return value;
}

INCLUDE_ASM(const s32, "code_00242308", effSelectCameraFileJobFromRows);

extern char D_r1_00456DF8[];
extern char D_r1_00415620[];
s32 func_r1_00245928(void) {
    return effSelectCameraFileJobFromRows(D_r1_00456DF8, D_r1_00415620, 0xF);
}

s32 effCreatePolyTrackControlTask(void) {
    return effSelectCameraFileJobFromRows(D_r1_00456DD8, D_r1_004157D0, 6);
}

s32 effCreatePolyTrackTask(void) {
    return effSelectCameraFileJobFromRows("POLY TRACK S", D_r1_00415880, 6);
}

s32 effCreateBlurTask(void) {
    return effSelectCameraFileJobFromRows(D_003BCA40, D_r1_00415970, 6);
}

s32 effCreateFilterTask(void) {
    return effSelectCameraFileJobFromRows(D_003BD050, D_r1_00415A20, 3);
}

s32 effCreateEnvironmentTask(void) {
    return effSelectCameraFileJobFromRows(D_r1_00456D48, D_r1_00415B20, 5);
}

extern char D_r1_00456DB8[];
extern char D_r1_00415AC0[];
s32 func_r1_00245A18(void) {
    return effSelectCameraFileJobFromRows(D_r1_00456DB8, D_r1_00415AC0, 0x3);
}

s32 effCreatePolyFlashTask(void) {
    return effSelectCameraFileJobFromRows(D_r1_00456DA8, D_r1_00415BB0, 0xB);
}

s32 effCreatePolyRingTask(void) {
    return effSelectCameraFileJobFromRows(D_r1_00456D98, D_r1_00415CF0, 6);
}

s32 effCreatePolyThunderTask(void) {
    return effSelectCameraFileJobFromRows(D_r1_00456D88, D_r1_00415DA0, 4);
}

s32 effCreatePolyTwinkleTask(void) {
    return effSelectCameraFileJobFromRows("POLY TWINKLE", D_r1_00415E10, 6);
}

extern char D_r1_00456D78[];
extern char D_r1_00415EC0[];
s32 func_r1_00245AE0(void) {
    return effSelectCameraFileJobFromRows(D_r1_00456D78, D_r1_00415EC0, 0x8);
}

s32 effCreatePolyScalyTask(void) {
    return effSelectCameraFileJobFromRows(D_r1_00456D68, D_r1_00415FA0, 5);
}

s32 effCreatePolyCrackTask(void) {
    return effSelectCameraFileJobFromRows(D_r1_00456D58, D_r1_00416070, 2);
}

extern char D_r1_00456D08[];
extern char D_r1_004160B0[];
s32 func_r1_00245B58(void) {
    return effSelectCameraFileJobFromRows(D_r1_00456D08, D_r1_004160B0, 0xD);
}

s32 effCreateTwoDimensionalTask(void) {
    return effSelectCameraFileJobFromRows(D_003BD000, D_r1_004162E0, 2);
}

s32 effCreatePolyWindTask(void) {
    return effSelectCameraFileJobFromRows(D_r1_00456DE8, D_r1_00416320, 7);
}

u32 effPollPartResource(void) {
    EffResourceBankSlot record;
    u32 state;
    u32 result = 0x400001;

    effPollResourceBankSlot(D_r1_00456E58, BTL_RESOURCE_SCAN_EP, &record);
    state = record.selectionStatus;
    if (state == BTL_RESOURCE_SELECTION_CANCELED) {
        result = 0x400000;
    } else if (state == BTL_RESOURCE_SELECTION_ACCEPTED) {
        if (effFileQueue != 0) {
            FileJob *job = fileAppendJobFromCommandPath(effFileQueue, (const char *)&record);
            u8 *asset = effFindAssetData(job);
            strcpy(job->name, *(char **)asset);
        }
        result = 0x400002;
    }
    return result;
}

u32 effPollNamedFileJob(void) {
    EffResourceBankSlot record;
    u32 state;
    u32 result = 0x400001;

    effPollResourceBankSlot(D_003B39E0, BTL_RESOURCE_SCAN_EPL, &record);
    state = record.selectionStatus;
    if (state == BTL_RESOURCE_SELECTION_CANCELED) {
        result = 0x400000;
    } else if (state == BTL_RESOURCE_SELECTION_ACCEPTED) {
        if (effFileQueue != 0) {
            fileQueueDestroy(effFileQueue);
        }
        strcpy(D_r1_004D54F0, record.name);
        effFileQueue = fileQueueCreateFromCommandState((const char *)&record);
        effEditedQueueHeader = *(EffectBlock128 *)effFileQueue;
        effPartsListScroll = 0;
        D_r1_00416D44[0] = 0;
        result = 0x400002;
    }
    return result;
}

INCLUDE_RODATA(const s32, "code_00242308", D_r1_00456F30);

INCLUDE_RODATA(const s32, "code_00242308", D_r1_00456F48);

INCLUDE_ASM(const s32, "code_00242308", effHandleDebugOrbitInput);

extern u32 D_r1_004D5640[];
extern u8 D_r1_004D5460[];

s32 effResetStaticState(void) {
    D_r1_004D5640[0] = 0;
    D_r1_004D5640[1] = 0;
    D_r1_004D5640[2] = 0;
    D_r1_004D5640[3] = 0;
    D_r1_004D5460[0x88] = 8;
    D_r1_004D5460[0x89] = 0;
    D_r1_004D5460[0x8A] = 0;
    return 0;
}

/* Synchronize the diagnostic camera position and calculate its positional guides.
 * Return input flags; the native diagnostic drawing providers are retail stubs. */
s32 effUpdateCameraDiagnostics(EffectStateSnapshot *snapshot, s8 mode) {
    u8 *camera = D_r1_00416F38;
    f32 vectors[6][4] __attribute__((aligned(16)));
    f32 length;
    f32 deltaY;
    s32 result = 1;

    if ((*(u32 *)(camera + 0x10) & 2) == 0) {
        if (D_r1_0043EED9[0] < 0) {
            result = 0;
        }
    }

    SDF_VU0_COPYVECTOR(D_003DFA20, snapshot->position);
    btlDebugMenuSetupNoOp(camera);
    btlDebugMenuBeforeRecordNoOp();
    btlDebugMenuRecordNoOp(camera);
    btlDebugMenuAfterRecordNoOp();
    SDF_VU0_COPYVECTOR(snapshot->position, D_003DFA20);

    if ((*(u32 *)(camera + 0x10) & 8) != 0) {
        result |= 2;
    }
    if ((result & 2) != 0 && (snapshot->flags & 0x18) != 0) {
        camFollowOffsetVec((u8 *)snapshot, vectors[0]);
        SDF_VU0_COPYVECTOR(snapshot->position, vectors[0]);
    }

    if (mode != 0) {
        effComputeBattleCameraPositionVU((u8 *)snapshot);
        SDF_VU0_LOADVECTOR_B(&effEditedQueueHeader);
        SDF_VU0_SUBVECTOR();
    } else {
        SDF_VU0_LOADVECTOR(snapshot->position);
    }
    SDF_VU0_STOREVECTOR(vectors[1]);
    VU0_SET_VF10_COMPONENT(y, 0.0f);
    SDF_VU0_GETLENGTH(length);
    SDF_VU0_LOADVECTOR(effBattleOffset);
    if (mode != 0) {
        SDF_VU0_LOADVECTOR_B(&effEditedQueueHeader);
        SDF_VU0_ADDVECTOR();
    }
    SDF_VU0_STOREVECTOR(vectors[5]);
    SDF_VU0_STOREVECTOR(vectors[3]);
    deltaY = vectors[1][1];
    vectors[3][1] += deltaY;
    if (vectors[3][1] == 0.0f) {
        vectors[3][1] = vectors[5][1] + (-2.0f);
    }
    if (length > 0.0f) {
        btlDebugDrawRadiusGuide(vectors[3], length, 0x80006060, 0);
        deltaY = vectors[1][1];
    }

    PCP_COPY_VECTOR_F32(vectors[2], vectors[3]);
    vectors[2][1] = vectors[5][1] + (-2.0f);
    if (deltaY != vectors[5][1]) {
        btlDebugDrawRadiusGuide(vectors[2], length, 0x80404040, 0);
        btlDebugDrawSegment(vectors[2], vectors[3], 0x80006060);
    }

    SDF_VU0_LOADVECTOR(vectors[1]);
    SDF_VU0_LOADVECTOR_B(vectors[5]);
    SDF_VU0_ADDVECTOR();
    SDF_VU0_STOREVECTOR_NOMEM(vectors[4]);
    btlDebugDrawSegment(vectors[3], vectors[4], 0x80006060);
    btlDebugDrawSegment(vectors[2], vectors[4], 0x80006060);
    btlDebugDrawPoint(vectors[4]);
    return result;
}

INCLUDE_ASM(const s32, "code_00242308", func_r1_00246318);

INCLUDE_ASM(const s32, "code_00242308", func_r1_00246340);

INCLUDE_ASM(const s32, "code_00242308", effResetCameraSnapshotSelection);

u32 effUpdateCameraSnapshotGuides(EffectCameraSnapshotView *state, s8 useBattlePosition) {
    EffectCameraDebugScratch scratch;
    u32 result = 1;

    if ((D_r1_00416FC8.flags & 2) == 0) {
        if (D_r1_0043EED9[0] < 0) {
            result = 0;
        }
    }

    SDF_VU0_COPYVECTOR(D_r1_004D5650, state->camera.quat);
    if (useBattlePosition != 0) {
        effComputeBattleCameraPositionVU((u8 *)state);
    } else {
        SDF_VU0_LOADVECTOR(state->camera.offset);
    }
    SDF_VU0_STOREVECTOR_NOMEM(&scratch.basePosition);
    SDF_VU0_STOREVECTOR(D_003DFA20);

    btlDebugMenuSetupNoOp(&D_r1_00416FC8);
    btlDebugMenuBeforeRecordNoOp();
    btlDebugMenuRecordNoOp(&D_r1_00416FC8);
    btlDebugMenuAfterRecordNoOp();
    SDF_VU0_COPYVECTOR(state->camera.quat, D_r1_004D5650);
    if (D_r1_00416FC8.flags & 8) {
        result |= 2;
    }
    SDF_VU0_LOADVECTOR(&scratch.basePosition);
    SDF_VU0_LOADVECTOR_B(effBattleOffset);
    SDF_VU0_ADDVECTOR();
    SDF_VU0_STOREVECTOR_NOMEM(&scratch.segmentStart);

    SDF_VU0_LOADVECTOR(state->camera.quat);
    ToMatrix();
    VU0_STORE_MATRIX_UNCLOBBERED(scratch.rotationMatrix);

    SDF_VU0_LOADVECTOR(effWorldUpVector);
    SDF_VU0_APPLYMATRIX();
    VU0_SCALE_VF(vf10, 250.0f);
    SDF_VU0_LOADVECTOR_B(&scratch.segmentStart);
    SDF_VU0_ADDVECTOR();
    SDF_VU0_STOREVECTOR_NOMEM(&scratch.segmentEnd);

    btlDebugDrawOrientedExtentGuide((f32 *)&scratch.segmentStart, 30.0f, -50.0f, 0x80006060, (f32 (*)[4])scratch.rotationMatrix);
    btlDebugDrawSegment((f32 *)&scratch.segmentStart, (f32 *)&scratch.segmentEnd, 0x80000080);
    btlDebugDrawPoint((f32 *)&scratch.segmentStart);

    SDF_VU0_LOADVECTOR_B(effBattleOffset);
    VU0_LOAD_MATRIX(scratch.rotationMatrix);
    SDF_VU0_LOAD_ROW3(&scratch.basePosition);
    SDF_VU0_LOADIDENTITYVECTOR();
    VU0_SET_VF10_COMPONENT(x, 100.0f);
    SDF_VU0_APPLYMATRIX_XYZ1();
    SDF_VU0_ADDVECTOR();
    SDF_VU0_STOREVECTOR_NOMEM(&scratch.segmentStart);
    SDF_VU0_LOADIDENTITYVECTOR();
    VU0_SET_VF10_COMPONENT(x, -100.0f);
    SDF_VU0_APPLYMATRIX_XYZ1();
    SDF_VU0_ADDVECTOR();
    SDF_VU0_STOREVECTOR_NOMEM(&scratch.segmentEnd);
    btlDebugDrawSegment((f32 *)&scratch.segmentStart, (f32 *)&scratch.segmentEnd, 0x80006060);

    SDF_VU0_LOADVECTOR_B(effBattleOffset);
    VU0_LOAD_MATRIX(scratch.rotationMatrix);
    SDF_VU0_LOAD_ROW3(&scratch.basePosition);
    SDF_VU0_LOADIDENTITYVECTOR();
    VU0_SET_VF10_COMPONENT(z, 25.0f);
    SDF_VU0_APPLYMATRIX_XYZ1();
    SDF_VU0_ADDVECTOR();
    SDF_VU0_STOREVECTOR_NOMEM(&scratch.segmentStart);
    SDF_VU0_LOADIDENTITYVECTOR();
    VU0_SET_VF10_COMPONENT(z, -25.0f);
    SDF_VU0_APPLYMATRIX_XYZ1();
    SDF_VU0_ADDVECTOR();
    SDF_VU0_STOREVECTOR_NOMEM(&scratch.segmentEnd);
    btlDebugDrawSegment((f32 *)&scratch.segmentStart, (f32 *)&scratch.segmentEnd, 0x80006060);

    return result;
}

u32 effLoadFileSlotAndPoll(void) {
    FileJob *file = fileQueueGetAt(effFileQueue, effGetSelectedFileQueueIndex());
    u32 result;

    memcpy(D_003DF840, file, 0x90);
    result = effUpdateCameraDiagnostics((EffectStateSnapshot *)D_003DF840, 1);
    memcpy(file, D_003DF840, 0x90);
    if (result & 1) {
        result |= 0x800000;
    }
    return result;
}

s32 effRunWithStateBackup(void) {
    EffectStateSnapshot snapshot = *(EffectStateSnapshot *)&effEditedQueueHeader;
    f32 *backup = snapshot.position;
    s32 result;

    SDF_VU0_COPYVECTOR(backup, &effEditedQueueHeader);
    result = effUpdateCameraDiagnostics(&snapshot, 0);
    SDF_VU0_COPYVECTOR(&effEditedQueueHeader, backup);
    if (result & 1) {
        result |= 0x800000;
    }
    return result;
}

u32 effProcessQueuedFileRecordAndPersistChanges(void) {
    FileJob *file = fileQueueGetAt(effFileQueue, effGetSelectedFileQueueIndex());
    u32 result;

    memcpy(D_003DF840, file, 0x90);
    result = effUpdateCameraSnapshotGuides((EffectCameraSnapshotView *)D_003DF840, 1);
    memcpy(file, D_003DF840, 0x90);
    if (result & 1) {
        result |= 0x800000;
    }
    return result;
}

void effResetFileResources(void) {
    D_r1_00416D44[0] = 0;
    effPartsPreviewReload = 0;
    effPartsListScroll = 0;
    if (effFileQueue != 0) {
        fileQueueDestroy(effFileQueue);
        effFileQueue = 0;
    }
    if (effAuxiliaryFileQueue != 0) {
        fileQueueDestroy(effAuxiliaryFileQueue);
        effAuxiliaryFileQueue = 0;
    }
    if (effTemporaryFileJob != 0) {
        fileJobDestroy(effTemporaryFileJob);
        effTemporaryFileJob = 0;
    }
}

void effBattleResourceDescriptorRelease(void) {
    if (effResourceBankDescriptor != 0) {
        btlDestroyResourceDescriptor(effResourceBankDescriptor);
        effResourceBankDescriptor = 0;
    }
    if (effResourceBankEntries != 0) {
        btlDestroyEntryList(effResourceBankEntries);
        effResourceBankEntries = 0;
    }
}

void effPollResourceBankSlot(char *path, u32 flags, EffResourceBankSlot *selection) {

    if (effResourceBankEntries == 0) {
        effResourceBankEntries = btlScanDirectory(path, flags);
        effResourceBankDescriptor = btlCreateResourceDescriptor(effResourceBankEntries);
        btlSetResourceBrowserPosition(effResourceBankDescriptor, 0xBA, 0x1C);
        return;
    }
    btlUpdateAndDrawResourceBrowser(effResourceBankDescriptor);
    selection->selectionStatus = btlGetResourceBrowserSelectionStatus(effResourceBankDescriptor);
    selection->type = btlFormatSelectedResourceName(effResourceBankDescriptor, selection->fullName);
    selection->pathVariant = btlGetResourcePathVariant(effResourceBankDescriptor);
    btlTrimResourceName(effResourceBankDescriptor, selection->name);
    if (selection->selectionStatus == BTL_RESOURCE_SELECTION_ACCEPTED) {
        btlDestroyResourceDescriptor(effResourceBankDescriptor);
        effResourceBankDescriptor = 0;
        btlDestroyEntryList(effResourceBankEntries);
        effResourceBankEntries = 0;
    }
}

void effInitializeResourceQueue(void) {
    FileJob *record;

    if (effFileQueueNameRecord != 0) {
        btlDestroyResourceNameRecord(effFileQueueNameRecord);
    }
    effFileQueueNameRecord = btlCreateResourceNameRecord(D_003BCF30);
    btlSetResourceNamePosition(effFileQueueNameRecord, 0xC2, 0xC8);
    btlSetResourceNameLengthLimit(effFileQueueNameRecord, 9);
    record = fileQueueGetAt(effFileQueue, effGetSelectedFileQueueIndex());
    btlResourceRecordSetName(effFileQueueNameRecord, record->name);
}

u32 effPollResourceQueue(void) {
    u32 state;

    btlUpdateResourceNameEditor(effFileQueueNameRecord);
    btlFormatResourceNameWithoutExtension(effFileQueueNameRecord, fileQueueGetAt(effFileQueue, effGetSelectedFileQueueIndex())->name);
    state = btlGetResourceNameSelectionStatus(effFileQueueNameRecord);
    if ((u32)(state - BTL_RESOURCE_SELECTION_ACCEPTED) < 2) {
        btlDestroyResourceNameRecord(effFileQueueNameRecord);
        effFileQueueNameRecord = 0;
        return 0;
    }
    return 0x200001;
}

void effReleaseQueuedResourceName(void) {
    if (effQueuedResourceNameRecord != 0) {
        btlDestroyResourceNameRecord(effQueuedResourceNameRecord);
        effQueuedResourceNameRecord = 0;
    }
}

void effQueueResource(const char *extension, const char *resourceName) {
    if (effQueuedResourceNameRecord == 0) {
        effQueuedResourceNameRecord = btlCreateResourceNameRecord(extension);
        btlSetResourceNamePosition(effQueuedResourceNameRecord, 0xC2, 0xC8);
    }
    btlResourceRecordSetName(effQueuedResourceNameRecord, resourceName);
}

void effUpdateResourceQueue(const char *directoryPath, const char *extension, EffQueueRecord *record) {
    u32 state;

    if (effQueuedResourceNameRecord == 0) {
        effQueuedResourceNameRecord = btlCreateResourceNameRecord(extension);
        btlSetResourceNamePosition(effQueuedResourceNameRecord, 0xC2, 0xC8);
        return;
    }
    btlUpdateResourceNameEditor(effQueuedResourceNameRecord);
    btlFormatResourceNameWithExtension(effQueuedResourceNameRecord, record->nameWithExtension);
    btlFormatResourceNameWithoutExtension(effQueuedResourceNameRecord, record->name);
    state = btlGetResourceNameSelectionStatus(effQueuedResourceNameRecord);
    record->completion.state = state;
    if (state == BTL_RESOURCE_SELECTION_ACCEPTED) {
        if (btlPollResourceNameOverwrite(effQueuedResourceNameRecord, directoryPath) !=
            BTL_RESOURCE_WRITE_PERMISSION_NOT_GRANTED) {
            btlDestroyResourceNameRecord(effQueuedResourceNameRecord);
            effQueuedResourceNameRecord = 0;
        } else {
            record->completion.state = BTL_RESOURCE_SELECTION_PENDING;
        }
    }
}

void effReleaseResourceBankDescriptors(void) {
    if (effResourceBankDescriptor != 0) {
        btlDestroyResourceDescriptor(effResourceBankDescriptor);
        effResourceBankDescriptor = 0;
    }
    if (effResourceBankEntries != 0) {
        btlDestroyEntryList(effResourceBankEntries);
        effResourceBankEntries = 0;
    }
}

void effPollResourceBank(s32 flags, EffectPollRecord *record) {
    if (effResourceBankEntries == 0) {
        effResourceBankEntries = btlScanDirectory(0, flags);
        if (flags & BTL_RESOURCE_SCAN_GENERAL) {
            u32 i = 0;
            u32 count = dds3BillboardGeneralNumber();
            if (count != 0) {
                do {
                    char name[0x70];
                    sprintf(name, "GENERAL %d", i);
                    btlAppendEntry(effResourceBankEntries, name, BTL_RESOURCE_ENTRY_CATEGORY_GENERAL, i, 0);
                    i++;
                } while (i < count);
            }
        }
        effResourceBankDescriptor = btlCreateResourceDescriptor(effResourceBankEntries);
        btlSetResourceBrowserPosition(effResourceBankDescriptor, 0xBA, 0x1C);
    } else {
        btlUpdateAndDrawResourceBrowser(effResourceBankDescriptor);
        record->selectionStatus = btlGetResourceBrowserSelectionStatus(effResourceBankDescriptor);
        record->type = btlFormatSelectedResourceName(effResourceBankDescriptor, record->fullName);
        record->pathVariant = btlGetResourcePathVariant(effResourceBankDescriptor);
        if (record->selectionStatus == BTL_RESOURCE_SELECTION_ACCEPTED) {
            btlDestroyResourceDescriptor(effResourceBankDescriptor);
            effResourceBankDescriptor = 0;
            btlDestroyEntryList(effResourceBankEntries);
            effResourceBankEntries = 0;
        }
    }
}

void effResetMappingFlags(void) {
    effColorEditRgbMenuOpen = 0;
    effColorEditPage = 0;
    effColorEditSelectedKey = 0;
    effColorEditSelectedAlphaKey = 1;
    effMappingState.cursor = 1;
    effMappingState.flags |= 0x10;
}

INCLUDE_ASM(const s32, "code_00242308", effEditColorTrack);

s32 effMapObjectWithTemporaryMappingTable(s32 request) {
    s32 result;
    s32 object = ((EffMappingRequest *)request)->object;

    effMappingState.rows = D_0038F7D8;
    effMappingState.count = 8;
    result = effEditColorTrack(object + 0x2c, object + 0x50, ((EffMappingObject *)object)->valueB8);
    effMappingState.rows = D_0038F658;
    effMappingState.count = 8;
    return result;
}

/* Edit the request's G-/K-family COLOR tracks; return the native editor status. */
s32 effEditGkColorTrack(s32 request) {
    s32 result;
    s32 object = ((EffMappingRequest *)request)->object;

    effMappingState.rows = D_0038F7D8;
    effMappingState.count = 8;
    result = effEditColorTrack(object + 0x48, object + 0x6c, ((EffMappingObject *)object)->valueD4);
    effMappingState.rows = D_0038F658;
    effMappingState.count = 8;
    return result;
}

/* B-/F-family COLOR callback: edit the request's color and adjacent alpha tracks. */
s32 effEditBfColorTrack(s32 request) {
    s32 object;

    object = ((EffMappingRequest *)request)->object;
    return effEditColorTrack(object, object + 0x24, ((EffMappingObject *)object)->valueB8);
}

/* HELPER's COLOR callback edits the shared default color/alpha data. */
s32 effEditHlpColorTrack(void) {
    return effEditColorTrack((s32)D_r1_004D3418, (s32)D_r1_004D3418 + 0x24, ((EffMappingObject *)D_r1_004D3418)->value34);
}

/* Edit T-BOARD color/alpha tracks using the alternate property-option table. */
s32 effEditTBoardColorTrack(s32 request) {
    s32 result;
    s32 object = ((EffMappingRequest *)request)->object;

    effMappingState.rows = D_0038F7D8;
    effMappingState.count = 8;
    result = effEditColorTrack(object, object + 0x24, ((EffMappingObject *)object)->value8C);
    effMappingState.rows = D_0038F658;
    effMappingState.count = 8;
    return result;
}

/* Pass the E-RAIN request's color/alpha tracks and frame bound to the editor. */
s32 effEditRainColorTrack(s32 request) {
    s32 object;

    object = ((EffMappingRequest *)request)->object;
    return effEditColorTrack(object, object + 0x24, ((EffMappingObject *)object)->value34);
}

/* Pass the E-FLARE request's color/alpha tracks and frame bound to the editor. */
s32 effEditFlareColorTrack(s32 request) {
    s32 object;

    object = ((EffMappingRequest *)request)->object;
    return effEditColorTrack(object, object + 0x24, ((EffMappingObject *)object)->value38);
}

/* COLOR property callback shared by the native S-family descriptors. */
s32 effEditSColorTrack(s32 request) {
    s32 object;

    object = ((EffMappingRequest *)request)->object;
    return effEditColorTrack(object, object + 0x24, ((EffMappingObject *)object)->value34);
}

/* Edit R-NOMAL/R-CRCLE COLOR tracks, then restore the default option table. */
s32 effEditRColorTrack(s32 request) {
    s32 result;
    s32 object = ((EffMappingRequest *)request)->object;

    effMappingState.rows = D_r1_00417158;
    effMappingState.count = 8;
    result = effEditColorTrack(object, object + 0x24, ((EffMappingObject *)object)->value34);
    effMappingState.rows = D_0038F658;
    effMappingState.count = 8;
    return result;
}

/* COLOR property callback for the native R-TRACK descriptor. */
s32 effEditRTrackColorTrack(s32 request) {
    s32 object;

    object = ((EffMappingRequest *)request)->object;
    return effEditColorTrack(object, object + 0x24, ((EffMappingObject *)object)->value34);
}

/* D-family COLOR callback; its DDS2 twin also serves O-family descriptors. */
s32 effEditDoColorTrack(s32 request) {
    s32 object;

    object = ((EffMappingRequest *)request)->object;
    return effEditColorTrack(object, object + 0x24, ((EffMappingObject *)object)->value34);
}

s32 effResolveMappingWithTemporaryTable(s32 request) {
    s32 result;
    s32 object = ((EffMappingRequest *)request)->object;

    effMappingState.rows = D_0038F7D8;
    effMappingState.count = 8;
    result = effEditColorTrack(object + 0x4c, object + 0x70, ((EffMappingObject *)object)->valueD8);
    effMappingState.rows = D_0038F658;
    effMappingState.count = 8;
    return result;
}

/* COLOR callback shared by the native N-SPIRL/N-CYLND/N-SPHRE descriptors. */
s32 effEditNColorTrack(s32 request) {
    s32 object;

    object = ((EffMappingRequest *)request)->object;
    return effEditColorTrack(object, object + 0x24, ((EffMappingObject *)object)->value34);
}

/* Edit COLOR in the N-family descriptors' nested POLYGON data. */
s32 effEditNPolygonColorTrack(s32 request) {
    s32 object;

    object = ((EffMappingRequest *)request)->object;
    return effEditColorTrack(object + 0x3c, object + 0x60, ((EffMappingObject *)object)->value80);
}

/* Edit the root U-CYLND color and alpha tracks selected by the request. */
s32 effEditUCylinderColorTrack(s32 request) {
    s32 object;

    object = ((EffMappingRequest *)request)->object;
    return effEditColorTrack(object, object + 0x24, ((EffMappingObject *)object)->value34);
}

/* Edit COLOR in U-CYLND's nested POLYGON data. */
s32 effEditUCylinderPolygonColorTrack(s32 request) {
    s32 object;

    object = ((EffMappingRequest *)request)->object;
    return effEditColorTrack(object + 0x3c, object + 0x60, ((EffMappingObject *)object)->value80);
}

/* U-SMOKE/U-SPIRL COLOR uses its own property options; forward editor status. */
s32 effEditUSmokeSpiralColorTrack(s32 request) {
    s32 result;
    s32 object = ((EffMappingRequest *)request)->object;

    effMappingState.rows = D_0038F7D8;
    effMappingState.count = 8;
    result = effEditColorTrack(object + 0x68, object + 0x8c, ((EffMappingObject *)object)->valueF4);
    effMappingState.rows = D_0038F658;
    effMappingState.count = 8;
    return result;
}

/* Edit U-SPHRE's root COLOR tracks using its frame bound. */
s32 effEditUSphereColorTrack(s32 request) {
    s32 object;

    object = ((EffMappingRequest *)request)->object;
    return effEditColorTrack(object, object + 0x24, ((EffMappingObject *)object)->value70);
}

/* COLOR callback for the CRACK polygon descriptor. */
s32 effEditCrackPolygonColorTrack(s32 request) {
    s32 object;

    object = ((EffMappingRequest *)request)->object;
    return effEditColorTrack(object, object + 0x24, ((EffMappingObject *)object)->value34);
}

/* COLOR callback for the FLASH polygon descriptor. */
s32 effEditFlashPolygonColorTrack(s32 request) {
    s32 object;

    object = ((EffMappingRequest *)request)->object;
    return effEditColorTrack(object + 0x50, object + 0x74, ((EffMappingObject *)object)->value84);
}

/* 2D's COLOR callback restores its property options and forwards editor status. */
s32 effEditTwoDimensionalColorTrack(s32 request) {
    s32 result;
    s32 object = ((EffMappingRequest *)request)->object;

    effMappingState.rows = D_r1_00417158;
    effMappingState.count = 8;
    result = effEditColorTrack(object, object + 0x24, ((EffMappingObject *)object)->value8C);
    effMappingState.rows = D_0038F658;
    effMappingState.count = 8;
    return result;
}

extern EffEditorRow D_r1_00417218[];
extern EffEditorRow D_r1_00417098[];

/* FUNC effEditMColorTrack */
#ifdef NON_MATCHING
/* 7 of 25 words differ, first at +0x20: retail loads the rows address before p->x0C and stores count and rows in a different order. */
void effEditMColorTrack(BaObj *p) {
    s32 object;

    effMappingState.rows = D_r1_00417218;
    effMappingState.count = 8;
    object = p->x0C;
    effEditColorTrack(object + 0x7C, object + 0xA0, ((EffMappingObject *)object)->value108);
    effMappingState.rows = D_r1_00417098;
    effMappingState.count = 8;
}
#else
INCLUDE_ASM(const s32, "code_00242308", effEditMColorTrack);
#endif

extern EffQueuedFileObject D_r1_00417410;
extern EffQueuedFileObject D_r1_00417528;
extern u32 D_r1_0046D9F8;
extern u32 D_r1_0046D9FC;
extern u32 D_r1_0046DA00;

void btlResetEffectWork(void) {
    D_r1_00417410.flags |= 0x10;
    D_r1_00417410.cursor = 0;
    D_r1_00417528.flags |= 0x10;
    D_r1_00417528.cursor = 0;
    D_r1_0046D9F8 = 0;
    D_r1_0046D9FC = 0;
    D_r1_0046DA00 = 0;
}

INCLUDE_ASM(const s32, "code_00242308", effEditScalarTrack);

/* Edit the requested P-family SCALE track; return the native editor status. */
s32 effEditPScaleTrack(BaObj *p) {
    return effEditScalarTrack((EffScalarTrack *)(p->x0C + 0x60), ((EffMappingObject *)p->x0C)->valueB8, D_0038F9D0, D_0038F8E8);
}

/* Edit the requested P-family ROTATE track with its independent editor state. */
s32 effEditPRotationTrack(BaObj *p) {
    return effEditScalarTrack((EffScalarTrack *)(p->x0C + 0x8C), ((EffMappingObject *)p->x0C)->valueB8, D_0038FAD0, D_0038FA20);
}

/* SCALE property callback shared by native G-/K-family descriptors. */
s32 effEditGkScaleTrack(BaObj *p) {
    return effEditScalarTrack((EffScalarTrack *)(p->x0C + 0x7C), ((EffMappingObject *)p->x0C)->valueD4, D_0038F9D0, D_0038F8E8);
}

/* ROTATE property callback for native K-family descriptors. */
s32 effEditKRotationTrack(BaObj *p) {
    return effEditScalarTrack((EffScalarTrack *)(p->x0C + 0xA8), ((EffMappingObject *)p->x0C)->valueD4, D_0038FAD0, D_0038FA20);
}

/* SCALE property callback for native B-family descriptors. */
s32 effEditBScaleTrack(BaObj *p) {
    return effEditScalarTrack((EffScalarTrack *)(p->x0C + 0x34), ((EffMappingObject *)p->x0C)->valueB8, D_0038F9D0, D_0038F8E8);
}

/* B-family ROTATE callback; identical forwarding code does not imply P-family SCALE. */
s32 effEditBRotationTrack(BaObj *p) {
    return effEditScalarTrack((EffScalarTrack *)(p->x0C + 0x60), ((EffMappingObject *)p->x0C)->valueB8, D_0038F9D0, D_0038F8E8);
}

/* AREA property callback shared by B-/F-family descriptors. */
s32 effEditBfAreaTrack(BaObj *p) {
    return effEditScalarTrack((EffScalarTrack *)(p->x0C + 0x8C), ((EffMappingObject *)p->x0C)->valueB8, D_0038F9D0, D_0038F8E8);
}

/* Pass the requested T-BOARD SCALE track and its frame bound to the editor. */
s32 effEditTBoardScaleTrack(BaObj *p) {
    return effEditScalarTrack((EffScalarTrack *)(p->x0C + 0x34), ((EffMappingObject *)p->x0C)->value8C, D_0038F9D0, D_0038F8E8);
}

extern char D_r1_00417478[];

s32 effEditTBoardRotationTrack(BaObj *p) {
    return effEditScalarTrack((EffScalarTrack *)(p->x0C + 0x60), ((EffMappingObject *)p->x0C)->value8C, &D_r1_00417528, D_r1_00417478);
}

/* SCALE property callback for native W-family descriptors. */
s32 effEditWScaleTrack(BaObj *p) {
    return effEditScalarTrack((EffScalarTrack *)(p->x0C + 0x80), ((EffMappingObject *)p->x0C)->valueD8, D_0038F9D0, D_0038F8E8);
}

/* ROTATE property callback for native W-family descriptors. */
s32 effEditWRotationTrack(BaObj *p) {
    return effEditScalarTrack((EffScalarTrack *)(p->x0C + 0xAC), ((EffMappingObject *)p->x0C)->valueD8, D_0038FAD0, D_0038FA20);
}

/* Edit the SCALE_UV curve in U-CYLND's POLYGON child; forward editor status. */
s32 effEditUCylinderPolygonUvScaleTrack(BaObj *p) {
    return effEditScalarTrack((EffScalarTrack *)(p->x0C + 0x90), ((EffMappingObject *)p->x0C)->value80, D_0038F9D0, D_r1_00417378);
}

/* Edit the shared U-SMOKE/U-SPIRL root SCALE curve; forward editor status. */
s32 effEditUSmokeSpiralScaleTrack(BaObj *p) {
    return effEditScalarTrack((EffScalarTrack *)(p->x0C + 0x9C), ((EffMappingObject *)p->x0C)->valueF4, D_0038F9D0, D_0038F8E8);
}

/* Edit the shared U-SMOKE/U-SPIRL root ROTATE curve; forward editor status. */
s32 effEditUSmokeSpiralRotationTrack(BaObj *p) {
    return effEditScalarTrack((EffScalarTrack *)(p->x0C + 0xC8), ((EffMappingObject *)p->x0C)->valueF4, D_0038FAD0, D_0038FA20);
}

/* Edit SCALE_UV in their POLYGON children; forward the numeric editor's status. */
s32 effEditUSmokeSpiralPolygonUvScaleTrack(BaObj *p) {
    return effEditScalarTrack((EffScalarTrack *)(p->x0C), ((EffMappingObject *)p->x0C)->valueF4, D_0038F9D0, D_r1_00417378);
}

/* Edit the native U-SPHRE root SCALE curve; forward editor status. */
s32 effEditUSphereScaleTrack(BaObj *p) {
    return effEditScalarTrack((EffScalarTrack *)(p->x0C + 0x34), ((EffMappingObject *)p->x0C)->value70, D_0038F9D0, D_r1_00417378);
}

/* Edit the native 2D root SCALE curve; forward editor status. */
s32 effEditTwoDimensionalScaleTrack(BaObj *p) {
    return effEditScalarTrack((EffScalarTrack *)(p->x0C + 0x34), ((EffMappingObject *)p->x0C)->value8C, D_0038F9D0, D_0038F8E8);
}

/* Edit the native 2D root ROTATE curve; forward editor status. */
s32 effEditTwoDimensionalRotationTrack(BaObj *p) {
    return effEditScalarTrack((EffScalarTrack *)(p->x0C + 0x60), ((EffMappingObject *)p->x0C)->value8C, D_0038F9D0, D_0038F8E8);
}


extern char D_r1_00417328[];

s32 effEditMScaleTrack(BaObj *p) {
    return effEditScalarTrack((EffScalarTrack *)(p->x0C + 0xB0), ((EffMappingObject *)p->x0C)->value108, &D_r1_00417410, &D_r1_00417328);
}

void func_r1_00249448(void) {
    effColorEditSelectedAlphaKey = 1;
}

INCLUDE_ASM(const s32, "code_00242308", func_r1_00249458);

/* ALPHA callback for POLYGON children of the native S-family descriptors. */
s32 effEditSPolygonAlphaTrack(s32 work) {
    return func_002BB188((SdfAlphaTrack *)(((BaObj *)work)->x0C + 0x3c), ((EffMappingObject *)((BaObj *)work)->x0C)->value4C);
}

/* D-family POLYGON FADE callback, shared with O in DDS2; retain bound plus one. */
s32 effEditDoPolygonFadeTrack(s32 work) {
    return func_002BB188((SdfAlphaTrack *)(((BaObj *)work)->x0C + 0x60), ((EffMappingObject *)((BaObj *)work)->x0C)->value3C + 1);
}

/* FADE callback for N-family POLYGON children; preserve bound plus one. */
s32 effEditNPolygonFadeTrack(s32 work) {
    return func_002BB188((SdfAlphaTrack *)(((BaObj *)work)->x0C + 0x70), ((EffMappingObject *)((BaObj *)work)->x0C)->value8C + 1);
}

/* FADE callback for the native U-CYLND POLYGON child. */
s32 effEditUCylinderPolygonFadeTrack(s32 work) {
    return func_002BB188((SdfAlphaTrack *)(((BaObj *)work)->x0C + 0x70), ((EffMappingObject *)((BaObj *)work)->x0C)->value8C + 1);
}

/* FADE callback for the native U-SPHRE POLYGON child. */
s32 effEditUSpherePolygonFadeTrack(s32 work) {
    return func_002BB188((SdfAlphaTrack *)(((BaObj *)work)->x0C + 0x60), ((EffMappingObject *)((BaObj *)work)->x0C)->value74 + 1);
}

s32 effPollFileQueueRecord(s32 request) {
    u8 record[0xE0];
    EffectPollRecord *status = (EffectPollRecord *)record;
    s32 selectionStatus;
    s32 result = 0x600001;

    effPollResourceBank(request, status);
    selectionStatus = status->selectionStatus;
    if (selectionStatus == BTL_RESOURCE_SELECTION_CANCELED) {
        result = 0x400000;
    } else if (selectionStatus == BTL_RESOURCE_SELECTION_ACCEPTED) {
        if (status->type != 8) {
            FileJob *entry = fileQueueGetAt(effFileQueue, -status->pathVariant);
            if (effCurrentFileQueueEntry != entry) {
                fileQueueDetachSectorFollower(effFileQueue, effCurrentFileQueueEntry);
                fileQueueLinkJobToSectorLeader(effFileQueue, effCurrentFileQueueEntry, entry);
            }
        } else {
            fileQueueDetachSectorFollower(effFileQueue, effCurrentFileQueueEntry);
            fileJobSetSecondaryData(effQueuedFileHandle, &status->pathVariant, 4, 4);
        }
        result = 0x400002;
    }
    return result;
}

/* Poll queued TMX/P2A/general resources with the TLP format enabled. */
s32 effPollResourceQueueWithTlp(void) {
    return effPollFileQueueRecord(BTL_RESOURCE_SCAN_TMX | BTL_RESOURCE_SCAN_P2A | BTL_RESOURCE_SCAN_GENERAL | BTL_RESOURCE_SCAN_TLP);
}

/* Separate native descriptor callback with the same TLP-enabled queue filter. */
s32 effPollAlternateResourceQueueWithTlp(void) {
    return effPollFileQueueRecord(BTL_RESOURCE_SCAN_TMX | BTL_RESOURCE_SCAN_P2A | BTL_RESOURCE_SCAN_GENERAL | BTL_RESOURCE_SCAN_TLP);
}

/* Poll the queue without adding the TLP flag to its format mask. */
s32 effPollResourceQueueWithoutTlp(void) {
    return effPollFileQueueRecord(BTL_RESOURCE_SCAN_TMX | BTL_RESOURCE_SCAN_P2A | BTL_RESOURCE_SCAN_GENERAL);
}

s32 effPollFileRecord(char *path, s32 flags) {
    EffResourceBankSlot record;
    s32 kind;
    s32 result = 0x600001;

    effPollResourceBankSlot(path, flags, &record);
    kind = record.selectionStatus;
    if (kind == BTL_RESOURCE_SELECTION_CANCELED) {
        result = 0x400000;
    } else if (kind == BTL_RESOURCE_SELECTION_ACCEPTED) {
        s32 type;
        fileQueueDetachSectorFollower(effFileQueue, effCurrentFileQueueEntry);
        type = record.type;
        if (type != 8) {
            fileJobCopyCommandIntoSecondaryData(effQueuedFileHandle,
                                                (const char *)&record, effClassifyResourceMask(type));
        } else {
            fileJobSetSecondaryData(effQueuedFileHandle, &record.pathVariant, 4, 4);
        }
        result = 0x400002;
    }
    return result;
}

/* Poll the TEXTURE property chooser in /tool/effect/mat/ and return its status. */
s32 effPollTextureResourceFile(void) {
    return effPollFileRecord(D_003B3B88, BTL_RESOURCE_SCAN_TMX | BTL_RESOURCE_SCAN_P2A | BTL_RESOURCE_SCAN_TLP);
}

/* Alternate descriptor family's TEXTURE callback; retain its separate entrypoint. */
s32 effPollAlternateTextureResourceFile(void) {
    return effPollFileRecord(D_003B3B88, BTL_RESOURCE_SCAN_TMX | BTL_RESOURCE_SCAN_P2A | BTL_RESOURCE_SCAN_TLP);
}

/* Poll EP resources from /tool/effect/ep/ and return the chooser status. */
s32 effPollEpResourceFile(void) {
    return effPollFileRecord(D_r1_00456E58, BTL_RESOURCE_SCAN_EP);
}

/* Poll EPL resources from /tool/effect/ and return the chooser status. */
s32 effPollEplResourceFile(void) {
    return effPollFileRecord(D_003B39E0, BTL_RESOURCE_SCAN_EPL);
}

/* Separate native EP descriptor callback using the same directory and format. */
s32 effPollAlternateEpResourceFile(void) {
    return effPollFileRecord(D_r1_00456E58, BTL_RESOURCE_SCAN_EP);
}

/* TMX-only TEXTURE chooser for B-/F-family MATERIAL groups. */
s32 effPollBfMaterialTmxFile(void) {
    return effPollFileRecord(D_003B3B88, BTL_RESOURCE_SCAN_TMX);
}

/* TMX-only TEXTURE chooser for N-family MATERIAL groups. */
s32 effPollNMaterialTmxFile(void) {
    return effPollFileRecord(D_003B3B88, BTL_RESOURCE_SCAN_TMX);
}

/* TMX-only TEXTURE chooser for S-RAD_T/SPK_T/RNG_T/SCT_T MATERIAL groups. */
s32 effPollSTMaterialTmxFile(void) {
    return effPollFileRecord(D_003B3B88, BTL_RESOURCE_SCAN_TMX);
}

u32 effFileJobSecondaryDataSet(void) {
    u32 zero = 0;
    fileJobSetSecondaryData(effQueuedFileHandle, &zero, 4, 4);
    return 0x400002;
}

/* Poll PB resources from /tool/effect/hlp/ and return the chooser status. */
s32 effPollHlpPbResourceFile(void) {
    return effPollFileRecord(D_003B3BA0, BTL_RESOURCE_SCAN_PB);
}

s32 effPollMMaterialPbFile(void) {
    return effPollFileRecord(D_r1_00457028, BTL_RESOURCE_SCAN_PB);
}

s32 effPollMMaterialEplFile(void) {
    return effPollFileRecord(D_r1_00456E70, BTL_RESOURCE_SCAN_EPL);
}

s32 effPollOMaterialPbFile(void) {
    return effPollFileRecord(D_r1_00457040, BTL_RESOURCE_SCAN_PB);
}

u32 fileLoadEffectSlotA(void) {
    EffResourceBankSlot fileInfo;
    FileJobPayload *job;
    FileJob *entry;
    EffFileResourceRecord *resource;
    void *fileData;
    s32 status;
    u32 result;

    effPollResourceBankSlot(D_003B3B88, BTL_RESOURCE_SCAN_PB, &fileInfo);
    status = fileInfo.selectionStatus;
    result = 0x600001;
    if (status == BTL_RESOURCE_SELECTION_CANCELED) {
        result = 0x400000;
    } else if (status == BTL_RESOURCE_SELECTION_ACCEPTED) {
        job = fileCreateJob(3);
        fileJobSetPrimaryData(job, D_r1_00408D60.start, D_r1_00408D60.length,
                      D_r1_00408D60.mode);
        fileJobCopyCommandIntoSecondaryData(job, (const char *)&fileInfo,
                      effClassifyResourceMask(fileInfo.type));
        entry = fileAppendJob(effFileQueue, (u32)job);
        effCurrentFileQueueEntry = entry;
        memcpy(effSelectedJobHeader, entry, 0x80);
        effQueuedFileHandle = (FileJobPayload *)entry->id;
        resource = (EffFileResourceRecord *)effFindAssetData(entry);
        strcpy(entry->name, resource->name);
        fileData = fileResolvePrimaryBuffer(effQueuedFileHandle);
        memcpy(resource->buffer, fileData, resource->size);
        D_003BD064 = effCreateBattleCameraJob((u8 *)resource);
        effQueuedFileObject = effFindAssetObject(entry);
        ((EffQueuedFileObject *)effQueuedFileObject)->linkedState = (u8 *)&effPartsListMenu;
        effResetFileResourceManager();
        if (effTemporaryFileJob != 0) {
            fileJobDestroy(effTemporaryFileJob);
            effTemporaryFileJob = 0;
        }
        result = 0x800002;
    }
    return result;
}

INCLUDE_ASM(const s32, "code_00242308", func_r1_00249FA0);

u32 fileLoadEffectSlotHelp(void) {
    EffResourceBankSlot fileInfo;
    u32 result;
    s32 status;
    FileJobPayload *job;
    FileJob *entry;
    EffFileResourceRecord *resource;
    u8 *buffer;
    sdfFileCmdHandle_t *command;
    u32 totalLength;
    u32 dataLength;
    struct sdfMemHandle_t *allocation;

    effPollResourceBankSlot(D_003B3BA0, BTL_RESOURCE_SCAN_PB, &fileInfo);
    status = fileInfo.selectionStatus;
    result = 0x600001;
    if (status == BTL_RESOURCE_SELECTION_CANCELED) {
        result = 0x400000;
    } else if (status == BTL_RESOURCE_SELECTION_ACCEPTED) {
        u32 headerBytes = 0x40;
        struct sdfMemHandle_t *oldAllocation;
        u32 queuedFile;
        job = fileCreateJob(6);
        command = sdfFileOpen(&fileInfo);
        dataLength = sdfFileLength(command);
        totalLength = dataLength + headerBytes;
        allocation = sdfMemAllocDirect(totalLength);
        buffer = (u8 *)sdfMemLock(allocation);
        memset(buffer, 0, headerBytes);
        sdfFileRead(command, buffer + headerBytes, dataLength);
        sdfFileClose(command);
        fileJobSetPrimaryData(job, buffer, totalLength, 0);
        entry = fileAppendJob(effFileQueue, (u32)job);
        effCurrentFileQueueEntry = entry;
        resource = (EffFileResourceRecord *)effFindAssetData(entry);
        strcpy(entry->name, resource->name);
        memcpy(effSelectedJobHeader, entry, 0x80);
        queuedFile = entry->id;
        oldAllocation = resource->allocation;
        effQueuedFileHandle = (FileJobPayload *)queuedFile;
        if (oldAllocation != 0) {
            sdfMemFreeDirect(oldAllocation);
        }
        resource->allocation = allocation;
        resource->buffer = buffer;
        resource->size = dataLength + headerBytes;
        resource->mode = 0;
        memcpy(D_r1_004D3418, D_r1_00409908, 0x3C);
        D_003BD064 = effCreateBattleCameraJob(resource);
        effQueuedFileObject = effFindAssetObject(entry);
        ((EffQueuedFileObject *)effQueuedFileObject)->linkedState = (u8 *)&effPartsListMenu;
        effResetFileResourceManager();
        if (effTemporaryFileJob != 0) {
            fileJobDestroy(effTemporaryFileJob);
            effTemporaryFileJob = 0;
        }
        result = 0x800002;
    }
    return result;
}

u32 fileLoadEffectSlotB(void) {
    EffResourceBankSlot fileInfo;
    FileJobPayload *job;
    FileJob *entry;
    EffFileResourceRecord *resource;
    void *fileData;
    s32 status;
    u32 result;

    effPollResourceBankSlot(D_003B3B88, BTL_RESOURCE_SCAN_PB, &fileInfo);
    status = fileInfo.selectionStatus;
    result = 0x600001;
    if (status == BTL_RESOURCE_SELECTION_CANCELED) {
        result = 0x400000;
    } else if (status == BTL_RESOURCE_SELECTION_ACCEPTED) {
        job = fileCreateJob(18);
        fileJobSetPrimaryData(job, D_r1_00411E28.start, D_r1_00411E28.length,
                      D_r1_00411E28.mode);
        fileJobCopyCommandIntoSecondaryData(job, (const char *)&fileInfo,
                      effClassifyResourceMask(fileInfo.type));
        entry = fileAppendJob(effFileQueue, (u32)job);
        effCurrentFileQueueEntry = entry;
        memcpy(effSelectedJobHeader, entry, 0x80);
        effQueuedFileHandle = (FileJobPayload *)entry->id;
        resource = (EffFileResourceRecord *)effFindAssetData(entry);
        strcpy(entry->name, resource->name);
        fileData = fileResolvePrimaryBuffer(effQueuedFileHandle);
        memcpy(resource->buffer, fileData, resource->size);
        D_003BD064 = effCreateBattleCameraJob(resource);
        effQueuedFileObject = effFindAssetObject(entry);
        ((EffQueuedFileObject *)effQueuedFileObject)->linkedState = (u8 *)&effPartsListMenu;
        effResetFileResourceManager();
        if (effTemporaryFileJob != 0) {
            fileJobDestroy(effTemporaryFileJob);
            effTemporaryFileJob = 0;
        }
        result = 0x800002;
    }
    return result;
}

INCLUDE_RODATA(const s32, "code_00242308", D_r1_00456FB8);

INCLUDE_RODATA(const s32, "code_00242308", D_r1_00456FC8);

INCLUDE_RODATA(const s32, "code_00242308", D_r1_00456FD8);

INCLUDE_RODATA(const s32, "code_00242308", D_r1_00456FE8);

INCLUDE_RODATA(const s32, "code_00242308", D_r1_00456FF8);

INCLUDE_RODATA(const s32, "code_00242308", D_r1_00457008);

INCLUDE_RODATA(const s32, "code_00242308", D_r1_00457018);

INCLUDE_RODATA(const s32, "code_00242308", D_r1_00457028);

INCLUDE_RODATA(const s32, "code_00242308", D_r1_00457040);

u32 effLoadFileSlotF2(void) {
    EffResourceBankSlot fileInfo;
    FileJobPayload *job;
    FileJob *entry;
    EffFileResourceRecord *resource;
    void *fileData;
    s32 status;
    u32 result;

    effPollResourceBankSlot("/tool/effect/f2/", BTL_RESOURCE_SCAN_F2, &fileInfo);
    status = fileInfo.selectionStatus;
    result = 0x600001;
    if (status == BTL_RESOURCE_SELECTION_CANCELED) {
        result = 0x400000;
    } else if (status == BTL_RESOURCE_SELECTION_ACCEPTED) {
        job = fileCreateJob(0x14);
        fileJobSetPrimaryData(job, D_r1_00414710.start, D_r1_00414710.length,
                      D_r1_00414710.mode);
        fileJobCopyCommandIntoSecondaryData(job, (const char *)&fileInfo,
                      effClassifyResourceMask(fileInfo.type));
        entry = fileAppendJob(effFileQueue, (u32)job);
        effCurrentFileQueueEntry = entry;
        memcpy(effSelectedJobHeader, entry, 0x80);
        effQueuedFileHandle = (FileJobPayload *)entry->id;
        resource = (EffFileResourceRecord *)effFindAssetData(entry);
        strcpy(entry->name, resource->name);
        fileData = fileResolvePrimaryBuffer(effQueuedFileHandle);
        memcpy(resource->buffer, fileData, resource->size);
        D_003BD064 = effCreateBattleCameraJob(resource);
        effQueuedFileObject = effFindAssetObject(entry);
        ((EffQueuedFileObject *)effQueuedFileObject)->linkedState = (u8 *)&effPartsListMenu;
        effResetFileResourceManager();
        if (effTemporaryFileJob != 0) {
            fileJobDestroy(effTemporaryFileJob);
            effTemporaryFileJob = 0;
        }
        result = 0x800002;
    }
    return result;
}

u32 effLoadMaterialFile(void) {
    EffResourceBankSlot fileInfo;
    FileJobPayload *job;
    FileJob *entry;
    EffFileResourceRecord *resource;
    void *fileData;
    s32 status;
    u32 result;

    effPollResourceBankSlot(D_003B3B88, BTL_RESOURCE_SCAN_P2A, &fileInfo);
    status = fileInfo.selectionStatus;
    result = 0x600001;
    if (status == BTL_RESOURCE_SELECTION_CANCELED) {
        result = 0x400000;
    } else if (status == BTL_RESOURCE_SELECTION_ACCEPTED) {
        job = fileCreateJob(0x16);
        fileJobSetPrimaryData(job, D_r1_00414EA8.start, D_r1_00414EA8.length,
                      D_r1_00414EA8.mode);
        fileJobCopyCommandIntoSecondaryData(job, (const char *)&fileInfo,
                      effClassifyResourceMask(fileInfo.type));
        entry = fileAppendJob(effFileQueue, (u32)job);
        effCurrentFileQueueEntry = entry;
        memcpy(effSelectedJobHeader, entry, 0x80);
        effQueuedFileHandle = (FileJobPayload *)entry->id;
        resource = (EffFileResourceRecord *)effFindAssetData(entry);
        strcpy(entry->name, resource->name);
        fileData = fileResolvePrimaryBuffer(effQueuedFileHandle);
        memcpy(resource->buffer, fileData, resource->size);
        D_003BD064 = effCreateBattleCameraJob(resource);
        effQueuedFileObject = effFindAssetObject(entry);
        ((EffQueuedFileObject *)effQueuedFileObject)->linkedState = (u8 *)&effPartsListMenu;
        effResetFileResourceManager();
        if (effTemporaryFileJob != 0) {
            fileJobDestroy(effTemporaryFileJob);
            effTemporaryFileJob = 0;
        }
        result = 0x800002;
    }
    return result;
}

void effResetFileResourceManager(void) {
    effColorEditRgbMenuOpen = 0;
    effColorEditSelectedAlphaKey = 1;
    effColorEditSelectedKey = 0;
    effColorEditPage = 0;
    D_003BD158 = 0;
    D_003BD15C = 0;
    D_003BD160 = 0;
}

u16 effClassifyResourceMask(s32 flags) {
    switch (flags) {
        case 1:   return 1;
        case 2:   return 2;
        case 4:   return 3;
        case 8:   return 4;
        case 16:  return 6;
        case 32:  return 5;
        case 64:  return 7;
        case 128: return 8;
        default:  return 0;
    }
}
INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D18C);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D190);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D198);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D1A0);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D1A8);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D1B0);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D1B8);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D1C0);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D1C8);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D1D0);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D1D8);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D1E0);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D1E8);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D1F0);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D1F8);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D200);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D208);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D210);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D218);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D220);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D228);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D230);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D238);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D240);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D248);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D250);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D258);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D260);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D268);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D270);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D278);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D280);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D288);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D290);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D298);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D2A0);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D2A8);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D2B0);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D2B8);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D2C0);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D2C8);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D2D0);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D2D8);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D2E0);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D2E8);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D2F0);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D2F8);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D300);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D308);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D310);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D318);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D320);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D328);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D330);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D338);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D340);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D348);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D350);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D358);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D360);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D364);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D368);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D370);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D378);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D380);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D388);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D390);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D398);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D3A0);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D3A8);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D3B0);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D3B8);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D3C0);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D3C8);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D3D0);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D3D8);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D3E0);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D3E8);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D3F0);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D3F8);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D400);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D408);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D410);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D418);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D420);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D428);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D430);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D438);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D440);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D448);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D450);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D458);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D460);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D468);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D470);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D478);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D480);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D488);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D490);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D498);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D4A0);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D4A8);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D4B0);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D4B8);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D4C0);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D4C8);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D4D0);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D4D8);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D4E0);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D4E8);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D4F0);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D4F8);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D500);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D508);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D510);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D518);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D520);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D528);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D530);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D538);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D540);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D548);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D550);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D558);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D560);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D568);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D570);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D578);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D580);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D588);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D590);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D598);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D5A0);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D5A8);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D5B0);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D5B8);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D5C0);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D5C8);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D5D0);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D5D8);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D5E0);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D5E8);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D5F0);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D5F8);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D600);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D608);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D610);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D618);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D620);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D628);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D630);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D638);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D640);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D648);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D650);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D658);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D660);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D668);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D670);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D678);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D680);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D688);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D690);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D698);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D6A0);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D6A8);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D6B0);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D6B8);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D6C0);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D6C8);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D6D0);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D6D8);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D6E0);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D6E8);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D6F0);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D6F8);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D700);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D708);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D710);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D718);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D720);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D728);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D730);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D738);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D740);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D748);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D750);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D758);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D760);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D768);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D770);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D778);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D780);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D788);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D790);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D798);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D7A0);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D7A8);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D7B0);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D7B8);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D7C0);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D7C8);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D7D0);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D7D8);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D7E0);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D7E8);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D7F0);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D7F8);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D800);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D808);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D810);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D818);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D820);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D828);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D830);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D838);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D840);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D848);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D850);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D858);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D860);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D868);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D870);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D878);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D880);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D888);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D890);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D898);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D8A0);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D8A8);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D8B0);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D8B8);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D8C0);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D8C8);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D8D0);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D8D8);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D8E0);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D8E8);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D8F0);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D8F8);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D900);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D904);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D908);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D90C);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D910);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D914);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D918);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D91C);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D920);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D928);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D930);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D938);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D940);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D944);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D948);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D950);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D958);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D960);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D968);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D970);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D978);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D980);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D988);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D990);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D998);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D9A0);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D9A8);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D9A9);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D9AA);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D9AB);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D9AC);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D9B0);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D9B4);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D9B8);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D9BC);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D9C0);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D9C4);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D9C8);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D9CC);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D9D0);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D9D8);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D9E0);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D9E8);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D9F0);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D9F8);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046D9FC);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046DA00);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046DA04);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046DA08);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046DA0C);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046DA10);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046DA18);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046DA20);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046DA28);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046DA30);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046DA38);

INCLUDE_SDATA(const s32, "code_00242308", D_r1_0046DA40);

