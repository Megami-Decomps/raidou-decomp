#ifndef DDS3SEQUENCELIST_H
#define DDS3SEQUENCELIST_H

#include "common.h"
#include "kernel.h"

struct SequenceParameter;
struct SBTL_START_DATA;

struct fldSceneParam_t;
void CallField(DDS3_ADMINI_CALL flg, struct fldSceneParam_t *param);
void CallEvent(DDS3_ADMINI_CALL flg, s32 *data);
void CallJimusyo(DDS3_ADMINI_CALL flg, struct SequenceParameter *);
void CallShop(DDS3_ADMINI_CALL flg, struct SequenceParameter *);
s32 CallNewBattle(void);
void CallNewBattleTest(DDS3_ADMINI_CALL flg, struct SBTL_START_DATA *start);
s32 CheckCamp(void);
s32 CheckEpSave(void);
s32 CheckEvent(void);
s32 CheckField(void);
s32 CheckJimusyo(void);
s32 CheckLoad(void);
s32 CheckNewBattleTest(void);
s32 CheckSave(void);
s32 CheckTitle(void);
s32 ExitCamp(void);
s32 ExitEpSave(void);
s32 ExitJimusyo(void);
s32 ExitLoad(void);
s32 ExitNewBattle(void);
s32 ExitNewBattleTest(void);
s32 ExitSave(void);
s32 ExitStaff(void);
s32 ExitShop(void);
s32 CheckShop(void);

#endif
