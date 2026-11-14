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

void effPCPCrackSizeInit(effPCPCrack_t *pPCPCrack, effPCPCrackPartFlash_t *pSize)
{
    f32 blend = pPCPCrack->offsetRandomness;
    f32 scale = pPCPCrack->scale;
    f32 t;

    pSize->length = pPCPCrack->offsetDistance *
        (dds3GetRandFloatA(dds3EffectBase) * blend + (1.0f - blend)) * scale;
    blend = pPCPCrack->extentRandomness;
    t = dds3GetRandFloatA(dds3EffectBase) * blend + (1.0f - blend);
    pSize->width_in = pPCPCrack->baseExtent * t * scale;
    pSize->width_out = pPCPCrack->tipExtent * t * scale;
    pSize->type = dds3GetRandA(dds3EffectBase) & 1;
    pSize->frame = -(dds3GetRandA(dds3EffectBase) % pPCPCrack->delaySpread);
}

void effPCPCrackInit(effPCPCrack_t *pPCPCrack)
{
    effCrackModel_t *model = dds3EffectPartGetHandle(pPCPCrack->part);
    effPCPCrackPart_t *group;
    effPCPCrackPartFlash_t *nextCells;
    u32 groupIndex;
    u32 cellIndex;

    if (pPCPCrack->delaySpread <= 0) {
        pPCPCrack->delaySpread = 1;
    }
    model->first->frameStep = pPCPCrack->frameStep;
    mdlSingleMotion(model, 0, 0);
    pPCPCrack->cellCount = model->first->frameCount;
    pPCPCrack->MarkCount = sdfModelGetMarkPosCount(model->inner);
    pPCPCrack->system = dds3TrackPolyCreate(pPCPCrack->MarkCount, pPCPCrack->cellCount, 1, 1);
    dds3TrackPolySetMode(pPCPCrack->system, pPCPCrack->drawBucket);
    pPCPCrack->groupsHandle = NULL;
    if (pPCPCrack->hasCells) {
        pPCPCrack->groupsHandle = sdfMemAllocDirect(pPCPCrack->MarkCount * sizeof(effPCPCrackPart_t)
            + pPCPCrack->cellCount * pPCPCrack->MarkCount * sizeof(effPCPCrackPartFlash_t));
        nextCells = (effPCPCrackPartFlash_t *)sdfMemLock(pPCPCrack->groupsHandle);
        pPCPCrack->pPart = (effPCPCrackPart_t *)nextCells;
        nextCells = (effPCPCrackPartFlash_t *)((u8 *)nextCells + pPCPCrack->MarkCount * sizeof(effPCPCrackPart_t));
        for (groupIndex = 0, group = pPCPCrack->pPart; groupIndex < pPCPCrack->MarkCount; groupIndex++, group++) {
            effPCPCrackPartFlash_t *cell;

            group->hFlash = dds3FlashPolyListCreate(pPCPCrack->cellCount);
            if (pPCPCrack->directionMode == 0) {
                group->dir[0] = 0.0f;
                group->dir[1] = -1.0f;
                group->dir[2] = 0.0f;
            } else {
                group->dir[0] = 0.0f;
                group->dir[1] = 1.0f;
                group->dir[2] = 0.0f;
            }
            group->rotate_add = (0.1f * (3.14159265f / 180.0f));
            group->rotate = 0.0f;
            for (cellIndex = 0; cellIndex < pPCPCrack->cellCount; cellIndex++) {
                f32 *vertices = dds3FlashPolyListGetVertex(group->hFlash, cellIndex);
                effCrackColorSlot_t *colors = dds3FlashPolyListGetVC(group->hFlash, cellIndex);

                colors->color[0] = 0;
                colors->color[1] = 0;
                colors->color[2] = 0;
                colors->color[3] = 0;
                colors->color[4] = 0;
                SDF_VU0_LOADIDENTITYVECTOR();
                SDF_VU0_STOREVECTOR(vertices);
                SDF_VU0_STOREVECTOR(vertices + 4);
                SDF_VU0_STOREVECTOR(vertices + 8);
                SDF_VU0_STOREVECTOR(vertices + 12);
                SDF_VU0_STOREVECTOR(vertices + 16);
            }
            dds3FlashPolyListSetBlend(group->hFlash, pPCPCrack->blend);
            group->pSize = nextCells;
            nextCells += pPCPCrack->cellCount;
            for (cellIndex = 0, cell = group->pSize; cellIndex < pPCPCrack->cellCount; cellIndex++, cell++) {
                effPCPCrackSizeInit(pPCPCrack, cell);
            }
        }
    }
}

