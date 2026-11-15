#ifndef SDFDEV9_H
#define SDFDEV9_H

#include "common.h"

s32 sdfBcdToDecimal(s32 aBcd);
char *sdfStrdupChip(const char *text);
u128 *sdfZeroMemQuad(u128 *dst, s32 count);

#endif
