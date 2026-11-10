#include "common.h"

/* Declarations from smt3-decomp src/smt3/sdf/sdfVu1.c */
#include "common.h"

#include "eeregs.h"

#include "ps2asm.h"

#include "sdf_lighting.h"

#include "sdf_resource.h"

/* Register selectors carried by GIF A+D writes in the two GS contexts. */
enum {
    SDF_GS_PRIM = 0x00,
    SDF_GS_RGBAQ = 0x01,
    SDF_GS_XYZ2 = 0x05,
    SDF_GS_TEX0_1 = 0x06,
    SDF_GS_TEX0_2 = 0x07,
    SDF_GS_CLAMP_1 = 0x08,
    SDF_GS_CLAMP_2 = 0x09,
    SDF_GIF_REGISTER_AD = 0x0E,
    SDF_GS_TEX1_1 = 0x14,
    SDF_GS_TEX1_2 = 0x15,
    SDF_GS_XYOFFSET_1 = 0x18,
    SDF_GS_XYOFFSET_2 = 0x19,
    SDF_GS_PRMODECONT = 0x1A,
    SDF_GS_TEXA = 0x3B,
    SDF_GS_TEXFLUSH = 0x3F,
    SDF_GS_SCISSOR_1 = 0x40,
    SDF_GS_SCISSOR_2 = 0x41,
    SDF_GS_ALPHA_1 = 0x42,
    SDF_GS_ALPHA_2 = 0x43,
    SDF_GS_DTHE = 0x45,
    SDF_GS_COLCLAMP = 0x46,
    SDF_GS_TEST_1 = 0x47,
    SDF_GS_TEST_2 = 0x48,
    SDF_GS_FRAME_1 = 0x4C,
    SDF_GS_FRAME_2 = 0x4D,
    SDF_GS_ZBUF_1 = 0x4E,
    SDF_GS_ZBUF_2 = 0x4F,
    SDF_GS_BITBLTBUF = 0x50,
    SDF_GS_TRXPOS = 0x51,
    SDF_GS_TRXREG = 0x52,
    SDF_GS_TRXDIR = 0x53
};

struct sdfCorePacketFaceDivParm_t;

struct sdfCorePacketPointBuff_tag;

struct sdfMatPacket_t;

struct sdfTexPacket_t;

struct sdfGsMemSlice_t;

struct sdfTexHandle_t;

struct TMX_HEADER_t;

/* Serialized byte consumed by the GS texture-state packet builder. */
typedef u8 SdfTextureClampMode;

/* A texture coordinate transform in a material packet. sdfMatSetUvwParm fills
 * it either as six floats or, for the identity case, as three quadword halves. */
typedef union {
    f32 m[6];
    u64 q[3];
} sdfMatTexTm_t;

/* VU0 input basis and emitted base/delta color vectors. */
typedef struct sdfCorePacketFaceDivParm_t {
    u8 opaque00[4]; /* 0x00 */
    f32 scaledY; /* 0x04 */
    u8 opaque08[4]; /* 0x08 */
    f32 scale; /* 0x0C */
    f32 colorBase[4]; /* 0x10 */
    f32 colorDelta[4]; /* 0x20 */
} sdfCorePacketFaceDivParm_t;

#include "sdfCorePacketPointBuff_tag.h"

#define SDF_GIF_PRIM_SHIFT 47

#define SDF_VU_TEXTURED_FLAG 0x10

extern addr_t corePktCurrLight;

extern u128 *corePktFaceList[][3];

extern u16 corePktPrimBase;

extern u8 corePktSoftenBias;

extern u16 corePktBackFaceList[];

extern f32 corePktZBias;

extern f32 sdf3DEyeOffset[4];

/* Shared byte-storage view: the local screen structs are not compatible C types. */
extern u8 sdf3DScreenParam[];

extern u8 corePktWorldLightMtxCache[];

/* Native 0xA0-byte draw-entry block allocated by SDF_ASSET_DRAW_ENTRY_BYTES. */
typedef struct sdfMatPacket_t {
    u32 pad00;             /* 0x00 */
    u32 ambient;             /* 0x04 */
    u32 diffuse;             /* 0x08 */
    u32 secondaryColor;    /* 0x0C: packed color applied by the secondary draw state */
    u32 primaryStateWordThird; /* 0x10: copied from the owning asset, or forced to zero. */
    u32 refmapColor;             /* 0x14 */
    u32 pad18;             /* 0x18 */
    f32 constant;             /* 0x1C */
    u32 alphaState;        /* 0x20: packed GS ALPHA_2 state emitted by the VU transfer builder */
    u32 secondaryMode;     /* 0x24: secondary packet mode copied from sdfMat_t */
    f32 y;                 /* 0x28 */
    f32 x;                 /* 0x2C */
    u8 pad30[8];           /* 0x30 */
    u64 tex1Data;          /* 0x38 */
    u64 tex0Data;
    u64 clampData;
    u64 mtexTex1Data;      /* 0x50 */
    u64 mtexTex0Data;
    u64 mtexClampData;
    /* sdfMatSetUvwParm writes one transform per texture: a 2x2 then an offset. */
    sdfMatTexTm_t texTm;   /* 0x68 */
    sdfMatTexTm_t mtexTexTm;
    u32 vifItopCommand;    /* 0x98: VIF ITOP with immediate 0xC */
    u32 vifMscalCommand;   /* 0x9C: VIF MSCAL with microprogram index zero */
} sdfMatPacket_t;

#include "sdfCorePacketCb_t.h"

void corePktEmitTexTris(sdfCorePacketCb_t *work);

void corePktEmitTris(sdfCorePacketCb_t *work);

