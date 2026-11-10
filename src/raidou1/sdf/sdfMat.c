#include "common.h"

#include "shared/sdf/sdfMat/sdfTexListNew.c"

#include "shared/sdf/sdfMat/sdfTexListFree.c"

#include "shared/sdf/sdfMat/sdfTexListAdd.c"

#include "shared/sdf/sdfMat/sdfTexListSetCapacity.c"

#include "shared/sdf/sdfMat/sdfTexListDup.c"

#include "shared/sdf/sdfMat/sdfTexListSetClutColor.c"

INCLUDE_ASM(const s32, "sdf/sdfMat", sdfMatInitSystem);

#include "shared/sdf/sdfMat/sdfMatListNew.c"

#include "shared/sdf/sdfMat/sdfMatListFree.c"

#include "shared/sdf/sdfMat/sdfMatListFreeResrc.c"

#include "shared/sdf/sdfMat/sdfMatListGrow.c"

#include "shared/sdf/sdfMat/sdfMatListAdd.c"

#include "shared/sdf/sdfMat/sdfMatSetAmbient.c"

#include "shared/sdf/sdfMat/sdfMatSetDiffuse.c"

#include "shared/sdf/sdfMat/sdfMatSetSpecular.c"

#include "shared/sdf/sdfMat/sdfMatSetRefMap.c"

#include "shared/sdf/sdfMat/sdfMatSetConstant.c"

#include "shared/sdf/sdfMat/sdfMatSetTex.c"

#include "shared/sdf/sdfMat/sdfMatAllocTexUvwParm.c"

#include "shared/sdf/sdfMat/sdfMatGetTexUvwParm.c"

#include "shared/sdf/sdfMat/sdfMatSetTexUvw.c"

#include "shared/sdf/sdfMat/sdfMatSetTexUvwByPtr.c"

#include "shared/sdf/sdfMat/sdfMatSetMultiDiffuse.c"

#include "shared/sdf/sdfMat/sdfMatSetMultiMethod.c"

#include "shared/sdf/sdfMat/sdfMatSetMultiTex.c"

#include "shared/sdf/sdfMat/sdfMatGetMultiTexUvwParm.c"

#include "shared/sdf/sdfMat/sdfMatSetMultiTexUvw.c"

#include "shared/sdf/sdfMat/sdfMatSetMultiTexUvwByPtr.c"

#include "shared/sdf/sdfMat/sdfMatSetFaceDivParm.c"

#include "shared/sdf/sdfMat/sdfMatNew.c"

#include "shared/sdf/sdfMat/sdfMatSet.c"

#include "shared/sdf/sdfMat/sdfMatFree.c"

#include "shared/sdf/sdfMat/sdfMatFreeDelayed.c"

#include "shared/sdf/sdfMat/sdfMatMakePacket.c"

INCLUDE_ASM(const s32, "sdf/sdfMat", func_00361A78);

#include "shared/sdf/sdfMat/sdfMatSetUvwParm.c"

#include "shared/sdf/sdfMat/sdfMatUpdatePacketUvwParm.c"

#include "shared/sdf/sdfMat/sdfMatUpdatePacketMultiTex.c"

#include "shared/sdf/sdfMat/sdfMatUpdatePacketFaceDiv.c"

#include "shared/sdf/sdfMat/sdfMatUpdatePacket.c"

INCLUDE_ASM(const s32, "sdf/sdfMat", func_00361D98);

#include "shared/sdf/sdfMat/sdfMatListMake.c"

INCLUDE_ASM(const s32, "sdf/sdfMat", func_00361F20);

INCLUDE_ASM(const s32, "sdf/sdfMat", func_00361F80);

#include "shared/sdf/sdfMat/sdfMatCopyStat.c"

#include "shared/sdf/sdfMat/sdfMatListCopyStat.c"