effPCPCrack_t *effPCPCrackCreate(effPCPCrackParam_t *pParam, void *pPB)
{
    effPCPCrack_t *work = sdfChipAllocDirect(0xAC);

    *(effCrackHead_t *)work = pParam->head;
    work->abgr = 0x80808080;
    work->frame = 0;
    work->part = dds3EffectPartCreate(3, pPB);
    effPCPCrackInit(work);
    return work;
}

INCLUDE_ASM(const s32, "effect/effPCPBoss", func_r1_00219990);

effPCPCrack_t *effPCPCrackCopy(effPCPCrack_t *src)
{
    effPCPCrack_t *work;

    work = sdfChipAllocDirect(sizeof(effPCPCrack_t));
    memcpy(work, src, sizeof(effCrackHead_t));
    work->frame = 0;
    work->abgr = 0x80808080;
    work->part = dds3EffectPartCopy(src->part);
    effPCPCrackInit(work);
    return work;
}

void effPCPCrackDelete(effPCPCrack_t *hData)
{
    u32 groupIndex;

    if (hData->groupsHandle != NULL) {
        for (groupIndex = 0; groupIndex < hData->MarkCount; groupIndex++) {
            dds3FlashPolyListDelete(hData->pPart[groupIndex].hFlash);
        }
        sdfMemFreeDirect(hData->groupsHandle);
    }
    dds3TrackPolyDelete(hData->system);
    dds3EffectPartDelete(hData->part);
    sdfChipFreeDirect(hData);
}

/* Same native algorithm as dds-decomp/redthing1's effPCPCrackDraw: transform
 * each trail sample, then emit and fade its delayed flash cells. */