extern void corePktXfmUV(void *vertices, s32 count, void *uv, void *transform);

extern void corePktApplyFaceDivColor(sdfCorePacketCb_t *cb, void *parameters);

extern void sdfGrClosePacketBuff(addr_t aBuffPtr);

void corePktEmitTexTrisCtx2(sdfCorePacketCb_t *cb, u64 samplingState, u64 textureState,
                          u64 clampState, u64 alphaState, s32 clearPrimitiveFlags);

/* Finish the selected node's transformed rows and emit its textured geometry. */ void corePktEmitMultiTexPass(sdfCorePacketCb_t *work);

/* Per-texture reference count and non-owning clone source (0x8). */
/* Texture buffer GPU command words (0x40); DDS1/2 game/code_002D10B0/00329F60.c. */
typedef struct sdfTexPacket_t {
    s32 gifTagWord; /* Low GIFtag word; NLOOP occupies bits 0-14. */
    u32 gifTagControl; /* High GIFtag control word; one packed register. */
    u64 gifTagRegisters; /* Packed register list: A+D. */
    u64 samplingState; /* GS TEX1 data, including minification/magnification filters. */
    u64 samplingRegister; /* GS TEX1_1/TEX1_2 register selector. */
    u64 textureState; /* GS TEX0 data. */
    u64 textureRegister; /* GS TEX0_1/TEX0_2 register selector. */
    u64 clampState; /* GS CLAMP data. */
    u64 clampRegister; /* GS CLAMP_1/CLAMP_2 register selector. */
} sdfTexPacket_t;

/* Acquire a texture from a serialized texture-file header. */
struct sdfTexHandle_t *sdfTexSet(const TMX_HEADER_t *aTex);

struct sdfTexHandle_t *sdfTexSetDirect(const TMX_HEADER_t *aTex);

/* Read the active packet cursor or restore it at a quadword boundary. */
s32 sdfGrOpenPacketBuff(void);

static inline sdfCorePacketPointBuff_tag *AllocNode(sdfCorePacketCb_t *work) {
    sdfCorePacketPointBuff_tag *node = work->freePointBuff;

    if (--work->freePointCount == 0) {
        work->freePointBuff = work->extPointBuff;
        work->freePointCount = work->extPointCount;
    } else {
        work->freePointBuff = node + 1;
    }
    *work->extPointLink = node;
    work->extPointLink = &node->next;
    return node;
}

extern saddr_t corePktPointBuff;

extern void corePktRelightBackFaces(u32 cb, s32 count);

void corePktSetupPointBuff(sdfCorePacketCb_t *cb, s32 vertCount);

void corePktInitCb(sdfCorePacketCb_t *cb, u16 *packetHdr, u32 attrMask);

void corePktLoadWorldLightMtx(void *vectors);

void corePktMakeFaceDivParm(sdfCorePacketFaceDivParm_t *result, u32 reference, u32 packedColor,
                   u32 firstColor, u32 secondColor,
                   f32 blend, f32 scale, f32 y);

void corePktSetFaceDivParm(sdfCorePacketFaceDivParm_t *out, sdfMatPacket_t *work, u32 packedColor, f32 deltaX, f32 deltaY);

void corePktLerpUVColorChain(sdfCorePacketPointBuff_tag *node);

void corePktSetConstColor(sdfCorePacketCb_t *cb, u32 color);

void corePktMakeEdgePoint(sdfCorePacketPointBuff_tag *out, sdfCorePacketPointBuff_tag *first, sdfCorePacketPointBuff_tag *second);

void corePktClipFace(sdfCorePacketCb_t *cb);

void corePktBuildFaceList(sdfCorePacketCb_t *cb);

void corePktBuildBackFaceList(sdfCorePacketCb_t *cb);

void corePktEmitBase(sdfCorePacketCb_t *cb);

void corePktEmitAllPasses(sdfCorePacketCb_t *cb);

void corePktOpenAndEmit(sdfCorePacketCb_t *cb);

void sdfVu1Init(void);

void corePktSetupPointBuff(sdfCorePacketCb_t *cb, s32 vertCount);

void corePktInitCb(sdfCorePacketCb_t *cb, u16 *packetHdr, u32 attrMask);

/* vu0 routine: rotate the basis at vectors+0x40 into vf24-vf26 and cache the original vectors. */
void corePktLoadWorldLightMtx(void *vectors);

/* vu0 routine: modulate and clamp packed colors against the reference rows. */
void corePktMakeFaceDivParm(sdfCorePacketFaceDivParm_t *result, u32 reference, u32 packedColor,
                   u32 firstColor, u32 secondColor,
                   f32 blend, f32 scale, f32 y);

void corePktSetFaceDivParm(sdfCorePacketFaceDivParm_t *out, sdfMatPacket_t *work, u32 packedColor, f32 deltaX, f32 deltaY);

/* vu0 routine: lerp of the XY pair at +0x30 by the weight at node + 0x40 */
void corePktLerpUVChain(sdfCorePacketPointBuff_tag *node);

/* vu0 routine: lerp of two source rows (+0x20, +0x30) by the weight at node + 0x40 */
void corePktLerpUVColorChain(sdfCorePacketPointBuff_tag *node);

void corePktSetConstColor(sdfCorePacketCb_t *cb, u32 color);

/* vu0 routine: interpolate a clip vertex between two source rows, COP2 macro math only */
void corePktMakeEdgePoint(sdfCorePacketPointBuff_tag *out, sdfCorePacketPointBuff_tag *first, sdfCorePacketPointBuff_tag *second);

/* vu0 routine: native clip facing and ordinary triangle fans. */
void corePktClipFace(sdfCorePacketCb_t *cb);

