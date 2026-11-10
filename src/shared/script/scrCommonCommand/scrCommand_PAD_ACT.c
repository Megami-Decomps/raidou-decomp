#include "shared/script/scrCommonCommand/decls.h"

s32 scrCommand_PAD_ACT(void)
{
    s32 port;
    s32 motor;

    port = scrGetIntPara(0);
    motor = scrGetIntPara(1);
    dds3PadVibrationSet(port, motor & 0xFF, scrGetIntPara(2));
    return 1;
}
