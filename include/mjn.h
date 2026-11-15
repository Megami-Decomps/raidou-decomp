#ifndef MJN_H
#define MJN_H

#include "common.h"
#include "kernel.h"

struct Agari_t;
struct NormalAnalysis_t;
struct TehaiAnalysis_t;

s32 AIChooseSutehai(s32 playerIndex, struct TehaiAnalysis_t *analysis);
void ApplyAgari(s32 playerIndex, s32 fromPlayerIndex);
void ApplyChonbo(s32 playerIndex, s32 type);
struct Agari_t *CalcAgari(s32 playerIndex, struct NormalAnalysis_t *analysis, s32 type);
s32 CalcFu(s32 playerIndex, struct NormalAnalysis_t *analysis, s32 tsumo, s32 agariHai);
s32 CalcHaiValue(s32 hai, s32 playerIndex);
s32 CalcHaiValueTanki(s32 hai, s32 playerIndex);
void CalcTensu(s32 playerIndex, struct Agari_t *agari, s32 tsumo);
void CalcYaku(s32 playerIndex, struct NormalAnalysis_t *analysis, struct Agari_t *agari, s32 tsumo, s32 agariHai);
s32 CheckChankan(void);
s32 CheckFuriten(s32 playerIndex);
s32 CheckHonitsu(s32 playerIndex, s32 *tehai14);
s32 CheckPinfu(s32 playerIndex, struct NormalAnalysis_t *analysis, s32 **tehai14, s32 agariHai);
s32 CheckSananko(s32 playerIndex, struct NormalAnalysis_t *analysis, s32 **tehai14, s32 tsumo, s32 agariHai);
s32 CheckTanyao(s32 playerIndex, s32 *tehai14);
s32 CheckYakuAri(s32 playerIndex, s32 tsumo, s32 agariHai);
void Draw3D(void);
void DrawHai3DExCalc(f32 x, f32 y, f32 z, s32 hai, s32 direction, s32 highlight);
void DrawHai3DIndex(s32 index);
void DrawInfoUnit(void);
void DrawOya3D(void);
void DrawTable(void);
s32 GetHaiColor(s32 hai);
s32 GetMentsuType(s32 *mentsu);
void InitCamera(void);
void InitLight(void);
void InitOyaModel(void);
void InitTenbouModel(void);
void M_InitScene_mjn(void);
void MenuInit(void);
void SetCamera(f32 posx, f32 posy, f32 posz, f32 targetx, f32 targety, f32 targetz);
void StartCutin(s32 playerIndex, s32 msgID, s32 nextPhase, s32 flag, s32 hai);
void UpdateTensu(struct Agari_t *agari);
s32 cmpGetsubMenuPID(void);
dds3ProcessFunc_t jmsRootWaitCalcProcess(dds3ProcessID_t *PID);
s32 nbCheckAgariSub(s32 playerIndex);

#endif
