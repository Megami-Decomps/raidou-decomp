#ifndef SCRTRACECODE_H
#define SCRTRACECODE_H

#include "common.h"
#include "scr.h"

f32 scrGetFloatPara(s32 n);
s32 scrGetIntPara(s32 n);
s32 scrGetLabelCodeAddress(s32 labelindex);
u8 *scrGetStrPara(s32 n);
void scrSetIntReturnValue(s32 i);
s32 scrTraceCode(scrProcessWork_s *pwork);

#endif
