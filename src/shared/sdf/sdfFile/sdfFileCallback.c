#include "shared/sdf/sdfFile/decls.h"

s32 sdfFileCallback(void *aCmd, s32 aOp, s32 aArg, s32 aResult)
{
    switch (aOp) {
    case 4:
        sdfFileLengthWork = aResult;
        break;
    case 3:
        sdfFileSeekWork = aResult;
        break;
    }
    return SignalSema(sdfFileSemaId);
}
