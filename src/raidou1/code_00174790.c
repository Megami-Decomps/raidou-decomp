#include "common.h"
#include "dds3KernelCore.h"
#include "dds3KernelDraw.h"
#include "dds3KernelMain.h"
#include "fldDraw.h"
#include "kernel_draw.h"
#include "sdf2D.h"
#include "sdfDraw.h"
#include "sdfPacketBuff.h"
#include "sdfPrim.h"
#include "sdf_packet_list.h"
#include "fld.h"
#include "fldNpcPath.h"
#include "fpu.h"
#include "ps2asm.h"
#include "sdfMath.h"
#include "sdfQuat.h"
#include "code_00174790.h"
extern sdfVector4_t mapNpcPathMake_RotTable;

/* body: matched in store @ 2026-11-16: /home/wsl/.local/state/raidou-decomp-work/clusters/store/raidou1/00174790.c:1, match */
void mapCallPrimAllocDrawTag(void)
{
    sdfDrawTag_t *list = sdfPrimAllocDrawTag(NULL);
    sdfGrRefPacketHeader_t *reference = EE_ADDR_AS(sdfGrRefPacketHeader_t *, sdfGrAllocPacketBuff(0x20));
    u64 *texturePacket;
    u64 *blendPacket;
    sdfGrPacketHeader_t *sprite;
    fldSpriteVertex_t *vertex;
    sdfGrOt_t *surface;

    sdfGrLinkDispFrameTex(list, EE_ADDR(&dds3DrawEnv[dds3GetFrameID()].stdEnv), reference);
    texturePacket = EE_ADDR_AS(u64 *, sdfGrAllocPacketBuff(0x40));
    texturePacket[0] = 3;
    texturePacket[1] = 0x5000000310000000ULL;
    texturePacket[2] = 0x1000000000008002ULL;
    texturePacket[3] = 0xE;
    texturePacket[4] = 0x8000000080ULL;
    texturePacket[5] = 0x3B;
    texturePacket[6] = 0;
    texturePacket[7] = 0x3F;
    sdfDrawTagLink(list, EE_ADDR(texturePacket));
    blendPacket = EE_ADDR_AS(u64 *, sdfGrAllocPacketBuff(0x40));
    blendPacket[0] = 3;
    blendPacket[1] = 0x5000000310000000ULL;
    blendPacket[2] = 0x1000000000008002ULL;
    blendPacket[3] = 0xE;
    blendPacket[4] = 0x33001;
    blendPacket[5] = 0x47;
    blendPacket[6] = 0x8000000064ULL;
    blendPacket[7] = 0x42;
    sdfDrawTagLink(list, EE_ADDR(blendPacket));
    sprite = sdf2DAllocSprite(1);
    vertex = sdf2DGetPrimPtr(sprite);
    vertex->r = 0x81;
    vertex->g = 0x81;
    vertex->b = 0x81;
    vertex->a = 0x80;
    vertex->corner[0].u = 0;
    vertex->corner[0].v = 0;
    vertex->corner[1].u = 0x2000;
    vertex->corner[1].v = 0xDFF;
    vertex->corner[0].x = 0x6FF8;
    vertex->corner[0].y = 0x78FB;
    vertex->corner[1].x = 0x8FF8;
    vertex->corner[1].y = 0x86FB;
    vertex->corner[0].mask = 0;
    vertex->corner[0].flag = 0;
    vertex->corner[1].mask = 0;
    vertex->corner[1].flag = 0;
    sdfDrawTagLink(list, EE_ADDR(sprite));
    surface = &sdfGrOtTable[rsetFrameMaskOtOffset];
    surface->regFunc(surface, list);
}

void mapCallPrimAllocDrawTag2(s32 alpha) {
    sdfDrawTag_t *list = sdfPrimAllocDrawTag(NULL);
    void *reference = EE_ADDR_AS(void *, sdfGrAllocPacketBuff(0x20));
    u64 *texturePacket;
    u64 *blendPacket;
    void *sprite;
    fldSpriteVertex_t *vertex;
    sdfGrOt_t *surface;

    sdfGrLinkDispFrameTex(list, EE_ADDR(&dds3DrawEnv[dds3GetFrameID()].stdEnv), reference);
    texturePacket = EE_ADDR_AS(u64 *, sdfGrAllocPacketBuff(0x40));
    texturePacket[0] = 3;
    texturePacket[1] = 0x5000000310000000ULL;
    texturePacket[2] = 0x1000000000008002ULL;
    texturePacket[3] = 0xE;
    texturePacket[4] = 0x8000000080ULL;
    texturePacket[5] = 0x3B;
    texturePacket[6] = 0;
    texturePacket[7] = 0x3F;
    sdfDrawTagLink(list, EE_ADDR(texturePacket));
    blendPacket = EE_ADDR_AS(u64 *, sdfGrAllocPacketBuff(0x40));
    blendPacket[0] = 3;
    blendPacket[1] = 0x5000000310000000ULL;
    blendPacket[2] = 0x1000000000008002ULL;
    blendPacket[3] = 0xE;
    blendPacket[4] = 0x33001;
    blendPacket[5] = 0x47;
    blendPacket[6] = ((u64)(u32)alpha << 32) | 0x64;
    blendPacket[7] = 0x42;
    sdfDrawTagLink(list, EE_ADDR(blendPacket));
    sprite = sdf2DAllocSprite(1);
    vertex = EE_ADDR_AS(fldSpriteVertex_t *, sdf2DGetPrimPtr(EE_ADDR_AS(sdfGrPacketHeader_t *, sprite)));
    vertex->r = 0x80;
    vertex->g = 0x80;
    vertex->b = 0x80;
    vertex->a = alpha;
    vertex->corner[0].u = 0;
    vertex->corner[0].v = 0;
    vertex->corner[0].x = 0x6FF8;
    vertex->corner[0].y = 0x78FB;
    vertex->corner[0].mask = 0;
    vertex->corner[0].flag = 0;
    vertex->corner[1].u = 0x2000;
    vertex->corner[1].v = 0xDFF;
    vertex->corner[1].x = 0x8FF8;
    vertex->corner[1].y = 0x86FB;
    vertex->corner[1].mask = 0;
    vertex->corner[1].flag = 0;
    sdfDrawTagLink(list, EE_ADDR(sprite));
    surface = &sdfGrOtTable[rsetFrameMaskOtOffset];
    surface->regFunc(surface, list);
}

