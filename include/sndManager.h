#ifndef SND_MANAGER_H
#define SND_MANAGER_H

#include "libsdf.h"
#include "titleGlobal.h"

extern char sndPathPrefix[]; /* "/sound/" */

char *sdfSndMakeFilename(char *aDstName, const char *aSrcName);

#endif
