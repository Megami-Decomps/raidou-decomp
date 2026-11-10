#ifndef DDS_ITF_BIT_RANGE_H

#define DDS_ITF_BIT_RANGE_H
#include "common.h"

/* Adjacent packed-resource offsets delimiting one bitstream entry. */
typedef struct ItfBitRange {
    u32 start;
    u32 end;
} ItfBitRange;

typedef char ItfBitRange_size[(sizeof(ItfBitRange) == 0x08) ? 1 : -1];

#endif
