#include "common.h"

#include "shared/model/mdlManager/mdlInitManager.c"

#include "shared/model/mdlManager/mdlSearchResrc.c"

#include "shared/model/mdlManager/mdlCheckLoading.c"

#include "shared/model/mdlManager/mdlRegLoading.c"

#include "shared/model/mdlManager/mdlUnRegLoading.c"

#include "shared/model/mdlManager/mdlRegist.c"

#include "shared/model/mdlManager/mdlUnRegistResrc.c"

#include "shared/model/mdlManager/mdlUnRegist.c"

#include "shared/model/mdlManager/mdlUnRegistAll.c"

#include "shared/model/mdlManager/mdlCreateMotion.c"

#include "shared/model/mdlManager/mdlFreeMotionCtrlr.c"

#include "shared/model/mdlManager/mdlFreeMotion.c"

#include "shared/model/mdlManager/mdlUnRegistResrcMotion.c"

#include "shared/model/mdlManager/mdlUnRegistMotion.c"

#include "shared/model/mdlManager/mdlRegistMotion.c"

#include "shared/model/mdlManager/mdlRegistModelSet.c"

#include "shared/model/mdlManager/mdlWaitLoadEnd.c"

#include "shared/model/mdlManager/TbMbAbLoadEnd.c"

#include "shared/model/mdlManager/TbLoadCallback.c"

#include "shared/model/mdlManager/MbLoadCallback.c"

#include "shared/model/mdlManager/AbLoadCallback.c"

#include "shared/model/mdlManager/mdlSetFileName.c"

#include "shared/model/mdlManager/mdlPrepare.c"

#include "shared/model/mdlManager/mdlLoadPrepare.c"

#include "shared/model/mdlManager/mdlUnlinkResrc.c"

#include "shared/model/mdlManager/mdlSetBlur.c"

#include "shared/model/mdlManager/mdlFreeBlur.c"

#include "shared/model/mdlManager/mdlLinkBlur.c"

#include "shared/model/mdlManager/mdlLoad.c"

#include "shared/model/mdlManager/mdlFree.c"

#include "shared/model/mdlManager/mdlLink.c"

INCLUDE_ASM(const s32, "model/mdlManager", func_0019A6A8);

INCLUDE_ASM(const s32, "model/mdlManager", func_0019A748);

#include "shared/model/mdlManager/mdlHideAllNode.c"

#include "shared/model/mdlManager/mdlMotion.c"

#include "shared/model/mdlManager/mdlLoopMotion.c"

#include "shared/model/mdlManager/mdlSingleMotion.c"

#include "shared/model/mdlManager/mdlSmoothLoopMotion.c"

#include "shared/model/mdlManager/mdlSmoothSingleMotion.c"

#include "shared/model/mdlManager/mdlGetMotionCtrlr.c"

#include "shared/model/mdlManager/mdlGetMotionNum.c"

#include "shared/model/mdlManager/mdlGetMotionLengthEx.c"

#include "shared/model/mdlManager/mdlGetMotionFrameEx.c"

#include "shared/model/mdlManager/mdlChkMotionEndEx.c"

#include "shared/model/mdlManager/mdlGetMotionSpeedEx.c"

#include "shared/model/mdlManager/mdlSetMotionSpeedEx.c"

#include "shared/model/mdlManager/mdlGetPosition.c"

#include "shared/model/mdlManager/mdlSetPosition.c"

#include "shared/model/mdlManager/mdlGetRotate.c"

#include "shared/model/mdlManager/mdlSetRotate.c"

#include "shared/model/mdlManager/mdlGetScale.c"

#include "shared/model/mdlManager/mdlSetScale.c"

#include "shared/model/mdlManager/mdlGetColor.c"

#include "shared/model/mdlManager/mdlSetEffectColor.c"

#include "shared/model/mdlManager/mdlSetColor.c"

#include "shared/model/mdlManager/mdlSetColorWithEffect.c"

#include "shared/model/mdlManager/mdlSetEffectScale.c"

#include "shared/model/mdlManager/mdlGetScreenOffset.c"

INCLUDE_ASM(const s32, "model/mdlManager", func_0019AFC8);

#include "shared/model/mdlManager/mdlMotionPause.c"

#include "shared/model/mdlManager/mdlMotionPauseOff.c"

#include "shared/model/mdlManager/mdlHasMotionEx.c"

#include "shared/model/mdlManager/mdlGetMajorNum.c"

#include "shared/model/mdlManager/mdlGetMinorNum.c"

INCLUDE_ASM(const s32, "model/mdlManager", mdlGetNumOfMajor);

#include "shared/model/mdlManager/mdlGetNumOfMinor.c"

#include "shared/model/mdlManager/mdlGetNumOfCut.c"

#include "shared/model/mdlManager/mdlSetClutColor.c"

#include "shared/model/mdlManager/mdlGetFilebase.c"

#include "shared/model/mdlManager/mdlPackCheck.c"

#include "shared/model/mdlManager/mdlPackSetOld.c"

#include "shared/model/mdlManager/mdlPackSet.c"

#include "shared/model/mdlManager/mdlFileCbFree.c"

#include "shared/model/mdlManager/mdlFileLoadCallback.c"

#include "shared/model/mdlManager/mdlFileLoad.c"

#include "shared/model/mdlManager/mdlPackDecode.c"

#include "shared/model/mdlManager/mdlFreeFileItem.c"

#include "shared/model/mdlManager/mdlReleaseHddFileList.c"

#include "shared/model/mdlManager/mdlMakeHddFileListDummy.c"

INCLUDE_ASM(const s32, "model/mdlManager", mdlMakeHddFileList);
INCLUDE_SDATA(const s32, "model/mdlManager", D_0046C378);

INCLUDE_SDATA(const s32, "model/mdlManager", D_0046C380);

INCLUDE_SDATA(const s32, "model/mdlManager", D_0046C384);

INCLUDE_SDATA(const s32, "model/mdlManager", D_0046C388);

INCLUDE_SDATA(const s32, "model/mdlManager", D_0046C390);

