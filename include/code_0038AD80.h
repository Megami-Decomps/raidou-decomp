#ifndef CODE_0038AD80_H
#define CODE_0038AD80_H

#include "common.h"
#include "enums.h"
#include "kernel.h"

struct tagCMP_WORK;
void mapAutoMapDraw3(void);
s32 cmpSetupCmpItemSub(CMP_SPR a_no);
void End_cmpItem(void);
struct tagCMP_WORK *cmpGetStockPID(void);
dds3ProcessID_t *cmpGetCharPID(void);
dds3ProcessID_t *cmpPartyDrawFuncSub(void);
dds3ProcessID_t *cmpFunc7(void);
void cmpUpdateSkillHDCmpKudaSub(u8 a_flag);
void cmpUpdateSkillHDCmpKudaSub2(u8 a_flag);
void cmpUpdateSkillHDCmpKudaSub3(s8 a_flag);
void cmpUpdateSkillHDCmpKudaSub4(u8 a_flag);
extern const char ProcessStart_cmpMapStrCampAutomapMain[];
extern const char ProcessStart_cmpMapStrCampAutomapDraw[];
/* Names indexed by id below 36; cmpData49 ("") for the rest. */
extern const char *cmpDrawPartsOmoideSkillSub3Data[];
extern const char cmpData49[];
const char *cmpDrawPartsOmoideSkillSub3(u32 id);
struct sprHandle_t;
struct itfSprList_t;
void cmpLeaf49(s32 a_sprNo, s32 a_x, s32 a_y, u32 a_alphaRate, struct sprHandle_t *a_hSpr);
extern struct itfSprList_t cmpDrawKeyHelp2Data[];
void cmpDrawKeyHelp2(u8 a_alphaRate, DDS3_OT a_otNo);
extern s8 cmpCallGetProcessData25Data;
extern struct itfSprList_t cmpCallGetSpriteHnd7Data[];
extern char cmpCallGetSpriteHnd2Data2[]; /* "%7d" */
/* Status screen layers, each drawn at an alpha rate. */
void cmpCallSetOt3(s32 a_alphaRate);
void cmpCallSetOt4(s32 a_alphaRate);
void cmpCallSetOt5(s32 a_alphaRate);
void cmpDrawRaidouSkill(s32 a_alphaRate);

#endif
