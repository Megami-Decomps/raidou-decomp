#include "common.h"

#include "shared/basic/dds3PathBasic/dds3PathEvalSpline.c"

INCLUDE_ASM(const s32, "basic/dds3PathBasic", dds3AddPathBasic);

#include "shared/basic/dds3PathBasic/dds3AllocPathBasicWorkData.c"

#include "shared/basic/dds3PathBasic/dds3DeletePathControl.c"

#include "shared/basic/dds3PathBasic/dds3PathBasicControlGetPos.c"

#include "shared/basic/dds3PathBasic/dds3PathBasicControlGetRot.c"

#include "shared/basic/dds3PathBasic/dds3PathBasicControlNextStep.c"

#include "shared/basic/dds3PathBasic/dds3PathBasicControlGetFovy.c"

#include "shared/basic/dds3PathBasic/dds3PathBasicControlIsFinished.c"

#include "shared/basic/dds3PathBasic/dds3PathBasicControlAlterMode.c"

#include "shared/basic/dds3PathBasic/dds3PathBasicControlGetMode.c"

#include "shared/basic/dds3PathBasic/dds3PathBasicControlSetRatio.c"

#include "shared/basic/dds3PathBasic/dds3PathBasicControlGetRatio.c"
