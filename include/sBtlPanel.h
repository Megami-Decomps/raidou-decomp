#ifndef SBTLPANEL_H
#define SBTLPANEL_H

#include "common.h"
#include "enums.h"
#include "kernel.h"

struct sprHandle_t;

void sbtlCancelMenuPara(void);
s32 sbtlCheckSelectAnim(void);
void sbtlDrawSprite_RGBA2(s32 x, s32 y, u32 *rgba, struct sprHandle_t *handle, s32 sprno, DDS3_OT ot);
s32 sbtlGetMenuStatus(void);
void sbtlResetMenuPara(void);
void sbtlResetMidashiAnim(s8 flg);
void sbtlResetSelectAnim(s8 flg);
s32 sbtlSetMenuPara(u16 mode, s32 stock_id, SKLID skillid, s32 itemid, s32 targetid, s32 smnid);
void sbtlSetSelectAnim(s8 flg);

#endif
