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

INCLUDE_ASM(const s32, "code_0023E1D0", func_0023E1D0);

INCLUDE_ASM(const s32, "code_0023E1D0", effCreateSmallHeaderFromFile);

INCLUDE_ASM(const s32, "code_0023E1D0", func_0023E248);

INCLUDE_ASM(const s32, "code_0023E1D0", effCloneSmallHeaderFromWork);

INCLUDE_ASM(const s32, "code_0023E1D0", func_0023E290);

INCLUDE_ASM(const s32, "code_0023E1D0", func_0023E298);

INCLUDE_ASM(const s32, "code_0023E1D0", effCreateFadeVectorWork);

INCLUDE_ASM(const s32, "code_0023E1D0", effCreateFadeVectorFromFile);

INCLUDE_ASM(const s32, "code_0023E1D0", effFreeFadeVectorWork);

INCLUDE_ASM(const s32, "code_0023E1D0", effCloneFadeVectorWork);

INCLUDE_ASM(const s32, "code_0023E1D0", func_0023E430);

INCLUDE_ASM(const s32, "code_0023E1D0", effDrawLensFlare);

INCLUDE_ASM(const s32, "code_0023E1D0", func_0023EAD0);

INCLUDE_ASM(const s32, "code_0023E1D0", func_0023EAE0);

INCLUDE_ASM(const s32, "code_0023E1D0", effResetSelectionEntryBuffers);

INCLUDE_ASM(const s32, "code_0023E1D0", effCreateSelectionFlagListFromWork);

INCLUDE_ASM(const s32, "code_0023E1D0", effCreateSelectionFlagListFromFile);

INCLUDE_ASM(const s32, "code_0023E1D0", effReleaseSelectionFlagList);

INCLUDE_ASM(const s32, "code_0023E1D0", effCreateEmbeddedSelectionFlagList);

INCLUDE_ASM(const s32, "code_0023E1D0", effResetSelectionEntriesAndState);

INCLUDE_ASM(const s32, "code_0023E1D0", effUpdateSelectionEntryState);

INCLUDE_ASM(const s32, "code_0023E1D0", effDrawSelectionEntryVectors);

INCLUDE_ASM(const s32, "code_0023E1D0", func_0023EBF8);

INCLUDE_ASM(const s32, "code_0023E1D0", func_0023EC38);

INCLUDE_ASM(const s32, "code_0023E1D0", effDuplicatePayloadHeader);

INCLUDE_ASM(const s32, "code_0023E1D0", effCreateSelectionHeaderFromFile);

INCLUDE_ASM(const s32, "code_0023E1D0", effReleaseSelectionHeaderAllocation);

INCLUDE_ASM(const s32, "code_0023E1D0", effCloneSelectionHeaderFromWork);

INCLUDE_ASM(const s32, "code_0023E1D0", func_0023ECF0);

INCLUDE_ASM(const s32, "code_0023E1D0", effSelectionFrameAdvance);

INCLUDE_ASM(const s32, "code_0023E1D0", effUpdateFadeBlendA);

INCLUDE_ASM(const s32, "code_0023E1D0", effCreateFadeBlendWorkFromOutput);

INCLUDE_ASM(const s32, "code_0023E1D0", effReleaseFadeBlendWork);

INCLUDE_ASM(const s32, "code_0023E1D0", effUpdateProjectedBlurFadeRectangle);

INCLUDE_ASM(const s32, "code_0023E1D0", effUpdateTarget);

INCLUDE_ASM(const s32, "code_0023E1D0", effCreateFixedSlotBlurWorkFromFadeOutput);

INCLUDE_ASM(const s32, "code_0023E1D0", effReleaseFixedSlotBlurWork);

INCLUDE_ASM(const s32, "code_0023E1D0", effUpdateFadeMapA);

INCLUDE_ASM(const s32, "code_0023E1D0", effTextureReferenceRelease);

INCLUDE_ASM(const s32, "code_0023E1D0", effCreateVariableSlotBlurWorkFromFadeOutput);

INCLUDE_ASM(const s32, "code_0023E1D0", effReleaseVariableSlotBlurWork);

INCLUDE_ASM(const s32, "code_0023E1D0", effUpdateFadeMapB);

INCLUDE_ASM(const s32, "code_0023E1D0", effReplaceKindLinkedTarget);

INCLUDE_ASM(const s32, "code_0023E1D0", effUpdateFadeBlendB);

INCLUDE_ASM(const s32, "code_0023E1D0", effUpdateFadeBlendC);

INCLUDE_ASM(const s32, "code_0023E1D0", effCreateFadeColorWorkFromOutput);

INCLUDE_ASM(const s32, "code_0023E1D0", effReleaseFadeColorWork);

INCLUDE_ASM(const s32, "code_0023E1D0", effUpdateProjectedFadeColor);

INCLUDE_ASM(const s32, "code_0023E1D0", effReplaceLinkedKindWorkTarget);

INCLUDE_ASM(const s32, "code_0023E1D0", effAllocateKindWork);

INCLUDE_ASM(const s32, "code_0023E1D0", effCreateKindWorkFromFile);

INCLUDE_ASM(const s32, "code_0023E1D0", effReleaseLinkedTarget);

INCLUDE_ASM(const s32, "code_0023E1D0", effCloneKindWork);

INCLUDE_ASM(const s32, "code_0023E1D0", func_0023FD80);

INCLUDE_ASM(const s32, "code_0023E1D0", func_0023FD88);

INCLUDE_ASM(const s32, "code_0023E1D0", func_0023FDF0);

INCLUDE_ASM(const s32, "code_0023E1D0", func_0023FE00);

INCLUDE_ASM(const s32, "code_0023E1D0", func_0023FE08);

INCLUDE_ASM(const s32, "code_0023E1D0", effAllocateAlternateKindWork);

INCLUDE_ASM(const s32, "code_0023E1D0", effCreateKindWorkFromFileB);