void effPCPCrackDraw(effPCPCrack_t *work) {
    sdfVector4_t perpendicular;
    sdfVector4_t direction;
    sdfVector4_t axis;
    sdfVector4_t scaleVector;
    sdfVector4_t widthVector;
    sdfVector4_t displacement;
    sdfVector4_t baseVector;
    sdfVector4_t tipVector;
    mdlObj_t *model;
    struct sdfModelRootNode_t *map;
    s32 i;
    s32 colorStart;
    s32 mode;
    u32 tint;
    u32 center;
    u128 *historyStart;
    s32 duration;
    s32 fadeIn;
    s32 fadeOut;
    s32 ramp;
    s32 colorFrames;
    u8 cellCountFromFrame;
    effPCPCrackPart_t *group;
    effPCPCrackPartFlash_t *cell;
    parCell_t *historyCell;
    u128 *positions; /* Position workspace reused for trail history and cell vertices. */
    u32 *colorWords;
    s32 frame;
    s32 groupCount;
    s32 colorCellCount;
    s32 j, count;
    s32 historyCount;
    s32 age, currentAge;
    u32 edge, alphaColor;
    f32 blend, trailLength, cellLength, taperStep;
    f32 taperRange;
    f32 baseExtent, tipExtent;

    frame = SDF_FRAME_TICK(work->frame);
    mode = work->directionMode;
    model = dds3EffectPartGetHandle(work->part);
    map = model->inner;
    SDF_VU0_LOADVECTOR(work->position);
    mdlSetPosition(model);
    scaleVector.x = work->scale;
    scaleVector.y = work->scale;
    scaleVector.z = work->scale;
    scaleVector.w = 0.0f;
    SDF_VU0_LOADVECTOR(&scaleVector);
    mdlSetScale(model);
    mdlLink(model, sdfModelOtTable);
    if (work->drawTrail) {
        colorStart = work->trailColorStartFrame;
        if (frame > colorStart) {
            if (work->trailColorTransitionFrames > 0) {
                blend = (f32)(frame - colorStart) / work->trailColorTransitionFrames;
                center = effColorBlend(work->trailStartCenterColor, work->trailEndCenterColor, blend);
                edge = effColorBlend(work->trailStartEdgeColor, work->trailEndEdgeColor, blend);
            } else {
                center = work->trailEndCenterColor;
                edge = work->trailEndEdgeColor;
            }
        } else {
            center = work->trailStartCenterColor;
            edge = work->trailStartEdgeColor;
        }
        groupCount = work->MarkCount;
        for (i = 0; i < groupCount; i++) {
            colorWords = work->system->cells[i].colors;
            colorCellCount = work->cellCount;
            if (colorCellCount != 0) {
                j = colorCellCount;
                do {
                    colorWords[0] = edge;
                    colorWords[1] = center;
                    colorWords[2] = edge;
                    colorWords += 3;
                    --j;
                } while (j != 0);
            }
            groupCount = work->MarkCount;
        }
    }
    groupCount = work->MarkCount;
    widthVector.x = (work->trailWidth * 0.5f) * scaleVector.x;
    widthVector.y = (work->trailWidth * 0.5f) * scaleVector.x;
    widthVector.z = (work->trailWidth * 0.5f) * scaleVector.x;
    widthVector.w = 0.0f;
    perpendicular.x = perpendicular.y = perpendicular.z = perpendicular.w = widthVector.w;
    tint = work->abgr;
    i = 0;
    if (groupCount > 0) {
        taperRange = -1.0f;
        do {
            historyCell = &work->system->cells[i];
            positions = historyCell->history;
            historyCell->color = tint;
            if (mode == 2) {
                direction.x = 0.0f;
                direction.y = 0.0f;
                direction.z = 1.0f;
                sdfModelGetMarkPosMatrix(map, i);
                SDF_VU0_LOADVECTOR(&direction);
                SDF_VU0_APPLYMATRIX_XYZ();
                SDF_VU0_STOREVECTOR(&direction);
                work->pPart[i].dir[0] = direction.x;
                work->pPart[i].dir[1] = direction.y;
                work->pPart[i].dir[2] = direction.z;
            }
            sdfModelGetMarkPos(map, i);
            if (frame < work->cellCount) {
                historyCount = historyCell->vertexCount;
                SDF_FRAME_WHEN(work->frame, SDF_VU0_STOREVECTOR_NOMEM(positions + historyCount));
                SDF_FRAME_WHEN(work->frame, SDF_VU0_STOREVECTOR_NOMEM(positions + historyCount + 1));
                SDF_FRAME_WHEN(work->frame, SDF_VU0_STOREVECTOR_NOMEM(positions + historyCount + 2));
                historyCell->vertexCount += 3 * SDF_FRAME_DUE(work->frame);
                taperStep = taperRange / frame;
                blend = 1.0f;
                SDF_VU0_LOADVECTOR_C(D_r1_004028C0);
                if (frame > 0) {
                    const f32 *trailWidth = &widthVector.x;
                    j = frame;
                    do {
                        SDF_VU0_LOADVECTOR(positions + 4);
                        SDF_VU0_LOADVECTOR_B(positions + 1);
                        SDF_VU0_SUBVECTOR();
                        SDF_VU0_GETLENGTH(trailLength);
                        if (trailLength > 1.0f) {
                            SDF_VU0_COPYVECTOR_C_TO_B();
                            SDF_VU0_OUTERPRODUCT();
                            SDF_VU0_NORMALIZE();
                            SDF_VU0_STOREVECTOR(&perpendicular);
                        } else {
                            SDF_VU0_LOADVECTOR(&perpendicular);
                        }
                        SDF_VU0_LOADVECTOR_B(trailWidth);
                        SDF_VU0_SCALEVECTOR_B_BITS(blend);
                        SDF_VU0_MULVECTOR();
                        SDF_VU0_COPYVECTOR_A_TO_B();
                        SDF_VU0_LOADVECTOR(positions + 1);
                        SDF_VU0_ADDVECTOR();
                        SDF_VU0_STOREVECTOR(positions);
                        SDF_VU0_SUBVECTOR();
                        SDF_VU0_SUBVECTOR();
                        SDF_VU0_STOREVECTOR(positions + 2);
                        --j;
                        blend += taperStep;
                        positions += 3;
                    } while (j != 0);
                }
            }
            i++;
        } while (i < groupCount);
    }
    if (work->drawTrail) {
        dds3TrackPolyDraw(work->system);
    }
    if (work->groupsHandle != NULL) {
        duration = work->cellDuration;
        count = frame - work->cellStartFrame;
        fadeIn = work->cellFadeIn;
        fadeOut = work->cellFadeOut;
        colorStart = work->cellColorStartAge;
        colorFrames = work->cellColorTransitionFrames;
        cellCountFromFrame = work->cellCountFromFrame;
        if (count > 0) {
            ramp = work->offsetRampFrames;
            if (cellCountFromFrame == 1) count = frame;
            if (count > work->cellCount) count = work->cellCount;
            group = work->pPart;
            for (i = 0; i < groupCount; i++, group++) {
                historyCell = &work->system->cells[i];
                historyStart = historyCell->history;
                cell = group->pSize;
                dds3FlashPolyListSetABGR(group->hFlash, tint);
                for (j = 0; j < count - 1; j++, cell++, historyStart += 3) {
                    positions = (u128 *)dds3FlashPolyListGetVertex(group->hFlash, j);
                    colorWords = dds3FlashPolyListGetVC(group->hFlash, j)->color;
                    age = cell->frame;
                    if (age > colorStart) {
                        if (work->cellColorTransitionFrames > 0 && age < colorStart + colorFrames) {
                            blend = (f32)(age - colorStart) / colorFrames;
                            center = effColorBlend(work->cellStartCenterColor, work->cellEndCenterColor, blend);
                            edge = effColorBlend(work->cellStartEdgeColor, work->cellEndEdgeColor, blend);
                        } else {
                            center = work->cellEndCenterColor;
                            edge = work->cellEndEdgeColor;
                        }
                    } else {
                        center = work->cellStartCenterColor;
                        edge = work->cellStartEdgeColor;
                    }
                    center &= 0xFFFFFF;
                    edge &= 0xFFFFFF;
                    currentAge = cell->frame;
                    if (currentAge >= 0) {
                        if (currentAge >= ramp) direction.y = cell->length;
                        else direction.y = (cell->length * currentAge) / ramp;
                        displacement.x = group->dir[0] * direction.y;
                        displacement.y = group->dir[1] * direction.y;
                        displacement.z = group->dir[2] * direction.y;
                        displacement.w = 0.0f;
                        if (mode == 2 && currentAge >= group->rotationStartAge) {
                            SDF_VU0_LOADVECTOR(historyStart + 4);
                            SDF_VU0_LOADVECTOR_B(historyStart + 1);
                            SDF_VU0_SUBVECTOR();
                            SDF_VU0_NORMALIZE();
                            SDF_VU0_STOREVECTOR_NOMEM(&axis);
                            SetRotateMatrixAxis(group->rotate, &axis);
                            SDF_VU0_LOADVECTOR(&displacement);
                            SDF_VU0_APPLYMATRIX_XYZ();
                            SDF_VU0_STOREVECTOR(&displacement);
                            group->rotate += SDF_FRAME_STEP(group->rotate_add);
                        }
                        baseExtent = cell->width_in;
                        baseVector.x = baseExtent;
                        baseVector.y = baseExtent;
                        baseVector.z = baseExtent;
                        baseVector.w = 0.0f;
                        tipExtent = cell->width_out;
                        tipVector.x = tipExtent;
                        tipVector.y = tipExtent;
                        tipVector.z = tipExtent;
                        tipVector.w = 0.0f;
                        SDF_VU0_LOADVECTOR(historyStart + 4);
                        SDF_VU0_LOADVECTOR_B(historyStart + 1);
                        SDF_VU0_SUBVECTOR();
                        SDF_VU0_GETLENGTH(cellLength);
                        if (cellLength > 1.0f) {
                            SDF_VU0_NORMALIZE();
                            SDF_VU0_COPYVECTOR_A_TO_B();
                            SDF_VU0_LOADVECTOR(&tipVector);
                            SDF_VU0_MULVECTOR();
                            SDF_VU0_STOREVECTOR(&tipVector);
                            SDF_VU0_LOADVECTOR(&baseVector);
                            SDF_VU0_MULVECTOR();
                            SDF_VU0_STOREVECTOR(&baseVector);
                            SDF_VU0_LOADVECTOR_C(&displacement);
                            SDF_VU0_LOADVECTOR(historyStart + 4);
                            SDF_VU0_LOADVECTOR_B(&baseVector);
                            SDF_VU0_STOREVECTOR(positions + 3);
                            SDF_VU0_ADDVECTOR();
                            SDF_VU0_STOREVECTOR(positions + 2);
                            SDF_VU0_SUBVECTOR();
                            SDF_VU0_SUBVECTOR();
                            SDF_VU0_STOREVECTOR(positions + 4);
                            SDF_VU0_LOADVECTOR(historyStart + 4);
                            SDF_VU0_LOADVECTOR_B(&tipVector);
                            SDF_VU0_ADDVECTOR_A_C();
                            SDF_VU0_ADDVECTOR();
                            SDF_VU0_STOREVECTOR(positions);
                            SDF_VU0_SUBVECTOR();
                            SDF_VU0_SUBVECTOR();
                            SDF_VU0_STOREVECTOR(positions + 1);
                        } else {
                            SDF_VU0_LOADIDENTITYVECTOR();
                            SDF_VU0_STOREVECTOR(positions);
                            SDF_VU0_STOREVECTOR(positions + 1);
                            SDF_VU0_STOREVECTOR(positions + 2);
                            SDF_VU0_STOREVECTOR(positions + 3);
                            SDF_VU0_STOREVECTOR(positions + 4);
                        }
                        alphaColor = 0;
                        if (age <= duration) {
                            if (age < fadeIn && fadeIn != 0) {
                                blend = (f32)age / fadeIn;
                                alphaColor = effMagaCylinderCalcColor(((u32)(blend * 2147483648.0f) & 0xFF000000) | 0x808080, tint);
                            } else if (duration - age <= fadeOut && fadeOut != 0) {
                                blend = (f32)(duration - age) / fadeOut;
                                alphaColor = effMagaCylinderCalcColor(((u32)(blend * 2147483648.0f) & 0xFF000000) | 0x808080, tint);
                            } else alphaColor = tint;
                        }
                        colorWords[0] = effMagaCylinderCalcColor(edge, alphaColor);
                        colorWords[1] = effMagaCylinderCalcColor(edge, alphaColor);
                        if (cell->type) {
                            colorWords[2] = effMagaCylinderCalcColor(0x80000000, alphaColor);
                            colorWords[3] = effMagaCylinderCalcColor(center | 0xFF000000, alphaColor);
                            colorWords[4] = colorWords[2];
                        } else {
                            colorWords[2] = effMagaCylinderCalcColor(0xFF000000, alphaColor);
                            colorWords[3] = effMagaCylinderCalcColor(center | 0x40000000, alphaColor);
                            colorWords[4] = colorWords[2];
                        }
                    }
                    cell->frame += SDF_FRAME_DUE(work->frame);
                }
                dds3FlashPolyListDraw(group->hFlash);
            }
        }
    }
    work->frame++;
}

