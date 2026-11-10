#include "common.h"

#include "shared/model/mdlEffect/mdlPibGetChunk.c"

#include "shared/model/mdlEffect/mdlPibGetFirstChunkItem.c"

#include "shared/model/mdlEffect/mdlPibGetNextChunkItem.c"

#include "shared/model/mdlEffect/mdlPibGetChunkItemCount.c"

#include "shared/model/mdlEffect/mdlPibCheckItemType.c"

#include "shared/model/mdlEffect/mdlPibGetEffectCount.c"

#include "shared/model/mdlEffect/mdlPibGetMotionCount.c"

INCLUDE_ASM(const s32, "model/mdlEffect", mdlPibDrawEffectInfo);

INCLUDE_ASM(const s32, "model/mdlEffect", mdlPibDrawTrackInfo);

INCLUDE_ASM(const s32, "model/mdlEffect", mdlPibEditTrackInfo);

#include "shared/model/mdlEffect/mdlCreateEffectResrc_P2A.c"

#include "shared/model/mdlEffect/mdlCreateEffectResrc_D3P.c"

#include "shared/model/mdlEffect/mdlCreateEffectResrc_IPU.c"

#include "shared/model/mdlEffect/mdlDeleteEffectResrc_IPU.c"

#include "shared/model/mdlEffect/mdlStartEffectResrc_IPU.c"

#include "shared/model/mdlEffect/mdlEffectResrcListNew.c"

#include "shared/model/mdlEffect/mdlEffectResrcListFree.c"

#include "shared/model/mdlEffect/mdlCreateEffectCtrlr.c"

#include "shared/model/mdlEffect/mdlEffectDup_P2A.c"

#include "shared/model/mdlEffect/mdlEffectDup_D3P.c"

#include "shared/model/mdlEffect/mdlGetEffectResrc.c"

#include "shared/model/mdlEffect/mdlCreateEffect_P2A_D3P.c"

#include "shared/model/mdlEffect/mdlCreateEffect_TRACK.c"

#include "shared/model/mdlEffect/mdlCreateEffect_BLUR.c"

#include "shared/model/mdlEffect/mdlCreateEffect_IPU.c"

#include "shared/model/mdlEffect/mdlDrawEffect_IPU.c"

INCLUDE_ASM(const s32, "model/mdlEffect", mdlCreateEffect);

#include "shared/model/mdlEffect/mdlGenEffect.c"

#include "shared/model/mdlEffect/mdlFreeEffect.c"

#include "shared/model/mdlEffect/mdlStopEffect.c"

#include "shared/model/mdlEffect/mdlEffect_P2A_D3P_GetPos.c"

#include "shared/model/mdlEffect/mdlLinkEffect.c"

#include "shared/model/mdlEffect/mdlSetEffectColorSub.c"

#include "shared/model/mdlEffect/mdlSetEffectScaleSub.c"

INCLUDE_RODATA(const s32, "model/mdlEffect", D_004B3F50);

INCLUDE_RODATA(const s32, "model/mdlEffect", D_004B3F60);

INCLUDE_RODATA(const s32, "model/mdlEffect", D_004B3F70);

INCLUDE_RODATA(const s32, "model/mdlEffect", D_004B3F80);

INCLUDE_RODATA(const s32, "model/mdlEffect", D_004B3F90);

INCLUDE_RODATA(const s32, "model/mdlEffect", D_004B3FA0);

INCLUDE_RODATA(const s32, "model/mdlEffect", D_004B3FB0);

INCLUDE_RODATA(const s32, "model/mdlEffect", D_004B3FC0);

INCLUDE_RODATA(const s32, "model/mdlEffect", D_004B3FD0);

INCLUDE_RODATA(const s32, "model/mdlEffect", D_004B3FE0);

INCLUDE_RODATA(const s32, "model/mdlEffect", D_004B3FF0);

INCLUDE_RODATA(const s32, "model/mdlEffect", D_004B4000);

INCLUDE_RODATA(const s32, "model/mdlEffect", D_004B4010);

INCLUDE_RODATA(const s32, "model/mdlEffect", D_004B4020);

INCLUDE_SDATA(const s32, "model/mdlEffect", D_004DF5F8);

INCLUDE_SDATA(const s32, "model/mdlEffect", D_004DF600);

INCLUDE_SDATA(const s32, "model/mdlEffect", D_004DF608);

INCLUDE_SDATA(const s32, "model/mdlEffect", D_004DF610);

INCLUDE_SDATA(const s32, "model/mdlEffect", D_004DF618);

INCLUDE_SDATA(const s32, "model/mdlEffect", D_004DF620);

INCLUDE_SDATA(const s32, "model/mdlEffect", D_004DF628);

INCLUDE_SDATA(const s32, "model/mdlEffect", D_004DF630);

INCLUDE_SDATA(const s32, "model/mdlEffect", D_004DF638);

INCLUDE_SDATA(const s32, "model/mdlEffect", D_004DF640);

INCLUDE_SDATA(const s32, "model/mdlEffect", D_004DF648);

INCLUDE_SDATA(const s32, "model/mdlEffect", D_004DF650);

INCLUDE_SDATA(const s32, "model/mdlEffect", D_004DF658);

INCLUDE_SDATA(const s32, "model/mdlEffect", D_004DF660);

INCLUDE_SDATA(const s32, "model/mdlEffect", D_004DF668);

INCLUDE_SDATA(const s32, "model/mdlEffect", D_004DF670);

INCLUDE_SDATA(const s32, "model/mdlEffect", D_004DF678);

INCLUDE_SDATA(const s32, "model/mdlEffect", D_004DF680);

INCLUDE_SDATA(const s32, "model/mdlEffect", D_004DF688);

INCLUDE_SDATA(const s32, "model/mdlEffect", D_004DF690);

