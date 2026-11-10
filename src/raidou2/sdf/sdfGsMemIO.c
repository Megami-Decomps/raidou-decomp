#include "common.h"

#include "shared/sdf/sdfGsMemIO/sdfSetStoreImage.c"

#include "shared/sdf/sdfGsMemIO/sdfLinkStoreImage.c"

#include "shared/sdf/sdfGsMemIO/sdfLinkStoreFrameImagePredraw.c"

INCLUDE_ASM(const s32, "sdf/sdfGsMemIO", sdfLinkStoreFrameImage);

#include "shared/sdf/sdfGsMemIO/sdfSetLoadImage.c"

#include "shared/sdf/sdfGsMemIO/sdfLinkLoadImage.c"
