#include "common.h"

/* Declarations from smt3-decomp src/smt3/effect/effPCPBoss.c */
#include "common.h"

#include "kernel.h"

#include "ps2asm.h"

#include "fpu.h"

#include "model.h"

#include "motion.h"

#include "draw3d.h"

#include "effect.h"

#include "event.h"

#include "nb.h"

#include "sdfFrame.h"

typedef struct
{
    u16 type;
    void *pHandle;
} effPartHandle_t;

typedef struct effPCPBossBED_t
{
    u8 unk0000[0xC3C];
    s32 active;
} effPCPBossBED_t;

typedef union effPCPBossVector_t
{
    u128 q;
    f32 f[4];
} effPCPBossVector_t;

/* The placement block every effEveBEDCutCreate call in this unit fills on the
 * stack. */
typedef struct effPCPBossPlace_t
{
    f32 payload[11];
    u32 color;
} effPCPBossPlace_t;

/* One crack segment: the extents effPCPCrackSizeInit randomizes plus the
 * negative age that delays its start. */
/* The fragment parameter head effPCP_ThunderPolyListCreate copies out of a
 * work; only the fragment count is set by hand here. */
typedef struct effPCPBossThunderHead_t
{
    u8 unk00[0x24];
    s32 fragmentCount;
    u8 unk28[0x2C];
} effPCPBossThunderHead_t;

typedef struct effPCPBossThunderParam_t
{
    f32 points[10][4];
    s32 count;
    effPCPBossThunderHead_t head;
} effPCPBossThunderParam_t;

typedef struct effPCPCrackPartFlash_t
{
    f32 length;
    f32 width_in;
    f32 width_out;
    s32 frame;
    u8 type;
    u8 unk11[3];
} effPCPCrackPartFlash_t;

typedef struct effPCPCrackPart_t
{
    void *hFlash;
    f32 dir[3];
    u8 unk10[4];
    f32 rotate;
    s32 rotationStartAge;
    f32 rotate_add;
    effPCPCrackPartFlash_t *pSize;
} effPCPCrackPart_t;

/* Parameter fields below are read directly by effPCPCrackDraw. The complete
 * 0x8C head is copied by Create/Copy, followed by the 0x20 runtime suffix. */
typedef struct effPCPCrack_t
{
    f32 position[4];
    u8 drawTrail;
    f32 frameStep;
    f32 scale;
    f32 trailWidth;
    s32 trailColorStartFrame;
    s32 trailColorTransitionFrames;
    u32 trailStartCenterColor;
    u32 trailStartEdgeColor;
    u32 trailEndCenterColor;
    u32 trailEndEdgeColor;
    u16 drawBucket;
    u8 unk3A[2];
    u8 hasCells;
    u8 cellCountFromFrame;
    s32 cellStartFrame;
    s32 delaySpread;
    s32 cellDuration;
    s32 cellFadeIn;
    s32 cellFadeOut;
    s32 offsetRampFrames;
    f32 offsetDistance;
    f32 offsetRandomness;
    f32 baseExtent;
    f32 tipExtent;
    f32 extentRandomness;
    s32 cellColorStartAge;
    s32 cellColorTransitionFrames;
    u32 cellStartCenterColor;
    u32 cellStartEdgeColor;
    u32 cellEndCenterColor;
    u32 cellEndEdgeColor;
    s32 blend;
    u8 directionMode;
    u8 unk89[3];
    effPCPCrackPart_t *pPart;
    void *groupsHandle;
    u32 MarkCount;
    u16 cellCount;
    u8 unk9A[2];
    s32 frame;
    u32 abgr;
    parSystem_t *system;
    effPartHandle_t *part;
} effPCPCrack_t;

/* The parameter block effPCPCrackCreate copies into the work head; the two
 * rotation fields sit where the work keeps its group pointer. */
/* The leading block effPCPCrackCreate copies into the work head in one go. */
typedef struct effCrackHead_t
{
    s32 word[35];
} effCrackHead_t;

typedef struct effPCPCrackParam_t
{
    effCrackHead_t head;
    s32 rotationStartAge;
    f32 angularSpeed;
} effPCPCrackParam_t;

typedef struct effCrackMotion_t
{
    u8 unk00[0x20];
    f32 frameStep;
    u8 unk24[0xA];
    u16 frameCount;
} effCrackMotion_t;

typedef struct effCrackModel_t
{
    u8 unk00[0x18];
    void *inner;
    effCrackMotion_t *first;
} effCrackModel_t;

/* Five vertex colour words per cell. */
typedef struct effCrackColorSlot_t
{
    u32 color[5];
} effCrackColorSlot_t;

typedef struct effBaelHead_t
{
    s32 unk00;
    s32 unk04;
    s32 unk08;
    s32 unk0C;
} effBaelHead_t;

typedef struct effPCPBossBael_t
{
    f32 target[4];
    effBaelHead_t param;
    s32 frame;
    u32 abgr;
    effPartHandle_t *hPB;
    effPartHandle_t *hPCF;
} effPCPBossBael_t;

typedef struct effPCPBossONGY_t
{
    struct nbFormation_t *param;
    sdfMat_t *pMat;
    f32 radian;
    s32 frame;
} effPCPBossONGY_t;

/* The argument block effPkt3DPrimMakePoly reads. */
typedef struct effPCPBossWork_t
{
    u16 primCount;
    u16 vertexCount;
    u16 unk04;
    u8 unk06[2];
    u32 color;
    const void *unk0C;
    effPCPBossVector_t *positions;
    u8 unk14[4];
    f32 *uvs;
    u8 unk1C[4];
    u32 *colors;
    u8 unk24[8];
} effPCPBossWork_t;

typedef struct effPCPBossBUNSIN_t
{
    f32 pos_s[4];
    f32 pos_e[4];
    effPCPBossBUNSINParam_t param;
    void *matrix;
    union {
        s32 unk2C;
        f32 radian;
    };
    s32 frame;
} effPCPBossBUNSIN_t;

typedef struct effPCPBossBUNSIN2_t
{
    effPCPBossBUNSIN2Param_t head;
    s32 frame;
    s32 state;
    effPCPBossBUNSIN_t *parts[3];
} effPCPBossBUNSIN2_t;

typedef struct effPCPBossDengeki_t
{
    u8 unk00[0x54];
    f32 scale;
    void *polyLists[4];
    effPCPBossBED_t *hBED;
    void *cut;
    s32 frame;
    u32 color;
} effPCPBossDengeki_t;

typedef struct effPCPBossThunderbolt_t
{
    u8 position[0x10];
    u8 param[0x54];
    void *pTPolyList;
    s32 frame;
    u32 color;
} effPCPBossThunderbolt_t;

typedef struct effPCPBossKAGUInvoke_t
{
    union
    {
        u8 param[0x3C0];
        struct
        {
            f32 directions[6][9][4];
            effPCPBossThunderHead_t head;
            s32 limit;
            s32 fadeIn;
            s32 fadeOut;
        } draw;
    };
    s32 pTPList;
    void *point[26];
    s32 frame;
    u32 color;
    void *handle;
} effPCPBossKAGUInvoke_t;

typedef struct effPCPBossKAGUAttack_t
{
    u8 unk00[0x54];
    f32 scale;
    s32 limit;
    s32 fadeIn;
    s32 fadeOut;
    u32 groupCount;
    u8 unk68[4];
    void *polyLists[32];
    effPCPBossBED_t *hBED;
    void *cut;
    s32 frame;
    u32 color;
} effPCPBossKAGUAttack_t;

typedef struct effPCPBossKAGUThunderbolt_t
{
    u8 position[0x10];
    u8 unk010[0xE0];
    f32 scale;
    s32 limit;
    s32 fadeIn;
    s32 fadeOut;
    void *pTPolyList;
    effPCPCrack_t *pCrack;
    effPCPBossBED_t *hBED;
    void *hBEDCut;
    s32 frame;
    u32 color;
} effPCPBossKAGUThunderbolt_t;

typedef struct effPCPBossKAGU2Attack_t
{
    u8 unk00[0x54];
    f32 scale;
    s32 duration;
    s32 fadeIn;
    s32 fadeOut;
    u32 groupCount;
    f32 randomness;
    void *polyLists[16];
    effPCPBossBED_t *hBED;
    void *hBEDCut;
    s32 frame;
    u32 color;
} effPCPBossKAGU2Attack_t;

typedef struct effPCPBossKAGU2MUHENK_t
{
    s32 param;
    u32 color;
} effPCPBossKAGU2MUHENK_t;

typedef struct effPCPBossMETATOBeam_t
{
    u8 position[0x10];
    u8 unk10[0x54];
    s32 limit;
    s32 fadeIn;
    s32 fadeOut;
    void *polyLists[2];
    s32 frame;
    u32 abgr;
} effPCPBossMETATOBeam_t;

typedef struct effPCPScreenCrackModel_t
{
    s32 param;
    effPartHandle_t *hModel;
    s32 frame;
    u32 abgr;
} effPCPScreenCrackModel_t;

typedef struct effPCPMuzzleFlashParam_t
{
    s32 scale_h;
    s32 scale_w;
} effPCPMuzzleFlashParam_t;

typedef struct effPCPMuzzleFlash_tag
{
    u8 matrix[0x40];
    u8 pos[0x10];
    effPCPMuzzleFlashParam_t param;
    void *hBillboard;
    s32 frame;
    f32 scale;
} effPCPMuzzleFlash_tag;

/* The block effPCPDANTE_Thunder_Copy hands straight back to Create. */
typedef struct effPCPDANTE_Thunder_Param_t
{
    u8 head[0x54];
    s32 mark;
    s32 limit;
    s32 fadeIn;
    s32 fadeOut;
} effPCPDANTE_Thunder_Param_t;

typedef struct effPCPDANTE_Thunder_tag
{
    u8 position[0x10];
    u8 matrix[0x40];
    effPCPDANTE_Thunder_Param_t params;
    f32 scale;
    u32 color;
    void *poly;
    s32 frame;
    u8 drawRequest;
    u8 unkC5[3];
    s32 process;
} effPCPDANTE_Thunder_tag;

typedef struct effPCPDANTE_UV_tag
{
    s32 u;
    s32 v;
    u32 w;
    u32 h;
} effPCPDANTE_UV_tag;

/* The 16-byte block effPCPDANTE_Damage_Create copies into the work head; the
 * last word is the per-target delay step. */
typedef struct effPCPDANTE_Damage_Param_t
{
    s32 total;
    s32 fadeIn;
    s32 fadeOut;
    s32 wait;
} effPCPDANTE_Damage_Param_t;

typedef struct effPCPDANTE_Damage_tag
{
    effPCPDANTE_Damage_Param_t param;
    u8 delay[0x20];
    s32 unk030;
    u8 unk034;
    u8 unk035[3];
    effPCPBossBED_t *bed;
    void *cuts[0x20];
    s32 counters[0x20];
    u32 color;
} effPCPDANTE_Damage_tag;

typedef struct effPCP_PBCamera_Param_t
{
    s32 life;
} effPCP_PBCamera_Param_t;

typedef struct effPCP_PBCamera_tag
{
    u8 matrix[0x40];
    u8 position[0x10];
    s32 life;
    effPartHandle_t *part;
    u8 cameraOverride;
    u8 unk59[3];
    s32 frame;
} effPCP_PBCamera_tag;

typedef struct effPCPDANTE_ShowTime_Param_t
{
    s32 reserve;
} effPCPDANTE_ShowTime_Param_t;

typedef struct effPCPDANTE_ShowTime_tag
{
    u8 position[0x10];
    s32 param;
    u8 flash[3][0x38];
    void *hTex[2];
    s32 frame;
    s32 frameParts[3]; /* Bingo, AreYouReady, ShowTime at +0xC8/+0xCC/+0xD0. */
    u8 texLoaded;
} effPCPDANTE_ShowTime_tag;

typedef struct effPCP_BED_Param_t
{
    s32 life;
    s32 fade_in;
    s32 fade_out;
} effPCP_BED_Param_t;

typedef struct effPCP_BED_tag
{
    u8 matrix[0x40];
    u8 pos[0x10];
    effPCP_BED_Param_t param;
    u32 abgr;
    s32 frame;
    effPCPBossBED_t *hBED;
    void *hBEDCut;
} effPCP_BED_tag;

typedef struct effPCP_HBED2_Param_t
{
    s32 word[7];
} effPCP_HBED2_Param_t;

typedef struct effPCP_HBED2_tag
{
    effPCP_HBED2_Param_t param;
    void *pHelperBED;
} effPCP_HBED2_tag;

extern u8 dds3EffectBase[];

extern void *memcpy(void *a, const void *b, size_t c);

extern void *sdfChipAllocDirect(u32 aSize);

extern void *sdfChipAllocZeroDirect(u32 aSize);

