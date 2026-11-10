#include "common.h"

/* Declarations from smt3-decomp src/smt3/effect/effManager.c */
#include "common.h"

#include "kernel.h"

#include "libsdf.h"

#include "ps2asm.h"

extern void dds3ParticleManagerExit(void);

extern void dds3PolygonManagerExit(void);

extern void dds3BillboardManagerExit(void);

extern void sdfMemFreeDirect(sdfMemHandle_t *aHandle);

/* An effect handle: the type's entry in EffMajorTable plus the instance that
 * entry's create returned. version is stamped from the data revision. */
typedef struct effNode_t
{
    u32 type;
    u32 arg;
    s32 instance;
    f32 version;
} effNode_t;

/* One row per effect type, 0x30 bytes, indexed by effNode_t::type. */
typedef struct effMajorEntry_t
{
    s32 (*create)(u16 arg, s32 param);
    void (*draw)(s32 instance);
    void (*del)(s32 instance);
    void (*reset)(s32 instance);
    s32 (*getstate)(s32 instance);
    void (*setposition)(s32 instance, sdfVector4_t *pos);
    void (*setrotatematrix)(s32 instance, sdfMatrix4_t *matrix);
    void (*setrgba)(s32 instance, u32 rgba);
    void (*setloop)(s32 instance, s32 loop);
    s32 (*getloop)(s32 instance);
    s32 (*copy)(s32 instance);
    void (*setscale)(s32 instance, f32 scale);
} effMajorEntry_t;

extern effMajorEntry_t EffMajorTable[];

/* Effect resource header: the file data dds3EffectLoad is handed. */
typedef struct effResourceHeader_t
{
    u32 type;
    u32 arg;
    u32 unk08;
    f32 version;
    u8 payload[16];
} effResourceHeader_t;

/* The two instance layouts that carry a matrix a pre-1.03 file leaves unset. */
typedef struct effLegacyInstance_t
{
    u8 unk00[0x20];
    sdfMatrix4_t matrix20;
    u8 unk60[0x50];
    sdfMatrix4_t matrixB0;
} effLegacyInstance_t;

extern void *sdfChipAllocDirect(u32 aSize);

extern void sdfChipFreeDirect(void *aPtr);

#include "sdk_libc.h"

extern void dds3PolygonLinkOt(void);

extern void dds3TrackPolyLinkOt(void);

extern void dds3BillboardLinkOt(void);

extern void dds3MultiBillboardLinkOt(void);

extern void dds3CrossfadeRun(void);

extern void Eve_EffectDraw(void);

effNode_t *dds3EffectCreate(u16 type, u16 arg, saddr_t param);

void dds3EffectDataRevision(effResourceHeader_t *header);

extern u8 dds3EffectBase[];

extern char dds3EffectProcessName[];

extern sdfGrOt_t sdfGrOtTable[];

extern void sdfRandomizeByTime(void *aCB);

extern void dds3BillboardManagerInit(void);

extern void dds3PolygonManagerInit(void);

extern void dds3ParticleManagerInit(void);

extern void effDebugInit(void);

extern void Eve_EffectInit(void);

extern dds3ProcessID_t *dds3StartProcess(char *v1, u32 dDS3_PRIORITY_DRAW_PB, s32 v2,
                                         s32 v3, dds3ProcessFunc_t drawExit,
                                         dds3ProcessFunc_t deleteExit, void *p);

extern s32 sdfGrAllocPacketBuff(s32 aSize);

extern void sdfDrawTagInit(sdfDrawTag_t *a);

extern void sdfDrawTagLink(sdfDrawTag_t *aTag, addr_t aPacket);

s32 dds3EffectManagerRunF(dds3ProcessID_t *ProcessID);

s32 dds3EffectManagerRun(dds3ProcessID_t *ProcessID);

void dds3EffectManagerExit(void);

void dds3EffectManagerInit(void);

void dds3EffectManagerExit(void);

