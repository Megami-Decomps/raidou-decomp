#include "shared/mc/slMain/decls.h"

saddr_t slSaveProc_writeFile(void) {
    slMcResult_t status = sdfMcFuncWrite1();
    if (status == SL_MC_RESULT_PENDING) {
        return 0;
    }
    if (status == SL_MC_RESULT_SUCCESS) {
        sdfMcFuncClose(slMcFd);
        return EE_ADDR_S(slSaveProc_closeFile);
    }
    if (status == SL_MC_RESULT_ERROR) {
        slSetInfo(0);
        gslErrorCode = 4;
        return EE_ADDR_S(slErrorWait);
    }
    return 0;
}