INCLUDE_ASM(const s32, "code_0023E1D0", effReleaseAlternateKindWork);

INCLUDE_ASM(const s32, "code_0023E1D0", effCloneAlternateKindWork);

INCLUDE_ASM(const s32, "code_0023E1D0", func_002400B8);

INCLUDE_ASM(const s32, "code_0023E1D0", func_002400C0);

INCLUDE_ASM(const s32, "code_0023E1D0", func_00240128);

INCLUDE_ASM(const s32, "code_0023E1D0", func_00240138);

INCLUDE_ASM(const s32, "code_0023E1D0", func_00240140);

INCLUDE_ASM(const s32, "code_0023E1D0", effCreateBillboardWork);

INCLUDE_ASM(const s32, "code_0023E1D0", effBillboardWorkRelease);

INCLUDE_ASM(const s32, "code_0023E1D0", effDuplicateBillState);

INCLUDE_ASM(const s32, "code_0023E1D0", effReplaceBillboardClone);

INCLUDE_ASM(const s32, "code_0023E1D0", effBillboardEntryFrameReset);

INCLUDE_ASM(const s32, "code_0023E1D0", func_00240368);

INCLUDE_ASM(const s32, "code_0023E1D0", func_00240470);

INCLUDE_ASM(const s32, "code_0023E1D0", func_00240480);

INCLUDE_ASM(const s32, "code_0023E1D0", func_00240498);

INCLUDE_ASM(const s32, "code_0023E1D0", func_002404A0);

INCLUDE_ASM(const s32, "code_0023E1D0", effClearBillFrames);

INCLUDE_ASM(const s32, "code_0023E1D0", effCreateBillFrameNode);

INCLUDE_ASM(const s32, "code_0023E1D0", effReleaseBillFrameNode);

INCLUDE_ASM(const s32, "code_0023E1D0", billAdvanceFrameInstances);

INCLUDE_ASM(const s32, "code_0023E1D0", billUpdateFrameDrawColorAndTransform);

INCLUDE_ASM(const s32, "code_0023E1D0", billResetCellIndices);

INCLUDE_ASM(const s32, "code_0023E1D0", billCreateCellNode);

INCLUDE_ASM(const s32, "code_0023E1D0", billReleaseCellNode);

INCLUDE_ASM(const s32, "code_0023E1D0", billAdvanceCellInstances);

INCLUDE_ASM(const s32, "code_0023E1D0", billUpdateCellDrawColorAndTransform);

INCLUDE_ASM(const s32, "code_0023E1D0", billResetParticleIndices);

INCLUDE_ASM(const s32, "code_0023E1D0", billCreateParticleNode);

INCLUDE_ASM(const s32, "code_0023E1D0", billReleaseParticleNode);

INCLUDE_ASM(const s32, "code_0023E1D0", billAdvanceParticleInstances);

INCLUDE_ASM(const s32, "code_0023E1D0", billUpdateParticleDrawColorAndTransform);

INCLUDE_ASM(const s32, "code_0023E1D0", effClearAnimatedFrames);

INCLUDE_ASM(const s32, "code_0023E1D0", billAllocateAnimatedTransformEntries);

INCLUDE_ASM(const s32, "code_0023E1D0", effInitializeAlternatingTransformRows);

INCLUDE_ASM(const s32, "code_0023E1D0", billCreateAnimatedTransform);

INCLUDE_ASM(const s32, "code_0023E1D0", billCloneAnimatedTransform);

INCLUDE_ASM(const s32, "code_0023E1D0", billReleaseAlternatingTransformNode);

INCLUDE_ASM(const s32, "code_0023E1D0", billAdvanceAnimatedFrameTransforms);

INCLUDE_ASM(const s32, "code_0023E1D0", billUpdateAlternatingDrawColorAndTransform);

INCLUDE_ASM(const s32, "code_0023E1D0", billResetEmitterIndices);

INCLUDE_ASM(const s32, "code_0023E1D0", billAllocEmitterNode);

INCLUDE_ASM(const s32, "code_0023E1D0", billInitializeEmitterRows);

INCLUDE_ASM(const s32, "code_0023E1D0", billCreateEmitterTransform);

INCLUDE_ASM(const s32, "code_0023E1D0", billCloneEmitterTransform);

INCLUDE_ASM(const s32, "code_0023E1D0", billReleaseEmitterNode);

INCLUDE_ASM(const s32, "code_0023E1D0", billAdvanceEmitterInstances);

INCLUDE_ASM(const s32, "code_0023E1D0", billUpdateEmitterDrawColorAndTransform);

INCLUDE_ASM(const s32, "code_0023E1D0", effClearStripFrames);

INCLUDE_ASM(const s32, "code_0023E1D0", billAllocStripNode);

INCLUDE_ASM(const s32, "code_0023E1D0", billInitializeStripRows);

INCLUDE_ASM(const s32, "code_0023E1D0", billCreateStripTransform);

INCLUDE_ASM(const s32, "code_0023E1D0", billCloneStripTransform);

INCLUDE_ASM(const s32, "code_0023E1D0", billReleaseStripNode);

INCLUDE_ASM(const s32, "code_0023E1D0", billAdvanceStripInstances);

INCLUDE_ASM(const s32, "code_0023E1D0", billUpdateStripDrawColorAndTransform);

INCLUDE_ASM(const s32, "code_0023E1D0", billResetTrailIndices);

INCLUDE_ASM(const s32, "code_0023E1D0", billCreateTrailNode);

INCLUDE_ASM(const s32, "code_0023E1D0", billReleaseTrailNode);

INCLUDE_ASM(const s32, "code_0023E1D0", billAdvanceTrailInstances);

INCLUDE_ASM(const s32, "code_0023E1D0", billUpdateTrailDrawColorAndTransform);

INCLUDE_ASM(const s32, "code_0023E1D0", billResetQuadIndices);

