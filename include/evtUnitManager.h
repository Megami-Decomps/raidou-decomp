#ifndef EVTUNITMANAGER_H
#define EVTUNITMANAGER_H

#include "common.h"
#include "basic.h"
#include "event.h"

s32 evtCheckUnitWait(evtUnit_t *h);
s32 evtDeleteUnitHandle(evtUnit_t *h);
evtUnit_t *evtGetUnitHandle(s32 serial);
void evtLinkLightObjectBasic(void *h, dds3Basic_t *light, s32 frame);
void evtRunUnitKutiMotion(addr_t h);
void evtSetUnitColorA(evtUnit_t *h, s32 frame, u32 a);
void evtSetUnitColorRGB(evtUnit_t *h, s32 frame, u32 rgb);
void evtSetUnitKutiMotion(evtUnit_t *h, s32 group, s32 motion);
void evtSetUnitLightAndAmbient(evtUnit_t *h, s32 frame, u32 lrgb, u32 argb);
void evtSetUnitLightDir(evtUnit_t *h, s32 frame);
void evtSetUnitMotion(evtUnit_t *h, s32 motion, s32 before, s32 smooth, s32 mode);
void evtSetUnitMove3(evtUnit_t *h, s32 pathID, s32 para1, s32 para2, s32 movesys, s32 movedir, s32 rotsys);
void evtSetUnitRot2(evtUnit_t *h, s32 posID, s32 para1);
u8 evtUnitLightAnimeStat(evtUnit_t *h);
void evtUnitLightOff(evtUnit_t *h, s32 frame);
void evtUnitLightOn(evtUnit_t *h);

#endif