extern void *memset(void *a, s32 b, size_t c);

extern void *sdfMatNew(void);

extern void sdfMatSetConstant(void *aMat, f32 aRatio);

extern void *dds3BillboardCreate(s32 count, void *params);

extern void *dds3BillboardCopy(void *hBillboard);

extern effPCPBossWork_t effPCPBossWork;

extern u8 D_0031FF20[];

extern void *sdfTexSet(const void *aTex);

extern void effEveBEDCutDelete(void *hEveBEDCut);

extern void effEveBEDDelete(effPCPBossBED_t *hEveBED);

extern effPCPBossBED_t *effEveBEDCreate(const void *pData);

extern void *effEveBEDCutCreate(effPCPBossBED_t *hEveBED, s32 CutNum, const effPCPBossPlace_t *pParams);

extern void *effPCPHelperBEDCreate(void *pParam, void *pPBData, void *pBEDData);

extern void effEveBEDCutSetParams(void *hEveBEDCut, const effPCPBossPlace_t *pParams);

extern void effEveBEDCutDraw(void *hEveBEDCut);

extern u32 effColorBlend(u32 abgr0, u32 abgr1, f32 rate);

extern void *dds3GetProcessData(dds3ProcessID_t *process);

extern u32 effBTLSkillCheckRun(void);

extern u32 effBTLSkillGetTargetBit(void);

extern u32 dds3GetRandIntA(void *aLimit, u32 name);

extern void *effBTLSkillGetInvokeFormation(void);

extern void effBTLModelGetMarkMatrix2(void *pFormation, s32 mark);

extern void effBTLModelGetMarkPosition2(void *pFormation, s32 mark);

extern void effDebugPointDraw(const void *pPos);

extern void effDebugLineDraw(const void *pPos0, const void *pPos1);

extern s32 sdfModelGetMarkPosMatrix(void *aRoot, s32 aId);

extern void effPCPCrackInit(effPCPCrack_t *pPCPCrack);

extern void dds3PadVibrationSet(s32 port, u8 power, s32 frames);

extern s32 effBTLModelGetID(nbFormation_t *pFormation);

extern u128 *effPCP_ThunderPolyListGetParam(void *pTPolyList);

extern void effPCP_ThunderPolyListDraw(void *hData);

extern void effPCP_ThunderPolyListSetColor(void *hData, u32 abgr);

extern void nbPanelFlashInit(void *pFlash, u32 color0, u32 color1, s32 x, f32 scaleX, f32 scaleY, s32 y);

extern void nbPanelFlashDraw(void *pFlash, u32 abgrBase);

extern void effBTLModelGetSphereCenter(void *pFormation);

extern void *effBTLSkillGetTargetFormation(s32 number);

extern void LookAtMatrix(const void *aTarget, const void *aEye, const void *aDown);

extern void ReverseMatrix(void);

extern void sdf3DPrimLinkTm(void *aTag, s32 aAllocator);

extern void *effPkt3DPrimMakePoly(effPCPBossWork_t *aParm);

extern void sdfDrawTagLink(sdfDrawTag_t *aTag, addr_t aPacket);

extern u8 dds3RenderTargetVector[], dds3RenderEyeVector[];

extern u8 effPCPBossTexRect[];

extern u8 effPCPBossLookDown[];

extern void dds3EffectPartSetRotateMatrix(effPartHandle_t *hEffPart, const void *pMatrix);

extern void dds3EffectPartSetPosition(effPartHandle_t *hEffPart, const void *pPos);

extern void dds3EffectPartSetColor(effPartHandle_t *hEffPart, u32 abgr);

extern void dds3EffectPartDraw(effPartHandle_t *hEffPart);

s32 effPCPDANTE_Thunder_ProcessDraw(dds3ProcessID_t *process);

void effPCPDANTE_Thunder_SetPosition(effPCPDANTE_Thunder_tag *hData, const void *pPos);

void effPCPDANTE_Thunder_SetRotateMatrix(effPCPDANTE_Thunder_tag *hData, const void *pRot);

extern void *effPCPHelperBEDCopy(void *helper);

extern void sdfChipFreeDirect(void *aPtr);

extern void *sdfMemAllocDirect(u32 aSize);

extern void *sdfMemLock(void *aHandle);

extern void sdfMemFreeDirect(void *aHandle);

extern u32 dds3GetRandA(u8 *name);

extern f32 dds3GetRandFloatA(u8 *name);

extern effPartHandle_t *dds3EffectPartCreate(u16 type, void *pData);

extern effPartHandle_t *dds3EffectPartCopy(effPartHandle_t *hEffPartSrc);

extern void dds3EffectPartDelete(effPartHandle_t *hEffPart);

extern void *dds3EffectPartGetHandle(effPartHandle_t *hEffPart);

extern void mdlSingleMotion(effCrackModel_t *aHandle, s32 aGroup, s32 aNumber);

extern s32 sdfModelGetMarkPosCount(void *aRoot);

extern void *dds3TrackPolyCreate(s32 number, s32 face, s32 rec, s32 type);

extern void dds3TrackPolySetMode(void *hTrackPoly, s32 mode);

extern void dds3TrackPolyDelete(void *hTrackPoly);

extern void *dds3FlashPolyListCreate(s32 face);

extern void dds3FlashPolyListDelete(void *hPoly);

extern f32 *dds3FlashPolyListGetVertex(void *pool, s32 index);

extern effCrackColorSlot_t *dds3FlashPolyListGetVC(void *pool, s32 index);

extern void dds3FlashPolyListSetBlend(void *hPoly, s32 blend);

extern effPCPCrack_t *effPCPCrackCreate(effPCPCrackParam_t *pParam, void *pPB);

extern effPCPCrack_t *effPCPCrackCopy(effPCPCrack_t *src);

extern void *effPCPBossTexHandle;

extern s32 effPCPBossONGYTexRefCount;

extern s32 effPCPBossONGY_RFrame;

extern s32 dds3RenderDrawEnv;

extern void dds3AllocRenderTexture(s32 sx, s32 sy, f32 aspect);

extern void dds3RenderTextureON(void);

extern void dds3RenderTextureOFF(void);

extern void dds3FreeRenderTexture(void);

extern void *mdlLoad(u32 aMajor, u32 aMinor);

extern void mdlFree(void *aHandle);

extern void mdlSmoothLoopMotion(void *aHandle, s32 aGroup, s32 aNumber, f32 aBeforeLeng,
                                f32 aSmoothLeng);

extern s32 dds3GetKernelCount(void);

#include "sdk_libc.h"

extern void sdfMatFreeDelayed(void *aMat);

extern void effPCP_ThunderPolyListDelete(void *hData);

extern void *effPCP_ThunderPolyListCreate(const effPCPBossThunderParam_t *param);

extern effPCPBossThunderbolt_t *effPCPBossThunderboltCreate(effPCPBossThunderbolt_t *pParam);

extern f32 effPCPBossKAGUDrawParam[4];

extern f32 effPCPBossKAGU2DrawParam[4];

extern void dds3BillboardDelete(void *hData);

extern void dds3BillboardSetRGBA(void *hBillboard, u32 abgr);

extern effPCPBossKAGUInvoke_t *effPCPBossKAGUInvokeCreate(effPCPBossKAGUInvoke_t *pParam);

extern effPCPBossMETATOBeam_t *effPCPBossMETATOBeamCreate(void *pParam);

extern void effPCPBossKAGUThunderboltDrawSub(effPCPBossKAGUThunderbolt_t *pPCPBossKAGUThunderbolt, const f32 *pConvPos);

extern void effPCPCrackDraw(effPCPCrack_t *hData);

extern void dds3KillProcess(s32 id, s32 shutdowntimer);

extern void effPCP_ThunderPolyDelete(void *hData);

extern void sdfTexFreeDelayed(void *aHandle);

extern void mdlSetRotate(mdlObj_t *aHandle);

extern void FromEuler(f32 x, f32 y, f32 z);

extern f32 atan2f(f32 y, f32 x);

extern f32 tanf(f32 x);

extern f32 acosf(f32 x);

extern void mdlSetPosition(mdlObj_t *aHandle);

extern void mdlLink(mdlObj_t *aHandle, void *aModelOt);

extern void sdfDrawTagInit(sdfDrawTag_t *aTag);

extern s32 sdfGrAllocPacketBuff(s32 aSize);

extern sdfModelOt_t sdfModelOtTable[];

extern f32 aViewEye[4], aViewTarget[4];

extern sdf3DScreenParam_t dds3ScreenParam;

extern void effPCP_BED_MatrixToQuaternion(f32 matrix[4][4]);

extern void *effPCPHelperBEDGetModel(void *pHelperBED);

extern void effPCPHelperBEDDelete(void *hData);

extern void effPCPHelperBEDDraw(void *hData);

extern void effPCPHelperBEDSetPosition(void *hData, const void *pPos);

extern void effPCPHelperBEDSetColor(void *hData, u32 abgr);

extern void effPCPHelperBEDSetScale(void *hData, f32 scale);

extern effPCPDANTE_Thunder_tag *effPCPDANTE_Thunder_Create(const effPCPDANTE_Thunder_Param_t *pParam);

extern effPCPDANTE_Damage_tag *effPCPDANTE_Damage_Create(void *pParam, void *pBED);

extern const f32 D_00533E10[];

extern const f32 D_00533DD0[];

extern const f32 D_00533DE0[];

extern void Mult(void);

extern void ToMatrix(void);

extern void MultMatrixNoArg(void);

extern s32 dds3BillboardGetAnimFrame(void *hBillboard);

extern void dds3BillboardSetAnimKey2(void *hBillboard, s32 frame);

extern f32 dds3BillboardGetDirectionRotate(void *pPos, const void *pVec);

extern f32 dds3BillboardGetDirectionScale(void *pPos, const void *pVec);

extern void dds3BillboardSetScale(void *hBillboard, f32 x, f32 y);

extern void dds3BillboardSetRotate(void *hBillboard, f32 rot);

extern void dds3BillboardSetPosition(void *hBillboard, const void *pPos);

extern void dds3BillboardDraw(void *hData);

extern s32 sdfModelGetMarkPos(void *aRoot, s32 aId);

extern void SetRotateMatrixY_B(f32 aAngle);

extern void SetRotateMatrixAxis(f32 aAngle, const void *aAxis);

extern void SetRotateMatrixZ(f32 aAngle);

extern void *effPCP_ThunderPolyCreate(const void *params);

extern s32 dds3StartProcess(const char *v1, s32 dDS3_PRIORITY_DRAW_PB, s32 v2, s32 v3,
                            void *drawExit, void *deleteExit, void *p);

extern void dds3SetProcessData(s32 id, void *data);

extern const char effPCPDANTE_ThunderProcessName[];

extern u128 *effPCP_ThunderPolyGetParam(void *pThunderPoly);

extern void effPCP_ThunderPolySetColor(void *hData, u32 abgr);

extern void effPCP_ThunderPolyDraw(void *hData);

extern void nbPnlSprite(s32 x, s32 y, s32 w, s32 h, s32 u, s32 v, u32 uw, u32 vh, u32 rgba,
                        void *th);

extern s32 effPadVibeFrames[2], effPadVibeRemain[2];

extern u8 effPadVibePower[2], effPadVibeFadeIn[2];

extern effPCPBossBael_t *effPCPBossBaelCreate(void *pParam, void *pPB, void *pPCF);

extern effPCPBossDengeki_t *effPCPBossDengekiCreate(effPCPBossDengeki_t *pParam, void *pBEDData);

extern effPCPBossKAGUAttack_t *effPCPBossKAGUAttackCreate(effPCPBossKAGUAttack_t *pParam,
                                                          void *pBEDData);

extern effPCPBossKAGUThunderbolt_t *effPCPBossKAGUThunderboltCreate(
    effPCPBossKAGUThunderbolt_t *pParam, void *pBP, void *pBED);

extern effPCPBossKAGU2Attack_t *effPCPBossKAGU2AttackCreate(effPCPBossKAGU2Attack_t *pParam,
                                                            void *pBEDData);

extern effPCPCrack_t *effPCPCrack2Create(effPCPCrackParam_t *pParam, void *pPB);

extern effPCPScreenCrackModel_t *effPCPScreenCrackModelCreate(effPCPScreenCrackModel_t *pParam, void *pPB);

extern effPCPMuzzleFlash_tag *effPCPMuzzleFlashCreate(effPCPMuzzleFlashParam_t *pParam,
                                                    void *pP2A);

extern effPCP_PBCamera_tag *effPCP_PBCamera_Create(effPCP_PBCamera_Param_t *pParam, void *pPB);

extern effPCPDANTE_ShowTime_tag *effPCPDANTE_ShowTime_Create(effPCPDANTE_ShowTime_Param_t *pParam,
                                                           void *pTMX0, void *pTMX1);