INCLUDE_ASM(const s32, "effect/effPCPBoss", func_r1_0021A588);

void func_r1_0021A598(effPCPCrack_t *hData, u32 abgr)
{
    hData->abgr = abgr;
}

effPCPBossBael_t *effPCPBossBaelCreate(void *pParam, void *pPB, void *pPCF)
{
    effPCPBossBael_t *work;

    work = sdfChipAllocDirect(sizeof(effPCPBossBael_t));
    memcpy(&work->param, pParam, 0x10);
    work->abgr = 0x80808080;
    work->frame = 0;
    work->hPB = dds3EffectPartCreate(3, pPB);
    work->hPCF = dds3EffectPartCreate(6, pPCF);
    return work;
}

INCLUDE_ASM(const s32, "effect/effPCPBoss", func_r1_0021A640);

effPCPBossBael_t *effPCPBossBaelCopy(effPCPBossBael_t *hData, s32 bPos, s32 tbldata)
{
    effPCPBossBael_t *work;

    work = sdfChipAllocDirect(sizeof(effPCPBossBael_t));
    memcpy(&work->param, &hData->param, 0x10);
    work->frame = 0;
    work->abgr = 0x80808080;
    work->hPB = dds3EffectPartCopy(hData->hPB);
    work->hPCF = dds3EffectPartCopy(hData->hPCF);
    return work;
}