INCLUDE_ASM(const s32, "code_0023E1D0", billAllocQuadNode);

INCLUDE_ASM(const s32, "code_0023E1D0", billInitializeQuadRows);

INCLUDE_ASM(const s32, "code_0023E1D0", billCreateQuadTransform);

INCLUDE_ASM(const s32, "code_0023E1D0", billCloneQuadTransform);

INCLUDE_ASM(const s32, "code_0023E1D0", billReleaseQuadNode);

INCLUDE_ASM(const s32, "code_0023E1D0", billAdvanceQuadInstances);

INCLUDE_ASM(const s32, "code_0023E1D0", billUpdateQuadDrawColorAndTransform);

INCLUDE_ASM(const s32, "code_0023E1D0", effAllocateActiveInstanceWork);

INCLUDE_ASM(const s32, "code_0023E1D0", effCreateResourceInstanceA);

INCLUDE_ASM(const s32, "code_0023E1D0", effCreateActiveInstanceFromFile);

INCLUDE_ASM(const s32, "code_0023E1D0", effDestroyActiveInstanceWork);

INCLUDE_ASM(const s32, "code_0023E1D0", effCreateActiveResource);

INCLUDE_ASM(const s32, "code_0023E1D0", effResetActiveInstanceFrame);

INCLUDE_ASM(const s32, "code_0023E1D0", effAdvanceActiveInstanceFrame);

INCLUDE_ASM(const s32, "code_0023E1D0", effDispatchActiveInstanceDraw);

INCLUDE_ASM(const s32, "code_0023E1D0", func_00245728);

INCLUDE_ASM(const s32, "code_0023E1D0", func_00245768);

INCLUDE_ASM(const s32, "code_0023E1D0", func_00245778);

INCLUDE_ASM(const s32, "code_0023E1D0", func_00245790);

INCLUDE_ASM(const s32, "code_0023E1D0", func_00245798);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_004BCB40);

INCLUDE_ASM(const s32, "code_0023E1D0", effCreateTrackSet);

INCLUDE_ASM(const s32, "code_0023E1D0", effCreateTrackSetWithSharedReferences);

INCLUDE_ASM(const s32, "code_0023E1D0", effReleaseResourceRefs);

INCLUDE_ASM(const s32, "code_0023E1D0", effDuplicateResourceRefs);

INCLUDE_ASM(const s32, "code_0023E1D0", func_00245B70);

INCLUDE_ASM(const s32, "code_0023E1D0", effLoadFlashTextures);

INCLUDE_ASM(const s32, "code_0023E1D0", func_002460D8);

INCLUDE_ASM(const s32, "code_0023E1D0", func_002460F0);

INCLUDE_ASM(const s32, "code_0023E1D0", effSegmentPointerSet);

INCLUDE_ASM(const s32, "code_0023E1D0", effReleaseRingResourceHandle);

INCLUDE_ASM(const s32, "code_0023E1D0", effUpdateExpandingRingPoints);

INCLUDE_ASM(const s32, "code_0023E1D0", billDrawCellBlendA);

INCLUDE_ASM(const s32, "code_0023E1D0", effResetClassFrameAndFlags);

INCLUDE_ASM(const s32, "code_0023E1D0", effCreateScaledClassDrawState);

INCLUDE_ASM(const s32, "code_0023E1D0", effReleaseClassDrawResources);

INCLUDE_ASM(const s32, "code_0023E1D0", effUpdateScaledClassFrameRows);

INCLUDE_ASM(const s32, "code_0023E1D0", billDrawClassUpdatedCellBlend);

INCLUDE_ASM(const s32, "code_0023E1D0", func_00246B48);

INCLUDE_ASM(const s32, "code_0023E1D0", effCreateRingHandle);

INCLUDE_ASM(const s32, "code_0023E1D0", effReleaseRingHandle);

INCLUDE_ASM(const s32, "code_0023E1D0", effUpdateCenteredRadialRingVertices);

INCLUDE_ASM(const s32, "code_0023E1D0", billDrawCellBlendB);

INCLUDE_ASM(const s32, "code_0023E1D0", effCreateClassWork);

INCLUDE_ASM(const s32, "code_0023E1D0", effCreateClassWorkFromFile);

INCLUDE_ASM(const s32, "code_0023E1D0", effDestroyClassWork);

INCLUDE_ASM(const s32, "code_0023E1D0", effCloneClassWork);

INCLUDE_ASM(const s32, "code_0023E1D0", effInitializeClassFrame);

INCLUDE_ASM(const s32, "code_0023E1D0", func_002471B8);

INCLUDE_ASM(const s32, "code_0023E1D0", effRunClassPostFrame);

INCLUDE_ASM(const s32, "code_0023E1D0", effUpdateClassFrame);

INCLUDE_ASM(const s32, "code_0023E1D0", func_00247280);

INCLUDE_ASM(const s32, "code_0023E1D0", func_00247290);

INCLUDE_ASM(const s32, "code_0023E1D0", func_002472A8);

INCLUDE_ASM(const s32, "code_0023E1D0", func_002472B0);

INCLUDE_ASM(const s32, "code_0023E1D0", effCreatePointSet4);

INCLUDE_ASM(const s32, "code_0023E1D0", effAssetQueueRelease);

INCLUDE_ASM(const s32, "code_0023E1D0", func_002473B8);

INCLUDE_ASM(const s32, "code_0023E1D0", effCreateSurfaceNode);

INCLUDE_ASM(const s32, "code_0023E1D0", effCreateSurfaceNodeForGrid);

INCLUDE_ASM(const s32, "code_0023E1D0", effResourceReferenceReplaceFromFile);

INCLUDE_ASM(const s32, "code_0023E1D0", effInitializeSurfaceForKind);

INCLUDE_ASM(const s32, "code_0023E1D0", effDestroySurfaceNode);

INCLUDE_ASM(const s32, "code_0023E1D0", effRecreateSurfaceNodeFromWork);

