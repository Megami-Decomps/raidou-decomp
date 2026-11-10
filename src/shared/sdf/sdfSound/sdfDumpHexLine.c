#include "shared/sdf/sdfSound/decls.h"

/* Print size bytes in address-aligned, sixteen-column hexadecimal rows. */
void sdfDumpHexLine(const u8 *data, s32 size) {
    s32 column = 0;
    s32 i;
    while (size > 0) {
        if (column == 0) {
            sdfPrintf(sdfDumpAddrFmt, data);
            column = EE_ADDR(data) & 0xF;
            for (i = column; i > 0; i--) {
                sdfPrintf(sdfDumpPadStr);
            }
        } else if (column == 8) {
            sdfPrintf(sdfDumpMidGapStr);
        } else {
            sdfPrintf(sdfDumpByteSepStr);
        }
        sdfPrintf(sdfDumpByteFmt, *data++);
        column++;
        if (column == 0x10) {
            column = 0;
            sdfPrintf(sdfDumpLineBuf);
        }
        size--;
    }
    if (column == 0) {
        sdfPrintf(sdfDumpLineBuf);
    } else {
        sdfPrintf(sdfDumpEndStr);
    }
}
