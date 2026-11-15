#ifndef SDFCD_H
#define SDFCD_H

#include "common.h"

s32 sdfDevOpenDiscFileAndGetSize(const char *filename);
s32 sdfCdImageRead(void *buff, s32 length);
s32 sdfCdImageGetCurrFileSize(void);
void sdfCdImageClose(void);
void sdfDevWaitForDisc(void);

#endif