/* vu0 routine: native clip-plane setup and ordinary triangle fans. */
void corePktBuildFaceList(sdfCorePacketCb_t *cb);

void corePktBuildBackFaceList(sdfCorePacketCb_t *cb);

void corePktEmitTexTris();

void corePktEmitTexTrisCtx2(sdfCorePacketCb_t *cb, u64 samplingState, u64 textureState, u64 clampState, u64 alphaState, s32 clearPrimitiveFlags);

void corePktEmitTris();

void corePktEmitBase(sdfCorePacketCb_t *cb);

void corePktEmitMultiTexPass(sdfCorePacketCb_t *cb);

void corePktEmitAllPasses(sdfCorePacketCb_t *cb);

void corePktOpenAndEmit(sdfCorePacketCb_t *cb);

u64 *corePktClosePacket(sdfCorePacketCb_t *cb);
/* End of declarations from src/smt3/sdf/sdfVu1.c */

void corePktSetupPointBuff(sdfCorePacketCb_t *cb, s32 vertCount) {
    void *ringEnd = (u8 *)cb + 0x78;
    if (vertCount < 0x80) {
        cb->freePointCount = 0x80 - vertCount;
        cb->extPointBuff = EE_ADDR_PTR(corePktPointBuff);
        cb->freePointBuff = EE_ADDR_PTR(vertCount * 96 + SDF_SCRATCHPAD_POINT_BUFFER);
        cb->extPointCount = 0x2000;
        cb->pointBuff = SDF_SCRATCHPAD_POINT_BUFFER;
    } else if (vertCount == 0x80) {
        cb->pointBuff = SDF_SCRATCHPAD_POINT_BUFFER;
        cb->freePointBuff = EE_ADDR_PTR(corePktPointBuff);
        cb->freePointCount = 0x2000;
        cb->extPointBuff = 0;
        cb->extPointCount = 0;
    } else {
        cb->freePointCount = 0x80;
        cb->pointBuff = corePktPointBuff;
        cb->freePointBuff = EE_ADDR_PTR(SDF_SCRATCHPAD_POINT_BUFFER);
        cb->extPointBuff = EE_ADDR_PTR(corePktPointBuff + vertCount * 96);
        cb->extPointCount = 0x2000 - vertCount;
    }
    cb->extPointLink = ringEnd;
}

void corePktInitCb(sdfCorePacketCb_t *cb, u16 *packetHdr, u32 attrMask) {
    u8 *payload = (u8 *)(packetHdr + 4);
    u16 ringPosition;
    u16 parameterFlags;
    u16 nextParameter;
    u16 selectedFlags;
    cb->faceCount = packetHdr[0];
    ringPosition = packetHdr[1];
    cb->pointCount = ringPosition;
    parameterFlags = packetHdr[2];
    nextParameter = packetHdr[3];
    selectedFlags = parameterFlags & attrMask;
    cb->orgAttrBits = parameterFlags;
    cb->packetBits = nextParameter;
    cb->attrBits = selectedFlags;
    corePktPrimBase = selectedFlags & 0x78;
    cb->dataPtr = payload;
    cb->packetOpenPtr = 0;
    corePktSetupPointBuff(cb, ringPosition);
}

/* vu0 routine: rotate the basis at vectors+0x40 into vf24-vf26 and cache the original vectors. */
void corePktLoadWorldLightMtx(void *vectors) {
    void *m = EE_ADDR_AS(void *, corePktCurrLight);
    VU0_ROTATE_BASIS_AND_CACHE(vectors, m, corePktWorldLightMtxCache);
}

/* vu0 routine: modulate and clamp packed colors against the reference rows. */
void corePktMakeFaceDivParm(sdfCorePacketFaceDivParm_t *result, u32 reference, u32 packedColor,
                   u32 firstColor, u32 secondColor,
                   f32 blend, f32 scale, f32 y) {
    f32 scaledY;

    if (scale < 1.0f) {
        scale = 1.0f;
    }
    scaledY = y * scale;
    result->scale = scale;
    result->scaledY = scaledY;
    VU0_BUILD_PACKED_COLOR_TRANSFORM(result, reference, packedColor,
                                     firstColor, secondColor, blend);
}

void corePktSetFaceDivParm(sdfCorePacketFaceDivParm_t *out, sdfMatPacket_t *work, u32 packedColor, f32 deltaX, f32 deltaY) {
    corePktMakeFaceDivParm(out, corePktCurrLight, packedColor,
                  work->diffuse, work->ambient,
                  work->constant,
                  work->x + deltaX,
                  work->y + deltaY);
}

INCLUDE_ASM(const s32, "sdf/sdfVu1", corePktSkinPointsFirst);

INCLUDE_ASM(const s32, "sdf/sdfVu1", corePktSkinPointsAccum);

INCLUDE_ASM(const s32, "sdf/sdfVu1", corePktSkinVertFirst);

INCLUDE_ASM(const s32, "sdf/sdfVu1", corePktSkinVertAccum);

INCLUDE_ASM(const s32, "sdf/sdfVu1", corePktLoadVerts);

INCLUDE_ASM(const s32, "sdf/sdfVu1", corePktLoadVertsNorms);

INCLUDE_ASM(const s32, "sdf/sdfVu1", corePktMorphVertsNorms);

INCLUDE_ASM(const s32, "sdf/sdfVu1", corePktMorphVerts);

INCLUDE_ASM(const s32, "sdf/sdfVu1", corePktNormalizeNorms);

INCLUDE_ASM(const s32, "sdf/sdfVu1", corePktXfmLightClip);

INCLUDE_ASM(const s32, "sdf/sdfVu1", corePktXfmLightClipPB);

