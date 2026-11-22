#ifndef CODE_002F5CF0_H
#define CODE_002F5CF0_H

#include "common.h"
#include "sBtlUnit.h"
#include "sBtlEff.h"
extern sdfVector4_t gTargetPosHokanBuff0;
extern sdfVector4_t gTargetPosHokanBuff1;
extern sdfVector4_t gTargetPosHokanBuff2;
extern sdfVector4_t gTargetPosHokanBuff3;
extern sdfVector4_t gTargetPosHokanBuff4;
extern s32 gTargetPosHokanFastFlag;
extern struct SBTLUNIT_LISTWORK *effQueuedResourceNameRecord7;

/* Battle interface sprite set (0x1C bytes), loaded by btlLoadInterfaceSprite. */
typedef struct sbtlInterfaceSpr_t {
    struct sprHandle_t *hSpr;
    s32 field04;
    u8 pad08[0x1C - 0x8];
} sbtlInterfaceSpr_t;
extern sbtlInterfaceSpr_t sbtlData135;
extern u8 sbtlPlayerDataData; /* bullet icon animation counter, 0..23 */
extern char sbtlData175[];
extern s32 DrawTimeData; /* DrawTime frame count */
extern char DrawTimeData2[]; /* "%02d" */
extern const char btlLoadInterfaceSpriteStrBattlePanel[];
extern const char btlLoadInterfaceSpriteStrBattleInterfaceSpr[];
void btlLoadInterfaceSprite(void);
void sbtlDamagePanelE(const sdfVector4_t *pos, s32 dmg, u32 rgba, u32 rgba2);
void sbtlDamagePanelN(const sdfVector4_t *pos, s32 dmg, u32 rgba, u32 rgba2, s32 minor);
void sbtlDamagePanelR(const sdfVector4_t *pos, s32 dmg, u32 rgba, u32 rgba2);
void sbtlInitSBtlSub2(void);
extern s32 sbtlDrawProcess_sbtlMenuData[]; /* help number per battle menu entry */
extern char *sbtlDrawProcess_sbtlMenuData2[]; /* .data, not small data */
extern s8 sbtlFuncDecideItem_sbtlMenuData;
extern s8 sbtlFuncDecideItemStockData;
s32 sbtlBtlFuncDecideItemSBtlMenuSub(s32 *a_tab, s32 *a_max);
s32 sbtlBtlFuncDecideItemSBtlMenuSub2(s32 *a_tab, s32 *a_max);
void sbtlBtlDrawProcessSBtlMenuSub(s32 a_tab, s32 a_max, s32 a_sel, s32 a_flag, f32 a_alphaRate);

#endif
