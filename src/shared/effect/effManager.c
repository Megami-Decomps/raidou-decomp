#include "common.h"
#include "kernel.h"
#include "libsdf.h"
#include "ps2asm.h"
#include "sdfChip.h"
#include "sdfDraw.h"
#include "billManager.h"
#include "sdfPacketBuff.h"
#include "polyManager.h"
#include "effManager.h"
#include "sdfRand.h"

/* One row per effect type, 0x30 bytes, indexed by effHandle_t::type. */
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

/* The two instance layouts that carry a matrix a pre-1.03 file leaves unset. */
typedef struct effLegacyInstance_t
{
    u8 unk00[0x20];
    sdfMatrix4_t matrix20;
    u8 unk60[0x50];
    sdfMatrix4_t matrixB0;
} effLegacyInstance_t;

#include "sdk_libc.h"

extern void dds3MultiBillboardLinkOt(void);
extern void dds3CrossfadeRun(void);
extern void Eve_EffectDraw(void);
extern char dds3EffectProcessName[];
extern sdfGrOt_t sdfGrOtTable[];
s32 dds3EffectManagerRun(dds3ProcessID_t *ProcessID);
void dds3EffectPCFSetLoop(void);
void dds3EffectPCFReset(void);
extern effMajorEntry_t EffMajorTable[5];
extern char dds3EffectProcessName[7];

#include "include_asm.h"
#include "sdf_resource.h"
#include "billboard.h"
#include "sdf_types.h"
#include "draw.h"
#include "sdf_movie_types.h"
#include "effect.h"
#include "draw3d.h"
#include "evtEventViewer.h"
#include "evtEventViewerMenu.h"

extern void effObjectListCountersReset(void);

extern sdfRandCB_t dds3EffectBase;
extern void fclEffectManagerInitSub(void);
extern void dds3PolygonManagerInit(void);
extern void dds3ParticleManagerInit(void);
extern void Eve_EffectInit(void);
extern void effBTLFieldColorResetFlags(void);
dds3TaskUpdateResult_t ManagerRunF(dds3ProcessID_t *task);
s32 ManagerExit(dds3ProcessID_t *task);

void dds3EffectManagerInit(void)
{
    sdfRandomizeByTime(&dds3EffectBase);
    fclEffectManagerInitSub();
    dds3BillboardManagerInit();
    dds3PolygonManagerInit();
    dds3ParticleManagerInit();
    dds3StartProcess("effect_f", 0x2B02, 0, 0, ManagerRunF, NULL, NULL);
    dds3StartProcess(dds3EffectProcessName, 0x2B16, 0, 0, dds3EffectManagerRun, ManagerExit, NULL);
    effEctManagerInitSub();
    Eve_EffectInit();
    effBTLFieldColorResetFlags();
}

INCLUDE_ASM(const s32, "effect/effManager", ManagerExit);

/* Submit the test and alpha state packets to both field draw buffers. */
dds3TaskUpdateResult_t ManagerRunF(dds3ProcessID_t *task) {
    sdfDrawTag_t *list;
    sdfPrimTransPacket_t *state;
    sdfGrOt_t *surface;

    (void)task;
    list = (sdfDrawTag_t *)sdfGrAllocPacketBuff(0x20);
    sdfDrawTagInit(list);
    state = (sdfPrimTransPacket_t *)sdfGrAllocPacketBuff(0x40);
    state->header.dmaWords.tag = 3;
    state->header.dmaWords.vif = 0x5000000310000000ULL;
    state->header.giftag.parm = 0x1000000000008002ULL;
    state->header.giftag.regs = 0xE;
    state->elem.test.data = 0x51801;
    state->elem.test.reg = 0x47;
    state->elem.alpha.data = 0x44;
    state->elem.alpha.reg = 0x42;
    sdfDrawTagLink(list, (u32)state);
#ifdef VERSION_RAIDOU1
    surface = &sdfGrOtTable[60];
#else
    surface = &sdfGrOtTable[61];
#endif
    surface->regFunc(surface, list);

    list = (sdfDrawTag_t *)sdfGrAllocPacketBuff(0x20);
    sdfDrawTagInit(list);
    state = (sdfPrimTransPacket_t *)sdfGrAllocPacketBuff(0x40);
    state->header.dmaWords.tag = 3;
    state->header.dmaWords.vif = 0x5000000310000000ULL;
    state->header.giftag.parm = 0x1000000000008002ULL;
    state->header.giftag.regs = 0xE;
    state->elem.test.data = 0x71801;
    state->elem.test.reg = 0x47;
    state->elem.alpha.data = 0x42;
    state->elem.alpha.reg = 0x42;
    sdfDrawTagLink(list, (u32)state);
    surface++;
    surface->regFunc(surface, list);

    effObjectListCountersReset();
    return DDS3_TASK_UPDATE_RETAIN_CALLBACK;
}

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