extern effPCP_BED_tag *effPCP_BED_Create(effPCP_BED_Param_t *pParam, void *pBED);

extern effPCP_HBED2_tag *effPCP_HBED2_Create(effPCP_HBED2_Param_t *pParam, void *pPBData, void *pBEDData);

void effPCPCrackSizeInit(effPCPCrack_t *pPCPCrack, effPCPCrackPartFlash_t *pSize);

void effPCPBossONGYTexRenderStart(void);

void effPCPBossONGYTexRenderEnd(void);

s32 effPCPBossONGYModelTexRenderCheck(void);

effPCPBossONGY_t *effPCPBossONGYCreate(effPCPBossONGYParam_t *param);

void effPCPBossONGYDelete(effPCPBossONGY_t *pPCPBossONGY);

s32 effPCPBossONGYGetLength(void);

effPCPBossONGY_t *effPCPBossONGY2Create(effPCPBossONGYParam_t *param);

void effPCPBossONGY2Delete(effPCPBossONGY_t *pPCPBossONGY2);

s32 effPCPBossONGY2GetLength(void);

effPCPBossBUNSIN_t *effPCPBossBUNSINCreate(effPCPBossBUNSINParam_t *pParam);

void effPCPBossBUNSINDelete(effPCPBossBUNSIN_t *pPCPBossBUNSIN);

void effPCPBossBUNSINDraw(effPCPBossBUNSIN_t *work);

s32 effPCPBossBUNSINGetLength(void);

effPCPBossBUNSIN2_t *effPCPBossBUNSIN2Create(effPCPBossBUNSIN2Param_t *param);

void effPCPBossBUNSIN2Delete(effPCPBossBUNSIN2_t *pPCPBossBUNSIN2);

void effPCPBossBUNSIN2Draw(effPCPBossBUNSIN2_t *pPCPBossBUNSIN2);

s32 effPCPBossBUNSIN2GetLength(void);

void effPCPCrackSizeInit(effPCPCrack_t *pPCPCrack, effPCPCrackPartFlash_t *pSize);

void effPCPCrackInit(effPCPCrack_t *pPCPCrack);

effPCPCrack_t *effPCPCrackCreate(effPCPCrackParam_t *pParam, void *pPB);

effPCPCrack_t *effPCPCrackCreatePCF(void *parameterTable);

effPCPCrack_t *effPCPCrackCopy(effPCPCrack_t *src);

void effPCPCrackDelete(effPCPCrack_t *hData);

extern f32 D_0031FF70[4];

extern void mdlSetScale(mdlObj_t *model);

extern void dds3TrackPolyDraw(parSystem_t *system);

extern void dds3FlashPolyListDraw(void *pool);

extern void dds3FlashPolyListSetABGR(void *pool, u32 color);

extern u32 effMagaCylinderCalcColor(u32 color, u32 scale);

/* Same native algorithm as dds-decomp/redthing1's effPCPCrackDraw: transform
 * each trail sample, then emit and fade its delayed flash cells. */
void effPCPCrackDraw(effPCPCrack_t *work);

void effPCPCrackSetPosition(effPCPCrack_t *hData, const void *pPos);

void effPCPCrackSetColor(effPCPCrack_t *hData, u32 abgr);

effPCPBossBael_t *effPCPBossBaelCreate(void *pParam, void *pPB, void *pPCF);

effPCPBossBael_t *effPCPBossBaelCreatePCF(void *parameterTable);

effPCPBossBael_t *effPCPBossBaelCopy(effPCPBossBael_t *hData, s32 bPos, s32 tbldata);

void effPCPBossBaelDelete(effPCPBossBael_t *hData);

void effPCPBossBaelDraw(effPCPBossBael_t *hData);

void effPCPBossBaelSetPosition(effPCPBossBael_t *hData, const void *pPos);

void effPCPBossBaelSetColor(effPCPBossBael_t *hData, u32 abgr);

effPCPBossDengeki_t *effPCPBossDengekiCreate(effPCPBossDengeki_t *pParam, void *pBEDData);

effPCPBossDengeki_t *effPCPBossDengekiCreatePCF(void *parameterTable);

effPCPBossDengeki_t *effPCPBossDengekiCopy(effPCPBossDengeki_t *hData, s32 bPos, s32 tbldata);

void effPCPBossDengekiDelete(effPCPBossDengeki_t *hData);

void effPCPBossDengekiDraw(effPCPBossDengeki_t *hData);

void effPCPBossDengekiSetColor(effPCPBossDengeki_t *hData, u32 abgr);

effPCPBossThunderbolt_t *effPCPBossThunderboltCreate(effPCPBossThunderbolt_t *pParam);

effPCPBossThunderbolt_t *effPCPBossThunderboltCreatePCF(void *hData, s32 bPos, s32 tbldata);

effPCPBossThunderbolt_t *effPCPBossThunderboltCopy(void *hData, s32 bPos, s32 tbldata);

void effPCPBossThunderboltDelete(effPCPBossThunderbolt_t *hData);

void effPCPBossThunderboltDraw(effPCPBossThunderbolt_t *hData);

void effPCPBossThunderboltSetPosition(effPCPBossThunderbolt_t *hData, const void *pPos);

void effPCPBossThunderboltSetColor(effPCPBossThunderbolt_t *hData, u32 abgr);

effPCPBossKAGUInvoke_t *effPCPBossKAGUInvokeCreate(effPCPBossKAGUInvoke_t *pParam);

effPCPBossKAGUInvoke_t *effPCPBossKAGUInvokeCreatePCF(void *hData, s32 bPos, s32 tbldata);

effPCPBossKAGUInvoke_t *effPCPBossKAGUInvokeCopy(void *hData, s32 bPos, s32 tbldata);

void effPCPBossKAGUInvokeDelete(effPCPBossKAGUInvoke_t *hData);

void effPCPBossKAGUInvokeSetColor(effPCPBossKAGUInvoke_t *hData, u32 abgr);

effPCPBossKAGUAttack_t *effPCPBossKAGUAttackCreate(effPCPBossKAGUAttack_t *pParam, void *pBEDData);

effPCPBossKAGUAttack_t *effPCPBossKAGUAttackCreatePCF(void *parameterTable);

effPCPBossKAGUAttack_t *effPCPBossKAGUAttackCopy(effPCPBossKAGUAttack_t *hData, s32 bPos, s32 tbldata);

void effPCPBossKAGUAttackDelete(effPCPBossKAGUAttack_t *hData);

/* vu0 routine: the five split points of the KAGU arc. */
void effPCPBossKAGUAttackGetSplitPoint(u128 *pPosBuff, const u128 *pPoint1, const u128 *pPoint2,
                                       const u128 *pCenter);

void effPCPBossKAGUAttackDraw(effPCPBossKAGUAttack_t *work);

void effPCPBossKAGUAttackSetColor(effPCPBossKAGUAttack_t *hData, u32 abgr);

effPCPBossKAGUThunderbolt_t *effPCPBossKAGUThunderboltCreate(effPCPBossKAGUThunderbolt_t *pParam,
                                                              void *pBP, void *pBED);

effPCPBossKAGUThunderbolt_t *effPCPBossKAGUThunderboltCreatePCF(void *parameterTable);

effPCPBossKAGUThunderbolt_t *effPCPBossKAGUThunderboltCopy(effPCPBossKAGUThunderbolt_t *hData,
                                                           s32 bPos, s32 tbldata);

void effPCPBossKAGUThunderboltDelete(effPCPBossKAGUThunderbolt_t *hData);

void effPCPBossKAGUThunderboltDrawSub(effPCPBossKAGUThunderbolt_t *pPCPBossKAGUThunderbolt, const f32 *pConvPos);

void effPCPBossKAGUThunderboltDraw(effPCPBossKAGUThunderbolt_t *hData);

void effPCPBossKAGUThunderboltSetPosition(effPCPBossKAGUThunderbolt_t *hData, const void *pPos);

void effPCPBossKAGUThunderboltSetColor(effPCPBossKAGUThunderbolt_t *hData, u32 abgr);

effPCPBossKAGU2Attack_t *effPCPBossKAGU2AttackCreate(effPCPBossKAGU2Attack_t *pParam, void *pBEDData);

effPCPBossKAGU2Attack_t *effPCPBossKAGU2AttackCreatePCF(void *parameterTable);

effPCPBossKAGU2Attack_t *effPCPBossKAGU2AttackCopy(effPCPBossKAGU2Attack_t *hData, s32 bPos,
                                                   s32 tbldata);

void effPCPBossKAGU2AttackDelete(effPCPBossKAGU2Attack_t *hData);

/* vu0 routine: the five split points of the KAGU2 arc. */
void effPCPBossKAGU2AttackGetSplitPoint(u128 *pPosBuff, const u128 *pPoint1, const effPCPBossVector_t *pPoint2,
                                        const u128 *pCenter, f32 rand, f32 rotate);

void effPCPBossKAGU2AttackDraw(effPCPBossKAGU2Attack_t *work);

void effPCPBossKAGU2AttackSetColor(effPCPBossKAGU2Attack_t *hData, u32 abgr);

void effPCPBossKAGU2ThunderboltDraw(effPCPBossKAGUThunderbolt_t *hData);

effPCPBossKAGU2MUHENK_t *effPCPBossKAGU2MUHENKCreate(void *pParam);

effPCPBossKAGU2MUHENK_t *effPCPBossKAGU2MUHENKCreatePCF(void *hData, s32 bPos, s32 tbldata);

effPCPBossKAGU2MUHENK_t *effPCPBossKAGU2MUHENKCopy(void *hData, s32 bPos, s32 tbldata);

void effPCPBossKAGU2MUHENKDelete(effPCPBossKAGU2MUHENK_t *pPCPBossKAGU2MUHENK);

/* vu0 routine: the seven MUHENK mark gizmos. */
void effPCPBossKAGU2MUHENKDraw(effPCPBossKAGU2MUHENK_t *pPCPBossKAGU2MUHENK);

void effPCPBossKAGU2MUHENKSetColor(effPCPBossKAGU2MUHENK_t *hData, u32 abgr);

effPCPCrack_t *effPCPCrack2Create(effPCPCrackParam_t *pParam, void *pPB);

effPCPCrack_t *effPCPCrack2CreatePCF(void *data);

effPCPCrack_t *effPCPCrack2Copy(effPCPCrack_t *src);

void effPCPCrack2Delete(effPCPCrack_t *hData);

void effPCPCrack2Draw(effPCPCrack_t *hData);

void effPCPCrack2SetPosition(effPCPCrack_t *hData, const void *pPos);

void effPCPCrack2SetColor(effPCPCrack_t *hData, u32 abgr);

effPCPBossMETATOBeam_t *effPCPBossMETATOBeamCreate(void *pParam);

effPCPBossMETATOBeam_t *effPCPBossMETATOBeamCreatePCF(void *hData, s32 bPos, s32 tbldata);

effPCPBossMETATOBeam_t *effPCPBossMETATOBeamCopy(void *hData, s32 bPos, s32 tbldata);

void effPCPBossMETATOBeamDelete(effPCPBossMETATOBeam_t *hData);

void effPCPBossMETATOBeamSetPosition(effPCPBossMETATOBeam_t *hData, const void *pPos);

void effPCPBossMETATOBeamSetColor(effPCPBossMETATOBeam_t *hData, u32 abgr);

void effPCPScreenCrackModelInit(effPCPScreenCrackModel_t *pPCPScrCrackModel);

effPCPScreenCrackModel_t *effPCPScreenCrackModelCreate(effPCPScreenCrackModel_t *pParam, void *pPB);

effPCPScreenCrackModel_t *effPCPScreenCrackModelCreatePCF(void *parameterTable);

void effPCPScreenCrackModelDelete(effPCPScreenCrackModel_t *hData);

void effPCPScreenCrackModelDraw(effPCPScreenCrackModel_t *hData);

void effPCPScreenCrackModelSetColor(effPCPScreenCrackModel_t *hData, u32 abgr);

effPCPMuzzleFlash_tag *effPCPMuzzleFlashCreate(effPCPMuzzleFlashParam_t *pParam, void *pP2A);

effPCPMuzzleFlash_tag *effPCPMuzzleFlashCreatePCF(void *parameterTable);

effPCPMuzzleFlash_tag *effPCPMuzzleFlashCopy(effPCPMuzzleFlash_tag *hData, s32 bPos, s32 tbldata);

void effPCPMuzzleFlashDelete(effPCPMuzzleFlash_tag *hData);

void effPCPMuzzleFlashDraw(effPCPMuzzleFlash_tag *hData);

void effPCPMuzzleFlashSetColor(effPCPMuzzleFlash_tag *hData, u32 abgr);

