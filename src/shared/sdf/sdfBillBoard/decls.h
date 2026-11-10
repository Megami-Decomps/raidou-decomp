/* Declarations of smt3-decomp src/smt3/sdf/sdfBillBoard.c, for its bodies in this directory. */
#ifndef SHARED_SDF_SDFBILLBOARD_H
#define SHARED_SDF_SDFBILLBOARD_H

#include "common.h"

#include "ps2asm.h"

#include "billboard.h"

#include "sdf_lighting.h"

/* Declared here because no header carries these. */
extern void *memset(void *a, s32 b, size_t c);

#define SDF_DMA_QWORD_SHIFT 4

#define SDF_QWORD_ALIGNMENT_MASK 0xF

#define SDF_VIF_ITOP_COMPACT_VERTEX 0x04000002

#define SDF_VIF_ITOP_WIDE_VERTEX 0x04000008

#define SDF_VIF_MSCAL_VERTICES 0x14000008

/* Reserve size bytes from the active packet buffer, rounded up to 16 bytes.
 * Returns the prior cursor as a 32-bit address; the buffer owns this storage. */
s32 sdfGrAllocPacketBuff(s32 aSize);

s32 sdfBillBoardGetPacketSize(s32 aNum);

extern void *memcpy(void *, const void *, size_t);

void sdfBillBoardMakeTMPacket(u8 *aPacket, const f32 *aTM);

void sdfBillBoardMake1(u8 *aPacket, const f32 *aCenter, u32 aColor, const void *aUVList,
                                   const f32 *aXYOffsetList);

u32 *sdfBillBoardMakeN(const u128 *aCenter, const void *aColor, const void *aUVList, const void *aXYOffsetList, s32 aNum, void *(*aAllocator)(s32));

void sdfMultiBillBoardMake1(u8 *aPacket, const f32 *aCenter, u32 aColor0, u32 aColor1,
                   const void *aUVList0, const void *aUVList1, const f32 *aXYOffsetList);

u32 sdfMultiBillBoardGetPacketSize(s32 aNum);

u32 *sdfMultiBillBoardMakeN(u128 *aCenter, void *aColor, void *aUVList, void *aXYOffsetList, s32 aNum, void *(*aAllocator)(s32));

addr_t sdfBillBoardVcMakeN(const u128 *aCenter, const void *aColor,
                  const void *aUVList, const void *aXYOffsetList,
                  s32 aNum, void *(*aAllocator)(s32));

void sdfMultiBillBoardVcMake1(u8 *aPacket, const f32 *aCenter, const void *aColor, const void *aUVList0,
                   const void *aUVList1, const void *aXYOffsetList);

u32 sdfMultiBillBoardVcGetPacketSize(s32 aNum);

addr_t sdfMultiBillBoardVcMakeN(u128 *aCenter, void *aColor, void *aUVList, void *aXYOffsetList, s32 aNum, void *(*aAllocator)(s32));

void sdfBillBoardMakeTMPacket(u8 *aPacket, const f32 *aTM);

void sdfBillBoardMake1(u8 *aPacket, const f32 *aCenter, u32 aColor, const void *aUVList,
                                   const f32 *aXYOffsetList);

s32 sdfBillBoardGetPacketSize(s32 aNum);

/* vu0 routine: pack aligned positions into the VIF three-word stream. */
u32 *sdfBillBoardMakeN(const u128 *aCenter, const void *aColor, const void *aUVList, const void *aXYOffsetList, s32 aNum, void *(*aAllocator)(s32));

void sdfMultiBillBoardMake1(u8 *aPacket, const f32 *aCenter, u32 aColor0, u32 aColor1,
                   const void *aUVList0, const void *aUVList1, const f32 *aXYOffsetList);

u32 sdfMultiBillBoardGetPacketSize(s32 aNum);

u32 *sdfMultiBillBoardMakeN(u128 *aCenter, void *aColor, void *aUVList, void *aXYOffsetList, s32 aNum, void *(*aAllocator)(s32));

void sdfBillBoardVcMake1(u8 *aPacket, const f32 *aCenter, const u32 *aColor, const void *aUVList,
                   const f32 *aXYOffsetList);

u32 sdfBillBoardVcGetPacketSize(s32 aNum);

addr_t sdfBillBoardVcMakeN(const u128 *aCenter, const void *aColor,
                  const void *aUVList, const void *aXYOffsetList,
                  s32 aNum, void *(*aAllocator)(s32));

void sdfMultiBillBoardVcMake1(u8 *aPacket, const f32 *aCenter, const void *aColor, const void *aUVList0,
                   const void *aUVList1, const void *aXYOffsetList);

u32 sdfMultiBillBoardVcGetPacketSize(s32 aNum);

/* vu0 routine: pack aligned positions into the VIF three-word stream. */
addr_t sdfMultiBillBoardVcMakeN(u128 *aCenter, void *aColor, void *aUVList, void *aXYOffsetList, s32 aNum, void *(*aAllocator)(s32));

#endif
