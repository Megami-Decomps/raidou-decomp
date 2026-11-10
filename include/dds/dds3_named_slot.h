#ifndef DDS_DDS3_NAMED_SLOT_H

#define DDS_DDS3_NAMED_SLOT_H
#include "common.h"

#include "kwln.h"

#if !defined(SMT3_DEFINES_fclNamedSlot_t)
/* Linked named-task slots shared by the DDS1 and DDS2 kernel windows. */
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
#endif

typedef char fclNamedSlot_size_must_be_44[(sizeof(fclNamedSlot_t) == 0x44) ? 1 : -1];

#endif /* DDS_DDS3_NAMED_SLOT_H */
