/* Declarations of smt3-decomp src/smt3/sdf/sdf_Decomp.c, for its bodies in this directory. */
#ifndef SHARED_SDF_SDF_DECOMP_H
#define SHARED_SDF_SDF_DECOMP_H

#include "common.h"

#include "ps2asm.h"

#include "sdf_types.h"

#include "eeregs.h"

/* Declared here because no header carries these. */
extern void *memset(void *a, s32 b, size_t c);

enum sdfDecomp_enum {
    SDF_DECOMP_STAT_END = 0,
    SDF_DECOMP_STAT_IDLE = 1,
    SDF_DECOMP_STAT_DIREC_LENG0 = 2,
    SDF_DECOMP_STAT_DIREC_LENG1 = 3,
    SDF_DECOMP_STAT_DIREC = 4,
    SDF_DECOMP_STAT_DEC_NORMAL = 5,
    SDF_DECOMP_STAT_DEC_RLE = 6,
    SDF_DECOMP_STAT_DEC_SD_STEP0 = 7,
    SDF_DECOMP_STAT_DEC_SD_STEP1 = 8,
    SDF_DECOMP_STAT_DEC_SD_STEP2 = 9,
    SDF_DECOMP_STAT_DEC_WORD = 10
};

enum sdfDecomp_enum2 {
    SDF_DECOMP_DIRECTIVE_NORMAL = 0x00,
    SDF_DECOMP_DIRECTIVE_ZERO = 0x20,
    SDF_DECOMP_DIRECTIVE_RLE = 0x40,
    SDF_DECOMP_DIRECTIVE_SD = 0x60,
    SDF_DECOMP_DIRECTIVE_SD2 = 0x80,
    SDF_DECOMP_DIRECTIVE_WORD = 0xA0,
    SDF_DECOMP_DIRECTIVE_CMD = 0xE0
};

enum sdfDecomp_enum3 {
    SDF_DECOMP_CMD_END = 0xFF
};

/* No IDB members: the two fields a directive byte splits into. */
enum {
    SDF_DECOMP_DIRECTIVE_MASK = 0xE0,
    SDF_DECOMP_LENGTH_MASK = 0x1F
};

struct sdfDecompCb_t;

/* Load matrix into the VU0 scratch bank and postmultiply the primary bank. */
void MultMatrixRev(const void *aMatrix);

extern const u8 *sdfDecompHistory[16];

/* sdf3DScreenParam + 0x70: the scale and trans rows. */
/* Shared byte-storage view: the local screen structs are not compatible C types. */
extern u8 sdf3DScreenParam[];

extern u8 sdf3DScreenTm[];

/* Incremental decoder access view shared by both providers. The backing
 * decoder block is allocated as 0x20 bytes; only this 0x14-byte prefix is
 * described here, and its trailing allocation bytes remain unknown. */
typedef struct sdfDecompCb_t {
    u8 stat;             /* 0x00 */
    u8 direc;           /* 0x01 */
    u16 direcLeng;     /* 0x02 */
    u8 *dstPtr;           /* 0x04 */
    const u8 *srcPtr;      /* 0x08 */
    s32 srcRemain;       /* 0x0C */
    u8 sdOfs;        /* 0x10 */
    u8 pad11[3];          /* 0x11: opaque */
} sdfDecompCb_t;

/* Read the active packet cursor or restore it at a quadword boundary. */
s32 sdfGrOpenPacketBuff(void);

void sdfGrClosePacketBuff(saddr_t aBuffPtr);

extern s32 sdfDecompHistoryIndex;

extern void *memcpy(void *, const void *, size_t);

s32 sdfDecomp(sdfDecompCb_t *cb, const u8 *src, s32 length);

void sdfDecompInit(sdfDecompCb_t *cb, u8 *dst);

sdfGrPacketHeader_t *sdfDrawModelWithMatrix(const f32 vertices[][4], const u32 *colors, s32 vertexCount, u32 primitiveFlags);

sdfGrPacketHeader_t *effDebugDrawWireModel(const sdfVector4_t *vertices, const u32 *colors, s32 vertexCount, u32 primitiveFlags);

sdfGrPacketHeader_t *sdfDrawRotatedModel(const f32 vertices[][4], const u32 *colors, s32 vertexCount, u32 primitiveFlags);

s32 sdfDecomp(sdfDecompCb_t *cb, const u8 *src, s32 length);

void sdfDecompInit(sdfDecompCb_t *cb, u8 *dst);

sdfGrPacketHeader_t *sdfDrawModelWithMatrix(const f32 vertices[][4], const u32 *colors, s32 vertexCount, u32 primitiveFlags);

sdfGrPacketHeader_t *effDebugDrawWireModel(const sdfVector4_t *vertices, const u32 *colors, s32 vertexCount, u32 primitiveFlags);

sdfGrPacketHeader_t *sdfDrawRotatedModel(const f32 vertices[][4], const u32 *colors, s32 vertexCount, u32 primitiveFlags);

#endif