void effPCPMuzzleFlashSetPosition(effPCPMuzzleFlash_tag *hData, const void *pPos);

void effPCPMuzzleFlashSetScale(effPCPMuzzleFlash_tag *hData, f32 scale);

/* vu0 routine: copy the rotation matrix through vf28-vf31 */
void effPCPMuzzleFlashSetRotateMatrix(effPCPMuzzleFlash_tag *hData, const void *pRot);

/* vu0 routine: the DANTE lightning bolt's work and its draw process. */
effPCPDANTE_Thunder_tag *effPCPDANTE_Thunder_Create(const effPCPDANTE_Thunder_Param_t *pParam);

extern void effPCPDANTE_Thunder_Draw(effPCPDANTE_Thunder_tag *hData);

s32 effPCPDANTE_Thunder_ProcessDraw(dds3ProcessID_t *process);

effPCPDANTE_Thunder_tag *effPCPDANTE_Thunder_CreatePCF(void *hData, s32 bPos, s32 tbldata);

void effPCPDANTE_Thunder_Delete(effPCPDANTE_Thunder_tag *hData);

effPCPDANTE_Thunder_tag *effPCPDANTE_Thunder_Copy(effPCPDANTE_Thunder_tag *hData, s32 bPos, s32 tbldata);

void effPCPDANTE_Thunder_DrawSub(effPCPDANTE_Thunder_tag *hData);

void effPCPDANTE_Thunder_Draw(effPCPDANTE_Thunder_tag *hData);

void effPCPDANTE_Thunder_SetColor(effPCPDANTE_Thunder_tag *hData, u32 abgr);

void effPCPDANTE_Thunder_SetPosition(effPCPDANTE_Thunder_tag *hData, const void *pPos);

/* vu0 routine: copy the rotation matrix through vf28-vf31 */
void effPCPDANTE_Thunder_SetRotateMatrix(effPCPDANTE_Thunder_tag *hData, const void *pRot);

/* The local held-byte register declaration reconstructs native allocation;
 * a target-specific compiler aid, not evidence of original source spelling. */
effPCPDANTE_Damage_tag *effPCPDANTE_Damage_Create(void *pParam, void *pBED);

effPCPDANTE_Damage_tag *effPCPDANTE_Damage_CreatePCF(void *parameterTable);

effPCPDANTE_Damage_tag *effPCPDANTE_Damage_Copy(effPCPDANTE_Damage_tag *hData, s32 bPos, s32 tbldata);

void effPCPDANTE_Damage_Delete(effPCPDANTE_Damage_tag *hData);

extern void nbSetUnitMotion(nbFormation_t *form, s32 motion, s32 mode, f32 rate);

extern f32 effBTLPartGetScalePtr(nbFormation_t *form);

void effPCPDANTE_Damage_Run(effPCPDANTE_Damage_tag *work);

void effPCPDANTE_Damage_SetColor(effPCPDANTE_Damage_tag *hData, u32 abgr);

effPCP_PBCamera_tag *effPCP_PBCamera_Create(effPCP_PBCamera_Param_t *pParam, void *pPB);

effPCP_PBCamera_tag *effPCP_PBCamera_CreatePCF(void *parameterTable);

effPCP_PBCamera_tag *effPCP_PBCamera_Copy(effPCP_PBCamera_tag *hData, s32 bPos, s32 tbldata);

void effPCP_PBCamera_Delete(effPCP_PBCamera_tag *hData);

extern s32 effBTLSkillGetID(void);

extern void mdlGetPosition(mdlObj_t *model);

extern void effPCP_PBCamera_SetPosition(effPCP_PBCamera_tag *work, const void *position);

extern s32 nbCameraDANTE_KAGIRI_Set(s32 frame);

extern void nbCameraSetState(u16 state);

extern void nbCameraHelperModelSetSub(mdlObj_t *model, u32 from, u32 to);

/* nbCameraProcess camera prefix; D_00320E64 is its fovy at +0x24. */
typedef struct effPCPBossCamera_t {
    sdfVector4_t target;
    sdfVector4_t direction;
    f32 distance;
    f32 fovy;
    u32 state_change;
    u32 state;
} effPCPBossCamera_t;

extern effPCPBossCamera_t pNbCamera;

void effPCP_PBCamera_Run(effPCP_PBCamera_tag *work);

void effPCP_PBCamera_SetPosition(effPCP_PBCamera_tag *hData, const void *pPos);

/* vu0 routine: copy the rotation matrix through vf28-vf31 */
void effPCP_PBCamera_SetRotateMatrix(effPCP_PBCamera_tag *hData, const void *pMatrix);

effPCPDANTE_ShowTime_tag *effPCPDANTE_ShowTime_Create(effPCPDANTE_ShowTime_Param_t *pParam,
                                                    void *pTMX0, void *pTMX1);

effPCPDANTE_ShowTime_tag *effPCPDANTE_ShowTime_CreatePCF(void *parameterTable);

effPCPDANTE_ShowTime_tag *effPCPDANTE_ShowTime_Copy(effPCPDANTE_ShowTime_tag *hData, s32 bPos, s32 tbldata);

void effPCPDANTE_ShowTime_Delete(effPCPDANTE_ShowTime_tag *hData);

void effPCPDANTE_Sprite_Draw(s32 x, s32 y, f32 scale, u32 color, effPCPDANTE_UV_tag *uv, void *tex);

extern effPCPDANTE_UV_tag effPCPDANTE_Bingo_UV;

extern void nbPnl_rsetAlpha(s32 mode);

extern effPCPDANTE_UV_tag effPCPDANTE_AreYouReady_UV;

void effPCPDANTE_ShowTime_AreYouReady_Draw(effPCPDANTE_ShowTime_tag *hData);

extern effPCPDANTE_UV_tag effPCPDANTE_ShowTime_UV;

void effPCPDANTE_ShowTime_ShowTime_Draw(effPCPDANTE_ShowTime_tag *work);

void effPCPDANTE_ShowTime_Bingo_Draw(effPCPDANTE_ShowTime_tag *work);

void effPCPDANTE_ShowTime_Draw(effPCPDANTE_ShowTime_tag *hData);

void effPCPDANTE_ShowTime_SetPosition(effPCPDANTE_ShowTime_tag *hData, const void *pPos);

effPCP_BED_tag *effPCP_BED_CreatePCF(void *parameterTable);

effPCP_BED_tag *effPCP_BED_Copy(effPCP_BED_tag *hData, s32 bPos, s32 tbldata);

void effPCP_BED_Delete(effPCP_BED_tag *hData);

void effPCP_BED_MatrixToQuaternion(f32 matrix[4][4]);

void effPCP_BED_Draw(effPCP_BED_tag *hData);

void effPCP_BED_SetPosition(effPCP_BED_tag *hData, const void *pPos);

/* vu0 routine: copy the rotation matrix through vf28-vf31 */
void effPCP_BED_SetRotateMatrix(effPCP_BED_tag *hData, const void *pMatrix);

void effPCP_BED_SetColor(effPCP_BED_tag *hData, u32 abgr);

effPCP_HBED2_tag *effPCP_HBED2_Create(effPCP_HBED2_Param_t *pParam, void *pPBData, void *pBEDData);

effPCP_HBED2_tag *effPCP_HBED2_CreatePCF(void *parameterTable);

effPCP_HBED2_tag *effPCP_HBED2_Copy(effPCP_HBED2_tag *hData, s32 bPos, s32 tbldata);

void effPCP_HBED2_Delete(effPCP_HBED2_tag *hData);

void effPCP_HBED2_Draw(effPCP_HBED2_tag *hData);

void effPCP_HBED2_SetPosition(effPCP_HBED2_tag *hData, const void *pPos);

void effPCP_HBED2_SetRotateMatrix(effPCP_HBED2_tag *hData, const void *pMatrix);

void effPCP_HBED2_SetColor(effPCP_HBED2_tag *hData, u32 abgr);

void effPCP_HBED2_SetScale(effPCP_HBED2_tag *hData, f32 scale);

void effPadVibeFadeUpdate(void);

void effPadVibeFadeOUT(u32 mode, u8 power, s32 frame);

void effPadVibeFadeIN(u32 mode, u8 power, s32 frame);

extern f32 sdfCos(f32 angle);

extern const effPCPBossVector_t D_00533DA0;

extern const f32 D_00533DB0[4];

extern const effPCPBossVector_t D_00533DC0;

extern const f32 D_00533DD0[4];

extern const f32 D_00533DE0[4];

extern const char effPCPDANTE_ThunderProcessName[32];

extern const f32 D_00533E10[8];

extern void *effPCPBossTexHandle;

extern s32 effPCPBossONGYTexRefCount;

extern s32 effPCPBossONGY_RFrame;

extern s32 effPadVibeFrames[2];

extern s32 effPadVibeRemain[2];

extern u8 effPadVibePower[2];

extern u8 effPadVibeFadeIn[2];
/* End of declarations from src/smt3/effect/effPCPBoss.c */

INCLUDE_ASM(const s32, "code_00226EC8", func_00226EC8);

INCLUDE_ASM(const s32, "code_00226EC8", effCreateSmallHeaderFromFile);

INCLUDE_ASM(const s32, "code_00226EC8", func_00226F40);

INCLUDE_ASM(const s32, "code_00226EC8", effCloneSmallHeaderFromWork);

INCLUDE_ASM(const s32, "code_00226EC8", func_00226F88);

INCLUDE_ASM(const s32, "code_00226EC8", func_00226F90);

INCLUDE_ASM(const s32, "code_00226EC8", effCreateFadeVectorWork);

INCLUDE_ASM(const s32, "code_00226EC8", effCreateFadeVectorFromFile);

INCLUDE_ASM(const s32, "code_00226EC8", effFreeFadeVectorWork);

INCLUDE_ASM(const s32, "code_00226EC8", effCloneFadeVectorWork);

INCLUDE_ASM(const s32, "code_00226EC8", func_00227128);

INCLUDE_ASM(const s32, "code_00226EC8", effDrawLensFlare);

INCLUDE_ASM(const s32, "code_00226EC8", func_002277C8);

INCLUDE_ASM(const s32, "code_00226EC8", func_002277D8);

INCLUDE_ASM(const s32, "code_00226EC8", effResetSelectionEntryBuffers);

INCLUDE_ASM(const s32, "code_00226EC8", effCreateSelectionFlagListFromWork);

INCLUDE_ASM(const s32, "code_00226EC8", effCreateSelectionFlagListFromFile);

INCLUDE_ASM(const s32, "code_00226EC8", effReleaseSelectionFlagList);

INCLUDE_ASM(const s32, "code_00226EC8", effCreateEmbeddedSelectionFlagList);

INCLUDE_ASM(const s32, "code_00226EC8", effResetSelectionEntriesAndState);

INCLUDE_ASM(const s32, "code_00226EC8", effUpdateSelectionEntryState);

INCLUDE_ASM(const s32, "code_00226EC8", effDrawSelectionEntryVectors);

INCLUDE_ASM(const s32, "code_00226EC8", func_002278F0);

INCLUDE_ASM(const s32, "code_00226EC8", func_00227930);

INCLUDE_ASM(const s32, "code_00226EC8", effDuplicatePayloadHeader);

INCLUDE_ASM(const s32, "code_00226EC8", effCreateSelectionHeaderFromFile);

INCLUDE_ASM(const s32, "code_00226EC8", effReleaseSelectionHeaderAllocation);

INCLUDE_ASM(const s32, "code_00226EC8", effCloneSelectionHeaderFromWork);

INCLUDE_ASM(const s32, "code_00226EC8", func_002279E8);

INCLUDE_ASM(const s32, "code_00226EC8", effSelectionFrameAdvance);

INCLUDE_ASM(const s32, "code_00226EC8", effUpdateFadeBlendA);

INCLUDE_ASM(const s32, "code_00226EC8", effCreateFadeBlendWorkFromOutput);

INCLUDE_ASM(const s32, "code_00226EC8", effReleaseFadeBlendWork);

INCLUDE_ASM(const s32, "code_00226EC8", effUpdateProjectedBlurFadeRectangle);

INCLUDE_ASM(const s32, "code_00226EC8", effUpdateTarget);

INCLUDE_ASM(const s32, "code_00226EC8", effCreateFixedSlotBlurWorkFromFadeOutput);

INCLUDE_ASM(const s32, "code_00226EC8", effReleaseFixedSlotBlurWork);

INCLUDE_ASM(const s32, "code_00226EC8", effUpdateFadeMapA);

INCLUDE_ASM(const s32, "code_00226EC8", effTextureReferenceRelease);

INCLUDE_ASM(const s32, "code_00226EC8", effCreateVariableSlotBlurWorkFromFadeOutput);

INCLUDE_ASM(const s32, "code_00226EC8", effReleaseVariableSlotBlurWork);

