#include "common.h"

#include "shared/sdf/sdfTex/sdfTexGetPacketLength.c"

#include "shared/sdf/sdfTex/sdfTexGetPacketCtx2.c"

#include "shared/sdf/sdfTex/sdfTexGetPacketCtx2Length.c"

#include "shared/sdf/sdfTex/sdfTexGetCLUTCount.c"

#include "shared/sdf/sdfTex/sdfTexGetCLUTAddr.c"

#include "shared/sdf/sdfTex/sdfTexGetPixelAddr.c"

INCLUDE_ASM(const s32, "sdf/sdfTex", sdfTexGetPixelBitCount);

#include "shared/sdf/sdfTex/sdfTexGetTexelBitCount.c"

#include "shared/sdf/sdfTex/sdfTexGetTex0Reg.c"

#include "shared/sdf/sdfTex/sdfTexGetTex1Reg.c"

#include "shared/sdf/sdfTex/sdfTexGetClampReg.c"

#include "shared/sdf/sdfTex/sdfTexSetFilter.c"

#include "shared/sdf/sdfTex/sdfTexSetFilterCtx2.c"

#include "shared/sdf/sdfTex/sdfTexSetWrapMode.c"

#include "shared/sdf/sdfTex/sdfTexTransData.c"

#include "shared/sdf/sdfTex/sdfTexTransCLUT.c"

#include "shared/sdf/sdfTex/sdfTexClutToGs.c"

#include "shared/sdf/sdfTex/sdfTexLink.c"

INCLUDE_ASM(const s32, "sdf/sdfTex", func_003591B8);

INCLUDE_ASM(const s32, "sdf/sdfTex", func_00359290);

INCLUDE_ASM(const s32, "sdf/sdfTex", func_003593E0);

INCLUDE_ASM(const s32, "sdf/sdfTex", func_003594B8);

INCLUDE_ASM(const s32, "sdf/sdfTex", sdfTexGetClutDataSize);

#include "shared/sdf/sdfTex/sdfTexAllocClutBuff.c"

#include "shared/sdf/sdfTex/sdfTexCopyClutData.c"

INCLUDE_ASM(const s32, "sdf/sdfTex", func_00359620);

INCLUDE_ASM(const s32, "sdf/sdfTex", func_00359798);

INCLUDE_ASM(const s32, "sdf/sdfTex", func_00359910);

INCLUDE_ASM(const s32, "sdf/sdfTex", func_00359998);

INCLUDE_ASM(const s32, "sdf/sdfTex", func_003599E0);

INCLUDE_ASM(const s32, "sdf/sdfTex", sdfTexSetPacket);

#include "shared/sdf/sdfTex/sdfTexMakeGIFPacketSub.c"

#include "shared/sdf/sdfTex/sdfTexMakeGIFPacket.c"

#include "shared/sdf/sdfTex/sdfTexMakeGIFPacketCtx2.c"

#include "shared/sdf/sdfTex/sdfTexRebuildPacket.c"

INCLUDE_ASM(const s32, "sdf/sdfTex", func_00359DA8);

INCLUDE_ASM(const s32, "sdf/sdfTex", func_00359F68);

INCLUDE_ASM(const s32, "sdf/sdfTex", func_0035A128);

INCLUDE_ASM(const s32, "sdf/sdfTex", func_0035A140);

INCLUDE_ASM(const s32, "sdf/sdfTex", func_0035A158);

#include "shared/sdf/sdfTex/sdfTexSetMonoClut.c"

INCLUDE_ASM(const s32, "sdf/sdfTex", func_0035A280);

#include "shared/sdf/sdfTex/sdfTexSearch.c"

#include "shared/sdf/sdfTex/sdfTexInit.c"