void effPCPBossBaelDelete(effPCPBossBael_t *hData)
{
    dds3EffectPartDelete(hData->hPCF);
    dds3EffectPartDelete(hData->hPB);
    sdfChipFreeDirect(hData);
}

void func_r1_0021A760(void) {
}

INCLUDE_ASM(const s32, "effect/effPCPBoss", func_r1_0021A768);

void func_r1_0021A780(effPCPCrack_t *hData, s32 frames)
{
    hData->trailColorTransitionFrames = frames;
}

s32 func_r1_0021A788(void)
{
    return 0;
}

s32 func_r1_0021A790(void)
{
    return 0;
}

s32 func_r1_0021A798(void)
{
    return 0;
}

void func_r1_0021A7A0(void) {
}

void func_r1_0021A7A8(void) {
}

void func_r1_0021A7B0(void) {
}

s32 func_r1_0021A7B8(void)
{
    return 0;
}

s32 func_r1_0021A7C0(void)
{
    return 0;
}

s32 func_r1_0021A7C8(void)
{
    return 0;
}

void func_r1_0021A7D0(void) {
}

void func_r1_0021A7D8(void) {
}

void func_r1_0021A7E0(void) {
}

void func_r1_0021A7E8(void) {
}

s32 func_r1_0021A7F0(void)
{
    return 0;
}