INCLUDE_ASM(const s32, "sdf/sdfVu1", corePktXfmLightClipUV);

INCLUDE_ASM(const s32, "sdf/sdfVu1", corePktXfmLightClipUVPB);

INCLUDE_ASM(const s32, "sdf/sdfVu1", corePktRelightBackFaces);

INCLUDE_ASM(const s32, "sdf/sdfVu1", corePktXfmUV);

/* vu0 routine: lerp of the XY pair at +0x30 by the weight at node + 0x40 */
void corePktLerpUVChain(sdfCorePacketPointBuff_tag *node) {
    while (node != NULL) {
        void *firstSourceRow = node->sourceA;
        void *secondSourceRow = node->sourceB;
        VU0_BLEND_NODE_XY(node, firstSourceRow, secondSourceRow);
        node = node->next;
    }
}

/* vu0 routine: lerp of two source rows (+0x20, +0x30) by the weight at node + 0x40 */
void corePktLerpUVColorChain(sdfCorePacketPointBuff_tag *node) {
    while (node != NULL) {
        void *firstSourceRow = node->sourceA;
        void *secondSourceRow = node->sourceB;
        VU0_BLEND_NODE_VECTORS(node, firstSourceRow, secondSourceRow);
        node = node->next;
    }
}

INCLUDE_ASM(const s32, "sdf/sdfVu1", corePktApplyFaceDivColor);

void corePktSetConstColor(sdfCorePacketCb_t *cb, u32 color) {
    f32 colorScale = 1.0f / 128.0f;
    sdfCorePacketPointBuff_tag *node;
    s32 remaining;
    u128 scaledColor;

    EE_MMI_RGBA_MULTIPLY_VECTOR(scaledColor, cb->color, color, colorScale);

    remaining = cb->pointCount;
    node = EE_ADDR_AS(sdfCorePacketPointBuff_tag *, cb->pointBuff);
    if (remaining != 0) {
        do {
            remaining--;
            *(u128 *)node->color = scaledColor;
            node++;
        } while (remaining != 0);
    }

    node = cb->extPointTop;
    while (node != NULL) {
        sdfCorePacketPointBuff_tag *source = (sdfCorePacketPointBuff_tag *)node->sourceA;
        SDF_VU0_COPYVECTOR_MEMOPS(node->color, source->color);
        node = node->next;
    }
}

/* vu0 routine: interpolate a clip vertex between two source rows, COP2 macro math only */
void corePktMakeEdgePoint(sdfCorePacketPointBuff_tag *out, sdfCorePacketPointBuff_tag *first, sdfCorePacketPointBuff_tag *second) {
    out->sourceA = first;
    out->sourceB = second;
    VU0_INTERPOLATE_CLIP_VERTEX(out, first, second);
}

/* vu0 routine: native clip facing and ordinary triangle fans. */
void corePktClipFace(sdfCorePacketCb_t *cb) {
    s32 triangleCount;
    s32 i;
    u16 *triangles;
    sdfCorePacketPointBuff_tag *nodes;
    s32 rejected;
    s32 acceptAll;
    u128 *(*out)[3];
    f32 scale;

    out = corePktFaceList;
    triangleCount = (s16)cb->faceCount;
    rejected = 0;
    scale = cb->backfaceSign;
    acceptAll = corePktSoftenBias;
    triangles = (u16 *)cb->facePtr;
    nodes = EE_ADDR_AS(sdfCorePacketPointBuff_tag *, cb->pointBuff);
    for (i = 0; i != triangleCount; i++) {
        u16 *triangle = triangles;
        sdfCorePacketPointBuff_tag *a = nodes + triangle[0];
        sdfCorePacketPointBuff_tag *b = nodes + triangle[1];
        sdfCorePacketPointBuff_tag *c = nodes + triangle[2];
        u32 bRegion;
        u32 aRegion;
        u32 cRegion;
        sdfCorePacketPointBuff_tag *d, *e, *f, *g;
        f32 facing;
        u32 clipA = a->clipFlags;
        u32 clipB = b->clipFlags;
        u32 clipC = c->clipFlags;

        triangles += 3;
        if ((clipA & clipB & clipC & 0x3F) != 0 ||
            ((clipA | clipB | clipC) & 0x30) != 0) {
            continue;
        }
        VU0_TRIANGLE_FACING(facing, a->position, b->position, c->position);
        facing *= scale;
        if (facing < 0.0f) {
            corePktBackFaceList[rejected++] = i;
            continue;
        }
        aRegion = a->region;
        bRegion = b->region;
        cRegion = c->region;

        if (acceptAll || (aRegion == bRegion && aRegion == cRegion)) {
            out[0][0] = (u128 *)a;
            out[0][1] = (u128 *)b;
            out[0][2] = (u128 *)c;
            out++;
            continue;
        }
        for (;;) {
            sdfCorePacketPointBuff_tag *firstVertex;
            u32 firstCode;

            if (bRegion == cRegion) {
                d = AllocNode(cb);
                e = AllocNode(cb);
                if (aRegion == 0) {
                    corePktMakeEdgePoint(d, b, a);
                    corePktMakeEdgePoint(e, c, a);
                    out[0][0] = (u128 *)a;
                    out[0][1] = (u128 *)d;
                    out[0][2] = (u128 *)e;
                    out[1][0] = (u128 *)e;
                    out[1][1] = (u128 *)d;
                    out[1][2] = (u128 *)b;
                    out[2][0] = (u128 *)e;
                    out[2][1] = (u128 *)b;
                    out[2][2] = (u128 *)c;
                    out += 3;
                    break;
                }
                corePktMakeEdgePoint(d, a, b);
                corePktMakeEdgePoint(e, a, c);
                out[0][0] = (u128 *)a;
                out[0][1] = (u128 *)d;
                out[0][2] = (u128 *)e;
                out++;
                if (bRegion == 0) {
                    out[0][0] = (u128 *)b;
                    out[0][1] = (u128 *)e;
                    out[0][2] = (u128 *)d;
                    out[1][0] = (u128 *)b;
                    out[1][1] = (u128 *)c;
                    out[1][2] = (u128 *)e;
                    out += 2;
                } else {
                    f = AllocNode(cb);
                    g = AllocNode(cb);
                    corePktMakeEdgePoint(f, b, a);
                    corePktMakeEdgePoint(g, c, a);
                    out[0][0] = (u128 *)e;
                    out[0][1] = (u128 *)d;
                    out[0][2] = (u128 *)f;
                    out[1][0] = (u128 *)e;
                    out[1][1] = (u128 *)f;
                    out[1][2] = (u128 *)g;
                    out[2][0] = (u128 *)g;
                    out[2][1] = (u128 *)f;
                    out[2][2] = (u128 *)b;
                    out[3][0] = (u128 *)g;
                    out[3][1] = (u128 *)b;
                    out[3][2] = (u128 *)c;
                    out += 4;
                }
                break;
            }
            if (aRegion == 0 && (bRegion ^ cRegion) == 3) {
                d = AllocNode(cb);
                e = AllocNode(cb);
                f = AllocNode(cb);
                g = AllocNode(cb);
                corePktMakeEdgePoint(d, b, a);
                corePktMakeEdgePoint(e, c, a);
                corePktMakeEdgePoint(f, b, c);
                corePktMakeEdgePoint(g, c, b);
                out[0][0] = (u128 *)a;
                out[0][1] = (u128 *)d;
                out[0][2] = (u128 *)e;
                out[1][0] = (u128 *)d;
                out[1][1] = (u128 *)b;
                out[1][2] = (u128 *)f;
                out[2][0] = (u128 *)e;
                out[2][1] = (u128 *)d;
                out[2][2] = (u128 *)f;
                out[3][0] = (u128 *)e;
                out[3][1] = (u128 *)f;
                out[3][2] = (u128 *)g;
                out[4][0] = (u128 *)e;
                out[4][1] = (u128 *)g;
                out[4][2] = (u128 *)c;
                out += 5;
                break;
            }
            firstVertex = a;
            firstCode = aRegion;
            a = b;
            aRegion = bRegion;
            b = c;
            bRegion = cRegion;
            c = firstVertex;
            cRegion = firstCode;
        }
    }
    cb->backfaceCount = rejected;
    cb->drawFaceCount = out - corePktFaceList;
    *cb->extPointLink = NULL;
}

