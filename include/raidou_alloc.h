#ifndef RAIDOU_ALLOC_H
#define RAIDOU_ALLOC_H

/* Structs inferred from allocation sites. The size is the allocation's; unkXX members are offsets
 * the allocator and its users access in both games, padXX bytes are never accessed. */

#include "common.h"

/* Allocated by dds3EffectFileListCreate. */
typedef struct Dds3EffectFileListCreateWork {
    s32 unk00; /* 0x00 */
    u8 pad04[48]; /* 0x04 */
    s32 unk34; /* 0x34 */
    s32 unk38; /* 0x38 */
} Dds3EffectFileListCreateWork;
SIZE_CHECK(Dds3EffectFileListCreateWorkSizeCheck, Dds3EffectFileListCreateWork, 0x3C);

/* Allocated by evtAddFrameInfo. */
typedef struct evtFrameInfoWork40_t {
    s32 unk00; /* 0x00 */
    s32 unk04; /* 0x04 */
    s32 unk08; /* 0x08 */
    s32 unk0C; /* 0x0C */
    s32 unk10; /* 0x10 */
    s32 unk14; /* 0x14 */
    u8 pad18[40]; /* 0x18 */
} evtFrameInfoWork40_t;
SIZE_CHECK(evtFrameInfoWork40SizeCheck, evtFrameInfoWork40_t, 0x40);

/* Allocated by evtAddFrameInfo. */
typedef struct evtFrameInfoWork_t {
    f32 unk00; /* 0x00 */
    u8 pad04[12]; /* 0x04 */
    f32 unk10; /* 0x10 */
    u8 pad14[12]; /* 0x14 */
    f32 unk20; /* 0x20 */
    u8 pad24[12]; /* 0x24 */
    f32 unk30; /* 0x30 */
    u8 pad34[12]; /* 0x34 */
    f32 unk40; /* 0x40 */
    u8 pad44[12]; /* 0x44 */
    f32 unk50; /* 0x50 */
    u8 pad54[12]; /* 0x54 */
    f32 unk60; /* 0x60 */
    u8 pad64[12]; /* 0x64 */
} evtFrameInfoWork_t;
SIZE_CHECK(evtFrameInfoWorkSizeCheck, evtFrameInfoWork_t, 0x70);

/* Allocated by dds3ObjectBaseInitControlWorkData. */
typedef struct Dds3ObjectBaseInitControlWork {
    u8 pad00[4]; /* 0x00 */
    s32 unk04; /* 0x04 */
    s32 unk08; /* 0x08 */
    u8 pad0C[4]; /* 0x0C */
    s32 unk10; /* 0x10 */
    u8 pad14[144]; /* 0x14 */
    s32 unkA4; /* 0xA4 */
    u8 padA8[4]; /* 0xA8 */
    s32 unkAC; /* 0xAC */
} Dds3ObjectBaseInitControlWork;
SIZE_CHECK(Dds3ObjectBaseInitControlWorkSizeCheck, Dds3ObjectBaseInitControlWork, 0xB0);

/* Allocated by effAllocateActiveEffectSlotNode. */
typedef struct effActiveEffectSlotNode_t {
    s32 unk00; /* 0x00 */
    s32 unk04; /* 0x04 */
    f32 unk08; /* 0x08 */
    u8 pad0C[84]; /* 0x0C */
    s32 unk60; /* 0x60 */
    s32 unk64; /* 0x64 */
    s32 unk68; /* 0x68 */
    s32 unk6C; /* 0x6C */
    s32 unk70; /* 0x70 */
    u8 pad74[8]; /* 0x74 */
    s16 unk7C; /* 0x7C */
    s16 unk7E; /* 0x7E */
} effActiveEffectSlotNode_t;
SIZE_CHECK(effActiveEffectSlotNodeSizeCheck, effActiveEffectSlotNode_t, 0x80);

/* Allocated by effDuplicateSmallHeader. */
typedef struct effSmallHeader_t {
    s32 unk00; /* 0x00 */
    u8 unk04[8]; /* 0x04 */
    s32 unk0C; /* 0x0C */
} effSmallHeader_t;
SIZE_CHECK(effSmallHeaderSizeCheck, effSmallHeader_t, 0x10);

