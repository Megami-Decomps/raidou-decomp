#ifndef DDS_SDF_KERNEL_THREAD_PARAM_H

#define DDS_SDF_KERNEL_THREAD_PARAM_H
#include "common.h"

/* Native EE CreateThread descriptor. The observed sound-worker setup reads or
 * writes the first six slots; retain the remaining descriptor bytes opaque. */
typedef struct SdfKernelThreadParam {
    s32 status;             /* 0x00 */
    void (*entry)(void);    /* 0x04 */
    void *stack;            /* 0x08 */
    s32 stackSize;          /* 0x0C */
    void *gp;               /* 0x10 */
    s32 priority;           /* 0x14 */
    u8 opaque18[0x18];      /* 0x18 */
} SdfKernelThreadParam;

typedef char SdfKernelThreadParam_size_must_be_0x30[
    (sizeof(SdfKernelThreadParam) == 0x30) ? 1 : -1];

#endif
