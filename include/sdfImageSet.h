#ifndef SDFIMAGESET_H
#define SDFIMAGESET_H

#include "common.h"

s32 sdfImageDataSize(u32 format, s32 width, s32 height);
void sdfImageSetChainReleaseMem(saddr_t p);
void sdfImageSetInit(void);
void sdfImageSetTrans(void);

#endif
