#include "shared/mc/slMain/decls.h"

saddr_t slFormatStart(void) {
    slSetInfo(5);
    dds3SoftResetOff();
    sdfMcFuncFormat(mccb);
    return EE_ADDR_S(slProc_Format);
}
