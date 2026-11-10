#include "shared/script/scrCommonCommand/decls.h"

s32 scrCommand_SYNC(void)
{
    return scrGetCommandTimer() != 0;
}
