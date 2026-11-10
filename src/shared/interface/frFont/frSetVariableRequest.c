#include "shared/interface/frFont/decls.h"

/* Store an unchecked replacement-text address word in the font's indexed table. */
void frSetVariableRequest(s32 VarNo, s32 pStr) {
    gVarStr[VarNo] = pStr;
}