INCLUDE_ASM(const s32, "code_00226EC8", effUpdateFadeMapB);

INCLUDE_ASM(const s32, "code_00226EC8", effReplaceKindLinkedTarget);

INCLUDE_ASM(const s32, "code_00226EC8", effUpdateFadeBlendB);

INCLUDE_ASM(const s32, "code_00226EC8", effUpdateFadeBlendC);

INCLUDE_ASM(const s32, "code_00226EC8", effCreateFadeColorWorkFromOutput);

INCLUDE_ASM(const s32, "code_00226EC8", effReleaseFadeColorWork);

INCLUDE_ASM(const s32, "code_00226EC8", effUpdateProjectedFadeColor);

INCLUDE_ASM(const s32, "code_00226EC8", effReplaceLinkedKindWorkTarget);

INCLUDE_ASM(const s32, "code_00226EC8", effAllocateKindWork);

INCLUDE_ASM(const s32, "code_00226EC8", effCreateKindWorkFromFile);

INCLUDE_ASM(const s32, "code_00226EC8", effReleaseLinkedTarget);

INCLUDE_ASM(const s32, "code_00226EC8", effCloneKindWork);

INCLUDE_ASM(const s32, "code_00226EC8", func_00228A78);

INCLUDE_ASM(const s32, "code_00226EC8", func_00228A80);

INCLUDE_ASM(const s32, "code_00226EC8", func_00228AE8);

INCLUDE_ASM(const s32, "code_00226EC8", func_00228AF8);

INCLUDE_ASM(const s32, "code_00226EC8", func_00228B00);

INCLUDE_ASM(const s32, "code_00226EC8", effAllocateAlternateKindWork);

INCLUDE_ASM(const s32, "code_00226EC8", effCreateKindWorkFromFileB);

INCLUDE_ASM(const s32, "code_00226EC8", effReleaseAlternateKindWork);

INCLUDE_ASM(const s32, "code_00226EC8", effCloneAlternateKindWork);

INCLUDE_ASM(const s32, "code_00226EC8", func_00228DB0);

INCLUDE_ASM(const s32, "code_00226EC8", func_00228DB8);

INCLUDE_ASM(const s32, "code_00226EC8", func_00228E20);

INCLUDE_ASM(const s32, "code_00226EC8", func_00228E30);

INCLUDE_ASM(const s32, "code_00226EC8", func_00228E38);

INCLUDE_ASM(const s32, "code_00226EC8", effCreateBillboardWork);

INCLUDE_ASM(const s32, "code_00226EC8", effBillboardWorkRelease);

INCLUDE_ASM(const s32, "code_00226EC8", effDuplicateBillState);

INCLUDE_ASM(const s32, "code_00226EC8", effReplaceBillboardClone);

INCLUDE_ASM(const s32, "code_00226EC8", effBillboardEntryFrameReset);

INCLUDE_ASM(const s32, "code_00226EC8", func_00229060);

INCLUDE_ASM(const s32, "code_00226EC8", func_00229168);

INCLUDE_ASM(const s32, "code_00226EC8", func_00229178);

INCLUDE_ASM(const s32, "code_00226EC8", func_00229190);

INCLUDE_ASM(const s32, "code_00226EC8", func_00229198);

INCLUDE_ASM(const s32, "code_00226EC8", effClearBillFrames);

INCLUDE_ASM(const s32, "code_00226EC8", effCreateBillFrameNode);

INCLUDE_ASM(const s32, "code_00226EC8", effReleaseBillFrameNode);

INCLUDE_ASM(const s32, "code_00226EC8", billAdvanceFrameInstances);

INCLUDE_ASM(const s32, "code_00226EC8", billUpdateFrameDrawColorAndTransform);

INCLUDE_ASM(const s32, "code_00226EC8", billResetCellIndices);

INCLUDE_ASM(const s32, "code_00226EC8", billCreateCellNode);

INCLUDE_ASM(const s32, "code_00226EC8", billReleaseCellNode);

INCLUDE_ASM(const s32, "code_00226EC8", billAdvanceCellInstances);

INCLUDE_ASM(const s32, "code_00226EC8", billUpdateCellDrawColorAndTransform);

INCLUDE_ASM(const s32, "code_00226EC8", billResetParticleIndices);

INCLUDE_ASM(const s32, "code_00226EC8", billCreateParticleNode);

INCLUDE_ASM(const s32, "code_00226EC8", billReleaseParticleNode);

INCLUDE_ASM(const s32, "code_00226EC8", billAdvanceParticleInstances);

INCLUDE_ASM(const s32, "code_00226EC8", billUpdateParticleDrawColorAndTransform);

INCLUDE_ASM(const s32, "code_00226EC8", effClearAnimatedFrames);

INCLUDE_ASM(const s32, "code_00226EC8", billAllocateAnimatedTransformEntries);

INCLUDE_ASM(const s32, "code_00226EC8", effInitializeAlternatingTransformRows);

INCLUDE_ASM(const s32, "code_00226EC8", billCreateAnimatedTransform);

INCLUDE_ASM(const s32, "code_00226EC8", billCloneAnimatedTransform);

INCLUDE_ASM(const s32, "code_00226EC8", billReleaseAlternatingTransformNode);

INCLUDE_ASM(const s32, "code_00226EC8", billAdvanceAnimatedFrameTransforms);

INCLUDE_ASM(const s32, "code_00226EC8", billUpdateAlternatingDrawColorAndTransform);

INCLUDE_ASM(const s32, "code_00226EC8", billResetEmitterIndices);

INCLUDE_ASM(const s32, "code_00226EC8", billAllocEmitterNode);

INCLUDE_ASM(const s32, "code_00226EC8", billInitializeEmitterRows);

INCLUDE_ASM(const s32, "code_00226EC8", billCreateEmitterTransform);

INCLUDE_ASM(const s32, "code_00226EC8", billCloneEmitterTransform);

INCLUDE_ASM(const s32, "code_00226EC8", billReleaseEmitterNode);

INCLUDE_ASM(const s32, "code_00226EC8", billAdvanceEmitterInstances);

INCLUDE_ASM(const s32, "code_00226EC8", billUpdateEmitterDrawColorAndTransform);

INCLUDE_ASM(const s32, "code_00226EC8", effClearStripFrames);

INCLUDE_ASM(const s32, "code_00226EC8", billAllocStripNode);

INCLUDE_ASM(const s32, "code_00226EC8", billInitializeStripRows);

INCLUDE_ASM(const s32, "code_00226EC8", billCreateStripTransform);

INCLUDE_ASM(const s32, "code_00226EC8", billCloneStripTransform);

INCLUDE_ASM(const s32, "code_00226EC8", billReleaseStripNode);

INCLUDE_ASM(const s32, "code_00226EC8", billAdvanceStripInstances);

INCLUDE_ASM(const s32, "code_00226EC8", billUpdateStripDrawColorAndTransform);

INCLUDE_ASM(const s32, "code_00226EC8", billResetTrailIndices);

INCLUDE_ASM(const s32, "code_00226EC8", billCreateTrailNode);

INCLUDE_ASM(const s32, "code_00226EC8", billReleaseTrailNode);

INCLUDE_ASM(const s32, "code_00226EC8", billAdvanceTrailInstances);

INCLUDE_ASM(const s32, "code_00226EC8", billUpdateTrailDrawColorAndTransform);

INCLUDE_ASM(const s32, "code_00226EC8", billResetQuadIndices);

INCLUDE_ASM(const s32, "code_00226EC8", billAllocQuadNode);

INCLUDE_ASM(const s32, "code_00226EC8", billInitializeQuadRows);

INCLUDE_ASM(const s32, "code_00226EC8", billCreateQuadTransform);

INCLUDE_ASM(const s32, "code_00226EC8", billCloneQuadTransform);

INCLUDE_ASM(const s32, "code_00226EC8", billReleaseQuadNode);

INCLUDE_ASM(const s32, "code_00226EC8", billAdvanceQuadInstances);

INCLUDE_ASM(const s32, "code_00226EC8", billUpdateQuadDrawColorAndTransform);

INCLUDE_ASM(const s32, "code_00226EC8", effAllocateActiveInstanceWork);

INCLUDE_ASM(const s32, "code_00226EC8", effCreateResourceInstanceA);

INCLUDE_ASM(const s32, "code_00226EC8", effCreateActiveInstanceFromFile);

INCLUDE_ASM(const s32, "code_00226EC8", effDestroyActiveInstanceWork);

INCLUDE_ASM(const s32, "code_00226EC8", effCreateActiveResource);

INCLUDE_ASM(const s32, "code_00226EC8", effResetActiveInstanceFrame);

INCLUDE_ASM(const s32, "code_00226EC8", effAdvanceActiveInstanceFrame);

INCLUDE_ASM(const s32, "code_00226EC8", effDispatchActiveInstanceDraw);

INCLUDE_ASM(const s32, "code_00226EC8", func_0022E420);

INCLUDE_ASM(const s32, "code_00226EC8", func_0022E460);

INCLUDE_ASM(const s32, "code_00226EC8", func_0022E470);

INCLUDE_ASM(const s32, "code_00226EC8", func_0022E488);

INCLUDE_ASM(const s32, "code_00226EC8", func_0022E490);

INCLUDE_RODATA(const s32, "code_00226EC8", D_00455D70);

INCLUDE_ASM(const s32, "code_00226EC8", effCreateTrackSet);

INCLUDE_ASM(const s32, "code_00226EC8", effCreateTrackSetWithSharedReferences);

INCLUDE_ASM(const s32, "code_00226EC8", effReleaseResourceRefs);

INCLUDE_ASM(const s32, "code_00226EC8", effDuplicateResourceRefs);

INCLUDE_ASM(const s32, "code_00226EC8", func_0022E868);

INCLUDE_ASM(const s32, "code_00226EC8", effLoadFlashTextures);

INCLUDE_ASM(const s32, "code_00226EC8", func_0022EDD0);

INCLUDE_ASM(const s32, "code_00226EC8", func_0022EDE8);

INCLUDE_ASM(const s32, "code_00226EC8", effSegmentPointerSet);

INCLUDE_ASM(const s32, "code_00226EC8", effReleaseRingResourceHandle);

INCLUDE_ASM(const s32, "code_00226EC8", effUpdateExpandingRingPoints);

INCLUDE_ASM(const s32, "code_00226EC8", billDrawCellBlendA);

INCLUDE_ASM(const s32, "code_00226EC8", effResetClassFrameAndFlags);

INCLUDE_ASM(const s32, "code_00226EC8", effCreateScaledClassDrawState);

INCLUDE_ASM(const s32, "code_00226EC8", effReleaseClassDrawResources);

INCLUDE_ASM(const s32, "code_00226EC8", effUpdateScaledClassFrameRows);

INCLUDE_ASM(const s32, "code_00226EC8", billDrawClassUpdatedCellBlend);

INCLUDE_ASM(const s32, "code_00226EC8", func_0022F840);

INCLUDE_ASM(const s32, "code_00226EC8", effCreateRingHandle);

INCLUDE_ASM(const s32, "code_00226EC8", effReleaseRingHandle);

INCLUDE_ASM(const s32, "code_00226EC8", effUpdateCenteredRadialRingVertices);

INCLUDE_ASM(const s32, "code_00226EC8", billDrawCellBlendB);

INCLUDE_ASM(const s32, "code_00226EC8", effCreateClassWork);

INCLUDE_ASM(const s32, "code_00226EC8", effCreateClassWorkFromFile);

INCLUDE_ASM(const s32, "code_00226EC8", effDestroyClassWork);

INCLUDE_ASM(const s32, "code_00226EC8", effCloneClassWork);

INCLUDE_ASM(const s32, "code_00226EC8", effInitializeClassFrame);

INCLUDE_ASM(const s32, "code_00226EC8", func_0022FEB0);

INCLUDE_ASM(const s32, "code_00226EC8", effRunClassPostFrame);

INCLUDE_ASM(const s32, "code_00226EC8", effUpdateClassFrame);

INCLUDE_ASM(const s32, "code_00226EC8", func_0022FF78);

INCLUDE_ASM(const s32, "code_00226EC8", func_0022FF88);

INCLUDE_ASM(const s32, "code_00226EC8", func_0022FFA0);

INCLUDE_ASM(const s32, "code_00226EC8", func_0022FFA8);

INCLUDE_ASM(const s32, "code_00226EC8", effCreatePointSet4);

INCLUDE_ASM(const s32, "code_00226EC8", effAssetQueueRelease);

INCLUDE_ASM(const s32, "code_00226EC8", func_002300B0);

INCLUDE_ASM(const s32, "code_00226EC8", effCreateSurfaceNode);

INCLUDE_ASM(const s32, "code_00226EC8", effCreateSurfaceNodeForGrid);

