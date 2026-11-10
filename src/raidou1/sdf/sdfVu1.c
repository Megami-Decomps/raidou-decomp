#include "common.h"

#include "shared/sdf/sdfVu1/corePktSetupPointBuff.c"

#include "shared/sdf/sdfVu1/corePktInitCb.c"

#include "shared/sdf/sdfVu1/corePktLoadWorldLightMtx.c"

#include "shared/sdf/sdfVu1/corePktMakeFaceDivParm.c"

#include "shared/sdf/sdfVu1/corePktSetFaceDivParm.c"

INCLUDE_ASM(const s32, "sdf/sdfVu1", corePktSkinPointsFirst);

INCLUDE_ASM(const s32, "sdf/sdfVu1", corePktSkinPointsAccum);

INCLUDE_ASM(const s32, "sdf/sdfVu1", corePktSkinVertFirst);

INCLUDE_ASM(const s32, "sdf/sdfVu1", corePktSkinVertAccum);

INCLUDE_ASM(const s32, "sdf/sdfVu1", corePktLoadVerts);

INCLUDE_ASM(const s32, "sdf/sdfVu1", corePktLoadVertsNorms);

INCLUDE_ASM(const s32, "sdf/sdfVu1", corePktMorphVertsNorms);

INCLUDE_ASM(const s32, "sdf/sdfVu1", corePktMorphVerts);

INCLUDE_ASM(const s32, "sdf/sdfVu1", corePktNormalizeNorms);

INCLUDE_ASM(const s32, "sdf/sdfVu1", corePktXfmLightClip);

INCLUDE_ASM(const s32, "sdf/sdfVu1", corePktXfmLightClipPB);

INCLUDE_ASM(const s32, "sdf/sdfVu1", corePktXfmLightClipUV);

INCLUDE_ASM(const s32, "sdf/sdfVu1", corePktXfmLightClipUVPB);

INCLUDE_ASM(const s32, "sdf/sdfVu1", corePktRelightBackFaces);

INCLUDE_ASM(const s32, "sdf/sdfVu1", corePktXfmUV);

#include "shared/sdf/sdfVu1/corePktLerpUVChain.c"

#include "shared/sdf/sdfVu1/corePktLerpUVColorChain.c"

INCLUDE_ASM(const s32, "sdf/sdfVu1", corePktApplyFaceDivColor);

#include "shared/sdf/sdfVu1/corePktSetConstColor.c"

#include "shared/sdf/sdfVu1/corePktMakeEdgePoint.c"

#include "shared/sdf/sdfVu1/corePktClipFace.c"

#include "shared/sdf/sdfVu1/corePktBuildFaceList.c"

#include "shared/sdf/sdfVu1/corePktBuildBackFaceList.c"

INCLUDE_ASM(const s32, "sdf/sdfVu1", corePktEmitTexTris);

#include "shared/sdf/sdfVu1/corePktEmitTexTrisCtx2.c"

INCLUDE_ASM(const s32, "sdf/sdfVu1", corePktEmitTris);

#include "shared/sdf/sdfVu1/corePktEmitBase.c"

#include "shared/sdf/sdfVu1/corePktEmitMultiTexPass.c"

INCLUDE_ASM(const s32, "sdf/sdfVu1", corePktEmitRefMapPass);

#include "shared/sdf/sdfVu1/corePktEmitAllPasses.c"

#include "shared/sdf/sdfVu1/corePktOpenAndEmit.c"

#include "shared/sdf/sdfVu1/corePktClosePacket.c"

INCLUDE_ASM(const s32, "sdf/sdfVu1", corePktVu0Run);
