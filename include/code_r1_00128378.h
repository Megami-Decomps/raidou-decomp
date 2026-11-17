#ifndef CODE_R1_00128378_H
#define CODE_R1_00128378_H

#include "common.h"
#include "basic.h"
#include "sdf_resource.h"
#include "fileManager.h"
#include "sdfRand.h"

void fldAiboCashLoad_Block(void);
s32 fldCtnGetSub(s32 modelId);
s32 fldPlayerALoadEndCheck(void);
void fldPlayerALoadStart(void);
void fldRegistUnitPad(void);
s32 fldSetUnitSub(void);
s32 fldSetUnitSub2(void);
s32 mapGetCircleCircleH(f32 x1, f32 y1, f32 r1, f32 x2, f32 y2, f32 r2, f32 *x3, f32 *y3);
void mapGetEyeTgtRot(f32 eyex, f32 eyez, f32 tgtx, f32 tgtz);
f32 mapGetEyeTgt_Rot_OfsPos(f32 eyex, f32 eyez, f32 tgtx, f32 tgtz, f32 olen, f32 *retx, f32 *retz);
dds3Basic_t *mapGetPlayersBasic(s32 type);
void mapGetTaniVector(f32 vx0, f32 vy0, f32 *vx1, f32 *vy1);
void mapRaidouBackUpPosReset(void);
s32 mapGetRaidouSerialID(void);
s32 mapGetAiboSerialID(void);
s32 mapGetAiboSerialID2(void);
s32 mapGetNakamaSerialID(void);
s32 mapGetNakamaSerialID2(void);
void effBTLFieldColorDraw(void);
void effRefDvBasic2(void);
void mapNakamaOikakeBuffReset2(void);
void mapCallGetRotPoseSet(void);
void mapCallGetRotPoseSet2(void);
void mapCutFileLoad(void);
void mapAM_MaskClear(void);
void fldFirstInit(void);
void mapRndEncParamSet(void);
void mapSetPlayersVisible(s32 r_onoff, s32 n_onoff, s32 d_onoff);
void fldAllShutDown(void);

s32 PlayerGetMdlLoadID(char *path);
void fldPlayerAFree(void);
void fldPlayerALoad(void);
void fldCallMemFreeDelayedDirect(void);
void fldCallMemFreeDelayedDirect2(void);
void fldCallMemFreeDelayedDirect3(void);
void fldCallPackDecode(void);
void nbResultFreeTextures(void);
void fldNakamaCashLoad_Block(void);
void fldJDataLoad(void);
void cmpModelFreeResource(void);

/* Field player model pack: the loaded block, its decode copy, the decode
 * pointer and sizes, and the pac request while it loads. */
extern sdfRandCB_t fldRandomBuf;
/* Automap mask slots: the (map, floor) pair of each automapmask and automapmask2 row. */
extern u8 mapProcSequenceSubData[80][2];
extern u8 mapProcSequenceSubData2[16][2];
extern sdfMemHandle_t *fldPlayerAnimData;
extern sdfMemHandle_t *effQueuedResourceNameRecord3;
extern void *gfldPlayerPtr;
extern u32 fldJDataLoadData2;
extern u32 gfldPlayerSizeBk;
extern fileRequest_t *gMapCash_pacHandle;

/* Party demon model cache: the loaded block, its data pointer and size, and
 * the load state fldNakamaCashLoad_EndCheck steps through. */
extern sdfMemHandle_t *fldData11;
extern void *nbResultFreeTexturesData;
extern s32 nbResultFreeTexturesData2;
extern s32 fldNakamaCashLoad_EndCheckData;

#endif