/* vu0 routine: native clip-plane setup and ordinary triangle fans. */
void corePktBuildFaceList(sdfCorePacketCb_t *cb) {
    s32 triangleCount;
    u16 *triangles;
    sdfCorePacketPointBuff_tag *nodes;
    s32 acceptAll;
    u128 *(*out)[3];
    f32 scale = cb->backfaceSign;
    f32 clipBias = corePktZBias;
    const f32 *clipPlane = sdf3DEyeOffset;
    const u8 *projection = sdf3DScreenParam;

    VU0_SET_CLIP_PLANES(clipBias, clipPlane, projection);
    if (cb->attrBits & 0x4000) {
        corePktClipFace(cb);
    }
    triangleCount = (s16)cb->faceCount;
    acceptAll = corePktSoftenBias;
    out = corePktFaceList;
    triangles = (u16 *)cb->facePtr;
    nodes = EE_ADDR_AS(sdfCorePacketPointBuff_tag *, cb->pointBuff);
    for (; triangleCount != 0; triangleCount--) {
        u16 *triangle = triangles;
        sdfCorePacketPointBuff_tag *a = nodes + triangle[0];
        sdfCorePacketPointBuff_tag *b = nodes + triangle[1];
        sdfCorePacketPointBuff_tag *c = nodes + triangle[2];
        u32 bRegion;
        u32 aRegion;
        u32 cRegion;
        sdfCorePacketPointBuff_tag *d, *e, *f, *g;
        f32 facing;
        u32 clipA = a->clipFlags;
        u32 clipB = b->clipFlags;
        u32 clipC = c->clipFlags;

        triangles += 3;
        if ((clipA & clipB & clipC & 0x3F) != 0 ||
            ((clipA | clipB | clipC) & 0x30) != 0) {
            continue;
        }
        VU0_TRIANGLE_FACING(facing, a->position, b->position, c->position);
        facing *= scale;
        if (facing < 0.0f) {
            continue;
        }
        aRegion = a->region;
        bRegion = b->region;
        cRegion = c->region;

        if (acceptAll || (aRegion == bRegion && aRegion == cRegion)) {
            out[0][0] = (u128 *)a;
            out[0][1] = (u128 *)b;
            out[0][2] = (u128 *)c;
            out++;
            continue;
        }
        for (;;) {
            sdfCorePacketPointBuff_tag *firstVertex;
            u32 firstCode;

            if (bRegion == cRegion) {
                d = AllocNode(cb);
                e = AllocNode(cb);
                if (aRegion == 0) {
                    corePktMakeEdgePoint(d, b, a);
                    corePktMakeEdgePoint(e, c, a);
                    out[0][0] = (u128 *)a;
                    out[0][1] = (u128 *)d;
                    out[0][2] = (u128 *)e;
                    out[1][0] = (u128 *)e;
                    out[1][1] = (u128 *)d;
                    out[1][2] = (u128 *)b;
                    out[2][0] = (u128 *)e;
                    out[2][1] = (u128 *)b;
                    out[2][2] = (u128 *)c;
                    out += 3;
                    break;
                }
                corePktMakeEdgePoint(d, a, b);
                corePktMakeEdgePoint(e, a, c);
                out[0][0] = (u128 *)a;
                out[0][1] = (u128 *)d;
                out[0][2] = (u128 *)e;
                out++;
                if (bRegion == 0) {
                    out[0][0] = (u128 *)b;
                    out[0][1] = (u128 *)e;
                    out[0][2] = (u128 *)d;
                    out[1][0] = (u128 *)b;
                    out[1][1] = (u128 *)c;
                    out[1][2] = (u128 *)e;
                    out += 2;
                } else {
                    f = AllocNode(cb);
                    g = AllocNode(cb);
                    corePktMakeEdgePoint(f, b, a);
                    corePktMakeEdgePoint(g, c, a);
                    out[0][0] = (u128 *)e;
                    out[0][1] = (u128 *)d;
                    out[0][2] = (u128 *)f;
                    out[1][0] = (u128 *)e;
                    out[1][1] = (u128 *)f;
                    out[1][2] = (u128 *)g;
                    out[2][0] = (u128 *)g;
                    out[2][1] = (u128 *)f;
                    out[2][2] = (u128 *)b;
                    out[3][0] = (u128 *)g;
                    out[3][1] = (u128 *)b;
                    out[3][2] = (u128 *)c;
                    out += 4;
                }
                break;
            }
            if (aRegion == 0 && (bRegion ^ cRegion) == 3) {
                d = AllocNode(cb);
                e = AllocNode(cb);
                f = AllocNode(cb);
                g = AllocNode(cb);
                corePktMakeEdgePoint(d, b, a);
                corePktMakeEdgePoint(e, c, a);
                corePktMakeEdgePoint(f, b, c);
                corePktMakeEdgePoint(g, c, b);
                out[0][0] = (u128 *)a;
                out[0][1] = (u128 *)d;
                out[0][2] = (u128 *)e;
                out[1][0] = (u128 *)d;
                out[1][1] = (u128 *)b;
                out[1][2] = (u128 *)f;
                out[2][0] = (u128 *)e;
                out[2][1] = (u128 *)d;
                out[2][2] = (u128 *)f;
                out[3][0] = (u128 *)e;
                out[3][1] = (u128 *)f;
                out[3][2] = (u128 *)g;
                out[4][0] = (u128 *)e;
                out[4][1] = (u128 *)g;
                out[4][2] = (u128 *)c;
                out += 5;
                break;
            }
            firstVertex = a;
            firstCode = aRegion;
            a = b;
            aRegion = bRegion;
            b = c;
            bRegion = cRegion;
            c = firstVertex;
            cRegion = firstCode;
        }
    }
    cb->drawFaceCount = out - corePktFaceList;
    *cb->extPointLink = NULL;
}

