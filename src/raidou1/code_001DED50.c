#include "common.h"

/* Declarations from dds-decomp src/dds1/game/code_00151F58.c */
#ifndef VERSION_DDS1
#define VERSION_DDS1 1
#endif

#include "dds/sdk_ps2.h"

#include "dds/eff_composite_gs_descriptor.h"

#include "dds/eff_unit_object.h"

#include "dds/sdf_projection.h"

#include "dds/sdf_gs_header.h"

#include "dds/sdf_quaternion.h"

#include "dds/common.h"

#include "dds/sdf_vu0.h"

#include "dds/sdf_vu_matrix.h"

#include "dds/eff_generated_texture_descriptor.h"

#include "dds/dds3_draw_buffer.h"

#include "dds/dds3_frame_draw_buffer.h"

#include "dds/sdf_trig.h"

#include "dds/eff_random.h"

#include "dds/sdf.h"

#include "dds/sdf_asset_state.h"

#include "dds/sdf_chip.h"

#include "dds/sdf_packet_list.h"

#include "dds/bill_object_api.h"

#include "dds/bill_linked_value.h"

#include "dds/par_cell_api.h"

#include "dds/sdf_heap_api.h"

#include "dds/sdf_gs_packet.h"

#include "dds/pcp_vu0.h"

#include "dds/sdf_vu_transform_api.h"

#include "dds/ee_mmi.h"

#include "dds/sdf_asset_create.h"

#include "dds/eff.h"

#include "dds/eff_particle_emitter_variants.h"

#include "dds/eff_template_scale_record.h"

#include "dds/eff_particle_view.h"

#include "dds/eff_look_at_ring.h"

#include "dds/eff_track_poly_list.h"

#include "dds/par_table.h"

#include "dds/par_color_ramp.h"

#include "dds/par_kind_api.h"

#include "dds/eff_particle_record.h"

#include "dds/fpu.h"

#include "dds/sdf_packet_alloc.h"

#include "dds/eff_packet_params.h"

#define BILL_VARIANT_MASK 0xFFFF

#define EFF_PACKET_LIST_BYTES 0x20

extern EffectConfig D_0034DF54[];

extern billHandle_t *hGeneralBillboard[];

void effDrawGeneratedTextureQuad(sdfDrawTag_t *packet, EffGeneratedTextureDescriptor *source);

void effDrawCompositeTextureQuad(sdfDrawTag_t *packet, EffCompositeGsDescriptor *source);

/* Create a billboard sharing the indexed entry's resource. Word two of the
 * resource stores the reference count; the billHandle_t payload is not an emitter. */
billHandle_t *dds3BillboardGeneral(s32 index);

u32 dds3BillboardGeneralNumber(void);

sdfTexHandle_t *dds3BillboardGetGeneralTex(s32 index);

void effCopyVector(f32 destination[4], const f32 source[4]);

/* Set the radian rotation applied to the billboard quad corners. */
void dds3BillboardSetRotate(billHandle_t *effect, float angle);

/* Set the two child scale components stored at +0x10 and +0x14. */
void dds3BillboardSetScale(billHandle_t *effect, float x, float y);

void dds3BillboardSetRGBA(billHandle_t *effect, u32 value);

void billSetChildTextureQuad(billHandle_t *effect, const BillTextureQuad *textureQuad);

/* Narrow mode for the child kinds, or update each resolved animation entry. */
void dds3BillboardSetMode(billHandle_t *effect, s32 mode);

/* Store the variant bits in one child payload or every child of a list. */
void dds3BillboardSetCondition(billHandle_t *effect, s16 variant);

sdfTexHandle_t *dds3BillboardGetTex(billHandle_t *effect);

u16 dds3BillboardGetType(billHandle_t *effect);

/* Store the low halfword through the kind-specific variant field. */
void dds3BillboardSetDrawType(billHandle_t *effect, s32 variantValue);

u16 dds3BillboardGetDrawType(billHandle_t *effect);

/* Replace the selected list entry only when its index changes. */
void dds3BillboardSetAnimNumber(billHandle_t *effect, u32 entryIndex);

/* Read the selected entry for list billboards; other kinds have none. */
s32 dds3BillboardGetCurrentAnimNumber(billHandle_t *effect);

s32 dds3BillboardGetAnimNumber(billHandle_t *billboard);

/* Select the frame and make the next animation update advance immediately. */
void dds3BillboardSetAnimKey(billHandle_t *effect, u32 startFrame);

/* Select the frame and hold it for one animation update before advancing. */
void dds3BillboardSetAnimKey2(billHandle_t *effect, u32 startFrame);

