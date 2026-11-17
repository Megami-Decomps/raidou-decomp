#include "common.h"
#include "field.h"
#include "fldDbg.h"
#include "lmap.h"
#include "field.h"
#include "fldDbg.h"
#include "lmap.h"

s32 fldNewGameStartSub(void) {
    fldDbg.encountOn = 1;
    fldDbg.sousaAllMSKOn = 0;
    fldDbg.word50 = 0;
    fldGlobalBase.word240 = 0;
    return getLeaf3(0);
}
