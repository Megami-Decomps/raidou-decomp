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

INCLUDE_RODATA(const s32, "model/mdlEffect", D_0044D7E0);

INCLUDE_RODATA(const s32, "model/mdlEffect", D_0044D7F0);

INCLUDE_RODATA(const s32, "model/mdlEffect", D_0044D800);

INCLUDE_RODATA(const s32, "model/mdlEffect", D_0044D810);

INCLUDE_RODATA(const s32, "model/mdlEffect", D_0044D820);

INCLUDE_RODATA(const s32, "model/mdlEffect", D_0044D830);

INCLUDE_RODATA(const s32, "model/mdlEffect", D_0044D840);

INCLUDE_RODATA(const s32, "model/mdlEffect", D_0044D850);

INCLUDE_RODATA(const s32, "model/mdlEffect", D_0044D860);

INCLUDE_RODATA(const s32, "model/mdlEffect", D_0044D870);

INCLUDE_RODATA(const s32, "model/mdlEffect", D_0044D880);

INCLUDE_RODATA(const s32, "model/mdlEffect", D_0044D890);

INCLUDE_RODATA(const s32, "model/mdlEffect", D_0044D8A0);

INCLUDE_RODATA(const s32, "model/mdlEffect", D_0044D8B0);

INCLUDE_SDATA(const s32, "model/mdlEffect", D_0046C398);

INCLUDE_SDATA(const s32, "model/mdlEffect", D_0046C3A0);

INCLUDE_SDATA(const s32, "model/mdlEffect", D_0046C3A8);

INCLUDE_SDATA(const s32, "model/mdlEffect", D_0046C3B0);

INCLUDE_SDATA(const s32, "model/mdlEffect", D_0046C3B8);

INCLUDE_SDATA(const s32, "model/mdlEffect", D_0046C3C0);

INCLUDE_SDATA(const s32, "model/mdlEffect", D_0046C3C8);

INCLUDE_SDATA(const s32, "model/mdlEffect", D_0046C3D0);

INCLUDE_SDATA(const s32, "model/mdlEffect", D_0046C3D8);

INCLUDE_SDATA(const s32, "model/mdlEffect", D_0046C3E0);

INCLUDE_SDATA(const s32, "model/mdlEffect", D_0046C3E8);

INCLUDE_SDATA(const s32, "model/mdlEffect", D_0046C3F0);

INCLUDE_SDATA(const s32, "model/mdlEffect", D_0046C3F8);

INCLUDE_SDATA(const s32, "model/mdlEffect", D_0046C400);

INCLUDE_SDATA(const s32, "model/mdlEffect", D_0046C408);

INCLUDE_SDATA(const s32, "model/mdlEffect", D_0046C410);

INCLUDE_SDATA(const s32, "model/mdlEffect", D_0046C418);

INCLUDE_SDATA(const s32, "model/mdlEffect", D_0046C420);

INCLUDE_SDATA(const s32, "model/mdlEffect", D_0046C428);

INCLUDE_SDATA(const s32, "model/mdlEffect", D_0046C430);