INCLUDE_ASM(const s32, "code_00226EC8", effResourceReferenceReplaceFromFile);

INCLUDE_ASM(const s32, "code_00226EC8", effInitializeSurfaceForKind);

INCLUDE_ASM(const s32, "code_00226EC8", effDestroySurfaceNode);

INCLUDE_ASM(const s32, "code_00226EC8", effRecreateSurfaceNodeFromWork);

INCLUDE_ASM(const s32, "code_00226EC8", effCreateSurfaceGridWithConfiguration);

INCLUDE_ASM(const s32, "code_00226EC8", effCloneSurfaceGridResources);

INCLUDE_ASM(const s32, "code_00226EC8", effRebuildSurfaceHandles);

INCLUDE_ASM(const s32, "code_00226EC8", effReplaceResourceRef);

INCLUDE_ASM(const s32, "code_00226EC8", effSetSurfaceRetainedResource);

INCLUDE_ASM(const s32, "code_00226EC8", effReplaceSurfacePrimaryBillboard);

INCLUDE_ASM(const s32, "code_00226EC8", effReplaceSurfaceFlaggedBillboard);

INCLUDE_ASM(const s32, "code_00226EC8", effRebuildSurfaceJobs);

INCLUDE_ASM(const s32, "code_00226EC8", effReplaceSurfaceResourceHolder);

INCLUDE_ASM(const s32, "code_00226EC8", effClearSurfaceNodeRecordReferences);

INCLUDE_ASM(const s32, "code_00226EC8", effAcquireSurfaceRecord);

INCLUDE_ASM(const s32, "code_00226EC8", func_00230EB0);

INCLUDE_ASM(const s32, "code_00226EC8", func_00231B70);

INCLUDE_ASM(const s32, "code_00226EC8", effMenuRecordVectorSet);

INCLUDE_ASM(const s32, "code_00226EC8", effSetSurfaceRecordSecondaryVector);

INCLUDE_ASM(const s32, "code_00226EC8", func_00231BE0);

INCLUDE_ASM(const s32, "code_00226EC8", effIndexedFloatCallbackDispatch);

INCLUDE_ASM(const s32, "code_00226EC8", func_00231C08);

INCLUDE_ASM(const s32, "code_00226EC8", effFillSurfaceGridColorGradient);

INCLUDE_ASM(const s32, "code_00226EC8", effReleaseSurfaceGridBuffers);

INCLUDE_ASM(const s32, "code_00226EC8", effResetSurfaceGridFrame);

INCLUDE_ASM(const s32, "code_00226EC8", effCopyRingFrameVectors);

INCLUDE_ASM(const s32, "code_00226EC8", effAppendSurfaceGridVectors);

INCLUDE_ASM(const s32, "code_00226EC8", effInterpolateSurfaceGridVectors);

INCLUDE_ASM(const s32, "code_00226EC8", effBeginMatrixVuDrawPacket);

INCLUDE_ASM(const s32, "code_00226EC8", effDrawSurfaceTrailHistory);

INCLUDE_ASM(const s32, "code_00226EC8", effSubmitIndexedRenderPacket);

INCLUDE_ASM(const s32, "code_00226EC8", func_00232638);

INCLUDE_ASM(const s32, "code_00226EC8", func_00232BD8);

INCLUDE_ASM(const s32, "code_00226EC8", func_00232C38);

INCLUDE_ASM(const s32, "code_00226EC8", func_00232E30);

INCLUDE_ASM(const s32, "code_00226EC8", func_00232F08);

INCLUDE_ASM(const s32, "code_00226EC8", func_00232F10);

INCLUDE_ASM(const s32, "code_00226EC8", func_00232F48);

INCLUDE_ASM(const s32, "code_00226EC8", func_00232F58);

INCLUDE_ASM(const s32, "code_00226EC8", func_00232F70);

INCLUDE_ASM(const s32, "code_00226EC8", func_00232F78);

INCLUDE_ASM(const s32, "code_00226EC8", func_00232F80);

INCLUDE_ASM(const s32, "code_00226EC8", func_00233AA0);

INCLUDE_ASM(const s32, "code_00226EC8", func_00233AD8);

INCLUDE_ASM(const s32, "code_00226EC8", effRandomizeParticleFields);

INCLUDE_ASM(const s32, "code_00226EC8", effCreateAlphaRampPointSetRows);

INCLUDE_ASM(const s32, "code_00226EC8", effFreeIndexedEntries);

INCLUDE_ASM(const s32, "code_00226EC8", effUpdateRadialWavePointRows);

INCLUDE_ASM(const s32, "code_00226EC8", effDrawTintedFivePointGroupRows);

INCLUDE_ASM(const s32, "code_00226EC8", effResetRadialClassResourceFrames);

INCLUDE_ASM(const s32, "code_00226EC8", effCreateRadialClassResourceEntries);

INCLUDE_ASM(const s32, "code_00226EC8", effDestroyRadialClassResourceEntries);

INCLUDE_ASM(const s32, "code_00226EC8", effAdvanceRadialClassResourceFrames);

INCLUDE_ASM(const s32, "code_00226EC8", effUpdateRadialClassInstances);

INCLUDE_ASM(const s32, "code_00226EC8", effSeedBillScaleRange);

INCLUDE_ASM(const s32, "code_00226EC8", effCreateRetainedPointSetColorRows);

INCLUDE_ASM(const s32, "code_00226EC8", effReleaseBillPointEntries);

INCLUDE_ASM(const s32, "code_00226EC8", effUpdateOrbitalPointSpanWaves);

INCLUDE_ASM(const s32, "code_00226EC8", effDrawRetainedPointSetRows);

INCLUDE_ASM(const s32, "code_00226EC8", effCreateClassResourceWork);

INCLUDE_ASM(const s32, "code_00226EC8", effCreateClassResourceFromFile);

INCLUDE_ASM(const s32, "code_00226EC8", effDestroyClassResourceWork);

INCLUDE_ASM(const s32, "code_00226EC8", effCloneClassResourceWork);

INCLUDE_ASM(const s32, "code_00226EC8", effResetDispatchCounter);

INCLUDE_ASM(const s32, "code_00226EC8", effAdvanceClassResourceFrame);

INCLUDE_ASM(const s32, "code_00226EC8", effDrawClassResourceWork);

INCLUDE_ASM(const s32, "code_00226EC8", func_00236618);

INCLUDE_ASM(const s32, "code_00226EC8", effCopyClassResourcePosition);

void effPCPBossBaelSetPosition(effPCPBossBael_t *hData, const void *pPos)
{
    SDF_VU0_COPYVECTOR(&hData->param, pPos);
}

INCLUDE_ASM(const s32, "code_00226EC8", effSetClassResourceColor);

INCLUDE_ASM(const s32, "code_00226EC8", effSetClassResourceMatrixComponent);

INCLUDE_ASM(const s32, "code_00226EC8", effCreatePointSet5);

INCLUDE_ASM(const s32, "code_00226EC8", effReleasePointSetAsset);

INCLUDE_ASM(const s32, "code_00226EC8", effDrawFivePointGroups);

INCLUDE_ASM(const s32, "code_00226EC8", effCreateStripNode);

INCLUDE_ASM(const s32, "code_00226EC8", effCreateStripNodeFromGrid);

INCLUDE_ASM(const s32, "code_00226EC8", effFileResourceReferenceReplace);

INCLUDE_ASM(const s32, "code_00226EC8", effReleaseModelResources);

INCLUDE_ASM(const s32, "code_00226EC8", effCloneStripResourceFromOwner);

INCLUDE_ASM(const s32, "code_00226EC8", effReplaceFileResourceRef);

INCLUDE_ASM(const s32, "code_00226EC8", effClearStripRecordReferences);

INCLUDE_ASM(const s32, "code_00226EC8", effAcquireStripRecord);

INCLUDE_ASM(const s32, "code_00226EC8", effDrawEffectStrip);

INCLUDE_ASM(const s32, "code_00226EC8", effUpdateAndDrawStripRecord);

INCLUDE_ASM(const s32, "code_00226EC8", effSetStripRecordVector);

INCLUDE_ASM(const s32, "code_00226EC8", effSetStripRecordSecondaryVector);

INCLUDE_ASM(const s32, "code_00226EC8", func_00237470);

INCLUDE_ASM(const s32, "code_00226EC8", effSetStripOpacity);

INCLUDE_ASM(const s32, "code_00226EC8", effResetBillTable);

INCLUDE_ASM(const s32, "code_00226EC8", effAllocateRingFadeEntries);

INCLUDE_ASM(const s32, "code_00226EC8", effFillRingFadeGradient);

INCLUDE_ASM(const s32, "code_00226EC8", effCreateRingFadeTable);

INCLUDE_ASM(const s32, "code_00226EC8", effAssetPointerSet);

INCLUDE_ASM(const s32, "code_00226EC8", effReleaseRingFadeTable);

INCLUDE_ASM(const s32, "code_00226EC8", func_00237890);

INCLUDE_ASM(const s32, "code_00226EC8", effBlendBillboardInstanceColorsAndTransforms);

INCLUDE_ASM(const s32, "code_00226EC8", func_002380F8);

INCLUDE_ASM(const s32, "code_00226EC8", effAllocateBillFadeFrameEntries);

INCLUDE_ASM(const s32, "code_00226EC8", effFillBillFadeGradient);

INCLUDE_ASM(const s32, "code_00226EC8", effCreateBillFadeTable);

INCLUDE_ASM(const s32, "code_00226EC8", effCloneBillFadeTable);

INCLUDE_ASM(const s32, "code_00226EC8", effReleaseBillFadeTable);

INCLUDE_ASM(const s32, "code_00226EC8", effUpdateColumnParticlesAndRibbons);

INCLUDE_ASM(const s32, "code_00226EC8", effBillBlendCellColorAndUpdateTransform);

INCLUDE_ASM(const s32, "code_00226EC8", func_00238D68);

INCLUDE_ASM(const s32, "code_00226EC8", effAllocateCompactRingFadeEntries);

INCLUDE_ASM(const s32, "code_00226EC8", effFillCompactRingFadeGradient);

INCLUDE_ASM(const s32, "code_00226EC8", effCreateCompactRingFadeTable);

INCLUDE_ASM(const s32, "code_00226EC8", effCloneBillboardFrameAsset);

INCLUDE_ASM(const s32, "code_00226EC8", effReleaseCompactRingFadeTable);

INCLUDE_ASM(const s32, "code_00226EC8", effUpdateBillSpiral);

INCLUDE_ASM(const s32, "code_00226EC8", effUpdateCompactRingDrawColorAndTransform);

INCLUDE_ASM(const s32, "code_00226EC8", func_002398F8);

INCLUDE_ASM(const s32, "code_00226EC8", func_00239948);

INCLUDE_ASM(const s32, "code_00226EC8", func_002399B8);

INCLUDE_ASM(const s32, "code_00226EC8", func_00239C00);

INCLUDE_ASM(const s32, "code_00226EC8", func_00239C60);

INCLUDE_ASM(const s32, "code_00226EC8", func_00239CC0);

INCLUDE_ASM(const s32, "code_00226EC8", func_00239CF0);

INCLUDE_ASM(const s32, "code_00226EC8", func_0023A3F8);

INCLUDE_ASM(const s32, "code_00226EC8", effAllocateBlock);

INCLUDE_ASM(const s32, "code_00226EC8", effCreateResourceInstanceB);

INCLUDE_ASM(const s32, "code_00226EC8", effCreateBlockResourceFromFile);

INCLUDE_ASM(const s32, "code_00226EC8", effDestroyBlockResourceWork);

INCLUDE_ASM(const s32, "code_00226EC8", effDuplicateActiveResourceB);

INCLUDE_ASM(const s32, "code_00226EC8", effResetBlockResourceFrame);

INCLUDE_ASM(const s32, "code_00226EC8", effAdvanceBlockResourceFrame);

INCLUDE_ASM(const s32, "code_00226EC8", effDrawBlockResourceWork);

INCLUDE_ASM(const s32, "code_00226EC8", func_0023A8C0);

INCLUDE_ASM(const s32, "code_00226EC8", func_0023A900);

INCLUDE_ASM(const s32, "code_00226EC8", func_0023A910);

INCLUDE_ASM(const s32, "code_00226EC8", func_0023A928);

INCLUDE_ASM(const s32, "code_00226EC8", func_0023A930);

INCLUDE_ASM(const s32, "code_00226EC8", effCreateRibbonWork);

INCLUDE_ASM(const s32, "code_00226EC8", effCreateRibbonWithSharedResource);

INCLUDE_ASM(const s32, "code_00226EC8", effSharedAssetReferenceRelease);

INCLUDE_ASM(const s32, "code_00226EC8", effCloneRibbonWithSharedResource);

