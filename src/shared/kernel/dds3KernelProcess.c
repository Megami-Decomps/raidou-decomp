#include "common.h"
#include "basic.h"
#include "kernel.h"
#include "kernel_draw.h"
#include "model.h"
#include "ps2asm.h"
#include "sdfFrame.h"
#include "dds3Basic.h"
#include "draw3d.h"
#include "libsdf.h"
#include "sdfDraw.h"
#include "sdfPacketBuff.h"
#include "sdk_libc.h"
#include "dds3KernelDraw.h"
#include "dds3KernelMain.h"
#include "sdfVu0.h"
#include "sdf3D.h"
#include "code_r1_00104370.h"
#include "sdf2D.h"
#include "sdfGraph.h"
#include "sdf_packet_list.h"


extern dds3Basic_t *dds3TheWorld;

typedef struct sdfGrRefPacketHeader_t sdfGrRefPacketHeader_t;

/* Sprite GIF payload: RGBAQ followed by UV/XYZ2 for each corner.
 * The GS ignores the unused lanes; native writes XYZ's 16-bit ADC lane. */
typedef struct {
    s32 u, v;
    u8 unusedUV[8];
    s32 x, y, z;
    s16 adc;
    u8 unusedXYZ[2];
} dds3BlurCorner_t;

typedef struct {
    s32 r, g, b, a;
    dds3BlurCorner_t corner[2];
} dds3BlurSprite_t;

extern u8 dds3ScreenBlurFlag, dds3DrawProcessFlag;
extern s16 dds3ScreenBlurAlpha, dds3ScreenBlurScale;
extern sdfDrawTag_t *dds3ClosedDrawTag;
s32 dds3BasicMainProcess(dds3ProcessID_t *id);
s32 dds3BasicDrawProcess(dds3ProcessID_t *id);

s32 dds3FirstProcess(dds3ProcessID_t *id)
{
    s32 count;
    s32 frame;
    sdfModelDrawEnv_t *env;

    count = 14;
    frame = dds3GetFrameID();
    sdfGrInitNewFrame();
    sdfGrOtInit(sdfGrOtTable, ARRAY_COUNT(sdfGrOtTable));
    sdfPreDrawChainInit(&dds3PreDrawChain);
    dds3InitDrawEnvSub(frame);
    env = dds3DrawEnv[frame].modelEnv;
    do {
        sdfModelSetDrawEnv(env++);
    } while (--count >= 0);
    sdfGrOtTable[0].regFunc(&sdfGrOtTable[0], &dds3DrawEnv[frame].initTag);
    return 0;
}

extern u32 dds3RenderTex, dds3RenderDrawEnv;
extern sdf3DLight_t *dds3Data5[4], *dds3RenderLightParallel[4];
extern sdfVector4_t dds3Data3, dds3Data6, dds3RenderAmbientColor, dds3RenderTargetVector, dds3RenderEyeVector, effPCPBossTexRect;
extern sdfVector4_t fldAutoMapRect, fldAutoMapEye, fldAutoMapUp, aViewTarget, aViewEye, aViewDown;
extern sdf3DScreenParam_t dds3RenderScreenParam, dds3ScreenParam, DrawEnv;
extern sdfMatrix4_t dds3RenderViewportMatrix, aProjMatrix, sdfViewMatrix;
extern sdf3DFogParam_t dds3RenderFogParam, dds3DrawFirstProcessData2, FogParam, dds3DrawFirstProcessData;
extern sdfDrawTag_t *dds3CallGrAllocPacketBuff9(void);
extern void evtCheckSelectionState(void);
extern void dds3SceneLightUpdateFirst(void);
extern void sdf3MakeScreenParam(sdf3DScreenParam_t *);
extern void sdf3DMakeViewportPacket(sdf3DViewportPacket_t *, sdf3DScreenParam_t *, void *);
extern void sdf3DMakeFogPacket(sdf3DFogPacket_t *, sdf3DFogParam_t *);
extern void sdf3DSetCamera(sdf3DScreenParam_t *, void *);
extern void sdfModelOtInit(sdfModelOt_t *, sdfGrOt_t *, sdfGrOt_t *, sdfGrOt_t *, sdfGrOt_t *);
extern sdfDrawTag_t *dds3LinkZClearDrawTag(s32);
extern void fldDrwZLoad(void);

