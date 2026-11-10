#ifndef DDS_KERNEL_H

#define DDS_KERNEL_H
#include "../kernel.h"
#include "common.h"

/* Shared 0x24-byte administration task state. The task creators in both titles
 * allocate this record and pass it to the mode-transition callbacks below. */
typedef u32 Dds3AdminControlFlags;

#endif /* DDS_KERNEL_H */