INCLUDE_ASM(const s32, "code_00226EC8", effDrawRibbon);

INCLUDE_ASM(const s32, "code_00226EC8", effLoadWindTexture);

INCLUDE_ASM(const s32, "code_00226EC8", effGetWindTextureHandle);

INCLUDE_ASM(const s32, "code_00226EC8", effResetAnimationFrameEntries);

INCLUDE_ASM(const s32, "code_00226EC8", effAllocateAnimationBuffer);

INCLUDE_ASM(const s32, "code_00226EC8", effFillFadeColorRows);

INCLUDE_ASM(const s32, "code_00226EC8", effPrepareTextureAnimation);

INCLUDE_ASM(const s32, "code_00226EC8", effPrepareOwnedTextureAnimation);

INCLUDE_ASM(const s32, "code_00226EC8", effReleaseTextureAnimationWork);

INCLUDE_ASM(const s32, "code_00226EC8", effUpdateBillFlame);

INCLUDE_ASM(const s32, "code_00226EC8", effUpdateTintedScaledMesh);

INCLUDE_ASM(const s32, "code_00226EC8", effInitializeAnimationPositions);

INCLUDE_ASM(const s32, "code_00226EC8", effClampSlotCount);

INCLUDE_ASM(const s32, "code_00226EC8", effCreateAnimationState);

INCLUDE_ASM(const s32, "code_00226EC8", effActivateAnimationState);

INCLUDE_ASM(const s32, "code_00226EC8", effReleaseAnimationFrameResources);

INCLUDE_ASM(const s32, "code_00226EC8", effSynchronizeFileTransform);

INCLUDE_ASM(const s32, "code_00226EC8", func_0023BE28);

INCLUDE_ASM(const s32, "code_00226EC8", effCreatePrimarySlotAnimationState);

INCLUDE_ASM(const s32, "code_00226EC8", effCreateAlternateSlotAnimationState);

INCLUDE_ASM(const s32, "code_00226EC8", func_0023C510);

INCLUDE_ASM(const s32, "code_00226EC8", effAllocateQuantizedBuffer);

INCLUDE_ASM(const s32, "code_00226EC8", effBuildQuantizedTexturedStrip);

INCLUDE_ASM(const s32, "code_00226EC8", effPrepareQuantizedTexture);

INCLUDE_ASM(const s32, "code_00226EC8", effPrepareOwnedQuantizedTexture);

INCLUDE_ASM(const s32, "code_00226EC8", effReleaseBillboardFrameAsset);

INCLUDE_ASM(const s32, "code_00226EC8", func_0023CAC0);

INCLUDE_ASM(const s32, "code_00226EC8", effUpdateFadedMeshTransform);

INCLUDE_ASM(const s32, "code_00226EC8", effAllocateBlockWithModel);

INCLUDE_ASM(const s32, "code_00226EC8", effCreateResourceInstanceC);

INCLUDE_ASM(const s32, "code_00226EC8", effCreateModelBlockFromFile);

INCLUDE_ASM(const s32, "code_00226EC8", effDestroyModelBlockWork);

INCLUDE_ASM(const s32, "code_00226EC8", effRecreateActiveByClass);

INCLUDE_ASM(const s32, "code_00226EC8", effResetModelBlockFrame);

INCLUDE_ASM(const s32, "code_00226EC8", effAdvanceDispatchCounter);

INCLUDE_ASM(const s32, "code_00226EC8", effDrawModelBlock);

INCLUDE_ASM(const s32, "code_00226EC8", func_0023CFE8);

INCLUDE_ASM(const s32, "code_00226EC8", func_0023D028);

INCLUDE_ASM(const s32, "code_00226EC8", func_0023D038);

INCLUDE_ASM(const s32, "code_00226EC8", func_0023D050);

INCLUDE_ASM(const s32, "code_00226EC8", func_0023D058);

INCLUDE_ASM(const s32, "code_00226EC8", effRetainScalyTextureReference);

INCLUDE_ASM(const s32, "code_00226EC8", effReleaseScalyTextureReference);

INCLUDE_ASM(const s32, "code_00226EC8", effAllocateTexturedStripWork);

INCLUDE_ASM(const s32, "code_00226EC8", effCreateTexturedStripWithSharedTexture);

INCLUDE_ASM(const s32, "code_00226EC8", effReleaseScalyStripResources);

INCLUDE_ASM(const s32, "code_00226EC8", effAllocateStripFromWorkAndRetainTexture);

INCLUDE_ASM(const s32, "code_00226EC8", func_0023D2B8);

INCLUDE_ASM(const s32, "code_00226EC8", effLoadScalyTexture);

INCLUDE_ASM(const s32, "code_00226EC8", effGetScalyTextureHandle);

INCLUDE_ASM(const s32, "code_00226EC8", effSeedParticleSpanParameters);

INCLUDE_ASM(const s32, "code_00226EC8", effCreateParticleSpanTable);

INCLUDE_ASM(const s32, "code_00226EC8", effReleaseParticleList);

INCLUDE_ASM(const s32, "code_00226EC8", func_0023DC90);

INCLUDE_ASM(const s32, "code_00226EC8", effDrawParticleSpanPointsAndReferences);

INCLUDE_ASM(const s32, "code_00226EC8", effCreateModelResourceWithInlineData);

INCLUDE_ASM(const s32, "code_00226EC8", effCreateModelResourceFromFile);

INCLUDE_ASM(const s32, "code_00226EC8", effDestroyModelResource);

INCLUDE_ASM(const s32, "code_00226EC8", effCreateModelResource);

INCLUDE_ASM(const s32, "code_00226EC8", effResetModelResourceUpdateCount);

INCLUDE_ASM(const s32, "code_00226EC8", effAdvanceModelResourceUpdateCount);

INCLUDE_ASM(const s32, "code_00226EC8", effDispatchModelResourceCallback);

INCLUDE_ASM(const s32, "code_00226EC8", func_0023EA00);

INCLUDE_ASM(const s32, "code_00226EC8", func_0023EA58);

INCLUDE_ASM(const s32, "code_00226EC8", func_0023EA68);

INCLUDE_ASM(const s32, "code_00226EC8", func_0023EA80);

INCLUDE_ASM(const s32, "code_00226EC8", func_0023EA88);

INCLUDE_ASM(const s32, "code_00226EC8", effCreatePointSet3);

INCLUDE_ASM(const s32, "code_00226EC8", effReleaseModelPointSetAsset);

INCLUDE_ASM(const s32, "code_00226EC8", effDrawThreePointGroups);

INCLUDE_ASM(const s32, "code_00226EC8", effInitializeParticleDirection);

INCLUDE_ASM(const s32, "code_00226EC8", effSetRotationFromAcceleratedFrameAngle);

INCLUDE_ASM(const s32, "code_00226EC8", func_0023EFE0);

INCLUDE_ASM(const s32, "code_00226EC8", effCreateConfiguredSlotNode);

INCLUDE_ASM(const s32, "code_00226EC8", effCreateSlotNodeFromFileJob);

INCLUDE_ASM(const s32, "code_00226EC8", effDestroyOwnedResources);

INCLUDE_ASM(const s32, "code_00226EC8", effCloneOwnedState);

INCLUDE_ASM(const s32, "code_00226EC8", effCloneSlotNodeSecondaryResources);

INCLUDE_ASM(const s32, "code_00226EC8", effRebuildResourceEntries);

INCLUDE_ASM(const s32, "code_00226EC8", effReplaceEffectSlotModelAndDeviceResources);

INCLUDE_ASM(const s32, "code_00226EC8", effRebuildResourceEntryClones);

INCLUDE_ASM(const s32, "code_00226EC8", effClearSurfaceRecordReferences);

INCLUDE_ASM(const s32, "code_00226EC8", func_0023F7F0);

INCLUDE_ASM(const s32, "code_00226EC8", func_0023F8E8);

INCLUDE_ASM(const s32, "code_00226EC8", func_0023FF28);

INCLUDE_ASM(const s32, "code_00226EC8", effSetActiveSlotPrimaryRecordVector);

INCLUDE_ASM(const s32, "code_00226EC8", effSetActiveSlotSecondaryRecordVector);

INCLUDE_ASM(const s32, "code_00226EC8", func_0023FFC0);

INCLUDE_ASM(const s32, "code_00226EC8", effSetActiveSlotOpacity);

INCLUDE_ASM(const s32, "code_00226EC8", effCreateParticleSharedResourceFromFile);

INCLUDE_ASM(const s32, "code_00226EC8", effReleaseParticleResources);

INCLUDE_ASM(const s32, "code_00226EC8", effCloneParticleSharedResource);

INCLUDE_ASM(const s32, "code_00226EC8", effReplaceSharedResource);

INCLUDE_ASM(const s32, "code_00226EC8", func_002403E8);

INCLUDE_ASM(const s32, "code_00226EC8", func_002403F0);

INCLUDE_ASM(const s32, "code_00226EC8", func_00240988);

void func_00240990(void) {
}

INCLUDE_ASM(const s32, "code_00226EC8", func_00240998);

INCLUDE_ASM(const s32, "code_00226EC8", func_002409C8);

INCLUDE_ASM(const s32, "code_00226EC8", func_002409F8);

INCLUDE_ASM(const s32, "code_00226EC8", func_00240B90);

INCLUDE_ASM(const s32, "code_00226EC8", func_00240BA8);

void func_00240C70(void) {
}

INCLUDE_ASM(const s32, "code_00226EC8", func_00240C78);

INCLUDE_ASM(const s32, "code_00226EC8", func_00240D38);

INCLUDE_ASM(const s32, "code_00226EC8", effComputeLightDirectionVU);

INCLUDE_ASM(const s32, "code_00226EC8", func_00240F98);

INCLUDE_ASM(const s32, "code_00226EC8", func_00240FE0);

INCLUDE_ASM(const s32, "code_00226EC8", func_002410B0);

INCLUDE_ASM(const s32, "code_00226EC8", func_002411F8);

INCLUDE_ASM(const s32, "code_00226EC8", func_00241230);

INCLUDE_ASM(const s32, "code_00226EC8", func_002412D8);

INCLUDE_ASM(const s32, "code_00226EC8", func_00241370);

INCLUDE_ASM(const s32, "code_00226EC8", func_00241D20);

INCLUDE_ASM(const s32, "code_00226EC8", func_00241DC8);

void func_00241FD8(void) {
}

INCLUDE_ASM(const s32, "code_00226EC8", func_00241FE0);

INCLUDE_ASM(const s32, "code_00226EC8", func_00241FE8);

INCLUDE_ASM(const s32, "code_00226EC8", effDestroyCopiedEffectPayload);

void func_00242080(void) {
}

void func_00242088(void) {
}

void func_00242090(void) {
}

void func_00242098(void) {
}

void func_002420A0(void) {
}

INCLUDE_ASM(const s32, "code_00226EC8", effResetSlots);

INCLUDE_ASM(const s32, "code_00226EC8", func_002420F0);

INCLUDE_RODATA(const s32, "code_00226EC8", D_00455EE8);

INCLUDE_RODATA(const s32, "code_00226EC8", D_00455F00);

INCLUDE_RODATA(const s32, "code_00226EC8", D_00455F10);

INCLUDE_RODATA(const s32, "code_00226EC8", D_00455F20);

INCLUDE_RODATA(const s32, "code_00226EC8", D_00455F30);

INCLUDE_RODATA(const s32, "code_00226EC8", D_00455F40);

INCLUDE_RODATA(const s32, "code_00226EC8", D_00455F50);

INCLUDE_RODATA(const s32, "code_00226EC8", D_00455F60);

INCLUDE_RODATA(const s32, "code_00226EC8", D_00455F70);

INCLUDE_RODATA(const s32, "code_00226EC8", D_00455F80);

INCLUDE_RODATA(const s32, "code_00226EC8", D_00455F90);

INCLUDE_RODATA(const s32, "code_00226EC8", D_00455FA0);

INCLUDE_RODATA(const s32, "code_00226EC8", D_00455FB0);

INCLUDE_RODATA(const s32, "code_00226EC8", D_00455FC0);

INCLUDE_RODATA(const s32, "code_00226EC8", D_00455FD0);

INCLUDE_RODATA(const s32, "code_00226EC8", D_00455FE0);

INCLUDE_RODATA(const s32, "code_00226EC8", D_00455FF0);

INCLUDE_RODATA(const s32, "code_00226EC8", D_00456000);

INCLUDE_RODATA(const s32, "code_00226EC8", D_00456010);

INCLUDE_RODATA(const s32, "code_00226EC8", D_00456020);

INCLUDE_RODATA(const s32, "code_00226EC8", D_00456030);

INCLUDE_RODATA(const s32, "code_00226EC8", D_00456040);

