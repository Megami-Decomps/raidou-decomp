#ifndef DDS_DDS3_PROCESS_H

#define DDS_DDS3_PROCESS_H
#include "../kernel.h"
#include "common.h"
#include "../draw3d.h"

struct sdfDrawTag_t;

/* Callback result: 0 retains the callback, -1 finishes the task, and
 * other words select a replacement callback. */
typedef s32 dds3TaskUpdateResult_t;

/* Called after linked children receive immediate kill requests and the task
 * leaves scheduler queues, while its allocation is still valid; the scheduler
 * frees it after the callback returns. */
typedef void (*dds3ProcessShutdownFunc_t)(dds3ProcessID_t *task);

typedef void (*TaskDestroy)(dds3ProcessID_t *task);

#endif /* DDS_DDS3_PROCESS_H */
