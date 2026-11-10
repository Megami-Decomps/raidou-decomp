#include "shared/script/scrCommonCommand/decls.h"

s32 scrCommand_QUAKE_OFF(void)
{
    dds3StopQuake();
    return 1;
}