s32 func_r1_0021A7F8(void)
{
    return 0;
}

s32 func_r1_0021A800(void)
{
    return 0;
}

void func_r1_0021A808(void) {
}

void func_r1_0021A810(void) {
}

void func_r1_0021A818(void) {
}

s32 func_r1_0021A820(void)
{
    return 0;
}

s32 func_r1_0021A828(void)
{
    return 0;
}

s32 func_r1_0021A830(void)
{
    return 0;
}

void func_r1_0021A838(void) {
}

void func_r1_0021A840(void) {
}

void func_r1_0021A848(void) {
}

s32 func_r1_0021A850(void)
{
    return 0;
}

s32 func_r1_0021A858(void)
{
    return 0;
}

s32 func_r1_0021A860(void)
{
    return 0;
}

void func_r1_0021A868(void) {
}

void func_r1_0021A870(void) {
}

void func_r1_0021A878(void) {
}

void func_r1_0021A880(void) {
}

s32 func_r1_0021A888(void)
{
    return 0;
}

s32 func_r1_0021A890(void)
{
    return 0;
}

s32 func_r1_0021A898(void)
{
    return 0;
}

void func_r1_0021A8A0(void) {
}

void func_r1_0021A8A8(void) {
}

void func_r1_0021A8B0(void) {
}

