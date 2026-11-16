#ifndef SDFFILEIO_H
#define SDFFILEIO_H

#include "common.h"
#include "libsdf.h"

s32 sdfFileCmdClearError(sdfFileCmd_t *aHandle);
s32 sdfFileCmdClose(void *aHandle);
s32 sdfFileCmdFileSize(void *aHandle);
s32 sdfFileCmdFree(sdfFileCmd_t *aHandle);
s32 sdfFileCmdRead(void *aHandle, void *aBuffer, s32 aLength);
s32 sdfFileCmdSeek(void *aHandle, s32 aOffset, s32 aWhence);
void sdfFileCmdStartThread(s32 deviceType);
void sdfFileCmdThreadCheckPrio(void);
void sdfFileCmdThreadSetHiPrio(void);
void sdfFileCmdThreadSetLoPrio(void);

#endif