INCLUDE_RODATA(const s32, "code_00226EC8", D_00456050);

INCLUDE_RODATA(const s32, "code_00226EC8", D_00456060);

INCLUDE_RODATA(const s32, "code_00226EC8", D_00456070);

INCLUDE_RODATA(const s32, "code_00226EC8", D_00456080);

INCLUDE_RODATA(const s32, "code_00226EC8", D_00456090);

INCLUDE_RODATA(const s32, "code_00226EC8", D_004560A0);

INCLUDE_RODATA(const s32, "code_00226EC8", D_004560B0);

INCLUDE_RODATA(const s32, "code_00226EC8", D_004560C0);

INCLUDE_RODATA(const s32, "code_00226EC8", D_004560D0);

INCLUDE_RODATA(const s32, "code_00226EC8", D_004560F0);

INCLUDE_RODATA(const s32, "code_00226EC8", D_00456110);

INCLUDE_RODATA(const s32, "code_00226EC8", D_00456130);

INCLUDE_RODATA(const s32, "code_00226EC8", D_00456150);

INCLUDE_RODATA(const s32, "code_00226EC8", D_00456170);

INCLUDE_RODATA(const s32, "code_00226EC8", D_00456190);

INCLUDE_RODATA(const s32, "code_00226EC8", D_004561B0);

INCLUDE_RODATA(const s32, "code_00226EC8", D_004561D0);

INCLUDE_RODATA(const s32, "code_00226EC8", D_004561F0);

INCLUDE_RODATA(const s32, "code_00226EC8", D_00456210);

INCLUDE_RODATA(const s32, "code_00226EC8", D_00456230);

INCLUDE_RODATA(const s32, "code_00226EC8", D_00456250);

INCLUDE_RODATA(const s32, "code_00226EC8", D_00456270);

INCLUDE_RODATA(const s32, "code_00226EC8", D_00456290);

INCLUDE_RODATA(const s32, "code_00226EC8", D_004562B0);

INCLUDE_RODATA(const s32, "code_00226EC8", D_004562D0);

INCLUDE_RODATA(const s32, "code_00226EC8", D_004562F0);

INCLUDE_RODATA(const s32, "code_00226EC8", D_00456310);

INCLUDE_RODATA(const s32, "code_00226EC8", D_00456330);

INCLUDE_RODATA(const s32, "code_00226EC8", D_00456350);

INCLUDE_RODATA(const s32, "code_00226EC8", D_00456370);

INCLUDE_RODATA(const s32, "code_00226EC8", D_00456390);

INCLUDE_RODATA(const s32, "code_00226EC8", D_004563B0);

INCLUDE_RODATA(const s32, "code_00226EC8", D_004563D0);

INCLUDE_RODATA(const s32, "code_00226EC8", D_004563F0);

INCLUDE_RODATA(const s32, "code_00226EC8", D_00456410);

INCLUDE_RODATA(const s32, "code_00226EC8", D_00456430);

INCLUDE_RODATA(const s32, "code_00226EC8", D_00456450);

INCLUDE_RODATA(const s32, "code_00226EC8", D_00456470);

INCLUDE_RODATA(const s32, "code_00226EC8", D_00456490);

INCLUDE_RODATA(const s32, "code_00226EC8", D_004564B0);

INCLUDE_RODATA(const s32, "code_00226EC8", D_004564D0);

INCLUDE_RODATA(const s32, "code_00226EC8", D_004564F0);

INCLUDE_RODATA(const s32, "code_00226EC8", D_00456518);

INCLUDE_RODATA(const s32, "code_00226EC8", D_00456538);

INCLUDE_RODATA(const s32, "code_00226EC8", D_00456558);

INCLUDE_RODATA(const s32, "code_00226EC8", D_00456578);

INCLUDE_RODATA(const s32, "code_00226EC8", D_00456598);

INCLUDE_RODATA(const s32, "code_00226EC8", D_004565B8);

INCLUDE_RODATA(const s32, "code_00226EC8", D_004565D8);

INCLUDE_RODATA(const s32, "code_00226EC8", D_004565F8);

INCLUDE_RODATA(const s32, "code_00226EC8", D_00456618);

INCLUDE_RODATA(const s32, "code_00226EC8", D_00456638);

INCLUDE_RODATA(const s32, "code_00226EC8", D_00456658);

INCLUDE_RODATA(const s32, "code_00226EC8", D_00456678);

INCLUDE_RODATA(const s32, "code_00226EC8", D_00456698);

INCLUDE_RODATA(const s32, "code_00226EC8", D_004566B8);

INCLUDE_RODATA(const s32, "code_00226EC8", D_004566D8);

INCLUDE_RODATA(const s32, "code_00226EC8", D_004566F8);

INCLUDE_RODATA(const s32, "code_00226EC8", D_00456718);

INCLUDE_RODATA(const s32, "code_00226EC8", D_00456738);

INCLUDE_RODATA(const s32, "code_00226EC8", D_00456758);

INCLUDE_RODATA(const s32, "code_00226EC8", D_00456778);

INCLUDE_RODATA(const s32, "code_00226EC8", D_00456798);

INCLUDE_RODATA(const s32, "code_00226EC8", D_004567B8);

INCLUDE_RODATA(const s32, "code_00226EC8", D_004567D8);

INCLUDE_RODATA(const s32, "code_00226EC8", D_004567F8);

INCLUDE_RODATA(const s32, "code_00226EC8", D_00456818);

INCLUDE_RODATA(const s32, "code_00226EC8", D_00456838);

INCLUDE_RODATA(const s32, "code_00226EC8", D_00456858);

INCLUDE_RODATA(const s32, "code_00226EC8", D_00456878);

INCLUDE_RODATA(const s32, "code_00226EC8", D_00456898);

INCLUDE_RODATA(const s32, "code_00226EC8", D_004568B8);

INCLUDE_RODATA(const s32, "code_00226EC8", D_004568D8);

INCLUDE_RODATA(const s32, "code_00226EC8", D_004568F8);

INCLUDE_RODATA(const s32, "code_00226EC8", D_00456918);

INCLUDE_RODATA(const s32, "code_00226EC8", D_00456938);

INCLUDE_RODATA(const s32, "code_00226EC8", D_00456958);

INCLUDE_RODATA(const s32, "code_00226EC8", D_00456978);

INCLUDE_RODATA(const s32, "code_00226EC8", D_00456998);

INCLUDE_RODATA(const s32, "code_00226EC8", D_004569B8);

INCLUDE_RODATA(const s32, "code_00226EC8", D_004569D8);

INCLUDE_RODATA(const s32, "code_00226EC8", D_004569F8);

INCLUDE_RODATA(const s32, "code_00226EC8", D_00456A18);

INCLUDE_RODATA(const s32, "code_00226EC8", D_00456A38);

INCLUDE_RODATA(const s32, "code_00226EC8", D_00456A58);

INCLUDE_RODATA(const s32, "code_00226EC8", D_00456A78);

INCLUDE_RODATA(const s32, "code_00226EC8", D_00456A98);

INCLUDE_RODATA(const s32, "code_00226EC8", D_00456AB8);

INCLUDE_RODATA(const s32, "code_00226EC8", D_00456AD8);

INCLUDE_RODATA(const s32, "code_00226EC8", D_00456AF8);

INCLUDE_RODATA(const s32, "code_00226EC8", D_00456B18);

INCLUDE_RODATA(const s32, "code_00226EC8", D_00456B38);

INCLUDE_RODATA(const s32, "code_00226EC8", D_00456B58);

INCLUDE_RODATA(const s32, "code_00226EC8", D_00456B78);

INCLUDE_RODATA(const s32, "code_00226EC8", D_00456B98);

INCLUDE_RODATA(const s32, "code_00226EC8", D_00456BB8);

INCLUDE_RODATA(const s32, "code_00226EC8", D_00456BD8);

INCLUDE_RODATA(const s32, "code_00226EC8", D_00456BF8);

INCLUDE_RODATA(const s32, "code_00226EC8", D_00456C18);

INCLUDE_RODATA(const s32, "code_00226EC8", D_00456C38);

INCLUDE_RODATA(const s32, "code_00226EC8", D_00456C48);

INCLUDE_RODATA(const s32, "code_00226EC8", D_00456C58);

INCLUDE_RODATA(const s32, "code_00226EC8", D_00456C68);

INCLUDE_RODATA(const s32, "code_00226EC8", D_00456C78);

INCLUDE_RODATA(const s32, "code_00226EC8", D_00456C88);

INCLUDE_RODATA(const s32, "code_00226EC8", D_00456C98);

INCLUDE_RODATA(const s32, "code_00226EC8", D_00456CA8);

INCLUDE_RODATA(const s32, "code_00226EC8", D_00456CB8);

INCLUDE_RODATA(const s32, "code_00226EC8", D_00456CC8);

INCLUDE_RODATA(const s32, "code_00226EC8", D_00456CD8);

INCLUDE_RODATA(const s32, "code_00226EC8", D_00456CE8);

INCLUDE_RODATA(const s32, "code_00226EC8", D_00456CF8);

INCLUDE_RODATA(const s32, "code_00226EC8", D_00456D08);

INCLUDE_RODATA(const s32, "code_00226EC8", D_00456D18);

INCLUDE_RODATA(const s32, "code_00226EC8", D_00456D28);

INCLUDE_RODATA(const s32, "code_00226EC8", D_00456D38);

INCLUDE_RODATA(const s32, "code_00226EC8", D_00456D48);

INCLUDE_RODATA(const s32, "code_00226EC8", D_00456D58);

INCLUDE_RODATA(const s32, "code_00226EC8", D_00456D68);

INCLUDE_RODATA(const s32, "code_00226EC8", D_00456D78);

INCLUDE_RODATA(const s32, "code_00226EC8", D_00456D88);

INCLUDE_RODATA(const s32, "code_00226EC8", D_00456D98);

INCLUDE_RODATA(const s32, "code_00226EC8", D_00456DA8);

INCLUDE_RODATA(const s32, "code_00226EC8", D_00456DB8);

INCLUDE_RODATA(const s32, "code_00226EC8", D_00456DC8);

INCLUDE_RODATA(const s32, "code_00226EC8", D_00456DD8);

INCLUDE_RODATA(const s32, "code_00226EC8", D_00456DE8);

INCLUDE_RODATA(const s32, "code_00226EC8", D_00456DF8);

INCLUDE_RODATA(const s32, "code_00226EC8", D_00456E08);

INCLUDE_RODATA(const s32, "code_00226EC8", D_00456E18);

INCLUDE_RODATA(const s32, "code_00226EC8", D_00456E28);

INCLUDE_RODATA(const s32, "code_00226EC8", D_00456E38);

INCLUDE_RODATA(const s32, "code_00226EC8", D_00456E48);

INCLUDE_RODATA(const s32, "code_00226EC8", D_00456E58);

INCLUDE_SDATA(const s32, "code_00226EC8", D_0046D118);

INCLUDE_SDATA(const s32, "code_00226EC8", D_0046D120);

INCLUDE_SDATA(const s32, "code_00226EC8", D_0046D124);

INCLUDE_SDATA(const s32, "code_00226EC8", D_0046D128);

INCLUDE_SDATA(const s32, "code_00226EC8", D_0046D12C);

INCLUDE_SDATA(const s32, "code_00226EC8", D_0046D130);

INCLUDE_SDATA(const s32, "code_00226EC8", D_0046D134);

INCLUDE_SDATA(const s32, "code_00226EC8", D_0046D138);

INCLUDE_SDATA(const s32, "code_00226EC8", D_0046D13C);

INCLUDE_SDATA(const s32, "code_00226EC8", D_0046D140);

INCLUDE_SDATA(const s32, "code_00226EC8", D_0046D144);

INCLUDE_SDATA(const s32, "code_00226EC8", D_0046D148);

INCLUDE_SDATA(const s32, "code_00226EC8", D_0046D14C);

INCLUDE_SDATA(const s32, "code_00226EC8", D_0046D150);

INCLUDE_SDATA(const s32, "code_00226EC8", D_0046D154);

INCLUDE_SDATA(const s32, "code_00226EC8", D_0046D158);

INCLUDE_SDATA(const s32, "code_00226EC8", D_0046D160);

INCLUDE_SDATA(const s32, "code_00226EC8", D_0046D164);

INCLUDE_SDATA(const s32, "code_00226EC8", D_0046D168);

INCLUDE_SDATA(const s32, "code_00226EC8", D_0046D169);

INCLUDE_SDATA(const s32, "code_00226EC8", D_0046D170);

INCLUDE_SDATA(const s32, "code_00226EC8", D_0046D178);

INCLUDE_SDATA(const s32, "code_00226EC8", D_0046D180);

INCLUDE_SDATA(const s32, "code_00226EC8", D_0046D188);

