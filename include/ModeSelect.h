#ifndef MODESELECT_H
#define MODESELECT_H

#include "common.h"

struct Player_t;
struct TehaiAnalysis_t;
struct dds3ModelHandle_t;

void AITurn(s32 playerIndex);
void Agari(void);
void ApplyReach(s32 playerIndex);
void ApplyReachEx(s32 playerIndex, s32 sutehaiIndex, s32 ichijunme);
void ApplyRon(s32 playerIndex, s32 fromPlayerIndex);
void ApplyRonRyukyoku(s32 flag);
void ApplyTsumo(s32 playerIndex);
s32 AskRon(s32 playerIndex);
s32 AskRonKokushi(s32 playerIndex);
s32 AskTsumo(s32 playerIndex);
s32 CheckAgari(s32 playerIndex);
s32 CheckAgariKokushi(s32 playerIndex);
s32 CheckAnkanReach(s32 playerIndex, s32 tsumoHai);
void EndMahjong(void);
void Ignore(void);
void InitEndHanchan(void);
void InitEx(s32 devil0, s32 devil1, s32 devil2);
void InitHanchan(void);
void InitInterface(void);
void InitKyoku(void);
void ManageEndHanchanPhase(void);
void ManageModelPhase(void);
void ManageSummonPhase(void);
struct dds3ModelHandle_t *ModelLoad_mjn(s32 major, s32 minor);
void NakiWait(void);
void NextKyoku(void);
void PadDealer(void);
void PlayerTurn(s32 playerIndex);
void Ryukyoku(void);
void SetHaipai(void);
void SetTehai14(s32 playerIndex);
s32 ShutDown(void);
s32 Start_mjn(void);
void Suteru(s32 playerIndex, s32 haiIndex);
void TochuRyukyoku(s32 flag);
void Tsumikomi(void);
void TsumikomiDecide(struct Player_t *player);
struct TehaiAnalysis_t *Tsumoru(s32 playerIndex);
s32 TsumoruDora(s32 playerIndex);

#endif