/* Read the animation modulus of the first entry, if this is a list billboard. */
s32 dds3BillboardGetAnimFrame(const billHandle_t *effect);

/* Read the animation-active gate for list billboards. */
u16 dds3BillboardGetAnimState(const billHandle_t *effect);

s32 dds3BillboardGetAnimType(billHandle_t *billboard);

void dds3BillboardSetAnimColorAdvMode(billHandle_t *billboard);

/* Kind 0 stores half the supplied width/height; other kinds remain unchanged. */
void dds3BillboardSetXYOffset(billHandle_t *billboard, float width, float height);

extern u8 sdfViewMatrix[];

extern u8 sdfProjectionMatrix[];

extern u8 D_00324650[];

extern u8 D_00324660[];

extern u8 D_0034E080[];

/* vu0 routine: project position and position + scaled offset; return the reverse XY heading. */
f32 effComputeProjectedOffsetAngle(const void *position, const void *offset);

extern f32 aViewTarget[4], aViewEye[4];

/* vu0 routine: 1 - |dot(normalised eye - target, axis)|, how far the axis is from the view direction */
f32 dds3BillboardGetDirectionScale(void *self, f32 *axis);

/* Create the indexed billboard and render asset; only the second matrix is initialized. */
u8 *dds3P2APolygonCreate(s32 entryIndex);

/* Retain a cloned billboard's shared data, create a fresh render asset, and reset both matrices. */
u8 *dds3P2APolygonCopy(EffUnitObject *source);

void dds3P2APolygonDelete(EffUnitObject *instance);

void dds3P2APolygonSetPosition(EffUnitObject *instance, const f32 position[4]);

void dds3P2APolygonSetScale(EffUnitObject *instance, float scale);

/* vu0 routine: copy the supplied 4x4 matrix to the instance's second matrix. */
void dds3P2APolygonSetRotateMatrix(EffUnitObject *instance, void *matrix);

void dds3P2APolygonSetRGBA(EffUnitObject *instance, u32 value);

/* vu0 routine: copy a 4x4 matrix (four quadwords) through vf28-vf31 */
void dds3P2APolygonSetDirection(void *dst, void *src);

/* Kind 1 writes mode plus one/two entry values; untouched output words retain their contents. */
void dds3P2APolygonGetResource(EffUnitObject *instance, s32 *modeValues);

extern u32 D_0034E090[],D_0034E0A0[];

extern sdfGrOt_t *D_0034E0B0[];

extern void *effPkt3DPrimMakePoly(sdf3DPrimPolyParm_t *);

extern void sdf3DPrimLinkMat(sdfDrawTag_t *packetList, sdfMat_t *asset, s32 (*allocatePacket)(s32));

extern void sdf3DPrimLinkTm(sdfDrawTag_t *packetList, s32 (*allocatePacket)(s32));

void dds3P2APolygonDraw(EffUnitObject *instance);

extern void sdfBillBoardMakeTMPacket(u8 *, const f32 *);

extern void sdfBillBoardMake1(u8 *, const f32 *, u32, const BillTextureQuad *, const f32 *);

void effDrawGeneratedTextureQuad(sdfDrawTag_t *list, EffGeneratedTextureDescriptor *source);

/* Allocate/init a packet list, generate its texture payload, then append it to the draw surface. */
void effSubmitGeneratedTexturePacket(sdfGrOt_t *surface, EffGeneratedTextureDescriptor *source);

extern dds3FrameDrawBuffer_t dds3FrameDrawPacketRecords[];

extern void sdfGrSetFrameMaskRGB(const u8 *, SdfDmaReferenceChainPacket *, s32);

extern void sdfGrSetFrameMaskReset(const u8 *, SdfDmaReferenceChainPacket *, s32);

extern void sdfMultiBillBoardMake1(u8 *, const f32 *, u32, u32,
                        const BillTextureQuad *, const BillTextureQuad *, const f32 *);

void effDrawCompositeTextureQuad(sdfDrawTag_t *list, EffCompositeGsDescriptor *source);

/* Allocate/init a packet list, generate its composite GS payload, then append it to the draw surface. */
void effSubmitCompositeGsPacket(sdfGrOt_t *surface, EffCompositeGsDescriptor *source);
/* End of declarations from src/dds1/game/code_00151F58.c */

