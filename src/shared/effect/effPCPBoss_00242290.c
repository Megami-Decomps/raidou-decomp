#include "common.h"
#include "sdfFrame.h"

extern s32 effPadVibeFrames[2], effPadVibeRemain[2];
extern u8 effPadVibePower[2], effPadVibeFadeIn[2];

void effPadVibeFadeOUT(u32 mode, u8 power, s32 frame)
{
    if (mode < 2) {
        effPadVibeFrames[mode] = SDF_FRAMES(frame);
        effPadVibePower[mode] = power;
        effPadVibeFadeIn[mode] = 0;
        effPadVibeRemain[mode] = SDF_FRAMES(frame);
    }
}

void effPadVibeFadeIN(u32 mode, u8 power, s32 frame)
{
    if (mode < 2) {
        effPadVibeFrames[mode] = SDF_FRAMES(frame);
        effPadVibePower[mode] = power;
        effPadVibeFadeIn[mode] = 1;
        effPadVibeRemain[mode] = SDF_FRAMES(frame);
    }
}