INCLUDE_ASM(const s32, "code_00174790", mapLeaf27);

void mapProcSequenceSub12(s32 arg) {
    mapData147 = arg;
}

s32 mapFunc11(dds3ProcessID_t *id) {
    if (mapData148 == 0) {
        return 0;
    }
    if (mapData147 == 0) {
        mapFunc11Data2++;
    }
    if (mapData147 == 1) {
        if (mapFunc11Data > 0) {
            mapFunc11Data--;
        } else {
            mapLeaf28();
        }
    }
    return 0;
}

INCLUDE_ASM(const s32, "code_00174790", mapLeaf28);

INCLUDE_ASM(const s32, "code_00174790", mapBtlEndWaitInitSub);

dds3ProcessID_t *mapFadeProc(void) {
    return dds3StartProcess(fldEncDrawName2, 0x2B0D, 0, 1, mapFunc11, NULL, NULL);
}

/* NON_MATCHING: 51 of 89 words match.  */
#ifdef NON_MATCHING
void GetElement(u32 *segment, f32 *weight, mapNpcPathData_t *pData, f32 frame) {
    u32 i;

    if (pData->nKey != 1) {
        for (i = 0; i < pData->nKey - 1; i++) {
            if (pData->pKey[i] <= frame && frame < pData->pKey[i + 1]) {
                *segment = i;
                *weight = 1.0f / (pData->pKey[i + 1] - pData->pKey[i]);
                *weight *= frame - pData->pKey[i];
                return;
            }
        }
    }
    *segment = pData->nKey - 2;
    *weight = 1.0f;
}
#else
INCLUDE_ASM(const s32, "code_00174790", GetElement);
#endif

void mapNpcPathMake_KeyFrame(s32 num, f32 spd, mapNpcPathData_t *pathdata) {
    sdfVector3_t *pItem;
    u32 *pKey = pathdata->pKey;
    u32 key = 0;
    s32 i;

    pKey[0] = 0;
    pItem = pathdata->item.pItem;
    for (i = 1; i < num; i++) {
        f32 dx = pItem[i].x - pItem[i - 1].x;
        f32 dy = pItem[i].y - pItem[i - 1].y;
        f32 dz = pItem[i].z - pItem[i - 1].z;

        key += (s32)(fsqrtf(dx * dx + dy * dy + dz * dz) / spd);
        pKey[i] = key;
    }
}

/* NON_MATCHING: 52 of 70 words match.  */
#ifdef NON_MATCHING
void mapNpcPathMake_Rot(s32 num, mapNpcPathData_t *pathdataT, mapNpcPathData_t *pathdataR) {
    sdfVector4_t axis;
    sdfVector4_t *pItemR;
    sdfVector3_t *pItem;
    s32 i;

    axis = mapNpcPathMake_RotTable;
    pItemR = pathdataR->item.pItemR;
    pItem = pathdataT->item.pItem;
    for (i = 1; i < num; i++) {
        f32 dx = pItem[i].x - pItem[i - 1].x;
        f32 dz = pItem[i].z - pItem[i - 1].z;

        if (dx > 0.099999995f || dx < -0.099999995f || dz > 0.099999995f || dz < -0.099999995f) {
            f32 angle = sdfAtan2(dx, dz);

            SDF_VU0_LOADVECTOR(&axis);
            FromRotation(3.13999963f - angle);
            SDF_VU0_STOREVECTOR(&pItemR[i]);
        }
    }
    SDF_VU0_LOADVECTOR(&pItemR[1]);
    SDF_VU0_STOREVECTOR(pItemR);
}
#else
INCLUDE_ASM(const s32, "code_00174790", mapNpcPathMake_Rot);
#endif

INCLUDE_ASM(const s32, "code_00174790", mapCallAtan22);

INCLUDE_RODATA(const s32, "code_00174790", fldEncDrawName2);

INCLUDE_RODATA(const s32, "code_00174790", mapNpcPathMake_RotTable);

