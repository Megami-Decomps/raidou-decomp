#ifndef DDS3ADMINIPROCESS_H
#define DDS3ADMINIPROCESS_H

#include "common.h"

void dds3AdminiCallSequence(s32 sequence, void *data, u32 datasize, s32 restore);
void dds3AdminiExitSequence(void);

#endif