INCLUDE_ASM(const s32, "code_0023E1D0", effCreateSurfaceGridWithConfiguration);

INCLUDE_ASM(const s32, "code_0023E1D0", effCloneSurfaceGridResources);

INCLUDE_ASM(const s32, "code_0023E1D0", effRebuildSurfaceHandles);

INCLUDE_ASM(const s32, "code_0023E1D0", effReplaceResourceRef);

INCLUDE_ASM(const s32, "code_0023E1D0", effSetSurfaceRetainedResource);

INCLUDE_ASM(const s32, "code_0023E1D0", effReplaceSurfacePrimaryBillboard);

INCLUDE_ASM(const s32, "code_0023E1D0", effReplaceSurfaceFlaggedBillboard);

INCLUDE_ASM(const s32, "code_0023E1D0", effRebuildSurfaceJobs);

INCLUDE_ASM(const s32, "code_0023E1D0", effReplaceSurfaceResourceHolder);

INCLUDE_ASM(const s32, "code_0023E1D0", effClearSurfaceNodeRecordReferences);

INCLUDE_ASM(const s32, "code_0023E1D0", effAcquireSurfaceRecord);

INCLUDE_ASM(const s32, "code_0023E1D0", func_002481B8);

INCLUDE_ASM(const s32, "code_0023E1D0", func_00248E78);

INCLUDE_ASM(const s32, "code_0023E1D0", effMenuRecordVectorSet);

INCLUDE_ASM(const s32, "code_0023E1D0", effSetSurfaceRecordSecondaryVector);

INCLUDE_ASM(const s32, "code_0023E1D0", func_00248EE8);

INCLUDE_ASM(const s32, "code_0023E1D0", effIndexedFloatCallbackDispatch);

INCLUDE_ASM(const s32, "code_0023E1D0", func_00248F10);

INCLUDE_ASM(const s32, "code_0023E1D0", effFillSurfaceGridColorGradient);

INCLUDE_ASM(const s32, "code_0023E1D0", effReleaseSurfaceGridBuffers);

INCLUDE_ASM(const s32, "code_0023E1D0", effResetSurfaceGridFrame);

INCLUDE_ASM(const s32, "code_0023E1D0", effCopyRingFrameVectors);

INCLUDE_ASM(const s32, "code_0023E1D0", effAppendSurfaceGridVectors);

INCLUDE_ASM(const s32, "code_0023E1D0", effInterpolateSurfaceGridVectors);

INCLUDE_ASM(const s32, "code_0023E1D0", effBeginMatrixVuDrawPacket);

INCLUDE_ASM(const s32, "code_0023E1D0", effDrawSurfaceTrailHistory);

INCLUDE_ASM(const s32, "code_0023E1D0", effSubmitIndexedRenderPacket);

INCLUDE_ASM(const s32, "code_0023E1D0", func_00249940);

INCLUDE_ASM(const s32, "code_0023E1D0", func_00249EE0);

INCLUDE_ASM(const s32, "code_0023E1D0", func_00249F40);

INCLUDE_ASM(const s32, "code_0023E1D0", func_0024A138);

INCLUDE_ASM(const s32, "code_0023E1D0", func_0024A210);

INCLUDE_ASM(const s32, "code_0023E1D0", func_0024A218);

INCLUDE_ASM(const s32, "code_0023E1D0", func_0024A250);

INCLUDE_ASM(const s32, "code_0023E1D0", func_0024A260);

INCLUDE_ASM(const s32, "code_0023E1D0", func_0024A278);

INCLUDE_ASM(const s32, "code_0023E1D0", func_0024A280);

INCLUDE_ASM(const s32, "code_0023E1D0", func_0024A288);

INCLUDE_ASM(const s32, "code_0023E1D0", func_0024ADA8);

INCLUDE_ASM(const s32, "code_0023E1D0", func_0024ADE0);

INCLUDE_ASM(const s32, "code_0023E1D0", effRandomizeParticleFields);

INCLUDE_ASM(const s32, "code_0023E1D0", effCreateAlphaRampPointSetRows);

INCLUDE_ASM(const s32, "code_0023E1D0", effFreeIndexedEntries);

INCLUDE_ASM(const s32, "code_0023E1D0", effUpdateRadialWavePointRows);

INCLUDE_ASM(const s32, "code_0023E1D0", effDrawTintedFivePointGroupRows);

INCLUDE_ASM(const s32, "code_0023E1D0", effResetRadialClassResourceFrames);

INCLUDE_ASM(const s32, "code_0023E1D0", effCreateRadialClassResourceEntries);

INCLUDE_ASM(const s32, "code_0023E1D0", effDestroyRadialClassResourceEntries);

INCLUDE_ASM(const s32, "code_0023E1D0", effAdvanceRadialClassResourceFrames);

INCLUDE_ASM(const s32, "code_0023E1D0", effUpdateRadialClassInstances);

INCLUDE_ASM(const s32, "code_0023E1D0", effSeedBillScaleRange);

INCLUDE_ASM(const s32, "code_0023E1D0", effCreateRetainedPointSetColorRows);

INCLUDE_ASM(const s32, "code_0023E1D0", effReleaseBillPointEntries);

INCLUDE_ASM(const s32, "code_0023E1D0", effUpdateOrbitalPointSpanWaves);

INCLUDE_ASM(const s32, "code_0023E1D0", effDrawRetainedPointSetRows);

INCLUDE_ASM(const s32, "code_0023E1D0", effCreateClassResourceWork);

INCLUDE_ASM(const s32, "code_0023E1D0", effCreateClassResourceFromFile);

INCLUDE_ASM(const s32, "code_0023E1D0", effDestroyClassResourceWork);

INCLUDE_ASM(const s32, "code_0023E1D0", effCloneClassResourceWork);

INCLUDE_ASM(const s32, "code_0023E1D0", effResetDispatchCounter);

