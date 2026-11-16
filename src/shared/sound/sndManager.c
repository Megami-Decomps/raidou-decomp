#include "common.h"
#include "sndManager.h"
#include "sdk_libc.h"

char *sdfSndMakeFilename(char *aDstName, const char *aSrcName)
{
    memcpy(aDstName, sndPathPrefix, 8);
    return strcat(aDstName, aSrcName);
}
