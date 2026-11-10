#include "common.h"

INCLUDE_ASM(const s32, "sdf/sdfImageSet", sdfImageDataSize);

#include "shared/sdf/sdfImageSet/sdfImageSetReq.c"

#include "shared/sdf/sdfImageSet/sdfImageSetChainTag.c"

#include "shared/sdf/sdfImageSet/sdfImageSetChainReleaseMem.c"

#include "shared/sdf/sdfImageSet/sdfImageSetClearCb.c"

#include "shared/sdf/sdfImageSet/sdfImageSetDmacHandler.c"

#include "shared/sdf/sdfImageSet/sdfImageSetTrans.c"

#include "shared/sdf/sdfImageSet/sdfImageSetInit.c"
