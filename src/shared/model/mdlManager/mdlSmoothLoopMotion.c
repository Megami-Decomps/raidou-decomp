#include "shared/model/mdlManager/decls.h"

void mdlSmoothLoopMotion(mdlObj_t *aHandle, s32 aGroup, s32 aNumber, f32 aBeforeLeng,
                         f32 aSmoothLeng)
{
    mdlMotion(aHandle, aGroup, aNumber, 1, aBeforeLeng, aSmoothLeng);
}
