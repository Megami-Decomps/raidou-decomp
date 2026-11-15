#ifndef SDK_LIBIPU_H
#define SDK_LIBIPU_H

#include "common.h"

void sceIpuInit(void);
u32 sceIpuSync(s32 mode, u16 timeout);

#endif