void corePktBuildBackFaceList(sdfCorePacketCb_t *cb) {
    /* DDS1 node region uses the tail byte at +0x50. */
    f32 scale = corePktZBias;
    s32 remaining;
    u16 *triangles;
    u16 *indices;
    sdfCorePacketPointBuff_tag *nodes;
    s32 acceptAll;
    u128 *(*out)[3];
    const f32 *clipPlane = sdf3DEyeOffset;
    const u8 *projection = sdf3DScreenParam;

    VU0_SET_CLIP_PLANES(scale, clipPlane, projection);
    acceptAll = corePktSoftenBias;
    remaining = cb->backfaceCount;
    indices = corePktBackFaceList;
    triangles = (u16 *)cb->facePtr;
    nodes = EE_ADDR_AS(sdfCorePacketPointBuff_tag *, cb->pointBuff);
    out = (u128 *(*)[3])corePktFaceList;
    if (remaining != 0) {
        do {
            u16 *triangle = triangles + *indices++ * 3;
            sdfCorePacketPointBuff_tag *a = nodes + triangle[0];
            sdfCorePacketPointBuff_tag *b = nodes + triangle[1];
            sdfCorePacketPointBuff_tag *c = nodes + triangle[2];
            u32 bRegion = b->region;
            u32 aRegion = a->region;
            u32 cRegion = c->region;
            sdfCorePacketPointBuff_tag *d, *e, *f, *g;
            if (!acceptAll && (aRegion != bRegion || aRegion != cRegion)) {
                goto classifyRegions;
            }
        out[0][0] = (u128 *)a;
        out[0][1] = (u128 *)b;
        out[0][2] = (u128 *)c;
        out += 1;
            goto advanceTriangle;
        appendInsidePair:
        out[0][0] = (u128 *)b;
        out[0][1] = (u128 *)e;
        out[0][2] = (u128 *)d;
        out[1][0] = (u128 *)b;
        out[1][1] = (u128 *)c;
        out[1][2] = (u128 *)e;
        out += 2;
            goto advanceTriangle;
        interpolateOutsideA:
        corePktMakeEdgePoint(d, a, b);
        corePktMakeEdgePoint(e, a, c);
        out[0][0] = (u128 *)a;
        out[0][1] = (u128 *)d;
        out[0][2] = (u128 *)e;
        out += 1;
            if (bRegion != 0) {
            f = AllocNode(cb);
            g = AllocNode(cb);
            corePktMakeEdgePoint(f, b, a);
            corePktMakeEdgePoint(g, c, a);
            out[0][0] = (u128 *)e;
            out[0][1] = (u128 *)d;
            out[0][2] = (u128 *)f;
            out[1][0] = (u128 *)e;
            out[1][1] = (u128 *)f;
            out[1][2] = (u128 *)g;
            out[2][0] = (u128 *)g;
            out[2][1] = (u128 *)f;
            out[2][2] = (u128 *)b;
            out[3][0] = (u128 *)g;
            out[3][1] = (u128 *)b;
            out[3][2] = (u128 *)c;
            out += 4;

            } else goto appendInsidePair;
            goto advanceTriangle;
        threeDifferentRegions: {
            if (aRegion != 0 || (bRegion ^ cRegion) != 3) goto rotateTriangle;
        d = AllocNode(cb);
        e = AllocNode(cb);
        f = AllocNode(cb);
        g = AllocNode(cb);
        corePktMakeEdgePoint(d, b, a);
        corePktMakeEdgePoint(e, c, a);
        corePktMakeEdgePoint(f, b, c);
        corePktMakeEdgePoint(g, c, b);
        out[0][0] = (u128 *)a;
        out[0][1] = (u128 *)d;
        out[0][2] = (u128 *)e;
        out[1][0] = (u128 *)d;
        out[1][1] = (u128 *)b;
        out[1][2] = (u128 *)f;
        out[2][0] = (u128 *)e;
        out[2][1] = (u128 *)d;
        out[2][2] = (u128 *)f;
        out[3][0] = (u128 *)e;
        out[3][1] = (u128 *)f;
        out[3][2] = (u128 *)g;
        out[4][0] = (u128 *)e;
        out[4][1] = (u128 *)g;
        out[4][2] = (u128 *)c;
        out += 5;
            goto advanceTriangle;
        }
        rotateTriangle: {
            sdfCorePacketPointBuff_tag *oldA = a;
            u32 oldRegion = aRegion;
            a = b; b = c; c = oldA;
            aRegion = bRegion; bRegion = cRegion; cRegion = oldRegion;
        }
        classifyRegions:
            if (bRegion != cRegion) goto threeDifferentRegions;
        d = AllocNode(cb);
        e = AllocNode(cb);
            if (aRegion != 0) goto interpolateOutsideA;
        corePktMakeEdgePoint(d, b, a);
        corePktMakeEdgePoint(e, c, a);
        out[0][0] = (u128 *)a;
        out[0][1] = (u128 *)d;
        out[0][2] = (u128 *)e;
        out[1][0] = (u128 *)e;
        out[1][1] = (u128 *)d;
        out[1][2] = (u128 *)b;
        out[2][0] = (u128 *)e;
        out[2][1] = (u128 *)b;
        out[2][2] = (u128 *)c;
        out += 3;
        advanceTriangle:
            ;
        } while (--remaining != 0);
    }
    cb->drawFaceCount = out - (u128 *(*)[3])corePktFaceList;
    *(sdfCorePacketPointBuff_tag **)cb->extPointLink = NULL;
}

