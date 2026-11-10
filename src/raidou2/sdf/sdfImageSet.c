#include "common.h"

INCLUDE_ASM(const s32, "sdf/sdfImageSet", sdfImageDataSize);

#include "shared/sdf/sdfImageSet/sdfImageSetReq.c"

#include "shared/sdf/sdfImageSet/sdfImageSetChainTag.c"

#include "shared/sdf/sdfImageSet/sdfImageSetChainReleaseMem.c"

#include "shared/sdf/sdfImageSet/sdfImageSetClearCb.c"

#include "shared/sdf/sdfImageSet/sdfImageSetDmacHandler.c"

INCLUDE_ASM(const s32, "sdf/sdfImageSet", sdfImageSetTrans);

#include "shared/sdf/sdfImageSet/sdfImageSetInit.c"