INCLUDE_ASM(const s32, "code_0023E1D0", effAdvanceClassResourceFrame);

INCLUDE_ASM(const s32, "code_0023E1D0", effDrawClassResourceWork);

INCLUDE_ASM(const s32, "code_0023E1D0", func_0024D920);

INCLUDE_ASM(const s32, "code_0023E1D0", effCopyClassResourcePosition);

void effPCPBossBaelSetPosition(effPCPBossBael_t *hData, const void *pPos)
{
    SDF_VU0_COPYVECTOR(&hData->param, pPos);
}

INCLUDE_ASM(const s32, "code_0023E1D0", effSetClassResourceColor);

INCLUDE_ASM(const s32, "code_0023E1D0", effSetClassResourceMatrixComponent);

INCLUDE_ASM(const s32, "code_0023E1D0", effCreatePointSet5);

INCLUDE_ASM(const s32, "code_0023E1D0", effReleasePointSetAsset);

INCLUDE_ASM(const s32, "code_0023E1D0", effDrawFivePointGroups);

INCLUDE_ASM(const s32, "code_0023E1D0", effCreateStripNode);

INCLUDE_ASM(const s32, "code_0023E1D0", effCreateStripNodeFromGrid);

INCLUDE_ASM(const s32, "code_0023E1D0", effFileResourceReferenceReplace);

INCLUDE_ASM(const s32, "code_0023E1D0", effReleaseModelResources);

INCLUDE_ASM(const s32, "code_0023E1D0", effCloneStripResourceFromOwner);

INCLUDE_ASM(const s32, "code_0023E1D0", effReplaceFileResourceRef);

INCLUDE_ASM(const s32, "code_0023E1D0", effClearStripRecordReferences);

INCLUDE_ASM(const s32, "code_0023E1D0", effAcquireStripRecord);

INCLUDE_ASM(const s32, "code_0023E1D0", effDrawEffectStrip);

INCLUDE_ASM(const s32, "code_0023E1D0", effUpdateAndDrawStripRecord);

INCLUDE_ASM(const s32, "code_0023E1D0", effSetStripRecordVector);

INCLUDE_ASM(const s32, "code_0023E1D0", effSetStripRecordSecondaryVector);

INCLUDE_ASM(const s32, "code_0023E1D0", func_0024E778);

INCLUDE_ASM(const s32, "code_0023E1D0", effSetStripOpacity);

INCLUDE_ASM(const s32, "code_0023E1D0", effResetBillTable);

INCLUDE_ASM(const s32, "code_0023E1D0", effAllocateRingFadeEntries);

INCLUDE_ASM(const s32, "code_0023E1D0", effFillRingFadeGradient);

INCLUDE_ASM(const s32, "code_0023E1D0", effCreateRingFadeTable);

INCLUDE_ASM(const s32, "code_0023E1D0", effAssetPointerSet);

INCLUDE_ASM(const s32, "code_0023E1D0", effReleaseRingFadeTable);

INCLUDE_ASM(const s32, "code_0023E1D0", func_0024EB98);

INCLUDE_ASM(const s32, "code_0023E1D0", effBlendBillboardInstanceColorsAndTransforms);

INCLUDE_ASM(const s32, "code_0023E1D0", func_0024F400);

INCLUDE_ASM(const s32, "code_0023E1D0", effAllocateBillFadeFrameEntries);

INCLUDE_ASM(const s32, "code_0023E1D0", effFillBillFadeGradient);

INCLUDE_ASM(const s32, "code_0023E1D0", effCreateBillFadeTable);

INCLUDE_ASM(const s32, "code_0023E1D0", effCloneBillFadeTable);

INCLUDE_ASM(const s32, "code_0023E1D0", effReleaseBillFadeTable);

INCLUDE_ASM(const s32, "code_0023E1D0", effUpdateColumnParticlesAndRibbons);

INCLUDE_ASM(const s32, "code_0023E1D0", effBillBlendCellColorAndUpdateTransform);

INCLUDE_ASM(const s32, "code_0023E1D0", func_00250070);

INCLUDE_ASM(const s32, "code_0023E1D0", effAllocateCompactRingFadeEntries);

INCLUDE_ASM(const s32, "code_0023E1D0", effFillCompactRingFadeGradient);

INCLUDE_ASM(const s32, "code_0023E1D0", effCreateCompactRingFadeTable);

INCLUDE_ASM(const s32, "code_0023E1D0", effCloneBillboardFrameAsset);

INCLUDE_ASM(const s32, "code_0023E1D0", effReleaseCompactRingFadeTable);

INCLUDE_ASM(const s32, "code_0023E1D0", effUpdateBillSpiral);

INCLUDE_ASM(const s32, "code_0023E1D0", effUpdateCompactRingDrawColorAndTransform);

INCLUDE_ASM(const s32, "code_0023E1D0", func_00250C00);

INCLUDE_ASM(const s32, "code_0023E1D0", func_00250C50);

INCLUDE_ASM(const s32, "code_0023E1D0", func_00250CC0);

INCLUDE_ASM(const s32, "code_0023E1D0", func_00250F08);

INCLUDE_ASM(const s32, "code_0023E1D0", func_00250F68);

INCLUDE_ASM(const s32, "code_0023E1D0", func_00250FC8);

INCLUDE_ASM(const s32, "code_0023E1D0", func_00250FF8);

INCLUDE_ASM(const s32, "code_0023E1D0", func_00251700);

INCLUDE_ASM(const s32, "code_0023E1D0", effAllocateBlock);

INCLUDE_ASM(const s32, "code_0023E1D0", effCreateResourceInstanceB);

INCLUDE_ASM(const s32, "code_0023E1D0", effCreateBlockResourceFromFile);

INCLUDE_ASM(const s32, "code_0023E1D0", effDestroyBlockResourceWork);

INCLUDE_ASM(const s32, "code_0023E1D0", effDuplicateActiveResourceB);

