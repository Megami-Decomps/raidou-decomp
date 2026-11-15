#ifndef SDFFILE_H
#define SDFFILE_H

#include "common.h"

void sdfFileClose(void *aHandle);
char *sdfFileGetHostRoot(void);
u32 sdfFileLength(void *aHandle);
s32 sdfFileRead(void *aHandle, void *aPtr, s32 aLeng);

s32 sdfFileCheckExist(char *aFilename);

#endif
