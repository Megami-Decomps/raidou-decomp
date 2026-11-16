#ifndef EVTUNITMANAGER_H
#define EVTUNITMANAGER_H

#include "common.h"
#include "basic.h"
#include "event.h"

struct mdlObj_t;

s32 evtCheckUnitWait(evtUnit_t *h);
evtUnit_t *evtCreateUnitHandle(dds3Basic_t *basic, struct mdlObj_t *model);
s32 evtDeleteUnitHandle(evtUnit_t *h);
void evtSetUnitExMotion(evtUnit_t *h, s32 group, s32 motion, s32 before, s32 smooth, s32 mode);
evtUnit_t *evtGetUnitHandle(s32 serial);
evtUnit_t *evtGetUnitHandle2(dds3Basic_t *unit);
void evtLinkLightObjectBasic(void *h, dds3Basic_t *light, s32 frame);
void evtUnlinkLightObjectBasic(evtUnit_t *h, s32 frame);
void evtRunUnitKutiMotion(addr_t h);
void evtSetUnitColorA(evtUnit_t *h, s32 frame, u32 a);
void evtSetUnitColorRGB(evtUnit_t *h, s32 frame, u32 rgb);
void evtSetUnitKutiMotion(evtUnit_t *h, s32 group, s32 motion);
void evtSetUnitLightAndAmbient(evtUnit_t *h, s32 frame, u32 lrgb, u32 argb);
void evtSetUnitLightDir(evtUnit_t *h, s32 frame);
void evtSetUnitLightAndAmbient3(evtUnit_t *h, s32 frame, u32 rgb0, u32 rgb1, u32 rgb2, u32 argb);
s32 evtSetUnitLightDir1(evtUnit_t *h, s32 frame);
s32 evtSetUnitLightDir2(evtUnit_t *h, s32 frame);
void evtSetWaitMotionSpeed(evtUnit_t *h, f32 sp);
void evtSetUnitMotion(evtUnit_t *h, s32 motion, s32 before, s32 smooth, s32 mode);
void evtSetUnitMove3(evtUnit_t *h, s32 pathID, s32 para1, s32 para2, s32 movesys, s32 movedir, s32 rotsys);
void evtSetUnitRot2(evtUnit_t *h, s32 posID, s32 para1);
u8 evtUnitLightAnimeStat(evtUnit_t *h);
void evtUnitLightOff(evtUnit_t *h, s32 frame);
void evtUnitLightOn(evtUnit_t *h);

/* A scene light set an event script assigns to units; type 3 binds it to one unit. */
typedef struct CustomSceneLightInfo {
    s32 lightType;
    evtUnit_t *unitHandle;
    f32 color[3][4];
    f32 dir[3][4];
    f32 ambient[4];
    f32 soften;
    f32 bias;
} CustomSceneLightInfo;

SIZE_CHECK(CustomSceneLightInfoSizeCheck, CustomSceneLightInfo, 0x80);

extern CustomSceneLightInfo CustomSceneLightArray[7];

void evtInitCustomSceneLightSub(f32 (*color)[4], f32 (*dir)[4], f32 *ambient, f32 *soften, f32 *bias);
void evtInitCustomSceneLight(void);
void evtSetCustomSceneLightData(s32 index, s32 lightType, evtUnit_t *h, f32 (*color)[4], f32 (*dir)[4], f32 *ambient);
void evtSetCustomSceneLightData_SoftenAndBias(s32 index, f32 soften, f32 bias);
void evtMakeCustomSLightPacket(evtUnit_t *h);
s32 evtGetCustomSLight_SoftenAndBias(evtUnit_t *h, f32 *soften, f32 *bias);
s32 evtUnitManagerPrimaryFunc(dds3PoseSet_t *pose, dds3Basic_t *basic);

extern const sdfVector4_t evtCreateUnitHandleTable;
extern const sdfVector4_t evtCreateUnitHandleTable2;
extern const sdfVector4_t evtCreateUnitHandleTable3;
extern const sdfVector4_t evtUnitRotationTarget_evtTable;
extern const sdfVector4_t evtUnitUpAxis;

#endif
