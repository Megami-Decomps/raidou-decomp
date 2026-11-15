#ifndef FCLDRAWBACK_H
#define FCLDRAWBACK_H

#include "common.h"

struct sprHandle_t;

void _BackSprite(s32 a_moveType, s32 a_x, s32 a_y, s32 a_z, u8 a_alphaRate, struct sprHandle_t *a_hSpr, s32 a_sprNo, u32 a_otNo);
void fclBackSprite(s32 a_type, u8 a_alphaRate1, u8 a_alphaRate2, struct sprHandle_t *a_hSpr);
void fclCommandHelp(s32 a_cmdID, u8 a_alphaRate);

#endif
