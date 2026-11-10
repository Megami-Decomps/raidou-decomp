#include "common.h"

#include "shared/sdf/sdfTex/sdfTexGetPacketLength.c"

#include "shared/sdf/sdfTex/sdfTexGetPacketCtx2.c"

#include "shared/sdf/sdfTex/sdfTexGetPacketCtx2Length.c"

#include "shared/sdf/sdfTex/sdfTexGetCLUTCount.c"

#include "shared/sdf/sdfTex/sdfTexGetCLUTAddr.c"

#include "shared/sdf/sdfTex/sdfTexGetPixelAddr.c"

INCLUDE_ASM(const s32, "sdf/sdfTex", func_003CEBD8);

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

INCLUDE_ASM(const s32, "sdf/sdfTex", func_003CEF40);

INCLUDE_ASM(const s32, "sdf/sdfTex", func_003CF018);

INCLUDE_ASM(const s32, "sdf/sdfTex", func_003CF168);

INCLUDE_ASM(const s32, "sdf/sdfTex", func_003CF240);

INCLUDE_ASM(const s32, "sdf/sdfTex", sdfTexGetClutDataSize);

#include "shared/sdf/sdfTex/sdfTexAllocClutBuff.c"

#include "shared/sdf/sdfTex/sdfTexCopyClutData.c"

INCLUDE_ASM(const s32, "sdf/sdfTex", func_003CF3A8);

INCLUDE_ASM(const s32, "sdf/sdfTex", func_003CF520);

INCLUDE_ASM(const s32, "sdf/sdfTex", func_003CF698);

INCLUDE_ASM(const s32, "sdf/sdfTex", func_003CF720);

INCLUDE_ASM(const s32, "sdf/sdfTex", func_003CF768);

INCLUDE_ASM(const s32, "sdf/sdfTex", sdfTexSetPacket);

#include "shared/sdf/sdfTex/sdfTexMakeGIFPacketSub.c"

#include "shared/sdf/sdfTex/sdfTexMakeGIFPacket.c"

#include "shared/sdf/sdfTex/sdfTexMakeGIFPacketCtx2.c"

#include "shared/sdf/sdfTex/sdfTexRebuildPacket.c"

INCLUDE_ASM(const s32, "sdf/sdfTex", func_003CFB30);

INCLUDE_ASM(const s32, "sdf/sdfTex", func_003CFCF0);

INCLUDE_ASM(const s32, "sdf/sdfTex", func_003CFEB0);

INCLUDE_ASM(const s32, "sdf/sdfTex", func_003CFEC8);

INCLUDE_ASM(const s32, "sdf/sdfTex", func_003CFEE0);

#include "shared/sdf/sdfTex/sdfTexSetMonoClut.c"

INCLUDE_ASM(const s32, "sdf/sdfTex", func_003D0008);

#include "shared/sdf/sdfTex/sdfTexSearch.c"

INCLUDE_ASM(const s32, "sdf/sdfTex", sdfTexInit);
