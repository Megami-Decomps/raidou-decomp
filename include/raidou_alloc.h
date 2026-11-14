#ifndef RAIDOU_ALLOC_H
#define RAIDOU_ALLOC_H

/* Structs inferred from allocation sites. The size is the allocation's; unkXX members are offsets
 * the allocator and its users access in both games, padXX bytes are never accessed. */

#include "common.h"

/* Allocated by evtAddFrameInfo. */
typedef struct EvtFrameInfoWork20 {
    f32 unk00; /* 0x00 */
    f32 unk04; /* 0x04 */
    f32 unk08; /* 0x08 */
    f32 unk0C; /* 0x0C */
    s32 unk10; /* 0x10 */
    u8 pad14[12]; /* 0x14 */
} EvtFrameInfoWork20;
SIZE_CHECK(EvtFrameInfoWork20SizeCheck, EvtFrameInfoWork20, 0x20);

/* Allocated by dds3EffectFileListCreate. */
typedef struct Dds3EffectFileListCreateWork {
    s32 unk00; /* 0x00 */
    u8 pad04[48]; /* 0x04 */
    s32 unk34; /* 0x34 */
    s32 unk38; /* 0x38 */
} Dds3EffectFileListCreateWork;
SIZE_CHECK(Dds3EffectFileListCreateWorkSizeCheck, Dds3EffectFileListCreateWork, 0x3C);

/* Allocated by evtAddFrameInfo. */
typedef struct EvtFrameInfoWork40 {
    s32 unk00; /* 0x00 */
    s32 unk04; /* 0x04 */
    s32 unk08; /* 0x08 */
    s32 unk0C; /* 0x0C */
    s32 unk10; /* 0x10 */
    s32 unk14; /* 0x14 */
    u8 pad18[40]; /* 0x18 */
} EvtFrameInfoWork40;
SIZE_CHECK(EvtFrameInfoWork40SizeCheck, EvtFrameInfoWork40, 0x40);

/* Allocated by evtAddFrameInfo. */
typedef struct EvtFrameInfoWork {
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
} EvtFrameInfoWork;
SIZE_CHECK(EvtFrameInfoWorkSizeCheck, EvtFrameInfoWork, 0x70);

/* Allocated by dds3InitPoseSet. */
typedef struct Dds3PoseSet {
    u8 pad00[64]; /* 0x00 */
    f32 unk40[4]; /* 0x40 */
    f32 unk50[4]; /* 0x50 */
    u8 pad60[80]; /* 0x60 */
    f32 unkB0[4]; /* 0xB0 */
    s32 unkC0; /* 0xC0 */
    s32 unkC4; /* 0xC4 */
    s32 unkC8; /* 0xC8 */
    u8 padCC[4]; /* 0xCC */
} Dds3PoseSet;
SIZE_CHECK(Dds3PoseSetSizeCheck, Dds3PoseSet, 0xD0);

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
typedef struct EffActiveEffectSlotNode {
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
} EffActiveEffectSlotNode;
SIZE_CHECK(EffActiveEffectSlotNodeSizeCheck, EffActiveEffectSlotNode, 0x80);

/* Allocated by effBTLCreate. */
typedef struct EffBTL {
    u32 unk00[2]; /* 0x00 */
    u32 unk08[2]; /* 0x08 */
    u32 unk10[2]; /* 0x10 */
    u32 unk18[2]; /* 0x18 */
    u8 pad20[84]; /* 0x20 */
    u16 unk74; /* 0x74 */
    u8 pad76[3010]; /* 0x76 */
    s32 unkC38; /* 0xC38 */
    s32 unkC3C; /* 0xC3C */
    s32 unkC40; /* 0xC40 */
} EffBTL;
SIZE_CHECK(EffBTLSizeCheck, EffBTL, 0xC44);

/* Allocated by effDuplicateSmallHeader. */
typedef struct EffSmallHeader {
    s32 unk00; /* 0x00 */
    u8 unk04[8]; /* 0x04 */
    s32 unk0C; /* 0x0C */
} EffSmallHeader;
SIZE_CHECK(EffSmallHeaderSizeCheck, EffSmallHeader, 0x10);

/* Allocated by effPCPBossBaelCreate and 1 more with this layout. */
typedef struct EffPCPBossBael {
    u8 pad00[32]; /* 0x00 */
    s32 unk20; /* 0x20 */
    s32 unk24; /* 0x24 */
    s32 unk28; /* 0x28 */
    s32 unk2C; /* 0x2C */
} EffPCPBossBael;
SIZE_CHECK(EffPCPBossBaelSizeCheck, EffPCPBossBael, 0x30);

