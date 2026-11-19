#ifndef CODE_002F5CF0_H
#define CODE_002F5CF0_H

#include "common.h"
#include "sBtlUnit.h"
#include "sBtlEff.h"
extern s32 sbtlTargetCursorData6;
extern struct SBTLUNIT_LISTWORK *effQueuedResourceNameRecord7;

/* Battle interface sprite set (0x1C bytes), loaded by btlLoadInterfaceSprite. */
typedef struct sbtlInterfaceSpr_t {
    struct sprHandle_t *hSpr;
    s32 field04;
    u8 pad08[0x1C - 0x8];
} sbtlInterfaceSpr_t;
extern sbtlInterfaceSpr_t sbtlData135;
extern char sbtlData175[];
extern s32 DrawTimeData; /* DrawTime frame count */
extern char DrawTimeData2[]; /* "%02d" */
extern const char btlLoadInterfaceSpriteStrBattlePanel[];
extern const char btlLoadInterfaceSpriteStrBattleInterfaceSpr[];
void btlLoadInterfaceSprite(void);
void sbtlInitSBtlSub2(void);

#endif