void effDrawGeneratedTextureQuad(sdfDrawTag_t *list, EffGeneratedTextureDescriptor *source) {
    f32 matrix[16] __attribute__((aligned(16)));
    BillTextureQuad uv;
    f32 corners[4][2];
    SdfGsTexturePacket *texturePacket;
    u64 *packet;
    s16 width, height;
    f32 halfWidth, halfHeight;
    f32 cosine, sine;
    f32 cornerX, cornerY;

    texturePacket = (SdfGsTexturePacket *)sdfGrAllocPacketBuff(sizeof(SdfGsTexturePacket));
    texturePacket->header.dmaTag = 4;
    texturePacket->header.vifCommands = 0x5000000410000000ULL;
    texturePacket->header.gifTag = 0x1000000000008003ULL;
    texturePacket->header.gifRegisters = SDF_GIF_REGISTER_AD;
    texturePacket->sampling.value = sdfTexGetTex1Reg(source->texture);
    texturePacket->sampling.registerId = SDF_GS_TEX1_1;
    texturePacket->texture.value = sdfTexGetTex0Reg(source->texture);
    texturePacket->texture.registerId = SDF_GS_TEX0_1;
    texturePacket->clamp.value = sdfTexGetClampReg(source->texture);
    texturePacket->clamp.registerId = SDF_GS_CLAMP_1;
    sdfDrawTagLink(list, (u32)texturePacket);

    EE_MMI_UNIT_MATRIX(matrix);
    packet = (u64 *)sdfGrAllocPacketBuff(0x38);
    sdfBillBoardMakeTMPacket((u8 *)packet, matrix);
    sdfDrawTagLink(list, (u32)packet);

    width = (u16)source->texture->width << 4;
    height = (u16)source->texture->height << 4;
    halfWidth = (f32)(width >> 5) * 0.0625f * source->scaleX;
    halfHeight = (f32)(height >> 5) * 0.0625f * source->scaleY;
    uv.components[0] = 0;
    uv.components[1] = 0;
    uv.components[2] = width;
    uv.components[3] = 0;
    uv.components[4] = width;
    uv.components[5] = height;
    uv.components[6] = 0;
    uv.components[7] = height;
    if (source->angle == 0.0f) {
        corners[0][0] = -halfWidth;
        corners[0][1] = -halfHeight;
        corners[1][0] = halfWidth;
        corners[1][1] = -halfHeight;
        corners[2][0] = halfWidth;
        corners[2][1] = halfHeight;
        corners[3][0] = -halfWidth;
        corners[3][1] = halfHeight;
    } else {
        cosine = sdfCos(source->angle);
        sine = sdfSin(source->angle);
        cornerX = -halfWidth;
        cornerY = -halfHeight;
        corners[0][0] = cornerX * cosine - cornerY * sine;
        corners[0][1] = cornerX * sine + cornerY * cosine;
        cornerX = halfWidth;
        cornerY = -halfHeight;
        corners[1][0] = cornerX * cosine - cornerY * sine;
        corners[1][1] = cornerX * sine + cornerY * cosine;
        cornerX = halfWidth;
        cornerY = halfHeight;
        corners[2][0] = cornerX * cosine - cornerY * sine;
        corners[2][1] = cornerX * sine + cornerY * cosine;
        cornerX = -halfWidth;
        cornerY = halfHeight;
        corners[3][0] = cornerX * cosine - cornerY * sine;
        corners[3][1] = cornerX * sine + cornerY * cosine;
    }

    packet = (u64 *)sdfGrAllocPacketBuff(0x80);
    sdfBillBoardMake1((u8 *)packet, source->position, source->color, &uv, &corners[0][0]);
    sdfDrawTagLink(list, (u32)packet);
}

/* Allocate/init a packet list, generate its texture payload, then append it to the draw surface. */
void effSubmitGeneratedTexturePacket(sdfGrOt_t *surface, EffGeneratedTextureDescriptor *source) {
    sdfDrawTag_t *packetList = (sdfDrawTag_t *)sdfGrAllocPacketBuff(EFF_PACKET_LIST_BYTES);

    sdfDrawTagInit(packetList);
    effDrawGeneratedTextureQuad(packetList, source);
    surface->regFunc(surface, packetList);
}

INCLUDE_ASM(const s32, "code_001DED50", func_001DF038);

/* Allocate/init a packet list, generate its composite GS payload, then append it to the draw surface. */
void effSubmitCompositeGsPacket(sdfGrOt_t *surface, EffCompositeGsDescriptor *source) {
    sdfDrawTag_t *packetList = (sdfDrawTag_t *)sdfGrAllocPacketBuff(EFF_PACKET_LIST_BYTES);

    sdfDrawTagInit(packetList);
    effDrawCompositeTextureQuad(packetList, source);
    surface->regFunc(surface, packetList);
}
