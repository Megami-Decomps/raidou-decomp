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


extern dds3Basic_t *dds3TheWorld;

typedef struct sdfPreDrawChain_t sdfPreDrawChain_t;

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

INCLUDE_ASM(const s32, "kernel/dds3KernelProcess", dds3FirstProcess);

extern u32 dds3RenderTex, dds3RenderDrawEnv;
extern s32 GlobalParamFlag;
extern sdf3DLight_t *dds3Data5[4], *dds3RenderLightParallel[4];
extern sdfVector4_t dds3Data3, dds3Data6, dds3RenderAmbientColor, dds3RenderTargetVector, dds3RenderEyeVector, effPCPBossTexRect;
extern sdfVector4_t fldAutoMapRect, fldAutoMapEye, fldAutoMapUp, aViewTarget, aViewEye, aViewDown;
extern sdf3DScreenParam_t dds3RenderScreenParam, dds3ScreenParam, DrawEnv;
extern sdfMatrix4_t dds3RenderViewportMatrix, aProjMatrix, sdfViewMatrix;
extern sdf3DFogParam_t dds3RenderFogParam, dds3DrawFirstProcessData2, FogParam, dds3DrawFirstProcessData;
extern sdfModelOt_t ModelOt[15];
extern sdfDrawTag_t *dds3CallGrAllocPacketBuff9(void);
extern void evtCheckSelectionState(void);
extern void dds3SceneLightUpdateFirst(void);
extern void sdf3MakeScreenParam(sdf3DScreenParam_t *);
extern void sdf3DMakeViewportPacket(sdf3DViewportPacket_t *, sdf3DScreenParam_t *, void *);
extern void sdf3DMakeFogPacket(sdf3DFogPacket_t *, sdf3DFogParam_t *);
extern void sdf3DSetCamera(sdf3DScreenParam_t *, void *);
extern void sdfModelOtInit(sdfModelOt_t *, sdfGrOt_t *, sdfGrOt_t *, sdfGrOt_t *, sdfGrOt_t *);
extern sdfDrawTag_t *dds3LinkZClearDrawTag(s32);
extern void eglSetOt(s32 ot);
extern void eglLinkTest(s32 ate, s32 atst, s32 aref, s32 afail, s32 date, s32 datm, s32 zte, s32 ztst);
extern void fldDrwZLoad(void);
extern void sdfPreDrawChainLink(sdfPreDrawChain_t *, sdfPreDrawTag_t *);
extern sdfPreDrawChain_t dds3PreDrawChain;
extern dds3DrawBank_t dds3DrawEnv[2];

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
    savedParamFlag = GlobalParamFlag;
    GlobalParamFlag = 0;
    sdf3MakeScreenParam(&DrawEnv);
    GlobalParamFlag = savedParamFlag;
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

INCLUDE_ASM(const s32, "kernel/dds3KernelProcess", dds3DrawProcess);

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

