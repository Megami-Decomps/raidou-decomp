#ifndef DDS3PADMANAGER_H
#define DDS3PADMANAGER_H

#include "common.h"

void dds3PadActAllStop(void);

extern s8 dds3PadButtonState[64];
extern u8 dds3PadStickState[8];

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

/* Strongest motor level dds3PadAct passes on. */
#ifdef VERSION_RAIDOU2
#define DDS3_PAD_ACT_LEVEL_MAX 128
#else
#define DDS3_PAD_ACT_LEVEL_MAX 48
#endif
void dds3PadAct(u32 mode, u8 pow, u32 frame);
void dds3RunProcessList(void);
void dds3PadInitialize(void);

#endif
