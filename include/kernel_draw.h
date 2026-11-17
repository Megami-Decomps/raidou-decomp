#ifndef KERNEL_DRAW_H
#define KERNEL_DRAW_H

#include "draw.h"
#include "draw3d.h"

/* Native offsets: parallel lights 0x00, ambient 0x10, fog 0x20. */
typedef struct
{
    sdf3DLight_t *parallel[4];
    sdfVector4_t ambient;
    sdf3DFogParam_t fog;
} dds3LightState_t;

extern dds3LightState_t dds3LightParam;

void dds3SetAmbient(sdfVector4_t *ambient);
extern sdf3DFogParam_t FogParam;
void dds3SetFog(sdf3DFogParam_t *fog);
void dds3SetLight(sdf3DLight_t *light, s32 no);

/* sdfGrSetStdDrawEnv and sdfGrLinkStdDrawEnv are handed the DMA tag that
 * prefixes the draw env, so the env's own packet header sits 0x10 in. */
typedef struct
{
    sdfDmaTag_t dma;
    sdfGrDrawEnv_t env;
    u8 unk180[0xA0];
} dds3StdDrawEnv_t;

/* One frame buffer's draw bank, stride 0x22A0. Three draw tags: the VU1 init
 * chain at 0x0, the scene chain at 0x12D0, and the second-screen chain at
 * 0x1A10. */
typedef struct
{
    sdfDrawTag_t initTag;
    sdf3DInitPacket_t initPacket;
    sdfModelDrawEnv_t modelEnv[15];
    sdfDrawTag_t drawTag;
    sdfPreDrawTag_t preDrawTag;
    dds3StdDrawEnv_t stdEnv;
    sdf3DViewportPacket_t viewport;
    sdf3DLightParallelPacket_t light;
    sdfDrawTag_t renderTag;
    sdfGrDrawEnv_t renderEnv;
    sdf3DViewportPacket_t renderViewport;
    sdf3DLightParallelPacket_t renderLight;
    sdfDrawTag_t subTag;
    sdf3DViewportPacket_t subViewport;
    sdf3DLightParallelPacket_t subLight;
} dds3DrawBank_t;

extern sdfPreDrawChain_t dds3PreDrawChain;
extern dds3DrawBank_t dds3DrawEnv[2];
extern sdf3DScreenParam_t dds3ScreenParam;
extern sdf3DScreenParam_t DrawEnv;
extern sdf3DLight_t *dds3Data5[4];
extern sdfVector4_t dds3Data6;
extern sdfVector4_t dds3LightObjectAmbient;
extern sdfMatrix4_t aProjMatrix;
extern sdfMatrix4_t sdfViewMatrix;
extern sdfMatrix4_t sdfProjectionMatrix;

/* Native frame renderer ordering table: map size 0xE40, stride 0x20. */
/* DDS3_OT_DEBUG: debug text and window surface (SMT3 83), the second to last entry.
 * DDS3_OT_DEBUG_WIRE: debug wireframe surface (SMT3 44). */
#ifdef VERSION_RAIDOU2
extern sdfGrOt_t sdfGrOtTable[115];
#define DDS3_OT_DEBUG 113
#define DDS3_OT_DEBUG_WIRE 65
#else
extern sdfGrOt_t sdfGrOtTable[114];
#define DDS3_OT_DEBUG 112
#define DDS3_OT_DEBUG_WIRE 64
#endif
extern sdfGrOt_t dds3Data10;
void sdfGrSetStdDrawEnv(dds3StdDrawEnv_t *aEnv, sdfGrFrameSet_t *aFrameSet);
void sdfGrSetClearPacket(sdfGrClearPacket_t *aClear, s32 aW, s32 aH, s32 aPixelFormat, s32 aZBuffFormat);
void sdf3DMakeViewportPacket(sdf3DViewportPacket_t *aPacket, sdf3DScreenParam_t *aParam, void *aCamera);
void sdf3MakeScreenParam(sdf3DScreenParam_t *camera);
void sdfModelSetDrawEnv(sdfModelDrawEnv_t *aEnv);
void sdfModelDrawEnvLinkOt(sdfGrOt_t **aOt, sdfModelDrawEnv_t *aEnv);

#endif
