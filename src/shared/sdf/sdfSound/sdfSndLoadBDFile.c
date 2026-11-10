#include "shared/sdf/sdfSound/decls.h"

u32 sdfSndLoadBDFile(s32 aVcChNum, char *aFilename) {
    u32 length = strlen(aFilename);
    return sdfSndSendSyncCmd(aVcChNum | 0x60, 0, aFilename, length);
}
