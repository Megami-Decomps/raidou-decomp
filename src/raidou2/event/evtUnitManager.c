#include "common.h"

#include "shared/event/evtUnitManager/evtVu0SubVectorXZAxis.c"

INCLUDE_ASM(const s32, "event/evtUnitManager", evtGetUnitDirVector);

INCLUDE_ASM(const s32, "event/evtUnitManager", evtCheckMoveEnd);

INCLUDE_ASM(const s32, "event/evtUnitManager", evtGetNextMovePos);

INCLUDE_ASM(const s32, "event/evtUnitManager", GetSplineLength);

INCLUDE_ASM(const s32, "event/evtUnitManager", func_001B9FE0);

INCLUDE_ASM(const s32, "event/evtUnitManager", GetBaseLightRGB);

INCLUDE_ASM(const s32, "event/evtUnitManager", func_001BB3D0);

INCLUDE_ASM(const s32, "event/evtUnitManager", func_001BB480);

INCLUDE_ASM(const s32, "event/evtUnitManager", GetBaseAmbientRGB);

INCLUDE_ASM(const s32, "event/evtUnitManager", func_001BB5E8);

INCLUDE_ASM(const s32, "event/evtUnitManager", func_001BB670);

INCLUDE_ASM(const s32, "event/evtUnitManager", func_001BB6F8);

INCLUDE_ASM(const s32, "event/evtUnitManager", func_001BB780);

INCLUDE_ASM(const s32, "event/evtUnitManager", func_001BB978);

INCLUDE_ASM(const s32, "event/evtUnitManager", func_001BB9E8);

INCLUDE_ASM(const s32, "event/evtUnitManager", func_001BBA50);

INCLUDE_ASM(const s32, "event/evtUnitManager", evtUnlinkExternalLightPacket);

INCLUDE_ASM(const s32, "event/evtUnitManager", evtSetUnitLightAndAmbient);

INCLUDE_ASM(const s32, "event/evtUnitManager", func_001BBB30);

INCLUDE_ASM(const s32, "event/evtUnitManager", func_001BBC20);

INCLUDE_ASM(const s32, "event/evtUnitManager", func_001BBC90);

INCLUDE_ASM(const s32, "event/evtUnitManager", func_001BBD00);

INCLUDE_ASM(const s32, "event/evtUnitManager", evtSetUnitColorRGB);

INCLUDE_ASM(const s32, "event/evtUnitManager", evtSetUnitColorA);

INCLUDE_ASM(const s32, "event/evtUnitManager", evtUnitLightAnimeStat);

INCLUDE_ASM(const s32, "event/evtUnitManager", evtUnitLightOn);

INCLUDE_ASM(const s32, "event/evtUnitManager", evtUnitLightOff);

#include "shared/event/evtUnitManager/evtGetUnitHandle.c"

#include "shared/event/evtUnitManager/evtGetUnitHandle2.c"

INCLUDE_ASM(const s32, "event/evtUnitManager", func_001BC058);

INCLUDE_ASM(const s32, "event/evtUnitManager", evtDeleteUnitHandle);

INCLUDE_ASM(const s32, "event/evtUnitManager", evtGetUnitStat);

INCLUDE_ASM(const s32, "event/evtUnitManager", func_001BC270);

INCLUDE_ASM(const s32, "event/evtUnitManager", func_001BC280);

INCLUDE_ASM(const s32, "event/evtUnitManager", evtSetWaitMotionSpeed);

INCLUDE_ASM(const s32, "event/evtUnitManager", func_001BC290);

INCLUDE_ASM(const s32, "event/evtUnitManager", evtCheckUnitWait);

INCLUDE_ASM(const s32, "event/evtUnitManager", evtSetUnitKutiMotion);

INCLUDE_ASM(const s32, "event/evtUnitManager", func_001BC318);

INCLUDE_ASM(const s32, "event/evtUnitManager", func_001BC348);

INCLUDE_ASM(const s32, "event/evtUnitManager", evtSetUnitMotion);

INCLUDE_ASM(const s32, "event/evtUnitManager", func_001BC4A0);

INCLUDE_ASM(const s32, "event/evtUnitManager", evtUnitRotationQuat);

INCLUDE_ASM(const s32, "event/evtUnitManager", evtUnitMoveType0);

INCLUDE_RODATA(const s32, "event/evtUnitManager", D_004B44C0);

INCLUDE_RODATA(const s32, "event/evtUnitManager", D_004B44D0);

INCLUDE_RODATA(const s32, "event/evtUnitManager", D_004B44E0);

INCLUDE_RODATA(const s32, "event/evtUnitManager", D_004B44F0);

INCLUDE_RODATA(const s32, "event/evtUnitManager", D_004B4500);

INCLUDE_ASM(const s32, "event/evtUnitManager", func_001BC7D0);

INCLUDE_ASM(const s32, "event/evtUnitManager", evtUnitMoveTypePathTrace);

INCLUDE_ASM(const s32, "event/evtUnitManager", func_001BCAB0);

INCLUDE_ASM(const s32, "event/evtUnitManager", evtSetUnitRot1);

INCLUDE_ASM(const s32, "event/evtUnitManager", evtSetUnitRot2);

INCLUDE_ASM(const s32, "event/evtUnitManager", evtSetUnitMove1);

INCLUDE_ASM(const s32, "event/evtUnitManager", evtSetUnitMove2);

INCLUDE_ASM(const s32, "event/evtUnitManager", evtSetUnitMove3);

INCLUDE_ASM(const s32, "event/evtUnitManager", func_001BCEB8);

INCLUDE_ASM(const s32, "event/evtUnitManager", func_001BCEC8);

INCLUDE_ASM(const s32, "event/evtUnitManager", func_001BCF70);

INCLUDE_ASM(const s32, "event/evtUnitManager", func_001BD018);

INCLUDE_ASM(const s32, "event/evtUnitManager", func_001BD040);

INCLUDE_ASM(const s32, "event/evtUnitManager", func_001BD768);

INCLUDE_ASM(const s32, "event/evtUnitManager", func_001BD810);

INCLUDE_ASM(const s32, "event/evtUnitManager", func_001BD8F8);

INCLUDE_ASM(const s32, "event/evtUnitManager", func_001BD928);

INCLUDE_ASM(const s32, "event/evtUnitManager", func_001BDA38);

INCLUDE_ASM(const s32, "event/evtUnitManager", func_001BDC38);