/* NON_MATCHING: 35 of 496 words match. Register allocation: mine saves s8, retail only s0-s7; layout and call order match. dds3DrawBank_t.modelEnv[15] and sdfGrOtTable[114] in headers.patch. */
#ifdef VERSION_RAIDOU1
#ifdef NON_MATCHING
s32 dds3DrawFirstProcess(dds3ProcessID_t *id)
{
    sdfVector4_t zero QWORD_ALIGNED;
    volatile f32 target[4] QWORD_ALIGNED;
    volatile f32 eye[4] QWORD_ALIGNED;
    u32 frame;
    s32 savedParamFlag;
    sdfDrawTag_t *tag;
    sdfDrawTag_t *sceneTag;
    sdf3DFogPacket_t *fog;
    sdf3DFogPacket_t *sceneFog;

    memset(&zero, 0, sizeof(zero));
    zero.w = 1.0f;
    frame = dds3GetFrameID();
    dds3RunQuake();
    evtCheckSelectionState();
    dds3SceneLightUpdateFirst();
    if (dds3RenderTex != 0) {
        sdf3DMakeLightParallelPacket(&dds3DrawEnv[frame].renderLight, dds3RenderLightParallel, &dds3RenderAmbientColor);
        dds3DrawEnv[frame].renderEnv.clear.clearColor = dds3RenderDrawEnv;
        if (dds3RenderTex != 0) {
            sdf3MakeScreenParam(&dds3RenderScreenParam);
            LookAtMatrix(&dds3RenderTargetVector, &dds3RenderEyeVector, &effPCPBossTexRect);
            SDF_VU0_STOREMATRIX(&dds3RenderViewportMatrix);
            sdf3DMakeViewportPacket(&dds3DrawEnv[frame].renderViewport, &dds3RenderScreenParam, &dds3RenderViewportMatrix);
            sdfGrOtTable[1].regFunc(&sdfGrOtTable[1], &dds3DrawEnv[frame].renderTag);
            tag = dds3CallGrAllocPacketBuff9();
            fog = EE_ADDR_AS(sdf3DFogPacket_t *, sdfGrAllocPacketBuff(0x50));
            sdf3DMakeFogPacket(fog, &dds3RenderFogParam);
            sdfDrawTagLink(tag, EE_ADDR(fog));
            sdfGrOtTable[1].regFunc(&sdfGrOtTable[1], tag);
        }
    }
    sdf3DMakeLightParallelPacket(&dds3DrawEnv[frame].subLight, dds3Data5, &dds3Data6);
    savedParamFlag = sdf3DScreenAdjust;
    sdf3DScreenAdjust = 0;
    sdf3MakeScreenParam(&DrawEnv);
    sdf3DScreenAdjust = savedParamFlag;
    LookAtMatrix(&fldAutoMapRect, &fldAutoMapEye, &fldAutoMapUp);
    SDF_VU0_STOREMATRIX(&aProjMatrix);
    sdf3DMakeViewportPacket(&dds3DrawEnv[frame].subViewport, &DrawEnv, &aProjMatrix);
    sdfGrOtTable[90].regFunc(&sdfGrOtTable[90], &dds3DrawEnv[frame].subTag);
    tag = dds3CallGrAllocPacketBuff9();
    fog = EE_ADDR_AS(sdf3DFogPacket_t *, sdfGrAllocPacketBuff(0x50));
    sdf3DMakeFogPacket(fog, &dds3DrawFirstProcessData2);
    sdfDrawTagLink(tag, EE_ADDR(fog));
    sdfGrOtTable[90].regFunc(&sdfGrOtTable[90], tag);
    sdf3DMakeLightParallelPacket(&dds3DrawEnv[frame].light, dds3LightParam.parallel, &dds3Data3);
    dds3DrawEnv[frame].stdEnv.env.clear.clearColor = dds3BackColor;
    sdf3MakeScreenParam(&dds3ScreenParam);
    SDF_VU0_LOADVECTOR(&aViewTarget);
    SDF_VU0_LOADVECTOR_B(&aViewEye);
    SDF_VU0_SUBVECTOR();
    SDF_VU0_NORMALIZE();
    SDF_VU0_SCALEVECTOR(600.0f);
    SDF_VU0_ADDVECTOR();
    SDF_VU0_LOADVECTOR_B(&zero);
    SDF_VU0_ADDVECTOR();
    SDF_VU0_STOREVECTOR(target);
    SDF_VU0_LOADVECTOR(&aViewEye);
    SDF_VU0_ADDVECTOR();
    SDF_VU0_STOREVECTOR(eye);
    LookAtMatrix(target, eye, &aViewDown);
    SDF_VU0_STOREMATRIX(&sdfViewMatrix);
    sdf3DMakeViewportPacket(&dds3DrawEnv[frame].viewport, &dds3ScreenParam, &sdfViewMatrix);
    sdf3DSetCamera(&dds3ScreenParam, &sdfViewMatrix);
    sdfGrOtTable[7].regFunc(&sdfGrOtTable[7], &dds3DrawEnv[frame].drawTag);
    sceneTag = dds3CallGrAllocPacketBuff9();
    sceneFog = EE_ADDR_AS(sdf3DFogPacket_t *, sdfGrAllocPacketBuff(0x50));
    sdf3DMakeFogPacket(sceneFog, &FogParam);
    sdfDrawTagLink(sceneTag, EE_ADDR(sceneFog));
    sdfGrOtTable[7].regFunc(&sdfGrOtTable[7], sceneTag);
    tag = dds3CallGrAllocPacketBuff9();
    fog = EE_ADDR_AS(sdf3DFogPacket_t *, sdfGrAllocPacketBuff(0x50));
    sdf3DMakeFogPacket(fog, &dds3DrawFirstProcessData);
    sdfDrawTagLink(tag, EE_ADDR(fog));
    sdfGrOtTable[32].regFunc(&sdfGrOtTable[32], tag);
    tag = dds3CallGrAllocPacketBuff9();
    fog = EE_ADDR_AS(sdf3DFogPacket_t *, sdfGrAllocPacketBuff(0x50));
    sdf3DMakeFogPacket(fog, &FogParam);
    sdfDrawTagLink(tag, EE_ADDR(fog));
    sdfGrOtTable[37].regFunc(&sdfGrOtTable[37], tag);
    sdfPreDrawChainLink(&dds3PreDrawChain, &dds3DrawEnv[frame].preDrawTag);
    sdfModelOtInit(&ModelOt[0], &sdfGrOtTable[44], &sdfGrOtTable[48], &sdfGrOtTable[64], &sdfGrOtTable[66]);
    sdfModelOtInit(&ModelOt[1], &sdfGrOtTable[47], &sdfGrOtTable[49], &sdfGrOtTable[65], &sdfGrOtTable[67]);
    sdfModelOtInit(&ModelOt[9], &sdfGrOtTable[42], &sdfGrOtTable[48], &sdfGrOtTable[51], &sdfGrOtTable[50]);
    sdfModelOtInit(&ModelOt[10], &sdfGrOtTable[73], &sdfGrOtTable[74], &sdfGrOtTable[75], &sdfGrOtTable[76]);
    sdfModelOtInit(&ModelOt[11], &sdfGrOtTable[85], &sdfGrOtTable[86], &sdfGrOtTable[87], &sdfGrOtTable[88]);
    sdfModelOtInit(&ModelOt[2], &sdfGrOtTable[8], &sdfGrOtTable[9], &sdfGrOtTable[10], &sdfGrOtTable[11]);
    sdfModelOtInit(&ModelOt[3], &sdfGrOtTable[13], &sdfGrOtTable[14], &sdfGrOtTable[15], &sdfGrOtTable[16]);
    sdfModelOtInit(&ModelOt[4], &sdfGrOtTable[18], &sdfGrOtTable[19], &sdfGrOtTable[20], &sdfGrOtTable[21]);
    sdfModelOtInit(&ModelOt[5], &sdfGrOtTable[23], &sdfGrOtTable[24], &sdfGrOtTable[25], &sdfGrOtTable[26]);
    sdfModelOtInit(&ModelOt[6], &sdfGrOtTable[28], &sdfGrOtTable[29], &sdfGrOtTable[30], &sdfGrOtTable[31]);
    sdfModelOtInit(&ModelOt[7], &sdfGrOtTable[53], &sdfGrOtTable[54], &sdfGrOtTable[55], &sdfGrOtTable[56]);
    sdfModelOtInit(&ModelOt[8], &sdfGrOtTable[33], &sdfGrOtTable[34], &sdfGrOtTable[35], &sdfGrOtTable[36]);
    sdfModelOtInit(&ModelOt[12], &sdfGrOtTable[58], &sdfGrOtTable[59], &sdfGrOtTable[62], &sdfGrOtTable[63]);
    sdfModelOtInit(&ModelOt[13], &sdfGrOtTable[91], &sdfGrOtTable[92], &sdfGrOtTable[93], &sdfGrOtTable[94]);
    sdfModelOtInit(&ModelOt[14], &sdfGrOtTable[2], &sdfGrOtTable[3], &sdfGrOtTable[4], &sdfGrOtTable[5]);
    sdf3DSetDefaultLight(&dds3DrawEnv[frame].light);
    sdf3DSetDefaultFog(sceneFog);
    tag = dds3LinkZClearDrawTag(0);
    sdfGrOtTable[101].regFunc(&sdfGrOtTable[101], tag);
    tag = dds3LinkZClearDrawTag(1);
    sdfGrOtTable[109].regFunc(&sdfGrOtTable[109], tag);
    tag = dds3LinkZClearDrawTag(2);
    sdfGrOtTable[111].regFunc(&sdfGrOtTable[111], tag);
    eglSetOt(53);
    eglLinkTest(1, 5, 128, 1, 0, 0, 1, 2);
    eglLinkAlpha(0);
    eglSetOt(54);
    eglLinkTest(1, 5, 128, 1, 0, 0, 1, 2);
    eglLinkAlpha(0);
    eglSetOt(55);
    eglLinkTest(1, 5, 128, 1, 0, 0, 1, 2);
    eglLinkAlpha(1);
    eglSetOt(56);
    eglLinkTest(1, 5, 128, 1, 0, 0, 1, 2);
    eglLinkAlpha(2);
    eglSetOt(43);
    eglLinkAlpha(0);
    eglLinkTest(1, 0, 128, 3, 0, 0, 1, 2);
    fldDrwZLoad();
    eglSetOt(46);
    eglLinkAlpha(0);
    eglLinkTest(1, 0, 128, 3, 0, 0, 1, 2);
    fldDrwZLoad();
    return 0;
}
#else
INCLUDE_ASM(const s32, "kernel/dds3KernelProcess", dds3DrawFirstProcess);
#endif
#else
INCLUDE_ASM(const s32, "kernel/dds3KernelProcess", dds3DrawFirstProcess);
#endif

