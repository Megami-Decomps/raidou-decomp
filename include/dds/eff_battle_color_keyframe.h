#ifndef DDS_EFF_BATTLE_COLOR_KEYFRAME_H

#define DDS_EFF_BATTLE_COLOR_KEYFRAME_H
#include "eff.h"

typedef struct EffBattleUnitRgbCommand {
    u32 color;
    u32 startFrame;
    s32 startDurationIndex;
    u32 endFrame;
    s32 endDurationIndex;
} EffBattleUnitRgbCommand;

typedef struct EffBattleUnitColorCommand {
    EffectVectorRequest request;
    u32 firstColor;
    u32 secondColor;
    u32 startFrame;
    s32 startDurationIndex;
    u32 endFrame;
    s32 endDurationIndex;
} EffBattleUnitColorCommand;

typedef char EffBattleUnitRgbCommandSizeCheck[(sizeof(EffBattleUnitRgbCommand) == 0x14) ? 1 : -1];

typedef char EffBattleUnitColorCommandSizeCheck[(sizeof(EffBattleUnitColorCommand) == 0x20) ? 1 : -1];

#endif