INCLUDE_ASM(const s32, "code_0023E1D0", effResetBlockResourceFrame);

INCLUDE_ASM(const s32, "code_0023E1D0", effAdvanceBlockResourceFrame);

INCLUDE_ASM(const s32, "code_0023E1D0", effDrawBlockResourceWork);

INCLUDE_ASM(const s32, "code_0023E1D0", func_00251BC8);

INCLUDE_ASM(const s32, "code_0023E1D0", func_00251C08);

INCLUDE_ASM(const s32, "code_0023E1D0", func_00251C18);

INCLUDE_ASM(const s32, "code_0023E1D0", func_00251C30);

INCLUDE_ASM(const s32, "code_0023E1D0", func_00251C38);

INCLUDE_ASM(const s32, "code_0023E1D0", effCreateRibbonWork);

INCLUDE_ASM(const s32, "code_0023E1D0", effCreateRibbonWithSharedResource);

INCLUDE_ASM(const s32, "code_0023E1D0", effSharedAssetReferenceRelease);

INCLUDE_ASM(const s32, "code_0023E1D0", effCloneRibbonWithSharedResource);

INCLUDE_ASM(const s32, "code_0023E1D0", effDrawRibbon);

INCLUDE_ASM(const s32, "code_0023E1D0", effLoadWindTexture);

INCLUDE_ASM(const s32, "code_0023E1D0", effGetWindTextureHandle);

INCLUDE_ASM(const s32, "code_0023E1D0", effResetAnimationFrameEntries);

INCLUDE_ASM(const s32, "code_0023E1D0", effAllocateAnimationBuffer);

INCLUDE_ASM(const s32, "code_0023E1D0", effFillFadeColorRows);

INCLUDE_ASM(const s32, "code_0023E1D0", effPrepareTextureAnimation);

INCLUDE_ASM(const s32, "code_0023E1D0", effPrepareOwnedTextureAnimation);

INCLUDE_ASM(const s32, "code_0023E1D0", effReleaseTextureAnimationWork);

INCLUDE_ASM(const s32, "code_0023E1D0", effUpdateBillFlame);

INCLUDE_ASM(const s32, "code_0023E1D0", effUpdateTintedScaledMesh);

INCLUDE_ASM(const s32, "code_0023E1D0", effInitializeAnimationPositions);

INCLUDE_ASM(const s32, "code_0023E1D0", effClampSlotCount);

INCLUDE_ASM(const s32, "code_0023E1D0", effCreateAnimationState);

INCLUDE_ASM(const s32, "code_0023E1D0", effActivateAnimationState);

INCLUDE_ASM(const s32, "code_0023E1D0", effReleaseAnimationFrameResources);

INCLUDE_ASM(const s32, "code_0023E1D0", effSynchronizeFileTransform);

INCLUDE_ASM(const s32, "code_0023E1D0", func_00253130);

INCLUDE_ASM(const s32, "code_0023E1D0", effCreatePrimarySlotAnimationState);

INCLUDE_ASM(const s32, "code_0023E1D0", effCreateAlternateSlotAnimationState);

INCLUDE_ASM(const s32, "code_0023E1D0", func_00253818);

INCLUDE_ASM(const s32, "code_0023E1D0", effAllocateQuantizedBuffer);

INCLUDE_ASM(const s32, "code_0023E1D0", effBuildQuantizedTexturedStrip);

INCLUDE_ASM(const s32, "code_0023E1D0", effPrepareQuantizedTexture);

INCLUDE_ASM(const s32, "code_0023E1D0", effPrepareOwnedQuantizedTexture);

INCLUDE_ASM(const s32, "code_0023E1D0", effReleaseBillboardFrameAsset);

INCLUDE_ASM(const s32, "code_0023E1D0", func_00253DC8);

INCLUDE_ASM(const s32, "code_0023E1D0", effUpdateFadedMeshTransform);

INCLUDE_ASM(const s32, "code_0023E1D0", effAllocateBlockWithModel);

INCLUDE_ASM(const s32, "code_0023E1D0", effCreateResourceInstanceC);

INCLUDE_ASM(const s32, "code_0023E1D0", effCreateModelBlockFromFile);

INCLUDE_ASM(const s32, "code_0023E1D0", effDestroyModelBlockWork);

INCLUDE_ASM(const s32, "code_0023E1D0", effRecreateActiveByClass);

INCLUDE_ASM(const s32, "code_0023E1D0", effResetModelBlockFrame);

INCLUDE_ASM(const s32, "code_0023E1D0", effAdvanceDispatchCounter);

INCLUDE_ASM(const s32, "code_0023E1D0", effDrawModelBlock);

INCLUDE_ASM(const s32, "code_0023E1D0", func_002542F0);

INCLUDE_ASM(const s32, "code_0023E1D0", func_00254330);

INCLUDE_ASM(const s32, "code_0023E1D0", func_00254340);

INCLUDE_ASM(const s32, "code_0023E1D0", func_00254358);

INCLUDE_ASM(const s32, "code_0023E1D0", func_00254360);

INCLUDE_ASM(const s32, "code_0023E1D0", effRetainScalyTextureReference);

INCLUDE_ASM(const s32, "code_0023E1D0", effReleaseScalyTextureReference);

INCLUDE_ASM(const s32, "code_0023E1D0", effAllocateTexturedStripWork);

INCLUDE_ASM(const s32, "code_0023E1D0", effCreateTexturedStripWithSharedTexture);

INCLUDE_ASM(const s32, "code_0023E1D0", effReleaseScalyStripResources);

INCLUDE_ASM(const s32, "code_0023E1D0", effAllocateStripFromWorkAndRetainTexture);

INCLUDE_ASM(const s32, "code_0023E1D0", func_002545C0);

INCLUDE_ASM(const s32, "code_0023E1D0", effLoadScalyTexture);

INCLUDE_ASM(const s32, "code_0023E1D0", effGetScalyTextureHandle);