void func_r1_0021A8B8(void) {
}

s32 func_r1_0021A8C0(void)
{
    return 0;
}

s32 func_r1_0021A8C8(void)
{
    return 0;
}

s32 func_r1_0021A8D0(void)
{
    return 0;
}

void func_r1_0021A8D8(void) {
}

void func_r1_0021A8E0(void) {
}

void func_r1_0021A8E8(void) {
}

effPCPCrack_t *effPCPCrack2Create(effPCPCrackParam_t *pParam, void *pPB)
{
    effPCPCrack_t *work;
    u32 groupIndex;

    work = effPCPCrackCreate(pParam, pPB);
    for (groupIndex = 0; groupIndex < work->MarkCount; groupIndex++) {
        work->pPart[groupIndex].rotationStartAge = pParam->rotationStartAge;
        work->pPart[groupIndex].rotate = 0.0f;
        work->pPart[groupIndex].rotate_add = pParam->angularSpeed;
    }
    return work;
}

INCLUDE_ASM(const s32, "effect/effPCPBoss", func_r1_0021A958);

effPCPCrack_t *effPCPCrack2Copy(effPCPCrack_t *src)
{
    effPCPCrack_t *work;
    u32 groupIndex;

    work = effPCPCrackCopy(src);
    for (groupIndex = 0; groupIndex < work->MarkCount; groupIndex++) {
        work->pPart[groupIndex].rotationStartAge = src->pPart[groupIndex].rotationStartAge;
        work->pPart[groupIndex].rotate = 0.0f;
        work->pPart[groupIndex].rotate_add = src->pPart[groupIndex].rotate_add;
    }
    return work;
}

void func_r1_0021AA18(effPCPCrack_t *hData)
{
    effPCPCrackDelete(hData);
}

void effPCPCrackDraw(effPCPCrack_t *hData);

void func_r1_0021AA30(effPCPCrack_t *hData)
{
    effPCPCrackDraw(hData);
}

INCLUDE_ASM(const s32, "effect/effPCPBoss", func_r1_0021AA48);

INCLUDE_ASM(const s32, "effect/effPCPBoss", func_r1_0021AA60);

effPCPBossMETATOBeam_t *effPCPBossMETATOBeamCreate(void *pParam)
{
    effPCPBossThunderParam_t thunder;
    effPCPBossMETATOBeam_t *work = sdfChipAllocDirect(sizeof(effPCPBossMETATOBeam_t));

    memcpy(work, pParam, 0x70);
    work->abgr = 0x80808080;
    work->frame = 0;
    memcpy(&thunder.head, work->unk10, 0x54);
    thunder.count = 4;
    work->polyLists[0] = effPCP_ThunderPolyListCreate(&thunder);
    work->polyLists[1] = effPCP_ThunderPolyListCreate(&thunder);
    return work;
}

effPCPBossMETATOBeam_t *effPCPBossMETATOBeamCreate(void *pParam);

effPCPBossMETATOBeam_t *func_r1_0021AC80(void *hData, s32 bPos, s32 tbldata)
{
    return effPCPBossMETATOBeamCreate(dds3EffectPCPDataGetData(hData, 0));
}

effPCPBossMETATOBeam_t *effPCPBossMETATOBeamCreate(void *pParam);

effPCPBossMETATOBeam_t *func_r1_0021ACA0(void *hData, s32 bPos, s32 tbldata)
{
    return effPCPBossMETATOBeamCreate(hData);
}

void effPCPBossMETATOBeamDelete(effPCPBossMETATOBeam_t *hData)
{
    effPCP_ThunderPolyListDelete(hData->polyLists[1]);
    effPCP_ThunderPolyListDelete(hData->polyLists[0]);
    sdfChipFreeDirect(hData);
}