/* Allocated by evtCreateUnitHandle. */
typedef struct evtUnitHandleAlloc_t {
    s32 unk00; /* 0x00 */
    u8 pad04[8]; /* 0x04 */
    s32 unk0C; /* 0x0C */
    f32 unk10[4]; /* 0x10 */
    u8 pad20[32]; /* 0x20 */
    f32 unk40[4]; /* 0x40 */
    s32 unk50; /* 0x50 */
    u8 pad54[8]; /* 0x54 */
    s32 unk5C; /* 0x5C */
    f32 unk60[4]; /* 0x60 */
    u8 pad70[32]; /* 0x70 */
    f32 unk90[4]; /* 0x90 */
    s32 unkA0; /* 0xA0 */
    u8 padA4[8]; /* 0xA4 */
    s32 unkAC; /* 0xAC */
    f32 unkB0[4]; /* 0xB0 */
    u8 padC0[4]; /* 0xC0 */
    s32 unkC4; /* 0xC4 */
    u8 padC8[24]; /* 0xC8 */
    f32 unkE0[4]; /* 0xE0 */
    s32 unkF0; /* 0xF0 */
    u8 padF4[8]; /* 0xF4 */
    s32 unkFC; /* 0xFC */
    u8 pad100[8]; /* 0x100 */
    s32 unk108; /* 0x108 */
    s32 unk10C; /* 0x10C */
    u8 pad110[16]; /* 0x110 */
    s32 unk120; /* 0x120 */
    u8 pad124[8]; /* 0x124 */
    s32 unk12C; /* 0x12C */
    u8 pad130[28]; /* 0x130 */
    s16 unk14C; /* 0x14C */
    s16 unk14E; /* 0x14E */
    u8 pad150[8]; /* 0x150 */
    f32 unk158; /* 0x158 */
    u8 pad15C[2]; /* 0x15C */
    s16 unk15E; /* 0x15E */
    s16 unk160; /* 0x160 */
    u8 pad162[4]; /* 0x162 */
    s16 unk166; /* 0x166 */
    s16 unk168; /* 0x168 */
    u8 pad16A[14]; /* 0x16A */
    s32 unk178; /* 0x178 */
    s32 unk17C; /* 0x17C */
    u8 pad180[134]; /* 0x180 */
    s16 unk206; /* 0x206 */
    u8 pad208[40]; /* 0x208 */
} evtUnitHandleAlloc_t;
SIZE_CHECK(evtUnitHandleAllocSizeCheck, evtUnitHandleAlloc_t, 0x230);

/* Allocated by sub_2019F8 and 3 more with this layout. */
typedef struct effWork2019F8_t {
    u8 pad00[16]; /* 0x00 */
    u32 unk10[4]; /* 0x10 */
    u32 unk20[4]; /* 0x20 */
    u32 unk30[4]; /* 0x30 */
    u32 unk40[4]; /* 0x40 */
    s32 unk50; /* 0x50 */
    f32 unk54; /* 0x54 */
    f32 unk58; /* 0x58 */
    s32 unk5C; /* 0x5C */
    s32 unk60; /* 0x60 */
    s32 unk64; /* 0x64 */
} effWork2019F8_t;
SIZE_CHECK(effWork2019F8SizeCheck, effWork2019F8_t, 0x68);

/* Allocated by sub_202038 and 5 more with this layout. */
typedef struct effWork202038_t {
    u8 pad00[24]; /* 0x00 */
    s32 unk18; /* 0x18 */
    f32 unk1C; /* 0x1C */
    s32 unk20; /* 0x20 */
} effWork202038_t;
SIZE_CHECK(effWork202038SizeCheck, effWork202038_t, 0x24);

/* Allocated by sub_202098 and 5 more with this layout. */
typedef struct effWork202098_t {
    u8 pad00[24]; /* 0x00 */
    f32 unk18; /* 0x18 */
    f32 unk1C; /* 0x1C */
    s32 unk20; /* 0x20 */
} effWork202098_t;
SIZE_CHECK(effWork202098SizeCheck, effWork202098_t, 0x24);

/* Allocated by sub_2258D0. */
typedef struct effWork2258D0_t {
    f32 unk00; /* 0x00 */
    u8 unk04; /* 0x04 */
    u8 pad05[3]; /* 0x05 */
    s32 unk08; /* 0x08 */
    s32 unk0C; /* 0x0C */
    u8 pad10[4]; /* 0x10 */
} effWork2258D0_t;
SIZE_CHECK(effWork2258D0SizeCheck, effWork2258D0_t, 0x14);

/* Allocated by sub_232638. */
typedef struct effWork232638_t {
    f32 unk00[4]; /* 0x00 */
    f32 unk10[4]; /* 0x10 */
    f32 unk20; /* 0x20 */
    s32 unk24; /* 0x24 */
    s32 unk28; /* 0x28 */
    u8 pad2C[4]; /* 0x2C */
    u8 unk30; /* 0x30 */
    u8 pad31[39]; /* 0x31 */
    s16 unk58; /* 0x58 */
    u8 pad5A[6]; /* 0x5A */
    f32 unk60; /* 0x60 */
    u8 unk64; /* 0x64 */
    u8 pad65[87]; /* 0x65 */
    s32 unkBC; /* 0xBC */
    s32 unkC0; /* 0xC0 */
    u8 padC4[12]; /* 0xC4 */
    s32 unkD0; /* 0xD0 */
    s32 unkD4; /* 0xD4 */
    s32 unkD8; /* 0xD8 */
    u8 padDC[8]; /* 0xDC */
    s32 unkE4; /* 0xE4 */
} effWork232638_t;
SIZE_CHECK(effWork232638SizeCheck, effWork232638_t, 0xE8);

/* Allocated by sub_252058. */
typedef struct effRaidouWork252058_t {
    s32 unk00; /* 0x00 */
    s32 unk04; /* 0x04 */
    s32 unk08; /* 0x08 */
    s32 unk0C; /* 0x0C */
} effRaidouWork252058_t;
SIZE_CHECK(effRaidouWork252058SizeCheck, effRaidouWork252058_t, 0x10);

#endif