INCLUDE_ASM(const s32, "code_0023E1D0", effSeedParticleSpanParameters);

INCLUDE_ASM(const s32, "code_0023E1D0", effCreateParticleSpanTable);

INCLUDE_ASM(const s32, "code_0023E1D0", effReleaseParticleList);

INCLUDE_ASM(const s32, "code_0023E1D0", func_00254F98);

INCLUDE_ASM(const s32, "code_0023E1D0", effDrawParticleSpanPointsAndReferences);

INCLUDE_ASM(const s32, "code_0023E1D0", effCreateModelResourceWithInlineData);

INCLUDE_ASM(const s32, "code_0023E1D0", effCreateModelResourceFromFile);

INCLUDE_ASM(const s32, "code_0023E1D0", effDestroyModelResource);

INCLUDE_ASM(const s32, "code_0023E1D0", effCreateModelResource);

INCLUDE_ASM(const s32, "code_0023E1D0", effResetModelResourceUpdateCount);

INCLUDE_ASM(const s32, "code_0023E1D0", effAdvanceModelResourceUpdateCount);

INCLUDE_ASM(const s32, "code_0023E1D0", effDispatchModelResourceCallback);

INCLUDE_ASM(const s32, "code_0023E1D0", func_00255D08);

INCLUDE_ASM(const s32, "code_0023E1D0", func_00255D60);

INCLUDE_ASM(const s32, "code_0023E1D0", func_00255D70);

INCLUDE_ASM(const s32, "code_0023E1D0", func_00255D88);

INCLUDE_ASM(const s32, "code_0023E1D0", func_00255D90);

INCLUDE_ASM(const s32, "code_0023E1D0", effCreatePointSet3);

INCLUDE_ASM(const s32, "code_0023E1D0", effReleaseModelPointSetAsset);

INCLUDE_ASM(const s32, "code_0023E1D0", effDrawThreePointGroups);

INCLUDE_ASM(const s32, "code_0023E1D0", effInitializeParticleDirection);

INCLUDE_ASM(const s32, "code_0023E1D0", effSetRotationFromAcceleratedFrameAngle);

INCLUDE_ASM(const s32, "code_0023E1D0", func_002562E8);

INCLUDE_ASM(const s32, "code_0023E1D0", effCreateConfiguredSlotNode);

INCLUDE_ASM(const s32, "code_0023E1D0", effCreateSlotNodeFromFileJob);

INCLUDE_ASM(const s32, "code_0023E1D0", effDestroyOwnedResources);

INCLUDE_ASM(const s32, "code_0023E1D0", effCloneOwnedState);

INCLUDE_ASM(const s32, "code_0023E1D0", effCloneSlotNodeSecondaryResources);

INCLUDE_ASM(const s32, "code_0023E1D0", effRebuildResourceEntries);

INCLUDE_ASM(const s32, "code_0023E1D0", effReplaceEffectSlotModelAndDeviceResources);

INCLUDE_ASM(const s32, "code_0023E1D0", effRebuildResourceEntryClones);

INCLUDE_ASM(const s32, "code_0023E1D0", effClearSurfaceRecordReferences);

INCLUDE_ASM(const s32, "code_0023E1D0", func_00256AF8);

INCLUDE_ASM(const s32, "code_0023E1D0", func_00256BF0);

INCLUDE_ASM(const s32, "code_0023E1D0", func_00257230);

INCLUDE_ASM(const s32, "code_0023E1D0", effSetActiveSlotPrimaryRecordVector);

INCLUDE_ASM(const s32, "code_0023E1D0", effSetActiveSlotSecondaryRecordVector);

INCLUDE_ASM(const s32, "code_0023E1D0", func_002572C8);

INCLUDE_ASM(const s32, "code_0023E1D0", effSetActiveSlotOpacity);

INCLUDE_ASM(const s32, "code_0023E1D0", effCreateParticleSharedResourceFromFile);

INCLUDE_ASM(const s32, "code_0023E1D0", effReleaseParticleResources);

INCLUDE_ASM(const s32, "code_0023E1D0", effCloneParticleSharedResource);

INCLUDE_ASM(const s32, "code_0023E1D0", effReplaceSharedResource);

INCLUDE_ASM(const s32, "code_0023E1D0", func_002576F0);

INCLUDE_ASM(const s32, "code_0023E1D0", func_002576F8);

INCLUDE_ASM(const s32, "code_0023E1D0", func_00257C90);

void func_00257C98(void) {
}

INCLUDE_ASM(const s32, "code_0023E1D0", func_00257CA0);

INCLUDE_ASM(const s32, "code_0023E1D0", func_00257CD0);

INCLUDE_ASM(const s32, "code_0023E1D0", func_00257D00);

INCLUDE_ASM(const s32, "code_0023E1D0", func_00257E98);

INCLUDE_ASM(const s32, "code_0023E1D0", func_00257EB0);

void func_00257F78(void) {
}

INCLUDE_ASM(const s32, "code_0023E1D0", func_00257F80);

INCLUDE_ASM(const s32, "code_0023E1D0", func_00258040);

INCLUDE_ASM(const s32, "code_0023E1D0", effComputeLightDirectionVU);

INCLUDE_ASM(const s32, "code_0023E1D0", func_002582A0);

INCLUDE_ASM(const s32, "code_0023E1D0", func_002582E8);

INCLUDE_ASM(const s32, "code_0023E1D0", func_002583B8);

INCLUDE_ASM(const s32, "code_0023E1D0", func_00258500);

INCLUDE_ASM(const s32, "code_0023E1D0", func_00258538);

INCLUDE_ASM(const s32, "code_0023E1D0", func_002585E0);

INCLUDE_ASM(const s32, "code_0023E1D0", func_00258678);

INCLUDE_ASM(const s32, "code_0023E1D0", func_00259038);

