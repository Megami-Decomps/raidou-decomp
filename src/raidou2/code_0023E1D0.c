#include "common.h"
#include "raidou_alloc.h"

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

extern f32 D_r1_004028C0[4];

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

/* Declarations from dds-decomp src/dds1/game/code_0029C530.c */
#ifndef VERSION_DDS1
#define VERSION_DDS1 1
#endif

#define SMT3_DEFINES_effBaelHead_t 1

#define SMT3_DEFINES_effCrackColorSlot_t 1

#define SMT3_DEFINES_effCrackHead_t 1

#define SMT3_DEFINES_effCrackModel_t 1

#define SMT3_DEFINES_effCrackMotion_t 1

#define SMT3_DEFINES_effPCPBossBED_t 1

#define SMT3_DEFINES_effPCPBossBUNSIN2_t 1

#define SMT3_DEFINES_effPCPBossBUNSIN_t 1

#define SMT3_DEFINES_effPCPBossBael_t 1

#define SMT3_DEFINES_effPCPBossCamera_t 1

#define SMT3_DEFINES_effPCPBossDengeki_t 1

#define SMT3_DEFINES_effPCPBossKAGU2Attack_t 1

#define SMT3_DEFINES_effPCPBossKAGU2MUHENK_t 1

#define SMT3_DEFINES_effPCPBossKAGUAttack_t 1

#define SMT3_DEFINES_effPCPBossKAGUInvoke_t 1

#define SMT3_DEFINES_effPCPBossKAGUThunderbolt_t 1

#define SMT3_DEFINES_effPCPBossMETATOBeam_t 1

#define SMT3_DEFINES_effPCPBossONGY_t 1

#define SMT3_DEFINES_effPCPBossPlace_t 1

#define SMT3_DEFINES_effPCPBossThunderHead_t 1

#define SMT3_DEFINES_effPCPBossThunderParam_t 1

#define SMT3_DEFINES_effPCPBossThunderbolt_t 1

#define SMT3_DEFINES_effPCPBossVector_t 1

#define SMT3_DEFINES_effPCPBossWork_t 1

#define SMT3_DEFINES_effPCPCrackParam_t 1

#define SMT3_DEFINES_effPCPCrackPartFlash_t 1

#define SMT3_DEFINES_effPCPCrackPart_t 1

#define SMT3_DEFINES_effPCPCrack_t 1

#define SMT3_DEFINES_effPCPDANTE_Damage_Param_t 1

#define SMT3_DEFINES_effPCPDANTE_Damage_tag 1

#define SMT3_DEFINES_effPCPDANTE_ShowTime_Param_t 1

#define SMT3_DEFINES_effPCPDANTE_ShowTime_tag 1

#define SMT3_DEFINES_effPCPDANTE_Thunder_Param_t 1

#define SMT3_DEFINES_effPCPDANTE_Thunder_tag 1

#define SMT3_DEFINES_effPCPDANTE_UV_tag 1

#define SMT3_DEFINES_effPCPMuzzleFlashParam_t 1

#define SMT3_DEFINES_effPCPMuzzleFlash_tag 1

#define SMT3_DEFINES_effPCPScreenCrackModel_t 1

#define SMT3_DEFINES_effPCP_BED_Param_t 1

#define SMT3_DEFINES_effPCP_BED_tag 1

#define SMT3_DEFINES_effPCP_HBED2_Param_t 1

#define SMT3_DEFINES_effPCP_HBED2_tag 1

#define SMT3_DEFINES_effPCP_PBCamera_Param_t 1

#define SMT3_DEFINES_effPCP_PBCamera_tag 1

#define SMT3_DEFINES_effPartHandle_t 1

#include "dds3obj.h"

#include "kernel_draw.h"

#include "dds/bill_object_api.h"

#include "dds/btl_stage_task_cleanup.h"

#include "dds/dds3_draw_buffer.h"

#include "dds/dds3_fade_timing.h"

#include "dds/dds3_path.h"

#include "dds/dds3_world_primary.h"

#include "dds/dds3obj.h"

#include "dds/eff.h"

#include "dds/eff_blur.h"

#include "dds/evt_unit.h"

#include "dds/evt_unit_color_transition.h"

#include "dds/file_pac_completion_api.h"

#include "dds/file_request_api.h"

#include "dds/fld_transfer_chunk.h"

#include "dds/mdl.h"

#include "dds/sdf_asset_packets.h"

#include "dds/sdf_asset_state.h"

#include "dds/sdf_dev_state.h"

#include "dds/sdf_packet_append.h"

#include "dds/sdf_packet_list.h"

#include "dds/sdf_texture_draw_packet.h"

#include "dds/sdf_trig.h"

#include "dds/sdf_vu_matrix_multiply_api.h"

#include "dds/sdf_vu_rotation_axes.h"

#include "dds/sdk_libc.h"

#include "dds/sdk_libm.h"

/* Set vf28-vf31 to vf28-vf31 * vf24-vf27 in place. */
void MultMatrixRevNoArg(void);

/* Load the X-axis rotation matrix into the primary VU0 matrix registers. */
void SetRotateMatrixX(f32 angle);

f32 atanf(f32 x);

dds3PathBasicControl_t *dds3AllocPathBasicWorkData(struct dds3Basic_t *object);

struct billHandle_t *dds3BillboardGeneral(s32 index);

struct sdfTexHandle_t *dds3BillboardGetGeneralTex(s32 index);

void dds3BillboardSetAnimColorAdvMode(struct billHandle_t *billboard);

void dds3BillboardSetAnimKey(struct billHandle_t *billboard, u32 frame);

void dds3BillboardSetMode(struct billHandle_t *billboard, s32 mode);

void dds3DeletePathControl(dds3PathBasicControl_t *path);

void dds3DeleteSceneBasicData(dds3Basic_t *node);

/* Factories accept the serialized parameter prefix used by effect callbacks. */
EffBlurScatterWork *dds3EffectDistortionBlurCreate(dds3Effect_DistortionBlurParam_t *params);

void dds3EffectDistortionBlurDelete(EffBlurScatterWork *work);

EffBlurTemplate *dds3EffectPartialMultiBlurCreate(const dds3Effect_PartialMultiBlurParam_t *body);

void dds3EffectPartialMultiBlurDelete(EffBlurTemplate *owner);

EffResourceRectWork *dds3EffectPartialMultiFilterCreate(dds3Effect_PartialMultiFilterParam_t *params);

void dds3EffectPartialMultiFilterDelete(EffResourceRectWork *work);

EffBlurScaleWork *dds3EffectRippleBlurCreate(dds3Effect_RippleBlurParam_t *params);

void dds3EffectRippleBlurDelete(EffBlurScaleWork *work);

dds3Basic_t *dds3GetActiveCameraSceneBasic(dds3Basic_t *world);

/* Return the borrowed primary world node, or NULL when no world is active. */
struct dds3Basic_t *dds3GetActiveScene(void);

/* Return the current frame draw-buffer slot (0 or 1). */
u32 dds3GetFrameID(void);

void dds3PathBasicControlGetPos(dds3PathBasicControl_t *path);

void dds3PathBasicControlGetRot(dds3PathBasicControl_t *path);

/* Configure the foreground fade timing mode and frame duration. */
void dds3SetQuake(s32 mode, s32 frames);

/* Disable configured timing while retaining the stored parameters. */
void dds3StopQuake(void);

void evtSetUnitColorRGB(evtUnitHandle_t *unit, s32 duration, u32 color);

/* Initialize an event unit's color transition for the requested duration. */
void evtSetUnitLightAndAmbient(evtUnitHandle_t *unit, s32 duration, u32 firstColor, u32 secondColor);

void evtSetUnitLightDir(evtUnitHandle_t *unit, EvtUnitDirectionTransitionFrames durationFrames);

void evtUnitLightOn(evtUnitHandle_t *unit);

/* Relocate resource words in buffer using this read-only transfer descriptor. */
void fldAddressResolver(u8 *buffer, const FldTransferChunk *chunk);

u16 mdlGetMajorNum(const dds3ModelHandle_t *ctx);

u16 mdlGetMinorNum(const dds3ModelHandle_t *ctx);

/* The referenced halfword is promoted to a word-sized SDK result. */
s32 mdlGetNumOfCut(const dds3ModelHandle_t *ctx, s32 searchId);

FilePacRequestCompletionResult pacCheckEnd(const struct FileRequest *request);

/* These title-specific wrappers forward an opaque stage-task owner and its
 * handled/not-handled result. */
s32 pacFileHandleFree_(void *request);

struct FileRequest *pacLoad(const char *requestName);

void pacSync(struct FileRequest *request);

SdfTextureDrawPacket *sdf2DLinkTexPacket(sdfDrawTag_t *packetList, sdfTexHandle_t *texture, s32 context);

void sdf3DPrimLinkMat(sdfDrawTag_t *packetList, sdfMat_t *asset,
                              s32 (*allocatePacket)(s32));

void sdfDrawTagLinkRef(struct sdfDrawTag_t *list, u32 packetAddress);

void sdfDrawTagLinkRefGif(struct sdfDrawTag_t *list, u32 packetAddress);

void sdfFileClose(sdfFileCmdHandle_t *state);

u32 sdfFileLength(sdfFileCmdHandle_t *state);

/* Stable state APIs. Callback factories keep caller-specific callback types. */
sdfFileCmdHandle_t *sdfFileOpen(const char *path);

void sdfFileRead(sdfFileCmdHandle_t *state, void *buffer, s32 byteCount);

/* Write the reference header for one buffered asset draw entry and return
 * the packet payload immediately after the header. */
struct SdfDmaTagHeader *sdfMatMakePacket(struct sdfMat_t *asset,
                                                           struct SdfDmaTagHeader *tag,
                                                           s32 frame);

void sdfMatSetMultiMethod(struct sdfMat_t *asset, u32 packetMode);

void sdfMatSetMultiTex(struct sdfMat_t *asset, struct sdfTexHandle_t *texture);

/* Store the primary or secondary texture owner and mark its draw state dirty. */
void sdfMatSetTex(struct sdfMat_t *asset, struct sdfTexHandle_t *texture);

/* Refresh one of the two draw entries and retain the other entry's dirty bits. */
void sdfMatUpdatePacket(struct sdfMat_t *asset, s32 entryIndex);

f32 sdfSin(f32 angle);

char *strcpy(char *dst, const char *src);

#include "dds/sdk_ps2.h"

#include "dds/eff_block128.h"

#include "dds/eff_editor_menu.h"

#include "dds/eff_surface_grid_node.h"

#include "dds/eff_surface_node.h"

#include "dds/eff_animation_state.h"

#include "dds/eff_model_resource.h"

#include "dds/eff_gs_packet.h"

#include "dds/eff_packet_params.h"

#include "dds/eff_copied_payload.h"

#include "dds/eff_class_ops.h"

#include "dds/eff_particle_shared_resource.h"

#include "dds/eff_camera_create_request.h"

#include "dds/eff_file_job_request.h"

#include "dds/eff_emitter_row.h"

#include "dds/fld.h"

#include "dds/btl_command.h"

#include "dds/sdf_quaternion.h"

#include "dds/btl_motion_transform.h"

#include "dds/sdf_asset_state.h"

#include "dds/btl_resource_browser.h"

#include "dds/sdf_packet_list.h"

#include "dds/eff_bill.h"

#include "dds/eff_anim.h"

#include "dds/eff_bill_column_particle.h"

#include "dds/eff_ribbon_work.h"

#include "dds/eff_class_work_api.h"

#include "dds/eff_scaled_class_frame_rows.h"

#include "dds/fld_transfer_chunk.h"

#include "dds/eff_point_set.h"

#include "dds/eff_point_set_table.h"

#include "dds/eff_span_work.h"

#include "dds/common.h"

#include "dds/dds3_camera_update_flags.h"

#include "dds/sdf_vu0.h"

#include "dds/eff_emitter_type.h"

#include "dds/sdf_vu_rotation_axes.h"

#include "dds/file_pac.h"

#include "dds/eff_resource_bank_slot.h"

#include "dds/eff_matrix4.h"

#include "dds/eff_active_resource.h"

#include "dds/eff_frame_asset.h"

#include "dds/eff_actor_transition_config.h"

#include "dds/eff_object_node.h"

#include "dds/eff_random_mod.h"

#include "dds/sdf_trig.h"

#include "dds/eff_random.h"

#include "dds/dds3_fade.h"

#include "dds/btl_stage_task_cleanup.h"

#include "dds/dds3obj.h"

#include "dds/dds3_world_primary.h"

#include "dds/sdf_packet_append.h"

#include "dds/sdf_dev_state.h"

#include "dds/sdf_chip.h"

#include "dds/eff_ref_obj.h"

#include "dds/sdf_heap_api.h"

#include "dds/eff_resource_slots.h"

#include "dds/eff_resource_source_lifetime.h"

#include "dds/dds3_path.h"

#include "dds/eff_transform.h"

#include "dds/btl_state.h"

#include "dds/evt_unit.h"

#include "dds/evt_unit_color_transition.h"

#include "dds/eff_blur.h"

#include "dds/eff_event_draw.h"

#include "dds/eff_curve.h"

#include "dds/file.h"

#include "dds/file_slot.h"

#include "dds/ee_mmi.h"

#include "dds/pcp_vu0.h"

#include "dds/eff_queue.h"

#include "dds/btl_resource_name.h"

#include "dds/mdl.h"

#include "dds/sdf.h"

#include "dds/sdf_draw.h"

#include "dds/fpu.h"

#include "dds/eff.h"

#include "dds/eff_billboard_work.h"

#include "dds/eff_resource_list.h"

#include "dds/file_request_api.h"

#include "dds/sdf_texture_file.h"

#include "dds/sdf_vu_matrix.h"

#include "dds/sdf_vu_matrix_multiply_api.h"

#include "dds/dds3_fade_timing.h"

#include "dds/sdf_asset_packets.h"

#include "dds/sdf_chunk.h"

/* Declared before first use (were implicit int). */
extern void btlBeginSceneLightColorTransition(f32 *lightColor, f32 *ambientColor, s32 frames);

extern void btlInitTintTransitionDefault(u16 soundId);

extern void btlInitTintTransitionResource(u32 resource, u16 soundId);

extern void evtDestroySelectionState(void);

extern s32 evtSelStateCreate(s32 countLimit, s16 frames, s32 firstValue, s32 secondValue);

extern void fileReadRecordSecondVector(FileSlotTable *record, u128 *out);

extern void fileReadVectorPtr20(FileSlotTable *record, u128 *out);

extern void fileSetRecordSecondVector(FileSlotTable *record, const void *src);

extern void kwlnPadResetMotorLevelsAndOutput(void);

extern void mnuRecordSetVector(FileSlotTable *record, const void *src);

static inline void effSubmitSurfacePacket(sdfGrOt_t *surface, sdfDrawTag_t *list) {
    surface->regFunc(surface, list);
}

#include "dds/sdf_packet_alloc.h"

extern u32 D_r1_004035D0[];

extern sdfGrOt_t *D_r1_00403610[];

extern sdfGrOt_t *D_r1_004037A8[];

extern u32 D_r1_00403440[];

extern sdfGrOt_t *D_0037EB78[];

#include "dds/bill_object_api.h"

extern char D_r1_00456E58[];

extern char D_r1_00456E70[];

extern char D_r1_00457028[];

extern char D_r1_00457040[];

extern f32 effComputeProjectedOffsetAngle(const void *position, const void *offset);

extern u8 D_003B2B10[];

extern u32 effColorEditRgbMenuOpen;

extern u32 effColorEditSelectedKey;

extern u32 effColorEditPage;

extern u32 D_r1_0046D9F8;

extern u32 D_r1_0046D9FC;

extern u32 D_r1_0046DA00;

extern u32 effColorEditSelectedAlphaKey;

extern struct BtlResourceEntryList *effResourceBankEntries;

extern struct BtlResourceDescriptor *effResourceBankDescriptor;

extern s32 effEditorCurrentMenu;

extern void *effQueuedFileObject;

extern u8 effPartsListExitRequested;

extern FileJobPayload *effTemporaryFileJob;

extern FileQueue *effAuxiliaryFileQueue;

extern FileQueue *effFileQueue;

extern u8 *effAllocateTexturedStripWork(u32 count, u32 repeat);

extern u32 effScalyTextureHandle;

extern s32 effSharedStripReferenceCount;

extern RefObj *effSharedScalyStripResource;

extern u32 effWindTextureHandle;

extern struct sdfMemHandle_t *D_003BC984;

extern struct sdfMemHandle_t *D_r1_0046D14C;

extern u8 D_r1_00415880[];

extern u8 D_r1_00415E10[];

extern u8 *D_r1_00403508[];

extern FileJobPayload *effQueuedFileHandle;

extern struct sdfDrawTag_t *effCurrentRenderPacket;

extern s32 effSharedRibbonReferenceCount;

extern u32 D_r1_0046D148;

extern struct EffExpandedList *effCreateTextureAnimationSet(u32);

extern struct SdfTextureFileHeader *effFlashTextureHandles;

extern struct EffModelResource *effCreateModelResourceWithInlineData(u16, void *, void *, u32);

extern EffSurfaceGridNode *effCreateSurfaceGridNode(u32, u32);

extern void effFillSurfaceGridColorGradient(EffSurfaceGridNode *, u32 *);

extern void effReleaseSurfaceGridBuffers(EffSurfaceGridNode *);

extern s32 dds3BillboardGeneralNumber(void);

extern sdf3DPrimPolyParm_t D_r1_004D1BC0;

extern EffTrackSet *effCreateTrackSetWithSharedReferences(u32, u32, u32);

extern void effReleaseResourceRefs(EffTrackSet *);

extern void dds3EffectFilter(void *);

extern f32 mnuMeasureProjectedPerpendicularDistance(f32);

extern void effBlurStepScaleSlotsAndDraw(void *);

extern s32 effRequestResourceByMode(u32, u32, u32, void **);

extern u32 D_r1_0046D108;

extern u32 D_r1_0046D10C;

typedef struct EffectObjectFlag {
    u32 state;
    u8 pad04[8];
    u32 flags;
} EffectObjectFlag;

extern EffectObjectNode *effFloorModelListHead;

extern u8 btlIsRuntimeAllocated(void);

void effDrawThreePointGroups(EffPointSet *, Matrix4 *);

extern u32 effPartsListScroll;

extern u32 D_r1_00416D44[];

extern s32 D_r1_0046D128[2];

extern RefObj *D_r1_0046D130[2];

extern u32 effSharedTextureReferenceCount;

extern s32 D_r1_0046D170[2];

extern s32 D_r1_0046D178[2];

extern u8 D_r1_0046D180[2];

extern u8 D_r1_0046D188[2];

/* Native resource operations add cloning before the frame callbacks.
 * The final word is the copied payload size, not another callback.
 * Factories retain this unit's existing address-word interface. */
typedef struct EffResourceOps {
    void (*initialize)();      /* 0x00 */
    u32 (*createResource)();    /* 0x04 */
    void (*destroyResource)(); /* 0x08 */
    u32 (*cloneResource)(void *); /* 0x0C */
    void (*update)();          /* 0x10 */
    void (*draw)();            /* 0x14 */
    u32 payloadSize;           /* 0x18 */
} EffResourceOps;

/* Active frame callbacks receive the parent; destruction receives its child. */
typedef struct EffActiveInstanceOps {
    void (*initialize)(EffClassWork *); /* 0x00 */
    u32 (*createResource)(); /* 0x04 */
    void (*destroyResource)(void *); /* 0x08 */
    u32 (*cloneResource)(void *); /* 0x0C */
    void (*update)(EffClassWork *); /* 0x10 */
    void (*draw)(EffClassWork *); /* 0x14 */
    u32 payloadSize; /* 0x18 */
} EffActiveInstanceOps;

typedef char EffActiveInstanceOpsSizeCheck[(sizeof(EffActiveInstanceOps) == 0x1C) ? 1 : -1];

/* Class-resource callbacks operate on the parent class-work owner. */
typedef struct EffClassResourceOps {
    void (*initialize)(EffClassWork *); /* 0x00 */
    void *(*createResource)(EffBillPointConfig *); /* 0x04 */
    void (*destroyResource)(EffClassWork *); /* 0x08 */
    void (*update)(EffClassWork *); /* 0x0C */
    void (*draw)(EffClassWork *); /* 0x10 */
    u32 payloadSize; /* 0x14 */
} EffClassResourceOps;

typedef char EffClassResourceOps_size_must_be_0x18[
    (sizeof(EffClassResourceOps) == 0x18) ? 1 : -1];

/* Block-resource callbacks receive the owning class work, not the payload. */
typedef struct EffBlockOps {
    void (*initialize)(EffClassWork *); /* 0x00 */
    u32 (*createResource)(); /* 0x04: preserve the existing generic result ABI */
    void (*destroyResource)(EffClassWork *); /* 0x08 */
    u32 (*cloneResource)(void *); /* 0x0C: preserve the existing generic result ABI */
    void (*update)(EffClassWork *); /* 0x10 */
    void (*draw)(EffClassWork *); /* 0x14 */
    u32 payloadSize; /* 0x18 */
} EffBlockOps;

typedef char EffBlockOps_size_must_be_0x1C[
    (sizeof(EffBlockOps) == 0x1C) ? 1 : -1];

struct EffModelResource;

struct EffSpanConfig;

struct EffSpanTable;

/* This table has a single populated kind with span-resource callbacks. */
typedef struct EffModelResourceOps {
    void (*initialize)(struct EffModelResource *); /* 0x00 */
    struct EffSpanTable *(*createResource)(struct EffSpanConfig *, dds3ModelHandle_t *); /* 0x04 */
    void (*destroyResource)(struct EffSpanTable *); /* 0x08 */
    void (*update)(struct EffModelResource *); /* 0x0C */
    void (*draw)(struct EffModelResource *); /* 0x10 */
    u32 payloadSize; /* 0x14 */
} EffModelResourceOps;

typedef char EffModelResourceOps_size_must_be_0x18[
    (sizeof(EffModelResourceOps) == 0x18) ? 1 : -1];

extern EffActiveInstanceOps effActiveInstanceOperations[];

extern EffClassOps effClassWorkOperations[];

extern EffClassResourceOps effClassResourceWorkOperations[];

extern EffBlockOps effBlockResourceOperations[];

extern EffBlockOps effModelBlockOperations[];

extern EffResourceOps effRuntimeResourceOperations[];

extern EffModelResourceOps effModelResourceOperations[];

/* Shared work object for the effEditScalarTrack helpers (layout inferred). */
typedef struct BaObj {
    u8 pad_0x00[0x0C]; // 0x00
    u8 *x0C;           // 0x0C
} BaObj; // 0x10

extern s32 effEditScalarTrack(EffScalarTrack *, u32, void *, void *);

extern u8 D_r1_00417410[];

extern u8 D_r1_00417528[];

extern u8 D_r1_00417328[];

extern u8 D_r1_00417478[];

extern u8 D_r1_00417378[];

extern struct BtlResourceNameRecord *effQueuedResourceNameRecord;

extern void effReleaseQueuedResourceName(void);

extern const char D_r1_0046D930[];

extern const char D_r1_0046D938[];

extern char D_r1_004D54F0[];

extern u8 D_0038F2B8[];

extern u8 effBattleOffset[];

extern u8 *D_r1_0046D91C;

extern void effDrawSurfaceNode(s32);

struct EffCameraCreateRow;

extern u32 effSelectCameraFileJobFromRows(const char *, const struct EffCameraCreateRow *, u32);

extern char D_003B3968[];

extern char D_r1_00456DD8[];

extern char D_r1_0046D208[];

extern char D_r1_0046D8F8[];

extern char D_r1_00456D48[];

extern char D_003B3938[];

extern char D_r1_00456DA8[];

extern char D_r1_00456D98[];

extern char D_r1_00456D88[];

extern char D_r1_00456DE8[];

extern char D_r1_00456D68[];

extern char D_r1_00456D58[];

extern char D_003B3888[];

extern char D_r1_0046D870[];

extern u8 D_0038DE70[];

extern u8 D_r1_004157D0[];

extern u8 D_r1_00415970[];

extern u8 D_r1_00415A20[];

extern u8 D_r1_00415B20[];

extern u8 D_0038E240[];

extern u8 D_r1_00415BB0[];

extern u8 D_r1_00415CF0[];

extern u8 D_r1_00415DA0[];

extern u8 D_r1_00416320[];

extern u8 D_r1_00415FA0[];

extern u8 D_r1_00416070[];

extern u8 D_0038E800[];

extern u8 D_r1_004162E0[];

extern u8 D_r1_004D5460[];

extern u8 D_r1_004D5650[];

extern u8 D_003BD108;

extern u8 D_003BD109;

extern u8 D_003BD10A;

extern s32 effEditColorTrack(s32, s32, s32);

extern u8 D_r1_004D3418[] __attribute__((aligned(4)));

/* Value holder with a float field and a u16 data pointer (layout inferred). */
typedef struct ValPtr44 {
    u8 pad_0x00[0x08]; // 0x00
    float f08;         // 0x08
    u8 pad_0x0C[0x38]; // 0x0C
    u16 *p44;          // 0x44
} ValPtr44; // 0x48

extern s32 effPollFileRecord(char *, s32);

struct EffKindWork;

/* Native kind operations include handle destruction and per-frame update;
 * zero-argument notifications retain the original unprototyped callback ABI. */
typedef struct EffKindDesc {
    void *(*create)(void *); // 0x00
    void (*destroy)(void *); // 0x04: release the created handle
    void (*update)();        // 0x08: advance work through its kind callback
    void (*initialize)(struct EffKindWork *, sdfTexHandle_t *); // 0x0C
    u32 payloadSize;         // 0x10: bytes copied after the work header
} EffKindDesc; // 0x14

/* Native kind work: 0x40-byte header, followed by the copied source payload.
 * Fade callbacks consume this same object: frame supplies their signed limit,
 * handle is the renderer output, and payload holds the curve configuration. */
typedef struct EffKindWork {
    f32 position[4]; /* 0x00: vector passed to projection routines */
    s32 mode;       // 0x10
    u32 color;      // 0x14
    f32 scale;      // 0x18
    s32 kind;       // 0x1C
    u32 frame;        // 0x20, incremented by the kind callback dispatcher
    void *handle;   // 0x24
    void *payload;  // 0x28
    u32 sourceKind; // 0x2C
    sdfTexHandle_t *target;     // 0x30
    u8 pad_0x34[0xC]; // 0x34
} EffKindWork; // 0x40

/* Source payload's kind selects the initial rendering mode. */
typedef struct EffKindSource {
    u8 pad_00[0x28];
    s32 modeKind;
} EffKindSource;

extern EffKindDesc D_r1_00403080[];

extern EffKindDesc D_r1_004030F8[];

/* VU0 model helpers consume vf10 directly, matching the original macro-mode setup. */
extern void effDrawScalyTexturedStrip(EffStripWork *, void *);

extern u16 D_003BC944;

extern void effInitModelVUState(dds3ModelHandle_t *);

extern s32 effComputeLightDirectionVU(dds3ModelHandle_t *, sdf3DLightParallelPacket_t *);

extern char D_r1_00455D00[];

void effDestroyModelContext(dds3ModelHandle_t *model);

dds3ModelHandle_t *effLoadViewerModelWithVUState(void *modelData, u32 modelDataSize);

typedef struct EffFadeHeaderWork {
    s32 frame;
    s32 payload[2];
} EffFadeHeaderWork;

typedef char EffFadeHeaderWorkSizeCheck[(sizeof(EffFadeHeaderWork) == 0x0C) ? 1 : -1];

EffFadeHeaderWork *effDuplicateSmallHeader(const void *source);

EffFadeHeaderWork *effCreateSmallHeaderFromFile(void *work);

void effReleaseFadeHeaderAllocation(EffFadeHeaderWork *allocation);

EffFadeHeaderWork *effCloneSmallHeaderFromWork(EffFadeHeaderWork *work);

void effFadeFrameReset(EffFadeHeaderWork *counter);

void effFadeFrameAdvance(EffFadeHeaderWork *counter);

EffFadeVectorWork *effCreateFadeVectorWork();

EffFadeVectorWork *effCreateFadeVectorFromFile(void *work);

void effFreeFadeVectorWork(EffFadeVectorWork *work);

EffFadeVectorWork *effCloneFadeVectorWork(EffFadeVectorWork *work);

void effCopyFadeWorkVector(f32 *dst, const f32 *src);

void effResetSelectionEntryBuffers(SdfFlagListWork *work);

SdfFlagListWork *effCreateSelectionFlagListFromWork(const SdfFlagListParams *work);

SdfFlagListWork *effCreateSelectionFlagListFromFile(void *file);

void effReleaseSelectionFlagList(SdfFlagListWork *work);

SdfFlagListWork *effCreateEmbeddedSelectionFlagList(SdfFlagListWork *work);

void effResetSelectionEntriesAndState(SdfFlagListWork *p);

void effUpdateSelectionEntryState(SdfFlagListWork *work);

void effDrawSelectionEntryVectors(SdfFlagListWork *work);

void effUpdateAndDrawSelectionEntries(SdfFlagListWork *work);

typedef struct EffSelectionHeaderWork {
    s32 frame;
    s32 payload[4];
} EffSelectionHeaderWork;

typedef char EffSelectionHeaderWorkSizeCheck[(sizeof(EffSelectionHeaderWork) == 0x14) ? 1 : -1];

EffSelectionHeaderWork *effDuplicatePayloadHeader(const void *source);

EffSelectionHeaderWork *effCreateSelectionHeaderFromFile(void *work);

void effReleaseSelectionHeaderAllocation(EffSelectionHeaderWork *allocation);

EffSelectionHeaderWork *effCloneSelectionHeaderFromWork(EffSelectionHeaderWork *work);

void effSelectionFrameReset(EffSelectionHeaderWork *counter);

void effSelectionFrameAdvance(EffSelectionHeaderWork *counter);

typedef struct EffFadeConfig {
    /* Color and alpha tracks at 0x00/0x24, followed by three scalar
     * tracks at 0x34, 0x60 and 0x8C. The scalar extents differ. */
    SdfColorTrack blendA;   /* 0x00 */
    SdfAlphaTrack blendB2;  /* 0x24 */
    EffScalarCurve blendB;   /* 0x34 */
    u8 pad58[8];
    EffScalarTrack rateA;   /* 0x60 */
    EffScalarTrack rateB;   /* 0x8C */
    s32 duration;         /* 0xB8 */
    u8 padBC[4];
    dds3Effect_Filter_t out; /* 0xC0 */
} EffFadeConfig;

typedef struct EffRateConfig {
    /* Color and alpha tracks at 0x00/0x24, followed by three scalar
     * tracks at 0x34, 0x60 and 0x8C. The scalar extents differ. */
    SdfColorTrack blendA;   /* 0x00 */
    SdfAlphaTrack blendB2;  /* 0x24 */
    EffScalarCurve blendB;   /* 0x34 */
    u8 pad58[8];
    EffScalarTrack rateA;   /* 0x60 */
    EffScalarTrack rateB;   /* 0x8C */
    s32 duration;         /* 0xB8 */
    u8 fixedMode;         /* 0xBC */
    u8 padBD[3];
    dds3Effect_Blur_t out;       /* 0xC0 */
} EffRateConfig;

/* Draw through the configured duration; zero duration uses frame zero.
 * Blend the packed color and use percent-scaled curves for the output rates. */
void effUpdateFadeBlendA(EffKindWork *work);

void *effCreateFadeBlendWorkFromOutput(void *work);

void effReleaseFadeBlendWork(void *resourceHandle);

/* Draw the fade in fixed subpixel units, either centered or at the projected position.
 * Projected X/Y use sixteen units per pixel; output Y is doubled. */
void effUpdateProjectedBlurFadeRectangle(EffKindWork *work);

void effUpdateTarget(EffKindWork *work, sdfTexHandle_t *target);

void *effCreateFixedSlotBlurWorkFromFadeOutput(void *source);

void effReleaseFixedSlotBlurWork(EffBlurScatterWork *handle);

/* Draw the pixel-unit fade using the handle's output record and payload's curves.
 * Zero projected mode suppresses drawing; the origin and Y scaling differ from subpixels. */
void effUpdateFadeMapA(EffKindWork *work);

void effTextureReferenceRelease(EffKindWork *work, sdfTexHandle_t *target);

void *effCreateVariableSlotBlurWorkFromFadeOutput(void *source);

void effReleaseVariableSlotBlurWork(EffBlurScaleWork *work);

/* Draw the same pixel-unit fade into the wider renderer output record.
 * The native work header and duration gate remain shared with the other kind callbacks. */
void effUpdateFadeMapB(EffKindWork *work);

void effReplaceKindLinkedTarget(EffKindWork *work, sdfTexHandle_t *target);

/* Draw a framebuffer fade using raw curve rates rather than percent-scaled rates.
 * The generic kind work supplies its packed color, mode and frame limit. */
void effUpdateFadeBlendB(EffKindWork *work);

/* Draw the color-only fade from the copied payload's curves.
 * Its output uses fixed rectangle bounds and the kind work's packed color and mode. */
void effUpdateFadeBlendC(EffKindWork *work);

void *effCreateFadeColorWorkFromOutput(void *work);

void effReleaseFadeColorWork(EffResourceRectWork *work);

/* This projected fade also consumes EffKindWork: position, handle and payload. */
extern void effComputeBlurRectBounds(EffResourceRectWork *work);

/* Update the fade rectangle's projected extent, center and multiplied tint. */
void effUpdateProjectedFadeColor(EffKindWork *work);

void effReplaceLinkedKindWorkTarget(EffKindWork *work, sdfTexHandle_t *target);

EffKindWork *effAllocateKindWork(u16 kind, u8 *source);

EffKindWork *effCreateKindWorkFromFile(FileJobPayload *work);

void effReleaseLinkedTarget(EffKindWork *work);

EffKindWork *effCloneKindWork(EffKindWork *work);

void effKindWorkFrameReset(EffKindWork *work);

void effAdvanceKindWorkFrame(EffKindWork *work);

void effCopyKindWorkPosition(void *dst, void *src);

void effSetKindWorkMatrixComponent(EffKindWork *work, float value);

/* Allocate work for the alternate kind table, copying its payload and creating its handle. */
EffKindWork *effAllocateAlternateKindWork(u16 kind, u8 *source);

EffKindWork *effCreateKindWorkFromFileB(FileJobPayload *work);

/* Release the alternate-kind handle, owned secondary reference, and work allocation. */
void effReleaseAlternateKindWork(EffKindWork *work);

/* Reallocate the source kind and payload; this path does not copy its secondary target. */
EffKindWork *effCloneAlternateKindWork(EffKindWork *work);

/* Reset the alternate kind-table work's frame counter. */
void effAlternateKindWorkFrameReset(EffKindWork *work);

/* Invoke the alternate kind-table update callback, then advance its frame counter. */
void effAlternateKindWorkFrameUpdate(EffKindWork *work);

/* Copy the leading vector of alternate kind-table work. */
void effCopyAlternateKindVector(void *dst, void *src);

/* Set the alternate kind-table work's packed color. */
void effSetAlternateKindColor(EffKindWork *work, u32 color);

/* Set the alternate kind-table work's scalar scale. */
void effSetAlternateKindScale(EffKindWork *work, float scale);

u8 *effCreateBillboardWork(FileJobPayload *source);

void effBillboardWorkRelease(EffBillboardWork *work);

void effReplaceBillboardClone(s32 dst, s32 src);

u8 *effDuplicateBillState(const u8 *source);

void effReplaceBillboardClone(s32 dst, s32 src);

void effBillboardEntryFrameReset(EffBillboardWork *work);

/* Update a live billboard frame's projected scale, rotation, and position before its callback. */
void effUpdateScaledBillboardFrame(EffBillboardWork *work);

void effCopyBillboardPosition(void *dst, void *src);

void effCopyBillboardOrientation(void *work, void *src);

/* Set the billboard's scalar scale; the leading vectors are not a Matrix4. */
void effSetBillboardMatrixComponent(EffBillboardWork *work, float value);

#include "dds/eff_frame_state.h"

void effClearBillFrames(u8 *work);

u8 *effCreateBillFrameNode(EffBillFrameConfig *config, u32 resource);

/* Release the frame node's shared tracks and backing allocation. */
void effReleaseBillFrameNode(s32 work);

typedef struct EffBillFrameEntry {
    s32 age;
    f32 spin;
    f32 angle;
    f32 radius;
    f32 innerWidth;
    f32 outerWidth;
} EffBillFrameEntry;

typedef char EffBillFrameEntrySizeCheck[(sizeof(EffBillFrameEntry) == 0x18) ? 1 : -1];

extern EffRandState effSharedRandomState[];

extern u8 effOnesVector[];

void billAdvanceFrameInstances(EffClassWork *work);

extern u8 effOnesVector[];

extern void effDrawTrackSet(EffTrackSet *, const Matrix4 *);

#include "dds/eff_bill_cell_draw_work.h"

void billUpdateFrameDrawColorAndTransform(BillCellDrawWork *work);

void billResetCellIndices(u8 *work);

u8 *billCreateCellNode(EffBillCellConfig *config, u32 resource);

void billReleaseCellNode(s32 work);

extern f32 billRingCosines[12];

extern f32 billRingSines[12];

/* Spawn and advance the kind-2 cell instances: each live cell moves out
 * along its heading and is drawn as a 13-vertex fan (center plus two
 * six-step rings of radii A and B), faded in and out over its lifetime. */
void billAdvanceCellInstances(EffClassWork *work);

void billUpdateCellDrawColorAndTransform(BillCellDrawWork *work);

void billResetParticleIndices(u8 *work);

u8 *billCreateParticleNode(EffBillParticleConfig *config, u32 resource);

void billReleaseParticleNode(s32 work);

void billUpdateParticleDrawColorAndTransform(BillCellDrawWork *work);

void effClearAnimatedFrames(u8 *work);

u8 *billAllocateAnimatedTransformEntries(EffBillAnimatedFrameConfig *config);

void effInitializeAlternatingTransformRows(u8 *node, u8 *config);

u8 *billCreateAnimatedTransform(EffBillAnimatedFrameConfig *config, u32 resource);

extern u8 *billAllocEmitterNode(u8 *);

extern u8 *billAllocQuadNode(u8 *);

extern EffTrackSet *effDuplicateResourceRefs(const EffTrackSet *);

extern void billInitializeEmitterRows(u8 *work, u8 *descriptor);

extern void billInitializeQuadRows(u8 *work, u8 *descriptor);

u8 *billCloneAnimatedTransform(u8 *work);

void billReleaseAlternatingTransformNode(s32 work);

/* Kind-4 animated frame entry: the radius grows by radiusStep each frame
 * for the config's drawProgress frames. */
typedef struct EffBillAnimatedEntry {
    s32 age;
    f32 spin;
    f32 angle;
    f32 radius;
    f32 radiusStep;
    f32 width;
} EffBillAnimatedEntry;

/* Spawn and advance the kind-4 animated frames: each live entry spins with
 * an accelerating rate, grows its radius over drawProgress frames and is
 * drawn as a four-vertex frame, faded in and out over its lifetime. */
void billAdvanceAnimatedFrameTransforms(EffClassWork *work);

void billUpdateAlternatingDrawColorAndTransform(BillCellDrawWork *work);

void billResetEmitterIndices(u8 *work);

u8 *billAllocEmitterNode(u8 *config);

void billInitializeEmitterRows(u8 *work, u8 *descriptor);

u8 *billCreateEmitterTransform(EffBillEmitterFrameConfig *config, u32 resource);

u8 *billCloneEmitterTransform(u8 *work);

void billReleaseEmitterNode(s32 work);

extern EffRandState effSharedRandomState[];

void billAdvanceEmitterInstances(EffClassWork *work);

void billUpdateEmitterDrawColorAndTransform(u8 *work);

void effClearStripFrames(u8 *work);

u8 *billAllocStripNode(u8 *config);

void billInitializeStripRows(u8 *node, u8 *config);

extern u8 *billAllocStripNode(u8 *);

u8 *billCreateStripTransform(EffBillStripFrameConfig *config, u32 resource);

u8 *billCloneStripTransform(u8 *work);

void billReleaseStripNode(s32 work);

extern EffRandState effSharedRandomState[];

void billAdvanceStripInstances(EffClassWork *work);

void billUpdateStripDrawColorAndTransform(u8 *work);

void billResetTrailIndices(u8 *work);

u8 *billCreateTrailNode(EffBillTrailFrameConfig *config, u32 resource);

void billReleaseTrailNode(s32 work);

void billUpdateTrailDrawColorAndTransform(u8 *work);

void billResetQuadIndices(u8 *work);

u8 *billAllocQuadNode(u8 *config);

void billInitializeQuadRows(u8 *work, u8 *descriptor);

u8 *billCreateQuadTransform(EffBillQuadFrameConfig *config, u32 resource);

u8 *billCloneQuadTransform(u8 *work);

void billReleaseQuadNode(s32 work);

extern EffRandState effSharedRandomState[];

void billAdvanceQuadInstances(EffClassWork *work);

void billUpdateQuadDrawColorAndTransform(u8 *work);

/* Four-byte owner header followed by its class-work pointer array. */
typedef struct EffClassWorkList {
    EffClassWork **entries;
} EffClassWorkList;

EffClassWork *effAllocateActiveInstanceWork(u16 kind, void *source);

/* The ring source selects a minimum of three segments and repeats its three colors. */
typedef struct EffRingSource {
    u8 pad_00[0x38];
    u32 segments;       // 0x38
    u8 pad_3C[8];
    u32 firstColor;     // 0x44
    u32 middleColor;    // 0x48
    u32 lastColor;      // 0x4C
} EffRingSource;

/* Class kind 3 copies this complete 0x68-byte serialized parameter record.
 * The color/alpha prefix is the existing interpolation provider's layout;
 * alphaTrack.surfaceIndex at 0x28 also selects the point-set draw type. */
typedef struct EffRadialRingParams {
    SdfColorTrack colorTrack; /* 0x00 */
    SdfAlphaTrack alphaTrack; /* 0x24 */
    u32 duration;            /* 0x34 */
    u32 segments;            /* 0x38 */
    u8 flag;                 /* 0x3C */
    u8 pad3D[3];
    f32 baseRadius;          /* 0x40 */
    u32 firstColor;          /* 0x44 */
    u32 middleColor;         /* 0x48 */
    u32 lastColor;           /* 0x4C */
    f32 widths[3];           /* 0x50 */
    f32 speed;               /* 0x5C */
    f32 acceleration;        /* 0x60 */
    u8 reverseTime;          /* 0x64 */
    u8 pad65[3];
} EffRadialRingParams;

typedef char EffRadialRingParams_size_must_be_0x68[(sizeof(EffRadialRingParams) == 0x68) ? 1 : -1];

EffClassWork *effCreateResourceInstanceA(u16 kind, void *source, u32 option);

EffClassWork *effCreateActiveInstanceFromFile(FileJobPayload *request);

void effDestroyActiveInstanceWork(EffClassWork *work);

EffClassWork *effCreateActiveResource(EffClassWork *work);

void effResetActiveInstanceFrame(EffClassWork *work);

void effAdvanceActiveInstanceFrame(EffClassWork *work);

void effDispatchActiveInstanceDraw(EffClassWork *work);

void effUpdateAndDrawActiveInstance(EffClassWork *work);

void effCopyActiveInstancePosition(void *dst, void *src);

void effCopyActiveInstanceOrientation(void *work, void *src);

void effSetActiveInstanceMatrixComponent(EffClassWork *work, float value);

EffTrackSet *effCreateTrackSet(s32 count, u16 kind);

extern struct SdfTextureFileHeader *D_r1_0046D124;

EffTrackSet *effCreateTrackSetWithSharedReferences(u32 count, u32 kind, u32 sharedRef);

/* Release the track set's retained reference, draw asset, and allocation. */
void effReleaseResourceRefs(EffTrackSet *refs);

/* Copy a track set: same size and kind, retaining the source's shared reference (or counting one more user of the built-in one). */
EffTrackSet *effDuplicateResourceRefs(const EffTrackSet *original);

extern sdfGrOt_t *D_0037EAB0[8];

extern u32 D_0037E9A0[20];

extern u32 D_0037E9F0[12];

extern u32 D_0037EA20[20];

extern u32 D_0037EA70[16];

extern sdfTexHandle_t *sdfGetOrCreateTextureForReference(sdfGrOt_t *, RefObj *);

struct sdfMat_t;

/* Draw track-set positions/colors/UVs; a null matrix selects the identity transform. */
void effDrawTrackSet(EffTrackSet *track, const Matrix4 *matrix);

extern struct sdfMemHandle_t *D_r1_0046D118[2];

void effLoadFlashTextures(void);

u32 effGetFlashTextureHandle(s32 flashTextureIndex);

typedef struct EffCounterHeader {
    u32 unk00;
    u32 frame;
} EffCounterHeader;

/* Class draw modes own either a counter header or per-segment scales. */
typedef struct EffClassDrawState {
    union {
        EffCounterHeader *ring;
        f32 *scales;
    };
    EffClassWork *effect;
    EffTrackSet *trackSet;
    struct sdfMemHandle_t *allocation;
} EffClassDrawState;

void effResetRingResourceFrame(s32 work);

/* Create a point-set reference and initialize four colors per segment. */
u32 *effSegmentPointerSet(u8 *work);

extern void effAssetQueueRelease(EffPointSet *);

void effReleaseRingResourceHandle(u32 *handle);

/* Class 1 ring parameters have a distinct 0x6C-byte copied layout. */
typedef struct EffExpandingRingParams {
    SdfColorTrack colorTrack;
    SdfAlphaTrack alphaTrack;
    s32 duration;
    u32 segments;
    u8 flag;
    u8 pad3D[3];
    f32 baseRadius;
    u32 colors[3];
    f32 widths[3];
    u32 unk5C;
    f32 speed;
    f32 acceleration;
    u8 reverseTime;
    u8 pad69[3];
} EffExpandingRingParams;

typedef char EffExpandingRingParams_size[(sizeof(EffExpandingRingParams) == 0x6C) ? 1 : -1];

/* Generate the timed ring's four radial point bands; reverse-time mode is retained. */
void effUpdateExpandingRingPoints(EffClassWork *work);

extern void effDrawFourPointGroups(EffPointSet *, const Matrix4 *);

void billDrawCellBlendA(BillCellDrawWork *work);

void effResetClassFrameAndFlags(s32 work);

extern EffRandState effSharedRandomState[];

/* The cached class payload is copied from the 0x5C-byte source prefix. */
typedef struct EffRingClassConfig {
    EffRingSource ring;
    u8 pad50[0xC];
    u8 classConfig[0x5C];
    u8 padB8[0x10];
    f32 scale;
    f32 scaleRand;
} EffRingClassConfig;

EffClassDrawState *effCreateScaledClassDrawState(EffRingClassConfig *source);

void effReleaseClassDrawResources(s32 work);

extern void effAdvanceClassFrame(EffClassWork *);

/* Advance frame geometry and rebuild scaled row offsets with predecessor wrapping. */
void effUpdateScaledClassFrameRows(BillCellDrawWork *work);

extern void effRunClassPostFrame(EffClassWork *);

void billDrawClassUpdatedCellBlend(BillCellDrawWork *work);

void effResetClassRingFrame(EffClassWork *work);

/* Create a point-set reference and initialize four colors per segment. */
EffRingResource *effCreateRingHandle(EffRadialRingParams *work);

void effReleaseRingHandle(EffRingResource *handle);

void effUpdateCenteredRadialRingVertices(EffClassWork *work);

void billDrawCellBlendB(EffClassWork *work);

EffClassWork *effCreateClassWork(u16 kind, void *source);

EffClassWork *effCreateClassWorkFromFile(FileJobPayload *request);

void effDestroyClassWork(EffClassWork *work);

EffClassWork *effCloneClassWork(EffClassWork *work);

void effInitializeClassFrame(EffClassWork *work);

void effAdvanceClassFrame(EffClassWork *work);

void effRunClassPostFrame(EffClassWork *work);

void effUpdateClassFrame(EffClassWork *work);

void effSetClassWorkPrimaryTransformVector(s128 *dst, const s128 *src);

void effSetClassWorkSecondaryTransformVector(EffClassWork *work, const s128 *src);

extern sdf3DPrimPolyParm_t D_r1_004D1BF0[];

EffPointSet *effCreatePointSet4(u32 count);

/* Queue the draw asset for release and return the backing allocation. */
void effAssetQueueRelease(EffPointSet *set);

/* vu0 routine: SDK loads the supplied transform or constructs identity. */
void effDrawFourPointGroups(EffPointSet *set, const Matrix4 *matrix);

/* Create an empty surface node for handleCount grid cells; no handles are allocated yet. */
EffectSurfaceNode *effCreateSurfaceNode(u32 handleCount);

EffectSurfaceNode *effCreateSurfaceNodeForGrid(FileKeyBlock *grid);

extern void effRebuildSurfaceHandles(EffectSurfaceNode *, u16, void *);

void effReplaceResourceRef(EffectSurfaceNode *node, u32 entryId, const void *resource);

EffectSurfaceNode *effResourceReferenceReplaceFromFile(FileJobPayload *work);

extern void effRebuildSurfaceJobs(EffectSurfaceNode *, void *);

extern void effReplaceSurfaceResourceHolder(EffectSurfaceNode *node, u32 resource);

void effReplaceSurfacePrimaryBillboard(EffectSurfaceNode *node, u32 resourceId);

void effReplaceSurfaceFlaggedBillboard(EffectSurfaceNode *node, u32 resourceId);

void effSetSurfaceRetainedResource(EffectSurfaceNode *node, u32 resourceId);

EffectSurfaceNode *effInitializeSurfaceForKind(FileJobPayload *work);

void effDestroySurfaceNode(EffectSurfaceNode *node);

EffectSurfaceNode *effRecreateSurfaceNodeFromWork(u8 *work);

extern void effCloneSurfaceGridResources(EffectSurfaceNode *, u8 *);

EffectSurfaceNode *effCreateSurfaceGridWithConfiguration(u8 *work);

/* Replace the destination's kind-selected resources with clones or retained references. */
void effCloneSurfaceGridResources(EffectSurfaceNode *dst, u8 *work);

/* Replace the per-cell grid handles from the copied source header. kind is unused here. */
void effRebuildSurfaceHandles(EffectSurfaceNode *node, u16 kind, void *source);

void effReplaceResourceRef(EffectSurfaceNode *node, u32 entryId, const void *resource);

void effSetSurfaceRetainedResource(EffectSurfaceNode *node, u32 resourceId);

void effReplaceSurfacePrimaryBillboard(EffectSurfaceNode *node, u32 resourceId);

void effReplaceSurfaceFlaggedBillboard(EffectSurfaceNode *node, u32 resourceId);

void effRebuildSurfaceJobs(EffectSurfaceNode *node, void *source);

void effReplaceSurfaceResourceHolder(EffectSurfaceNode *node, u32 resource);

void effClearSurfaceNodeRecordReferences(s32 node);

void effAcquireSurfaceRecord(s32 node);

void effUpdateSurfaceRecordAndDraw(s32 node);

void effMenuRecordVectorSet(s32 node, const u128 *value);

void effSetSurfaceRecordSecondaryVector(s32 node, const u128 *value);

void effIndexedFloatCallbackDispatch(ValPtr44 *p, float v);

extern sdf3DPrimPolyParm_t D_r1_004D1C20;

extern sdf3DPrimPolyParm_t D_r1_004D1C50;

extern u8 D_r1_004034A0[];

extern u8 D_0037EBE0[];

/* vu0 routine: grid surface node with rows of three vertex columns */
EffSurfaceGridNode *effCreateSurfaceGridNode(u32 count, u32 columns);

/* vu0 routine: fade-blended vertex colours written into the surface node's tail */
void effFillSurfaceGridColorGradient(EffSurfaceGridNode *node, u32 *colors);

void effReleaseSurfaceGridBuffers(EffSurfaceGridNode *node);

void effResetSurfaceGridFrame(EffSurfaceGridNode *node);

/* Three-vector sampler reads the primary surface ring counters. */
void effCopyRingFrameVectors(EffSurfaceGridNode *node, u128 *destination, s32 frame);

/* data holds two vector addresses: the ring head, then the input frame.
 * ring_mirror_pointer_epochs_probe (hooked==unhooked): GCSE preserves the
 * head-to-mirror copy; separate append/mirror lifetimes keep native allocation. */
void effAppendSurfaceGridVectors(EffSurfaceGridNode *work, const f32 (*source)[4]);

extern void vuCatmullRomPoint(f32 (*)[4], f32);

/* vu0 routine: interpolate three-vector ring frames with SDK spline and lerp operations. */
void effInterpolateSurfaceGridVectors(EffSurfaceGridNode *work, const f32 (*vectors)[4]);

void effBeginMatrixVuDrawPacket(const Matrix4 *matrix);

/* Point history of a grid-surface trail: a ring of 16-byte points with packed colors,
 * drawn as triangle strips into the current render packet, then a fixed end cap. */
typedef struct EffSurfaceTrailHistory {
    u8 pad00[0x10];
    s32 pointCapacity;        /* 0x10 */
    s32 activePointCount;     /* 0x14 */
    s32 nextPointWriteIndex;  /* 0x18 */
    u8 pad1C[0x08];
    u128 *points;             /* 0x24 */
    u32 *colors;              /* 0x28 */
    s32 resourceHandle;       /* 0x2C */
    u128 *endPoints;          /* 0x30 */
    u32 *endColors;           /* 0x34 */
} EffSurfaceTrailHistory;

void effDrawSurfaceTrailHistory(EffSurfaceTrailHistory *history, u32 color);

typedef struct EffDrawEntry {
    u32 handle;
    s32 frame;
    u8 pad08[8];
    void (*draw)(u8 *, struct sdfDrawTag_t *);
} EffDrawEntry;

void effSubmitIndexedRenderPacket(u32 index);

extern EffQuadWork *effCreateQuadWork(FileJobPayload *job);

void effDuplicateRenderResourceOwner(EffQuadWork *work, const EffQuadWork *source);

extern sdf3DPrimPolyParm_t effQuadPacketParams;

extern u32 D_0037EC10[];

extern u32 D_0037EC20[];

EffQuadWork *effCreateQuadWork(FileJobPayload *job);

void effReleaseRenderResources(EffQuadWork *work);

EffQuadWork *effCloneRenderResourceWork(const EffQuadWork *source);

void effDuplicateRenderResourceOwner(EffQuadWork *work, const EffQuadWork *source);

void effResetRenderResourceKind(EffQuadWork *work);

void effCopyRenderResourcePosition(EffQuadWork *work, const f32 *source);

void effCopyRenderResourceOrientation(EffQuadWork *work, const f32 *source);

void effSetRenderResourceMatrixComponent(EffQuadWork *work, f32 value);

void effRandomizeParticleFields(EffClassWork *work);

EffPointSetTable *effCreateAlphaRampPointSetRows(EffBillPointConfig *src);

void effFreeIndexedEntries(EffClassWork *work);

extern f32 effWorldUpVector[4];

/* Advance radial five-point wave strips from the class orientation. */
void effUpdateRadialWavePointRows(EffClassWork *work);

typedef struct Matrix4 Matrix4;

void effDrawFivePointGroups(EffPointSet *set, Matrix4 *matrix);

/* Per-frame update of a five-point-group class: tint every active set and draw it under the class transform. */
void effDrawTintedFivePointGroupRows(EffClassWork *work);

extern void effResetDispatchCounter(EffClassWork *);

void effResetRadialClassResourceFrames(EffClassWork *work);

extern EffClassWork *effCreateClassResourceWork(u16, void *);

EffClassWorkList *effCreateRadialClassResourceEntries(EffBillPointConfig *config);

extern void effDestroyClassResourceWork(EffClassWork *);

void effDestroyRadialClassResourceEntries(EffClassWork *work);

extern void effAdvanceClassResourceFrame(EffClassWork *);

void effAdvanceRadialClassResourceFrames(EffClassWork *work);

extern void effCopyClassResourcePosition(void *, void *);

extern void effCopyClassResourceOrientation(void *, void *);

extern void effSetClassResourceMatrixComponent(EffClassWork *, f32);

extern void effSetClassResourceColor(EffClassWork *, u32);

extern void effDrawClassResourceWork(EffClassWork *);

/* vu0 routine: packed color blend and SDK vector copies. */
void effUpdateRadialClassInstances(EffClassWork *work);

typedef struct EffScaleRange {
    EffScaleRangeEntry *entries;
    f32 start;
    f32 delta;
    struct sdfMemHandle_t *allocation;
} EffScaleRange;

void effSeedBillScaleRange(EffClassWork *work);

EffScaleRange *effCreateRetainedPointSetColorRows(EffBillPointConfig *src);

void effReleaseBillPointEntries(EffClassWork *work);

/* Advance orbiting entries and rebuild their camera-facing, wavy point spans. */
void effUpdateOrbitalPointSpanWaves(EffClassWork *work);

/* vu0 routine: tint point sets and draw under the class quaternion transform.
 * Retail color homes +0x50/+0x60/+0x70/+0x80 are 16-byte SDK-sized locals. */
void effDrawRetainedPointSetRows(EffClassWork *work);

EffClassWork *effCreateClassResourceWork(u16 kind, void *source);

EffClassWork *effCreateClassResourceFromFile(FileJobPayload *request);

void effDestroyClassResourceWork(EffClassWork *work);

EffClassWork *effCloneClassResourceWork(EffClassWork *work);

void effResetDispatchCounter(EffClassWork *work);

void effAdvanceClassResourceFrame(EffClassWork *work);

void effDrawClassResourceWork(EffClassWork *work);

void effUpdateAndDrawClassResource(EffClassWork *work);

void effCopyClassResourcePosition(void *dst, void *src);

void effCopyClassResourceOrientation(void *work, void *src);

void effSetClassResourceColor(EffClassWork *work, u32 color);

void effSetClassResourceMatrixComponent(EffClassWork *work, float value);

extern sdf3DPrimPolyParm_t D_r1_004D1CB0[];

EffPointSet *effCreatePointSet5(s32 count);

/* Queue the draw asset for release and return the backing allocation. */
void effReleasePointSetAsset(EffPointSet *set);

void effDrawFivePointGroups(EffPointSet *set, Matrix4 *matrix);

EffectStripNode *effCreateStripNode(u32 percent);

EffectStripNode *effCreateStripNodeFromGrid(FileKeyBlock *grid);

extern void effReplaceFileResourceRef(EffectStripNode *, u32, const void *);

EffectStripNode *effFileResourceReferenceReplace(FileJobPayload *work);

void effReleaseModelResources(EffectStripNode *work);

EffectStripNode *effCloneStripResourceFromOwner(EffectStripNode *work);

void effReplaceFileResourceRef(EffectStripNode *node, u32 entryId, const void *resource);

void effClearStripRecordReferences(EffectStripNode *node);

void effAcquireStripRecord(EffectStripNode *node);

extern f32 aViewDown[4];

void effDrawEffectStrip(EffectStripNode *node);

/* Advance the active strip record, rebuild its geometry, and submit its tracks. */
void effUpdateAndDrawStripRecord(EffectStripNode *node);

void effSetStripRecordVector(EffectStripNode *node, const u128 *value);

void effSetStripRecordSecondaryVector(EffectStripNode *node, const u128 *value);

/* Store opacity and notify the strip's active-record callback. */
void effSetStripOpacity(EffectStripNode *p, f32 v);

void effResetBillTable(u8 *work);

u8 *effAllocateRingFadeEntries(EffBillVortexConfig *config);

/* Ring fade tables of the ring effects: `segments + 1` entries of colors
 * (4 words each) and radii (8 words each). The alpha fades in over the first
 * fadeIn fraction of the entries and out from the fadeOut fraction; the table
 * is then copied for every remaining layer. */
void effFillRingFadeGradient(u8 *node, EffBillEmitterCommon *config);

u8 *effCreateRingFadeTable(EffBillVortexConfig *config, u32 resource);

u8 *effAssetPointerSet(u8 *work);

void effReleaseRingFadeTable(s32 work);

extern void effDrawRibbon(EffRibbonWork *, const Matrix4 *);

void effBlendBillboardInstanceColorsAndTransforms(u8 *work);

void effResetBillboardFrameInstanceCounters(u8 *work);

u8 *effAllocateBillFadeFrameEntries(EffBillColumnConfig *config);

void effFillBillFadeGradient(u8 *node, EffBillEmitterCommon *config);

u8 *effCreateBillFadeTable(EffBillColumnConfig *config, u32 resource);

u8 *effCloneBillFadeTable(u8 *work);

void effReleaseBillFadeTable(s32 work);

/* Update the column's particle records and the ribbon vertices for this frame. */
void effUpdateColumnParticlesAndRibbons(EffClassWork *work);

void effBillBlendCellColorAndUpdateTransform(BillCellDrawWork *work);

void effResetParticleBillFrameCounters(u8 *work);

u8 *effAllocateCompactRingFadeEntries(EffBillSpiralConfig *config);

void effFillCompactRingFadeGradient(u8 *node, EffBillEmitterCommon *config);

u8 *effCreateCompactRingFadeTable(EffBillSpiralConfig *config, u32 resource);

u8 *effCloneBillboardFrameAsset(u8 *work);

void effReleaseCompactRingFadeTable(s32 work);

typedef struct EffSpiralParticle {
    s32 timer;      /* -1 free, -2 retired */
    u8 pad04[4];
    f32 tiltX;      /* 0x08 */
    f32 tiltY;      /* 0x0C */
    f32 radius;     /* 0x10 */
    f32 radiusStep; /* 0x14 */
    f32 spin;       /* 0x18 */
    f32 phase;      /* 0x1C */
    f32 width;      /* 0x20 */
    f32 sweep;      /* 0x24 */
    f32 height;     /* 0x28 */
} EffSpiralParticle;

/* vu0 routine: native matrix, cross/normalize and ribbon extrusion. */
void effUpdateBillSpiral(EffClassWork *work);

void effUpdateCompactRingDrawColorAndTransform(u8 *work);

EffClassWork *effAllocateBlock(u16 kind, void *source);

extern EffClassWork *effAllocateBlock(u16, void *);

EffClassWork *effCreateResourceInstanceB(u16 kind, void *source, u32 option);

EffClassWork *effCreateBlockResourceFromFile(FileJobPayload *request);

void effDestroyBlockResourceWork(EffClassWork *work);

EffClassWork *effDuplicateActiveResourceB(EffClassWork *work);

void effResetBlockResourceFrame(EffClassWork *work);

void effAdvanceBlockResourceFrame(EffClassWork *work);

void effDrawBlockResourceWork(EffClassWork *work);

void effUpdateAndDrawBlockResource(EffClassWork *work);

void effCopyBlockResourcePosition(void *dst, void *src);

void effCopyBlockResourceOrientation(void *work, void *src);

void effSetBlockResourceMatrixComponent(EffClassWork *work, float value);

extern sdf3DPrimPolyParm_t D_r1_004D1CE0;

EffRibbonWork *effCreateRibbonWork(u32 count, u32 repeat);

EffRibbonWork *effCreateRibbonWithSharedResource(u32 count, u32 repeat, struct SdfTextureFileHeader *resource);

void effSharedAssetReferenceRelease(EffRibbonWork *work);

extern sdfGrOt_t *D_r1_004036B8[];

EffRibbonWork *effCloneRibbonWithSharedResource(EffRibbonWork *source);

static inline void unpackNormalizedColor(u32 color) {
    u32 rgba[4];
    rgba[0] = color;
    EE_MMI_RGBA_UNPACK(rgba, 1.0f / 128.0f);
}

static inline u32 packNormalizedColor(void) {
    u32 rgba[4];
    EE_MMI_RGBA_PACK_UNIT(rgba[0], 128.0f);
    return rgba[0];
}

void effDrawRibbon(EffRibbonWork *work, const Matrix4 *matrix);

void effLoadWindTexture(void);

u32 effGetWindTextureHandle(void);

void effResetAnimationFrameEntries(u8 *work);

u32 *effAllocateAnimationBuffer(EffBillFlameConfig *work);

void effFillFadeColorRows(u8 *work, u8 *config);

extern u32 effCreateTexturedStripWithSharedTexture(u32, u32);

u32 *effPrepareTextureAnimation(EffBillFlameConfig *work);

extern u32 effAllocateStripFromWorkAndRetainTexture(u8 *);

u32 *effPrepareOwnedTextureAnimation(u8 *work);

void effReleaseScalyStripResources(u8 *work);

void effReleaseTextureAnimationWork(s32 work);

/* vu0 routine: fade-blended colour and scaled transform of a mesh draw record */
void effUpdateTintedScaledMesh(BillCellDrawWork *work);

void effInitializeAnimationPositions(EffClassWork *work);

u32 effClampSlotCount(FileKeyBlock *grid);

extern u32 effRetainScalyTextureReference(void);

EffAnimationState *effCreateAnimationState(u32 unused, u32 count);

EffAnimationState *effActivateAnimationState(s32 work);

extern void effReleaseScalyTextureReference(s32);

void effReleaseAnimationFrameResources(u8 *work);

void effSynchronizeFileTransform(u8 *work);

extern u32 effClampSlotCount(FileKeyBlock *);

EffAnimationState *effCreatePrimarySlotAnimationState(u8 *work);

EffAnimationState *effCreateAlternateSlotAnimationState(u8 *work);

void effResetSlotAnimationRecord(s32 work);

u32 *effAllocateQuantizedBuffer(EffBillQuantizedConfig *work);

void effBuildQuantizedTexturedStrip(u32 *buffer, EffBillQuantizedConfig *config);

extern void effBuildQuantizedTexturedStrip(u32 *, EffBillQuantizedConfig *);

u32 *effPrepareQuantizedTexture(EffBillQuantizedConfig *work);

u32 *effPrepareOwnedQuantizedTexture(u8 *work);

void effReleaseBillboardFrameAsset(s32 work);

void effOffsetNodeRowsVU(u8 *work);

/* vu0 routine: fade-blended colour and scaled transform of a mesh draw record */
void effUpdateFadedMeshTransform(BillCellDrawWork *work);

EffClassWork *effAllocateBlockWithModel(u16 kind, void *source);

extern EffClassWork *effAllocateBlockWithModel(u16, void *);

EffClassWork *effCreateResourceInstanceC(u16 kind, void *source);

EffClassWork *effCreateModelBlockFromFile(FileJobPayload *request);

void effDestroyModelBlockWork(EffClassWork *work);

EffClassWork *effRecreateActiveByClass(EffClassWork *work);

void effResetModelBlockFrame(EffClassWork *work);

void effAdvanceDispatchCounter(EffClassWork *work);

void effDrawModelBlock(EffClassWork *work);

void effUpdateAndDrawModelBlock(EffClassWork *work);

void effCopyModelBlockPosition(void *dst, void *src);

void effCopyModelBlockOrientation(void *work, void *src);

void effSetModelBlockMatrixComponent(EffClassWork *work, float value);

/* Retain and return the lazy shared reference to /effect/scaly00.tmx. */
u32 effRetainScalyTextureReference(void);

/* Drop a scaly texture reference; unused is ignored and the last owner releases the clone. */
void effReleaseScalyTextureReference(s32 unused);

extern sdf3DPrimPolyParm_t D_003DCB30;

/* Allocate position/UV/color rows and a draw handle using count and repeat. */
u8 *effAllocateTexturedStripWork();

/* Create a scaly strip and retain its shared texture reference. */
u32 effCreateTexturedStripWithSharedTexture(u32 count, u32 repeat);

void effReleaseScalyStripResources(u8 *work);

/* Recreate the source strip's dimensions and retain another shared texture reference. */
u32 effAllocateStripFromWorkAndRetainTexture(u8 *work);

#include "dds/dds3_draw_buffer.h"

#include "dds/dds3_frame_draw_buffer.h"

#include "dds/sdf_gs_header.h"

extern sdfGrOt_t *D_0037EE20[];

extern vu8 sdfFrameId;

extern dds3FrameDrawBuffer_t dds3FrameDrawPacketRecords[];

extern void sdfGrSetFrameMaskRGB(const u8 *, SdfDmaReferenceChainPacket *, s32);

extern void sdfGrSetFrameMaskReset(const u8 *, SdfDmaReferenceChainPacket *, s32);

void effDrawScalyTexturedStrip(EffStripWork *work, void *matrix);

void effLoadScalyTexture(void);

u32 effGetScalyTextureHandle(void);

typedef char EffModelResource_size_must_be_0x40[
    (sizeof(EffModelResource) == 0x40) ? 1 : -1];

void effSeedParticleSpanParameters(EffModelResource *work);

extern EffPointSet *effCreatePointSet3(s32 count);

/* Build the point and reference sets for each model map-position record. */
EffSpanTable *effCreateParticleSpanTable(EffSpanConfig *config, dds3ModelHandle_t *model);

extern void effReleaseModelPointSetAsset(EffPointSet *);

void effReleaseParticleList(EffSpanTable *list);

/* vu0 routine: quaternion matrix and SDK RGBA modulation. */
void effDrawParticleSpanPointsAndReferences(EffModelResource *work);

EffModelResource *effCreateModelResourceWithInlineData(u16 kind, void *source, void *secondary, u32 param);

u32 effCreateModelResourceFromFile(FileJobPayload *work);

typedef struct EffModelCreateRequest {
    u8 pad0[0x2C];
    u16 kind;
    u8 pad2E[2];
    dds3ModelHandle_t *assetId;
    u32 attributes;
    u8 pad38[4];
    void *source;
} EffModelCreateRequest;

void effDestroyModelResource(EffModelResource *effect);

EffModelResource *effCreateModelResource(EffModelCreateRequest *work);

void effResetModelResourceUpdateCount(EffModelResource *effect);

void effAdvanceModelResourceUpdateCount(EffModelResource *effect);

void effDispatchModelResourceCallback(EffModelResource *effect);

void effStepModelResourceCallbacks(EffModelResource *effect);

void effSetModelResourcePrimaryTransformVector(EffModelResource *effect, const f32 *position);

void effSetModelResourceSecondaryTransformVector(EffModelResource *effect, const f32 *orientation);

extern sdf3DPrimPolyParm_t D_r1_004D1D40[];

EffPointSet *effCreatePointSet3(s32 count);

/* Queue the draw asset for release and return the backing allocation. */
void effReleaseModelPointSetAsset(EffPointSet *set);

void effDrawThreePointGroups(EffPointSet *set, Matrix4 *matrix);

extern u32 effBTLFieldColorGetVariantSelector(void);

extern u32 effBTLFieldColorGetOverrideSelector(void);

extern u32 effBTLFieldColorGetFinalSelector(void);

extern void effBTLPartGetPositionPtr(BtlUnit *owner, EffectVectorRequest *request, u128 *out);

void effGetWorldVector(u32 which);

#define EFF_COLLECT_ACTORS_VARIANT_SELECTOR 0

#define EFF_COLLECT_ACTORS_ORIGINAL_SELECTOR 1

#define EFF_COLLECT_ACTORS_VARIANT_SELECTOR_ALIAS 2

#define EFF_COLLECT_ACTORS_ORIGINAL_GROUP 3

#define EFF_COLLECT_ACTORS_VARIANT_GROUP 4

#define EFF_COLLECT_ACTORS_EITHER_GROUP 5

#define EFF_COLLECT_ACTORS_OVERRIDE_SELECTOR 6

#define EFF_COLLECT_ACTORS_FINAL_SELECTOR 7

s32 effCollectModelEffectActors(BtlUnit **out, u32 kind);

extern void evtUnitLightOff(evtUnitHandle_t *, s32);

/* Restore actor light endpoints and directions when the field-light effect ends. */
void effRestoreBattleActorLighting(u32 unusedResource);

#include "dds/btl_effect_position.h"

extern void effBattleMiscDirectionTo(BtlUnit *, EffectVectorRequest *, f32 *);

extern u32 btlCameraVectorHasNaN(void);

extern u32 btlBlendColorVec(f32 *, f32 *, f32);

/* Apply timed actor light transitions, then fade back to the original colors/directions. */
void effUpdateBattleActorLighting(EffActiveResource *work);

void effSyncLinkedActorChildParameter(void);

/* Apply a temporary tint to eligible actors, then restore their original RGB. */
void effTransitionSelectedActorRgbColors(EffActiveResource *work);

extern u128 D_r1_004D1DB0[];

extern s8 D_r1_0046D169;

extern u128 D_r1_004D1DC0[];

extern u128 D_r1_004D1D80[];

extern sdf3DLight_t *D_r1_004037C0[3];

s32 effComputeLightDirectionVU(dds3ModelHandle_t *vector, sdf3DLightParallelPacket_t *target);

extern u128 *D_00324770[];

extern u128 D_003DCB90[];

extern u128 kwlnDefaultColorVector[];

void effResetDefaultColorTables(void);

/* Payload of resource kind 2: two colours plus the position request that feeds the shared light vector. */
typedef struct EffFieldColorConfig {
    u32 firstColor;   /* 0x00 */
    u32 secondColor;  /* 0x04 */
    EffectVectorRequest direction; /* 0x08 */
} EffFieldColorConfig;

void effInitializeFieldLightColors(EffActiveResource *work);

void effResetCopiedPayloadTargets(EffCopiedPayloadWork *work);

EffCopiedPayload *effAllocateCopiedEffectPayload(u32 owner, const void *source, s32 size);

typedef struct EffCopiedSpawnList {
    u32 count;
    FldSpawnBatch *groups;
} EffCopiedSpawnList;

extern dds3Basic_t *dds3AddPathBasic(s32 key, Dds3PathCurveTable *work, const char *name);

/* Capture constructor arguments before consuming the entry; see
 * docs/idioms.md, "Trace-justified cursor consumption: copied payload slots". */
void effInitializeCopiedPayloadSlots(EffCopiedPayload *payload);

EffCopiedPayload *effCreateAndInitializeCopiedPayload(u32 type, const void *source, s32 size);

EffCopiedPayload *effCloneEffectPayloadFromOwner(EffCopiedPayloadWork *work);

void effDestroyCopiedEffectPayload(EffCopiedPayload *payload);

#include "dds/eff_camera_path_parameters.h"

extern void btlSetRuntimeFlag2000(void);

extern u32 btlIsRuntimeFlag2000Clear(void);

extern void btlClearAllUnitDefeatCandidates(void);

extern void btlFlagMatchingUnitsDefeatCandidate(s32);

extern void btlFlagAllUnitsDefeatCandidate(void);

extern void btlFlagUnitDefeatCandidate(BtlUnit *);

extern void btlCopyUnitRotationQuaternion(BtlUnit *, f32 dst[4]);

extern void dds3CameraObjectBasicSetFovy(dds3Basic_t *, f32);

extern f32 D_003BC9A8;

extern u128 D_0037EE90, D_0037EEA0, D_0037EEB0, D_0037EEC0, D_0037EED0;

extern u128 D_0037E110;

/* vu0 routine: follow the selected actor with the active camera path. */
s32 func_002B3698(EffActiveResource *owner);

extern void btlApplyScaledUnitEffectParameter(BtlUnit *, s32, s32, f32);

extern void btlStartMoveOtherUnitsTask(BtlUnit *, s32);

typedef struct EffAnimInfo {
    u16 id;
    u16 flags;
    u8 actorSelection;
    u8 unk05;
    u16 loop;
} EffAnimInfo;

void effApplyOverlaySpecs(u8 *work);

extern void sndLoadAndPlayStationedSe(u32);

void effReportResourceStatus(EffActiveResource *work);

/* Apply the selected battle state's stored tint vectors immediately when its mask is set. */
void effApplyBattleStateTint(void);

/* Start the payload's packed-color tint transition on frame zero. */
void effStartTintTransitionFromColors(EffActiveResource *work);

void effResetSlots(void);

void effTickSlotVolumeFade(void);

void effSetDormantSlot(u32 index, u8 color, u32 value);

void effSetActiveSlot(u32 index, u8 color, u32 value);

void effResetActiveEffectSlots(void);

void effUpdateEffectSlotTransitions(EffActiveResource *work);

void effBattleTintTransitionInitialize(void);

/* The final timer word also supplies the default tint's halfword ID. */
typedef struct EffTintTransitionParams {
    u32 duration;
    u32 resourceArgument;
    u16 resourceKind;
    u16 unk0A;
    union {
        u32 delay;
        struct {
            u16 defaultKind;
            u16 unk0E;
        } ids;
    } tail;
} EffTintTransitionParams;

void effUpdateSlotTimerPair(EffActiveResource *work);

extern f32 D_003BC9A8;

/* Interpolates camera field of view in degrees and stores it in radians. */
void effApplyKeyframeAngle(EffActiveResource *work);

EffActiveResource *effAllocateResourcePayload(u16 kind, void *source);

EffActiveResource *effCreateResourceInstance(u16 kind, void *source, void *secondary, u32 param);

EffActiveResource *effCreateActiveResourceFromFile(FileJobPayload *request);

void effDestroyResourceInstance(EffActiveResource *work);

EffActiveResource *effDuplicateActiveResource(EffActiveResource *source);

void effClearCallbackFrame(EffActiveResource *work);

void effDispatchIndexedCallback(EffActiveResource *work);

void effDispatchEnabledCallback(EffActiveResource *work);

void effAdvanceActiveResourceCallbacks(EffActiveResource *work);

void effCopyActiveResourceVector(EffActiveResource *dst, const void *src);

void effCopyActiveResourceSecondaryVector(EffActiveResource *work, const void *src);

void effSetActiveResourceColor(EffActiveResource *work, u32 color);

void effSetActiveResourceScale(EffActiveResource *work, f32 value);

/* Alternate particle handles share a header but occupy distinct slots. */
EffParticleSharedResource *effCreateParticleSharedResourceFromFile(FileJobPayload *source);

void effReleaseParticleResources(EffParticleSharedResource *work);

void effReplaceSharedResource(EffParticleSharedResource *, EffParticleSharedResource *);

EffParticleSharedResource *effCloneParticleSharedResource(EffParticleSharedResource *source);

void effReplaceSharedResource(EffParticleSharedResource *work, EffParticleSharedResource *source);

void effResetParticleStateWord(EffParticleSharedResource *work);

extern sdfGrOt_t D_003255A8;

extern void sdfPrimLinkGt4(sdfDrawTag_t *list, s32 primitive, s32 x0, s32 y0, s32 u0, s32 v0, s32 color0, s32 x1, s32 y1, s32 u1, s32 v1, s32 color1, s32 x2, s32 y2, s32 u2, s32 v2, s32 color2, s32 x3, s32 y3, s32 u3, s32 v3, s32 color3, s32 depth, s32 (*alloc)(s32));

/* Submit frames below the exclusive limit; zero leaves the cursor unbounded. */
void effDrawAndAdvanceSharedParticleFrame(EffParticleSharedResource *work);

void btlInitializeEffectWork(void);

void effResetFileResourceStores(void);

s32 btlUpdateEffectWork(void);

void func_002B51A8(void);

void effResetFileResources(void);

extern void effResetFileResourceManager(void);

void effResetFileResourceStores(void);

void effSetBattleOffset(void *src);

extern void camFollowOffsetVec(u8 *, void *);

void effComputeBattleCameraPositionVU(u8 *effect);

extern void camAimRotation(void *, void *);

void effMultiplyQuatWithFlag(u8 *effect);

extern u8 effSelectedJobHeader[];

extern void effComputeBattleCameraPositionVU(u8 *);

void effApplyBattleCameraToObject();

u32 effCreateBattleCameraJob(u8 *request);

FileJobPayload *effLoadFileJobPayload(EffFileJobRequest *request, u32 existingJob);

void effInvokeFileJobWithBattleCamera(u32 job);

void effFileJobQueueRelease(u32 job);

extern char D_r1_0046D928[];

extern const char D_r1_0046D920[];

/* Payload option (+0x0C) selects the asset identifier at +0x08. */
typedef struct EffAssetIdentifier {
    u8 pad_00[4];
    u16 type;       // 0x04
    u8 pad_06[2];
    u16 id;         // 0x08
} EffAssetIdentifier;

u32 effPollPrimaryFile(void);

void effQueueNamedResourceRequest(void);

u32 effPollNamedFile(void);

void effQueueAttachedResourceRequest(void);

u32 effPollAttachedFile(void);

#include "dds/effect_asset_link.h"

extern EffectAssetLink *D_0038E9D8[22];

u8 *effFindAssetData(FileJob *entry);

EffQueuedFileObject *effFindAssetObject(FileJob *entry);

/* Return the selected queue row plus the offset of its visible window. */
u32 effGetSelectedFileQueueIndex(void);

extern EffectBlock128 effEditedQueueHeader;

extern EffQueuedFileObject D_0038F428;

extern s8 dds3PadBitTable[];

extern const char D_003BD0E8[];

extern u32 btlDebugMenuSetupNoOp(void *context);

extern void btlDebugMenuBeforeRecordNoOp(void);

extern void btlDebugMenuRecordNoOp(void *context);

extern void btlDebugMenuAfterRecordNoOp(void);

typedef struct EffCameraCreateRow {
    EffCameraCreateRequest request;
    u32 sourceTail;
} EffCameraCreateRow;

typedef char EffCameraCreateRowSizeCheck[(sizeof(EffCameraCreateRow) == 0x1C) ? 1 : -1];

extern void fileQueueInitTransform(void *);

extern FileJob *fileAppendJob(FileQueue *, u32);

extern s32 fileQueueCountLinkedJobs(FileQueue *);

extern FileJob *effCurrentFileQueueEntry;

extern u32 D_r1_0046D90C;

extern EffQueuedFileObject effPartsListMenu;

u32 effAppendPositionedCameraFileJob(const EffCameraCreateRequest *request);

/* Load the selected job's payload and camera snapshots; return the refresh status. */
u32 effLoadSelectedFileJob(void);

extern u32 D_r1_00416484[];

u32 effReinitializeFileQueue(void);

extern u32 D_r1_00416554[];

u32 effResetFileQueueState(void);

extern u32 D_r1_004164EC[];

u32 effFinalizeQueuedFile(void);

extern FileJob *fileDuplicateAndAppendJob(FileQueue *, FileJobPayload *);

extern FileJob *fileQueueFindById(FileQueue *, u32);

/* Clone the selected job, preserve its sector leader, and refresh the selected asset. */
u32 effCloneSelectedFileJob(void);

extern u32 effSelectLinkedFileState();

u32 effFlagFileQueueActive(void);

typedef struct EffectBlock90 {
    u32 word[36];
} EffectBlock90;

typedef struct EffectAlignedBlock128 {
    u64 word[16];
} EffectAlignedBlock128;

extern EffQueuedFileObject effBattleCameraSnapshotSelection;

extern EffEditorRow D_r1_00416650[];

extern EffEditorRow D_r1_00416698[];

extern u8 effPartsPreviewReload;

u32 effLoadBattleCameraSnapshot(u32 value);

u32 effStoreBattleCameraSnapshot(u32 value);

/* Present named camera/file requests, maintaining a preview job and returning selection status. */
u32 effSelectCameraFileJobFromRows(const char *nameKey, const EffCameraCreateRow *requests, u32 sourceCount);

s32 effCreateParticleTask(void);

s32 effCreatePolyTrackControlTask(void);

s32 effCreatePolyTrackTask(void);

s32 effCreateBlurTask(void);

s32 effCreateFilterTask(void);

s32 effCreateEnvironmentTask(void);

s32 effCreatePolyTextureTask(void);

s32 effCreatePolyFlashTask(void);

s32 effCreatePolyRingTask(void);

s32 effCreatePolyThunderTask(void);

s32 effCreatePolyTwinkleTask(void);

s32 effCreatePolyWindTask(void);

s32 effCreatePolyScalyTask(void);

s32 effCreatePolyCrackTask(void);

s32 effCreateBattleOnlyTask(void);

s32 effCreateTwoDimensionalTask(void);

extern void effPollResourceBankSlot(char *, u32, EffResourceBankSlot *);

u32 effPollPartResource(void);

u32 effPollNamedFileJob(void);

extern void btlBossDebugPrintfN(s32, s32, s32, const char *, ...);

extern void SetRotateMatrixY(f32);

extern char D_003B3A90[], D_003B3AA8[];

extern s8 dds3PadBitTable[];

extern f32 D_0038F530[4];

extern f32 D_r1_004D5640[4];

s32 effHandleDebugOrbitInput(void);

s32 effResetStaticState(void);

typedef struct EffectStateSnapshot {
    s128 vectors[4];
    f32 position[4]; /* 0x40: copied to the debug camera position and follow offset. */
    s128 unused50;
    u8 pad60[8];
    u32 flags; /* 0x68: transform flags tested by the camera diagnostic. */
    u8 pad6C[0x14];
} EffectStateSnapshot;

typedef char EffectStateSnapshotSizeCheck[(sizeof(EffectStateSnapshot) == 0x80) ? 1 : -1];

extern s32 effUpdateCameraDiagnostics(EffectStateSnapshot *, s8);

extern u8 D_r1_00416F38[];

extern s8 D_r1_0043EED9[];

extern void btlDebugDrawPoint(f32 *position);

extern void btlDebugDrawRadiusGuide(f32 *center, f32 radius, u32 color, s32 flags);

extern void btlDebugDrawSegment(f32 *from, f32 *to, u32 color);

extern void btlDebugMenuBeforeRecordNoOp(void);

extern void btlDebugMenuAfterRecordNoOp(void);

extern u32 btlDebugMenuSetupNoOp(void *camera);

extern void btlDebugMenuRecordNoOp(void *camera);

extern void camFollowOffsetVec(u8 *camera, void *destination);

extern void effComputeBattleCameraPositionVU(u8 *snapshot);

/* Synchronize the diagnostic camera position and calculate its positional guides.
 * Return input flags; the native diagnostic drawing providers are retail stubs. */
s32 effUpdateCameraDiagnostics(EffectStateSnapshot *snapshot, s8 mode);

void effCopyCameraSnapshotSelection(void);

#include "dds/sdf_vu_matrix.h"

extern u8 D_003BD10B;

s32 func_002B8108(void);

s32 effResetCameraSnapshotSelection(void);

/* The camera snapshot payload is the 0x90-byte record consumed by this updater. */
typedef union EffectCameraSnapshotView {
    u32 word[36];
    struct {
        u8 pad00[0x40];
        f32 offset[4];
        f32 quat[4];
        u8 pad60[0x30];
    } camera;
} EffectCameraSnapshotView;

typedef struct EffectCameraDebugState {
    u32 unknown00[4];
    u32 flags;
    u32 unknown14[9];
} EffectCameraDebugState;

extern EffectCameraDebugState D_r1_00416FC8;

extern s8 D_r1_0043EED9[];

extern f32 effWorldUpVector[4];

extern void btlDebugDrawOrientedExtentGuide(f32 *, f32, f32, u32, f32 (*)[4]);

extern void btlDebugDrawPoint(f32 *);

typedef struct EffectCameraDebugScratch {
    s128 basePosition;
    s128 segmentStart;
    s128 segmentEnd;
    s128 rotationMatrix[4];
} EffectCameraDebugScratch;

u32 effUpdateCameraSnapshotGuides(EffectCameraSnapshotView *state, s8 useBattlePosition);

u32 effLoadFileSlotAndPoll(void);

s32 effRunWithStateBackup(void);

extern u32 effUpdateCameraSnapshotGuides(EffectCameraSnapshotView *, s8);

u32 effProcessQueuedFileRecordAndPersistChanges(void);

extern u8 effPartsPreviewReload;

void effResetFileResources(void);

void effBattleResourceDescriptorRelease(void);

void effPollResourceBankSlot(char *path, u32 flags, EffResourceBankSlot *selection);

extern struct BtlResourceNameRecord *effFileQueueNameRecord;

extern const char D_r1_0046D7A0[];

void effInitializeResourceQueue(void);

u32 effPollResourceQueue(void);

void effReleaseQueuedResourceName(void);

void effQueueResource(const char *extension, const char *resourceName);

void effUpdateResourceQueue(const char *directoryPath, const char *extension, EffQueueRecord *record);

void effReleaseResourceBankDescriptors(void);

typedef struct EffectPollRecord {
    char fullName[0xC8]; /* Formatted path or selected built-in name. */
    s32 type;   // 0xC8
    s32 selectionStatus;  // 0xCC
    s32 pathVariant; // 0xD0
} EffectPollRecord;

void effPollResourceBank(s32 flags, EffectPollRecord *record);

extern EffQueuedFileObject effMappingState;

void effResetMappingFlags(void);

extern EffEditorRow D_r1_00417218[];

extern EffEditorRow D_r1_00417158[];

extern EffEditorRow D_r1_00417098[];

/* The effect mapping dispatcher stores its data pointer at +0x0C. */
typedef struct EffMappingRequest {
    u8 pad_00[0x0C];
    s32 object;          // 0x0C
} EffMappingRequest;

/* Parameters passed to the mapping dispatcher from distinct object channels. */
typedef struct EffMappingObject {
    u8 pad_00[0x34];
    s32 value34;       // 0x34
    u32 value38;       // 0x38
    s32 value3C;
    u8 pad40[0xC];
    u32 value4C;
    u8 pad50[0x20];
    u32 value70;       // 0x70
    s32 value74;
    u8 pad78[8];
    u32 value80;       // 0x80
    u32 value84;       // 0x84
    u8 pad_88[4];
    s32 value8C;       // 0x8C
    u8 pad_90[0x28];
    u32 valueB8;       // 0xB8
    u8 pad_BC[0x18];
    s32 valueD4;       // 0xD4
    s32 valueD8;       // 0xD8
    u8 pad_DC[0x18];
    s32 valueF4;       // 0xF4
} EffMappingObject;

s32 effMapObjectWithTemporaryMappingTable(s32 request);

/* Edit the request's G-/K-family COLOR tracks; return the native editor status. */
s32 effEditGkColorTrack(s32 request);

/* B-/F-family COLOR callback: edit the request's color and adjacent alpha tracks. */
s32 effEditBfColorTrack(s32 request);

/* HELPER's COLOR callback edits the shared default color/alpha data. */
s32 effEditHlpColorTrack(void);

/* Edit T-BOARD color/alpha tracks using the alternate property-option table. */
s32 effEditTBoardColorTrack(s32 request);

/* Pass the E-RAIN request's color/alpha tracks and frame bound to the editor. */
s32 effEditRainColorTrack(s32 request);

/* Pass the E-FLARE request's color/alpha tracks and frame bound to the editor. */
s32 effEditFlareColorTrack(s32 request);

/* COLOR property callback shared by the native S-family descriptors. */
s32 effEditSColorTrack(s32 request);

/* Edit R-NOMAL/R-CRCLE COLOR tracks, then restore the default option table. */
s32 effEditRColorTrack(s32 request);

/* COLOR property callback for the native R-TRACK descriptor. */
s32 effEditRTrackColorTrack(s32 request);

/* D-family COLOR callback; its DDS2 twin also serves O-family descriptors. */
s32 effEditDoColorTrack(s32 request);

s32 effResolveMappingWithTemporaryTable(s32 request);

/* COLOR callback shared by the native N-SPIRL/N-CYLND/N-SPHRE descriptors. */
s32 effEditNColorTrack(s32 request);

/* Edit COLOR in the N-family descriptors' nested POLYGON data. */
s32 effEditNPolygonColorTrack(s32 request);

/* Edit the root U-CYLND color and alpha tracks selected by the request. */
s32 effEditUCylinderColorTrack(s32 request);

/* Edit COLOR in U-CYLND's nested POLYGON data. */
s32 effEditUCylinderPolygonColorTrack(s32 request);

/* U-SMOKE/U-SPIRL COLOR uses its own property options; forward editor status. */
s32 effEditUSmokeSpiralColorTrack(s32 request);

/* Edit U-SPHRE's root COLOR tracks using its frame bound. */
s32 effEditUSphereColorTrack(s32 request);

/* COLOR callback for the CRACK polygon descriptor. */
s32 effEditCrackPolygonColorTrack(s32 request);

/* COLOR callback for the FLASH polygon descriptor. */
s32 effEditFlashPolygonColorTrack(s32 request);

/* 2D's COLOR callback restores its property options and forwards editor status. */
s32 effEditTwoDimensionalColorTrack(s32 request);

void btlResetEffectWork(void);

extern s8 dds3PadBitTable[];

extern EffScalarTrack D_003DFA78;

extern f32 D_003BD164;

extern f32 D_003BD168;

extern s32 D_003BD16C;

extern char D_003BD190[];

extern void btlDebugDrawRectangle(s32, s32, s32, s32);

extern void btlDebugDrawTwoColorRectangle(s32 x, s32 y, s32 width, s32 height, u32 color0, u32 color1);

extern void btlDebugDrawMappingColorSpan(s32 x, s32 y, s32 width, s32 height, u32 color0, u32 color1);

extern void btlDebugDrawLine2D(s32 x0, s32 y0, s32 x1, s32 y1, u32 color);

extern void btlDebugDrawPoint2DNoOp();

s32 effEditScalarTrack(EffScalarTrack *track, u32 frames, void *panelWork, void *subPanelWork);

/* Edit the requested P-family SCALE track; return the native editor status. */
s32 effEditPScaleTrack(BaObj *p);

/* Edit the requested P-family ROTATE track with its independent editor state. */
s32 effEditPRotationTrack(BaObj *p);

/* SCALE property callback shared by native G-/K-family descriptors. */
s32 effEditGkScaleTrack(BaObj *p);

/* ROTATE property callback for native K-family descriptors. */
s32 effEditKRotationTrack(BaObj *p);

/* SCALE property callback for native B-family descriptors. */
s32 effEditBScaleTrack(BaObj *p);

/* B-family ROTATE callback; identical forwarding code does not imply P-family SCALE. */
s32 effEditBRotationTrack(BaObj *p);

/* AREA property callback shared by B-/F-family descriptors. */
s32 effEditBfAreaTrack(BaObj *p);

/* Pass the requested T-BOARD SCALE track and its frame bound to the editor. */
s32 effEditTBoardScaleTrack(BaObj *p);

/* Pass the requested T-BOARD ROTATE track and its frame bound to the editor. */
s32 effEditTBoardRotationTrack(BaObj *p);

/* SCALE property callback for native W-family descriptors. */
s32 effEditWScaleTrack(BaObj *p);

/* ROTATE property callback for native W-family descriptors. */
s32 effEditWRotationTrack(BaObj *p);

/* Edit the SCALE_UV curve in U-CYLND's POLYGON child; forward editor status. */
s32 effEditUCylinderPolygonUvScaleTrack(BaObj *p);

/* Edit the shared U-SMOKE/U-SPIRL root SCALE curve; forward editor status. */
s32 effEditUSmokeSpiralScaleTrack(BaObj *p);

/* Edit the shared U-SMOKE/U-SPIRL root ROTATE curve; forward editor status. */
s32 effEditUSmokeSpiralRotationTrack(BaObj *p);

/* Edit SCALE_UV in their POLYGON children; forward the numeric editor's status. */
s32 effEditUSmokeSpiralPolygonUvScaleTrack(BaObj *p);

/* Edit the native U-SPHRE root SCALE curve; forward editor status. */
s32 effEditUSphereScaleTrack(BaObj *p);

/* Edit the native 2D root SCALE curve; forward editor status. */
s32 effEditTwoDimensionalScaleTrack(BaObj *p);

/* Edit the native 2D root ROTATE curve; forward editor status. */
s32 effEditTwoDimensionalRotationTrack(BaObj *p);

/* Reset the color/alpha editor's selected curve key to one, not a pending flag. */
void effResetCurveEditorSelection(void);

extern SdfAlphaTrack effColorEditAlphaTrack;

extern u8 D_0038FB38[];

extern s32 effColorEditFadeInFrame;

extern s32 effColorEditFadeOutFrame;

extern char effColorEditFrameCountFormat[];

extern void btlDebugDrawMappingColorSpan(s32, s32, s32, s32, u32, u32);

/* Edit the alpha fade interval and its four-breakpoint graph. */
s32 func_r1_00249458(SdfAlphaTrack *range, s32 frames);

/* ALPHA callback for POLYGON children of the native S-family descriptors. */
s32 effEditSPolygonAlphaTrack(s32 work);

/* D-family POLYGON FADE callback, shared with O in DDS2; retain bound plus one. */
s32 effEditDoPolygonFadeTrack(s32 work);

/* FADE callback for N-family POLYGON children; preserve bound plus one. */
s32 effEditNPolygonFadeTrack(s32 work);

/* FADE callback for the native U-CYLND POLYGON child. */
s32 effEditUCylinderPolygonFadeTrack(s32 work);

/* FADE callback for the native U-SPHRE POLYGON child. */
s32 effEditUSpherePolygonFadeTrack(s32 work);

/* Poll record from the resource-bank queue; the path variant selects its entry. */
extern void effPollResourceBank(s32, EffectPollRecord *);

s32 effPollFileQueueRecord(s32 request);

/* Poll queued TMX/P2A/general resources with the TLP format enabled. */
s32 effPollResourceQueueWithTlp(void);

/* Separate native descriptor callback with the same TLP-enabled queue filter. */
s32 effPollAlternateResourceQueueWithTlp(void);

/* Poll the queue without adding the TLP flag to its format mask. */
s32 effPollResourceQueueWithoutTlp(void);

extern u16 effClassifyResourceMask(s32);

typedef struct EffFileResourceRecord {
    char *name;
    u8 pad4[4];
    u16 mode;
    u8 padA[2];
    u8 *buffer;
    u32 size;
    struct sdfMemHandle_t *allocation;
} EffFileResourceRecord;

s32 effPollFileRecord(char *path, s32 flags);

/* Poll the TEXTURE property chooser in /tool/effect/mat/ and return its status. */
s32 effPollTextureResourceFile(void);

/* Alternate descriptor family's TEXTURE callback; retain its separate entrypoint. */
s32 effPollAlternateTextureResourceFile(void);

/* Poll EP resources from /tool/effect/ep/ and return the chooser status. */
s32 effPollEpResourceFile(void);

/* Poll EPL resources from /tool/effect/ and return the chooser status. */
s32 effPollEplResourceFile(void);

/* Separate native EP descriptor callback using the same directory and format. */
s32 effPollAlternateEpResourceFile(void);

/* TMX-only TEXTURE chooser for B-/F-family MATERIAL groups. */
s32 effPollBfMaterialTmxFile(void);

/* TMX-only TEXTURE chooser for N-family MATERIAL groups. */
s32 effPollNMaterialTmxFile(void);

/* TMX-only TEXTURE chooser for S-RAD_T/SPK_T/RNG_T/SCT_T MATERIAL groups. */
s32 effPollSTMaterialTmxFile(void);

u32 effFileJobSecondaryDataSet(void);

/* Poll PB resources from /tool/effect/hlp/ and return the chooser status. */
s32 effPollHlpPbResourceFile(void);

typedef struct EffectFileHeader {
    u8 unk_00[8];
    u16 mode;
    u8 unk_0A[2];
    u32 start;
    u32 length;
} EffectFileHeader;

extern void effResetFileResourceManager(void);

extern EffectFileHeader D_r1_00414EA8;

extern EffectFileHeader D_r1_00408D60;

extern EffectFileHeader D_r1_00411E28;

u32 fileLoadEffectSlotA(void);

extern u8 D_r1_00409908[] __attribute__((aligned(4)));

u32 fileLoadEffectSlotHelp(void);

u32 fileLoadEffectSlotB(void);

extern EffectFileHeader D_r1_00414710;

u32 effLoadFileSlotF2(void);

u32 effLoadMaterialFile(void);

void effResetFileResourceManager(void);

u16 effClassifyResourceMask(s32 flags);

EffectList *mnuAllocateValueRecord(u32 mode);

void effDestroyEffectList(EffectList *list);

u32 mnuGetValueRecordOwner(const EffectList *list);

s32 effAppendListEntry(EffectList *list, u32 value, u32 length,
                          u32 kind, u32 reference);

s32 effRemoveListEntry(EffectList *list);

/* Drives the head request of the list: mode 0 asks for a resource by name, modes 1 and 2 stream a package and hand each
 * finished job's buffer to the entry's destination. Returns the remaining entry count. */
s32 effPollResourceList(EffectList *list);
/* End of declarations from src/dds1/game/code_0029C530.c */

INCLUDE_ASM(const s32, "code_0023E1D0", func_r1_00226EC8);

EffFadeHeaderWork *effCreateSmallHeaderFromFile(void *work) {
    void *resource;

    resource = fileResolvePrimaryBuffer((FileJobPayload *)work);
    return effDuplicateSmallHeader(resource);
}

INCLUDE_ASM(const s32, "code_0023E1D0", func_r1_00226F40);

EffFadeHeaderWork *effCloneSmallHeaderFromWork(EffFadeHeaderWork *work) {
    return effDuplicateSmallHeader(work->payload);
}

void effResetSmallHeaderFrame(EffFadeHeaderWork *work) {
    work->frame = 0;
}

INCLUDE_ASM(const s32, "code_0023E1D0", func_r1_00226F90);

INCLUDE_ASM(const s32, "code_0023E1D0", effCreateFadeVectorWork);

EffFadeVectorWork *effCreateFadeVectorFromFile(void *work) {
    const EffLensFlareParams *resource;

    resource = fileResolvePrimaryBuffer(work);
    return effCreateFadeVectorWork(resource);
}

void effFreeFadeVectorWork(EffFadeVectorWork *work) {
    sdfChipFreeDirect(work);
}

EffFadeVectorWork *effCloneFadeVectorWork(EffFadeVectorWork *work) {
    return effCreateFadeVectorWork(&work->source);
}

void effResetFadeVectorFrame(EffFadeVectorWork *work) {
    work->frame = 0;
}

INCLUDE_ASM(const s32, "code_0023E1D0", effDrawLensFlare);

INCLUDE_ASM(const s32, "code_0023E1D0", func_r1_002277C8);

void effSetFadeVectorColor(EffFadeVectorWork *work, u32 color) {
    work->color = color;
}

void effResetSelectionEntryBuffers(SdfFlagListWork *work) {
    u32 count;
    SdfFlagListMark *entries;
    u32 index;

    index = 0;
    count = work->params.count;
    entries = work->marks;
    if (count != 0) {
        do {
            index = index + 1;
            entries->timer = -1;
            entries++;
        } while (index < count);
    }
    memset(work->colors, 0, count << 3);
}

SdfFlagListWork *effCreateSelectionFlagListFromWork(const SdfFlagListParams *work) {
    return sdfCreateFlagListWork(work);
}

SdfFlagListWork *effCreateSelectionFlagListFromFile(void *file) {
    return sdfInitializeFlagListFromResource(file);
}

void effReleaseSelectionFlagList(SdfFlagListWork *work) {
    sdfReleaseFlagListResource(work);
}

SdfFlagListWork *effCreateEmbeddedSelectionFlagList(SdfFlagListWork *work) {
    return effCreateSelectionFlagListFromWork(&work->params);
}

void effResetSelectionEntriesAndState(SdfFlagListWork *p) {
    effResetSelectionEntryBuffers(p);
    p->frame = 0;
}

void effUpdateSelectionEntryState(SdfFlagListWork *work) {
    sdfUpdateFlagListEntries(work);
}

void effDrawSelectionEntryVectors(SdfFlagListWork *work) {
    sdfDrawFlagListVertices(work);
}

INCLUDE_ASM(const s32, "code_0023E1D0", func_r1_002278F0);

void effSetSelectionFlagListColor(SdfFlagListWork *work, u32 color) {
    work->unk04 = color;
}

EffSelectionHeaderWork *effDuplicatePayloadHeader(const void *source) {
    EffSelectionHeaderWork *buffer = sdfChipAllocDirect(sizeof(EffSelectionHeaderWork));
    buffer->frame = 0;
    memcpy(buffer->payload, source, sizeof(buffer->payload));
    return buffer;
}

EffSelectionHeaderWork *effCreateSelectionHeaderFromFile(void *work) {
    void *resource;

    resource = fileResolvePrimaryBuffer((FileJobPayload *)work);
    return effDuplicatePayloadHeader(resource);
}

void effReleaseSelectionHeaderAllocation(EffSelectionHeaderWork *allocation) {
    evtDestroySelectionState();
    sdfChipFreeDirect(allocation);
}

EffSelectionHeaderWork *effCloneSelectionHeaderFromWork(EffSelectionHeaderWork *work) {
    return effDuplicatePayloadHeader(work->payload);
}

void effResetSelectionHeaderFrame(EffSelectionHeaderWork *work) {
    work->frame = 0;
}

void effSelectionFrameAdvance(EffSelectionHeaderWork *counter) {
    s32 frame;

    frame = counter->frame;
    if (frame == 0) {
        evtSelStateCreate(counter->payload[0], (s16)counter->payload[1], counter->payload[2], counter->payload[3]);
        frame = counter->frame;
    }
    counter->frame = frame + 1;
}

/* Draw through the configured duration; zero duration uses frame zero.
 * Blend the packed color and use percent-scaled curves for the output rates. */
void effUpdateFadeBlendA(EffKindWork *work) {
    EffRateConfig *config = (EffRateConfig *)work->payload;
    s32 duration = config->duration;
    s32 frame = 0;
    dds3Effect_Blur_t *out = &config->out;
    s32 color1[4];
    s32 color2[4];
    s32 blended[4];
    u32 packed;
    u32 unit;
    u32 second;

    if (duration != 0) {
        frame = work->frame;
    }
    if (duration < frame) {
        return;
    }
    out->center[0] = 0;
    out->center[1] = 0;
    out->xy[0][0] = 0;
    out->xy[0][1] = 0;
    out->xy[1][0] = 0x200;
    out->xy[1][1] = 0x1C0;
    second = effSampleColorAlphaTracks(&config->blendA, &config->blendB2, frame, duration);
    color1[0] = work->color;
    unit = 0x3C000000;
    EE_MMI_RGBA_UNPACK(color1, unit);
    SDF_VU0_COPYVECTOR_A_TO_B();
    color2[0] = second;
    EE_MMI_RGBA_UNPACK(color2, unit);
    SDF_VU0_MULVECTOR();
    EE_MMI_RGBA_PACK(packed);
    blended[0] = packed;
    out->abgr = blended[0];
    out->scale = effSampleScalarCurve(&config->blendB, frame, duration) * 0.01f + 1.0f;
    out->rotate = effSampleScalarCurve(&config->rateA.curve, frame, duration) * 0.01f;
    out->blend = work->mode;
    effDrawBlurRectangle(out);
}

void *effCreateFadeBlendWorkFromOutput(void *work) {
    return dds3EffectPartialMultiBlurCreate((dds3Effect_PartialMultiBlurParam_t *)((u8 *)work + 0xc0));
}

void effReleaseFadeBlendWork(void *resourceHandle) {
    dds3EffectPartialMultiBlurDelete((EffBlurTemplate *)resourceHandle);
}

/* Draw the fade in fixed subpixel units, either centered or at the projected position.
 * Projected X/Y use sixteen units per pixel; output Y is doubled. */
void effUpdateProjectedBlurFadeRectangle(EffKindWork *work) {
    EffRateConfig *config = (EffRateConfig *)work->payload;
    EffBlurTemplate *out = (EffBlurTemplate *)work->handle;
    s32 duration = config->duration;
    s32 frame = 0;
    f32 rate;
    f32 pos[4];
    s32 color1[4];
    s32 color2[4];
    s32 blended[4];
    u32 packed;
    u32 unit;
    u32 second;

    if (duration != 0) {
        frame = work->frame;
    }
    if (duration < frame) {
        return;
    }
    rate = effSampleScalarCurve(&config->rateB.curve, frame, duration);
    if (config->fixedMode != 0) {
        out->body.Blur.center[0] = 0;
        out->body.Blur.center[1] = 0;
        out->body.area = (s32)(rate * 16.0f);
    } else {
        s32 mode;
        s32 px;
        s32 py;

        rate *= work->scale;
        SDF_VU0_LOADVECTOR(work);
        mode = (s32)(mnuMeasureProjectedPerpendicularDistance(rate) * 16.0f);
        out->body.area = mode;
        if (mode == 0) {
            return;
        }
        SDF_VU0_STOREVECTOR_NOMEM(pos);
        py = (s32)(pos[1] * 16.0f) - 0x8000;
        px = (s32)(pos[0] * 16.0f) - 0x8000;
        out->body.Blur.center[0] = px;
        out->body.Blur.center[1] = py << 1;
    }
    second = effSampleColorAlphaTracks(&config->blendA, &config->blendB2, frame, duration);
    color1[0] = work->color;
    unit = 0x3C000000;
    EE_MMI_RGBA_UNPACK(color1, unit);
    SDF_VU0_COPYVECTOR_A_TO_B();
    color2[0] = second;
    EE_MMI_RGBA_UNPACK(color2, unit);
    SDF_VU0_MULVECTOR();
    EE_MMI_RGBA_PACK(packed);
    blended[0] = packed;
    out->body.Blur.abgr = blended[0];
    out->body.Blur.scale = effSampleScalarCurve(&config->blendB, frame, duration) * 0.01f + 1.0f;
    out->body.Blur.rotate = effSampleScalarCurve(&config->rateA.curve, frame, duration) * 0.01f;
    out->body.Blur.blend = work->mode;
    effDrawBlurFixedPointRectangle(out);
}

void effUpdateTarget(EffKindWork *work, sdfTexHandle_t *target) {
    sdfTexHandle_t *previous = work->target;
    if (previous != 0 && previous != target) {
        if (work->sourceKind != 4) {
            sdfTexFreeDelayed(previous);
        }
        work->target = target;
    }
    ((EffBlurTemplate *)work->handle)->texture = target;
}

void *effCreateFixedSlotBlurWorkFromFadeOutput(void *source) {
    return dds3EffectDistortionBlurCreate((dds3Effect_DistortionBlurParam_t *)((u8 *)source + 0xC0));
}

void effReleaseFixedSlotBlurWork(EffBlurScatterWork *handle) {
    dds3EffectDistortionBlurDelete(handle);
}

/* Draw the pixel-unit fade using the handle's output record and payload's curves.
 * Zero projected mode suppresses drawing; the origin and Y scaling differ from subpixels. */
void effUpdateFadeMapA(EffKindWork *work) {
    EffRateConfig *config = (EffRateConfig *)work->payload;
    EffBlurScatterWork *out = (EffBlurScatterWork *)work->handle;
    s32 duration = config->duration;
    s32 frame = 0;
    f32 rate;
    f32 pos[4];
    s32 color1[4];
    s32 color2[4];
    s32 blended[4];
    u32 packed;
    u32 unit;
    u32 second;

    if (duration != 0) {
        frame = work->frame;
    }
    if (duration < frame) {
        return;
    }
    rate = effSampleScalarCurve(&config->rateB.curve, frame, duration);
    if (config->fixedMode != 0) {
        out->params.area = (s32)rate;
        out->params.center[0] = 0;
        out->params.center[1] = 0;
    } else {
        s32 mode;

        rate *= work->scale;
        SDF_VU0_LOADVECTOR(work);
        mode = (s32)mnuMeasureProjectedPerpendicularDistance(rate);
        out->params.area = mode;
        if (mode == 0) {
            return;
        }
        SDF_VU0_STOREVECTOR(pos);
        out->params.center[0] = (s32)pos[0] - 0x800;
        out->params.center[1] = ((s32)pos[1] - 0x800) << 1;
    }
    second = effSampleColorAlphaTracks(&config->blendA, &config->blendB2, frame, duration);
    color1[0] = work->color;
    unit = 0x3C000000;
    EE_MMI_RGBA_UNPACK(color1, unit);
    SDF_VU0_COPYVECTOR_A_TO_B();
    color2[0] = second;
    EE_MMI_RGBA_UNPACK(color2, unit);
    SDF_VU0_MULVECTOR();
    EE_MMI_RGBA_PACK(packed);
    blended[0] = packed;
    out->params.abgr = blended[0];
    out->params.distortion = effSampleScalarCurve(&config->blendB, frame, duration) * 0.01f;
    out->params.rotate = effSampleScalarCurve(&config->rateA.curve, frame, duration) * 0.01f;
    out->params.blend = work->mode;
    effBlurStepScatterSlotsAndDraw(out);
}

void effTextureReferenceRelease(EffKindWork *work, sdfTexHandle_t *target) {
    sdfTexHandle_t *previous = work->target;
    if (previous != 0 && previous != target) {
        if (work->sourceKind != 4) {
            sdfTexFreeDelayed(previous);
        }
        work->target = target;
    }
    ((EffBlurScatterWork *)work->handle)->texture = target;
}

void *effCreateVariableSlotBlurWorkFromFadeOutput(void *source) {
    return dds3EffectRippleBlurCreate((dds3Effect_RippleBlurParam_t *)((u8 *)source + 0xc0));
}

void effReleaseVariableSlotBlurWork(EffBlurScaleWork *work) {
    dds3EffectRippleBlurDelete(work);
}

/* Draw the same pixel-unit fade into the wider renderer output record.
 * The native work header and duration gate remain shared with the other kind callbacks. */
void effUpdateFadeMapB(EffKindWork *work) {
    EffRateConfig *config = (EffRateConfig *)work->payload;
    EffBlurScaleWork *out = (EffBlurScaleWork *)work->handle;
    s32 duration = config->duration;
    s32 frame = 0;
    f32 rate;
    f32 pos[4];
    s32 color1[4];
    s32 color2[4];
    s32 blended[4];
    u32 packed;
    u32 unit;
    u32 second;

    if (duration != 0) {
        frame = work->frame;
    }
    if (duration < frame) {
        return;
    }
    rate = effSampleScalarCurve(&config->rateB.curve, frame, duration);
    if (config->fixedMode != 0) {
        out->params.area = (s32)rate;
        out->params.center[0] = 0;
        out->params.center[1] = 0;
    } else {
        s32 mode;

        rate *= work->scale;
        SDF_VU0_LOADVECTOR(work);
        mode = (s32)mnuMeasureProjectedPerpendicularDistance(rate);
        out->params.area = mode;
        if (mode == 0) {
            return;
        }
        SDF_VU0_STOREVECTOR(pos);
        out->params.center[0] = (s32)pos[0] - 0x800;
        out->params.center[1] = ((s32)pos[1] - 0x800) << 1;
    }
    second = effSampleColorAlphaTracks(&config->blendA, &config->blendB2, frame, duration);
    color1[0] = work->color;
    unit = 0x3C000000;
    EE_MMI_RGBA_UNPACK(color1, unit);
    SDF_VU0_COPYVECTOR_A_TO_B();
    color2[0] = second;
    EE_MMI_RGBA_UNPACK(color2, unit);
    SDF_VU0_MULVECTOR();
    EE_MMI_RGBA_PACK(packed);
    blended[0] = packed;
    out->params.abgr = blended[0];
    out->params.frequency = effSampleScalarCurve(&config->blendB, frame, duration) * 0.01f;
    out->params.rotate = effSampleScalarCurve(&config->rateA.curve, frame, duration) * 0.01f;
    out->params.blend = work->mode;
    effBlurStepScaleSlotsAndDraw(out);
}

void effReplaceKindLinkedTarget(EffKindWork *work, sdfTexHandle_t *target) {
    sdfTexHandle_t *previous = work->target;
    if (previous != 0 && previous != target) {
        if (work->sourceKind != 4) {
            sdfTexFreeDelayed(previous);
        }
        work->target = target;
    }
    ((EffBlurScaleWork *)work->handle)->texture = target;
}

/* Draw a framebuffer fade using raw curve rates rather than percent-scaled rates.
 * The generic kind work supplies its packed color, mode and frame limit. */
void effUpdateFadeBlendB(EffKindWork *work) {
    EffRateConfig *config = (EffRateConfig *)work->payload;
    s32 duration = config->duration;
    s32 frame = 0;
    dds3Effect_Blur_t *out = &config->out;
    s32 color1[4];
    s32 color2[4];
    s32 blended[4];
    u32 packed;
    u32 unit;
    u32 second;

    if (duration != 0) {
        frame = work->frame;
    }
    if (duration < frame) {
        return;
    }
    out->center[0] = 0;
    out->center[1] = 0;
    out->xy[0][0] = 0;
    out->xy[0][1] = 0;
    out->xy[1][0] = 0x200;
    out->xy[1][1] = 0x1C0;
    second = effSampleColorAlphaTracks(&config->blendA, &config->blendB2, frame, duration);
    color1[0] = work->color;
    unit = 0x3C000000;
    EE_MMI_RGBA_UNPACK(color1, unit);
    SDF_VU0_COPYVECTOR_A_TO_B();
    color2[0] = second;
    EE_MMI_RGBA_UNPACK(color2, unit);
    SDF_VU0_MULVECTOR();
    EE_MMI_RGBA_PACK(packed);
    blended[0] = packed;
    out->abgr = blended[0];
    out->scale = effSampleScalarCurve(&config->blendB, frame, duration) + 1.0f;
    out->rotate = effSampleScalarCurve(&config->rateA.curve, frame, duration);
    out->blend = work->mode;
    effBlurDrawFramebufferQuad(out);
}

/* Draw the color-only fade from the copied payload's curves.
 * Its output uses fixed rectangle bounds and the kind work's packed color and mode. */
void effUpdateFadeBlendC(EffKindWork *work) {
    EffFadeConfig *config = work->payload;
    s32 duration = config->duration;
    s32 frame = 0;
    dds3Effect_Filter_t *out = &config->out;
    u32 packed;
    u32 unit;
    u32 second;
    u32 baseColor;

    if (duration != 0) {
        frame = work->frame;
    }
    if (duration < frame) {
        return;
    }
    out->xy[0][0] = 0;
    out->xy[0][1] = 0;
    out->xy[1][0] = 0x200;
    out->xy[1][1] = 0x1C0;
    second = effSampleColorAlphaTracks(&config->blendA, &config->blendB2, frame, duration);
    /* Capture the base word before the unit: see docs/idioms.md,
     * "Trace-justified base color capture: effUpdateFadeBlendC". */
    baseColor = work->color;
    unit = 0x3C000000;
    eeUnpackRGBAWord(baseColor, unit);
    SDF_VU0_COPYVECTOR_A_TO_B();
    eeUnpackRGBAWord(second, unit);
    SDF_VU0_MULVECTOR();
    packed = eePackRGBAWord(128.0f);
    out->abgr = packed;
    out->blend = work->mode;
    dds3EffectFilter(out);
}

void *effCreateFadeColorWorkFromOutput(void *work) {
    return dds3EffectPartialMultiFilterCreate((dds3Effect_PartialMultiFilterParam_t *)((u8 *)work + 0xc0));
}

void effReleaseFadeColorWork(EffResourceRectWork *work) {
    dds3EffectPartialMultiFilterDelete(work);
}

/* Update the fade rectangle's projected extent, center and multiplied tint. */
void effUpdateProjectedFadeColor(EffKindWork *work) {
    EffRateConfig *config = (EffRateConfig *)work->payload;
    EffResourceRectWork *out = (EffResourceRectWork *)work->handle;
    s32 duration = config->duration;
    s32 frame = 0;
    f32 rate;
    f32 pos[4];
    s32 color1[4];
    s32 color2[4];
    s32 blended[4];
    u32 packed;
    u32 second;

    if (duration != 0) {
        frame = work->frame;
    }
    if (duration < frame) {
        return;
    }
    rate = effSampleScalarCurve(&config->rateB.curve, frame, duration);
    if (config->fixedMode != 0) {
        out->params.center[0] = 0;
        out->params.center[1] = 0;
        out->params.area = (s32)(rate * 16.0f);
    } else {
        s32 mode;
        s32 px;
        s32 py;

        rate *= work->scale;
        SDF_VU0_LOADVECTOR(work);
        mode = (s32)(mnuMeasureProjectedPerpendicularDistance(rate) * 16.0f);
        out->params.area = mode;
        if (mode == 0) {
            return;
        }
        SDF_VU0_STOREVECTOR_NOMEM(pos);
        py = (s32)(pos[1] * 16.0f) - 0x8000;
        px = (s32)(pos[0] * 16.0f) - 0x8000;
        out->params.center[0] = px;
        out->params.center[1] = py << 1;
    }
    second = effSampleColorAlphaTracks(&config->blendA, &config->blendB2, frame, duration);
    color1[0] = work->color;
    EE_MMI_RGBA_UNPACK(color1, 1.0f / 128.0f);
    SDF_VU0_COPYVECTOR_A_TO_B();
    color2[0] = second;
    EE_MMI_RGBA_UNPACK(color2, 1.0f / 128.0f);
    SDF_VU0_MULVECTOR();
    EE_MMI_RGBA_PACK_F128(packed);
    blended[0] = packed;
    *(u32 *)out->params.Filter.color = blended[0];
    out->params.Filter.blendControl = work->mode;
    effComputeBlurRectBounds(out);
}

void effReplaceLinkedKindWorkTarget(EffKindWork *work, sdfTexHandle_t *target) {
    sdfTexHandle_t *previous = work->target;
    if (previous != 0 && previous != target) {
        if (work->sourceKind != 4) {
            sdfTexFreeDelayed(previous);
        }
        work->target = target;
    }
    ((EffResourceRectWork *)work->handle)->texture = target;
}

EffKindWork *effAllocateKindWork(u16 kind, u8 *source) {
    u32 headerSize = 0x40;
    u32 size = D_r1_00403080[kind].payloadSize;
    EffKindWork *work = sdfChipAllocZeroDirect(size + headerSize);

    work->kind = kind;
    work->target = 0;
    work->color = 0x80808080;
    work->scale = 1.0f;
    VU0_STORE_VF($vf0, work);
    work->payload = (u8 *)work + headerSize;
    memcpy(work->payload, source, size);
    switch (((EffKindSource *)source)->modeKind) {
    case 1:
        work->mode = 0x44;
        break;
    case 2:
        work->mode = 0x48;
        break;
    case 3:
    case 4:
        work->mode = 0x42;
        break;
    case 5:
    case 6:
        work->mode = 6;
        break;
    }
    if (D_r1_00403080[kind].create != NULL) {
        work->handle = D_r1_00403080[kind].create(source);
    }
    return work;
}

EffKindWork *effCreateKindWorkFromFile(FileJobPayload *work) {
    void *source = fileResolvePrimaryBuffer(work);
    EffKindWork *effect = effAllocateKindWork(work->option, source);
    u32 *secondary = fileResolveSecondaryBuffer(work);

    if (secondary != NULL) {
        if (D_r1_00403080[effect->kind].initialize != NULL) {
            u32 kind = work->primary.selector;
            effect->sourceKind = kind;
            switch (kind) {
            case 1:
                effect->target = sdfTexSet((SdfTextureFileHeader *)(secondary));
                break;
            case 4:
                effect->target = dds3BillboardGetGeneralTex(secondary[0]);
                break;
            }
            D_r1_00403080[effect->kind].initialize(effect, effect->target);
        }
    }
    return effect;
}

void effReleaseLinkedTarget(EffKindWork *work) {
    void *handle = work->handle;
    if (handle != NULL) {
        D_r1_00403080[work->kind].destroy(handle);
    }
    if (work->target != 0 && work->sourceKind != 4) {
        sdfTexFreeDelayed(work->target);
    }
    sdfChipFreeDirect(work);
}

EffKindWork *effCloneKindWork(EffKindWork *work) {
    return effAllocateKindWork((u16)work->kind, work->payload);
}

void effResetKindWorkFrame(EffKindWork *work) {
    work->frame = 0;
}

INCLUDE_ASM(const s32, "code_0023E1D0", func_r1_00228A80);

INCLUDE_ASM(const s32, "code_0023E1D0", func_r1_00228AE8);

void effSetKindWorkColor(EffKindWork *work, u32 color) {
    work->color = color;
}

void effSetKindWorkScale(EffKindWork *work, f32 scale) {
    work->scale = scale;
}

/* Allocate work for the alternate kind table, copying its payload and creating its handle. */
EffKindWork *effAllocateAlternateKindWork(u16 kind, u8 *source) {
    u32 headerSize = 0x40;
    u32 size = D_r1_004030F8[kind].payloadSize;
    EffKindWork *work = sdfChipAllocZeroDirect(size + headerSize);

    work->kind = kind;
    work->target = 0;
    work->color = 0x80808080;
    work->scale = 1.0f;
    VU0_STORE_VF($vf0, work);
    work->payload = (u8 *)work + headerSize;
    memcpy(work->payload, source, size);
    switch (((EffKindSource *)source)->modeKind) {
    case 1:
        work->mode = 0x44;
        break;
    case 2:
        work->mode = 0x48;
        break;
    case 3:
    case 4:
        work->mode = 0x42;
        break;
    case 5:
    case 6:
        work->mode = 6;
        break;
    }
    if (D_r1_004030F8[kind].create != NULL) {
        work->handle = D_r1_004030F8[kind].create(source);
    }
    return work;
}

EffKindWork *effCreateKindWorkFromFileB(FileJobPayload *work) {
    void *source = fileResolvePrimaryBuffer(work);
    EffKindWork *effect = effAllocateAlternateKindWork(work->option, source);
    u32 *secondary = fileResolveSecondaryBuffer(work);

    if (secondary != NULL) {
        if (D_r1_004030F8[effect->kind].initialize != NULL) {
            u32 kind = work->primary.selector;
            effect->sourceKind = kind;
            switch (kind) {
            case 1:
                effect->target = sdfTexSet((SdfTextureFileHeader *)(secondary));
                break;
            case 4:
                effect->target = dds3BillboardGetGeneralTex(secondary[0]);
                break;
            }
            D_r1_004030F8[effect->kind].initialize(effect, effect->target);
        }
    }
    return effect;
}

/* Release the alternate-kind handle, owned secondary reference, and work allocation. */
void effReleaseAlternateKindWork(EffKindWork *work) {
    void *handle = work->handle;
    if (handle != NULL) {
        D_r1_004030F8[work->kind].destroy(handle);
    }
    if (work->target != 0 && work->sourceKind != 4) {
        sdfTexFreeDelayed(work->target);
    }
    sdfChipFreeDirect(work);
}

/* Reallocate the source kind and payload; this path does not copy its secondary target. */
EffKindWork *effCloneAlternateKindWork(EffKindWork *work) {
    return effAllocateAlternateKindWork((u16)work->kind, work->payload);
}

void effResetAlternateKindWorkFrame(EffKindWork *work) {
    work->frame = 0;
}

INCLUDE_ASM(const s32, "code_0023E1D0", func_r1_00228DB8);

INCLUDE_ASM(const s32, "code_0023E1D0", func_r1_00228E20);

void effSetAlternateKindWorkColor(EffKindWork *work, u32 color) {
    work->color = color;
}

void effSetAlternateKindWorkScale(EffKindWork *work, f32 scale) {
    work->scale = scale;
}

u8 *effCreateBillboardWork(FileJobPayload *source) {
    u8 *work = (u8 *)sdfChipAllocDirect(0x68);
    memset(work, 0, 0x68);
    ((EffBillboardWork *)work)->frame = 0;
    ((EffBillboardWork *)work)->color = 0x80808080;
    ((EffBillboardWork *)work)->scale = 1.0f;
    VU0_STORE_VF(vf0, work);
    VU0_STORE_VF(vf0, work + 0x10);
    if (source == NULL) {
        return work;
    }
    memcpy(work + 0x2C, fileResolvePrimaryBuffer(source),
           source->primary.size);
    ((EffBillboardWork *)work)->billboard =
        dds3BillboardCreate(1, (u32)fileResolveSecondaryBuffer(source));
    return work;
}

void effBillboardWorkRelease(EffBillboardWork *work) {
    billHandle_t *billboard;

    billboard = work->billboard;
    if (billboard != 0) {
        dds3BillboardDelete(billboard);
    }
    sdfChipFreeDirect(work);
}

u8 *effDuplicateBillState(const u8 *source) {
    u8 *effect = (u8 *)effCreateBillboardWork(NULL);
    memcpy(effect + 0x2C, source + 0x2C, 0x38);
    effReplaceBillboardClone((s32)effect, (s32)source);
    return effect;
}

void effReplaceBillboardClone(s32 dst, s32 src) {
    billHandle_t *billboard;

    if (((EffBillboardWork *)dst)->billboard != 0) {
        dds3BillboardDelete(((EffBillboardWork *)dst)->billboard);
    }
    billboard = dds3BillboardCopy(((EffBillboardWork *)src)->billboard);
    ((EffBillboardWork *)dst)->billboard = billboard;
}

void effBillboardEntryFrameReset(EffBillboardWork *work) {
    dds3BillboardSetAnimKey2(work->billboard, 0);
    work->frame = 0;
}

INCLUDE_ASM(const s32, "code_0023E1D0", func_r1_00229060);

INCLUDE_ASM(const s32, "code_0023E1D0", func_r1_00229168);

INCLUDE_ASM(const s32, "code_0023E1D0", func_r1_00229178);

void effSetBillboardColor(EffBillboardWork *work, u32 color) {
    work->color = color;
}

void effSetBillboardScale(EffBillboardWork *work, f32 scale) {
    work->scale = scale;
}

void effClearBillFrames(u8 *work) {
    u8 *state = (u8 *)((EffClassWork *)work)->resource;
    u8 *config = ((EffClassWork *)work)->payload;
    EffTrackSet *asset = ((EffBillFrameState *)state)->asset;
    s32 remaining = ((EffBillTimedHeader *)config)->count;
    u8 *entry = ((EffBillFrameState *)state)->entries;

    memset(asset->buffer, 0, asset->rows * 0x10);
    if (remaining > 0) {
        s32 i;
        for (i = remaining; i != 0; --i) {
            *(s32 *)entry = -1;
            entry += 0x18;
        }
    }
}

u8 *effCreateBillFrameNode(EffBillFrameConfig *config, u32 resource) {
    s32 count = config->frame.output.timed.count;
    u32 headerSize = 0x10;
    struct sdfMemHandle_t *base = sdfMemAllocDirect(count * 0x18 + headerSize);
    u8 *body = (u8 *)sdfMemLock(base);
    u8 *node = body;

    body += headerSize;
    ((EffBillFrameState *)node)->allocation = base;
    *(u8 **)node = body;
    ((EffBillFrameState *)node)->asset = effCreateTrackSetWithSharedReferences(count, 0, resource);
    return node;
}

/* Release the frame node's shared tracks and backing allocation. */
void effReleaseBillFrameNode(s32 work) {
    effReleaseResourceRefs(((EffBillFrameState *)work)->asset);
    sdfMemFreeDirect(((EffBillFrameState *)work)->allocation);
}

void billAdvanceFrameInstances(EffClassWork *work) {
    EffBillFrameConfig *config = (EffBillFrameConfig *)work->payload;
    EffBillFrameState *resource;
    EffTrackSet *asset;
    EffBillFrameEntry *entries;
    s32 lifetime;
    s32 remainingSpawn;
    u32 randomAge;
    s32 item;
    s32 itemCount;
    s32 fadeStart;
    s32 fadeEnd;
    s32 retiredState;
    s32 waitingState;
    s32 inactiveState;
    u32 respawn;
    u32 innerColor;
    u32 outerColor;
    u32 *colors;
    u128 *vertices;
    f32 innerVector[4];
    f32 outerVector[4];
    f32 direction[4];
    f32 radius[4];
    f32 innerWidth[4];
    f32 outerWidth[4];
    f32 acceleration;

    if (config->frame.output.timed.time.progress < work->frame &&
        config->frame.output.timed.time.progress != 0) return;
    resource = (EffBillFrameState *)work->resource;
    lifetime = config->frame.lifetime;
    entries = (EffBillFrameEntry *)resource->entries;
    asset = resource->asset;
    itemCount = (s32)config->frame.output.timed.count;
    if (lifetime == 0) return;
    if (config->frame.spawnAllOnStart != 0 && work->frame == 0) {
        remainingSpawn = itemCount;
        randomAge = 1;
    } else {
        remainingSpawn = config->frame.spawnCount;
        randomAge = 0;
    }
    innerColor = config->innerColor & 0xFFFFFF;
    vertices = (u128 *)asset->buffer;
    colors = (u32 *)asset->tail;
    direction[1] = 0.0f;
    direction[3] = 0.0f;
    eeUnpackRGBAWordFloat(innerColor, 1.0f / 128.0f);
    SDF_VU0_STOREVECTOR_NOMEM(innerVector);
    outerColor = config->outerColor & 0xFFFFFF;
    eeUnpackRGBAWordFloat(outerColor, 1.0f / 128.0f);
    SDF_VU0_STOREVECTOR_NOMEM(outerVector);
    fadeStart = (s32)(config->frame.fadeInFraction * (f32)lifetime);
    fadeEnd = (s32)(config->frame.fadeOutFraction * (f32)lifetime);
    acceleration = config->spinAccel;
    respawn = config->frame.respawn;
    item = 0;
    if (itemCount <= 0) return;
    waitingState = -1;
    inactiveState = -2;
    retiredState = respawn != 0 ? waitingState : inactiveState;
    for (; item < itemCount; item++, entries++, vertices += 5, colors += 5) {
        EffBillFrameEntry *entry = entries;
        s32 age = entry->age;
        if (age == inactiveState) continue;
        if (age == waitingState) {
            if (remainingSpawn != 0) {
                f32 randomValue;
                f32 factor;
                entry->angle = dds3GetRandFloatA(effSharedRandomState) * 6.28318500518798828125f;
                randomValue = dds3GetRandFloatA(effSharedRandomState);
                entry->spin = config->spin * (randomValue * config->spinJitter + (1.0f - config->spinJitter));
                if (dds3GetRandA((EffRandState *)effSharedRandomState) & 1) entry->spin = -entry->spin;
                randomValue = dds3GetRandFloatA(effSharedRandomState);
                entry->radius = config->radius * (randomValue * config->radiusJitter + (1.0f - config->radiusJitter));
                randomValue = dds3GetRandFloatA(effSharedRandomState);
                factor = randomValue * config->widthJitter + (1.0f - config->widthJitter);
                entry->innerWidth = config->innerWidth * factor;
                entry->outerWidth = config->outerWidth * factor;
                if (randomAge != 0) entry->age = (s32)(dds3GetRandA((EffRandState *)effSharedRandomState) % (u32)lifetime);
                else entry->age = 0;
                remainingSpawn = (s32)((u32)remainingSpawn - 1u);
            }
        } else if (age >= lifetime) {
            u32 *color = colors;
            u128 *position = vertices;
            s32 count = 4;
            do {
                VU0_STORE_VF(vf0, position);
                *color = 0;
                position++;
                color++;
            } while (--count >= 0);
            entry->age = retiredState;
        } else {
            f32 spin = entry->spin;
            f32 time = (f32)age;
            f32 angle;
            f32 cosine;
            f32 sine;
            f32 alpha;
            u32 outerPacked;
            u32 innerPacked;
            if (spin > 0.0f) angle = (spin + (acceleration * time) * 0.5f) * time;
            else angle = (spin - (acceleration * time) * 0.5f) * time;
            angle += entry->angle;
            SDF_VU0_LOADVECTOR(effOnesVector);
            SDF_VU0_COPYVECTOR_A_TO_B();
            VU0_SCALE_VF(vf10, entry->radius);
            SDF_VU0_STOREVECTOR_NOMEM(radius);
            SDF_VU0_COPYVECTOR_B_TO_A();
            VU0_SCALE_VF(vf10, entry->innerWidth);
            VU0_SCALE_VF(vf11, entry->outerWidth);
            SDF_VU0_STOREVECTOR_NOMEM(innerWidth);
            SDF_VU0_STOREVECTOR_B_NOMEM(outerWidth);
            direction[0] = sdfCos(angle);
            direction[2] = sdfSin(angle);
            SDF_VU0_LOADVECTOR(direction);
            SDF_VU0_COPYVECTOR_A_TO_C();
            SDF_VU0_GETVECTOR_X(cosine);
            SDF_VU0_GETVECTOR_Z(sine);
            VU0_SCALAR_OP(sine, "vaddx.x vf10, vf0, vf2x");
            cosine = -cosine;
            VU0_SCALAR_OP(cosine, "vaddx.z vf10, vf0, vf2x");
            SDF_VU0_COPYVECTOR_A_TO_B();
            SDF_VU0_LOADVECTOR(innerWidth);
            SDF_VU0_MULVECTOR();
            SDF_VU0_STOREVECTOR_NOMEM(innerWidth);
            SDF_VU0_LOADVECTOR(outerWidth);
            SDF_VU0_MULVECTOR();
            SDF_VU0_STOREVECTOR_NOMEM(outerWidth);
            SDF_VU0_COPYVECTOR_C_TO_A();
            SDF_VU0_LOADVECTOR_B(radius);
            SDF_VU0_MULVECTOR();
            SDF_VU0_COPYVECTOR_A_TO_C();
            SDF_VU0_LOADVECTOR(innerWidth);
            SDF_VU0_COPYVECTOR_A_TO_B();
            SDF_VU0_STOREVECTOR(vertices + 2);
            VU0_NEGATE_XYZ(vf11);
            SDF_VU0_STOREVECTOR_B(vertices + 4);
            SDF_VU0_LOADVECTOR(outerWidth);
            SDF_VU0_COPYVECTOR_A_TO_B();
            SDF_VU0_ADDVECTOR_A_C();
            VU0_NEGATE_XYZ(vf11);
            SDF_VU0_STOREVECTOR(vertices);
            SDF_VU0_ADDVECTOR_B_C();
            SDF_VU0_STOREVECTOR_B(vertices + 1);
            if (age < fadeStart) alpha = time / (f32)fadeStart;
            else if (fadeEnd < age) alpha = (f32)(s32)((u32)lifetime - (u32)age) / (f32)(s32)((u32)lifetime - (u32)fadeEnd);
            else alpha = 1.0f;
            SDF_VU0_LOADVECTOR_B(effOnesVector);
            VU0_SCALE_VF(vf11, alpha);
            SDF_VU0_LOADVECTOR(outerVector);
            SDF_VU0_MULVECTOR();
            outerPacked = eePackRGBAWord(128.0f);
            colors[0] = outerPacked;
            colors[1] = outerPacked;
            SDF_VU0_LOADVECTOR(innerVector);
            SDF_VU0_MULVECTOR();
            innerPacked = eePackRGBAWord(128.0f);
            if (item & 1) {
                colors[3] = innerPacked | 0xFF000000;
                innerPacked = (u32)(alpha * 128.0f) << 24;
                colors[2] = innerPacked;
                colors[4] = innerPacked;
            } else {
                colors[3] = innerPacked | 0x40000000;
                innerPacked = (u32)(alpha * 255.0f) << 24;
                colors[2] = innerPacked;
                colors[4] = innerPacked;
            }
            entry->age = (s32)((u32)age + 1u);
        }
    }
}

void billUpdateFrameDrawColorAndTransform(BillCellDrawWork *work) {
    u8 *config = work->config;
    u32 limit = work->frameLimit;
    u32 progress = ((EffBillTimedHeader *)config)->time.progress;
    u32 *list = work->instances;
    EffTrackSet *out = (EffTrackSet *)list[1];
    u128 mtx[4];
    u32 packed;
    u32 unit;
    u32 second;

    if (progress < limit && progress != 0) {
        return;
    }
    second = effSampleColorAlphaTracks(&((EffBillTimedHeader *)config)->colorTrack, &((EffBillTimedHeader *)config)->alphaTrack, limit, progress);
    unit = 0x3C000000;
    eeUnpackRGBAWord(work->baseColor, unit);
    SDF_VU0_COPYVECTOR_A_TO_B();
    eeUnpackRGBAWord(second, unit);
    SDF_VU0_MULVECTOR();
    packed = eePackRGBAWord(128.0f);
    out->color = packed;
    out->type = ((EffBillTimedHeader *)config)->alphaTrack.surfaceIndex;
    out->flag = ((EffBillFrameHeader *)config)->mode;
    SDF_VU0_LOADVECTOR(work->transform);
    ToMatrix();
    SDF_VU0_LOADVECTOR(effOnesVector);
    VU0_SCALAR_OP_CLOBBER(work->scale, "vmulx.xyzw vf10, vf10, vf2x");
    SDF_VU0_SCALE_XYZ();
    SDF_VU0_LOADVECTOR(work);
    SDF_VU0_VECTOR_W_SET_ONE();
    SDF_VU0_SET_ROW3();
    VU0_STORE_MATRIX(mtx);
    effDrawTrackSet(out, (Matrix4 *)mtx);
}

void billResetCellIndices(u8 *work) {
    u8 *state = (u8 *)((EffClassWork *)work)->resource;
    u8 *config = ((EffClassWork *)work)->payload;
    EffTrackSet *asset = ((EffBillFrameState *)state)->asset;
    s32 remaining = ((EffBillTimedHeader *)config)->count;
    u8 *entry = ((EffBillFrameState *)state)->entries;

    memset(asset->buffer, 0, asset->rows * 0x10);
    if (remaining > 0) {
        s32 i;
        for (i = remaining; i != 0; --i) {
            *(s32 *)entry = -1;
            entry += 0x1C;
        }
    }
}

u8 *billCreateCellNode(EffBillCellConfig *config, u32 resource) {
    s32 count = config->frame.output.timed.count;
    u32 headerSize = 0x10;
    struct sdfMemHandle_t *base = sdfMemAllocDirect(count * 0x1C + headerSize);
    u8 *body = (u8 *)sdfMemLock(base);
    u8 *node = body;

    body += headerSize;
    ((EffBillFrameState *)node)->allocation = base;
    *(u8 **)node = body;
    ((EffBillFrameState *)node)->asset = effCreateTrackSetWithSharedReferences(count, 1, resource);
    return node;
}

void billReleaseCellNode(s32 work) {
    effReleaseResourceRefs(((EffBillFrameState *)work)->asset);
    sdfMemFreeDirect(((EffBillFrameState *)work)->allocation);
}

/* Spawn and advance the kind-2 cell instances: each live cell moves out
 * along its heading and is drawn as a 13-vertex fan (center plus two
 * six-step rings of radii A and B), faded in and out over its lifetime. */
void billAdvanceCellInstances(EffClassWork *work) {
    EffBillCellConfig *config = (EffBillCellConfig *)work->payload;
    EffBillFrameState *resource;
    EffTrackSet *asset;
    EffBillCellEntry *entries;
    s32 lifetime;
    s32 remainingSpawn;
    u32 randomAge;
    s32 item;
    s32 itemCount;
    s32 outputCount;
    s32 fadeStart;
    s32 fadeEnd;
    s32 startAge;
    s32 endAge;
    s32 ageStep;
    s32 retiredState;
    s32 waitingState;
    s32 inactiveState;
    u32 firstColor;
    u32 secondColor;
    u32 respawn;
    u32 *colors;
    u128 *vertices;
    f32 firstVector[4];
    f32 secondVector[4];
    f32 center[4];
    f32 direction[4];
    f32 width[4];
    f32 radialA[4];
    f32 radialB[4];
    f32 acceleration;

    if (config->frame.output.timed.time.progress < work->frame &&
        config->frame.output.timed.time.progress != 0) return;
    resource = (EffBillFrameState *)work->resource;
    lifetime = config->frame.lifetime;
    entries = (EffBillCellEntry *)resource->entries;
    asset = resource->asset;
    itemCount = (s32)config->frame.output.timed.count;
    if (lifetime == 0) return;
    if (config->reverse != 0) {
        fadeStart = (s32)((1.0f - config->frame.fadeOutFraction) * (f32)lifetime);
        fadeEnd = (s32)((1.0f - config->frame.fadeInFraction) * (f32)lifetime);
        startAge = lifetime;
        endAge = 0;
        ageStep = -1;
    } else {
        fadeStart = (s32)(config->frame.fadeInFraction * (f32)lifetime);
        fadeEnd = (s32)(config->frame.fadeOutFraction * (f32)lifetime);
        startAge = 0;
        endAge = lifetime;
        ageStep = 1;
    }
    if (config->frame.spawnAllOnStart != 0 && work->frame == 0) {
        remainingSpawn = itemCount;
        randomAge = 1;
    } else {
        remainingSpawn = config->frame.spawnCount;
        randomAge = 0;
    }
    firstColor = config->colorA & 0xFFFFFF;
    vertices = (u128 *)asset->buffer;
    colors = (u32 *)asset->tail;
    direction[1] = 0.0f;
    direction[3] = 0.0f;
    eeUnpackRGBAWordFloat(firstColor, 1.0f / 128.0f);
    SDF_VU0_STOREVECTOR_NOMEM(firstVector);
    secondColor = config->colorB & 0xFFFFFF;
    eeUnpackRGBAWordFloat(secondColor, 1.0f / 128.0f);
    SDF_VU0_STOREVECTOR_NOMEM(secondVector);
    acceleration = config->acceleration;
    respawn = config->frame.respawn;
    if (itemCount <= 0) return;
    waitingState = -1;
    inactiveState = -2;
    retiredState = respawn != 0 ? waitingState : inactiveState;
    item = itemCount;
    for (; item != 0; item--, entries++, vertices += 13, colors += 13) {
        EffBillCellEntry *entry = entries;
        s32 age = entry->age;
        if (age == inactiveState) continue;
        if (age == waitingState) {
            if (remainingSpawn != 0) {
                f32 randomValue;
                f32 jitter;
                f32 base;
                entry->angle = dds3GetRandFloatA(effSharedRandomState) * 6.28318500518798828125f;
                jitter = config->radiusJitter;
                randomValue = dds3GetRandFloatA(effSharedRandomState);
                base = 1.0f - jitter;
                entry->radiusA = config->radius * (randomValue * jitter + base);
                randomValue = dds3GetRandFloatA(effSharedRandomState);
                entry->radiusB = config->radius * (randomValue * jitter + base);
                jitter = config->centerOffsetJitter;
                randomValue = dds3GetRandFloatA(effSharedRandomState);
                entry->centerOffset = config->centerOffset * (randomValue * jitter + (1.0f - jitter));
                randomValue = dds3GetRandFloatA(effSharedRandomState);
                entry->velocity = config->velocity * (randomValue * config->velocityJitter + (1.0f - config->velocityJitter));
                randomValue = dds3GetRandFloatA(effSharedRandomState);
                jitter = randomValue * config->extentJitter + (1.0f - config->extentJitter);
                entry->extent = config->extent * jitter;
                if (randomAge != 0) entry->age = (s32)(dds3GetRandA((EffRandState *)effSharedRandomState) % (u32)lifetime);
                else entry->age = startAge;
                remainingSpawn = (s32)((u32)remainingSpawn - 1u);
            }
        } else if (age == endAge) {
            u32 *color = colors;
            u128 *position = vertices;
            outputCount = 12;
            do {
                VU0_STORE_VF(vf0, position);
                *color = 0;
                position++;
                color++;
            } while (--outputCount >= 0);
            entry->age = retiredState;
        } else {
            f32 time = (f32)age;
            f32 motionTime = time;
            f32 speed;
            f32 angle = entry->angle;
            f32 distance;
            f32 cosine;
            f32 sine;
            f32 alpha;
            u32 secondPacked;
            u32 firstPacked;
            f32 *ringA;
            f32 *ringB;
            u128 *position;
            u32 *color;
            if (acceleration < 0.0f) {
                f32 stopTime;
                speed = entry->velocity;
                stopTime = (-speed / (acceleration * 0.5f)) * 0.5f;
                if (stopTime < time) motionTime = stopTime;
            } else {
                speed = entry->velocity;
            }
            distance = (speed + (acceleration * motionTime) * 0.5f) * motionTime;
            distance += entry->centerOffset;
            SDF_VU0_LOADVECTOR(effOnesVector);
            VU0_SCALE_VF(vf10, entry->extent);
            SDF_VU0_STOREVECTOR_NOMEM(width);
            direction[0] = sdfCos(angle);
            direction[2] = sdfSin(angle);
            SDF_VU0_LOADVECTOR(direction);
            SDF_VU0_COPYVECTOR_A_TO_C();
            VU0_SCALE_VF(vf10, entry->radiusA);
            SDF_VU0_COPYVECTOR_C_TO_B();
            SDF_VU0_STOREVECTOR_NOMEM(radialA);
            VU0_SCALE_VF(vf11, entry->radiusB);
            SDF_VU0_STOREVECTOR_B_NOMEM(radialB);
            SDF_VU0_COPYVECTOR_C_TO_A();
            VU0_SCALE_VF(vf10, distance);
            SDF_VU0_STOREVECTOR_NOMEM(center);
            VU0_MOVE_VF(vf2, vf10);
            SDF_VU0_COPYVECTOR_C_TO_A();
            VU0_MOVE_VF(vf12, vf2);
            SDF_VU0_GETVECTOR_X(cosine);
            SDF_VU0_GETVECTOR_Z(sine);
            VU0_SET_VF10_COMPONENT(x, sine);
            cosine = -cosine;
            VU0_SET_VF10_COMPONENT(z, cosine);
            SDF_VU0_LOADVECTOR_B(width);
            SDF_VU0_MULVECTOR();
            SDF_VU0_STOREVECTOR_NOMEM(width);
            SDF_VU0_STOREVECTOR_C(vertices);
            for (outputCount = 0; outputCount < 6; outputCount++) {
                SDF_VU0_LOADVECTOR(radialA);
                VU0_SCALE_VF(vf10, billRingSines[outputCount]);
                SDF_VU0_ADDVECTOR_A_C();
                SDF_VU0_LOADVECTOR_B(width);
                VU0_SCALE_VF(vf11, billRingCosines[outputCount]);
                SDF_VU0_ADDVECTOR();
                SDF_VU0_STOREVECTOR(vertices + 1 + outputCount);
            }
            for (outputCount = 0; outputCount < 6; outputCount++) {
                SDF_VU0_LOADVECTOR(radialB);
                VU0_SCALE_VF(vf10, billRingSines[outputCount + 6]);
                SDF_VU0_ADDVECTOR_A_C();
                SDF_VU0_LOADVECTOR_B(width);
                VU0_SCALE_VF(vf11, billRingCosines[outputCount + 6]);
                SDF_VU0_ADDVECTOR();
                SDF_VU0_STOREVECTOR(vertices + 7 + outputCount);
            }
            if (age < fadeStart) alpha = time / (f32)fadeStart;
            else if (fadeEnd < age) alpha = (f32)(s32)((u32)lifetime - (u32)age) / (f32)(s32)((u32)lifetime - (u32)fadeEnd);
            else alpha = 1.0f;
            SDF_VU0_LOADVECTOR_B(effOnesVector);
            VU0_SCALE_VF(vf11, alpha);
            SDF_VU0_LOADVECTOR(secondVector);
            SDF_VU0_MULVECTOR();
            secondPacked = eePackRGBAWord(128.0f);
            color = colors + 1;
            outputCount = 11;
            do {
                *color = secondPacked;
                color++;
            } while (--outputCount >= 0);
            SDF_VU0_LOADVECTOR(firstVector);
            SDF_VU0_MULVECTOR();
            firstPacked = eePackRGBAWord(128.0f);
            colors[0] = firstPacked | ((u32)(alpha * 128.0f) << 24);
            entry->age = (s32)((u32)age + (u32)ageStep);
        }
    }
}

void billUpdateCellDrawColorAndTransform(BillCellDrawWork *work) {
    u8 *config = work->config;
    u32 limit = work->frameLimit;
    u32 progress = ((EffBillTimedHeader *)config)->time.progress;
    u32 *list = work->instances;
    EffTrackSet *out = (EffTrackSet *)list[1];
    u128 mtx[4];
    u32 packed;
    u32 unit;
    u32 second;

    if (progress < limit && progress != 0) {
        return;
    }
    second = effSampleColorAlphaTracks(&((EffBillTimedHeader *)config)->colorTrack, &((EffBillTimedHeader *)config)->alphaTrack, limit, progress);
    unit = 0x3C000000;
    eeUnpackRGBAWord(work->baseColor, unit);
    SDF_VU0_COPYVECTOR_A_TO_B();
    eeUnpackRGBAWord(second, unit);
    SDF_VU0_MULVECTOR();
    packed = eePackRGBAWord(128.0f);
    out->color = packed;
    out->type = ((EffBillTimedHeader *)config)->alphaTrack.surfaceIndex;
    out->flag = ((EffBillFrameHeader *)config)->mode;
    SDF_VU0_LOADVECTOR(work->transform);
    ToMatrix();
    SDF_VU0_LOADVECTOR(effOnesVector);
    VU0_SCALAR_OP_CLOBBER(work->scale, "vmulx.xyzw vf10, vf10, vf2x");
    SDF_VU0_SCALE_XYZ();
    SDF_VU0_LOADVECTOR(work);
    SDF_VU0_VECTOR_W_SET_ONE();
    SDF_VU0_SET_ROW3();
    VU0_STORE_MATRIX(mtx);
    effDrawTrackSet(out, (Matrix4 *)mtx);
}

void billResetParticleIndices(u8 *work) {
    u8 *state = (u8 *)((EffClassWork *)work)->resource;
    u8 *config = ((EffClassWork *)work)->payload;
    EffTrackSet *asset = ((EffBillFrameState *)state)->asset;
    s32 remaining = ((EffBillTimedHeader *)config)->count;
    EffBillParticleEntry *entry = (EffBillParticleEntry *)((EffBillFrameState *)state)->entries;

    memset(asset->buffer, 0, asset->rows * 0x10);
    if (remaining > 0) {
        s32 i;
        for (i = remaining; i != 0; --i) {
            entry->age = -1;
            ++entry;
        }
    }
}

u8 *billCreateParticleNode(EffBillParticleConfig *config, u32 resource) {
    s32 count = config->frame.output.timed.count;
    u32 headerSize = 0x10;
    struct sdfMemHandle_t *base = sdfMemAllocDirect(count * 0x2C + headerSize);
    u8 *body = (u8 *)sdfMemLock(base);
    u8 *node = body;

    body += headerSize;
    ((EffBillFrameState *)node)->allocation = base;
    *(u8 **)node = body;
    ((EffBillFrameState *)node)->asset = effCreateTrackSetWithSharedReferences(count, 1, resource);
    return node;
}

void billReleaseParticleNode(s32 work) {
    effReleaseResourceRefs(((EffBillFrameState *)work)->asset);
    sdfMemFreeDirect(((EffBillFrameState *)work)->allocation);
}

INCLUDE_ASM(const s32, "code_0023E1D0", billAdvanceParticleInstances);

void billUpdateParticleDrawColorAndTransform(BillCellDrawWork *work) {
    u8 *config = work->config;
    u32 limit = work->frameLimit;
    u32 progress = ((EffBillTimedHeader *)config)->time.progress;
    u32 *list = work->instances;
    EffTrackSet *out = (EffTrackSet *)list[1];
    u128 mtx[4];
    u32 packed;
    u32 unit;
    u32 second;

    if (progress < limit && progress != 0) {
        return;
    }
    second = effSampleColorAlphaTracks(&((EffBillTimedHeader *)config)->colorTrack, &((EffBillTimedHeader *)config)->alphaTrack, limit, progress);
    unit = 0x3C000000;
    eeUnpackRGBAWord(work->baseColor, unit);
    SDF_VU0_COPYVECTOR_A_TO_B();
    eeUnpackRGBAWord(second, unit);
    SDF_VU0_MULVECTOR();
    packed = eePackRGBAWord(128.0f);
    out->color = packed;
    out->type = ((EffBillTimedHeader *)config)->alphaTrack.surfaceIndex;
    out->flag = ((EffBillFrameHeader *)config)->mode;
    SDF_VU0_LOADVECTOR(work->transform);
    ToMatrix();
    SDF_VU0_LOADVECTOR(effOnesVector);
    VU0_SCALAR_OP_CLOBBER(work->scale, "vmulx.xyzw vf10, vf10, vf2x");
    SDF_VU0_SCALE_XYZ();
    SDF_VU0_LOADVECTOR(work);
    SDF_VU0_VECTOR_W_SET_ONE();
    SDF_VU0_SET_ROW3();
    VU0_STORE_MATRIX(mtx);
    effDrawTrackSet(out, (Matrix4 *)mtx);
}

void effClearAnimatedFrames(u8 *work) {
    u8 *state = (u8 *)((EffClassWork *)work)->resource;
    u8 *config = ((EffClassWork *)work)->payload;
    EffTrackSet *asset = ((EffBillFrameState *)state)->asset;
    s32 remaining = ((EffBillTimedHeader *)config)->count;
    u8 *entry = ((EffBillFrameState *)state)->entries;

    memset(asset->buffer, 0, asset->rows * 0x10);
    if (remaining > 0) {
        s32 i;
        for (i = remaining; i != 0; --i) {
            *(s32 *)entry = -1;
            entry += 0x18;
        }
    }
}

u8 *billAllocateAnimatedTransformEntries(EffBillAnimatedFrameConfig *config) {
    u32 headerSize = 0x10;
    struct sdfMemHandle_t *base = sdfMemAllocDirect(config->frame.output.timed.count * 0x18 + headerSize);
    u8 *node = (u8 *)sdfMemLock(base);
    u8 *entries = node + headerSize;

    ((EffBillFrameState *)node)->allocation = base;
    *(u8 **)node = entries;
    if (config->drawProgress == 0) {
        config->drawProgress = 1;
    }
    return node;
}

void effInitializeAlternatingTransformRows(u8 *node, u8 *config) {
    u32 count = ((EffBillTimedHeader *)config)->count;
    u32 index;
    f32 *transform;

    if (count == 0) {
        return;
    }
    transform = (f32 *)((EffBillFrameState *)node)->asset->columns;
    index = 0;
    for (; index < count; index++, transform += 8) {
        if (index & 1) {
            transform[0] = 0.0f;
            transform[1] = 0.0f;
            transform[2] = 0.5f;
            transform[3] = 0.0f;
            transform[4] = 0.0f;
            transform[5] = 1.0f;
            transform[6] = 0.5f;
        } else {
            transform[0] = 0.5f;
            transform[1] = 0.0f;
            transform[2] = 1.0f;
            transform[3] = 0.0f;
            transform[4] = 0.5f;
            transform[5] = 1.0f;
            transform[6] = 1.0f;
        }
        transform[7] = 1.0f;
    }
}

u8 *billCreateAnimatedTransform(EffBillAnimatedFrameConfig *config, u32 resource) {
    u8 *node = billAllocateAnimatedTransformEntries(config);
    ((EffBillFrameState *)node)->asset = effCreateTrackSetWithSharedReferences(config->frame.output.timed.count, 3, resource);
    effInitializeAlternatingTransformRows(node, config);
    return node;
}

u8 *billCloneAnimatedTransform(u8 *work) {
    u8 *config = ((EffClassWork *)work)->payload;
    u8 *state = (u8 *)((EffClassWork *)work)->resource;
    u8 *node = billAllocateAnimatedTransformEntries(config);
    ((EffBillFrameState *)node)->asset = effDuplicateResourceRefs(((EffBillFrameState *)state)->asset);
    effInitializeAlternatingTransformRows(node, config);
    return node;
}

void billReleaseAlternatingTransformNode(s32 work) {
    effReleaseResourceRefs(((EffBillFrameState *)work)->asset);
    sdfMemFreeDirect(((EffBillFrameState *)work)->allocation);
}

/* Spawn and advance the kind-4 animated frames: each live entry spins with
 * an accelerating rate, grows its radius over drawProgress frames and is
 * drawn as a four-vertex frame, faded in and out over its lifetime. */
void billAdvanceAnimatedFrameTransforms(EffClassWork *work) {
    EffBillAnimatedFrameConfig *config = (EffBillAnimatedFrameConfig *)work->payload;
    EffBillFrameState *resource;
    EffTrackSet *asset;
    EffBillAnimatedEntry *entries;
    s32 lifetime;
    s32 remainingSpawn;
    u32 randomAge;
    s32 item;
    s32 itemCount;
    s32 fadeStart;
    s32 fadeEnd;
    s32 frames;
    s32 retiredState;
    s32 waitingState;
    s32 inactiveState;
    u32 respawn;
    u32 firstColor;
    u32 secondColor;
    u32 *colors;
    u128 *vertices;
    f32 firstVector[4];
    f32 secondVector[4];
    f32 direction[4];
    f32 radius[4];
    f32 width[4];
    f32 acceleration;

    if (config->frame.output.timed.time.progress < work->frame &&
        config->frame.output.timed.time.progress != 0) return;
    resource = (EffBillFrameState *)work->resource;
    lifetime = config->frame.lifetime;
    entries = (EffBillAnimatedEntry *)resource->entries;
    asset = resource->asset;
    itemCount = (s32)config->frame.output.timed.count;
    if (lifetime == 0) return;
    if (config->frame.spawnAllOnStart != 0 && work->frame == 0) {
        remainingSpawn = itemCount;
        randomAge = 1;
    } else {
        remainingSpawn = config->frame.spawnCount;
        randomAge = 0;
    }
    firstColor = config->colorA;
    vertices = (u128 *)asset->buffer;
    colors = (u32 *)asset->tail;
    direction[1] = 0.0f;
    direction[3] = 0.0f;
    eeUnpackRGBAWordFloat(firstColor, 1.0f / 128.0f);
    SDF_VU0_STOREVECTOR_NOMEM(firstVector);
    secondColor = config->colorB;
    eeUnpackRGBAWordFloat(secondColor, 1.0f / 128.0f);
    SDF_VU0_STOREVECTOR_NOMEM(secondVector);
    fadeStart = (s32)(config->frame.fadeInFraction * (f32)lifetime);
    fadeEnd = (s32)(config->frame.fadeOutFraction * (f32)lifetime);
    acceleration = config->spinAccel;
    frames = config->drawProgress;
    respawn = config->frame.respawn;
    if (itemCount <= 0) return;
    waitingState = -1;
    inactiveState = -2;
    retiredState = respawn != 0 ? waitingState : inactiveState;
    item = itemCount;
    for (; item != 0; item--, entries++, vertices += 4, colors += 4) {
        EffBillAnimatedEntry *entry = entries;
        s32 age = entry->age;
        if (age == inactiveState) continue;
        if (age == waitingState) {
            if (remainingSpawn != 0) {
                f32 randomValue;
                entry->angle = dds3GetRandFloatA(effSharedRandomState) * 6.28318500518798828125f;
                randomValue = dds3GetRandFloatA(effSharedRandomState);
                entry->spin = config->spin * (randomValue * config->spinJitter + (1.0f - config->spinJitter));
                if (dds3GetRandA((EffRandState *)effSharedRandomState) & 1) entry->spin = -entry->spin;
                entry->radius = 0.0f;
                randomValue = dds3GetRandFloatA(effSharedRandomState);
                entry->radiusStep = config->radius * (randomValue * config->radiusJitter + (1.0f - config->radiusJitter)) / (f32)frames;
                randomValue = dds3GetRandFloatA(effSharedRandomState);
                entry->width = config->width * (randomValue * config->widthJitter + (1.0f - config->widthJitter));
                if (randomAge != 0) {
                    entry->age = (s32)(dds3GetRandA((EffRandState *)effSharedRandomState) % (u32)lifetime);
                    entry->radius += entry->radiusStep * (f32)(entry->age % frames);
                } else {
                    entry->age = 0;
                }
                remainingSpawn = (s32)((u32)remainingSpawn - 1u);
            }
        } else if (age >= lifetime) {
            u32 *color = colors;
            u128 *position = vertices;
            s32 count = 3;
            do {
                VU0_STORE_VF(vf0, position);
                *color = 0;
                position++;
                color++;
            } while (--count >= 0);
            entry->age = retiredState;
        } else {
            f32 spin = entry->spin;
            f32 time = (f32)age;
            f32 angle;
            f32 cosine;
            f32 sine;
            f32 alpha;
            u32 secondPacked;
            u32 firstPacked;
            if (spin > 0.0f) angle = (spin + (acceleration * time) * 0.5f) * time;
            else angle = (spin - (acceleration * time) * 0.5f) * time;
            angle += entry->angle;
            if (age < frames) entry->radius += entry->radiusStep;
            SDF_VU0_LOADVECTOR(effOnesVector);
            SDF_VU0_COPYVECTOR_A_TO_B();
            VU0_SCALE_VF(vf10, entry->radius);
            SDF_VU0_STOREVECTOR_NOMEM(radius);
            SDF_VU0_COPYVECTOR_B_TO_A();
            VU0_SCALE_VF(vf10, entry->width);
            SDF_VU0_STOREVECTOR_NOMEM(width);
            direction[0] = sdfCos(angle);
            direction[2] = sdfSin(angle);
            SDF_VU0_LOADVECTOR(direction);
            SDF_VU0_COPYVECTOR_A_TO_C();
            SDF_VU0_GETVECTOR_X(cosine);
            SDF_VU0_GETVECTOR_Z(sine);
            VU0_SCALAR_OP(sine, "vaddx.x vf10, vf0, vf2x");
            cosine = -cosine;
            VU0_SCALAR_OP(cosine, "vaddx.z vf10, vf0, vf2x");
            SDF_VU0_COPYVECTOR_A_TO_B();
            SDF_VU0_LOADVECTOR(width);
            SDF_VU0_MULVECTOR();
            SDF_VU0_STOREVECTOR_NOMEM(width);
            SDF_VU0_COPYVECTOR_C_TO_A();
            SDF_VU0_LOADVECTOR_B(radius);
            SDF_VU0_MULVECTOR();
            SDF_VU0_COPYVECTOR_A_TO_C();
            SDF_VU0_LOADVECTOR(width);
            SDF_VU0_COPYVECTOR_A_TO_B();
            SDF_VU0_STOREVECTOR(vertices + 2);
            VU0_NEGATE_XYZ(vf11);
            SDF_VU0_STOREVECTOR_B(vertices + 3);
            SDF_VU0_LOADVECTOR(width);
            SDF_VU0_COPYVECTOR_A_TO_B();
            SDF_VU0_ADDVECTOR_A_C();
            VU0_NEGATE_XYZ(vf11);
            SDF_VU0_STOREVECTOR(vertices);
            SDF_VU0_ADDVECTOR_B_C();
            SDF_VU0_STOREVECTOR_B(vertices + 1);
            if (age < fadeStart) alpha = time / (f32)fadeStart;
            else if (fadeEnd < age) alpha = (f32)(s32)((u32)lifetime - (u32)age) / (f32)(s32)((u32)lifetime - (u32)fadeEnd);
            else alpha = 1.0f;
            SDF_VU0_LOADVECTOR_B(effOnesVector);
            VU0_SCALE_VF(vf11, alpha);
            SDF_VU0_LOADVECTOR(secondVector);
            SDF_VU0_MULVECTOR();
            secondPacked = eePackRGBAWord(128.0f);
            colors[0] = secondPacked;
            colors[1] = secondPacked;
            SDF_VU0_LOADVECTOR(firstVector);
            SDF_VU0_MULVECTOR();
            firstPacked = eePackRGBAWord(128.0f);
            colors[2] = firstPacked;
            colors[3] = firstPacked;
            entry->age = (s32)((u32)age + 1u);
        }
    }
}

void billUpdateAlternatingDrawColorAndTransform(BillCellDrawWork *work) {
    u8 *config = work->config;
    u32 limit = work->frameLimit;
    u32 progress = ((EffBillTimedHeader *)config)->time.progress;
    u32 *list = work->instances;
    EffTrackSet *out = (EffTrackSet *)list[1];
    u128 mtx[4];
    u32 packed;
    u32 unit;
    u32 second;

    if (progress < limit && progress != 0) {
        return;
    }
    second = effSampleColorAlphaTracks(&((EffBillTimedHeader *)config)->colorTrack, &((EffBillTimedHeader *)config)->alphaTrack, limit, progress);
    unit = 0x3C000000;
    eeUnpackRGBAWord(work->baseColor, unit);
    SDF_VU0_COPYVECTOR_A_TO_B();
    eeUnpackRGBAWord(second, unit);
    SDF_VU0_MULVECTOR();
    packed = eePackRGBAWord(128.0f);
    out->color = packed;
    out->type = ((EffBillTimedHeader *)config)->alphaTrack.surfaceIndex;
    out->flag = ((EffBillFrameHeader *)config)->mode;
    SDF_VU0_LOADVECTOR(work->transform);
    ToMatrix();
    SDF_VU0_LOADVECTOR(effOnesVector);
    VU0_SCALAR_OP_CLOBBER(work->scale, "vmulx.xyzw vf10, vf10, vf2x");
    SDF_VU0_SCALE_XYZ();
    SDF_VU0_LOADVECTOR(work);
    SDF_VU0_VECTOR_W_SET_ONE();
    SDF_VU0_SET_ROW3();
    VU0_STORE_MATRIX(mtx);
    effDrawTrackSet(out, (Matrix4 *)mtx);
}

void billResetEmitterIndices(u8 *work) {
    u8 *state = (u8 *)((EffClassWork *)work)->resource;
    u8 *config = ((EffClassWork *)work)->payload;
    EffTrackSet *asset = ((EffBillFrameState *)state)->asset;
    s32 remaining = ((EffBillTimedHeader *)config)->count;
    u8 *entry = ((EffBillFrameState *)state)->entries;

    memset(asset->buffer, 0, asset->rows * 0x10);
    if (remaining > 0) {
        s32 i;
        for (i = remaining; i != 0; --i) {
            *(s32 *)entry = -1;
            entry += 0x18;
        }
    }
}

u8 *billAllocEmitterNode(u8 *config) {
    u32 headerSize = 0x10;
    struct sdfMemHandle_t *base = sdfMemAllocDirect(((EffBillTimedHeader *)config)->count * 0x18 + headerSize);
    u8 *body = (u8 *)sdfMemLock(base);
    u8 *node = body;

    body += headerSize;
    ((EffBillFrameState *)node)->allocation = base;
    *(u8 **)node = body;
    return node;
}

void billInitializeEmitterRows(u8 *work, u8 *descriptor) {
    u32 count = ((EffBillTimedHeader *)descriptor)->count;
    u32 index;
    f32 *transform;

    if (count == 0) {
        return;
    }
    transform = (f32 *)((EffBillFrameState *)work)->asset->columns;
    index = 0;
    for (; index < count; index++, transform += 8) {
        if (index & 1) {
            transform[0] = 0.0f;
            transform[1] = 0.0f;
            transform[2] = 0.5f;
            transform[3] = 0.0f;
            transform[4] = 0.0f;
            transform[5] = 1.0f;
            transform[6] = 0.5f;
        } else {
            transform[0] = 0.5f;
            transform[1] = 0.0f;
            transform[2] = 1.0f;
            transform[3] = 0.0f;
            transform[4] = 0.5f;
            transform[5] = 1.0f;
            transform[6] = 1.0f;
        }
        transform[7] = 1.0f;
    }
}

u8 *billCreateEmitterTransform(EffBillEmitterFrameConfig *config, u32 resource) {
    u8 *node = billAllocEmitterNode(config);
    ((EffBillFrameState *)node)->asset = effCreateTrackSetWithSharedReferences(config->frame.output.timed.count, 4, resource);
    billInitializeEmitterRows(node, config);
    return node;
}

u8 *billCloneEmitterTransform(u8 *work) {
    u8 *config = ((EffClassWork *)work)->payload;
    u8 *state = (u8 *)((EffClassWork *)work)->resource;
    u8 *node = billAllocEmitterNode(config);
    ((EffBillFrameState *)node)->asset = effDuplicateResourceRefs(((EffBillFrameState *)state)->asset);
    billInitializeEmitterRows(node, config);
    return node;
}

void billReleaseEmitterNode(s32 work) {
    effReleaseResourceRefs(((EffBillFrameState *)work)->asset);
    sdfMemFreeDirect(((EffBillFrameState *)work)->allocation);
}

void billAdvanceEmitterInstances(EffClassWork *work) {
    EffBillEmitterFrameConfig *config = (EffBillEmitterFrameConfig *)work->payload;
    EffBillFrameState *resource;
    EffTrackSet *asset;
    EffEmitterRow *entries;
    s32 lifetime;
    s32 remainingSpawn;
    u32 randomAge;
    s32 item;
    s32 itemCount;
    s32 fadeStart;
    s32 fadeEnd;
    s32 startAge;
    s32 endAge;
    s32 ageStep;
    s32 retiredState;
    s32 waitingState;
    s32 inactiveState;
    u32 firstColor;
    u32 secondColor;
    u32 respawn;
    u32 *colors;
    u128 *vertices;
    f32 firstVector[4];
    f32 secondVector[4];
    f32 direction[4];
    f32 width[4];
    f32 radial[4];
    f32 acceleration;

    if (config->frame.output.timed.time.progress < work->frame &&
        config->frame.output.timed.time.progress != 0) return;
    resource = (EffBillFrameState *)work->resource;
    lifetime = config->frame.lifetime;
    entries = (EffEmitterRow *)resource->entries;
    asset = resource->asset;
    itemCount = (s32)config->frame.output.timed.count;
    if (lifetime == 0) return;
    if (config->reverse != 0) {
        fadeStart = (s32)((1.0f - config->frame.fadeOutFraction) * (f32)lifetime);
        fadeEnd = (s32)((1.0f - config->frame.fadeInFraction) * (f32)lifetime);
        startAge = lifetime;
        endAge = 0;
        ageStep = -1;
    } else {
        fadeStart = (s32)(config->frame.fadeInFraction * (f32)lifetime);
        fadeEnd = (s32)(config->frame.fadeOutFraction * (f32)lifetime);
        startAge = 0;
        endAge = lifetime;
        ageStep = 1;
    }
    if (config->frame.spawnAllOnStart != 0 && work->frame == 0) {
        remainingSpawn = itemCount;
        randomAge = 1;
    } else {
        remainingSpawn = config->frame.spawnCount;
        randomAge = 0;
    }
    firstColor = config->firstColor;
    vertices = (u128 *)asset->buffer;
    colors = (u32 *)asset->tail;
    direction[1] = 0.0f;
    direction[3] = 0.0f;
    eeUnpackRGBAWordFloat(firstColor, 1.0f / 128.0f);
    SDF_VU0_STOREVECTOR_NOMEM(firstVector);
    secondColor = config->secondColor;
    eeUnpackRGBAWordFloat(secondColor, 1.0f / 128.0f);
    SDF_VU0_STOREVECTOR_NOMEM(secondVector);
    acceleration = config->acceleration;
    respawn = config->frame.respawn;
    if (itemCount <= 0) return;
    waitingState = -1;
    inactiveState = -2;
    retiredState = respawn != 0 ? waitingState : inactiveState;
    item = itemCount;
    for (; item != 0; item--, entries++, vertices += 4, colors += 4) {
        EffEmitterRow *entry = entries;
        s32 age = entry->age;
        if (age == inactiveState) continue;
        if (age == waitingState) {
            if (remainingSpawn != 0) {
                f32 randomValue;
                f32 jitter;
                entry->angle = dds3GetRandFloatA(effSharedRandomState) * 6.28318500518798828125f;
                jitter = config->radiusJitter;
                randomValue = dds3GetRandFloatA(effSharedRandomState);
                entry->radius = config->radius * (randomValue * jitter + (1.0f - jitter));
                jitter = config->offsetJitter;
                randomValue = dds3GetRandFloatA(effSharedRandomState);
                entry->offset = config->offset * (randomValue * jitter + (1.0f - jitter));
                randomValue = dds3GetRandFloatA(effSharedRandomState);
                entry->speed = config->speed * (randomValue * config->speedJitter + (1.0f - config->speedJitter));
                randomValue = dds3GetRandFloatA(effSharedRandomState);
                jitter = randomValue * config->widthJitter + (1.0f - config->widthJitter);
                entry->width = config->width * jitter;
                if (randomAge != 0) entry->age = (s32)(dds3GetRandA((EffRandState *)effSharedRandomState) % (u32)lifetime);
                else entry->age = startAge;
                remainingSpawn = (s32)((u32)remainingSpawn - 1u);
            }
        } else if (age == endAge) {
            u32 *color = colors;
            u128 *position = vertices;
            s32 count = 3;
            do {
                VU0_STORE_VF(vf0, position);
                *color = 0;
                position++;
                color++;
            } while (--count >= 0);
            entry->age = retiredState;
        } else {
            f32 time = (f32)age;
            f32 motionTime = time;
            f32 speed;
            f32 angle = entry->angle;
            f32 distance;
            f32 cosine;
            f32 sine;
            f32 alpha;
            u32 secondPacked;
            u32 firstPacked;
            if (acceleration < 0.0f) {
                f32 stopTime;
                speed = entry->speed;
                stopTime = (-speed / (acceleration * 0.5f)) * 0.5f;
                if (stopTime < time) motionTime = stopTime;
            } else {
                speed = entry->speed;
            }
            distance = (speed + (acceleration * motionTime) * 0.5f) * motionTime;
            distance += entry->offset;
            direction[0] = sdfCos(angle);
            direction[2] = sdfSin(angle);
            SDF_VU0_LOADVECTOR(direction);
            SDF_VU0_COPYVECTOR_A_TO_B();
            VU0_SCALE_VF(vf10, entry->radius);
            SDF_VU0_STOREVECTOR_NOMEM(radial);
            SDF_VU0_COPYVECTOR_B_TO_A();
            VU0_SCALE_VF(vf10, distance);
            SDF_VU0_COPYVECTOR_A_TO_C();
            SDF_VU0_COPYVECTOR_B_TO_A();
            SDF_VU0_GETVECTOR_X(cosine);
            SDF_VU0_GETVECTOR_Z(sine);
            VU0_SCALAR_OP(sine, "vaddx.x vf10, vf0, vf2x");
            cosine = -cosine;
            VU0_SCALAR_OP(cosine, "vaddx.z vf10, vf0, vf2x");
            VU0_SCALE_VF(vf10, entry->width);
            SDF_VU0_STOREVECTOR_NOMEM(width);
            SDF_VU0_LOADVECTOR(radial);
            SDF_VU0_ADDVECTOR_A_C();
            SDF_VU0_LOADVECTOR_B(width);
            SDF_VU0_ADDVECTOR();
            SDF_VU0_STOREVECTOR(vertices);
            SDF_VU0_SUBVECTOR();
            SDF_VU0_SUBVECTOR();
            SDF_VU0_STOREVECTOR(vertices + 1);
            SDF_VU0_LOADVECTOR(radial);
            SDF_VU0_NEGVECTOR_XYZ();
            SDF_VU0_ADDVECTOR_A_C();
            SDF_VU0_LOADVECTOR_B(width);
            SDF_VU0_ADDVECTOR();
            SDF_VU0_STOREVECTOR(vertices + 2);
            SDF_VU0_SUBVECTOR();
            SDF_VU0_SUBVECTOR();
            SDF_VU0_STOREVECTOR(vertices + 3);
            if (age < fadeStart) alpha = time / (f32)fadeStart;
            else if (fadeEnd < age) alpha = (f32)(s32)((u32)lifetime - (u32)age) / (f32)(s32)((u32)lifetime - (u32)fadeEnd);
            else alpha = 1.0f;
            SDF_VU0_LOADVECTOR_B(effOnesVector);
            VU0_SCALE_VF(vf11, alpha);
            SDF_VU0_LOADVECTOR(secondVector);
            SDF_VU0_MULVECTOR();
            secondPacked = eePackRGBAWord(128.0f);
            colors[0] = secondPacked;
            colors[1] = secondPacked;
            SDF_VU0_LOADVECTOR(firstVector);
            SDF_VU0_MULVECTOR();
            firstPacked = eePackRGBAWord(128.0f);
            colors[2] = firstPacked;
            colors[3] = firstPacked;
            entry->age = (s32)((u32)age + (u32)ageStep);
        }
    }
}

void billUpdateEmitterDrawColorAndTransform(u8 *work) {
    u8 *config = ((BillCellDrawWork *)work)->config;
    u32 limit = ((BillCellDrawWork *)work)->frameLimit;
    u32 progress = ((EffBillTimedHeader *)config)->time.progress;
    u32 *list = ((BillCellDrawWork *)work)->instances;
    EffTrackSet *out = (EffTrackSet *)list[1];
    u128 mtx[4];
    u32 packed;
    u32 unit;
    u32 second;

    if (progress < limit && progress != 0) {
        return;
    }
    second = effSampleColorAlphaTracks(&((EffBillTimedHeader *)config)->colorTrack, &((EffBillTimedHeader *)config)->alphaTrack, limit, progress);
    unit = 0x3C000000;
    eeUnpackRGBAWord(((BillCellDrawWork *)work)->baseColor, unit);
    SDF_VU0_COPYVECTOR_A_TO_B();
    eeUnpackRGBAWord(second, unit);
    SDF_VU0_MULVECTOR();
    packed = eePackRGBAWord(128.0f);
    out->color = packed;
    out->type = ((EffBillTimedHeader *)config)->alphaTrack.surfaceIndex;
    out->flag = ((EffBillFrameHeader *)config)->mode;
    SDF_VU0_LOADVECTOR(work + 0x10);
    ToMatrix();
    SDF_VU0_LOADVECTOR(effOnesVector);
    VU0_SCALAR_OP_CLOBBER(((BillCellDrawWork *)work)->scale, "vmulx.xyzw vf10, vf10, vf2x");
    SDF_VU0_SCALE_XYZ();
    SDF_VU0_LOADVECTOR(work);
    SDF_VU0_VECTOR_W_SET_ONE();
    SDF_VU0_SET_ROW3();
    VU0_STORE_MATRIX(mtx);
    effDrawTrackSet(out, (Matrix4 *)mtx);
}

void effClearStripFrames(u8 *work) {
    u8 *state = (u8 *)((EffClassWork *)work)->resource;
    u8 *config = ((EffClassWork *)work)->payload;
    EffTrackSet *asset = ((EffBillFrameState *)state)->asset;
    s32 remaining = ((EffBillTimedHeader *)config)->count;
    u8 *entry = ((EffBillFrameState *)state)->entries;

    memset(asset->buffer, 0, asset->rows * 0x10);
    if (remaining > 0) {
        s32 i;
        for (i = remaining; i != 0; --i) {
            *(s32 *)entry = -1;
            entry += 0x28;
        }
    }
}

u8 *billAllocStripNode(u8 *config) {
    u32 headerSize = 0x10;
    struct sdfMemHandle_t *base = sdfMemAllocDirect(((EffBillTimedHeader *)config)->count * 0x28 + headerSize);
    u8 *body = (u8 *)sdfMemLock((struct sdfMemHandle_t *)((u32)base));
    u8 *node = body;

    body += headerSize;
    ((EffBillFrameState *)node)->allocation = base;
    *(u8 **)node = body;
    return node;
}

void billInitializeStripRows(u8 *node, u8 *config) {
    u32 count = ((EffBillTimedHeader *)config)->count;
    u32 index;
    f32 *transform;

    if (count == 0) {
        return;
    }
    transform = (f32 *)((EffBillFrameState *)node)->asset->columns;
    index = 0;
    for (; index < count; index++, transform += 8) {
        if (index & 1) {
            transform[0] = 0.0f;
            transform[1] = 0.0f;
            transform[2] = 0.5f;
            transform[3] = 0.0f;
            transform[4] = 0.0f;
            transform[5] = 1.0f;
            transform[6] = 0.5f;
        } else {
            transform[0] = 0.5f;
            transform[1] = 0.0f;
            transform[2] = 1.0f;
            transform[3] = 0.0f;
            transform[4] = 0.5f;
            transform[5] = 1.0f;
            transform[6] = 1.0f;
        }
        transform[7] = 1.0f;
    }
}

u8 *billCreateStripTransform(EffBillStripFrameConfig *config, u32 resource) {
    u8 *node = billAllocStripNode(config);

    ((EffBillFrameState *)node)->asset = effCreateTrackSetWithSharedReferences(config->frame.output.timed.count, 3, resource);
    billInitializeStripRows(node, config);
    return node;
}

u8 *billCloneStripTransform(u8 *work) {
    u8 *config = ((EffClassWork *)work)->payload;
    u8 *state = (u8 *)((EffClassWork *)work)->resource;
    u8 *node = billAllocStripNode(config);
    ((EffBillFrameState *)node)->asset = effDuplicateResourceRefs(((EffBillFrameState *)state)->asset);
    billInitializeStripRows(node, config);
    return node;
}

void billReleaseStripNode(s32 work) {
    effReleaseResourceRefs(((EffBillFrameState *)work)->asset);
    sdfMemFreeDirect(((EffBillFrameState *)work)->allocation);
}

void billAdvanceStripInstances(EffClassWork *work) {
    EffBillStripFrameConfig *config = (EffBillStripFrameConfig *)work->payload;
    EffBillFrameState *resource;
    EffBillStripEntry *entries;
    EffTrackSet *asset;
    s32 lifetime;
    s32 itemCount;
    s32 remainingSpawn;
    u32 randomAge;
    s32 remainingItems;
    s32 fadeStart;
    s32 fadeEnd;
    s32 waitingState;
    s32 inactiveState;
    s32 retiredState;
    u32 respawn;
    u32 colorA;
    u32 colorB;
    u32 *colors;
    u128 *vertices;
    f32 colorVectorA[4];
    f32 colorVectorB[4];
    f32 center[4];
    f32 direction[4];
    f32 lateral[4];
    f32 acceleration;

    if (config->frame.output.timed.time.progress < work->frame &&
        config->frame.output.timed.time.progress != 0) {
        return;
    }
    lifetime = config->frame.lifetime;
    resource = (EffBillFrameState *)work->resource;
    entries = (EffBillStripEntry *)resource->entries;
    asset = resource->asset;
    itemCount = (s32)config->frame.output.timed.count;
    if (lifetime == 0) {
        return;
    }
    if (config->frame.spawnAllOnStart != 0 && work->frame == 0) {
        remainingSpawn = itemCount;
        randomAge = 1;
    } else {
        remainingSpawn = config->frame.spawnCount;
        randomAge = 0;
    }

    colorA = config->colorA;
    vertices = (u128 *)asset->buffer;
    colors = (u32 *)asset->tail;
    center[1] = 0.0f;
    center[3] = 0.0f;
    eeUnpackRGBAWordFloat(colorA, 1.0f / 128.0f);
    SDF_VU0_STOREVECTOR_NOMEM(colorVectorA);
    colorB = config->colorB;
    eeUnpackRGBAWordFloat(colorB, 1.0f / 128.0f);
    SDF_VU0_STOREVECTOR_NOMEM(colorVectorB);
    fadeStart = (s32)(config->frame.fadeInFraction * (f32)lifetime);
    fadeEnd = (s32)(config->frame.fadeOutFraction * (f32)lifetime);
    acceleration = config->spinAccel;
    respawn = config->frame.respawn;
    waitingState = -1;
    inactiveState = -2;
    {
    u32 packedB[4];
    u32 packedA[4];

    for (remainingItems = 0; remainingItems < itemCount; remainingItems++, entries++, vertices += 4, colors += 4) {
        EffBillStripEntry *entry = entries;
        s32 age = entry->age;
        f32 time;

        retiredState = respawn != 0 ? waitingState : inactiveState;
        if (age == inactiveState) {
            continue;
        }
        if (age == waitingState) {
            if (remainingSpawn != 0) {
                f32 randomValue;
                f32 start;
                f32 end;

                entry->angle = dds3GetRandFloatA(effSharedRandomState) * 6.28318500518798828125f;
                randomValue = dds3GetRandFloatA(effSharedRandomState);
                start = config->lengthStart * (randomValue * config->lengthStartJitter + (1.0f - config->lengthStartJitter));
                randomValue = dds3GetRandFloatA(effSharedRandomState);
                end = config->lengthEnd * (randomValue * config->lengthEndJitter + (1.0f - config->lengthEndJitter));
                entry->length = start;
                entry->lengthStep = (end - start) / (f32)lifetime;

                randomValue = dds3GetRandFloatA(effSharedRandomState);
                start = config->radiusStart * (randomValue * config->radiusStartJitter + (1.0f - config->radiusStartJitter));
                randomValue = dds3GetRandFloatA(effSharedRandomState);
                end = config->radiusEnd * (randomValue * config->radiusEndJitter + (1.0f - config->radiusEndJitter));
                entry->radius = start;
                entry->radiusStep = (end - start) / (f32)lifetime;

                randomValue = dds3GetRandFloatA(effSharedRandomState);
                start = config->tiltStart * (randomValue * config->tiltStartJitter + (1.0f - config->tiltStartJitter));
                randomValue = dds3GetRandFloatA(effSharedRandomState);
                end = config->tiltEnd * (randomValue * config->tiltEndJitter + (1.0f - config->tiltEndJitter));
                entry->tilt = start;
                entry->tiltStep = (end - start) / (f32)lifetime;

                randomValue = dds3GetRandFloatA(effSharedRandomState);
                entry->width = config->width * (randomValue * config->widthJitter + (1.0f - config->widthJitter));
                randomValue = dds3GetRandFloatA(effSharedRandomState);
                entry->spin = config->spin * (randomValue * config->spinJitter + (1.0f - config->spinJitter));
                if (randomAge != 0) {
                    time = (f32)(dds3GetRandA((struct EffRandState *)effSharedRandomState) % (u32)lifetime);
                    entry->age = (s32)time;
                    entry->length += entry->lengthStep * time;
                    entry->radius += entry->radiusStep * time;
                    entry->tilt += entry->tiltStep * time;
                } else {
                    entry->age = 0;
                }
                remainingSpawn = (s32)((u32)remainingSpawn - 1);
            }
        } else if (age >= lifetime) {
            u32 *color = colors;
            u128 *position = vertices;
            s32 count = 3;
            do {
                VU0_STORE_VF(vf0, position);
                *color = 0;
                position++;
                color++;
            } while (--count >= 0);
            entry->age = retiredState;
        } else {
            f32 angle;
            f32 radius;
            f32 tilt;
            f32 cosine;
            f32 sine;
            f32 alpha;
            u32 packedColor;

            time = (f32)age;
            angle = (entry->spin + (acceleration * time) * 0.5f) * time + entry->angle;
            radius = entry->radius;
            tilt = entry->tilt;
            cosine = sdfCos(angle);
            sine = sdfSin(angle);

            center[0] = cosine * radius;
            center[2] = sine * radius;
            direction[0] = cosine * tilt;
            direction[1] = tilt + -1.0f;
            direction[2] = sine * tilt;
            SDF_VU0_LOADVECTOR(direction);
            SDF_VU0_NORMALIZE();
            SDF_VU0_COPYVECTOR_A_TO_C();

            direction[0] = sine;
            direction[1] = 0.0f;
            cosine = -cosine;
            direction[2] = cosine;
            SDF_VU0_LOADVECTOR(direction);
            VU0_SCALE_VF(vf10, entry->width);
            SDF_VU0_STOREVECTOR_NOMEM(lateral);
            SDF_VU0_LOADVECTOR(center);
            SDF_VU0_LOADVECTOR_B(lateral);
            SDF_VU0_ADDVECTOR();
            SDF_VU0_STOREVECTOR(vertices + 2);
            SDF_VU0_SUBVECTOR();
            SDF_VU0_SUBVECTOR();
            SDF_VU0_STOREVECTOR(vertices + 3);
            SDF_VU0_COPYVECTOR_C_TO_B();
            VU0_SCALE_VF(vf11, entry->length);
            SDF_VU0_LOADVECTOR(center);
            SDF_VU0_ADDVECTOR();
            SDF_VU0_LOADVECTOR_B(lateral);
            SDF_VU0_ADDVECTOR();
            SDF_VU0_STOREVECTOR(vertices + 0);
            SDF_VU0_SUBVECTOR();
            SDF_VU0_SUBVECTOR();
            SDF_VU0_STOREVECTOR(vertices + 1);

            entry->length += entry->lengthStep;
            entry->tilt += entry->tiltStep;
            entry->radius += entry->radiusStep;
            if (age < fadeStart) {
                alpha = time / (f32)fadeStart;
            } else if (fadeEnd < age) {
                alpha = (f32)(s32)((u32)lifetime - (u32)age) / (f32)(s32)((u32)lifetime - (u32)fadeEnd);
            } else {
                alpha = 1.0f;
            }
            SDF_VU0_LOADVECTOR_B(effOnesVector);
            VU0_SCALE_VF(vf11, alpha);
            SDF_VU0_LOADVECTOR(colorVectorB);
            SDF_VU0_MULVECTOR();
            {
                EE_MMI_RGBA_PACK_UNIT(packedB[0], 128.0f);
                packedColor = packedB[0];
                colors[0] = packedColor;
                colors[1] = packedColor;
            }
            SDF_VU0_LOADVECTOR(colorVectorA);
            SDF_VU0_MULVECTOR();
            {
                EE_MMI_RGBA_PACK_UNIT(packedA[0], 128.0f);
                packedColor = packedA[0];
                colors[2] = packedColor;
                colors[3] = packedColor;
            }
            entry->age = (s32)((u32)age + 1);
        }
    }
    }
}

void billUpdateStripDrawColorAndTransform(u8 *work) {
    u8 *config = ((BillCellDrawWork *)work)->config;
    u32 limit = ((BillCellDrawWork *)work)->frameLimit;
    u32 progress = ((EffBillTimedHeader *)config)->time.progress;
    u32 *list = ((BillCellDrawWork *)work)->instances;
    EffTrackSet *out = (EffTrackSet *)list[1];
    u128 mtx[4];
    u32 packed;
    u32 unit;
    u32 second;

    if (progress < limit && progress != 0) {
        return;
    }
    second = effSampleColorAlphaTracks(&((EffBillTimedHeader *)config)->colorTrack, &((EffBillTimedHeader *)config)->alphaTrack, limit, progress);
    unit = 0x3C000000;
    eeUnpackRGBAWord(((BillCellDrawWork *)work)->baseColor, unit);
    SDF_VU0_COPYVECTOR_A_TO_B();
    eeUnpackRGBAWord(second, unit);
    SDF_VU0_MULVECTOR();
    packed = eePackRGBAWord(128.0f);
    out->color = packed;
    out->type = ((EffBillTimedHeader *)config)->alphaTrack.surfaceIndex;
    out->flag = ((EffBillFrameHeader *)config)->mode;
    SDF_VU0_LOADVECTOR(work + 0x10);
    ToMatrix();
    SDF_VU0_LOADVECTOR(effOnesVector);
    VU0_SCALAR_OP_CLOBBER(((BillCellDrawWork *)work)->scale, "vmulx.xyzw vf10, vf10, vf2x");
    SDF_VU0_SCALE_XYZ();
    SDF_VU0_LOADVECTOR(work);
    SDF_VU0_VECTOR_W_SET_ONE();
    SDF_VU0_SET_ROW3();
    VU0_STORE_MATRIX(mtx);
    effDrawTrackSet(out, (Matrix4 *)mtx);
}

void billResetTrailIndices(u8 *work) {
    u8 *state = (u8 *)((EffClassWork *)work)->resource;
    u8 *config = ((EffClassWork *)work)->payload;
    EffTrackSet *asset = ((EffBillFrameState *)state)->asset;
    s32 remaining = ((EffBillTimedHeader *)config)->count;
    u8 *entry = ((EffBillFrameState *)state)->entries;

    memset(asset->buffer, 0, asset->rows * 0x10);
    if (remaining > 0) {
        s32 i;
        for (i = remaining; i != 0; --i) {
            *(s32 *)entry = -1;
            entry += 0x2C;
        }
    }
}

u8 *billCreateTrailNode(EffBillTrailFrameConfig *config, u32 resource) {
    u32 headerSize = 0x10;
    struct sdfMemHandle_t *base = sdfMemAllocDirect(config->frame.output.timed.count * 0x2C + headerSize);
    u8 *cursor = (u8 *)sdfMemLock((struct sdfMemHandle_t *)((u32)base));
    u8 *header = cursor;
    /* Read the configured count before writing the retained header. */
    u32 frameCount = config->frame.output.timed.count;

    cursor += headerSize;
    ((EffBillFrameState *)header)->allocation = base;
    *(u8 **)header = cursor;
    ((EffBillFrameState *)header)->asset = effCreateTrackSetWithSharedReferences(frameCount, 0, resource);
    return header;
}

void billReleaseTrailNode(s32 work) {
    effReleaseResourceRefs(((EffBillFrameState *)work)->asset);
    sdfMemFreeDirect(((EffBillFrameState *)work)->allocation);
}

INCLUDE_ASM(const s32, "code_0023E1D0", billAdvanceTrailInstances);

void billUpdateTrailDrawColorAndTransform(u8 *work) {
    u8 *config = ((BillCellDrawWork *)work)->config;
    u32 limit = ((BillCellDrawWork *)work)->frameLimit;
    u32 progress = ((EffBillTimedHeader *)config)->time.progress;
    u32 *list = ((BillCellDrawWork *)work)->instances;
    EffTrackSet *out = (EffTrackSet *)list[1];
    u128 mtx[4];
    u32 packed;
    u32 unit;
    u32 second;

    if (progress < limit && progress != 0) {
        return;
    }
    second = effSampleColorAlphaTracks(&((EffBillTimedHeader *)config)->colorTrack, &((EffBillTimedHeader *)config)->alphaTrack, limit, progress);
    unit = 0x3C000000;
    eeUnpackRGBAWord(((BillCellDrawWork *)work)->baseColor, unit);
    SDF_VU0_COPYVECTOR_A_TO_B();
    eeUnpackRGBAWord(second, unit);
    SDF_VU0_MULVECTOR();
    packed = eePackRGBAWord(128.0f);
    out->color = packed;
    out->type = ((EffBillTimedHeader *)config)->alphaTrack.surfaceIndex;
    out->flag = ((EffBillFrameHeader *)config)->mode;
    SDF_VU0_LOADVECTOR(work + 0x10);
    ToMatrix();
    SDF_VU0_LOADVECTOR(effOnesVector);
    VU0_SCALAR_OP_CLOBBER(((BillCellDrawWork *)work)->scale, "vmulx.xyzw vf10, vf10, vf2x");
    SDF_VU0_SCALE_XYZ();
    SDF_VU0_LOADVECTOR(work);
    SDF_VU0_VECTOR_W_SET_ONE();
    SDF_VU0_SET_ROW3();
    VU0_STORE_MATRIX(mtx);
    effDrawTrackSet(out, (Matrix4 *)mtx);
}

void billResetQuadIndices(u8 *work) {
    u8 *state = (u8 *)((EffClassWork *)work)->resource;
    u8 *config = ((EffClassWork *)work)->payload;
    EffTrackSet *asset = ((EffBillFrameState *)state)->asset;
    s32 remaining = ((EffBillTimedHeader *)config)->count;
    EffBillQuadEntry *entry = (EffBillQuadEntry *)((EffBillFrameState *)state)->entries;

    memset(asset->buffer, 0, asset->rows * 0x10);
    if (remaining > 0) {
        s32 i;
        for (i = remaining; i != 0; --i) {
            entry->timer = -1;
            entry++;
        }
    }
}

u8 *billAllocQuadNode(u8 *config) {
    u32 headerSize = 0x10;
    struct sdfMemHandle_t *base = sdfMemAllocDirect(((EffBillTimedHeader *)config)->count * sizeof(EffBillQuadEntry) + headerSize);
    u8 *body = (u8 *)sdfMemLock((struct sdfMemHandle_t *)((u32)base));
    u8 *node = body;

    body += headerSize;
    ((EffBillFrameState *)node)->allocation = base;
    *(u8 **)node = body;
    return node;
}

void billInitializeQuadRows(u8 *work, u8 *descriptor) {
    u32 count = ((EffBillTimedHeader *)descriptor)->count;
    u32 index;
    f32 *transform;

    if (count == 0) {
        return;
    }
    transform = (f32 *)((EffBillFrameState *)work)->asset->columns;
    index = 0;
    for (; index < count; index++, transform += 8) {
        if (index & 1) {
            transform[0] = 0.0f;
            transform[1] = 0.0f;
            transform[2] = 0.5f;
            transform[3] = 0.0f;
            transform[4] = 0.0f;
            transform[5] = 1.0f;
            transform[6] = 0.5f;
        } else {
            transform[0] = 0.5f;
            transform[1] = 0.0f;
            transform[2] = 1.0f;
            transform[3] = 0.0f;
            transform[4] = 0.5f;
            transform[5] = 1.0f;
            transform[6] = 1.0f;
        }
        transform[7] = 1.0f;
    }
}

u8 *billCreateQuadTransform(EffBillQuadFrameConfig *config, u32 resource) {
    u8 *node = billAllocQuadNode(config);
    ((EffBillFrameState *)node)->asset = effCreateTrackSetWithSharedReferences(config->frame.output.timed.count, 4, resource);
    billInitializeQuadRows(node, config);
    return node;
}

u8 *billCloneQuadTransform(u8 *work) {
    u8 *config = ((EffClassWork *)work)->payload;
    u8 *state = (u8 *)((EffClassWork *)work)->resource;
    u8 *node = billAllocQuadNode(config);
    ((EffBillFrameState *)node)->asset = effDuplicateResourceRefs(((EffBillFrameState *)state)->asset);
    billInitializeQuadRows(node, config);
    return node;
}

void billReleaseQuadNode(s32 work) {
    effReleaseResourceRefs(((EffBillFrameState *)work)->asset);
    sdfMemFreeDirect(((EffBillFrameState *)work)->allocation);
}

void billAdvanceQuadInstances(EffClassWork *work) {
    EffBillQuadFrameConfig *config = (EffBillQuadFrameConfig *)work->payload;
    EffBillFrameState *resource;
    EffBillQuadEntry *entries;
    EffTrackSet *asset;
    s32 lifetime;
    s32 itemCount;
    s32 remainingSpawn;
    u32 randomAge;
    s32 remainingItems;
    s32 fadeStart;
    s32 fadeEnd;
    s32 startFrame;
    s32 stopFrame;
    s32 frameStep;
    s32 stopEntry;
    s32 waitingState;
    s32 inactiveState;
    u32 respawn;
    u32 colorA;
    u32 colorB;
    f32 colorVectorA[4];
    f32 colorVectorB[4];
    f32 center[4];
    f32 lateral[4];
    f32 basis[4];
    f32 acceleration;
    u32 *colors;
    u128 *vertices;

    if (config->frame.output.timed.time.progress < work->frame &&
        config->frame.output.timed.time.progress != 0) {
        return;
    }

    lifetime = config->frame.lifetime;
    resource = (EffBillFrameState *)work->resource;
    entries = (EffBillQuadEntry *)resource->entries;
    asset = resource->asset;
    itemCount = (s32)config->frame.output.timed.count;
    if (lifetime == 0) {
        return;
    }

    if (config->reverse != 0) {
        startFrame = lifetime;
        stopFrame = 0;
        frameStep = -1;
        fadeStart = (s32)((1.0f - config->frame.fadeOutFraction) * (f32)lifetime);
        fadeEnd = (s32)((1.0f - config->frame.fadeInFraction) * (f32)lifetime);
    } else {
        startFrame = 0;
        stopFrame = lifetime;
        frameStep = 1;
        fadeStart = (s32)(config->frame.fadeInFraction * (f32)lifetime);
        fadeEnd = (s32)(config->frame.fadeOutFraction * (f32)lifetime);
    }
    if (config->frame.spawnAllOnStart != 0 && work->frame == 0) {
        remainingSpawn = itemCount;
        randomAge = 1;
    } else {
        remainingSpawn = config->frame.spawnCount;
        randomAge = 0;
    }

    colorA = config->colorA;
    vertices = (u128 *)asset->buffer;
    colors = (u32 *)asset->tail;
    center[1] = 0.0f;
    center[3] = 0.0f;
    eeUnpackRGBAWordFloat(colorA, 1.0f / 128.0f);
    SDF_VU0_STOREVECTOR_NOMEM(colorVectorA);
    colorB = config->colorB;
    eeUnpackRGBAWordFloat(colorB, 1.0f / 128.0f);
    SDF_VU0_STOREVECTOR_NOMEM(colorVectorB);

    acceleration = config->acceleration;
    respawn = config->frame.respawn;
    if (itemCount <= 0) {
        return;
    }

    waitingState = -1;
    inactiveState = -2;
    stopEntry = respawn != 0 ? waitingState : inactiveState;
    remainingItems = itemCount;
    for (; remainingItems != 0; remainingItems--, entries++, vertices += 4, colors += 4) {
        EffBillQuadEntry *entry = entries;
        s32 frame = entry->timer;

        if (frame == inactiveState) {
            continue;
        }
        if (frame == waitingState) {
            if (remainingSpawn != 0) {
                f32 randomValue;
                f32 randomFactor;
                f32 heightStartJitter;
                f32 heightEndJitter;
                f32 velocityJitter;
                f32 widthJitter;
                f32 tiltJitter;
                f32 radialStart;
                f32 radialEnd;

                entry->angle = dds3GetRandFloatA(effSharedRandomState) * 6.28318500518798828125f;
                randomFactor = config->radiusJitter;
                randomValue = dds3GetRandFloatA(effSharedRandomState);
                entry->radius = config->radius * (randomValue * randomFactor + (1.0f - randomFactor));
                randomValue = dds3GetRandFloatA(effSharedRandomState);
                heightStartJitter = config->heightStartJitter;
                radialStart = config->heightStart * (randomValue * heightStartJitter + (1.0f - heightStartJitter));
                randomValue = dds3GetRandFloatA(effSharedRandomState);
                heightEndJitter = config->heightEndJitter;
                radialEnd = config->heightEnd * (randomValue * heightEndJitter + (1.0f - heightEndJitter));
                entry->height = radialStart;
                entry->heightStep = (radialEnd - radialStart) / (f32)lifetime;
                randomValue = dds3GetRandFloatA(effSharedRandomState);
                velocityJitter = config->velocityJitter;
                entry->velocity = config->velocity * (randomValue * velocityJitter + (1.0f - velocityJitter));
                randomValue = dds3GetRandFloatA(effSharedRandomState);
                widthJitter = config->widthJitter;
                randomFactor = randomValue * widthJitter + (1.0f - widthJitter);
                entry->width = config->width * randomFactor;
                randomValue = dds3GetRandFloatA(effSharedRandomState);
                tiltJitter = config->tiltJitter;
                entry->tilt = config->tilt * (randomValue * tiltJitter + (1.0f - tiltJitter));
                if (randomAge != 0) {
                    entry->timer = (s32)(dds3GetRandA((struct EffRandState *)effSharedRandomState) % (u32)lifetime);
                } else {
                    entry->timer = startFrame;
                }
                remainingSpawn = (s32)((u32)remainingSpawn - 1u);
            }
            continue;
        }
        if (frame == stopFrame) {
            {
                s32 corner;
                for (corner = 0; corner < 4; corner++) {
                    VU0_STORE_VF(vf0, vertices + corner);
                    colors[corner] = 0;
                }
            }
            entry->timer = stopEntry;
            continue;
        }

        {
            f32 angle = entry->angle;
            f32 time = (f32)frame;
            f32 velocity;
            f32 shapeFactor;
            f32 motionDistance;
            f32 radialOffset;
            f32 cosine;
            f32 sine;
            f32 alpha;
            u32 packedColor;

            if (acceleration < 0.0f) {
                f32 maxTime;
                velocity = entry->velocity;
                maxTime = ((-velocity) / (acceleration * 0.5f)) * 0.5f;
                if (maxTime < time) {
                    time = maxTime;
                }
            } else {
                velocity = entry->velocity;
            }
            motionDistance = (velocity + (acceleration * time) * 0.5f) * time;
            radialOffset = entry->height + entry->heightStep * time;
            shapeFactor = entry->tilt;
            cosine = sdfCos(angle);
            sine = sdfSin(angle);

            center[0] = cosine * shapeFactor;
            center[1] = shapeFactor + -1.0f;
            center[2] = sine * shapeFactor;
            SDF_VU0_LOADVECTOR(center);
            SDF_VU0_COPYVECTOR_A_TO_B();
            VU0_SCALE_VF(vf10, entry->radius);
            SDF_VU0_STOREVECTOR(basis);
            SDF_VU0_COPYVECTOR_B_TO_A();
            VU0_SCALE_VF(vf10, motionDistance);
            SDF_VU0_STOREVECTOR(center);
            center[0] += cosine * radialOffset;
            center[2] += sine * radialOffset;

            VU0_SCALAR_OP(sine, "vaddx.x vf10, vf0, vf2x");
            VU0_SCALAR_OP(0.0f, "vaddx.y vf10, vf0, vf2x");
            cosine = -cosine;
            VU0_SCALAR_OP(cosine, "vaddx.z vf10, vf0, vf2x");
            VU0_SCALE_VF(vf10, entry->width);
            SDF_VU0_STOREVECTOR(lateral);

            SDF_VU0_LOADVECTOR_C(center);
            SDF_VU0_LOADVECTOR(basis);
            SDF_VU0_ADDVECTOR_A_C();
            SDF_VU0_LOADVECTOR_B(lateral);
            SDF_VU0_ADDVECTOR();
            SDF_VU0_STOREVECTOR(vertices + 0);
            SDF_VU0_SUBVECTOR();
            SDF_VU0_SUBVECTOR();
            SDF_VU0_STOREVECTOR(vertices + 1);
            SDF_VU0_LOADVECTOR(basis);
            SDF_VU0_NEGVECTOR_XYZ();
            SDF_VU0_ADDVECTOR_A_C();
            SDF_VU0_LOADVECTOR_B(lateral);
            SDF_VU0_ADDVECTOR();
            SDF_VU0_STOREVECTOR(vertices + 2);
            SDF_VU0_SUBVECTOR();
            SDF_VU0_SUBVECTOR();
            SDF_VU0_STOREVECTOR(vertices + 3);

            if (frame < fadeStart) {
                alpha = (f32)frame / (f32)fadeStart;
            } else if (fadeEnd < frame) {
                alpha = (f32)(lifetime - frame) / (f32)(lifetime - fadeEnd);
            } else {
                alpha = 1.0f;
            }
            SDF_VU0_LOADVECTOR_B(effOnesVector);
            VU0_SCALE_VF(vf11, alpha);
            SDF_VU0_LOADVECTOR(colorVectorB);
            SDF_VU0_MULVECTOR();
            packedColor = eePackRGBAWord(128.0f);
            colors[0] = packedColor;
            colors[1] = packedColor;
            SDF_VU0_LOADVECTOR(colorVectorA);
            SDF_VU0_MULVECTOR();
            packedColor = eePackRGBAWord(128.0f);
            colors[2] = packedColor;
            colors[3] = packedColor;
            entry->timer = (s32)((u32)frame + (u32)frameStep);
        }
    }
}

void billUpdateQuadDrawColorAndTransform(u8 *work) {
    u8 *config = ((BillCellDrawWork *)work)->config;
    u32 limit = ((BillCellDrawWork *)work)->frameLimit;
    u32 progress = ((EffBillTimedHeader *)config)->time.progress;
    u32 *list = ((BillCellDrawWork *)work)->instances;
    EffTrackSet *out = (EffTrackSet *)list[1];
    u128 mtx[4];
    u32 packed;
    u32 unit;
    u32 second;

    if (progress < limit && progress != 0) {
        return;
    }
    second = effSampleColorAlphaTracks(&((EffBillTimedHeader *)config)->colorTrack, &((EffBillTimedHeader *)config)->alphaTrack, limit, progress);
    unit = 0x3C000000;
    eeUnpackRGBAWord(((BillCellDrawWork *)work)->baseColor, unit);
    SDF_VU0_COPYVECTOR_A_TO_B();
    eeUnpackRGBAWord(second, unit);
    SDF_VU0_MULVECTOR();
    packed = eePackRGBAWord(128.0f);
    out->color = packed;
    out->type = ((EffBillTimedHeader *)config)->alphaTrack.surfaceIndex;
    out->flag = ((EffBillFrameHeader *)config)->mode;
    SDF_VU0_LOADVECTOR(work + 0x10);
    ToMatrix();
    SDF_VU0_LOADVECTOR(effOnesVector);
    VU0_SCALAR_OP_CLOBBER(((BillCellDrawWork *)work)->scale, "vmulx.xyzw vf10, vf10, vf2x");
    SDF_VU0_SCALE_XYZ();
    SDF_VU0_LOADVECTOR(work);
    SDF_VU0_VECTOR_W_SET_ONE();
    SDF_VU0_SET_ROW3();
    VU0_STORE_MATRIX(mtx);
    effDrawTrackSet(out, (Matrix4 *)mtx);
}

EffClassWork *effAllocateActiveInstanceWork(u16 kind, void *source) {
    u32 headerSize = 0x40;
    u32 size = effActiveInstanceOperations[kind].payloadSize;
    u8 *effect = sdfChipAllocDirect(size + headerSize);
    ((EffClassWork *)effect)->payload = effect + headerSize;
    ((EffClassWork *)effect)->color = 0x80808080;
    ((EffClassWork *)effect)->scale = 1.0f;
    ((EffClassWork *)effect)->kind = kind;
    ((EffClassWork *)effect)->frame = 0;
    VU0_STORE_VF($vf0, effect);
    VU0_STORE_VF($vf0, effect + 0x10);
    memcpy(((EffClassWork *)effect)->payload, source, size);
    return (EffClassWork *)effect;
}

EffClassWork *effCreateResourceInstanceA(u16 kind, void *source, u32 option) {
    EffClassWork *effect = effAllocateActiveInstanceWork(kind, source);
    effect->resource = (void *)effActiveInstanceOperations[kind].createResource(source, option);
    effActiveInstanceOperations[kind].initialize(effect);
    return effect;
}

EffClassWork *effCreateActiveInstanceFromFile(FileJobPayload *request) {
    u32 *secondary = fileResolveSecondaryBuffer(request);
    void *source;
    switch (request->primary.selector) {
    case 1:
        break;
    case 4:
        secondary = NULL;
        break;
    }
    source = fileResolvePrimaryBuffer(request);
    return effCreateResourceInstanceA(request->option, source, (u32)secondary);
}

void effDestroyActiveInstanceWork(EffClassWork *work) {
    effActiveInstanceOperations[work->kind].destroyResource(work->resource);
    sdfChipFreeDirect(work);
}

EffClassWork *effCreateActiveResource(EffClassWork *work) {
    EffClassWork *effect;
    if (effActiveInstanceOperations[work->kind].cloneResource == NULL) {
        effect = effCreateResourceInstanceA(work->kind, work->payload, 0);
    } else {
        void *clonedResource;
        s32 kind;
        effect = effAllocateActiveInstanceWork(work->kind, work->payload);
        clonedResource = (void *)effActiveInstanceOperations[work->kind].cloneResource(work);
        /* Capture the clone's class before publishing its resource. */
        kind = work->kind;
        effect->resource = clonedResource;
        effActiveInstanceOperations[kind].initialize(effect);
    }
    return effect;
}

void effResetActiveInstanceFrame(EffClassWork *work) {
    effActiveInstanceOperations[work->kind].initialize(work);
    work->frame = 0;
}

void effAdvanceActiveInstanceFrame(EffClassWork *work) {
    effActiveInstanceOperations[work->kind].update(work);
    work->frame = work->frame + 1;
}

void effDispatchActiveInstanceDraw(EffClassWork *work) {
    effActiveInstanceOperations[work->kind].draw(work);
}

INCLUDE_ASM(const s32, "code_0023E1D0", func_r1_0022E420);

INCLUDE_ASM(const s32, "code_0023E1D0", func_r1_0022E460);

INCLUDE_ASM(const s32, "code_0023E1D0", func_r1_0022E470);

void effSetActiveInstanceColor(EffClassWork *work, u32 color) {
    work->color = color;
}

void effSetActiveInstanceScale(EffClassWork *work, f32 scale) {
    work->scale = scale;
}

INCLUDE_RODATA(const s32, "code_0023E1D0", D_r1_00455D70);

EffTrackSet *effCreateTrackSet(s32 count, u16 kind) {
    s32 rows;
    s32 cols;
    s32 size;
    struct sdfMemHandle_t *base;
    u8 *trackStorage;
    EffTrackSet *set;

    switch (kind) {
    case 0:
        cols = 0;
        rows = count * 5;
        break;
    case 1:
        cols = 0;
        rows = count * 13;
        break;
    case 2:
        rows = count * 4;
        cols = 0;
        break;
    case 3:
        rows = count * 4;
        cols = rows;
        break;
    case 4:
        rows = count * 4;
        cols = rows;
        break;
    default:
        rows = 0;
        cols = 0;
        break;
    }
    size = ((rows * 2 + cols) * 2 + rows) * 4;
    size = (((size >> 4) + ((size & 0xF) != 0)) << 4);
    base = sdfMemAllocDirect(size + 0x30);
    trackStorage = (u8 *)sdfMemLock(base);
    set = (EffTrackSet *)(trackStorage + size);
    set->buffer = trackStorage;
    trackStorage += rows * 16;
    if (cols > 0) {
        set->columns = trackStorage;
        trackStorage += cols * 8;
    } else {
        set->columns = 0;
    }
    set->color = 0x80808080;
    set->type = 2;
    set->tail = trackStorage;
    set->rows = rows;
    set->kind = kind;
    set->count = count;
    set->allocation = base;
    set->flag = 0;
    set->shared = 0;
    set->handle = sdfMatNew();
    sdfMatSetConstant(set->handle, 1.0f);
    memset(&D_r1_004D1BC0, 0, 0x2C);
    D_r1_004D1BC0.attr = 0x4000;
    return set;
}

INCLUDE_SDATA(const s32, "code_0023E1D0", D_r1_0046D118);

INCLUDE_SDATA(const s32, "code_0023E1D0", D_r1_0046D120);

INCLUDE_SDATA(const s32, "code_0023E1D0", D_r1_0046D124);

INCLUDE_SDATA(const s32, "code_0023E1D0", D_r1_0046D128);

EffTrackSet *effCreateTrackSetWithSharedReferences(u32 count, u32 kind, u32 sharedRef) {
    EffTrackSet *effect = effCreateTrackSet(count, (u16)kind);

    if (effect->columns != 0) {
        if (sharedRef == 0) {
            switch (effect->kind) {
            case 3:
                if (D_r1_0046D128[0] == 0) {
                    D_r1_0046D130[0] = effCloneSharedReferenceWithValue(effFlashTextureHandles, 0x100);
                }
                D_r1_0046D128[0]++;
                break;
            case 4:
                if (D_r1_0046D128[1] == 0) {
                    D_r1_0046D130[1] = effCloneSharedReferenceWithValue(D_r1_0046D124, 0x101);
                }
                D_r1_0046D128[1]++;
                break;
            }
        } else {
            effect->shared = effCreateSharedTextureReference((struct SdfTextureFileHeader *)sharedRef);
        }
    }
    return effect;
}

/* Release the track set's retained reference, draw asset, and allocation. */
void effReleaseResourceRefs(EffTrackSet *refs) {
    if (refs->columns != 0) {
        RefObj *ref = refs->shared;
        if (ref == NULL) {
            switch (refs->kind) {
            case 3:
                if (--D_r1_0046D128[0] == 0) {
                    effReleaseSharedReference(D_r1_0046D130[0]);
                    D_r1_0046D130[0] = NULL;
                }
                break;
            case 4:
                if (--D_r1_0046D128[1] == 0) {
                    effReleaseSharedReference(D_r1_0046D130[1]);
                    D_r1_0046D130[1] = NULL;
                }
                break;
            }
        } else {
            effReleaseSharedReference(ref);
        }
    }
    sdfMatFreeDelayed(refs->handle);
    sdfMemFreeDirect(refs->allocation);
}

/* Copy a track set: same size and kind, retaining the source's shared reference (or counting one more user of the built-in one). */
EffTrackSet *effDuplicateResourceRefs(const EffTrackSet *original) {
    EffTrackSet *effect = effCreateTrackSet(original->count, original->kind);

    if (effect->columns != 0) {
        if (original->shared != 0) {
            effect->shared = effRetainSharedReference(original->shared);
        } else {
            switch (effect->kind) {
            case 3:
                D_r1_0046D128[0]++;
                break;
            case 4:
                D_r1_0046D128[1]++;
                break;
            }
        }
    }
    return effect;
}

INCLUDE_ASM(const s32, "code_0023E1D0", effDrawTrackSet);

void effLoadFlashTextures(void) {
    D_r1_0046D118[0] = sdfDevLoadFileEx("/effect/flash00.tmx", (u32 *)&effFlashTextureHandles, 0);
    D_r1_0046D118[1] = sdfDevLoadFileEx("/effect/flash01.tmx", (u32 *)(&effFlashTextureHandles + 1), 0);
}

INCLUDE_ASM(const s32, "code_0023E1D0", func_r1_0022EDD0);

INCLUDE_ASM(const s32, "code_0023E1D0", func_r1_0022EDE8);

/* Create a point-set reference and initialize four colors per segment. */
u32 *effSegmentPointerSet(u8 *work) {
    u32 *pointSetRef = sdfChipAllocDirect(4);
    u32 segments = ((EffRingSource *)work)->segments;
    EffPointSet *pointSet;
    u32 *entry;
    u32 groups;
    u32 i;
    u32 first;
    u32 second;
    u32 third;

    if (segments < 3) {
        ((EffRingSource *)work)->segments = 3;
        segments = 3;
    }
    pointSet = effCreatePointSet4(segments);
    first = ((EffRingSource *)work)->firstColor;
    groups = pointSet->rows / 4;
    *pointSetRef = (u32)pointSet;
    entry = (u32 *)pointSet->tail;
    second = ((EffRingSource *)work)->middleColor;
    third = ((EffRingSource *)work)->lastColor;
    for (i = 0; i < groups; i++) {
        entry[0] = first;
        entry[1] = second;
        entry[2] = second;
        entry[3] = third;
        entry += 4;
    }
    return pointSetRef;
}

void effReleaseRingResourceHandle(u32 *handle) {
    effAssetQueueRelease((EffPointSet *)*handle);
    sdfChipFreeDirect(handle);
}

/* Generate the timed ring's four radial point bands; reverse-time mode is retained. */
void effUpdateExpandingRingPoints(EffClassWork *work) {
    EffExpandingRingParams *config = work->payload;
    EffPointSet *points = ((EffRingResource *)work->resource)->pointSet;
    f32 direction[4];
    f32 bands[3][4];
    f32 time;
    f32 speed;
    f32 acceleration;
    f32 radius;
    f32 angle;
    f32 angleStep;
    f32 limit;
    f32 firstBand;
    f32 secondBand;
    f32 thirdBand;
    f32 (*out)[4];
    u32 groups;
    u32 i;

    if (config->duration < (s32)work->frame && config->duration != 0) {
        return;
    }
    if (config->reverseTime != 0) {
        time = (f32)(s32)((u32)config->duration - work->frame);
    } else {
        time = (f32)(s32)work->frame;
    }
    speed = ffabsf(config->speed);
    acceleration = config->acceleration;
    if (acceleration < 0.0f) {
        limit = -speed / (acceleration * 0.5f) * 0.5f;
        if (limit < time) {
            time = limit;
        }
    }
    radius = (speed + acceleration * time * 0.5f) * time;
    radius += config->baseRadius;
    groups = (u32)(points->rows / 4);
    out = (f32 (*)[4])points->buffer;
    angle = 0.0f;
    angleStep = EFFECT_RING_FULL_TURN / (f32)config->segments;
    direction[1] = 0.0f;
    direction[3] = 1.0f;
    firstBand = config->widths[0] + radius;
    secondBand = firstBand + config->widths[1];
    thirdBand = secondBand + config->widths[2];
    SDF_VU0_LOADVECTOR(effOnesVector);
    SDF_VU0_COPYVECTOR_A_TO_B();
    VU0_SCALAR_OP_CLOBBER(firstBand, "vmulx.xyzw vf10, vf10, vf2x");
    SDF_VU0_STOREVECTOR(bands[0]);
    SDF_VU0_COPYVECTOR_B_TO_A();
    VU0_SCALAR_OP_CLOBBER(secondBand, "vmulx.xyzw vf10, vf10, vf2x");
    VU0_SCALAR_OP_CLOBBER(thirdBand, "vmulx.xyzw vf11, vf11, vf2x");
    SDF_VU0_STOREVECTOR(bands[1]);
    SDF_VU0_STOREVECTOR_B(bands[2]);
    for (i = 0; i < groups; i++) {
        direction[0] = sdfCos(angle);
        direction[2] = sdfSin(angle);
        SDF_VU0_LOADVECTOR(direction);
        SDF_VU0_COPYVECTOR_A_TO_B();
        VU0_SCALAR_OP_CLOBBER(radius, "vmulx.xyzw vf10, vf10, vf2x");
        SDF_VU0_STOREVECTOR(out[0]);
        SDF_VU0_LOADVECTOR(bands[0]);
        SDF_VU0_MULVECTOR();
        SDF_VU0_STOREVECTOR(out[1]);
        SDF_VU0_LOADVECTOR(bands[1]);
        SDF_VU0_MULVECTOR();
        SDF_VU0_STOREVECTOR(out[2]);
        SDF_VU0_LOADVECTOR(bands[2]);
        SDF_VU0_MULVECTOR();
        SDF_VU0_STOREVECTOR(out[3]);
        angle += angleStep;
        out += 4;
    }
}

void billDrawCellBlendA(BillCellDrawWork *work) {
    u8 *config = work->config;
    u32 limit = work->frameLimit;
    u32 progress = ((EffBillTimedHeader *)config)->time.progress;
    u32 *list = work->instances;
    EffPointSet *out = (EffPointSet *)(u32)list[0];
    u128 mtx[4];
    u32 packed;
    u32 unit;
    u32 second;

    if (progress < limit && progress != 0) {
        return;
    }
    second = effSampleColorAlphaTracks(&((EffBillTimedHeader *)config)->colorTrack, &((EffBillTimedHeader *)config)->alphaTrack, limit, progress);
    unit = 0x3C000000;
    eeUnpackRGBAWord(work->baseColor, unit);
    SDF_VU0_COPYVECTOR_A_TO_B();
    eeUnpackRGBAWord(second, unit);
    SDF_VU0_MULVECTOR();
    packed = eePackRGBAWord(128.0f);
    out->color = packed;
    if ((packed & 0xFF000000) != 0) {
        out->type = ((EffBillTimedHeader *)config)->alphaTrack.surfaceIndex;
        out->flag = ((EffBillOutputHeader *)config)->outputMode;
        SDF_VU0_LOADVECTOR(work->transform);
        ToMatrix();
        SDF_VU0_LOADVECTOR(effOnesVector);
        VU0_SCALAR_OP_CLOBBER(work->scale, "vmulx.xyzw vf10, vf10, vf2x");
        SDF_VU0_SCALE_XYZ();
        SDF_VU0_LOADVECTOR(work);
        SDF_VU0_VECTOR_W_SET_ONE();
        SDF_VU0_SET_ROW3();
        VU0_STORE_MATRIX(mtx);
        effDrawFourPointGroups(out, (Matrix4 *)mtx);
    }
}

void effResetClassFrameAndFlags(s32 work) {
    EffClassWork *effect;

    effect = ((EffClassDrawState *)((EffClassWork *)work)->resource)->effect;
    ((EffCounterHeader *)((EffClassDrawState *)((EffClassWork *)work)->resource)->trackSet)->frame = 0;
    effInitializeClassFrame(effect);
}

EffClassDrawState *effCreateScaledClassDrawState(EffRingClassConfig *source) {
    u32 count = source->ring.segments;
    u32 size;
    struct sdfMemHandle_t *allocation;
    f32 *scales;
    EffClassDrawState *state;
    EffTrackSet *tracks;
    u32 *colors;
    u32 first;
    u32 second;
    u32 i;

    if (count < 3) {
        source->ring.segments = 3;
        count = 3;
    }
    size = count * sizeof(f32);
    allocation = sdfMemAllocDirect(size + sizeof(EffClassDrawState));
    scales = (f32 *)sdfMemLock(allocation);
    state = (EffClassDrawState *)((u8 *)scales + size);
    state->allocation = allocation;
    state->scales = scales;
    memcpy(source->classConfig, source, sizeof(source->classConfig));
    state->effect = effCreateClassWork(1, source->classConfig);
    tracks = effCreateTrackSetWithSharedReferences(count, 2, 0);
    first = source->ring.firstColor;
    state->trackSet = tracks;
    colors = (u32 *)tracks->tail;
    second = source->ring.middleColor;
    for (i = 0; i < count; i++) {
        colors[0] = first;
        colors[1] = first;
        colors[2] = (first & second) + (((first ^ second) & 0xFEFEFEFE) >> 1);
        colors[3] = first;
        colors += 4;
        *scales++ = source->scale * (dds3GetRandFloatA(effSharedRandomState) * source->scaleRand + (1.0f - source->scaleRand));
    }
    return state;
}

void effReleaseClassDrawResources(s32 work) {
    effReleaseResourceRefs(((EffClassDrawState *)work)->trackSet);
    effDestroyClassWork(((EffClassDrawState *)work)->effect);
    sdfMemFreeDirect(((EffClassDrawState *)work)->allocation);
}

/* Advance frame geometry and rebuild scaled row offsets with predecessor wrapping. */
void effUpdateScaledClassFrameRows(BillCellDrawWork *work) {
    u8 *config = work->config;
    s32 limit = work->frameLimit;
    s32 progress = ((EffBillTimedHeader *)config)->time.progress;
    u32 *list = work->instances;
    EffClassWork *classWork;
    f32 *scales;
    EffClassFrameResource **resource;
    EffClassRowOutput *output;
    EffClassFrameResource *frames;
    u8 *src;
    u8 *dst;
    u8 *wrap;
    u32 count;
    u32 lastFrame;
    u32 i;

    classWork = (EffClassWork *)list[1];
    resource = (EffClassFrameResource **)classWork->resource;
    output = (EffClassRowOutput *)list[2];
    scales = (f32 *)list[0];

    if (progress < limit && progress != 0) {
        return;
    }
    effAdvanceClassFrame(classWork);
    count = ((EffBillTimedHeader *)config)->count;
    frames = *resource;
    lastFrame = frames->frameCount - 1;
    dst = output->rows;
    src = frames->rows;
    if (config[0xC4] != 0) {
        return;
    }
    i = 0;
    if (count == 0) {
        return;
    }
    wrap = src + lastFrame * 0x10 - 0x60;
    do {
        SDF_VU0_LOADVECTOR(src + 0x10);
        SDF_VU0_COPYVECTOR_A_TO_C();
        SDF_VU0_LOADVECTOR_B(src + 0x30);
        SDF_VU0_STOREVECTOR(dst + 0x20);
        SDF_VU0_SUBVECTOR();
        SDF_VU0_NORMALIZE();
        VU0_SCALAR_OP(*scales, "vmulx.xyzw vf10, vf10, vf2x");
        SDF_VU0_ADDVECTOR_A_C();
        SDF_VU0_STOREVECTOR(dst);
        SDF_VU0_COPYVECTOR(dst + 0x10, src + 0x50);
        if (i == 0) {
            SDF_VU0_COPYVECTOR(dst + 0x30, wrap);
        } else {
            SDF_VU0_COPYVECTOR(dst + 0x30, src - 0x30);
        }
        i++;
        dst += 0x40;
        wrap += 0x40;
        src += 0x40;
        scales++;
    } while (i < count);
}

void billDrawClassUpdatedCellBlend(BillCellDrawWork *work) {
    u8 *config = work->config;
    u32 limit = work->frameLimit;
    u32 progress = ((EffBillTimedHeader *)config)->time.progress;
    u32 *list = work->instances;
    EffTrackSet *out = (EffTrackSet *)list[2];
    u128 mtx[4];
    s32 color1[4];
    s32 color2[4];
    s32 blended[4];
    u32 packed;
    u32 unit;
    u32 second;

    if (progress < limit && progress != 0) {
        return;
    }
    second = effSampleColorAlphaTracks(&((EffBillTimedHeader *)config)->colorTrack, &((EffBillTimedHeader *)config)->alphaTrack, limit, progress);
    unit = 0x3C000000;
    color1[0] = work->baseColor;
    EE_MMI_RGBA_UNPACK(color1, unit);
    SDF_VU0_COPYVECTOR_A_TO_B();
    color2[0] = second;
    EE_MMI_RGBA_UNPACK(color2, unit);
    SDF_VU0_MULVECTOR();
    EE_MMI_RGBA_PACK_UNCLOBBERED(packed);
    blended[0] = packed;
    out->color = packed;
    if (packed & 0xFF000000) {
        EffClassWork *dst = (EffClassWork *)list[1];

        dst->color = work->baseColor;
        dst->scale = work->scale;
        SDF_VU0_COPYVECTOR(dst->transform, work);
        SDF_VU0_COPYVECTOR(dst->transform + 0x10, work->transform);
        effRunClassPostFrame((s32)dst);
        out->type = ((EffBillTimedHeader *)config)->alphaTrack.surfaceIndex;
        out->flag = ((EffBillOutputHeader *)config)->outputMode;
        SDF_VU0_LOADVECTOR(work->transform);
        ToMatrix();
        SDF_VU0_LOADVECTOR(effOnesVector);
        VU0_SCALAR_OP(work->scale, "vmulx.xyzw vf10, vf10, vf2x");
        SDF_VU0_SCALE_XYZ();
        SDF_VU0_LOADVECTOR(work);
        SDF_VU0_VECTOR_W_SET_ONE();
        SDF_VU0_SET_ROW3();
        VU0_STORE_MATRIX(mtx);
        effDrawTrackSet(out, (Matrix4 *)mtx);
    }
}

INCLUDE_ASM(const s32, "code_0023E1D0", func_r1_0022F840);

/* Create a point-set reference and initialize four colors per segment. */
EffRingResource *effCreateRingHandle(EffRadialRingParams *work) {
    EffRingResource *pointSetRef = sdfChipAllocDirect(sizeof(EffRingResource));
    u32 segments = work->segments;
    EffPointSet *pointSet;
    u32 *entry;
    u32 groups;
    u32 i;
    u32 first;
    u32 second;
    u32 third;

    if (segments < 3) {
        work->segments = 3;
        segments = 3;
    }
    pointSet = effCreatePointSet4(segments);
    first = work->firstColor;
    groups = pointSet->rows / 4;
    pointSetRef->pointSet = pointSet;
    entry = (u32 *)pointSet->tail;
    second = work->middleColor;
    third = work->lastColor;
    for (i = 0; i < groups; i++) {
        entry[0] = first;
        entry[1] = second;
        entry[2] = second;
        entry[3] = third;
        entry += 4;
    }
    return pointSetRef;
}

void effReleaseRingHandle(EffRingResource *handle) {
    effAssetQueueRelease(handle->pointSet);
    sdfChipFreeDirect(handle);
}

void effUpdateCenteredRadialRingVertices(EffClassWork *work) {
    EffRadialRingParams *config = work->payload;
    EffPointSet *points = ((EffRingResource *)work->resource)->pointSet;
    f32 direction[4];
    f32 bands[3][4];
    f32 time;
    f32 speed;
    f32 acceleration;
    f32 radius;
    f32 angle;
    f32 angleStep;
    f32 limit;
    f32 firstBand;
    f32 secondBand;
    f32 thirdBand;
    f32 (*out)[4];
    u32 groups;
    u32 i;

    if ((s32)config->duration < (s32)work->frame && config->duration != 0) {
        return;
    }
    if (config->reverseTime != 0) {
        time = (f32)(s32)((u32)config->duration - work->frame);
    } else {
        time = (f32)(s32)work->frame;
    }
    speed = ffabsf(config->speed);
    acceleration = config->acceleration;
    if (acceleration < 0.0f) {
        limit = -speed / (acceleration * 0.5f) * 0.5f;
        if (limit < time) {
            time = limit;
        }
    }
    radius = (speed + acceleration * time * 0.5f) * time;
    radius += config->baseRadius;
    groups = (u32)(points->rows / 4);
    out = (f32 (*)[4])points->buffer;
    angle = 0.0f;
    angleStep = EFFECT_RING_FULL_TURN / (f32)config->segments;
    direction[1] = 0.0f;
    direction[3] = 1.0f;
    firstBand = config->widths[0] + radius;
    secondBand = firstBand + config->widths[1];
    thirdBand = secondBand + config->widths[2];
    SDF_VU0_LOADVECTOR(effOnesVector);
    SDF_VU0_COPYVECTOR_A_TO_B();
    VU0_SCALAR_OP_CLOBBER(firstBand, "vmulx.xyzw vf10, vf10, vf2x");
    SDF_VU0_STOREVECTOR(bands[0]);
    SDF_VU0_COPYVECTOR_B_TO_A();
    VU0_SCALAR_OP_CLOBBER(secondBand, "vmulx.xyzw vf10, vf10, vf2x");
    VU0_SCALAR_OP_CLOBBER(thirdBand, "vmulx.xyzw vf11, vf11, vf2x");
    SDF_VU0_STOREVECTOR(bands[1]);
    SDF_VU0_STOREVECTOR_B(bands[2]);
    for (i = 0; i < groups; i++) {
        direction[0] = sdfCos(angle);
        direction[2] = sdfSin(angle);
        VU0_STORE_VF(vf0, out[0]);
        SDF_VU0_LOADVECTOR_B(direction);
        SDF_VU0_LOADVECTOR(bands[0]);
        SDF_VU0_MULVECTOR();
        SDF_VU0_STOREVECTOR(out[1]);
        SDF_VU0_LOADVECTOR(bands[1]);
        SDF_VU0_MULVECTOR();
        SDF_VU0_STOREVECTOR(out[2]);
        SDF_VU0_LOADVECTOR(bands[2]);
        SDF_VU0_MULVECTOR();
        SDF_VU0_STOREVECTOR(out[3]);
        angle += angleStep;
        out += 4;
    }
}

void billDrawCellBlendB(EffClassWork *work) {
    EffRadialRingParams *config = work->payload;
    u32 limit = work->frame;
    u32 progress = config->duration;
    EffRingResource *list = (EffRingResource *)work->resource;
    EffPointSet *out = list->pointSet;
    u128 mtx[4];
    u32 packed;
    u32 unit;
    u32 second;

    if (progress < limit && progress != 0) {
        return;
    }
    second = effSampleColorAlphaTracks(&config->colorTrack, &config->alphaTrack, limit, progress);
    unit = 0x3C000000;
    eeUnpackRGBAWord(work->color, unit);
    SDF_VU0_COPYVECTOR_A_TO_B();
    eeUnpackRGBAWord(second, unit);
    SDF_VU0_MULVECTOR();
    packed = eePackRGBAWord(128.0f);
    out->color = packed;
    if ((packed & 0xFF000000) != 0) {
        out->type = config->alphaTrack.surfaceIndex;
        out->flag = config->flag;
        SDF_VU0_LOADVECTOR(work->vectors.orientation);
        ToMatrix();
        SDF_VU0_LOADVECTOR(effOnesVector);
        VU0_SCALAR_OP_CLOBBER(work->scale, "vmulx.xyzw vf10, vf10, vf2x");
        SDF_VU0_SCALE_XYZ();
        SDF_VU0_LOADVECTOR(work);
        SDF_VU0_VECTOR_W_SET_ONE();
        SDF_VU0_SET_ROW3();
        VU0_STORE_MATRIX(mtx);
        effDrawFourPointGroups(out, (Matrix4 *)mtx);
    }
}

EffClassWork *effCreateClassWork(u16 kind, void *source) {
    u32 headerSize = 0x40;
    u32 size = effClassWorkOperations[kind].payloadSize;
    u8 *effect = sdfChipAllocDirect(size + headerSize);
    ((EffClassWork *)effect)->payload = effect + headerSize;
    ((EffClassWork *)effect)->color = 0x80808080;
    ((EffClassWork *)effect)->scale = 1.0f;
    ((EffClassWork *)effect)->frame = 0;
    ((EffClassWork *)effect)->kind = kind;
    VU0_STORE_VF_UNCLOBBERED($vf0, effect);
    VU0_STORE_VF_UNCLOBBERED($vf0, effect + 0x10);
    memcpy(((EffClassWork *)effect)->payload, source, size);
    ((EffClassWork *)effect)->resource = effClassWorkOperations[kind].createResource(source);
    effClassWorkOperations[kind].initialize(effect);
    return (EffClassWork *)effect;
}

EffClassWork *effCreateClassWorkFromFile(FileJobPayload *request) {
    void *source;

    source = fileResolvePrimaryBuffer(request);
    return effCreateClassWork(request->option, source);
}

void effDestroyClassWork(EffClassWork *work) {
    effClassWorkOperations[work->kind].destroyResource(work->resource);
    sdfChipFreeDirect(work);
}

EffClassWork *effCloneClassWork(EffClassWork *work) {
    return effCreateClassWork(work->kind, work->payload);
}

void effInitializeClassFrame(EffClassWork *work) {
    effClassWorkOperations[work->kind].initialize(work);
    work->frame = 0;
}

INCLUDE_ASM(const s32, "code_0023E1D0", func_r1_0022FEB0);

void effRunClassPostFrame(EffClassWork *work) {
    effClassWorkOperations[work->kind].draw(work);
}

void effUpdateClassFrame(EffClassWork *work) {
    effAdvanceClassFrame(work);
    effRunClassPostFrame(work);
}

INCLUDE_ASM(const s32, "code_0023E1D0", func_r1_0022FF78);

INCLUDE_ASM(const s32, "code_0023E1D0", func_r1_0022FF88);

void effSetClassWorkColor(EffClassWork *work, u32 color) {
    work->color = color;
}

void effSetClassWorkScale(EffClassWork *work, f32 scale) {
    work->scale = scale;
}

EffPointSet *effCreatePointSet4(u32 count) {
    s32 rows = count * 4 + 4;
    s32 size = rows * 20;
    struct sdfMemHandle_t *base;
    u8 *data;
    EffPointSet *set;

    size = ((size >> 4) + ((size & 0xF) != 0)) << 4;
    base = sdfMemAllocDirect(size + 0x20);
    data = (u8 *)sdfMemLock(base);
    set = (EffPointSet *)(data + size);
    set->buffer = data;
    data += rows * 16;
    set->color = 0x80808080;
    set->type = 2;
    set->rows = rows;
    set->allocation = base;
    set->tail = data;
    set->flag = 0;
    set->handle = sdfMatNew();
    sdfMatSetConstant(set->handle, 1.0f);
    memset(D_r1_004D1BF0, 0, 0x2C);
    D_r1_004D1BF0[0].attr = 0x4000;
    return set;
}

/* Queue the draw asset for release and return the backing allocation. */
void effAssetQueueRelease(EffPointSet *set) {
    sdfMatFreeDelayed(set->handle);
    sdfMemFreeDirect(set->allocation);
}

INCLUDE_ASM(const s32, "code_0023E1D0", effDrawFourPointGroups);

/* Create an empty surface node for handleCount grid cells; no handles are allocated yet. */
EffectSurfaceNode *effCreateSurfaceNode(u32 handleCount) {
    EffectSurfaceNode *node = sdfChipAllocZeroDirect(sizeof(EffectSurfaceNode));
    node->handleCount = handleCount;
    node->color = 0x80808080;
    node->opacity = 1.0f;
    node->index = 0;
    node->record = 0;
    node->count = 1;
    return node;
}

EffectSurfaceNode *effCreateSurfaceNodeForGrid(FileKeyBlock *grid) {
    u32 n;
    s32 w = grid->emissionDuration;
    /* Retail 002A5A58 interprets the capacity factors signed (MULT). */
    n = (w ? w : (s32)grid->spawnRate) * (w ? (s32)grid->spawnRate : grid->length);
    return effCreateSurfaceNode(n > 100 ? 100 : n);
}

EffectSurfaceNode *effResourceReferenceReplaceFromFile(FileJobPayload *work) {
    u8 *source = fileResolvePrimaryBuffer(work);
    FileKeyBlock *params = (FileKeyBlock *)(source + 0x1C);
    EffectSurfaceNode *node = effCreateSurfaceNodeForGrid(params);

    effRebuildSurfaceHandles(node, work->option, source);
    effReplaceResourceRef(node, work->option, params);
    return node;
}

EffectSurfaceNode *effInitializeSurfaceForKind(FileJobPayload *work) {
    EffectSurfaceNode *node = effResourceReferenceReplaceFromFile(work);
    u32 *secondary = fileResolveSecondaryBuffer(work);

    if (secondary == NULL) {
        return node;
    }
    switch (work->primary.selector) {
    case 1:
        effReplaceSurfacePrimaryBillboard(node, (u32)secondary);
        break;
    case 2:
        effReplaceSurfaceFlaggedBillboard(node, (u32)secondary);
        break;
    case 3:
        break;
    case 4:
        effSetSurfaceRetainedResource(node, *secondary);
        break;
    case 5:
        effRebuildSurfaceJobs(node, secondary);
        break;
    case 6:
        break;
    case 7:
        effReplaceSurfaceResourceHolder(node, (u32)secondary);
        break;
    }
    node->kind = work->primary.selector;
    return node;
}

void effDestroySurfaceNode(EffectSurfaceNode *node) {
    u32 i;
    u32 count;

    if (node->resource != NULL) {
        dds3BillboardDelete(node->resource);
    }
    if (node->jobAllocation != 0) {
        count = node->record->count;
        for (i = 0; i < count; i++) {
            fileJobDestroy(node->jobs[i]);
        }
        sdfMemFreeDirect(node->jobAllocation);
    }
    if (node->index != 0) {
        for (i = 0; i < node->handleCount; i++) {
            effReleaseSurfaceGridBuffers((EffSurfaceGridNode *)((u32 *)node->index)[i]);
        }
        sdfMemFreeDirect(node->handleBuffer);
    }
    if (node->resourceHolder != 0) {
        effReleaseReferenceHolder(node->resourceHolder);
    }
    if (node->record != 0) {
        effPCPHelperBEDGetModel(node->record);
    }
    sdfChipFreeDirect(node);
}

EffectSurfaceNode *effRecreateSurfaceNodeFromWork(u8 *work) {
    FileKeyBlock *params = (FileKeyBlock *)((EffectSurfaceNode *)work)->record->data1;
    EffectSurfaceNode *node = effCreateSurfaceNodeForGrid(params);
    effRebuildSurfaceHandles(node, ((EffectSurfaceNode *)work)->record->type, &((EffectSurfaceNode *)work)->params);
    effReplaceResourceRef(node, ((EffectSurfaceNode *)work)->record->type, params);
    return node;
}

EffectSurfaceNode *effCreateSurfaceGridWithConfiguration(u8 *work) {
    FileKeyBlock *params = (FileKeyBlock *)((EffectSurfaceNode *)work)->record->data1;
    EffectSurfaceNode *node = effCreateSurfaceNodeForGrid(params);
    effRebuildSurfaceHandles(node, ((EffectSurfaceNode *)work)->record->type, &((EffectSurfaceNode *)work)->params);
    effReplaceResourceRef(node, ((EffectSurfaceNode *)work)->record->type, params);
    effCloneSurfaceGridResources(node, work);
    return node;
}

/* Replace the destination's kind-selected resources with clones or retained references. */
void effCloneSurfaceGridResources(EffectSurfaceNode *dst, u8 *work) {
    EffectSurfaceNode *src = (EffectSurfaceNode *)work;
    u32 selector = src->kind;

    switch (selector) {
    case 1:
    case 2:
    case 4:
        if (dst->resource != NULL) {
            dds3BillboardDelete(dst->resource);
        }
        dst->resource = dds3BillboardCopy(src->resource);
        dds3BillboardSetAnimColorAdvMode(dst->resource);
        if (dst->record != 0) {
            FileSlotTable *record = dst->record;
            dds3BillboardSetMode(dst->resource, (s16)((FileKeyBlock *)record->data0)->alphaTrack.surfaceIndex);
        }
        break;
    case 5: {
        u32 count = src->record->count;
        s32 size;
        u32 i;

        if (dst->jobAllocation != 0) {
            for (i = 0; i < count; i++) {
                fileJobDestroy(dst->jobs[i]);
            }
            sdfMemFreeDirect(dst->jobAllocation);
            dst->jobs = 0;
            dst->jobAllocation = 0;
        }
        size = count * 4;
        if (size == 0) {
            return;
        }
        dst->jobAllocation = sdfMemAllocDirect(size);
        dst->jobs = (FileJobPayload **)sdfMemLock(dst->jobAllocation);
        for (i = 0; i < count; i++) {
            dst->jobs[i] = fileJobCreateChild(src->jobs[0]);
        }
        break;
    }
    case 7:
        if (dst->resourceHolder != 0) {
            effReleaseReferenceHolder(dst->resourceHolder);
        }
        dst->resourceHolder = effReferenceObjectRetain(src->resourceHolder);
        break;
    }
    dst->kind = src->kind;
}

/* Replace the per-cell grid handles from the copied source header. kind is unused here. */
void effRebuildSurfaceHandles(EffectSurfaceNode *node, u16 kind, void *source) {
    EffSurfaceParams *params = source;
    u32 size;
    u32 i;
    EffSurfaceGridNode *handle;

    node->params = *params;
    size = node->handleCount * 4;
    if (size == 0) {
        return;
    }
    if (node->index != 0) {
        for (i = 0; i < node->handleCount; i++) {
            effReleaseSurfaceGridBuffers((EffSurfaceGridNode *)((u32 *)node->index)[i]);
        }
        sdfMemFreeDirect(node->handleBuffer);
    }
    node->handleBuffer = sdfMemAllocDirect(size);
    node->index = sdfMemLock(node->handleBuffer);
    for (i = 0; i < node->handleCount; i++) {
        handle = effCreateSurfaceGridNode(params->opaque00[0], params->opaque00[1]);
        ((u32 *)node->index)[i] = (u32)handle;
        effFillSurfaceGridColorGradient(handle, params->opaque00 + 2);
    }
}

void effReplaceResourceRef(EffectSurfaceNode *node, u32 entryId, const void *resource) {
    if (node->record != NULL) {
        effPCPHelperBEDGetModel(node->record);
    }
    node->record = fileAllocateGridRecordSlots((u16)entryId, node->handleCount, resource);
}

void effSetSurfaceRetainedResource(EffectSurfaceNode *node, u32 resourceId) {
    billHandle_t *resource = node->resource;
    if (resource != 0) {
        dds3BillboardDelete(resource);
    }
    resource = dds3BillboardGeneral(resourceId);
    node->resource = resource;
    if (node->record != 0) {
        dds3BillboardSetMode(resource, (s16)((FileKeyBlock *)node->record->data0)->alphaTrack.surfaceIndex);
    }
}

void effReplaceSurfacePrimaryBillboard(EffectSurfaceNode *node, u32 resourceId) {
    billHandle_t *resource = node->resource;
    if (resource != 0) {
        dds3BillboardDelete(resource);
    }
    resource = dds3BillboardCreate(0, resourceId);
    node->resource = resource;
    if (node->record != 0) {
        dds3BillboardSetMode(resource, (s16)((FileKeyBlock *)node->record->data0)->alphaTrack.surfaceIndex);
    }
}

void effReplaceSurfaceFlaggedBillboard(EffectSurfaceNode *node, u32 resourceId) {
    billHandle_t *resource = node->resource;
    if (resource != 0) {
        dds3BillboardDelete(resource);
    }
    resource = dds3BillboardCreate(1, resourceId);
    node->resource = resource;
    dds3BillboardSetAnimColorAdvMode(resource);
    if (node->record != 0) {
        dds3BillboardSetMode(node->resource, (s16)((FileKeyBlock *)node->record->data0)->alphaTrack.surfaceIndex);
    }
}

void effRebuildSurfaceJobs(EffectSurfaceNode *node, void *source) {
    u32 count = node->record->count;
    u32 i;
    u32 size;

    if (node->jobAllocation != 0) {
        for (i = 0; i < count; i++) {
            fileJobDestroy(node->jobs[i]);
        }
        sdfMemFreeDirect(node->jobAllocation);
        node->jobs = 0;
        node->jobAllocation = 0;
    }
    size = count * 4;
    if (size != 0) {
        node->jobAllocation = sdfMemAllocDirect(size);
        node->jobs = (FileJobPayload **)sdfMemLock(node->jobAllocation);
        node->jobs[0] = fileJobCreateFromJob((FileJobPayload *)source);
        for (i = 1; i < count; i++) {
            node->jobs[i] = fileJobCreateChild(node->jobs[0]);
        }
    }
}

void effReplaceSurfaceResourceHolder(EffectSurfaceNode *node, u32 resource) {
    struct EffExpandedList *holder;

    if (node->resourceHolder != 0) {
        effReleaseReferenceHolder(node->resourceHolder);
    }
    holder = effCreateTextureAnimationSet(resource);
    node->resourceHolder = holder;
}

void effClearSurfaceNodeRecordReferences(s32 node) {
    if (((EffectSurfaceNode *)node)->record != 0) {
        fileClearRecordReferences(((EffectSurfaceNode *)node)->record);
        return;
    }
}

void effAcquireSurfaceRecord(s32 node) {
    if (((EffectSurfaceNode *)node)->record != 0) {
        fileAcquireRecord(((EffectSurfaceNode *)node)->record);
        return;
    }
}

INCLUDE_ASM(const s32, "code_0023E1D0", func_r1_00230EB0);

INCLUDE_ASM(const s32, "code_0023E1D0", func_r1_00231B70);

void effMenuRecordVectorSet(s32 node, const u128 *value) {
    mnuRecordSetVector(((EffectSurfaceNode *)node)->record, value);
}

void effSetSurfaceRecordSecondaryVector(s32 node, const u128 *value) {
    fileSetRecordSecondVector(((EffectSurfaceNode *)node)->record, value);
}

void effSetSurfaceNodeColor(EffectSurfaceNode *work, u32 color) {
    work->color = color;
}

void effIndexedFloatCallbackDispatch(ValPtr44 *p, float v) {
    p->f08 = v;
    dds3DispatchIndexedCallback(p->p44, v);
}

INCLUDE_ASM(const s32, "code_0023E1D0", effCreateSurfaceGridNode);

/* vu0 routine: fade-blended vertex colours written into the surface node's tail */
void effFillSurfaceGridColorGradient(EffSurfaceGridNode *node, u32 *colors) {
    u32 count = node->rows / 3;
    u32 *out = node->tail;
    f32 step = 1.0f / count;
    f32 t = 0.0f;
    f32 corner0[4];
    f32 corner2[4];
    f32 corner1[4];
    f32 corner3[4];
    s32 color0[4];
    s32 color1[4];
    s32 color2[4];
    s32 color3[4];
    s32 blendedA[4];
    s32 blendedB[4];
    u32 packedA;
    u32 packedB;
    u32 i;

    color0[0] = colors[0];
    EE_MMI_RGBA_UNPACK(color0, 1.0f / 128.0f);
    SDF_VU0_STOREVECTOR_NOMEM(corner0);
    color1[0] = colors[1];
    EE_MMI_RGBA_UNPACK(color1, 1.0f / 128.0f);
    SDF_VU0_STOREVECTOR_NOMEM(corner1);
    color2[0] = colors[2];
    EE_MMI_RGBA_UNPACK(color2, 1.0f / 128.0f);
    SDF_VU0_STOREVECTOR_NOMEM(corner2);
    color3[0] = colors[3];
    EE_MMI_RGBA_UNPACK(color3, 1.0f / 128.0f);
    SDF_VU0_STOREVECTOR_NOMEM(corner3);
    for (i = 0; i < count; i++) {
        SDF_VU0_LOADVECTOR(corner0);
        SDF_VU0_LOADVECTOR_B(corner2);
        VU0_SCALAR_OP(t, "vmulx.xyzw vf10, vf10, vf2x");
        VU0_SCALAR_OP(1.0f - t, "vmulx.xyzw vf11, vf11, vf2x");
        SDF_VU0_ADDVECTOR();
        EE_MMI_RGBA_PACK_UNIT(packedA, 128.0f);
        blendedA[0] = packedA;
        out[1] = blendedA[0];
        SDF_VU0_LOADVECTOR(corner1);
        SDF_VU0_LOADVECTOR_B(corner3);
        VU0_SCALAR_OP(t, "vmulx.xyzw vf10, vf10, vf2x");
        VU0_SCALAR_OP(1.0f - t, "vmulx.xyzw vf11, vf11, vf2x");
        SDF_VU0_ADDVECTOR();
        EE_MMI_RGBA_PACK_UNIT(packedB, 128.0f);
        blendedB[0] = packedB;
        out[2] = out[0] = blendedB[0];
        t += step;
        out += 3;
    }
}

void effReleaseSurfaceGridBuffers(EffSurfaceGridNode *node) {
    sdfMatFreeDelayed(node->material);
    sdfMemFreeDirect(node->allocation);
}

void effResetSurfaceGridFrame(EffSurfaceGridNode *node) {
    node->field_14 = 0;
    node->field_18 = 3;
}

/* Three-vector sampler reads the primary surface ring counters. */
void effCopyRingFrameVectors(EffSurfaceGridNode *node, u128 *destination, s32 frame) {
    s32 index = node->field_18 - 3 * (node->columns * (frame - 1) + frame);
    u128 *source;
    s32 i;

    if (index < 3) {
        index += node->rows - 3;
    }
    source = node->buffer + index;
    for (i = 0; i < 3; i++) {
        SDF_VU0_COPYVECTOR(destination + i, source + i);
    }
}

/* data holds two vector addresses: the ring head, then the input frame.
 * ring_mirror_pointer_epochs_probe (hooked==unhooked): GCSE preserves the
 * head-to-mirror copy; separate append/mirror lifetimes keep native allocation. */
void effAppendSurfaceGridVectors(EffSurfaceGridNode *work, const f32 (*source)[4]) {
    s32 index = work->field_18;
    const u128 *data = work->buffer;
    u128 *destination = (u128 *)data + index;
    s32 capacity;
    s32 i;

    for (i = 0; i < 3; i++) {
        SDF_VU0_COPYVECTOR(destination + i, source + i);
    }
    capacity = work->rows;
    work->field_18 = index + 3;
    if (work->field_18 == capacity) {
        u128 *mirror = (u128 *)data;
        data = (const u128 *)source;
        for (i = 0; i < 3; i++) {
            SDF_VU0_COPYVECTOR(mirror + i, data + i);
        }
        work->field_18 = 3;
    }
    if (work->field_14 < capacity - 3) {
        work->field_14 += 3;
    }
}

/* vu0 routine: interpolate three-vector ring frames with SDK spline and lerp operations. */
void effInterpolateSurfaceGridVectors(EffSurfaceGridNode *work, const f32 (*vectors)[4]) {
    f32 frames[4][3][4];
    f32 controlVectors[4][4];
    f32 output[3][4];
    s32 count = work->columns;
    s32 retainedVectors = work->field_14;
    s32 i;
    s32 j;
    f32 step;
    f32 t;

    if (count == 1 || retainedVectors == 0) {
        effAppendSurfaceGridVectors(work, vectors);
        return;
    }
    if (retainedVectors >= count * 6) {
        effCopyRingFrameVectors(work, (u128 *)frames[0], 2);
        effCopyRingFrameVectors(work, (u128 *)frames[1], 1);
        for (j = 0; j < 3; j++) {
            SDF_VU0_COPYVECTOR(frames[2][j], vectors[j]);
            SDF_VU0_COPYVECTOR(frames[3][j], vectors[j]);
        }
        step = 1.0f / count;
        t = 0.0f;
        for (i = 0; i < count; i++) {
            t += step;
            for (j = 0; j < 3; j++) {
                SDF_VU0_COPYVECTOR(controlVectors[0], frames[0][j]);
                SDF_VU0_COPYVECTOR(controlVectors[1], frames[1][j]);
                SDF_VU0_COPYVECTOR(controlVectors[2], frames[2][j]);
                SDF_VU0_COPYVECTOR(controlVectors[3], frames[3][j]);
                vuCatmullRomPoint(controlVectors, t);
                SDF_VU0_STOREVECTOR(output[j]);
            }
            effAppendSurfaceGridVectors(work, output);
        }
    } else {
        effCopyRingFrameVectors(work, (u128 *)controlVectors, 1);
        step = 1.0f / count;
        t = 0.0f;
        for (i = 0; i < count; i++) {
            t += step;
            for (j = 0; j < 3; j++) {
                SDF_VU0_LOADVECTOR(controlVectors[j]);
                SDF_VU0_LOADVECTOR_B(vectors[j]);
                VU0_LERP_VF10(t);
                SDF_VU0_STOREVECTOR(output[j]);
            }
            effAppendSurfaceGridVectors(work, output);
        }
    }
}

void effBeginMatrixVuDrawPacket(const Matrix4 *matrix) {
    struct sdfDrawTag_t *work = (struct sdfDrawTag_t *)sdfGrAllocPacketBuff(0x20);
    effCurrentRenderPacket = work;
    sdfDrawTagInit(work);
    VU0_LOAD_MATRIX(matrix);
    sdf3DPrimLinkTm(effCurrentRenderPacket, 0);
}

void effDrawSurfaceTrailHistory(EffSurfaceTrailHistory *history, u32 color) {
    s32 start[4];
    s32 length[4];
    s32 recent;
    s32 wrapped;
    s32 i;
    s32 remaining;
    sdf3DPrimPolyParm_t *draw;
    u32 *colors;
    sdfDrawTag_t *list;

    if ((color & 0xFF000000) == 0) {
        return;
    }
    if (history->activePointCount < 6) {
        return;
    }
    sdf3DPrimLinkMat(effCurrentRenderPacket, history->resourceHandle, 0);
    recent = history->activePointCount;
    start[0] = history->nextPointWriteIndex - recent;
    if (start[0] < 3) {
        wrapped = start[0] - 3;
        start[1] = 0;
        start[0] = wrapped + history->pointCapacity;
        length[0] = -wrapped;
        length[1] = recent - length[0] + 3;
    } else {
        length[0] = recent;
        length[1] = 0;
    }
    D_r1_004D1C20.vertexColor = history->colors;
    for (i = 0; i < 2; i++) {
        draw = &D_r1_004D1C20;
        draw->vertex = history->points + start[i];
        draw->color = color;
        draw->faceCount = 16;
        remaining = length[i];
        draw->pointCount = 15;
        while (remaining >= 15) {
            remaining -= 12;
            list = effCurrentRenderPacket;
            sdfDrawTagLink(list, effPkt3DPrimMakePoly(&D_r1_004D1C20));
            D_r1_004D1C20.vertex += 12;
            D_r1_004D1C20.vertexColor += 12;
        }
        if (remaining >= 6) {
            draw->faceCount = (remaining / 3) * 4 - 4;
            draw->pointCount = remaining;
            list = effCurrentRenderPacket;
            sdfDrawTagLink(list, effPkt3DPrimMakePoly(draw));
            draw->vertexColor += remaining;
        }
    }
    colors = history->endColors;
    D_r1_004D1C50.vertex = history->endPoints;
    colors[0] = D_r1_004D1C20.vertexColor[-2];
    D_r1_004D1C50.vertexColor = colors;
    colors[7] = colors[6] = colors[5] = colors[4] =
        colors[3] = colors[2] = colors[1] = D_r1_004D1C20.vertexColor[-3];
    D_r1_004D1C50.color = color;
    list = effCurrentRenderPacket;
    sdfDrawTagLink(list, effPkt3DPrimMakePoly(&D_r1_004D1C50));
}

void effSubmitIndexedRenderPacket(u32 index) {
    u8 *entry = D_r1_00403508[index];
    ((EffDrawEntry *)entry)->draw(entry, effCurrentRenderPacket);
    effCurrentRenderPacket = NULL;
}

INCLUDE_ASM(const s32, "code_0023E1D0", func_r1_00232638);

INCLUDE_ASM(const s32, "code_0023E1D0", effReleaseRenderResources);

INCLUDE_ASM(const s32, "code_0023E1D0", func_r1_00232C38);

INCLUDE_ASM(const s32, "code_0023E1D0", func_r1_00232E30);

void effResetRenderResourceFrame(EffQuadWork *work) {
    work->frame = 0;
}

INCLUDE_ASM(const s32, "code_0023E1D0", func_r1_00232F10);

INCLUDE_ASM(const s32, "code_0023E1D0", func_r1_00232F48);

INCLUDE_ASM(const s32, "code_0023E1D0", func_r1_00232F58);

void effSetRenderResourceColor(EffQuadWork *work, u32 color) {
    work->color = color;
}

void effSetRenderResourceScale(EffQuadWork *work, f32 scale) {
    work->scale = scale;
}

INCLUDE_ASM(const s32, "code_0023E1D0", func_r1_00232F80);

INCLUDE_ASM(const s32, "code_0023E1D0", func_r1_00233AA0);

INCLUDE_ASM(const s32, "code_0023E1D0", func_r1_00233AD8);

void effRandomizeParticleFields(EffClassWork *work) {
    u32 index = 0;
    u32 count = ((EffBillTimedHeader *)work->payload)->count;
    EffPointSetRow *entry = ((EffPointSetTable *)work->resource)->rows;

    if (count != 0) {
        do {
            entry->key = -1 - (dds3GetRandA((struct EffRandState *)effSharedRandomState) & 3);
            entry++;
            index++;
        } while (index < count);
    }
}

EffPointSetTable *effCreateAlphaRampPointSetRows(EffBillPointConfig *src) {
    u32 count = src->timed.count;
    EffPointSetTable *table;
    EffPointSetRow *row;
    u32 i;
    u32 alphaA;
    u32 alphaB;
    u32 alphaC;
    u32 lowA;
    u32 lowB;
    u32 lowC;
    s32 rampIn;
    s32 rampOut;

    table = sdfChipAllocDirect(count * sizeof(EffPointSetRow) + 4);
    table->rows = (EffPointSetRow *)(table + 1);
    if ((u32)src->layers < 3) {
        src->layers = 3;
    }
    lowA = src->colorA & 0xFFFFFF;
    lowB = src->colorB & 0xFFFFFF;
    lowC = src->colorC & 0xFFFFFF;
    alphaA = src->colorA >> 24;
    alphaB = src->colorB >> 24;
    alphaC = src->colorC >> 24;
    rampIn = (s32)(src->rangeFadeInEnd * (f32)(src->layers + 1));
    rampOut = (s32)(src->rangeFadeOutStart * (f32)(src->layers + 1));
    row = table->rows;
    for (i = 0; i < count; i++) {
        EffPointSet *set = effCreatePointSet5(src->layers);
        u32 n;
        u32 *rec;
        u32 j;

        row->set = set;
        n = set->rows / 5;
        rec = (u32 *)set->tail;
        for (j = 0; j < n; j++) {
            f32 ratio;

            if (j < rampIn) {
                ratio = (f32)j / (f32)rampIn;
            } else if (j <= rampOut) {
                ratio = 1.0f;
            } else {
                ratio = (f32)(n - j) / (f32)(n - rampOut);
            }
            rec[0] = lowC | ((u32)((f32)alphaC * ratio) << 24);
            rec[1] = lowB | ((u32)((f32)alphaB * ratio) << 24);
            rec[2] = lowA | ((u32)((f32)alphaA * ratio) << 24);
            rec[3] = rec[1];
            rec[4] = rec[0];
            rec += 5;
        }
        row->key = ~(i * 4);
        row->angle = 0.0f;
        row++;
    }
    return table;
}

void effFreeIndexedEntries(EffClassWork *work) {
    u32 index = 0;
    u32 count = ((EffBillTimedHeader *)work->payload)->count;
    EffPointSetTable *table = (EffPointSetTable *)work->resource;
    EffPointSetRow *entry = table->rows;

    if (count != 0) {
        do {
            effReleasePointSetAsset(entry->set);
            entry++;
            index++;
        } while (index < count);
    }
    sdfChipFreeDirect(table);
}

/* Advance radial five-point wave strips from the class orientation. */
void effUpdateRadialWavePointRows(EffClassWork *work) {
    f32 offsets[5];
    f32 widths[5];
    f32 direction[4];
    f32 view[4];
    f32 axis[4];
    EffBillPointConfig *config = work->payload;
    EffPointSetRow *entry = ((EffPointSetTable *)work->resource)->rows;
    u32 progress = config->timed.time.progress;
    s32 count;
    s32 remaining;
    s32 respawnDelay;
    f32 layers;
    f32 step;
    f32 waveStep;
    f32 amplitude;
    f32 bendScale;
    f32 phase;
    f32 speed;
    f32 speedJitter;

    if (progress < work->frame && progress != 0) {
        return;
    }
    count = config->timed.count;
    respawnDelay = config->respawnDelay;
    speed = config->speed;
    speedJitter = config->speedJitter;
    layers = (f32)(u32)config->layers;
    step = config->totalLength / layers;
    waveStep = ((f32)config->periods * 3.14159265f) / layers;
    amplitude = config->amplitude;
    if (step <= 0.0f) {
        return;
    }
    SDF_VU0_LOADVECTOR(aViewEye);
    SDF_VU0_LOADVECTOR_B(aViewTarget);
    SDF_VU0_SUBVECTOR();
    SDF_VU0_STOREVECTOR(view);
    SDF_VU0_LOADVECTOR(work->vectors.orientation);
    ToMatrix();
    SDF_VU0_LOADVECTOR(effWorldUpVector);
    SDF_VU0_APPLYMATRIX_XYZ();
    SDF_VU0_STOREVECTOR(direction);
    bendScale = 0.26179933547973633f;
    widths[0] = 0;
    widths[1] = config->edgeWidth;
    widths[2] = widths[1] + config->halfWidth;
    widths[3] = widths[2] + config->halfWidth;
    widths[4] = widths[3] + config->edgeWidth;
    if (count > 0) {
        remaining = count;
        for (; remaining != 0; remaining--, entry++) {
            if (entry->key == -1) {
                entry->color = 0xA0808080;
            } else {
                f32 r = dds3GetRandFloatA(effSharedRandomState);
                f32 bendStep = speed * (r * speedJitter + (1.0f - speedJitter));
                f32 bend;

                phase = entry->angle;
                bendStep /= layers;
                if (dds3GetRandA((struct EffRandState *)effSharedRandomState) & 1) {
                    bendStep = -bendStep;
                }
                bend = bendStep;
                if (entry->color & 0xFF000000) {
                    entry->color += 0xE0000000;
                } else {
                    entry->key = -1;
                    if (respawnDelay > 0) {
                        entry->key -= (u32)dds3GetRandA((struct EffRandState *)effSharedRandomState) % (u32)respawnDelay;
                    }
                    continue;
                }
                {
                    EffPointSet *set = entry->set;
                    f32 *vertices = (f32 *)set->buffer;
                    s32 rows = set->rows / 5;
                    f32 stepSquared;
                    f32 waveAngle;
                    f32 waveHeight;
                    f32 waveRadius;
                    f32 oldSlope;
                    f32 inner;
                    f32 width;
                    f32 newHeight;
                    f32 delta;
                    f32 segmentLength;
                    s32 j;
                    s32 k;

                    if (dds3GetRandA((struct EffRandState *)effSharedRandomState) & 1) {
                        amplitude = -amplitude;
                    }
                    stepSquared = step * step;
                    waveAngle = dds3GetRandFloatA(effSharedRandomState) * 3.14159265f;
                    waveRadius = amplitude * (dds3GetRandFloatA(effSharedRandomState) * 0.9f + (1.0f - 0.9f));
                    waveHeight = sdfSin(waveAngle) * waveRadius;
                    waveAngle += waveStep;
                    newHeight = sdfSin(waveAngle) * waveRadius;
                    delta = newHeight - waveHeight;
                    waveHeight = newHeight;
                    segmentLength = fsqrtf(delta * delta + stepSquared);
                    oldSlope = asinf(delta / segmentLength) + phase;
                    inner = config->halfWidth;
                    width = inner + config->edgeWidth;
                    SDF_VU0_LOADVECTOR(direction);
                    SDF_VU0_LOADVECTOR_B(view);
                    SDF_VU0_OUTERPRODUCT();
                    SDF_VU0_NORMALIZE();
                    SDF_VU0_COPYVECTOR_A_TO_C();
                    SDF_VU0_LOADVECTOR_B(direction);
                    SDF_VU0_OUTERPRODUCT();
                    SDF_VU0_STOREVECTOR(axis);
                    SDF_VU0_COPYVECTOR_C_TO_A();
                    SDF_VU0_COPYVECTOR_C_TO_B();
                    VU0_SCALE_VF(vf10, width);
                    SDF_VU0_STOREVECTOR(vertices);
                    SDF_VU0_NEGVECTOR_XYZ();
                    SDF_VU0_STOREVECTOR(vertices + 16);
                    VU0_SCALE_VF(vf11, inner);
                    SDF_VU0_STOREVECTOR_B(vertices + 4);
                    VU0_NEGATE_XYZ(vf11);
                    SDF_VU0_STOREVECTOR_B(vertices + 12);
                    VU0_STORE_VF(vf0, vertices + 8);
                    SDF_VU0_COPYVECTOR_C_TO_B();
                    VU0_SCALE_VF(vf11, waveHeight);
                    for (j = 0; j < 5; j++) {
                        SDF_VU0_LOADVECTOR(vertices + j * 4);
                        SDF_VU0_ADDVECTOR();
                        SDF_VU0_STOREVECTOR(vertices + j * 4);
                        offsets[j] = 0;
                    }
                    vertices += 20;
                    for (k = 1; k < rows; k++) {
                        f32 tilt = bendScale * (dds3GetRandFloatA(effSharedRandomState) * 0.75 + 0.25);
                        f32 slope;
                        f32 tangentScale;
                        f32 sine;

                        if (dds3GetRandA((struct EffRandState *)effSharedRandomState) & 1) {
                            bendScale = -bendScale;
                        }
                        waveAngle += waveStep;
                        if (waveAngle >= 3.14159265f) {
                            waveAngle -= 3.14159265f;
                            amplitude = -amplitude;
                            waveRadius = amplitude * (dds3GetRandFloatA(effSharedRandomState) * 0.9f + (1.0f - 0.9f));
                        }
                        newHeight = sdfSin(waveAngle) * waveRadius;
                        delta = newHeight - waveHeight;
                        segmentLength = fsqrtf(delta * delta + stepSquared);
                        slope = asinf(delta / segmentLength) + (tilt + bend);
                        SetRotateMatrixAxis(oldSlope, (const struct RwV3d *)axis);
                        slope += phase;
                        SDF_VU0_LOADVECTOR(direction);
                        SDF_VU0_APPLYMATRIX_XYZ();
                        delta = (slope - oldSlope) * 0.5f;
                        oldSlope = slope;
                        waveHeight = newHeight;
                        bend += bendStep;
                        sine = sdfSin(delta);
                        tangentScale = sine / sdfCos(delta);
                        SDF_VU0_COPYVECTOR_A_TO_C();
                        for (j = 0; j < 5; j++) {
                            f32 extra = widths[j] * tangentScale;
                            f32 length;
                            offsets[j] += extra;
                            SDF_VU0_COPYVECTOR_C_TO_B();
                            SDF_VU0_LOADVECTOR(vertices + j * 4 - 20);
                            length = segmentLength + offsets[j];
                            VU0_SCALE_VF(vf11, length);
                            SDF_VU0_ADDVECTOR();
                            SDF_VU0_STOREVECTOR(vertices + j * 4);
                            offsets[j] = extra;
                        }
                        vertices += 20;
                    }
                }
            }
            entry->key = (s32)((u32)entry->key + 1);
        }
    }
}

/* Per-frame update of a five-point-group class: tint every active set and draw it under the class transform. */
void effDrawTintedFivePointGroupRows(EffClassWork *work) {
    EffBillPointConfig *config = (EffBillPointConfig *)work->payload;
    EffBillTimedHeader *header = &config->timed;
    EffPointSetRow *row = ((EffPointSetTable *)work->resource)->rows;
    s32 frame = work->frame;
    s32 time = header->time.duration;
    s32 count;
    s32 i;
    f32 tint[4];
    u128 mtx[4];
    s32 color1[4];
    s32 color2[4];
    s32 rowColor[4];
    s32 blended[4];
    u32 packed;
    u32 unit;
    u32 second;

    if (time < frame && time != 0) {
        return;
    }
    count = header->count;
    second = effSampleColorAlphaTracks(&header->colorTrack, &header->alphaTrack, frame, time);
    color1[0] = work->color;
    unit = 0x3C000000;
    EE_MMI_RGBA_UNPACK(color1, unit);
    SDF_VU0_COPYVECTOR_A_TO_B();
    color2[0] = second;
    EE_MMI_RGBA_UNPACK(color2, unit);
    SDF_VU0_MULVECTOR();
    SDF_VU0_STOREVECTOR_NOMEM(tint);
    SDF_VU0_LOADIDENTITY();
    SDF_VU0_LOADVECTOR(effOnesVector);
    VU0_SCALAR_OP_CLOBBER(work->scale, "vmulx.xyzw vf10, vf10, vf2x");
    SDF_VU0_SCALE_XYZ();
    SDF_VU0_LOADVECTOR(work);
    SDF_VU0_VECTOR_W_SET_ONE();
    SDF_VU0_SET_ROW3();
    VU0_STORE_MATRIX(mtx);
    for (i = 0; i < count; i++, row++) {
        if (row->key > 0) {
            EffPointSet *set = row->set;

            rowColor[0] = row->color;
            EE_MMI_RGBA_UNPACK(rowColor, 1.0f / 128.0f);
            SDF_VU0_LOADVECTOR_B(tint);
            SDF_VU0_MULVECTOR();
            EE_MMI_RGBA_PACK_UNIT(packed, 128.0f);
            blended[0] = packed;
            set->color = blended[0];
            set->type = header->alphaTrack.surfaceIndex;
            set->flag = config->drawFlag;
            effDrawFivePointGroups(set, mtx);
        }
    }
}

void effResetRadialClassResourceFrames(EffClassWork *work) {
    u32 index = 0;
    u32 count = ((EffBillTimedHeader *)work->payload)->count;
    EffClassWork **entry = ((EffClassWorkList *)work->resource)->entries;

    if (count != 0) {
        do {
            effResetDispatchCounter(*entry);
            entry++;
            index++;
        } while (index < count);
    }
}

EffClassWorkList *effCreateRadialClassResourceEntries(EffBillPointConfig *config) {
    u32 count = config->timed.count;
    EffClassWorkList *allocation = sdfChipAllocDirect(count * 4 + 4);
    EffClassWork **entries = (EffClassWork **)(allocation + 1);
    EffBillPointConfig copy;
    u32 index;
    f32 step;
    f32 position;
    f32 offset;

    allocation->entries = entries;
    if ((u32)config->layers < 3) {
        config->layers = 3;
    }
    if (count == 0) {
        return allocation;
    }
    memcpy(&copy, config, 0x84);
    copy.timed.count = 1;
    copy.totalLength = config->totalLength;
    step = 6.2831852f / (f32)count;
    position = step * ((dds3GetRandFloatA(effSharedRandomState) - 0.5f) * 2.0f);
    for (index = 0; index < count; index++) {
        EffClassWork *resourceWork = effCreateClassResourceWork(1, &copy);
        u8 **resourceSlot = (u8 **)resourceWork->resource;

        *entries++ = resourceWork;
        *(f32 *)(*resourceSlot + 0xC) = position;
        offset = (dds3GetRandFloatA(effSharedRandomState) - 0.5f) * 2.0f;
        position += step + step * offset * 0.25f;
    }
    return allocation;
}

void effDestroyRadialClassResourceEntries(EffClassWork *work) {
    u8 *config = work->payload;
    EffClassWorkList *state = (EffClassWorkList *)work->resource;
    u32 count = ((EffBillTimedHeader *)config)->count;
    u32 index = 0;
    EffClassWork **entry = state->entries;

    if (count != 0) {
        do {
            effDestroyClassResourceWork(*entry);
            entry++;
        } while (++index < count);
    }
    sdfChipFreeDirect(state);
}

void effAdvanceRadialClassResourceFrames(EffClassWork *work) {
    u32 index = 0;
    u32 count = ((EffBillTimedHeader *)work->payload)->count;
    EffClassWork **entry = ((EffClassWorkList *)work->resource)->entries;

    if (count != 0) {
        do {
            effAdvanceClassResourceFrame(*entry);
            entry++;
            index++;
        } while (index < count);
    }
}

/* vu0 routine: packed color blend and SDK vector copies. */
void effUpdateRadialClassInstances(EffClassWork *work) {
    EffBillRadialConfig *config = work->payload;
    s32 frame = work->frame;
    s32 progress = config->point.timed.time.duration;
    EffClassWork **entries = ((EffClassWorkList *)work->resource)->entries;
    f32 position[4];
    f32 orientation[4];
    u32 unit;
    u32 second;
    u32 color;
    u32 count;
    u32 i;
    f32 radius;
    f32 scale;

    if (progress < frame && progress != 0) {
        return;
    }
    count = config->point.timed.count;
    radius = config->radius;
    second = effSampleColorAlphaTracks(&config->point.timed.colorTrack, &config->point.timed.alphaTrack, frame, progress);
    unit = 0x3C000000;
    eeUnpackRGBAWord(work->color, unit);
    SDF_VU0_COPYVECTOR_A_TO_B();
    eeUnpackRGBAWord(second, unit);
    SDF_VU0_MULVECTOR();
    color = eePackRGBAWord(128.0f);
    scale = work->scale;
    SDF_VU0_COPYVECTOR(position, work->vectors.position);
    SDF_VU0_COPYVECTOR(orientation, work->vectors.orientation);
    for (i = 0; i < count; i++, entries++) {
        EffClassWork *entry = *entries;
        EffPointSetTable *points = (EffPointSetTable *)entry->resource;
        f32 angle = points->rows->angle;

        position[0] = work->vectors.position[0] - sdfSin(angle) * radius;
        position[1] = work->vectors.position[1] - sdfCos(angle) * radius;
        effCopyClassResourcePosition(*entries, position);
        effCopyClassResourceOrientation(*entries, orientation);
        effSetClassResourceMatrixComponent(*entries, scale);
        effSetClassResourceColor(*entries, color);
        effDrawClassResourceWork(*entries);
    }
}

void effSeedBillScaleRange(EffClassWork *work) {
    EffBillRangeConfig *config = work->payload;
    EffScaleRange *range = (EffScaleRange *)work->resource;
    s32 steps = config->point.timed.time.duration;
    EffScaleRangeEntry *entry = range->entries;
    f32 start = config->startBase * (dds3GetRandFloatA(effSharedRandomState) * config->startRand + (1.0f - config->startRand));
    u32 index;
    u32 count;

    if (steps > 0) {
        f32 end = config->endBase * (dds3GetRandFloatA(effSharedRandomState) * config->endRand + (1.0f - config->endRand));
        range->start = start;
        range->delta = (end - start) / (f32)steps;
    } else {
        range->start = start;
        range->delta = 0.0f;
    }
    count = config->point.timed.count;
    index = 0;
    if (count != 0) {
        do {
            index++;
            entry->negativeSeed = -1 - (dds3GetRandA((struct EffRandState *)effSharedRandomState) & 7);
            entry++;
        } while (index < count);
    }
}

EffScaleRange *effCreateRetainedPointSetColorRows(EffBillPointConfig *src) {
    u32 count = src->timed.count;
    struct sdfMemHandle_t *allocation;
    EffScaleRange *table;
    EffScaleRangeEntry *row;
    u32 i;
    u32 alphaA;
    u32 alphaB;
    u32 alphaC;
    u32 lowA;
    u32 lowB;
    u32 lowC;
    s32 rampIn;
    s32 rampOut;

    allocation = sdfMemAllocDirect(count * sizeof(EffScaleRangeEntry) + sizeof(EffScaleRange));
    table = (EffScaleRange *)sdfMemLock((struct sdfMemHandle_t *)((u32)allocation));
    table->allocation = allocation;
    table->entries = (EffScaleRangeEntry *)(table + 1);
    if ((u32)src->layers < 3) {
        src->layers = 3;
    }
    lowA = src->colorA & 0xFFFFFF;
    lowB = src->colorB & 0xFFFFFF;
    lowC = src->colorC & 0xFFFFFF;
    alphaA = src->colorA >> 24;
    alphaB = src->colorB >> 24;
    alphaC = src->colorC >> 24;
    rampIn = (s32)(src->rangeFadeInEnd * (f32)(src->layers + 1));
    rampOut = (s32)(src->rangeFadeOutStart * (f32)(src->layers + 1));
    row = table->entries;
    for (i = 0; i < count; i++) {
        EffPointSet *set = effCreatePointSet5(src->layers);
        u32 n;
        u32 *rec;
        u32 j;

        row->set = set;
        n = set->rows / 5;
        rec = (u32 *)set->tail;
        for (j = 0; j < n; j++) {
            f32 ratio;
            if (j < rampIn) {
                ratio = (f32)j / (f32)rampIn;
            } else if (j <= rampOut) {
                ratio = 1.0f;
            } else {
                ratio = (f32)(n - j) / (f32)(n - rampOut);
            }
            rec[0] = lowC | ((u32)((f32)alphaC * ratio) << 24);
            rec[1] = lowB | ((u32)((f32)alphaB * ratio) << 24);
            rec[2] = lowA | ((u32)((f32)alphaA * ratio) << 24);
            rec[3] = rec[1];
            rec[4] = rec[0];
            rec += 5;
        }
        row->negativeSeed = ~(i * 4);
        row++;
    }
    return table;
}

void effReleaseBillPointEntries(EffClassWork *work) {
    EffBillTimedHeader *config = work->payload;
    EffScaleRange *range = (EffScaleRange *)work->resource;
    u32 count = config->count;
    u32 index = 0;
    EffScaleRangeEntry *entry = range->entries;

    if (count != 0) {
        do {
            effReleasePointSetAsset(entry->set);
            entry++;
        } while (++index < count);
    }
    sdfMemFreeDirect(range->allocation);
}

/* Advance orbiting entries and rebuild their camera-facing, wavy point spans. */
void effUpdateOrbitalPointSpanWaves(EffClassWork *work) {
    f32 offsets[5];
    f32 widths[5];
    f32 direction[4];
    f32 radial[4];
    f32 tangent[4];
    f32 view[4];
    f32 position[4];
    f32 previous[4];
    f32 rotated[4];
    EffBillRangeConfig *config = work->payload;
    EffScaleRange *range = (EffScaleRange *)work->resource;
    EffScaleRangeEntry *entry = (EffScaleRangeEntry *)range->entries;
    u32 progress = config->point.timed.time.progress;
    s32 count;
    u8 useHeight;
    s32 respawnDelay;
    s32 remaining;
    f32 step;
    f32 layers;
    f32 phase;
    f32 waveStep;
    f32 amplitude;
    f32 bendScale;
    f32 acceleration;
    f32 bendStep;
    f32 radius;
    if (progress < work->frame && progress != 0) {
        return;
    }
    count = config->point.timed.count;
    respawnDelay = config->point.respawnDelay;
    layers = (f32)(u32)config->point.layers;
    step = config->point.totalLength / layers;
    waveStep = ((f32)config->point.periods * 3.14159265f) / layers;
    amplitude = config->point.amplitude;
    acceleration = config->angularAcceleration;
    useHeight = config->tilted;
    if (step <= 0.0f) {
        return;
    }
    SDF_VU0_LOADVECTOR(aViewEye);
    SDF_VU0_LOADVECTOR_B(aViewTarget);
    SDF_VU0_SUBVECTOR();
    SDF_VU0_STOREVECTOR(view);
    widths[0] = 0;
    widths[1] = config->point.edgeWidth;
    widths[2] = widths[1] + config->point.halfWidth;
    widths[3] = widths[2] + config->point.halfWidth;
    widths[4] = widths[3] + config->point.edgeWidth;
    position[3] = direction[3] = 1.0f;
    bendStep = -((step / 10.0f) * (5.0f * (3.14159265f / 180.0f)));
    bendScale = (20.0f * (3.14159265f / 180.0f));
    radius = range->start;
    range->start = radius + range->delta;
    if (count > 0) {
    remaining = count;
    for (; remaining != 0; remaining--, entry++) {
        if (entry->negativeSeed == -1) {
            f32 r = dds3GetRandFloatA(effSharedRandomState);
            f32 jitter = config->angularRand;
            f32 baseSpeed = config->angularBase;
            entry->phase = 0;
            entry->angularRate = baseSpeed * (r * jitter + (1.0f - jitter));
            if (!useHeight) {
                entry->yaw = (dds3GetRandFloatA(effSharedRandomState) - 0.5f) * 2.0f * 3.14159265f;
                entry->height = 0;
            } else {
                entry->yaw = (dds3GetRandFloatA(effSharedRandomState) - 0.5f) * 2.0f * (45.0f * (3.14159265f / 180.0f));
                entry->height = -config->heightScale * dds3GetRandFloatA(effSharedRandomState);
            }
            entry->pitch = (dds3GetRandFloatA(effSharedRandomState) - 0.5f) * 2.0f * 3.14159265f;
            SetRotateMatrixX(entry->yaw);
            SetRotateMatrixY_B(entry->pitch);
            MultMatrixRevNoArg();
            phase = entry->phase;
            position[0] = sdfCos(phase) * radius;
            position[1] = 0;
            position[2] = sdfSin(phase) * radius;
            SDF_VU0_LOADVECTOR(position);
            SDF_VU0_APPLYMATRIX_XYZ();
            SDF_VU0_STOREVECTOR(position);
            entry->position[0] = position[0];
            entry->color = 0xA0808080;
            entry->position[1] = position[1];
            entry->position[2] = position[2];
        } else {
            if (entry->color & 0xFF000000) {
                entry->color += 0xE0000000;
            } else {
                entry->negativeSeed = -1;
                if (respawnDelay > 0) {
                    entry->negativeSeed -= (u32)dds3GetRandA((struct EffRandState *)effSharedRandomState) % (u32)respawnDelay;
                }
                continue;
            }
            {
                EffPointSet *set = entry->set;
                f32 *vertices = (f32 *)set->buffer;
                s32 rows = set->rows / 5;
                f32 stepSquared;
                f32 waveAngle;
                f32 waveHeight;
                f32 waveRadius;
                f32 oldSlope;
                f32 inner;
                f32 edgeWidth;
                f32 width;
                f32 curve;
                f32 curveSample;
                f32 newHeight;
                f32 delta;
                f32 segmentLength;
                s32 j;
                s32 k;
                if (dds3GetRandA((struct EffRandState *)effSharedRandomState) & 1) {
                    amplitude = -amplitude;
                }
                stepSquared = step * step;
                waveAngle = dds3GetRandFloatA(effSharedRandomState) * 3.14159265f;
                waveRadius = amplitude * (dds3GetRandFloatA(effSharedRandomState) * 0.95f + (1.0f - 0.95f));
                waveHeight = sdfSin(waveAngle) * waveRadius;
                waveAngle += waveStep;
                newHeight = sdfSin(waveAngle) * waveRadius;
                delta = newHeight - waveHeight;
                waveHeight = newHeight;
                segmentLength = fsqrtf(delta * delta + stepSquared);
                oldSlope = asinf(delta / segmentLength);
                inner = config->point.halfWidth;
                edgeWidth = config->point.edgeWidth;
                curveSample = dds3GetRandFloatA(effSharedRandomState);
                width = inner + edgeWidth;
                phase = (entry->angularRate + acceleration * (f32)(entry->negativeSeed + 1) * 0.5f) * (f32)(entry->negativeSeed + 1);
                phase = phase + entry->phase;
                curve = bendStep * (curveSample * 0.7f + (1.0f - 0.7f));
                SetRotateMatrixX(entry->yaw);
                SetRotateMatrixY_B(entry->pitch);
                MultMatrixRevNoArg();
                position[0] = sdfCos(phase) * radius;
                position[1] = 0;
                position[2] = sdfSin(phase) * radius;
                SDF_VU0_LOADVECTOR(position);
                SDF_VU0_APPLYMATRIX_XYZ();
                SDF_VU0_STOREVECTOR(position);
                SDF_VU0_STOREVECTOR(previous);
                SDF_VU0_NORMALIZE();
                SDF_VU0_STOREVECTOR(radial);
                direction[0] = previous[0] - entry->position[0];
                direction[1] = previous[1] - entry->position[1];
                direction[2] = previous[2] - entry->position[2];
                SDF_VU0_LOADVECTOR(direction);
                SDF_VU0_NORMALIZE();
                SDF_VU0_COPYVECTOR_A_TO_C();
                SDF_VU0_LOADVECTOR_B(radial);
                SDF_VU0_STOREVECTOR(direction);
                SDF_VU0_OUTERPRODUCT();
                SDF_VU0_STOREVECTOR(tangent);
                SDF_VU0_LOADVECTOR_B(view);
                SDF_VU0_COPYVECTOR_C_TO_A();
                SDF_VU0_OUTERPRODUCT();
                SDF_VU0_NORMALIZE();
                SDF_VU0_COPYVECTOR_A_TO_C();
                SDF_VU0_COPYVECTOR_A_TO_B();
                VU0_SCALE_VF(vf10, width);
                SDF_VU0_STOREVECTOR(vertices);
                SDF_VU0_NEGVECTOR_XYZ();
                SDF_VU0_STOREVECTOR(vertices + 16);
                VU0_SCALE_VF(vf11, inner);
                SDF_VU0_STOREVECTOR_B(vertices + 4);
                VU0_NEGATE_XYZ(vf11);
                SDF_VU0_STOREVECTOR_B(vertices + 12);
                VU0_STORE_VF(vf0, vertices + 8);
                position[1] += entry->height;
                SDF_VU0_LOADVECTOR_B(position);
                for (j = 0; j < 5; j++) {
                    SDF_VU0_LOADVECTOR(vertices + j * 4);
                    SDF_VU0_ADDVECTOR();
                    SDF_VU0_STOREVECTOR(vertices + j * 4);
                    offsets[j] = 0;
                }
                entry->position[0] = previous[0];
                entry->position[1] = previous[1];
                entry->position[2] = previous[2];
                vertices += 20;
                for (k = 1; k < rows; k++) {
                    f32 tilt = bendScale * (dds3GetRandFloatA(effSharedRandomState) * 0.75 + 0.25);
                    f32 slope;
                    f32 distance;
                    f32 tangentScale;
                    if (dds3GetRandA((struct EffRandState *)effSharedRandomState) & 1) {
                        bendScale = -bendScale;
                    }
                    waveAngle += waveStep;
                    if (waveAngle >= 3.14159265f) {
                        waveAngle -= 3.14159265f;
                        amplitude = -amplitude;
                        waveRadius = amplitude * (dds3GetRandFloatA(effSharedRandomState) * 0.95f + (1.0f - 0.95f));
                        curve = bendStep * (dds3GetRandFloatA(effSharedRandomState) * 0.7f + (1.0f - 0.7f));
                    }
                    newHeight = sdfSin(waveAngle) * waveRadius;
                    delta = newHeight - waveHeight;
                    segmentLength = fsqrtf(delta * delta + stepSquared);
                    slope = asinf(delta / segmentLength) + tilt;
                    SetRotateMatrixAxis(oldSlope, (const struct RwV3d *)radial);
                    SDF_VU0_LOADVECTOR(direction);
                    SDF_VU0_APPLYMATRIX_XYZ();
                    SDF_VU0_STOREVECTOR(rotated);
                    SetRotateMatrixAxis(curve, (const struct RwV3d *)tangent);
                    SDF_VU0_LOADVECTOR(direction);
                    SDF_VU0_APPLYMATRIX_XYZ();
                    SDF_VU0_STOREVECTOR(direction);
                    SDF_VU0_LOADVECTOR(previous);
                    SDF_VU0_COPYVECTOR_A_TO_B();
                    SDF_VU0_APPLYMATRIX_XYZ();
                    SDF_VU0_STOREVECTOR(previous);
                    SDF_VU0_SUBVECTOR();
                    SDF_VU0_GETLENGTH(distance);
                    tangentScale = atanf((slope - oldSlope) * 0.5f);
                    oldSlope = slope;
                    waveHeight = newHeight;
                    SDF_VU0_LOADVECTOR_C(rotated);
                    for (j = 0; j < 5; j++) {
                        f32 extra = widths[j] * tangentScale;
                        f32 length;
                        offsets[j] += extra;
                        SDF_VU0_COPYVECTOR_C_TO_B();
                        SDF_VU0_LOADVECTOR(vertices + j * 4 - 20);
                        length = distance + offsets[j];
                        VU0_SCALE_VF(vf11, length);
                        SDF_VU0_ADDVECTOR();
                        SDF_VU0_STOREVECTOR(vertices + j * 4);
                        offsets[j] = extra;
                    }
                    vertices += 20;
                }
            }
        }
        entry->negativeSeed++;
    }
    }
}

/* vu0 routine: tint point sets and draw under the class quaternion transform.
 * Retail color homes +0x50/+0x60/+0x70/+0x80 are 16-byte SDK-sized locals. */
void effDrawRetainedPointSetRows(EffClassWork *work) {
    EffBillRangeConfig *config = work->payload;
    s32 frame = work->frame;
    s32 duration = config->point.timed.time.duration;
    EffScaleRangeEntry *row = ((EffScaleRange *)work->resource)->entries;
    s32 count;
    f32 colorFactor[4];
    Matrix4 matrix;
    s32 classColor[4];
    s32 fadeColor[4];
    s32 entryColor[4];
    s32 blended[4];
    u32 fade;
    u32 packed;
    u32 unit;

    if (duration < frame && duration != 0) {
        return;
    }
    count = config->point.timed.count;
    fade = effSampleColorAlphaTracks(&config->point.timed.colorTrack,
        &config->point.timed.alphaTrack, frame, duration);
    classColor[0] = work->color;
    unit = 0x3C000000;
    EE_MMI_RGBA_UNPACK(classColor, unit);
    SDF_VU0_COPYVECTOR_A_TO_B();
    fadeColor[0] = fade;
    EE_MMI_RGBA_UNPACK(fadeColor, unit);
    SDF_VU0_MULVECTOR();
    SDF_VU0_STOREVECTOR_NOMEM(colorFactor);
    SDF_VU0_LOADVECTOR(work->vectors.orientation);
    ToMatrix();
    SDF_VU0_LOADVECTOR(effOnesVector);
    VU0_SCALAR_OP_CLOBBER(work->scale, "vmulx.xyzw vf10, vf10, vf2x");
    SDF_VU0_SCALE_XYZ();
    SDF_VU0_LOADVECTOR(work->vectors.position);
    SDF_VU0_VECTOR_W_SET_ONE();
    SDF_VU0_SET_ROW3();
    VU0_STORE_MATRIX(&matrix);
    if (count > 0) {
        s32 remaining = count;
        do {
            if (row->negativeSeed > 0) {
                EffPointSet *set = row->set;
                entryColor[0] = row->color;
                EE_MMI_RGBA_UNPACK(entryColor, 1.0f / 128.0f);
                SDF_VU0_LOADVECTOR_B(colorFactor);
                SDF_VU0_MULVECTOR();
                EE_MMI_RGBA_PACK_UNIT(packed, 128.0f);
                blended[0] = packed;
                set->color = blended[0];
                set->type = config->point.timed.alphaTrack.surfaceIndex;
                set->flag = config->point.drawFlag;
                effDrawFivePointGroups(set, &matrix);
            }
            remaining--;
            row++;
        } while (remaining != 0);
    }
}

EffClassWork *effCreateClassResourceWork(u16 kind, void *source) {
    u32 headerSize = 0x40;
    u32 size = effClassResourceWorkOperations[kind].payloadSize;
    EffClassWork *effect = sdfChipAllocDirect(size + headerSize);
    effect->payload = (u8 *)effect + headerSize;
    effect->color = 0x80808080;
    effect->scale = 1.0f;
    effect->frame = 0;
    effect->kind = kind;
    VU0_STORE_VF_UNCLOBBERED($vf0, effect);
    VU0_STORE_VF_UNCLOBBERED($vf0, effect->vectors.orientation);
    memcpy(effect->payload, source, size);
    effect->resource = effClassResourceWorkOperations[kind].createResource(source);
    effClassResourceWorkOperations[kind].initialize(effect);
    return effect;
}

EffClassWork *effCreateClassResourceFromFile(FileJobPayload *request) {
    void *source;

    source = fileResolvePrimaryBuffer(request);
    return effCreateClassResourceWork(request->option, source);
}

void effDestroyClassResourceWork(EffClassWork *work) {
    effClassResourceWorkOperations[work->kind].destroyResource(work);
    sdfChipFreeDirect(work);
}

EffClassWork *effCloneClassResourceWork(EffClassWork *work) {
    return effCreateClassResourceWork((u16)work->kind, work->payload);
}

void effResetDispatchCounter(EffClassWork *work) {
    effClassResourceWorkOperations[work->kind].initialize(work);
    work->frame = 0;
}

void effAdvanceClassResourceFrame(EffClassWork *work) {
    effClassResourceWorkOperations[work->kind].update(work);
    work->frame = work->frame + 1;
}

void effDrawClassResourceWork(EffClassWork *work) {
    effClassResourceWorkOperations[work->kind].draw(work);
}

INCLUDE_ASM(const s32, "code_0023E1D0", func_r1_00236618);

void effCopyClassResourcePosition(void *dst, void *src) {
    SDF_VU0_COPYVECTOR(dst, src);
}

void effPCPBossBaelSetPosition(effPCPBossBael_t *hData, const void *pPos)
{
    SDF_VU0_COPYVECTOR(&hData->param, pPos);
}

void effSetClassResourceColor(EffClassWork *work, u32 color) {
    work->color = color;
}

void effSetClassResourceMatrixComponent(EffClassWork *work, float value) {
    work->scale = value;
}

EffPointSet *effCreatePointSet5(s32 count) {
    s32 rows = count * 5 + 5;
    s32 size = rows * 20;
    struct sdfMemHandle_t *base;
    u8 *data;
    EffPointSet *set;

    size = ((size >> 4) + ((size & 0xF) != 0)) << 4;
    base = sdfMemAllocDirect(size + 0x20);
    data = (u8 *)sdfMemLock(base);
    set = (EffPointSet *)(data + size);
    set->buffer = data;
    data += rows * 16;
    set->color = 0x80808080;
    set->type = 2;
    set->rows = rows;
    set->allocation = base;
    set->tail = data;
    set->flag = 0;
    set->handle = sdfMatNew();
    sdfMatSetConstant(set->handle, 1.0f);
    memset(D_r1_004D1CB0, 0, 0x2C);
    D_r1_004D1CB0[0].attr = 0x4000;
    return set;
}

/* Queue the draw asset for release and return the backing allocation. */
void effReleasePointSetAsset(EffPointSet *set) {
    sdfMatFreeDelayed(set->handle);
    sdfMemFreeDirect(set->allocation);
}

void effDrawFivePointGroups(EffPointSet *set, Matrix4 *matrix) {
    void *list;
    EffGsPacket *packet;
    s32 remaining;

    if (set->color & 0xFF000000) {
        list = (void *)sdfGrAllocPacketBuff(0x20);
        sdfDrawTagInit(list);
        if (matrix == NULL) {
            SDF_VU0_LOADIDENTITY();
        } else {
            VU0_LOAD_MATRIX(matrix);
        }
        sdf3DPrimLinkTm(list, 0);
        sdf3DPrimLinkMat(list, set->handle, 0);
        if (set->flag == 0) {
            packet = (EffGsPacket *)sdfGrAllocPacketBuff(0x30);
            packet->dmaTag = 2;
            packet->vifTag = (((u64)0x50000002 << 16) | 0x1000) << 16;
            packet->gifTag = ((u64)0x10000000 << 32) | 0x8001;
            packet->registerList = 0xE;
            packet->registerValue = 0x31801;
            packet->registerAddress = 0x47;
            sdfDrawTagLink(list, (u32)packet);
        }
        remaining = set->rows;
        D_r1_004D1CB0[0].vertexColor = (u32 *)set->tail;
        D_r1_004D1CB0[0].vertex = (u128 *)set->buffer;
        D_r1_004D1CB0[0].color = set->color;
        D_r1_004D1CB0[0].faceCount = 0x10;
        D_r1_004D1CB0[0].pointCount = 0xF;
        D_r1_004D1CB0[0].faceIndex = D_r1_004035D0;
        while (remaining >= 0xF) {
            remaining -= 0xA;
            sdfDrawTagLink(list, effPkt3DPrimMakePoly(D_r1_004D1CB0));
            D_r1_004D1CB0[0].vertex += 0xA;
            D_r1_004D1CB0[0].vertexColor += 0xA;
        }
        if (remaining >= 0xA) {
            D_r1_004D1CB0[0].faceCount = 8;
            D_r1_004D1CB0[0].pointCount = remaining;
            sdfDrawTagLink(list, effPkt3DPrimMakePoly(D_r1_004D1CB0));
        }
        if (set->flag == 0) {
            packet = (EffGsPacket *)sdfGrAllocPacketBuff(0x30);
            packet->dmaTag = 2;
            packet->vifTag = (((u64)0x50000002 << 16) | 0x1000) << 16;
            packet->gifTag = ((u64)0x10000000 << 32) | 0x8001;
            packet->registerList = 0xE;
            packet->registerValue = 0x51801;
            packet->registerAddress = 0x47;
            sdfDrawTagLink(list, (u32)packet);
        }
        D_r1_00403610[set->type]->regFunc(D_r1_00403610[set->type], list);
    }
}

EffectStripNode *effCreateStripNode(u32 percent) {
    EffectStripNode *node = sdfChipAllocZeroDirect(sizeof(EffectStripNode));
    node->percent = percent;
    node->color = 0x80808080;
    node->opacity = 1.0f;
    node->active = 0;
    node->trackSet = effCreateTrackSetWithSharedReferences(percent * 4, 2, 0);
    node->resource = dds3BillboardGeneral(0);
    node->count = 1;
    return node;
}

EffectStripNode *effCreateStripNodeFromGrid(FileKeyBlock *grid) {
    u32 n;
    s32 w = grid->emissionDuration;
    /* The strip capacity uses the same signed product as 002A5A58. */
    n = (w ? w : (s32)grid->spawnRate) * (w ? (s32)grid->spawnRate : grid->length);
    return effCreateStripNode(n > 100 ? 100 : n);
}

EffectStripNode *effFileResourceReferenceReplace(FileJobPayload *work) {
    u8 *source = fileResolvePrimaryBuffer(work);
    FileKeyBlock *params = (FileKeyBlock *)(source + 0x20);
    EffectStripNode *node = effCreateStripNodeFromGrid(params);

    memcpy(&node->copiedHeader, source, sizeof(node->copiedHeader));
    effReplaceFileResourceRef(node, work->option, params);
    return node;
}

void effReleaseModelResources(EffectStripNode *work) {
    if (work->resource != NULL) {
        dds3BillboardDelete(work->resource);
    }
    if (work->trackSet != NULL) {
        effReleaseResourceRefs(work->trackSet);
    }
    if (work->active != 0) {
        effPCPHelperBEDGetModel(work->active);
    }
    sdfChipFreeDirect(work);
}

EffectStripNode *effCloneStripResourceFromOwner(EffectStripNode *work) {
    FileKeyBlock *params = (FileKeyBlock *)work->active->data1;
    EffectStripNode *node = effCreateStripNodeFromGrid(params);
    memcpy(&node->copiedHeader, params, sizeof(node->copiedHeader));
    effReplaceFileResourceRef(node, work->active->type, params);
    return node;
}

void effReplaceFileResourceRef(EffectStripNode *node, u32 entryId, const void *resource) {
    if (node->active != 0) {
        effPCPHelperBEDGetModel(node->active);
    }
    node->active = fileAllocateGridRecordSlots((u16)entryId, node->percent, resource);
}

void effClearStripRecordReferences(EffectStripNode *node) {
    if (node->active != 0) {
        fileClearRecordReferences(node->active);
        return;
    }
}

void effAcquireStripRecord(EffectStripNode *node) {
    if (node->active != 0) {
        fileAcquireRecord(node->active);
        return;
    }
}

void effDrawEffectStrip(EffectStripNode *node) {
    Matrix4 matrix;
    f32 nodeTint[4];
    f32 tint[4];
    u128 translation[1];
    u128 rotation[1];
    f32 view[4];
    f32 rightAxis[4];
    f32 viewUp[4];
    f32 right[4];
    f32 up[4];
    f32 position[4];
    f32 innerTint[4];
    f32 outerTint[4];
    u32 packedInnerColor;
    u32 packedOuterColor;
    u32 childColor;
    FileKeyBlock *info;
    EffTrackSet *output;
    FileSlot *entry;
    f32 (*positions)[4];
    f32 (*quad)[4];
    u32 *colors;
    u32 *quadColors;
    struct billHandle_t *billboard;
    s32 count;
    s32 remaining;
    u32 hasTransform;
    s32 i;
    f32 opacity;
    f32 billSize;
    f32 crossSize;
    f32 longSize;
    f32 sideSize;
    f32 scale;
    f32 cross;
    f32 billExtent;
    f32 along;
    f32 side;

    info = (FileKeyBlock *)node->active->data0;
    eeUnpackRGBAWordFloat(node->color, 1.0f / 128.0f);
    SDF_VU0_STOREVECTOR_NOMEM(nodeTint);
    eeUnpackRGBAWordFloat(node->copiedHeader.tint, 1.0f / 128.0f);
    SDF_VU0_LOADVECTOR_B(nodeTint);
    SDF_VU0_MULVECTOR();
    SDF_VU0_STOREVECTOR_NOMEM(tint);
    hasTransform = node->active->flags & 1;
    if (!hasTransform) {
        EE_MMI_UNIT_MATRIX(&matrix);
    } else {
        fileReadRecordSecondVector(node->active, rotation);
        fileReadVectorPtr20(node->active, translation);
        SDF_VU0_LOADVECTOR(rotation);
        ToMatrix();
        SDF_VU0_LOADVECTOR(translation);
        SDF_VU0_VECTOR_W_SET_ONE();
        SDF_VU0_SET_ROW3();
        VU0_STORE_MATRIX(&matrix);
    }
    billboard = node->resource;
    opacity = node->opacity;
    billSize = node->copiedHeader.billSize;
    dds3BillboardSetMode(billboard, (s16)info->alphaTrack.surfaceIndex);
    SDF_VU0_LOADVECTOR(aViewTarget);
    SDF_VU0_LOADVECTOR_B(aViewEye);
    SDF_VU0_SUBVECTOR();
    SDF_VU0_NORMALIZE();
    SDF_VU0_STOREVECTOR_NOMEM(view);
    SDF_VU0_LOADVECTOR_B(aViewDown);
    SDF_VU0_STOREVECTOR_B_NOMEM(up);
    SDF_VU0_OUTERPRODUCT();
    SDF_VU0_STOREVECTOR_NOMEM(right);
    output = node->trackSet;
    entry = node->active->slots;
    count = node->percent;
    positions = (f32 (*)[4])output->buffer;
    colors = (u32 *)output->tail;
    if (info->length <= 0 || count == 0) {
        return;
    }
    longSize = node->copiedHeader.longSize;
    sideSize = node->copiedHeader.sideSize;
    crossSize = node->copiedHeader.crossSize;
    eeUnpackRGBAWordFloat(node->copiedHeader.innerColor, 1.0f / 128.0f);
    SDF_VU0_STOREVECTOR_NOMEM(innerTint);
    eeUnpackRGBAWordFloat(node->copiedHeader.outerColor, 1.0f / 128.0f);
    SDF_VU0_STOREVECTOR_NOMEM(outerTint);
    if (count > 0) {
        remaining = count;
        do {
            if ((u32)(FILE_SLOT_FRAME(entry) + 2) < 2) {
                quad = positions;
                for (i = 0; i < 4; i++, quad += 4) {
                    VU0_STORE_VF(vf0, quad[0]);
                    VU0_STORE_VF(vf0, quad[1]);
                    VU0_STORE_VF(vf0, quad[2]);
                    VU0_STORE_VF(vf0, quad[3]);
                }
            } else if (FILE_SLOT_FRAME(entry) > 0) {
                scale = entry->scale * opacity;
                if (hasTransform) {
                    VU0_LOAD_MATRIX(&matrix);
                    SDF_VU0_LOADVECTOR(entry->pos);
                    SDF_VU0_APPLYMATRIX_XYZ1();
                    SDF_VU0_STOREVECTOR(position);
                } else {
                    SDF_VU0_COPYVECTOR(position, entry->pos);
                }
                eeUnpackRGBAWordFloat(entry->color, 1.0f / 128.0f);
                SDF_VU0_LOADVECTOR_B(tint);
                SDF_VU0_MULVECTOR();
                childColor = eePackRGBAWord(128.0f);
                cross = crossSize * scale;
                billExtent = billSize * scale;
                dds3BillboardSetRGBA(billboard, childColor);
                dds3BillboardSetScale(billboard, billExtent, billExtent);
                dds3BillboardSetRotate(billboard, entry->angle);
                effCopyVector(billboard->pos, position);
                dds3BillboardDraw(billboard);
                along = longSize * scale * 1.5f;
                side = sideSize * scale * 1.5f;
                SetRotateMatrixAxis(entry->angle, (const RwV3d *)view);
                SDF_VU0_LOADVECTOR(right);
                SDF_VU0_APPLYMATRIX_XYZ();
                SDF_VU0_STOREVECTOR_NOMEM(rightAxis);
                SDF_VU0_LOADVECTOR(up);
                SDF_VU0_APPLYMATRIX_XYZ();
                SDF_VU0_STOREVECTOR_NOMEM(viewUp);
                SDF_VU0_LOADVECTOR_C(entry->pos);
                SDF_VU0_STOREVECTOR_C(positions[2]);
                SDF_VU0_STOREVECTOR_C(positions[6]);
                SDF_VU0_LOADVECTOR(rightAxis);
                VU0_SCALE_VF(vf10, along);
                SDF_VU0_COPYVECTOR_A_TO_B();
                SDF_VU0_ADDVECTOR_A_C();
                SDF_VU0_STOREVECTOR(positions[0]);
                VU0_NEGATE_XYZ(vf11);
                SDF_VU0_ADDVECTOR_B_C();
                SDF_VU0_STOREVECTOR_B(positions[4]);
                SDF_VU0_LOADVECTOR(viewUp);
                VU0_SCALE_VF(vf10, cross);
                SDF_VU0_COPYVECTOR_A_TO_B();
                SDF_VU0_ADDVECTOR_A_C();
                SDF_VU0_STOREVECTOR(positions[1]);
                SDF_VU0_STOREVECTOR(positions[5]);
                VU0_NEGATE_XYZ(vf11);
                SDF_VU0_ADDVECTOR_B_C();
                SDF_VU0_STOREVECTOR_B(positions[3]);
                SDF_VU0_STOREVECTOR_B(positions[7]);
                quad = positions + 8;
                /* Retail addresses this store from positions (+0xA0) but the next one from quad (+0x60):
                 * a bare volatile asm (no memory clobber) flushes cse's table, so the second address is not
                 * folded (cc1probe --cse, hooked==unhooked: uid599 stays quad+96). */
                VU0_STORE_VF_UNCLOBBERED(vf12, quad[2]);
                SDF_VU0_STOREVECTOR_C(quad[6]);
                SDF_VU0_LOADVECTOR(viewUp);
                VU0_SCALE_VF(vf10, side);
                SDF_VU0_COPYVECTOR_A_TO_B();
                SDF_VU0_ADDVECTOR_A_C();
                SDF_VU0_STOREVECTOR(quad[0]);
                VU0_NEGATE_XYZ(vf11);
                SDF_VU0_ADDVECTOR_B_C();
                SDF_VU0_STOREVECTOR_B(quad[4]);
                SDF_VU0_LOADVECTOR(rightAxis);
                VU0_SCALE_VF(vf10, cross);
                SDF_VU0_COPYVECTOR_A_TO_B();
                SDF_VU0_ADDVECTOR_A_C();
                SDF_VU0_STOREVECTOR(quad[1]);
                SDF_VU0_STOREVECTOR(quad[5]);
                VU0_NEGATE_XYZ(vf11);
                SDF_VU0_ADDVECTOR_B_C();
                SDF_VU0_STOREVECTOR_B(quad[3]);
                SDF_VU0_STOREVECTOR_B(quad[7]);
                eeUnpackRGBAWordFloat((entry->color & 0xFF000000) | 0x808080, 1.0f / 128.0f);
                SDF_VU0_COPYVECTOR_A_TO_B();
                SDF_VU0_LOADVECTOR(innerTint);
                SDF_VU0_MULVECTOR();
                packedInnerColor = eePackRGBAWord(128.0f);
                SDF_VU0_LOADVECTOR(outerTint);
                SDF_VU0_MULVECTOR();
                packedOuterColor = eePackRGBAWord(128.0f);
                quadColors = colors;
                for (i = 3; i >= 0; i--, quadColors += 4) {
                    quadColors[0] = packedOuterColor;
                    quadColors[1] = packedOuterColor;
                    quadColors[2] = packedInnerColor;
                    quadColors[3] = packedOuterColor;
                }
            }
            entry++;
            positions += 16;
            colors += 16;
        } while (--remaining != 0);
    }
    output->color = node->color;
    output->flag = node->copiedHeader.flag;
    output->type = info->alphaTrack.surfaceIndex;
    effDrawTrackSet(output, &matrix);
}

/* Advance the active strip record, rebuild its geometry, and submit its tracks. */
void effUpdateAndDrawStripRecord(EffectStripNode *node) {
    effAcquireStripRecord(node);
    effDrawEffectStrip(node);
}

void effSetStripRecordVector(EffectStripNode *node, const u128 *value) {
    mnuRecordSetVector(node->active, value);
}

void effSetStripRecordSecondaryVector(EffectStripNode *node, const u128 *value) {
    fileSetRecordSecondVector(node->active, value);
}

void effSetStripRecordColor(EffectStripNode *work, u32 color) {
    work->color = color;
}

/* Store opacity and notify the strip's active-record callback. */
void effSetStripOpacity(EffectStripNode *p, f32 v) {
    p->opacity = v;
    dds3DispatchIndexedCallback((u16 *)p->active, v);
}

void effResetBillTable(u8 *work) {
    u32 index = 0;
    u8 *state = (u8 *)((EffClassWork *)work)->resource;
    u32 count = ((EffBillEmitterCommon *)((EffClassWork *)work)->payload)->header.timed.count;
    EffBillEmitterEntry *entry = (EffBillEmitterEntry *)((EffFrameState *)state)->entries;
    u32 *flags = ((EffRibbonWork *)((EffFrameState *)state)->asset)->colors;

    if (count != 0) {
        do {
            index++;
            *flags++ = 0;
            entry->timer = -1;
            entry++;
        } while (index < count);
    }
}

u8 *effAllocateRingFadeEntries(EffBillVortexConfig *config) {
    struct sdfMemHandle_t *base = sdfMemAllocDirect(config->common.header.timed.count * sizeof(EffBillEmitterEntry) + 0xC);
    EffFrameState *node = (EffFrameState *)sdfMemLock(base);
    u32 count = config->common.header.segments;
    u8 *entries = (u8 *)(node + 1);

    node->entries = entries;
    node->allocation = base;
    if (count < 3) {
        config->common.header.segments = 3;
    }
    return (u8 *)node;
}

/* Ring fade tables of the ring effects: `segments + 1` entries of colors
 * (4 words each) and radii (8 words each). The alpha fades in over the first
 * fadeIn fraction of the entries and out from the fadeOut fraction; the table
 * is then copied for every remaining layer. */
void effFillRingFadeGradient(u8 *node, EffBillEmitterCommon *config) {
    EffBillEmitterCommon *cfg = config;
    EffRibbonWork *table;
    u32 *colors;
    f32 *radii;
    u32 *colorsStart;
    f32 *radiiStart;
    u32 layers;
    s32 segments;
    s32 fadeInEnd;
    s32 fadeOutStart;
    u32 entries;
    u32 words;
    f32 radius;
    f32 fadeIn;
    f32 fadeOut;
    f32 fade;
    u32 alpha;
    u32 color;
    u32 i;

    layers = cfg->header.timed.count;
    if (layers == 0) {
        return;
    }
    segments = cfg->header.segments;
    entries = segments + 1;
    words = entries * 4;
    table = (EffRibbonWork *)((EffFrameState *)node)->asset;
    colors = (u32 *)table->extra;
    radii = (f32 *)table->uvs;
    colorsStart = colors;
    radiiStart = radii;
    fadeOut = cfg->header.fadeOut;
    fadeIn = cfg->header.fadeIn;
    fadeInEnd = (s32)(fadeIn * (f32)segments);
    fadeOutStart = (s32)(fadeOut * (f32)segments);
    radius = cfg->rowRadius / 3.0f;
    for (i = 0; i < entries; i++) {
        if (i < fadeInEnd) {
            fade = (f32)i / (f32)fadeInEnd;
        } else {
            fade = 1.0f;
            if (fadeOutStart < i) {
                fade = (f32)(segments - i) / (f32)(segments - fadeOutStart);
            }
        }
        alpha = (u32)(fade * 128.0f);
        color = (alpha << 24) | 0x808080;
        colors[0] = 0x808080;
        colors[1] = color;
        colors[2] = color;
        colors[3] = 0x808080;
        radii[0] = 0.0f;
        radii[2] = radius;
        radii[4] = radius * 2.0f;
        radii[6] = radius * 3.0f;
        colors += 4;
        radii += 8;
    }
    for (i = 1; i < layers; i++) {
        memcpy(colors, colorsStart, words * 4);
        colors += words;
        memcpy(radii, radiiStart, words * 8);
        radii += words * 2;
    }
}

u8 *effCreateRingFadeTable(EffBillVortexConfig *config, u32 resource) {
    u8 *node = effAllocateRingFadeEntries(config);

    ((EffFrameState *)node)->asset = (u8 *)effCreateRibbonWithSharedResource(config->common.header.timed.count, config->common.header.segments, (struct SdfTextureFileHeader *)(u32)resource);
    effFillRingFadeGradient(node, &config->common);
    return node;
}

u8 *effAssetPointerSet(u8 *work) {
    EffBillVortexConfig *config = ((EffClassWork *)work)->payload;
    u8 *state = (u8 *)((EffClassWork *)work)->resource;
    u8 *node = effAllocateRingFadeEntries(config);
    ((EffFrameState *)node)->asset = (u8 *)effCloneRibbonWithSharedResource((EffRibbonWork *)((EffFrameState *)state)->asset);
    effFillRingFadeGradient(node, &config->common);
    return node;
}

void effReleaseRingFadeTable(s32 work) {
    EffFrameState *state;

    state = (EffFrameState *)((EffClassWork *)work)->resource;
    effSharedAssetReferenceRelease((EffRibbonWork *)state->asset);
    sdfMemFreeDirect(state->allocation);
}

INCLUDE_ASM(const s32, "code_0023E1D0", func_r1_00237890);

void effBlendBillboardInstanceColorsAndTransforms(u8 *work) {
    EffBillVortexConfig *config = (EffBillVortexConfig *)((BillCellDrawWork *)work)->config;
    u32 limit = ((BillCellDrawWork *)work)->frameLimit;
    u32 progress = config->common.header.timed.time.progress;
    u32 *list = ((BillCellDrawWork *)work)->instances;
    EffRibbonWork *out = (EffRibbonWork *)(u32)list[1];
    u128 mtx[4];
    u32 packed;
    u32 unit;
    u32 second;

    if (progress < limit && progress != 0) {
        return;
    }
    second = effSampleColorAlphaTracks(&config->common.header.timed.colorTrack, &config->common.header.timed.alphaTrack, limit, progress);
    unit = 0x3C000000;
    eeUnpackRGBAWord(((BillCellDrawWork *)work)->baseColor, unit);
    SDF_VU0_COPYVECTOR_A_TO_B();
    eeUnpackRGBAWord(second, unit);
    SDF_VU0_MULVECTOR();
    packed = eePackRGBAWord(128.0f);
    out->color = packed;
    out->surfaceIndex = config->common.header.timed.alphaTrack.surfaceIndex;
    out->drawMode = config->common.drawMode;
    SDF_VU0_LOADVECTOR(work + 0x10);
    ToMatrix();
    SDF_VU0_LOADVECTOR(effOnesVector);
    VU0_SCALAR_OP_CLOBBER(((BillCellDrawWork *)work)->scale, "vmulx.xyzw vf10, vf10, vf2x");
    SDF_VU0_SCALE_XYZ();
    SDF_VU0_LOADVECTOR(work);
    SDF_VU0_VECTOR_W_SET_ONE();
    SDF_VU0_SET_ROW3();
    VU0_STORE_MATRIX(mtx);
    effDrawRibbon(out, (Matrix4 *)mtx);
}

INCLUDE_ASM(const s32, "code_0023E1D0", effResetBillboardFrameInstanceCounters);

u8 *effAllocateBillFadeFrameEntries(EffBillColumnConfig *config) {
    struct sdfMemHandle_t *base = sdfMemAllocDirect(config->common.header.timed.count * sizeof(EffBillEmitterEntry) + 0xC);
    EffFrameState *node = (EffFrameState *)sdfMemLock(base);
    u32 count = config->common.header.segments;
    u8 *entries = (u8 *)(node + 1);

    node->entries = entries;
    node->allocation = base;
    if (count < 3) {
        config->common.header.segments = 3;
    }
    return (u8 *)node;
}

void effFillBillFadeGradient(u8 *node, EffBillEmitterCommon *config) {
    EffBillEmitterCommon *cfg = config;
    EffRibbonWork *table;
    u32 *colors;
    f32 *radii;
    u32 *colorsStart;
    f32 *radiiStart;
    u32 layers;
    s32 segments;
    s32 fadeInEnd;
    s32 fadeOutStart;
    u32 entries;
    u32 words;
    f32 radius;
    f32 fadeIn;
    f32 fadeOut;
    f32 fade;
    u32 alpha;
    u32 color;
    u32 i;

    layers = cfg->header.timed.count;
    if (layers == 0) {
        return;
    }
    segments = cfg->header.segments;
    entries = segments + 1;
    words = entries * 4;
    table = (EffRibbonWork *)((EffFrameState *)node)->asset;
    colors = (u32 *)table->extra;
    radii = (f32 *)table->uvs;
    colorsStart = colors;
    radiiStart = radii;
    fadeOut = cfg->header.fadeOut;
    fadeIn = cfg->header.fadeIn;
    fadeInEnd = (s32)(fadeIn * (f32)segments);
    fadeOutStart = (s32)(fadeOut * (f32)segments);
    radius = cfg->rowRadius / 3.0f;
    for (i = 0; i < entries; i++) {
        if (i < fadeInEnd) {
            fade = (f32)i / (f32)fadeInEnd;
        } else {
            fade = 1.0f;
            if (fadeOutStart < i) {
                fade = (f32)(segments - i) / (f32)(segments - fadeOutStart);
            }
        }
        alpha = (u32)(fade * 128.0f);
        color = (alpha << 24) | 0x808080;
        colors[0] = 0x808080;
        colors[1] = color;
        colors[2] = color;
        colors[3] = 0x808080;
        radii[0] = 0.0f;
        radii[2] = radius;
        radii[4] = radius * 2.0f;
        radii[6] = radius * 3.0f;
        colors += 4;
        radii += 8;
    }
    for (i = 1; i < layers; i++) {
        memcpy(colors, colorsStart, words * 4);
        colors += words;
        memcpy(radii, radiiStart, words * 8);
        radii += words * 2;
    }
}

u8 *effCreateBillFadeTable(EffBillColumnConfig *config, u32 resource) {
    u8 *node = effAllocateBillFadeFrameEntries(config);

    ((EffFrameState *)node)->asset = (u8 *)effCreateRibbonWithSharedResource(config->common.header.timed.count, config->common.header.segments, (struct SdfTextureFileHeader *)(u32)resource);
    effFillBillFadeGradient(node, &config->common);
    return node;
}

u8 *effCloneBillFadeTable(u8 *work) {
    EffBillColumnConfig *config = ((EffClassWork *)work)->payload;
    u8 *state = (u8 *)((EffClassWork *)work)->resource;
    u8 *node = effAllocateBillFadeFrameEntries(config);
    ((EffFrameState *)node)->asset = (u8 *)effCloneRibbonWithSharedResource((EffRibbonWork *)((EffFrameState *)state)->asset);
    effFillBillFadeGradient(node, &config->common);
    return node;
}

void effReleaseBillFadeTable(s32 work) {
    EffFrameState *state;

    state = (EffFrameState *)((EffClassWork *)work)->resource;
    effSharedAssetReferenceRelease((EffRibbonWork *)state->asset);
    sdfMemFreeDirect(state->allocation);
}

/* Update the column's particle records and the ribbon vertices for this frame. */
void effUpdateColumnParticlesAndRibbons(EffClassWork *work) {
    EffBillColumnConfig *config = (EffBillColumnConfig *)work->payload;
    EffBillEmitterCommon *common = &config->common;
    u32 frameLimit = work->frame;
    u32 progress = common->header.timed.time.progress;
    u32 layer;
    u32 particleCount;
    s32 vertexStride;
    s32 spawnAll;
    u8 respawn;
    f32 rise;
    f32 spinAccel;
    f32 gravity;
    f32 flareCenter;
    f32 columnHeight;
    u32 life;
    u32 segmentCount;
    f32 segmentCountAsFloat;
    u32 spawnRemaining;
    EffFrameState *frameState;
    EffBillColumnParticle *particles;
    EffRibbonWork *asset;
    u32 *colors;
    f32 *vertices;
    f32 *heightRows;

    if (progress < frameLimit && progress != 0) {
        return;
    }
    particleCount = common->header.timed.count;
    life = common->header.life;
    if (life == 0) {
        return;
    }

    if (common->burst != 0 && frameLimit == 0) {
        spawnAll = 1;
        spawnRemaining = particleCount;
    } else {
        spawnAll = 0;
        spawnRemaining = common->header.spawnRate;
    }
    frameState = (EffFrameState *)work->resource;
    particles = (EffBillColumnParticle *)frameState->entries;
    asset = (EffRibbonWork *)frameState->asset;
    segmentCount = common->header.segments + 1;
    respawn = common->header.respawn;
    spinAccel = config->spinAccel;
    gravity = config->gravity;
    vertices = (f32 *)asset->positions;
    heightRows = (f32 *)asset->uvs;
    colors = asset->colors;
    vertexStride = asset->rowStride;
    segmentCountAsFloat = (f32)segmentCount;
    columnHeight = common->height;
    layer = 0;
    flareCenter = config->flareCenter;
    rise = common->rise;

    for (; layer < particleCount; layer++, particles++) {
        EffBillColumnParticle *particle = particles;
        s32 age = particle->age;
        f32 ageF;

        if (age != -2) {
            if (age == -1) {
                f32 radius;
                f32 radiusStep;

                if (spawnRemaining != 0) {
                    radius = common->radiusStart *
                        (dds3GetRandFloatA(effSharedRandomState) * common->radiusStartJitter +
                         (1.0f - common->radiusStartJitter));
                    radiusStep =
                        (common->radiusEnd * (dds3GetRandFloatA(effSharedRandomState) * common->radiusEndJitter +
                                              (1.0f - common->radiusEndJitter)) - radius) /
                        (f32)life;
                    particle->radius = radius;
                    particle->radiusStep = radiusStep;
                    particle->drag = config->drag *
                        (dds3GetRandFloatA(effSharedRandomState) * config->dragJitter +
                         (1.0f - config->dragJitter));
                    particle->verticalOffset = -config->velocity * dds3GetRandFloatA(effSharedRandomState);
                    particle->flare = config->flare *
                        (dds3GetRandFloatA(effSharedRandomState) * config->flareJitter +
                         (1.0f - config->flareJitter));
                    particle->spinPhase = dds3GetRandFloatA(effSharedRandomState) * 6.2831853f;
                    particle->spin = config->spin *
                        (dds3GetRandFloatA(effSharedRandomState) * config->spinJitter +
                         (1.0f - config->spinJitter));
                    particle->width = common->width *
                        (dds3GetRandFloatA(effSharedRandomState) * common->widthJitter +
                         (1.0f - common->widthJitter));
                    particle->extent = common->extent *
                        (dds3GetRandFloatA(effSharedRandomState) * common->extentJitter +
                         (1.0f - common->extentJitter)) * 10.0f;
                    {
                        f32 random = dds3GetRandFloatA(effSharedRandomState);
                        f32 height = common->height *
                            (random * common->heightJitter + (1.0f - common->heightJitter));
                        colors[0] = 0;
                        particle->height = height;
                    }

                    if (spawnAll) {
                        s32 birthAge = dds3GetRandA((EffRandState *)effSharedRandomState) % life;
                        f32 gravityDisplacement;
                        ageF = (f32)birthAge;
                        gravityDisplacement = gravity * ageF * ageF * 0.5f;
                        particle->age = birthAge;
                        particle->radius += particle->radiusStep * ageF;
                        particle->verticalOffset -= particle->drag * ageF;
                        particle->verticalOffset += gravityDisplacement;
                    } else {
                        particle->age = 0;
                    }
                    spawnRemaining--;
                }
            } else if ((u32)age >= life) {
                colors[0] = 0;
                particle->age = respawn ? -1 : -2;
            } else {
                ageF = (f32)age;
                {
                    f32 angularStep = particle->spin + spinAccel * ageF * 0.5f;
                    f32 angle = angularStep * ageF;
                    f32 oldHeight;
                    f32 radius;
                    f32 currentY;
                    f32 flare;
                    f32 phaseOffset;
                    f32 center[4];
                    f32 direction[4];
                    f32 width;
                    f32 extentStep;
                    f32 cosine;
                    f32 sine;
                    u32 segment;

                    angle += particle->spinPhase;
                    oldHeight = particle->height;
                    radius = particle->radius;
                    currentY = particle->verticalOffset;
                    flare = particle->flare;
                    phaseOffset = ageF / (f32)life - flareCenter;
                    radius += phaseOffset * phaseOffset * flare;
                    cosine = sdfCos(angle);
                    sine = sdfSin(angle);
                    center[0] = cosine * radius;
                    center[1] = currentY;
                    center[2] = sine * radius;
                    SDF_VU0_LOADVECTOR_B(center);
                    VU0_MOVE_VF_EXTENDED(vf12, vf11);

                    currentY -= particle->drag;
                    currentY += gravity * ageF;
                    radius = particle->radius + particle->radiusStep;
                    {
                        f32 nextPhaseOffset = (ageF + 1.0f) / (f32)life - flareCenter;
                        radius += nextPhaseOffset * nextPhaseOffset * flare;
                    }


                    center[0] = cosine * radius;
                    center[1] = currentY;
                    center[2] = sine * radius;
                    SDF_VU0_LOADVECTOR(center);
                    SDF_VU0_SUBVECTOR_EXTENDED();
                    VU0_NORMALIZE_VF10_EXTENDED();
                    SDF_VU0_STOREVECTOR(direction);

                    width = particle->width * 0.25f;
                    extentStep = particle->extent / segmentCountAsFloat;
                    particle->verticalOffset = currentY;
                    particle->radius += particle->radiusStep;
                    center[0] = cosine;
                    center[1] = 0.0f;
                    center[2] = sine;
                    SDF_VU0_LOADVECTOR_B(center);
                    VU0_CROSS_XYZ_EXTENDED(vf10, vf10, vf11);
                    VU0_SCALE_VF(vf10, width);
                    SDF_VU0_COPYVECTOR_A_TO_B_EXTENDED();

                    for (segment = 0; segment < segmentCount; segment++) {
                        f32 fraction;
                        f32 pointY;
                        VU0_MOVE_VF_EXTENDED(vf10, vf12);
                        VU0_ADD_EXTENDED(vf10, vf10, vf11);
                        SDF_VU0_STOREVECTOR(vertices + 4);
                        VU0_ADD_EXTENDED(vf10, vf10, vf11);
                        SDF_VU0_STOREVECTOR(vertices);
                        VU0_MOVE_VF_EXTENDED(vf10, vf12);
                        SDF_VU0_SUBVECTOR_EXTENDED();
                        SDF_VU0_STOREVECTOR(vertices + 8);
                        SDF_VU0_SUBVECTOR_EXTENDED();
                        SDF_VU0_STOREVECTOR(vertices + 12);
                        vertices += 16;
                        fraction = (f32)segment / segmentCountAsFloat;
                        pointY = columnHeight * fraction + oldHeight;
                        heightRows[1] = pointY;
                        heightRows[3] = pointY;
                        heightRows[5] = pointY;
                        heightRows[7] = pointY;
                        heightRows += 8;

                        SDF_VU0_LOADVECTOR(direction);
                        VU0_SCALE_VF(vf10, extentStep);
                        VU0_ADD_EXTENDED(vf12, vf12, vf10);
                    }

                    *colors++ = effSampleColorAlphaTracks(&common->header.emitterColorTrack,
                                                           &common->header.emitterAlphaTrack, age, life);
                    particle->age = (s32)((u32)particle->age + 1U);
                    particle->height += rise;
                }
                continue;
            }
        }
        colors++;
        vertices += vertexStride * 4;
        heightRows += vertexStride * 2;
    }
}

void effBillBlendCellColorAndUpdateTransform(BillCellDrawWork *work) {
    EffBillColumnConfig *config = (EffBillColumnConfig *)work->config;
    u32 limit = work->frameLimit;
    u32 progress = config->common.header.timed.time.progress;
    u32 *list = work->instances;
    EffRibbonWork *out = (EffRibbonWork *)(u32)list[1];
    u128 mtx[4];
    u32 packed;
    u32 unit;
    u32 second;

    if (progress < limit && progress != 0) {
        return;
    }
    second = effSampleColorAlphaTracks(&config->common.header.timed.colorTrack, &config->common.header.timed.alphaTrack, limit, progress);
    unit = 0x3C000000;
    eeUnpackRGBAWord(work->baseColor, unit);
    SDF_VU0_COPYVECTOR_A_TO_B();
    eeUnpackRGBAWord(second, unit);
    SDF_VU0_MULVECTOR();
    packed = eePackRGBAWord(128.0f);
    out->color = packed;
    out->surfaceIndex = config->common.header.timed.alphaTrack.surfaceIndex;
    out->drawMode = config->common.drawMode;
    SDF_VU0_LOADVECTOR(work->transform);
    ToMatrix();
    SDF_VU0_LOADVECTOR(effOnesVector);
    VU0_SCALAR_OP_CLOBBER(work->scale, "vmulx.xyzw vf10, vf10, vf2x");
    SDF_VU0_SCALE_XYZ();
    SDF_VU0_LOADVECTOR(work);
    SDF_VU0_VECTOR_W_SET_ONE();
    SDF_VU0_SET_ROW3();
    VU0_STORE_MATRIX(mtx);
    effDrawRibbon(out, (Matrix4 *)mtx);
}

INCLUDE_ASM(const s32, "code_0023E1D0", effResetParticleBillFrameCounters);

u8 *effAllocateCompactRingFadeEntries(EffBillSpiralConfig *config) {
    struct sdfMemHandle_t *base = sdfMemAllocDirect(config->common.header.timed.count * 0x2C + 0xC);
    EffFrameState *node = (EffFrameState *)sdfMemLock(base);
    u32 count = config->common.header.segments;
    u8 *entries = (u8 *)(node + 1);

    node->entries = entries;
    node->allocation = base;
    if (count < 3) {
        config->common.header.segments = 3;
    }
    return (u8 *)node;
}

void effFillCompactRingFadeGradient(u8 *node, EffBillEmitterCommon *config) {
    EffBillEmitterCommon *cfg = config;
    EffRibbonWork *table;
    u32 *colors;
    f32 *radii;
    u32 *colorsStart;
    f32 *radiiStart;
    u32 layers;
    s32 segments;
    s32 fadeInEnd;
    s32 fadeOutStart;
    u32 entries;
    u32 words;
    f32 radius;
    f32 fadeIn;
    f32 fadeOut;
    f32 fade;
    u32 alpha;
    u32 color;
    u32 i;

    layers = cfg->header.timed.count;
    if (layers == 0) {
        return;
    }
    segments = cfg->header.segments;
    entries = segments + 1;
    words = entries * 4;
    table = (EffRibbonWork *)((EffFrameState *)node)->asset;
    colors = (u32 *)table->extra;
    radii = (f32 *)table->uvs;
    colorsStart = colors;
    radiiStart = radii;
    fadeOut = cfg->header.fadeOut;
    fadeIn = cfg->header.fadeIn;
    fadeInEnd = (s32)(fadeIn * (f32)segments);
    fadeOutStart = (s32)(fadeOut * (f32)segments);
    radius = cfg->rowRadius / 3.0f;
    for (i = 0; i < entries; i++) {
        if (i < fadeInEnd) {
            fade = (f32)i / (f32)fadeInEnd;
        } else {
            fade = 1.0f;
            if (fadeOutStart < i) {
                fade = (f32)(segments - i) / (f32)(segments - fadeOutStart);
            }
        }
        alpha = (u32)(fade * 128.0f);
        color = (alpha << 24) | 0x808080;
        colors[0] = 0x808080;
        colors[1] = color;
        colors[2] = color;
        colors[3] = 0x808080;
        radii[0] = 0.0f;
        radii[2] = radius;
        radii[4] = radius * 2.0f;
        radii[6] = radius * 3.0f;
        colors += 4;
        radii += 8;
    }
    for (i = 1; i < layers; i++) {
        memcpy(colors, colorsStart, words * 4);
        colors += words;
        memcpy(radii, radiiStart, words * 8);
        radii += words * 2;
    }
}

u8 *effCreateCompactRingFadeTable(EffBillSpiralConfig *config, u32 resource) {
    u8 *node = effAllocateCompactRingFadeEntries(config);

    ((EffFrameState *)node)->asset = (u8 *)effCreateRibbonWithSharedResource(config->common.header.timed.count, config->common.header.segments, (struct SdfTextureFileHeader *)(u32)resource);
    effFillCompactRingFadeGradient(node, &config->common);
    return node;
}

u8 *effCloneBillboardFrameAsset(u8 *work) {
    EffBillSpiralConfig *config = ((EffClassWork *)work)->payload;
    u8 *state = (u8 *)((EffClassWork *)work)->resource;
    u8 *node = effAllocateCompactRingFadeEntries(config);

    ((EffFrameState *)node)->asset = (u8 *)effCloneRibbonWithSharedResource((EffRibbonWork *)((EffFrameState *)state)->asset);
    effFillCompactRingFadeGradient(node, &config->common);
    return node;
}

void effReleaseCompactRingFadeTable(s32 work) {
    EffFrameState *state;

    state = (EffFrameState *)((EffClassWork *)work)->resource;
    effSharedAssetReferenceRelease((EffRibbonWork *)state->asset);
    sdfMemFreeDirect(state->allocation);
}

/* vu0 routine: native matrix, cross/normalize and ribbon extrusion. */
void effUpdateBillSpiral(EffClassWork *work) {
    EffBillSpiralConfig *config = (EffBillSpiralConfig *)work->payload;
    EffFrameState *state;
    EffRibbonWork *asset;
    EffSpiralParticle *particle;
    f32 pos[4];
    u32 *colors;
    u32 i;
    u32 count;
    u32 stride;
    u32 segments;
    s32 burst;
    u32 spawn;
    u8 respawn;
    u32 life;
    f32 (*positions)[4];
    f32 *heights;
    u32 j;
    s32 timer;
    f32 segmentCount;
    f32 spinAccel;
    f32 heightScale;
    f32 rise;
    f32 radius;
    f32 width;
    f32 angle;
    f32 step;
    f32 height;
    f32 start;
    f32 time;

    if (config->common.header.timed.time.progress < work->frame && config->common.header.timed.time.progress != 0) {
        return;
    }
    life = config->common.header.life;
    count = config->common.header.timed.count;
    if (life == 0) {
        return;
    }
    if (config->common.burst != 0 && work->frame == 0) {
        burst = 1;
        spawn = count;
    } else {
        burst = 0;
        spawn = config->common.header.spawnRate;
    }
    segments = config->common.header.segments + 1;
    state = (EffFrameState *)work->resource;
    /* The creator stores the ribbon address in this actual 12-byte header. */
    asset = (EffRibbonWork *)state->asset;
    particle = (EffSpiralParticle *)state->entries;
    respawn = config->common.header.respawn;
    /* Config reads before the ribbon's buffer loads: in retail the spin acceleration load (+0xD4) precedes the
     * asset+0x1C positions load (cc1probe --sched, sched1 equal-priority ties decide@luid), which keeps the
     * positions spill store behind it in sched2. */
    spinAccel = config->spinAccel;
    positions = (f32 (*)[4])asset->positions;
    heights = (f32 *)asset->uvs;
    colors = asset->colors;
    stride = asset->rowStride;
    segmentCount = segments;
    heightScale = config->common.height;
    rise = config->common.rise;
    for (i = 0; i < count; i++, particle++) {
        timer = particle->timer;
        if (timer != -2) {
            if (timer == -1) {
                if (spawn != 0) {
                    f32 radiusDelta;
                    start = config->common.radiusStart * (dds3GetRandFloatA(effSharedRandomState) * config->common.radiusStartJitter + (1.0f - config->common.radiusStartJitter));
                    radiusDelta = (config->common.radiusEnd * (dds3GetRandFloatA(effSharedRandomState) * config->common.radiusEndJitter + (1.0f - config->common.radiusEndJitter)) - start) / (f32)life;
                    particle->radius = start;
                    particle->radiusStep = radiusDelta;
                    particle->tiltX = (dds3GetRandFloatA(effSharedRandomState) - 0.5f) * 2.0f * 3.14159265f;
                    particle->tiltY = (dds3GetRandFloatA(effSharedRandomState) - 0.5f) * 2.0f * 3.14159265f;
                    particle->phase = dds3GetRandFloatA(effSharedRandomState) * 6.2831853f;
                    particle->spin = config->spin * (dds3GetRandFloatA(effSharedRandomState) * config->spinJitter + (1.0f - config->spinJitter));
                    particle->width = config->common.width * (dds3GetRandFloatA(effSharedRandomState) * config->common.widthJitter + (1.0f - config->common.widthJitter));
                    particle->sweep = config->common.extent * (dds3GetRandFloatA(effSharedRandomState) * config->common.extentJitter + (1.0f - config->common.extentJitter));
                    particle->height = config->common.height * (dds3GetRandFloatA(effSharedRandomState) * config->common.heightJitter + (1.0f - config->common.heightJitter));
                    *colors = 0;
                    if (burst != 0) {
                        particle->timer = dds3GetRandA(effSharedRandomState) % life;
                        time = (f32)particle->timer;
                        particle->radius += particle->radiusStep * time;
                    } else {
                        particle->timer = 0;
                    }
                    spawn--;
                }
            } else if ((u32)timer >= life) {
                *colors = 0;
                particle->timer = respawn ? -1 : -2;
            } else {
                particle->radius += particle->radiusStep;
                radius = particle->radius;
                time = (f32)timer;
                angle = (particle->spin + spinAccel * time * 0.5f) * time;
                angle += particle->phase;
                width = particle->width * 0.25f;
                if (particle->spin >= 0.0f) {
                    step = particle->sweep / segmentCount;
                } else {
                    step = -particle->sweep / segmentCount;
                }
                height = particle->height;
                SetRotateMatrixX(particle->tiltX);
                SetRotateMatrixY_B(particle->tiltY);
                MultMatrixRevNoArg();
                pos[0] = sdfCos(angle);
                pos[1] = 0.0f;
                pos[2] = sdfSin(angle);
                for (j = 0; j < segments; j++) {
                    f32 rowHeight;
                    SDF_VU0_LOADVECTOR(pos);
                    SDF_VU0_APPLYMATRIX_XYZ();
                    VU0_SCALE_VF(vf10, radius);
                    SDF_VU0_COPYVECTOR_A_TO_C();
                    angle += step;
                    pos[0] = sdfCos(angle);
                    pos[1] = 0.0f;
                    pos[2] = sdfSin(angle);
                    SDF_VU0_LOADVECTOR(pos);
                    SDF_VU0_APPLYMATRIX_XYZ();
                    VU0_SCALE_VF(vf10, radius);
                    SDF_VU0_COPYVECTOR_C_TO_B();
                    SDF_VU0_SUBVECTOR();
                    SDF_VU0_OUTERPRODUCT();
                    SDF_VU0_NORMALIZE();
                    VU0_SCALE_VF(vf10, width);
                    SDF_VU0_COPYVECTOR_A_TO_B();
                    SDF_VU0_COPYVECTOR_C_TO_A();
                    SDF_VU0_ADDVECTOR();
                    SDF_VU0_STOREVECTOR(positions[1]);
                    SDF_VU0_ADDVECTOR();
                    SDF_VU0_STOREVECTOR(positions[0]);
                    SDF_VU0_COPYVECTOR_C_TO_A();
                    SDF_VU0_SUBVECTOR();
                    SDF_VU0_STOREVECTOR(positions[2]);
                    SDF_VU0_SUBVECTOR();
                    SDF_VU0_STOREVECTOR(positions[3]);
                    positions += 4;
                    rowHeight = heightScale * ((f32)j / segmentCount) + height;
                    heights[1] = rowHeight;
                    heights[3] = rowHeight;
                    heights[5] = rowHeight;
                    heights[7] = rowHeight;
                    heights += 8;
                }
                *colors++ = effSampleColorAlphaTracks(&config->common.header.emitterColorTrack, &config->common.header.emitterAlphaTrack, timer, life);
                particle->height += rise;
                particle->timer++;
                continue;
            }
        }
        colors++;
        positions += stride;
        heights += stride * 2;
    }
}

void effUpdateCompactRingDrawColorAndTransform(u8 *work) {
    EffBillSpiralConfig *config = (EffBillSpiralConfig *)((BillCellDrawWork *)work)->config;
    u32 limit = ((BillCellDrawWork *)work)->frameLimit;
    u32 progress = config->common.header.timed.time.progress;
    u32 *list = ((BillCellDrawWork *)work)->instances;
    EffRibbonWork *out = (EffRibbonWork *)(u32)list[1];
    u128 mtx[4];
    u32 packed;
    u32 unit;
    u32 second;

    if (progress < limit && progress != 0) {
        return;
    }
    second = effSampleColorAlphaTracks(&config->common.header.timed.colorTrack, &config->common.header.timed.alphaTrack, limit, progress);
    unit = 0x3C000000;
    eeUnpackRGBAWord(((BillCellDrawWork *)work)->baseColor, unit);
    SDF_VU0_COPYVECTOR_A_TO_B();
    eeUnpackRGBAWord(second, unit);
    SDF_VU0_MULVECTOR();
    packed = eePackRGBAWord(128.0f);
    out->color = packed;
    out->surfaceIndex = config->common.header.timed.alphaTrack.surfaceIndex;
    out->drawMode = config->common.drawMode;
    SDF_VU0_LOADVECTOR(work + 0x10);
    ToMatrix();
    SDF_VU0_LOADVECTOR(effOnesVector);
    VU0_SCALAR_OP_CLOBBER(((BillCellDrawWork *)work)->scale, "vmulx.xyzw vf10, vf10, vf2x");
    SDF_VU0_SCALE_XYZ();
    SDF_VU0_LOADVECTOR(work);
    SDF_VU0_VECTOR_W_SET_ONE();
    SDF_VU0_SET_ROW3();
    VU0_STORE_MATRIX(mtx);
    effDrawRibbon(out, (Matrix4 *)mtx);
}

INCLUDE_ASM(const s32, "code_0023E1D0", func_r1_002398F8);

INCLUDE_ASM(const s32, "code_0023E1D0", func_r1_00239948);

INCLUDE_ASM(const s32, "code_0023E1D0", func_r1_002399B8);

INCLUDE_ASM(const s32, "code_0023E1D0", func_r1_00239C00);

INCLUDE_ASM(const s32, "code_0023E1D0", func_r1_00239C60);

void func_r1_00239CC0(s32 work) {
    EffFrameState *state;

    state = (EffFrameState *)((EffClassWork *)work)->resource;
    effSharedAssetReferenceRelease((EffRibbonWork *)state->asset);
    sdfMemFreeDirect(state->allocation);
}

INCLUDE_ASM(const s32, "code_0023E1D0", func_r1_00239CF0);

INCLUDE_ASM(const s32, "code_0023E1D0", func_r1_0023A3F8);

EffClassWork *effAllocateBlock(u16 kind, void *source) {
    u32 headerSize = 0x40;
    u32 size = effBlockResourceOperations[kind].payloadSize;
    EffClassWork *effect = sdfChipAllocDirect(size + headerSize);
    effect->payload = (u8 *)effect + headerSize;
    effect->color = 0x80808080;
    effect->scale = 1.0f;
    effect->kind = kind;
    effect->frame = 0;
    VU0_STORE_VF(vf0, effect);
    VU0_STORE_VF(vf0, (u8 *)effect + 0x10);
    memcpy(effect->payload, source, size);
    return effect;
}

EffClassWork *effCreateResourceInstanceB(u16 kind, void *source, u32 option) {
    EffClassWork *effect = effAllocateBlock(kind, source);
    effect->resource = (void *)effBlockResourceOperations[kind].createResource(source, option);
    effBlockResourceOperations[kind].initialize(effect);
    return effect;
}

EffClassWork *effCreateBlockResourceFromFile(FileJobPayload *request) {
    u32 *secondary = fileResolveSecondaryBuffer(request);
    void *source;
    switch (request->primary.selector) {
    case 1:
        break;
    case 4:
        secondary = NULL;
        break;
    }
    source = fileResolvePrimaryBuffer(request);
    return effCreateResourceInstanceB(request->option, source, (u32)secondary);
}

void effDestroyBlockResourceWork(EffClassWork *work) {
    effBlockResourceOperations[work->kind].destroyResource(work);
    sdfChipFreeDirect(work);
}

EffClassWork *effDuplicateActiveResourceB(EffClassWork *work) {
    EffClassWork *effect = effAllocateBlock(work->kind, work->payload);
    void *active = (void *)effBlockResourceOperations[work->kind].cloneResource(work);
    s32 kind = work->kind;
    effect->resource = active;
    effBlockResourceOperations[kind].initialize(effect);
    return effect;
}

void effResetBlockResourceFrame(EffClassWork *work) {
    effBlockResourceOperations[work->kind].initialize(work);
    work->frame = 0;
}

void effAdvanceBlockResourceFrame(EffClassWork *work) {
    effBlockResourceOperations[work->kind].update(work);
    work->frame = work->frame + 1;
}

void effDrawBlockResourceWork(EffClassWork *work) {
    effBlockResourceOperations[work->kind].draw(work);
}

INCLUDE_ASM(const s32, "code_0023E1D0", func_r1_0023A8C0);

INCLUDE_ASM(const s32, "code_0023E1D0", func_r1_0023A900);

INCLUDE_ASM(const s32, "code_0023E1D0", func_r1_0023A910);

void effSetBlockResourceColor(EffClassWork *work, u32 color) {
    work->color = color;
}

void effSetBlockResourceScale(EffClassWork *work, f32 scale) {
    work->scale = scale;
}

EffRibbonWork *effCreateRibbonWork(u32 count, u32 repeat) {
    u32 rowStride = repeat * 4 + 4;
    u32 size = (rowStride * 0x1C + 4) * count;
    u32 cells = rowStride * count;
    struct sdfMemHandle_t *allocation = sdfMemAllocDirect(size + 0x34);
    u8 *p = (u8 *)sdfMemLock((struct sdfMemHandle_t *)((u32)allocation));
    EffRibbonWork *work = (EffRibbonWork *)(p + size);
    u32 i;

    work->positions = p;
    p += cells * 16;
    work->uvs = p;
    p += cells * 8;
    work->extra = p;
    p += cells * 4;
    work->surfaceIndex = 2;
    work->color = 0x80808080;
    work->rowStride = rowStride;
    work->repeat = repeat;
    work->allocation = allocation;
    work->colors = (u32 *)p;
    work->count = count;
    work->drawMode = 0;
    for (i = 0; i < count; i++) {
        ((u32 *)p)[i] = 0x80808080;
    }
    work->handle = sdfMatNew();
    sdfMatSetConstant(work->handle, 1.0f);
    memset(&D_r1_004D1CE0, 0, sizeof(sdf3DPrimPolyParm_t));
    D_r1_004D1CE0.attr = 0x4000;
    return work;
}

EffRibbonWork *effCreateRibbonWithSharedResource(u32 count, u32 repeat, struct SdfTextureFileHeader *resource) {
    EffRibbonWork *node = effCreateRibbonWork(count, repeat);

    if (resource == 0) {
        s32 references = effSharedRibbonReferenceCount;
        node->resource = NULL;
        if (references == 0) {
            D_r1_0046D148 = (u32)effCloneSharedReferenceWithValue((struct SdfTextureFileHeader *)effWindTextureHandle, 0x300);
            references = effSharedRibbonReferenceCount;
        }
        references++;
        effSharedRibbonReferenceCount = references;
    } else {
        node->resource = effCreateSharedTextureReference(resource);
    }
    return node;
}

void effSharedAssetReferenceRelease(EffRibbonWork *work) {
    if (work->resource == NULL) {
        effSharedRibbonReferenceCount = effSharedRibbonReferenceCount - 1;
        if (effSharedRibbonReferenceCount == 0) {
            effReleaseSharedReference((RefObj *)D_r1_0046D148);
            D_r1_0046D148 = 0;
        }
    }
    else {
        effReleaseSharedReference(work->resource);
    }
    sdfMatFreeDelayed(work->handle);
    sdfMemFreeDirect(work->allocation);
}

EffRibbonWork *effCloneRibbonWithSharedResource(EffRibbonWork *source) {
    EffRibbonWork *node = effCreateRibbonWork(source->count, source->repeat);
    RefObj *texture = source->resource;

    if (texture != NULL) {
        node->resource = effRetainSharedReference(texture);
    } else {
        effSharedRibbonReferenceCount++;
        node->resource = NULL;
    }
    return node;
}

void effDrawRibbon(EffRibbonWork *work, const Matrix4 *matrix) {
    sdfGrOt_t *surface;
    void *list;
    EffGsPacket *packet;
    u32 segment;
    u32 count;
    s32 rowStride;
    s32 remaining;
    u32 *colors;
    u8 *positions;
    u8 *uvs;
    u8 *extra;
    u32 color;
    u32 segmentColor;
    f32 baseVector[4];
    sdfTexHandle_t *texture;

    count = work->count;
    if (count == 0) {
        return;
    }
    color = work->color;
    if ((color & 0xFF000000) == 0) {
        return;
    }
    surface = D_r1_004036B8[work->surfaceIndex];
    eeUnpackRGBAWordFloat(color, 1.0f / 128.0f);
    SDF_VU0_STOREVECTOR(baseVector);
    list = (void *)sdfGrAllocPacketBuff(0x20);
    sdfDrawTagInit(list);
    if (matrix == NULL) {
        SDF_VU0_LOADIDENTITY();
    } else {
        VU0_LOAD_MATRIX(matrix);
    }
    sdf3DPrimLinkTm(list, 0);
    if (work->resource == NULL) {
        texture = sdfGetOrCreateTextureForReference(surface, (RefObj *)D_r1_0046D148);
    } else {
        texture = sdfGetOrCreateTextureForReference(surface, work->resource);
    }
    sdfMatSetTex(work->handle, texture);
    sdf3DPrimLinkMat(list, work->handle, 0);
    if (work->drawMode == 0) {
        packet = (EffGsPacket *)sdfGrAllocPacketBuff(0x30);
        packet->dmaTag = 2;
        packet->vifTag = (((u64)0x50000002 << 16) | 0x1000) << 16;
        packet->gifTag = ((u64)0x10000000 << 32) | 0x8001;
        packet->registerList = 0xE;
        packet->registerValue = 0x31801;
        packet->registerAddress = 0x47;
        sdfDrawTagLink(list, (u32)packet);
    }
    positions = work->positions;
    uvs = work->uvs;
    extra = work->extra;
    colors = work->colors;
    rowStride = work->rowStride;
    segment = 0;
    if (count != 0) {
        s32 extraStep;
        s32 uvStep;
        s32 positionStep;

        positionStep = rowStride * 16;
        uvStep = rowStride * 8;
        extraStep = rowStride * 4;

        do {
            segmentColor = *colors;
            if (segmentColor & 0xFF000000) {
                remaining = rowStride;
                D_r1_004D1CE0.vertexColor = (u32 *)extra;
                D_r1_004D1CE0.vertex = (u128 *)positions;
                D_r1_004D1CE0.uv = (f32 *)uvs;
                unpackNormalizedColor(segmentColor);
                SDF_VU0_LOADVECTOR_B(baseVector);
                SDF_VU0_MULVECTOR();
                D_r1_004D1CE0.color = packNormalizedColor();
                D_r1_004D1CE0.faceCount = 12;
                D_r1_004D1CE0.pointCount = 12;
                D_r1_004D1CE0.faceIndex = D_r1_00403440;
                while (remaining >= 12) {
                    remaining -= 8;
                    sdfDrawTagLink(list, effPkt3DPrimMakePoly(&D_r1_004D1CE0));
                    D_r1_004D1CE0.vertex += 8;
                    D_r1_004D1CE0.vertexColor += 8;
                    D_r1_004D1CE0.uv += 8;
                }
                if (remaining >= 8) {
                    D_r1_004D1CE0.faceCount = 6;
                    D_r1_004D1CE0.pointCount = remaining;
                    sdfDrawTagLink(list, effPkt3DPrimMakePoly(&D_r1_004D1CE0));
                }
            }
            positions += positionStep;
            colors++;
            segment++;
            uvs += uvStep;
            extra += extraStep;
        } while (segment < count);
    }
    if (work->drawMode == 0) {
        packet = (EffGsPacket *)sdfGrAllocPacketBuff(0x30);
        packet->dmaTag = 2;
        packet->vifTag = (((u64)0x50000002 << 16) | 0x1000) << 16;
        packet->gifTag = ((u64)0x10000000 << 32) | 0x8001;
        packet->registerList = 0xE;
        packet->registerValue = 0x51801;
        packet->registerAddress = 0x47;
        sdfDrawTagLink(list, (u32)packet);
    }
    surface->regFunc(surface, (sdfDrawTag_t *)list);
}

INCLUDE_ASM(const s32, "code_0023E1D0", effLoadWindTexture);

u32 effGetWindTextureHandle(void) {
    return effWindTextureHandle;
}

void effResetAnimationFrameEntries(u8 *work) {
    u32 index = 0;
    u8 *state = (u8 *)((EffClassWork *)work)->resource;
    u32 count = ((EffBillFlameConfig *)((EffClassWork *)work)->payload)->header.timed.count;
    EffBillEmitterEntry *entry = (EffBillEmitterEntry *)((EffFrameState *)state)->entries;
    u32 *flags = ((EffStripWork *)((EffFrameState *)state)->asset)->colors;

    if (count != 0) {
        do {
            index++;
            *flags++ = 0;
            entry->timer = -1;
            entry++;
        } while (index < count);
    }
}

u32 *effAllocateAnimationBuffer(EffBillFlameConfig *work) {
    u32 age;
    u32 *buffer;
    struct sdfMemHandle_t *allocation = sdfMemAllocDirect(work->header.timed.count * sizeof(EffBillEmitterEntry) + 0xC);

    buffer = (u32 *)sdfMemLock(allocation);
    age = work->header.segments;

    buffer[0] = (u32)(buffer + 3);
    buffer[2] = (u32)allocation;
    if (age < 3) {
        work->header.segments = 3;
    }
    return buffer;
}

void effFillFadeColorRows(u8 *work, u8 *config) {
    EffBillFlameConfig *cfg = (EffBillFlameConfig *)config;
    u32 rows = cfg->header.timed.count;
    u32 i;

    if (rows != 0) {
        s32 width = cfg->header.segments;
        f32 fw = width;
        s32 fadeInEnd = cfg->header.fadeIn * fw;
        s32 fadeOutStart = cfg->header.fadeOut * fw;
        u32 cols = width + 1;
        u32 stride = cols * 4;
        u32 *color = ((EffFrameAsset *)((EffFrameState *)work)->asset)->colorRows;
        u32 *first = color;

        for (i = 0; i < cols; i++) {
            f32 t;
            u32 alpha;

            if (i < fadeInEnd) {
                t = (f32)i / (f32)fadeInEnd;
            } else if (fadeOutStart < i) {
                t = (f32)(width - i) / (f32)(width - fadeOutStart);
            } else {
                t = 1.0f;
            }
            alpha = (u32)(t * 128.0f) << 24;
            color[0] = 0x808080;
            color[1] = alpha | 0x808080;
            color[2] = alpha | 0x808080;
            color[3] = 0x808080;
            color += 4;
        }
        for (i = 1; i < rows; i++) {
            memcpy(color, first, stride * 4);
            color += stride;
        }
    }
}

u32 *effPrepareTextureAnimation(EffBillFlameConfig *work) {
    u32 *buffer = effAllocateAnimationBuffer(work);

    buffer[1] = effCreateTexturedStripWithSharedTexture(work->header.timed.count, work->header.segments);
    effFillFadeColorRows(buffer, work);
    return buffer;
}

u32 *effPrepareOwnedTextureAnimation(u8 *work) {
    u8 *anim = ((EffClassWork *)work)->payload;
    u8 *owner = (u8 *)((EffClassWork *)work)->resource;
    u32 *buffer = effAllocateAnimationBuffer(anim);

    buffer[1] = effAllocateStripFromWorkAndRetainTexture(((EffFrameState *)owner)->asset);
    effFillFadeColorRows(buffer, anim);
    return buffer;
}

void effReleaseTextureAnimationWork(s32 work) {
    EffFrameState *state;

    state = (EffFrameState *)((EffClassWork *)work)->resource;
    effReleaseScalyStripResources((u8 *)state->asset);
    sdfMemFreeDirect(state->allocation);
}

INCLUDE_ASM(const s32, "code_0023E1D0", effUpdateBillFlame);

/* vu0 routine: fade-blended colour and scaled transform of a mesh draw record */
void effUpdateTintedScaledMesh(BillCellDrawWork *work) {
    u8 *config = work->config;
    u32 limit = work->frameLimit;
    u32 progress = ((EffBillFlameConfig *)config)->header.timed.time.progress;
    u32 *list = work->instances;
    u8 *out = (u8 *)list[1];
    u128 mtx[4];
    u32 packed;
    u32 unit;
    u32 second;

    if (progress < limit && progress != 0) {
        return;
    }
    second = effSampleColorAlphaTracks(&((EffBillFlameConfig *)config)->header.timed.colorTrack, &((EffBillFlameConfig *)config)->header.timed.alphaTrack, limit, progress);
    unit = 0x3C000000;
    eeUnpackRGBAWord(work->baseColor, unit);
    SDF_VU0_COPYVECTOR_A_TO_B();
    eeUnpackRGBAWord(second, unit);
    SDF_VU0_MULVECTOR();
    packed = eePackRGBAWord(128.0f);
    ((EffStripWork *)out)->color = packed;
    ((EffStripWork *)out)->type = ((EffBillFlameConfig *)config)->header.timed.alphaTrack.surfaceIndex;
    ((EffStripWork *)out)->flag = ((EffBillFlameConfig *)config)->meshMode;
    SDF_VU0_LOADVECTOR(work->transform);
    ToMatrix();
    SDF_VU0_LOADVECTOR(effOnesVector);
    VU0_SCALAR_OP(work->scale, "vmulx.xyzw vf10, vf10, vf2x");
    SDF_VU0_SCALE_XYZ();
    SDF_VU0_LOADVECTOR(work);
    SDF_VU0_VECTOR_W_SET_ONE();
    SDF_VU0_SET_ROW3();
    VU0_STORE_MATRIX(mtx);
    effDrawScalyTexturedStrip(out, mtx);
}

void effInitializeAnimationPositions(EffClassWork *work) {
    EffAnimationState *state = (EffAnimationState *)work->resource;
    FileSlotTable *record = state->record;
    float *positions = state->positions;
    u32 count = record->count;
    u32 i = 0;

    fileClearRecordReferences(record);
    for (; i < count; i++) {
        positions[0] = dds3GetRandFloatA(effSharedRandomState);
        positions[1] = dds3GetRandFloatA(effSharedRandomState);
        positions += 2;
    }
}

u32 effClampSlotCount(FileKeyBlock *grid) {
    u32 n;
    s32 w = grid->emissionDuration;
    /* Retail 002AF560 uses signed capacity factors before the unsigned cap. */
    n = (w ? w : (s32)grid->spawnRate) * (w ? (s32)grid->spawnRate : grid->length);
    return n > 200 ? 200 : n;
}

EffAnimationState *effCreateAnimationState(u32 unused, u32 count) {
    struct sdfMemHandle_t *allocation = sdfMemAllocDirect(count * 8 + 0x10);
    EffAnimationState *state = (EffAnimationState *)sdfMemLock(allocation);

    state->allocation = allocation;
    state->positions = (f32 *)(state + 1);
    state->record = 0;
    state->textureHandle = effRetainScalyTextureReference();
    return state;
}

EffAnimationState *effActivateAnimationState(s32 work) {
    EffAnimationState *owner = (EffAnimationState *)((EffClassWork *)work)->resource;
    FileSlotTable *resource = owner->record;
    EffAnimationState *state = effCreateAnimationState((u32)((EffClassWork *)work)->payload, resource->count);

    resource = owner->record;
    state->record = fileAllocateGridRecordSlots(resource->type, resource->count,
                               resource->data1);
    return state;
}

void effReleaseAnimationFrameResources(u8 *work) {
    EffAnimationState *state = (EffAnimationState *)((EffClassWork *)work)->resource;

    effReleaseScalyTextureReference(state->textureHandle);
    if (state->record != 0) {
        effPCPHelperBEDGetModel(state->record);
    }
    sdfMemFreeDirect(state->allocation);
}

void effSynchronizeFileTransform(u8 *work) {
    EffAnimationState *state = (EffAnimationState *)((EffClassWork *)work)->resource;
    FileSlotTable *record = state->record;

    if (record != 0) {
        mnuRecordSetVector(record, work);
        fileSetRecordSecondVector(state->record, work + 0x10);
        dds3DispatchIndexedCallback((u16 *)state->record, ((EffClassWork *)work)->scale);
        fileAcquireRecord(state->record);
    }
}

INCLUDE_ASM(const s32, "code_0023E1D0", func_r1_0023BE28);

EffAnimationState *effCreatePrimarySlotAnimationState(u8 *work) {
    FileKeyBlock *mapping = (FileKeyBlock *)(work + 0x3C);
    u32 count = effClampSlotCount(mapping);
    EffAnimationState *state = effCreateAnimationState((u32)work, count);

    state->record = fileAllocateGridRecordSlots(1, count, mapping);
    return state;
}

EffAnimationState *effCreateAlternateSlotAnimationState(u8 *work) {
    FileKeyBlock *mapping = (FileKeyBlock *)(work + 0x3C);
    u32 count = effClampSlotCount(mapping);
    EffAnimationState *state = effCreateAnimationState((u32)work, count);

    state->record = fileAllocateGridRecordSlots(3, count, mapping);
    return state;
}

INCLUDE_ASM(const s32, "code_0023E1D0", func_r1_0023C510);

u32 *effAllocateQuantizedBuffer(EffBillQuantizedConfig *work) {
    struct sdfMemHandle_t *allocation = sdfMemAllocDirect(0xC);
    u32 *buffer = (u32 *)sdfMemLock(allocation);
    u32 count = work->samples.quantizedSamples;

    buffer[2] = (u32)allocation;
    if (count < 4) {
        work->samples.quantizedSamples = 4;
        count = 4;
    }
    buffer[0] = count >> 2;
    if ((work->samples.quantizedSamples & 3) != 0) {
        buffer[0] = (count >> 2) + 1;
    }
    return buffer;
}

void effBuildQuantizedTexturedStrip(u32 *buffer, EffBillQuantizedConfig *config) {
    f32 radii[4];
    f32 heights[4];
    s32 segment;
    s32 segmentCount;
    f32 rowStep;
    f32 angleStep;
    f32 rowScaleB;
    f32 rowScaleA;
    s32 fadeOutLimit;
    s32 vertexCount;
    u32 *colors;
    s32 rowCount;
    s32 fadeInLimit;
    EffStripWork *strip;
    f32 *positions;
    u32 *vertexColors;
    f32 *uvA;
    f32 *uvB;
    f32 sampleStep;
    f32 radius;
    f32 vertexScaleB;
    f32 vertexScaleA;
    f32 segmentAngle;
    f32 angle;
    f32 rowAngle;
    s32 row;
    s32 sample;
    f32 cosine;
    f32 sine;
    f32 fraction;
    f32 fractionStep;
    f32 rowFraction;
    f32 alpha;

    segmentCount = (s32)buffer[0];
    vertexCount = segmentCount * 4;
    angle = config->angularSpanDegrees * 0.017453292f;
    angleStep = angle / segmentCount;
    rowCount = config->samples.signedRows;
    rowStep = 6.2831852f / rowCount;
    rowCount++;
    strip = (EffStripWork *)buffer[1];
    vertexColors = (u32 *)strip->extra;
    uvA = (f32 *)strip->uvsA;
    uvB = (f32 *)strip->uvsB;
    colors = strip->colors;
    positions = (f32 *)strip->positions;
    sampleStep = angleStep / 3.0f;
    radius = config->radius;
    rowScaleB = config->fadeInEnd;
    vertexScaleB = config->fadeOutStart;
    rowScaleA = config->uvARowScale;
    vertexScaleA = config->uvBVertexScale;
    fadeInLimit = config->alphaFadeInFraction * vertexCount;
    fadeOutLimit = config->alphaFadeOutFraction * vertexCount;
    segmentAngle = 0.0f;

    for (segment = 0; segment < segmentCount; segment++) {
        angle = segmentAngle;
        for (sample = 0; sample < 4; sample++) {
            radii[sample] = radius * sdfSin(angle);
            heights[sample] = -radius * sdfCos(angle);
            angle += sampleStep;
        }
        segmentAngle += angleStep;
        rowAngle = 0.0f;
        /* Retail keeps this unused sample (DDS1 002B0008 / DDS2 002F33F8). */
        sdfSin(segmentAngle);
        for (row = 0; row < rowCount; row++) {
            f32 *position;
            f32 *texA;
            f32 *texB;
            cosine = sdfCos(rowAngle);
            sine = sdfSin(rowAngle);
            rowFraction = (f32)row / rowCount;
            fraction = (f32)segment / segmentCount;
            fractionStep = (1.0f / segmentCount) / 3.0f;
            texB = uvB;
            texA = uvA;
            position = positions;
            for (sample = 0; sample < 4; sample++) {
                s32 index;
                position[0] = cosine * radii[sample];
                position[1] = heights[sample];
                position[2] = sine * radii[sample];
                position += 4;
                texA[1] = fraction * vertexScaleA;
                texA[0] = rowFraction * rowScaleA;
                texA += 2;
                texB[1] = fraction * vertexScaleB;
                texB[0] = rowFraction * rowScaleB;
                texB += 2;
                fraction += fractionStep;
                index = segment * 4 + sample;
                if (index < fadeInLimit) {
                    alpha = (f32)index / fadeInLimit;
                } else if (index > fadeOutLimit) {
                    alpha = (f32)(vertexCount - index) / (vertexCount - fadeOutLimit);
                } else {
                    alpha = 1.0f;
                }
                vertexColors[sample] = 0x00808080 | ((u32)(alpha * 128.0f) << 24);
            }
            rowAngle += rowStep;
            positions += 16;
            vertexColors += 4;
            uvA += 8;
            uvB += 8;
        }
        *colors++ = 0x80808080;
    }
}

u32 *effPrepareQuantizedTexture(EffBillQuantizedConfig *work) {
    u32 *buffer = effAllocateQuantizedBuffer(work);

    buffer[1] = effCreateTexturedStripWithSharedTexture(buffer[0], work->samples.quantizedSamples);
    effBuildQuantizedTexturedStrip(buffer, work);
    return buffer;
}

u32 *effPrepareOwnedQuantizedTexture(u8 *work) {
    EffBillQuantizedConfig *anim = (EffBillQuantizedConfig *)((EffClassWork *)work)->payload;
    u8 *owner = (u8 *)((EffClassWork *)work)->resource;
    u32 *buffer = effAllocateQuantizedBuffer(anim);

    buffer[1] = effAllocateStripFromWorkAndRetainTexture(((EffFrameState *)owner)->asset);
    effBuildQuantizedTexturedStrip(buffer, anim);
    return buffer;
}

void effReleaseBillboardFrameAsset(s32 work) {
    EffFrameState *state;

    state = (EffFrameState *)((EffClassWork *)work)->resource;
    effReleaseScalyStripResources((u8 *)state->asset);
    sdfMemFreeDirect(state->allocation);
}

INCLUDE_ASM(const s32, "code_0023E1D0", effOffsetNodeRowsVU);

/* vu0 routine: fade-blended colour and scaled transform of a mesh draw record */
void effUpdateFadedMeshTransform(BillCellDrawWork *work) {
    u8 *config = work->config;
    u32 limit = work->frameLimit;
    u32 progress = ((EffBillQuantizedConfig *)config)->drawProgress;
    u32 *list = work->instances;
    u8 *out = (u8 *)list[1];
    u128 mtx[4];
    s32 color1[4];
    s32 color2[4];
    s32 blended[4];
    u32 packed;
    u32 unit;
    u32 second;
    f32 scale;

    if (progress < limit && progress != 0) {
        return;
    }
    second = effSampleColorAlphaTracks(&((EffBillQuantizedConfig *)config)->colorTrack, &((EffBillQuantizedConfig *)config)->alphaTrack, limit, progress);
    unit = 0x3C000000;
    color1[0] = work->baseColor;
    EE_MMI_RGBA_UNPACK(color1, unit);
    SDF_VU0_COPYVECTOR_A_TO_B();
    color2[0] = second;
    EE_MMI_RGBA_UNPACK(color2, unit);
    SDF_VU0_MULVECTOR();
    EE_MMI_RGBA_PACK(packed);
    blended[0] = packed;
    ((EffStripWork *)out)->color = blended[0];
    ((EffStripWork *)out)->type = ((EffBillQuantizedConfig *)config)->alphaTrack.surfaceIndex;
    ((EffStripWork *)out)->flag = ((EffBillQuantizedConfig *)config)->meshMode;
    scale = effSampleScalarCurve(&((EffBillQuantizedConfig *)config)->scaleCurve, limit, progress) * work->scale;
    SDF_VU0_LOADVECTOR(work->transform);
    ToMatrix();
    SDF_VU0_LOADVECTOR(effOnesVector);
    VU0_SCALAR_OP(scale, "vmulx.xyzw vf10, vf10, vf2x");
    SDF_VU0_SCALE_XYZ();
    SDF_VU0_LOADVECTOR(work);
    SDF_VU0_VECTOR_W_SET_ONE();
    SDF_VU0_SET_ROW3();
    VU0_STORE_MATRIX(mtx);
    effDrawScalyTexturedStrip(out, mtx);
}

EffClassWork *effAllocateBlockWithModel(u16 kind, void *source) {
    u32 headerSize = 0x40;
    u32 size = effModelBlockOperations[kind].payloadSize;
    EffClassWork *effect = sdfChipAllocDirect(size + headerSize);
    effect->payload = (u8 *)effect + headerSize;
    effect->color = 0x80808080;
    effect->scale = 1.0f;
    effect->kind = kind;
    effect->frame = 0;
    VU0_STORE_VF($vf0, effect);
    VU0_STORE_VF($vf0, effect->vectors.orientation);
    memcpy(effect->payload, source, size);
    return effect;
}

EffClassWork *effCreateResourceInstanceC(u16 kind, void *source) {
    EffClassWork *effect = effAllocateBlockWithModel(kind, source);
    effect->resource = (void *)effModelBlockOperations[kind].createResource(source);
    effModelBlockOperations[kind].initialize(effect);
    return effect;
}

EffClassWork *effCreateModelBlockFromFile(FileJobPayload *request) {
    void *source;

    source = fileResolvePrimaryBuffer(request);
    return effCreateResourceInstanceC(request->option, source);
}

void effDestroyModelBlockWork(EffClassWork *work) {
    effModelBlockOperations[work->kind].destroyResource(work);
    sdfChipFreeDirect(work);
}

EffClassWork *effRecreateActiveByClass(EffClassWork *work) {
    EffClassWork *effect = effAllocateBlockWithModel(work->kind, work->payload);
    void *resource = (void *)effModelBlockOperations[work->kind].cloneResource(work);
    s32 kind = work->kind;
    effect->resource = resource;
    effModelBlockOperations[kind].initialize(effect);
    return effect;
}

void effResetModelBlockFrame(EffClassWork *work) {
    effModelBlockOperations[work->kind].initialize(work);
    work->frame = 0;
}

void effAdvanceDispatchCounter(EffClassWork *work) {
    effModelBlockOperations[work->kind].update(work);
    work->frame++;
}

void effDrawModelBlock(EffClassWork *work) {
    effModelBlockOperations[work->kind].draw(work);
}

INCLUDE_ASM(const s32, "code_0023E1D0", func_r1_0023CFE8);

INCLUDE_ASM(const s32, "code_0023E1D0", func_r1_0023D028);

INCLUDE_ASM(const s32, "code_0023E1D0", func_r1_0023D038);

void effSetModelBlockColor(EffClassWork *work, u32 color) {
    work->color = color;
}

void effSetModelBlockScale(EffClassWork *work, f32 scale) {
    work->scale = scale;
}

/* Retain and return the lazy shared reference to /effect/scaly00.tmx. */
u32 effRetainScalyTextureReference(void) {
    if (effSharedStripReferenceCount == 0) {
        effSharedScalyStripResource = effCloneSharedReferenceWithValue((struct SdfTextureFileHeader *)effScalyTextureHandle, 0x200);
    }
    effSharedStripReferenceCount = effSharedStripReferenceCount + 1;
    return (u32)effSharedScalyStripResource;
}

/* Drop a scaly texture reference; unused is ignored and the last owner releases the clone. */
void effReleaseScalyTextureReference(s32 unused) {
    effSharedStripReferenceCount = effSharedStripReferenceCount - 1;
    if (effSharedStripReferenceCount == 0) {
        effReleaseSharedReference(effSharedScalyStripResource);
        effSharedScalyStripResource = 0;
    }
}

INCLUDE_ASM(const s32, "code_0023E1D0", effAllocateTexturedStripWork);

/* Create a scaly strip and retain its shared texture reference. */
u32 effCreateTexturedStripWithSharedTexture(u32 count, u32 repeat) {
    u32 strip = effAllocateTexturedStripWork(count, repeat);
    effRetainScalyTextureReference();
    return strip;
}

void effReleaseScalyStripResources(u8 *work) {
    effReleaseScalyTextureReference((s32)effSharedScalyStripResource);
    sdfMatFreeDelayed(((EffStripWork *)work)->handle);
    sdfMemFreeDirect(((EffStripWork *)work)->allocation);
}

/* Recreate the source strip's dimensions and retain another shared texture reference. */
u32 effAllocateStripFromWorkAndRetainTexture(u8 *work) {
    u32 strip = effAllocateTexturedStripWork(((EffStripWork *)work)->count, ((EffStripWork *)work)->repeat);
    effSharedStripReferenceCount++;
    return strip;
}

INCLUDE_ASM(const s32, "code_0023E1D0", effDrawScalyTexturedStrip);

void effLoadScalyTexture(void) {
    D_r1_0046D14C = (u32)sdfDevLoadFileEx("/effect/scaly00.tmx", &effScalyTextureHandle, 0);
}

u32 effGetScalyTextureHandle(void) {
    return effScalyTextureHandle;
}

void effSeedParticleSpanParameters(EffModelResource *work) {
    u32 index = 0;
    EffSpanTable *table = work->childResource;
    EffSpanConfig *config = work->source;
    u32 total = table->total;
    u32 per = config->perSpan;
    u32 spans = total / per;
    EffSpanRecord *record = table->records;
    u32 span;
    EffSpanEntry *entry;

    if (total % per != 0) {
        spans++;
    }
    if (table->count != 0) {
        do {
            record->pointCount = 0;
            entry = record->entries;
            span = 0;
            if (spans != 0) {
                do {
                    span++;
                    entry->first = dds3GetRandFloatA(effSharedRandomState) * config->firstRand + (1.0f - config->firstRand);
                    entry->second = config->secondBase * (dds3GetRandFloatA(effSharedRandomState) * config->rangeRand + (1.0f - config->rangeRand));
                    entry->referenceAge = 0;
                    entry++;
                } while (span < spans);
            }
            index++;
            record++;
        } while (index < table->count);
    }
}

/* Build the point and reference sets for each model map-position record. */
EffSpanTable *effCreateParticleSpanTable(EffSpanConfig *config, dds3ModelHandle_t *model) {
    u32 total = model->first->frameLength;
    u32 count = sdfModelGetMarkPosCount(model->inner);
    u32 spans;
    u32 partialSpan;
    struct sdfMemHandle_t *allocation;
    EffSpanTable *table;
    EffSpanRecord *record;
    EffSpanEntry *entries;
    u32 i;
    u32 j;
    u32 triplets;
    u32 *colors;
    u32 middleColor;
    u32 edgeColor;
    EffTrackSet *tracks;

    model->first->speed = 1.0f;
    mdlSingleMotion(model, 0, 0);
    if (config->perSpan == 0) {
        config->perSpan = 1;
    }
    partialSpan = total % config->perSpan != 0;
    spans = partialSpan + total / config->perSpan;
    if (config->referenceRampDuration == 0) {
        config->referenceRampDuration = 1;
    }
    allocation = sdfMemAllocDirect(sizeof(EffSpanTable) +
                 count * sizeof(EffSpanRecord) + count * spans * sizeof(EffSpanEntry));
    table = (EffSpanTable *)sdfMemLock(allocation);
    table->allocation = allocation;
    table->records = (EffSpanRecord *)(table + 1);
    entries = (EffSpanEntry *)(table->records + count);
    table->total = total;
    table->count = count;
    for (i = 0, record = table->records; i < count; i++, record++) {
        record->pointSet = effCreatePointSet3(total);
        record->pointSet->type = (u32)config->pointAlphaTrack.surfaceIndex;
        record->pointSet->flag = config->pointSetFlag;
        if (config->drawPoints) {
            triplets = record->pointSet->rows / 3;
            colors = (u32 *)record->pointSet->tail;
            middleColor = config->middleColor;
            edgeColor = config->edgeColor;
            for (j = 0; j < triplets; j++, colors += 3) {
                colors[0] = edgeColor;
                colors[1] = middleColor;
                colors[2] = edgeColor;
            }
        }
        record->entries = entries;
        entries += spans;
        if (config->drawReferences) {
            tracks = effCreateTrackSetWithSharedReferences(spans, 0, 0);
            record->references = tracks;
            tracks->type = (u32)config->referenceAlphaTrack.surfaceIndex;
            tracks->flag = config->pointSetFlag;
        } else {
            record->references = 0;
        }
    }
    return table;
}

void effReleaseParticleList(EffSpanTable *list) {
    u32 i;
    EffSpanRecord *entry = list->records;

    for (i = 0; i < list->count; i++, entry++) {
        effReleaseModelPointSetAsset(entry->pointSet);
        if (entry->references != 0) {
            effReleaseResourceRefs(entry->references);
        }
    }
    sdfMemFreeDirect(list->allocation);
}

INCLUDE_ASM(const s32, "code_0023E1D0", effUpdateParticleSpanGeometry);

/* vu0 routine: quaternion matrix and SDK RGBA modulation. */
void effDrawParticleSpanPointsAndReferences(EffModelResource *work) {
    Matrix4 matrix;
    u32 color1[4];
    u32 color2[4];
    u32 blended[4];
    EffSpanConfig *config = work->source;
    EffSpanTable *table = work->childResource;
    f32 scale = work->scale * config->drawScale;
    u32 count;
    u32 i;
    u32 color;
    EffSpanRecord *record;

    SDF_VU0_LOADVECTOR(work->orientation);
    ToMatrix();
    SDF_VU0_LOADVECTOR(effOnesVector);
    VU0_SCALAR_OP(scale, "vmulx.xyzw vf10, vf10, vf2x");
    SDF_VU0_SCALE_XYZ();
    SDF_VU0_LOADVECTOR(work->position);
    SDF_VU0_VECTOR_W_SET_ONE();
    SDF_VU0_SET_ROW3();
    VU0_STORE_MATRIX_M(matrix.u.rows[0], matrix.u.rows[1], matrix.u.rows[2], matrix.u.rows[3]);
    count = table->count;

    if (config->drawPoints != 0 && work->updateCount < config->progress) {
        color = effSampleColorAlphaTracks(&config->pointColorTrack, &config->pointAlphaTrack, work->updateCount, config->progress);
        color1[0] = work->color;
        EE_MMI_RGBA_UNPACK(color1, 0.0078125f);
        SDF_VU0_COPYVECTOR_A_TO_B();
        color2[0] = color;
        EE_MMI_RGBA_UNPACK(color2, 0.0078125f);
        SDF_VU0_MULVECTOR();
        EE_MMI_RGBA_PACK_F128(blended[0]);
        color = blended[0];

        record = table->records;
        for (i = 0; i < count; i++, record++) {
            if (record->pointCount != 0) {
                record->pointSet->rows = record->pointCount;
                record->pointSet->color = color;
                effDrawThreePointGroups(record->pointSet, &matrix);
            }
        }
    }

    if (config->drawReferences != 0) {
        color = work->color;
        record = table->records;
        for (i = 0; i < count; i++, record++) {
            EffTrackSet *references = record->references;

            references->color = color;
            effDrawTrackSet(references, &matrix);
        }
    }
}

EffModelResource *effCreateModelResourceWithInlineData(u16 kind, void *source, void *secondary, u32 param) {
    u32 headerSize = 0x40;
    u32 size = effModelResourceOperations[kind].payloadSize;
    EffModelResource *effect = (EffModelResource *)sdfChipAllocDirect(size + headerSize);

    effect->source = (EffSpanConfig *)((u8 *)effect + headerSize);
    effect->color = 0x80808080;
    effect->scale = 1.0f;
    effect->updateCount = 0;
    effect->kind = kind;
    VU0_STORE_VF_UNCLOBBERED($vf0, effect->position);
    VU0_STORE_VF_UNCLOBBERED($vf0, effect->orientation);
    memcpy(effect->source, source, size);
    if (secondary != NULL) {
        effect->model = effLoadViewerModelWithVUState(secondary, param);
        effect->attributes = param;
        effect->childResource = effModelResourceOperations[kind].createResource(effect->source, effect->model);
        effModelResourceOperations[kind].initialize(effect);
    }
    return effect;
}

u32 effCreateModelResourceFromFile(FileJobPayload *work) {
    void *first = fileResolvePrimaryBuffer(work);
    void *second = fileResolveSecondaryBuffer(work);
    return (u32)effCreateModelResourceWithInlineData(work->option, first, second, work->secondary.size);
}

void effDestroyModelResource(EffModelResource *effect) {
    effModelResourceOperations[effect->kind].destroyResource(effect->childResource);
    effDestroyModelContext(effect->model);
    sdfChipFreeDirect(effect);
}

EffModelResource *effCreateModelResource(EffModelCreateRequest *work) {
    EffModelResource *effect = effCreateModelResourceWithInlineData(work->kind, work->source, 0, 0);
    s32 x = mdlGetMajorNum(work->assetId);
    s32 y = mdlGetMinorNum(work->assetId);
    dds3ModelHandle_t *model = mdlLoad(x, y);

    effect->model = model;
    effInitModelVUState(model);
    effect->attributes = work->attributes;
    effect->childResource = effModelResourceOperations[effect->kind].createResource(effect->source, effect->model);
    effModelResourceOperations[effect->kind].initialize(effect);
    return effect;
}

void effResetModelResourceUpdateCount(EffModelResource *effect) {
    effModelResourceOperations[effect->kind].initialize(effect);
    effect->updateCount = 0;
}

void effAdvanceModelResourceUpdateCount(EffModelResource *effect) {
    effModelResourceOperations[effect->kind].update(effect);
    effect->updateCount = effect->updateCount + 1;
}

void effDispatchModelResourceCallback(EffModelResource *effect) {
    effModelResourceOperations[effect->kind].draw(effect);
}

INCLUDE_ASM(const s32, "code_0023E1D0", func_r1_0023EA00);

INCLUDE_ASM(const s32, "code_0023E1D0", func_r1_0023EA58);

INCLUDE_ASM(const s32, "code_0023E1D0", func_r1_0023EA68);

void effSetModelResourceColor(EffModelResource *work, u32 color) {
    work->color = color;
}

void effSetModelResourceScale(EffModelResource *work, f32 scale) {
    work->scale = scale;
}

EffPointSet *effCreatePointSet3(s32 count) {
    s32 rows = count * 3 + 3;
    s32 size = rows * 20;
    struct sdfMemHandle_t *base;
    u8 *data;
    EffPointSet *set;

    size = ((size >> 4) + ((size & 0xF) != 0)) << 4;
    base = sdfMemAllocDirect(size + 0x20);
    data = (u8 *)sdfMemLock(base);
    set = (EffPointSet *)(data + size);
    set->buffer = data;
    data += rows * 16;
    set->color = 0x80808080;
    set->type = 2;
    set->rows = rows;
    set->allocation = base;
    set->tail = data;
    set->flag = 0;
    set->handle = sdfMatNew();
    sdfMatSetConstant(set->handle, 1.0f);
    memset(D_r1_004D1D40, 0, 0x2C);
    D_r1_004D1D40[0].attr = 0x4000;
    return set;
}

/* Queue the draw asset for release and return the backing allocation. */
void effReleaseModelPointSetAsset(EffPointSet *set) {
    sdfMatFreeDelayed(set->handle);
    sdfMemFreeDirect(set->allocation);
}

void effDrawThreePointGroups(EffPointSet *set, Matrix4 *matrix) {
    void *list;
    EffGsPacket *packet;
    s32 remaining;

    if (set->color & 0xFF000000) {
        list = (void *)sdfGrAllocPacketBuff(0x20);
        sdfDrawTagInit(list);
        if (matrix == NULL) {
            SDF_VU0_LOADIDENTITY();
        } else {
            VU0_LOAD_MATRIX(matrix);
        }
        sdf3DPrimLinkTm(list, 0);
        sdf3DPrimLinkMat(list, set->handle, 0);
        if (set->flag == 0) {
            packet = (EffGsPacket *)sdfGrAllocPacketBuff(0x30);
            packet->dmaTag = 2;
            packet->vifTag = (((u64)0x50000002 << 16) | 0x1000) << 16;
            packet->gifTag = ((u64)0x10000000 << 32) | 0x8001;
            packet->registerList = 0xE;
            packet->registerValue = 0x31801;
            packet->registerAddress = 0x47;
            sdfDrawTagLink(list, (u32)packet);
        }
        remaining = set->rows;
        D_r1_004D1D40[0].vertexColor = (u32 *)set->tail;
        D_r1_004D1D40[0].vertex = (u128 *)set->buffer;
        D_r1_004D1D40[0].color = set->color;
        D_r1_004D1D40[0].faceCount = 0x10;
        D_r1_004D1D40[0].pointCount = 0xF;
        D_r1_004D1D40[0].faceIndex = (u32 *)D_r1_004034A0;
        while (remaining >= 0xF) {
            remaining -= 0xC;
            sdfDrawTagLink(list, effPkt3DPrimMakePoly(D_r1_004D1D40));
            D_r1_004D1D40[0].vertex += 0xC;
            D_r1_004D1D40[0].vertexColor += 0xC;
        }
        if (remaining >= 6) {
            D_r1_004D1D40[0].faceCount = 4;
            D_r1_004D1D40[0].pointCount = remaining;
            sdfDrawTagLink(list, effPkt3DPrimMakePoly(D_r1_004D1D40));
        }
        if (set->flag == 0) {
            packet = (EffGsPacket *)sdfGrAllocPacketBuff(0x30);
            packet->dmaTag = 2;
            packet->vifTag = (((u64)0x50000002 << 16) | 0x1000) << 16;
            packet->gifTag = ((u64)0x10000000 << 32) | 0x8001;
            packet->registerList = 0xE;
            packet->registerValue = 0x51801;
            packet->registerAddress = 0x47;
            sdfDrawTagLink(list, (u32)packet);
        }
        D_r1_004037A8[set->type]->regFunc(D_r1_004037A8[set->type], list);
    }
}

INCLUDE_ASM(const s32, "code_0023E1D0", effInitializeParticleDirection);

INCLUDE_ASM(const s32, "code_0023E1D0", effSetRotationFromAcceleratedFrameAngle);

INCLUDE_ASM(const s32, "code_0023E1D0", func_r1_0023EFE0);

INCLUDE_ASM(const s32, "code_0023E1D0", effCreateConfiguredSlotNode);

INCLUDE_ASM(const s32, "code_0023E1D0", effCreateSlotNodeFromFileJob);

INCLUDE_ASM(const s32, "code_0023E1D0", effDestroyOwnedResources);

INCLUDE_ASM(const s32, "code_0023E1D0", effCloneOwnedState);

INCLUDE_ASM(const s32, "code_0023E1D0", effCloneSlotNodeSecondaryResources);

INCLUDE_ASM(const s32, "code_0023E1D0", effRebuildResourceEntries);

INCLUDE_ASM(const s32, "code_0023E1D0", effReplaceEffectSlotModelAndDeviceResources);

INCLUDE_ASM(const s32, "code_0023E1D0", effRebuildResourceEntryClones);

typedef struct EffectSlotNode80 {
    u32 count;
    u32 color;
    f32 opacity;
    s32 resourceKind;
    u8 randomDirection;
    u8 pad11[3];
    f32 seedDirection[3];
    u8 pad20[4];
    f32 angularSpeed;
    f32 speedRandomness;
    f32 angularAcceleration;
    u16 scaleByOpacity;
    u8 pad32[0x2E];
    dds3ModelHandle_t *model;
    void *deviceSlot;
    FileQueue **resourceEntries;
    struct sdfMemHandle_t *entryAllocation;
    FileSlotTable *record;
    u8 *positions;
    struct sdfMemHandle_t *positionAllocation;
    u16 active;
} EffectSlotNode80;


void func_r1_00224D28(FileSlotTable *record);

void effClearSurfaceRecordReferences(void *node) {
    FileSlotTable *record = ((EffectSlotNode80 *)node)->record;

    if (record != NULL) {
        func_r1_00224D28(record);
    }
}

INCLUDE_ASM(const s32, "code_0023E1D0", func_r1_0023F7F0);

INCLUDE_ASM(const s32, "code_0023E1D0", func_r1_0023F8E8);

INCLUDE_ASM(const s32, "code_0023E1D0", func_r1_0023FF28);

void effSetActiveSlotPrimaryRecordVector(void *node, f32 vector[4]) {
    mnuRecordSetVector(((EffectSlotNode80 *)node)->record, vector);
}

void effSetActiveSlotSecondaryRecordVector(void *node, f32 vector[4]) {
    fileSetRecordSecondVector(((EffectSlotNode80 *)node)->record, vector);
}

void effSetActiveSlotColor(EffectSlotNode80 *work, u32 color) {
    work->color = color;
}

void effSetActiveSlotOpacity(void *work, f32 opacity) {
    ((EffectSlotNode80 *)work)->opacity = opacity;
    dds3DispatchIndexedCallback((u16 *)((EffectSlotNode80 *)work)->record, opacity);
}

/* Alternate particle handles share a header but occupy distinct slots. */
EffParticleSharedResource *effCreateParticleSharedResourceFromFile(FileJobPayload *source) {
    EffParticleSharedResource *work = sdfChipAllocDirect(sizeof(EffParticleSharedResource));
    void *buffer;

    memset(work, 0, sizeof(EffParticleSharedResource));
    work->color = 0x80808080;
    work->billHandle = NULL;
    work->reference = NULL;
    if (source == NULL) {
        return work;
    }
    work->option = source->option;
    buffer = fileResolvePrimaryBuffer(source);
    memcpy(&work->colorTrack, buffer, EFF_PARTICLE_SHARED_PAYLOAD_BYTES);
    buffer = fileResolveSecondaryBuffer(source);
    if (buffer != NULL) {
        switch (source->primary.selector) {
        case 1:
            work->billHandle = dds3BillboardCreate(0, (u32)buffer);
            break;
        case 2:
            work->billHandle = dds3BillboardCreate(1, (u32)buffer);
            break;
        case 4:
            work->billHandle = dds3BillboardGeneral(*(s32 *)buffer);
            break;
        case 7:
            work->reference = effCreateTextureAnimationSet((u32)buffer);
            break;
        }
        if (work->billHandle != NULL) {
            dds3BillboardSetAnimColorAdvMode(work->billHandle);
        }
    }
    return work;
}

void effReleaseParticleResources(EffParticleSharedResource *work) {
    billHandle_t *particle = work->billHandle;
    if (particle != NULL) {
        dds3BillboardDelete(particle);
    }
    if (work->reference != NULL) {
        effReleaseReferenceHolder(work->reference);
    }
    sdfChipFreeDirect(work);
}

EffParticleSharedResource *effCloneParticleSharedResource(EffParticleSharedResource *source) {
    EffParticleSharedResource *effect = effCreateParticleSharedResourceFromFile(NULL);
    memcpy(&effect->colorTrack, &source->colorTrack, EFF_PARTICLE_SHARED_PAYLOAD_BYTES);
    effReplaceSharedResource(effect, source);
    return effect;
}

void effReplaceSharedResource(EffParticleSharedResource *work, EffParticleSharedResource *source) {
    billHandle_t *billboard;
    if (source->billHandle != 0) {
        if (work->billHandle != 0) {
            dds3BillboardDelete(work->billHandle);
        }
        billboard = dds3BillboardCopy(
            source->billHandle);
        work->billHandle = billboard;
        dds3BillboardSetAnimColorAdvMode(billboard);
        return;
    }
    if (work->reference != NULL) {
        effReleaseReferenceHolder(work->reference);
    }
    work->reference = effReferenceObjectRetain(source->reference);
}

void effResetParticleSharedState(EffParticleSharedResource *work) {
    work->state = 0;
}

INCLUDE_ASM(const s32, "code_0023E1D0", func_r1_002403F0);

void effSetParticleSharedColor(EffParticleSharedResource *work, u32 color) {
    work->color = color;
}

void func_r1_00240990(void) {
}

INCLUDE_ASM(const s32, "code_0023E1D0", func_r1_00240998);

INCLUDE_ASM(const s32, "code_0023E1D0", func_r1_002409C8);

INCLUDE_ASM(const s32, "code_0023E1D0", func_r1_002409F8);

INCLUDE_ASM(const s32, "code_0023E1D0", func_r1_00240B90);

INCLUDE_ASM(const s32, "code_0023E1D0", func_r1_00240BA8);

void func_r1_00240C70(void) {
}

INCLUDE_ASM(const s32, "code_0023E1D0", func_r1_00240C78);

INCLUDE_ASM(const s32, "code_0023E1D0", func_r1_00240D38);

s32 effComputeLightDirectionVU(dds3ModelHandle_t *vector, sdf3DLightParallelPacket_t *target) {
    s32 result = btlIsRuntimeAllocated();

    if (result != 0) {
        if (D_r1_0046D169 == 0) {
            return 0;
        }
        mdlGetPosition(vector);
        VU0_LOAD_VF($vf11, D_r1_004D1DC0);
        VU0_SUB($vf10, $vf10, $vf11);
        SDF_VU0_VECTOR_W_SET_ZERO();
        SDF_VU0_NORMALIZE();
        VU0_STORE_VF($vf10, D_r1_004D1D80);
        sdf3DMakeLightParallelPacket(target, D_r1_004037C0, D_r1_004D1DB0);
        return 1;
    }
    return result;
}

INCLUDE_ASM(const s32, "code_0023E1D0", effResetDefaultColorTables);

INCLUDE_ASM(const s32, "code_0023E1D0", func_r1_00240FE0);

INCLUDE_ASM(const s32, "code_0023E1D0", func_r1_002410B0);

INCLUDE_ASM(const s32, "code_0023E1D0", func_r1_002411F8);

INCLUDE_ASM(const s32, "code_0023E1D0", func_r1_00241230);

INCLUDE_ASM(const s32, "code_0023E1D0", func_r1_002412D8);

INCLUDE_ASM(const s32, "code_0023E1D0", func_r1_00241370);

INCLUDE_ASM(const s32, "code_0023E1D0", func_r1_00241D20);

INCLUDE_ASM(const s32, "code_0023E1D0", func_r1_00241DC8);

void func_r1_00241FD8(void) {
}

INCLUDE_ASM(const s32, "code_0023E1D0", func_r1_00241FE0);

INCLUDE_ASM(const s32, "code_0023E1D0", func_r1_00241FE8);

void effDestroyCopiedEffectPayload(EffCopiedPayload *payload) {
    dds3Basic_t **effects = payload->effects;
    dds3PathBasicControl_t **targets = payload->targets;
    u32 i;

    for (i = 0; i < 5; i++) {
        if (*targets != 0) {
            dds3DeletePathControl(*targets);
        }
        targets++;
        if (*effects != 0) {
            dds3DeleteSceneBasicData(*effects);
        }
        effects++;
    }
    sdfMemFreeDirect(payload->allocation);
}

void func_r1_00242080(void) {
}

void func_r1_00242088(void) {
}

void func_r1_00242090(void) {
}

void func_r1_00242098(void) {
}

void func_r1_002420A0(void) {
}

void effResetSlots(void) {
    D_r1_0046D170[0] = 0;
    D_r1_0046D178[0] = 0;
    D_r1_0046D180[0] = 0;
    D_r1_0046D188[0] = 0;
    D_r1_0046D170[1] = 0;
    D_r1_0046D178[1] = 0;
    D_r1_0046D180[1] = 0;
    D_r1_0046D188[1] = 0;
    kwlnPadResetMotorLevelsAndOutput();
}

INCLUDE_ASM(const s32, "code_0023E1D0", func_r1_002420F0);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_r1_00455EE8);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_r1_00455F00);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_r1_00455F10);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_r1_00455F20);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_r1_00455F30);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_r1_00455F40);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_r1_00455F50);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_r1_00455F60);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_r1_00455F70);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_r1_00455F80);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_r1_00455F90);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_r1_00455FA0);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_r1_00455FB0);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_r1_00455FC0);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_r1_00455FD0);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_r1_00455FE0);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_r1_00455FF0);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_r1_00456000);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_r1_00456010);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_r1_00456020);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_r1_00456030);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_r1_00456040);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_r1_00456050);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_r1_00456060);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_r1_00456070);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_r1_00456080);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_r1_00456090);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_r1_004560A0);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_r1_004560B0);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_r1_004560C0);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_r1_004560D0);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_r1_004560F0);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_r1_00456110);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_r1_00456130);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_r1_00456150);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_r1_00456170);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_r1_00456190);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_r1_004561B0);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_r1_004561D0);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_r1_004561F0);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_r1_00456210);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_r1_00456230);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_r1_00456250);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_r1_00456270);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_r1_00456290);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_r1_004562B0);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_r1_004562D0);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_r1_004562F0);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_r1_00456310);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_r1_00456330);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_r1_00456350);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_r1_00456370);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_r1_00456390);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_r1_004563B0);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_r1_004563D0);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_r1_004563F0);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_r1_00456410);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_r1_00456430);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_r1_00456450);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_r1_00456470);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_r1_00456490);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_r1_004564B0);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_r1_004564D0);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_r1_004564F0);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_r1_00456518);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_r1_00456538);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_r1_00456558);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_r1_00456578);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_r1_00456598);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_r1_004565B8);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_r1_004565D8);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_r1_004565F8);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_r1_00456618);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_r1_00456638);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_r1_00456658);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_r1_00456678);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_r1_00456698);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_r1_004566B8);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_r1_004566D8);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_r1_004566F8);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_r1_00456718);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_r1_00456738);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_r1_00456758);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_r1_00456778);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_r1_00456798);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_r1_004567B8);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_r1_004567D8);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_r1_004567F8);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_r1_00456818);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_r1_00456838);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_r1_00456858);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_r1_00456878);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_r1_00456898);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_r1_004568B8);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_r1_004568D8);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_r1_004568F8);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_r1_00456918);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_r1_00456938);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_r1_00456958);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_r1_00456978);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_r1_00456998);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_r1_004569B8);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_r1_004569D8);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_r1_004569F8);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_r1_00456A18);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_r1_00456A38);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_r1_00456A58);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_r1_00456A78);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_r1_00456A98);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_r1_00456AB8);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_r1_00456AD8);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_r1_00456AF8);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_r1_00456B18);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_r1_00456B38);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_r1_00456B58);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_r1_00456B78);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_r1_00456B98);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_r1_00456BB8);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_r1_00456BD8);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_r1_00456BF8);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_r1_00456C18);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_r1_00456C38);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_r1_00456C48);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_r1_00456C58);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_r1_00456C68);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_r1_00456C78);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_r1_00456C88);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_r1_00456C98);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_r1_00456CA8);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_r1_00456CB8);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_r1_00456CC8);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_r1_00456CD8);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_r1_00456CE8);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_r1_00456CF8);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_r1_00456D08);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_r1_00456D18);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_r1_00456D28);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_r1_00456D38);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_r1_00456D48);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_r1_00456D58);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_r1_00456D68);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_r1_00456D78);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_r1_00456D88);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_r1_00456D98);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_r1_00456DA8);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_r1_00456DB8);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_r1_00456DC8);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_r1_00456DD8);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_r1_00456DE8);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_r1_00456DF8);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_r1_00456E08);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_r1_00456E18);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_r1_00456E28);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_r1_00456E38);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_r1_00456E48);

INCLUDE_RODATA(const s32, "code_0023E1D0", D_r1_00456E58);

INCLUDE_SDATA(const s32, "code_0023E1D0", D_r1_0046D130);

INCLUDE_SDATA(const s32, "code_0023E1D0", D_r1_0046D134);

INCLUDE_SDATA(const s32, "code_0023E1D0", D_r1_0046D138);

INCLUDE_SDATA(const s32, "code_0023E1D0", D_r1_0046D13C);

INCLUDE_SDATA(const s32, "code_0023E1D0", D_r1_0046D140);

INCLUDE_SDATA(const s32, "code_0023E1D0", D_r1_0046D144);

INCLUDE_SDATA(const s32, "code_0023E1D0", D_r1_0046D148);

INCLUDE_SDATA(const s32, "code_0023E1D0", D_r1_0046D14C);

INCLUDE_SDATA(const s32, "code_0023E1D0", D_r1_0046D150);

INCLUDE_SDATA(const s32, "code_0023E1D0", D_r1_0046D154);

INCLUDE_SDATA(const s32, "code_0023E1D0", D_r1_0046D158);

INCLUDE_SDATA(const s32, "code_0023E1D0", D_r1_0046D160);

INCLUDE_SDATA(const s32, "code_0023E1D0", D_r1_0046D164);

INCLUDE_SDATA(const s32, "code_0023E1D0", D_r1_0046D168);

INCLUDE_SDATA(const s32, "code_0023E1D0", D_r1_0046D169);

INCLUDE_SDATA(const s32, "code_0023E1D0", D_r1_0046D170);

INCLUDE_SDATA(const s32, "code_0023E1D0", D_r1_0046D178);

INCLUDE_SDATA(const s32, "code_0023E1D0", D_r1_0046D180);

INCLUDE_SDATA(const s32, "code_0023E1D0", D_r1_0046D188);