#ifdef VERSION_RAIDOU1
s32 dds3DrawProcess(dds3ProcessID_t *task) {
    s32 bufferIndex = dds3GetFrameID();
    sdfModelOt_t *groupHeads;
    sdfModelDrawEnv_t *packetGroups;
    sdfDrawTag_t *packetList;
    u64 *texturePacket;
    u64 *blendPacket;
    dds3BlurSprite_t *vertex;
    void *spritePacket;
    sdfDrawTag_t *poolHead;
    s32 edgeDistances[4];
    s32 i;

    dds3DrawDistRipOverError();
    dds3BistaUpdate();
    dds3DrawBista();
    dds3FadeUpdate();
    dds3CallGetFrameID();
    dds3FilterCalc();
    dds3FilterDraw();
    dds3BlurUpdate();
    groupHeads = ModelOt;
    packetGroups = dds3DrawEnv[bufferIndex].modelEnv;
    for (i = 14; i >= 0; i--) {
        sdfModelDrawEnvLinkOt(groupHeads->ot, packetGroups);
        groupHeads++;
        packetGroups++;
    }
    if (dds3ScreenBlurFlag != 0 && dds3NullDrawProc() == 0) {
        packetList = EE_ADDR_AS(sdfDrawTag_t *, sdfGrAllocPacketBuff(0x20));
        sdfDrawTagInit(packetList);
        sdfGrLinkDispFrameTex(packetList, EE_ADDR(&dds3DrawEnv[bufferIndex].stdEnv), EE_ADDR_AS(sdfGrRefPacketHeader_t *, sdfGrAllocPacketBuff(0x20)));
        texturePacket = EE_ADDR_AS(u64 *, sdfGrAllocPacketBuff(0x40));
        texturePacket[0] = 3;
        /* VIF FLUSHA, then DIRECT for the three following quadwords. */
        texturePacket[1] = ((u64)0x50000003 << 16 | 0x1000) << 16;
        texturePacket[2] = ((u64)0x10000000 << 32) | 0x8002;
        texturePacket[3] = 0xE;
        texturePacket[4] = ((u64)0x80 << 32) | 0x80;
        texturePacket[5] = 0x3B;
        texturePacket[6] = 0;
        texturePacket[7] = 0x3F;
        sdfDrawTagLink(packetList, EE_ADDR(texturePacket));
        blendPacket = EE_ADDR_AS(u64 *, sdfGrAllocPacketBuff(0x40));
        blendPacket[0] = 3;
        blendPacket[1] = ((u64)0x50000003 << 16 | 0x1000) << 16;
        blendPacket[2] = ((u64)0x10000000 << 32) | 0x8002;
        blendPacket[3] = 0xE;
        blendPacket[4] = 0x31001;
        blendPacket[5] = 0x47;
        blendPacket[6] = 0x44;
        blendPacket[7] = 0x42;
        sdfDrawTagLink(packetList, EE_ADDR(blendPacket));
        edgeDistances[0] = dds3ScreenBlurOfsX[0] + 0x100;
        edgeDistances[1] = dds3ScreenBlurOfsX[1] + 0xE0;
        edgeDistances[2] = 0x100 - dds3ScreenBlurOfsX[0];
        edgeDistances[3] = 0xE0 - dds3ScreenBlurOfsX[1];
        /* Signed Q12 scale; X and Y use distinct GS coordinate units below. */
        for (i = 0; i != 4; i++) {
            edgeDistances[i] = edgeDistances[i] * dds3ScreenBlurScale >> 12;
        }
        spritePacket = sdf2DAllocSprite(1);
        vertex = (dds3BlurSprite_t *)sdf2DGetPrimPtr(spritePacket);
        vertex->r = 0x80;
        vertex->g = 0x80;
        vertex->b = 0x80;
        vertex->a = dds3ScreenBlurAlpha;
        vertex->corner[0].u = 0;
        vertex->corner[0].v = 0;
        vertex->corner[0].x = 0x7000 - edgeDistances[0] * 16;
        vertex->corner[0].y = 0x7900 - edgeDistances[1] * 8;
        vertex->corner[0].z = 0;
        vertex->corner[0].adc = 0;
        vertex->corner[1].u = 0x2000;
        vertex->corner[1].v = 0xE00;
        vertex->corner[1].x = 0x9000 + edgeDistances[2] * 16;
        vertex->corner[1].y = 0x8700 + edgeDistances[3] * 8;
        vertex->corner[1].z = 0;
        vertex->corner[1].adc = 0;
        sdfDrawTagLink(packetList, EE_ADDR(spritePacket));
        dds3Data10.regFunc(&dds3Data10, packetList);
    }
    poolHead = sdfGrOtClose(sdfGrOtTable);
    dds3ClosedDrawTag = poolHead;
    if (dds3DrawProcessFlag != 0) {
        sdfGrFlushOtChain(EE_ADDR_AS(u32 *, poolHead->head));
        dds3DrawProcessFlag = 0;
    }
    if (!(dds3DrawFlag & 0x2000000)) {
        sdfGrReqDraw(poolHead, &dds3PreDrawChain);
    } else {
        dds3DrawFlag &= 0xFDFFFFFF;
    }
    sdfDrawPublishFlag = 0;
    return 0;
}
#else
INCLUDE_ASM(const s32, "kernel/dds3KernelProcess", dds3DrawProcess);
#endif

s32 dds3BasicMainProcess(dds3ProcessID_t *id) {
    dds3CallBasicMainProc(dds3TheWorld);
    return 0;
}

#ifdef VERSION_RAIDOU1
s32 dds3BasicDrawProcess(dds3ProcessID_t *id) {
    dds3CallBasicDrawProc(dds3TheWorld);
    return 0;
}
#else
INCLUDE_ASM(const s32, "kernel/dds3KernelProcess", dds3BasicDrawProcess);
#endif

INCLUDE_SDATA(const s32, "kernel/dds3KernelProcess", dds3ClosedDrawTag);