INCLUDE_ASM(const s32, "sdf/sdfVu1", corePktEmitTexTris);

void corePktEmitTexTrisCtx2(sdfCorePacketCb_t *cb, u64 samplingState, u64 textureState, u64 clampState, u64 alphaState, s32 clearPrimitiveFlags) {
    s32 remainingTriangles = cb->drawFaceCount;
    if (remainingTriangles != 0) {
        u128 *(*triangleRefs)[3] = corePktFaceList;
        u32 *packetCursor = cb->packetPtr;
        s32 firstBatch = 1;
        do {
            s32 batchTriangles = (remainingTriangles <= 0x10) ? remainingTriangles : 0x10;
            remainingTriangles -= batchTriangles;
            while ((EE_ADDR(packetCursor) & 0xC) != 4) {
                *packetCursor++ = 0;
            }
            if (firstBatch) {
                *packetCursor++ = 0x6501C000;
                *packetCursor++ = 6;
                *packetCursor++ = ((batchTriangles * 9 + 7) << 16) | 0x6C00C001;
                firstBatch = 0;
                *(u64 *)packetCursor = 0x1000000000000005ULL;
                packetCursor += 2;
                *(u64 *)packetCursor = SDF_GIF_REGISTER_AD;
                packetCursor += 2;
                *(u64 *)packetCursor = samplingState;
                packetCursor += 2;
                *(u64 *)packetCursor = 0x15;
                packetCursor += 2;
                *(u64 *)packetCursor = textureState;
                packetCursor += 2;
                *(u64 *)packetCursor = 7;
                packetCursor += 2;
                *(u64 *)packetCursor = clampState;
                packetCursor += 2;
                *(u64 *)packetCursor = 9;
                packetCursor += 2;
                *(u64 *)packetCursor = 0x51801;
                packetCursor += 2;
                *(u64 *)packetCursor = 0x48;
                packetCursor += 2;
                *(u64 *)packetCursor = alphaState;
                packetCursor += 2;
                *(u64 *)packetCursor = 0x43;
                packetCursor += 2;
            } else {
                *packetCursor++ = 0x6501C000;
                *packetCursor++ = 0;
                *packetCursor++ = ((batchTriangles * 9 + 1) << 16) | 0x6C00C001;
            }
            *(u64 *)packetCursor = ((u64)((corePktPrimBase & ~clearPrimitiveFlags) | 0x203) << SDF_GIF_PRIM_SHIFT) | (batchTriangles * 3) | 0x3000400000008000ULL;
            packetCursor += 2;
            *(u64 *)packetCursor = 0x412;
            packetCursor += 2;
            do {
                u128 *vertexA = (*triangleRefs)[0];
                u128 *vertexB = (*triangleRefs)[1];
                u128 *vertexC = (*triangleRefs)[2];
                triangleRefs++;
                ((u128 *)packetCursor)[0] = vertexA[0];
                ((u128 *)packetCursor)[1] = vertexA[3];
                ((u128 *)packetCursor)[2] = vertexA[2];
                ((u128 *)packetCursor)[3] = vertexB[0];
                ((u128 *)packetCursor)[4] = vertexB[3];
                ((u128 *)packetCursor)[5] = vertexB[2];
                ((u128 *)packetCursor)[6] = vertexC[0];
                ((u128 *)packetCursor)[7] = vertexC[3];
                ((u128 *)packetCursor)[8] = vertexC[2];
                packetCursor += 0x24;
            } while (--batchTriangles != 0);
            *packetCursor++ = 0x14000004;
        } while (remainingTriangles != 0);
        cb->packetPtr = packetCursor;
    }
}