INCLUDE_ASM(const s32, "code_0023E1D0", func_002590E0);

void func_002592F0(void) {
}

INCLUDE_ASM(const s32, "code_0023E1D0", func_002592F8);

INCLUDE_ASM(const s32, "code_0023E1D0", func_00259300);

INCLUDE_ASM(const s32, "code_0023E1D0", effDestroyCopiedEffectPayload);

void func_00259398(void) {
}

void func_002593A0(void) {
}

void func_002593A8(void) {
}

void func_002593B0(void) {
}

void func_002593B8(void) {
}

INCLUDE_ASM(const s32, "code_0023E1D0", effResetSlots);

INCLUDE_ASM(const s32, "code_0023E1D0", func_00259408);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_004BCCB8);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_004BCCD0);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_004BCCE0);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_004BCCF0);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_004BCD00);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_004BCD10);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_004BCD20);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_004BCD30);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_004BCD40);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_004BCD50);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_004BCD60);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_004BCD70);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_004BCD80);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_004BCD90);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_004BCDA0);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_004BCDB0);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_004BCDC0);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_004BCDD0);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_004BCDE0);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_004BCDF0);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_004BCE00);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_004BCE10);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_004BCE20);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_004BCE30);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_004BCE40);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_004BCE50);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_004BCE60);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_004BCE70);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_004BCE80);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_004BCE90);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_004BCEA0);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_004BCEC0);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_004BCEE0);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_004BCF00);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_004BCF20);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_004BCF40);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_004BCF60);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_004BCF80);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_004BCFA0);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_004BCFC0);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_004BCFE0);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_004BD000);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_004BD020);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_004BD040);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_004BD060);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_004BD080);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_004BD0A0);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_004BD0C0);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_004BD0E0);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_004BD100);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_004BD120);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_004BD140);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_004BD160);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_004BD180);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_004BD1A0);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_004BD1C0);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_004BD1E0);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_004BD200);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_004BD220);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_004BD240);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_004BD260);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_004BD280);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_004BD2A0);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_004BD2C0);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_004BD2E8);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_004BD308);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_004BD328);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_004BD348);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_004BD368);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_004BD388);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_004BD3A8);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_004BD3C8);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_004BD3E8);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_004BD408);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_004BD428);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_004BD448);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_004BD468);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_004BD488);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_004BD4A8);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_004BD4C8);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_004BD4E8);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_004BD508);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_004BD528);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_004BD548);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_004BD568);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_004BD588);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_004BD5A8);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_004BD5C8);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_004BD5E8);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_004BD608);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_004BD628);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_004BD648);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_004BD668);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_004BD688);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_004BD6A8);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_004BD6C8);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_004BD6E8);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_004BD708);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_004BD728);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_004BD748);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_004BD768);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_004BD788);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_004BD7A8);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_004BD7C8);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_004BD7E8);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_004BD808);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_004BD828);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_004BD848);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_004BD868);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_004BD888);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_004BD8A8);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_004BD8C8);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_004BD8E8);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_004BD908);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_004BD928);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_004BD948);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_004BD968);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_004BD988);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_004BD9A8);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_004BD9C8);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_004BD9E8);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_004BDA08);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_004BDA18);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_004BDA28);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_004BDA38);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_004BDA48);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_004BDA58);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_004BDA68);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_004BDA78);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_004BDA88);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_004BDA98);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_004BDAA8);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_004BDAB8);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_004BDAC8);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_004BDAD8);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_004BDAE8);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_004BDAF8);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_004BDB08);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_004BDB18);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_004BDB28);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_004BDB38);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_004BDB48);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_004BDB58);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_004BDB68);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_004BDB78);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_004BDB88);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_004BDB98);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_004BDBA8);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_004BDBB8);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_004BDBC8);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_004BDBD8);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_004BDBE8);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_004BDBF8);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_004BDC08);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_004BDC18);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_004BDC28);

INCLUDE_SDATA(const s32, "code_0023E1D0", D_004E1858);

INCLUDE_SDATA(const s32, "code_0023E1D0", D_004E1860);

INCLUDE_SDATA(const s32, "code_0023E1D0", D_004E1864);

INCLUDE_SDATA(const s32, "code_0023E1D0", D_004E1868);

INCLUDE_SDATA(const s32, "code_0023E1D0", D_004E186C);

INCLUDE_SDATA(const s32, "code_0023E1D0", D_004E1870);

INCLUDE_SDATA(const s32, "code_0023E1D0", D_004E1874);

INCLUDE_SDATA(const s32, "code_0023E1D0", D_004E1878);

INCLUDE_SDATA(const s32, "code_0023E1D0", D_004E187C);

INCLUDE_SDATA(const s32, "code_0023E1D0", D_004E1880);

INCLUDE_SDATA(const s32, "code_0023E1D0", D_004E1884);

INCLUDE_SDATA(const s32, "code_0023E1D0", D_004E1888);

INCLUDE_SDATA(const s32, "code_0023E1D0", D_004E188C);

INCLUDE_SDATA(const s32, "code_0023E1D0", D_004E1890);

INCLUDE_SDATA(const s32, "code_0023E1D0", D_004E1894);

INCLUDE_SDATA(const s32, "code_0023E1D0", D_004E1898);

INCLUDE_SDATA(const s32, "code_0023E1D0", D_004E18A0);

INCLUDE_SDATA(const s32, "code_0023E1D0", D_004E18A4);

INCLUDE_SDATA(const s32, "code_0023E1D0", D_004E18A8);

INCLUDE_SDATA(const s32, "code_0023E1D0", D_004E18A9);

INCLUDE_SDATA(const s32, "code_0023E1D0", D_004E18B0);

INCLUDE_SDATA(const s32, "code_0023E1D0", D_004E18B8);

INCLUDE_SDATA(const s32, "code_0023E1D0", D_004E18C0);

INCLUDE_SDATA(const s32, "code_0023E1D0", D_004E18C8);