s32 dds3EffectManagerRunF(dds3ProcessID_t *ProcessID);

s32 dds3EffectManagerRun(dds3ProcessID_t *ProcessID);

void dds3EffectPCFSetLoop(void);

u32 dds3EffectPCFGetLoop(void);

void dds3EffectPCFReset(void);

u32 dds3EffectPCFGetState(void);

effNode_t *dds3EffectCreate(u16 type, u16 arg, saddr_t param);

void dds3EffectDelete(effNode_t *pData);

void dds3EffectDraw(effNode_t *pData);

void dds3EffectReset(effNode_t *hEffect);

void dds3EffectSetScale(effNode_t *pData, f32 scale);

s32 dds3EffectGetState(effNode_t *hEffect);

void dds3EffectSetPosition(effNode_t *pData, sdfVector4_t *pPos);

void dds3EffectSetRotateMatrix(effNode_t *pData, sdfMatrix4_t *pMatrix);

void dds3EffectSetRGBA(effNode_t *pData, u32 abgr);

void dds3EffectSetLoop(effNode_t *hEffect, u8 loop);

s32 dds3EffectGetLoop(effNode_t *hEffect);

effNode_t *dds3EffectLoad(effResourceHeader_t *pD3P);

effNode_t *dds3EffectFileLoad(const char *pFilename);

effNode_t *dds3EffectCopy(effNode_t *node);

void dds3EffectDataRevision(effResourceHeader_t *header);

s32 dds3ParticleCreate(u16 arg, s32 param);

void dds3ParticleDraw(s32 instance);

void dds3ParticleDelete(s32 instance);

void dds3ParticleReset(s32 instance);

s32 dds3ParticleGetState(s32 instance);

void dds3ParticleSetPosition(s32 instance, sdfVector4_t *pos);

void dds3ParticleSetRotateMatrix(s32 instance, sdfMatrix4_t *matrix);

void dds3ParticleSetRGBA(s32 instance, u32 rgba);

void dds3ParticleSetTypeFlag(s32 instance, s32 loop);

s32 dds3ParticleGetLoop(s32 instance);

s32 dds3ParticleCopy(s32 instance);

void dds3ParticleSetScale(s32 instance, f32 scale);

s32 dds3ParticleExpCreate(u16 arg, s32 param);

void dds3ParticleExpDraw(s32 instance);

void dds3ParticleExpDelete(s32 instance);

void dds3ParticleExpReset(s32 instance);

s32 dds3ParticleExpGetState(s32 instance);

void dds3ParticleExpSetPosition(s32 instance, sdfVector4_t *pos);

void dds3ParticleExpSetRotateMatrix(s32 instance, sdfMatrix4_t *matrix);

void dds3ParticleExpSetRGBA(s32 instance, u32 rgba);

void dds3ParticleExpSetLoop(s32 instance, s32 loop);

s32 dds3ParticleExpGetLoop(s32 instance);

s32 dds3ParticleExpCopy(s32 instance);

void dds3ParticleExpSetScale(s32 instance, f32 scale);

s32 dds3SpPolyCreate(u16 arg, s32 param);

void dds3SpPolyDraw(s32 instance);

void dds3SpPolyDelete(s32 instance);

void dds3SpPolyReset(s32 instance);

s32 dds3SpPolyGetState(s32 instance);

void dds3SpPolySetPosition(s32 instance, sdfVector4_t *pos);

void dds3SpPolySetRotateMatrix(s32 instance, sdfMatrix4_t *matrix);

void dds3SpPolySetRGBA(s32 instance, u32 rgba);

void dds3SpPolySetLoop(s32 instance, s32 loop);

s32 dds3SpPolyGetLoop(s32 instance);

s32 dds3SpPolyCopy(s32 instance);

void dds3SpPolySetScale(s32 instance, f32 scale);

s32 dds3MagatuhiParCreate(u16 arg, s32 param);

void dds3MagatuhiParDraw(s32 instance);

