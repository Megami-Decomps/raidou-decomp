#ifndef DDS3ADMINIPROCESS_H
#define DDS3ADMINIPROCESS_H

#include "common.h"

void dds3AdminiCallSequence(s32 sequence, void *data, u32 datasize, s32 restore);
void dds3AdminiExitSequence(void);

#include "kernel.h"

saddr_t dds3AdminiStartSequence(dds3ProcessID_t *id);
void dds3InitAdmini(void);

#endif
