#ifndef SBTLSCENE_H
#define SBTLSCENE_H

#include "common.h"
#include "sdf_types.h"

s32 BasicSceneFree(void);
s32 Init_sbtlScene(void);
s32 SetTestCamera(void);
s32 sbtlDefaultCutScroll(void);
void sbtlSetCutScroll(sdfVector4_t *pos);
void sbtlDefaultZoom2(s32 time);
s32 sbtlReLoadMap(void);
s32 sbtlSceneLoad(void);

#ifdef VERSION_RAIDOU2
#include "fileManager.h"
extern sdfMemHandle_t *sbtlCallMemGetPtrData;
extern sdfMemHandle_t *sbtlCallMemGetPtr2Data;
extern u8 *sbtlData157;
extern u8 *sbtlData158;
extern s32 sbtlData159;
extern s32 sbtlData160;
extern fileRequest_t *sbtlCallMemGetPtrData2;
extern fileRequest_t *sbtlCallMemGetPtr2Data2;
extern sdfMemHandle_t *sbtlCallPackDecodeData;
extern u8 *sbtlCallPackDecodeData2;
extern s32 sbtlCallPackDecodeData3;
extern fileRequest_t *sbtlCallMemGetPtr3Data;
extern s32 LoadMap_FromMemoryData3;
extern sdfVector4_t LoadMap_FromMemoryData; /* HD nakama_pos */
extern sdfVector4_t LoadMap_FromMemoryData2; /* HD nakama_rot */
extern s32 sbtlData156;
extern f32 sbtlData161;
void sbtlModelFieldLb(void);
void sbtlModelFieldLb2(void);
void sbtlModelFieldLb3(void);
void sbtlCallMemGetPtr(void);
void sbtlCallMemGetPtr2(void);
void sbtlCallMemGetPtr3(void);
void sbtlLoadMapFromMemorySub(void);
void sbtlLoadMapFromMemorySub2(void);
void sbtlCallPackDecode(void);
void sbtlCallSync2(void);
void SetMapLight(struct dds3Basic_t *unit, u32 lrgb, u32 argb);
void sbtlSetRndEncBg(s32 rnd_enc_id);
#endif
#endif