void dds3EffectPCFSetLoop(void) {
}

INCLUDE_ASM(const s32, "effect/effManager", dds3EffectPCFGetLoop);

void dds3EffectPCFReset(void) {
}

INCLUDE_ASM(const s32, "effect/effManager", dds3EffectPCFGetState);

effHandle_t *dds3EffectCreate(u16 type, u16 arg, saddr_t param)
{
    effHandle_t *node = sdfChipAllocDirect(sizeof(effHandle_t));

    node->type = type;
    node->version = 1.03f;
    node->arg = arg;
    node->instance = EffMajorTable[type].create(arg, param);
    return node;
}

void dds3EffectDelete(effHandle_t *pData)
{
    EffMajorTable[pData->type].del(pData->instance);
    sdfChipFreeDirect(pData);
}

INCLUDE_ASM(const s32, "effect/effManager", dds3EffectDraw_dds3Effect);

INCLUDE_ASM(const s32, "effect/effManager", dds3EffectReset);

INCLUDE_ASM(const s32, "effect/effManager", dds3EffectSetScale);

INCLUDE_ASM(const s32, "effect/effManager", dds3EffectGetState);

INCLUDE_ASM(const s32, "effect/effManager", dds3EffectSetPosition);

INCLUDE_ASM(const s32, "effect/effManager", dds3EffectSetRotateMatrix);

INCLUDE_ASM(const s32, "effect/effManager", effSetEffectColorSubSub);

INCLUDE_ASM(const s32, "effect/effManager", dds3EffectSetLoop_dds3Effect);

INCLUDE_ASM(const s32, "effect/effManager", dds3EffectGetLoop);

effHandle_t *dds3EffectLoad(effResourceHeader_t *pD3P)
{
    effHandle_t *node;
    effLegacyInstance_t *instance;

    if (pD3P->version < 1.03f) {
        printf("old version!![%f]\n", pD3P->version);
        dds3EffectDataRevision(pD3P);
    }
    node = dds3EffectCreate((u16)pD3P->type, (u16)pD3P->arg, EE_ADDR_S(pD3P->payload));
    if (pD3P->version <= 1.02f) {
        switch (node->type) {
        case 0:
        case 1:
            instance = EE_ADDR_AS(effLegacyInstance_t *, node->instance);
            SDF_VU0_STOREIDENTITY(&instance->matrixB0);
            break;
        case 2:
            instance = EE_ADDR_AS(effLegacyInstance_t *, node->instance);
            SDF_VU0_STOREIDENTITY(&instance->matrix20);
            break;
        }
        printf("effManager:set Identity matrix\n");
    }
    return node;
}

INCLUDE_ASM(const s32, "effect/effManager", dds3EffectLoadEPL);

INCLUDE_ASM(const s32, "effect/effManager", sdfDevLoadTexList);

effHandle_t *dds3EffectCopy(effHandle_t *node)
{
    effHandle_t *copy = sdfChipAllocDirect(sizeof(effHandle_t));
    s32 instance = node->instance;

    copy->type = node->type;
    copy->arg = node->arg;
    copy->instance = EffMajorTable[node->type].copy(instance);
    return copy;
}

void dds3EffectDataRevision(effResourceHeader_t *header)
{
    u8 *payload = header->payload;

    if (header->version == 1.0f) {
        u32 type = header->type;
        switch (type) {
        case 0:
            *(u32 *)(payload + 0xA0) = 0xAC;
            break;
        case 1:
            {
                u8 *source = payload + *(u32 *)(payload + 4);
                payload = source + 0x10;
                *(u32 *)(payload + 0xA0) = 0xAC;
            }
            break;
        case 2:
            *(u32 *)(payload + 0x18) = 0x20;
            break;
        }
    }

    if (header->version <= 1.01f) {
        u32 type = header->type;
        if (type == 1) {
            u8 *source = payload + *(u32 *)(payload + 4);
            payload = source + 0x10;
            *(f32 *)(payload + 0x8C) =
                *(f32 *)(payload + 0x8C) * 0.1f * (f32)*(s32 *)(payload + 0x24) +
                *(f32 *)(payload + 0x10);
        }
        if (type < 2 && header->arg == 0) {
            printf("effManager:par spiral old version!![%f]\n", header->version);
        }
    }
}

INCLUDE_SDATA(const s32, "effect/effManager", dds3EffectProcessName);

