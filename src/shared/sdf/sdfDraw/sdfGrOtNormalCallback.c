#include "shared/sdf/sdfDraw/decls.h"

s32 sdfGrOtNormalCallback(sdfGrOt_t *aOt, s32 aFunc, sdfDrawTag_t *aData) {
    if (aFunc == 1) {
        sdfGrOtFuncLinkToHead(aOt, aData);
    }
}