/* Allocated by evtAddFrameInfo. */
typedef struct EvtFrameInfo {
    u16 unk00; /* 0x00 */
    s16 unk02; /* 0x02 */
    s32 unk04; /* 0x04 */
    u8 unk08; /* 0x08 */
    u8 unk09; /* 0x09 */
    s16 unk0A; /* 0x0A */
    u8 unk0C; /* 0x0C */
    u8 unk0D; /* 0x0D */
    u8 unk0E; /* 0x0E */
    u8 unk0F; /* 0x0F */
    u8 unk10; /* 0x10 */
    u8 unk11; /* 0x11 */
    u8 unk12; /* 0x12 */
    u8 unk13; /* 0x13 */
    s16 unk14; /* 0x14 */
    s16 unk16; /* 0x16 */
    s16 unk18; /* 0x18 */
    s16 unk1A; /* 0x1A */
    u8 unk1C; /* 0x1C */
    u8 unk1D; /* 0x1D */
    u8 unk1E; /* 0x1E */
    u8 pad1F; /* 0x1F */
    u64 unk20; /* 0x20 */
    s32 unk28; /* 0x28 */
    s32 unk2C; /* 0x2C */
    s32 unk30; /* 0x30 */
    s32 unk34; /* 0x34 */
} EvtFrameInfo;
SIZE_CHECK(EvtFrameInfoSizeCheck, EvtFrameInfo, 0x38);

/* Allocated by evtAddViewerObject. */
typedef struct EvtViewerObject {
    s32 unk00; /* 0x00 */
    s32 unk04; /* 0x04 */
    s32 unk08; /* 0x08 */
    s32 unk0C; /* 0x0C */
    s32 unk10; /* 0x10 */
    s16 unk14; /* 0x14 */
    s16 unk16; /* 0x16 */
    u8 pad18[12]; /* 0x18 */
    s32 unk24; /* 0x24 */
    u8 pad28[84]; /* 0x28 */
    s32 unk7C; /* 0x7C */
    s32 unk80; /* 0x80 */
} EvtViewerObject;
SIZE_CHECK(EvtViewerObjectSizeCheck, EvtViewerObject, 0x84);

/* Allocated by evtCreateUnitHandle. */
typedef struct EvtUnitHandle {
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
} EvtUnitHandle;
SIZE_CHECK(EvtUnitHandleSizeCheck, EvtUnitHandle, 0x230);

/* Allocated by sub_2019F8 and 3 more with this layout. */
typedef struct EffWork2019F8 {
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
} EffWork2019F8;
SIZE_CHECK(EffWork2019F8SizeCheck, EffWork2019F8, 0x68);

/* Allocated by sub_202038 and 5 more with this layout. */
typedef struct EffWork202038 {
    u8 pad00[24]; /* 0x00 */
    s32 unk18; /* 0x18 */
    f32 unk1C; /* 0x1C */
    s32 unk20; /* 0x20 */
} EffWork202038;
SIZE_CHECK(EffWork202038SizeCheck, EffWork202038, 0x24);

/* Allocated by sub_202098 and 5 more with this layout. */
typedef struct EffWork202098 {
    u8 pad00[24]; /* 0x00 */
    f32 unk18; /* 0x18 */
    f32 unk1C; /* 0x1C */
    s32 unk20; /* 0x20 */
} EffWork202098;
SIZE_CHECK(EffWork202098SizeCheck, EffWork202098, 0x24);

/* Allocated by sub_2258D0. */
typedef struct EffWork2258D0 {
    f32 unk00; /* 0x00 */
    u8 unk04; /* 0x04 */
    u8 pad05[3]; /* 0x05 */
    s32 unk08; /* 0x08 */
    s32 unk0C; /* 0x0C */
    u8 pad10[4]; /* 0x10 */
} EffWork2258D0;
SIZE_CHECK(EffWork2258D0SizeCheck, EffWork2258D0, 0x14);

/* Allocated by sub_232638. */
typedef struct EffWork232638 {
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
} EffWork232638;
SIZE_CHECK(EffWork232638SizeCheck, EffWork232638, 0xE8);

/* Allocated by sub_252058. */
typedef struct RaidouWork252058 {
    s32 unk00; /* 0x00 */
    s32 unk04; /* 0x04 */
    s32 unk08; /* 0x08 */
    s32 unk0C; /* 0x0C */
} RaidouWork252058;
SIZE_CHECK(RaidouWork252058SizeCheck, RaidouWork252058, 0x10);

#endif