void dds3MagatuhiParDelete(s32 instance);

void func_001A8CD0(s32 instance);

s32 dds3MagatuhiParIsActive(s32 instance);

void dds3MagatuhiParSetPosition(s32 instance, sdfVector4_t *pos);

void dds3MagatuhiParSetRotateMatrix(s32 instance, sdfMatrix4_t *matrix);

void dds3MagatuhiParSetRGBA(s32 instance, u32 rgba);

void dds3MagatuhiParSetByType(s32 instance, s32 loop);

s32 func_001A8DF8(s32 instance);

s32 dds3MagatuhiParCopy(s32 instance);

void func_001A8EA8(s32 instance, f32 scale);

s32 dds3EffectPartCreate(u16 arg, s32 param);

void dds3EffectPartDraw(s32 instance);

void dds3EffectPartDelete(s32 instance);

void dds3EffectPartSetPosition(s32 instance, sdfVector4_t *pos);

void dds3EffectPartSetRotateMatrix(s32 instance, sdfMatrix4_t *matrix);

void dds3EffectPartSetColor(s32 instance, u32 rgba);

s32 dds3EffectPartCopy(s32 instance);

void dds3EffectPartSetScale(s32 instance, f32 scale);

extern effMajorEntry_t EffMajorTable[5];
/* End of declarations from src/smt3/effect/effManager.c */

INCLUDE_ASM(const s32, "effect/effManager", func_001E97E0);

INCLUDE_ASM(const s32, "effect/effManager", func_001E9888);

INCLUDE_ASM(const s32, "effect/effManager", func_001E98B0);

s32 dds3EffectManagerRun(dds3ProcessID_t *ProcessID)
{
    dds3PolygonLinkOt();
    dds3TrackPolyLinkOt();
    dds3BillboardLinkOt();
    dds3MultiBillboardLinkOt();
    dds3CrossfadeRun();
    Eve_EffectDraw();
    return 0;
}

void func_001E9A60(void) {
}

INCLUDE_ASM(const s32, "effect/effManager", func_001E9A68);

void func_001E9A70(void) {
}

INCLUDE_ASM(const s32, "effect/effManager", func_001E9A78);

INCLUDE_ASM(const s32, "effect/effManager", dds3EffectCreate);

void dds3EffectDelete(effNode_t *pData)
{
    EffMajorTable[pData->type].del(pData->instance);
    sdfChipFreeDirect(pData);
}

INCLUDE_ASM(const s32, "effect/effManager", func_001E9B50);

INCLUDE_ASM(const s32, "effect/effManager", func_001E9B88);

INCLUDE_ASM(const s32, "effect/effManager", func_001E9BC0);

INCLUDE_ASM(const s32, "effect/effManager", func_001E9BF8);

INCLUDE_ASM(const s32, "effect/effManager", func_001E9C40);

INCLUDE_ASM(const s32, "effect/effManager", func_001E9C78);

INCLUDE_ASM(const s32, "effect/effManager", func_001E9CB0);

INCLUDE_ASM(const s32, "effect/effManager", func_001E9CE8);

INCLUDE_ASM(const s32, "effect/effManager", func_001E9D28);

INCLUDE_ASM(const s32, "effect/effManager", dds3EffectLoad);

INCLUDE_ASM(const s32, "effect/effManager", func_001E9E78);

INCLUDE_ASM(const s32, "effect/effManager", func_001E9E98);

effNode_t *dds3EffectCopy(effNode_t *node)
{
    effNode_t *copy = sdfChipAllocDirect(sizeof(effNode_t));
    s32 instance = node->instance;

    copy->type = node->type;
    copy->arg = node->arg;
    copy->instance = EffMajorTable[node->type].copy(instance);
    return copy;
}

INCLUDE_ASM(const s32, "effect/effManager", dds3EffectDataRevision);

INCLUDE_SDATA(const s32, "effect/effManager", D_0046D078);

