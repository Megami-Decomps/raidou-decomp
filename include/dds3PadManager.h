#ifndef DDS3PADMANAGER_H
#define DDS3PADMANAGER_H

#include "common.h"

void dds3PadIsTypeCD(void);

#include "kernel.h"

/* Called after linked children receive immediate kill requests and the task
 * leaves scheduler queues, while its allocation is still valid; the scheduler
 * frees it after the callback returns. */
typedef void (*dds3ProcessShutdownFunc_t)(dds3ProcessID_t *task);

/* Linked named-task slots of the kernel windows. */
typedef struct fclNamedSlot_t {
    s32 kind;               /* 0x00 */
    char name[32];          /* 0x04 */
    dds3ProcessID_t *task;         /* 0x24: task passed to scheduler APIs. */
    dds3ProcessFunc_t update; /* 0x28 */
    dds3ProcessShutdownFunc_t destroy; /* 0x2C */
    void (*setState)(s32);  /* 0x30: state callback. */
    s32 enabled;            /* 0x34 */
    void *userValue;        /* 0x38 */
    s32 previous;           /* 0x3C: previous slot index. */
    s32 next;               /* 0x40: next slot index. */
} fclNamedSlot_t;

typedef char fclNamedSlot_size_must_be_44[(sizeof(fclNamedSlot_t) == 0x44) ? 1 : -1];

#endif