INCLUDE_ASM(const s32, "sdf/sdfVu1", corePktEmitTris);

void corePktEmitBase(sdfCorePacketCb_t *cb) {
    if ((cb->attrBits & SDF_VU_TEXTURED_FLAG) != 0) {
        corePktEmitTexTris(cb);
        return;
    }
    corePktEmitTris(cb);
}

void corePktEmitMultiTexPass(sdfCorePacketCb_t *cb) {
    sdfMatPacket_t *node;
    u32 mode;
    u32 secondaryColor;
    u32 primaryStateWordSecond;

    corePktXfmUV(EE_ADDR_AS(void *, cb->pointBuff), cb->pointCount, cb->uv2Ptr,
                  (u8 *)cb->matPacketPtr + 0x80);
    corePktLerpUVChain(cb->extPointTop);
    node = cb->matPacketPtr;
    mode = node->secondaryMode;
    secondaryColor = node->secondaryColor;
    primaryStateWordSecond = node->diffuse;
    switch (mode) {
        case 0:
        case 1:
        case 2:
            if (primaryStateWordSecond != secondaryColor) {
                u128 parameters[3];

                corePktMakeFaceDivParm((sdfCorePacketFaceDivParm_t *)parameters, corePktCurrLight, cb->color,
                              secondaryColor, node->ambient, node->constant,
                              node->x + cb->fdSoften, node->y + cb->fdBias);
                corePktApplyFaceDivColor(cb, parameters);
            }
            break;
        case 3:
            corePktSetConstColor(cb, secondaryColor);
            break;
    }
    corePktEmitTexTrisCtx2(cb, node->mtexTex1Data,
                                  node->mtexTex0Data,
                                  node->mtexClampData, node->alphaState, 0);
}

INCLUDE_ASM(const s32, "sdf/sdfVu1", corePktEmitRefMapPass);

void corePktEmitAllPasses(sdfCorePacketCb_t *cb) {
    u32 selectedFlags;

    corePktEmitBase(cb);
    selectedFlags = cb->attrBits;
    if ((selectedFlags & 0x1000) != 0) {
        corePktEmitMultiTexPass(cb);
        selectedFlags = cb->attrBits;
    }
    if ((selectedFlags & 1) != 0) {
        corePktEmitRefMapPass(cb);
        return;
    }
}

void corePktOpenAndEmit(sdfCorePacketCb_t *cb) {
    u32 packetStart = sdfGrOpenPacketBuff();
    u32 alignedStart = (packetStart + 0x3F) & ~0x3F;
    u32 ringAddress = EE_UNCACHED_RING(EE_PHYSICAL(alignedStart + 0x40));
    cb->packetOpenPtr = packetStart;
    cb->packetTopPtr = alignedStart + 0x30;
    cb->packetStartPtr = EE_ADDR_AS(u32 *, ringAddress);
    cb->packetPtr = EE_ADDR_AS(u32 *, ringAddress);
    if ((cb->attrBits & 0x4000) != 0) {
        u8 savedVectors[0x30];
        VU0_STORE_VF24_VF25_VF26(savedVectors);
        corePktBuildFaceList(cb);
        corePktEmitAllPasses(cb);
        corePktSetupPointBuff(cb, cb->pointCount);
        VU0_LOAD_VF24_VF25_VF26(savedVectors);
        corePktRelightBackFaces(cb->pointBuff, cb->pointCount);
        corePktBuildBackFaceList(cb);
        corePktEmitAllPasses(cb);
    } else {
        corePktBuildFaceList(cb);
        corePktEmitAllPasses(cb);
    }
}

u64 *corePktClosePacket(sdfCorePacketCb_t *cb) {
    addr_t cursor;
    addr_t start;
    s32 quadwords;
    u32 descriptor;
    u64 gifTag;
    u64 *header;

    if (cb->packetOpenPtr == 0) {
        return NULL;
    }

    cursor = EE_ADDR(cb->packetPtr);
    start = EE_ADDR(cb->packetStartPtr);
    if (cursor == start) {
        sdfGrClosePacketBuff(cb->packetOpenPtr);
        return NULL;
    }

    while (((u32)cursor & 0xC) != 0) {
        *EE_ADDR_AS(u32 *, cursor) = 0;
        cursor += 4;
    }
    quadwords = (s32)(cursor - start) >> 4;

    while (((u32)cursor & 0x30) != 0) {
        QUAD_ZERO(*EE_ADDR_AS(u128 *, cursor));
        cursor += 0x10;
    }

    sdfGrClosePacketBuff(EE_PHYSICAL(cursor));
    header = EE_ADDR_AS(u64 *, cb->packetTopPtr);
    descriptor = 0x20000000 | (quadwords & 0xFFFF);
    gifTag = 0x1100000000000000ULL;
    header[0] = descriptor;
    header[1] = gifTag;
    return header;
}

INCLUDE_ASM(const s32, "sdf/sdfVu1", corePktVu0Run);
