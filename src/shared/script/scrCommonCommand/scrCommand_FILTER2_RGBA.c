#include "shared/script/scrCommonCommand/decls.h"

s32 scrCommand_FILTER2_RGBA(void)
{
    s32 byte2;
    s32 byte0;
    s32 byte3;
    s32 byte1;

    byte0 = scrGetIntPara(0);
    byte3 = scrGetIntPara(3);
    byte2 = scrGetIntPara(2);
    byte1 = scrGetIntPara(1);
    dds3SetFilter2RGBA(((byte0 & 0xFF) | (byte3 << 24)) |
                       (((byte2 & 0xFF) << 16) | ((byte1 & 0xFF) << 8)));
    return 1;
}
