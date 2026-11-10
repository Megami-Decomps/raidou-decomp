#include "shared/sdf/sdfPrint/decls.h"

/* Print text through the bound IOP dev client, or locally when it is unbound. */
void sdfPuts(const char *str) {
    u8 buffer[0x13F];
    u8 *alignedBuffer;
    s32 length;
    if (sdfPrintIopFlag != 0) {
        length = strlen(str);
        if (length != 0) {
            alignedBuffer = EE_ADDR_AS(u8 *, ((EE_ADDR(&buffer[0x3F])) & ~0x3F));
            memcpy(alignedBuffer, str, length);
            sceSifMCallRpc((sceSifMClientData *)&sdfPrintRpcClient.client, 0x7D1, 0, alignedBuffer, length, NULL, 0, NULL, NULL);
        }
    } else {
        scePrintf(sdfPrintStrFmt, str);
    }
}
