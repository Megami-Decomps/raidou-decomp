#ifndef DDS_BILL_LINKED_VALUE_H

#define DDS_BILL_LINKED_VALUE_H
#include "common.h"

typedef struct BillLinkedValue {
    s32 opaque00;
    s32 value;
} BillLinkedValue;

typedef char BillLinkedValue_size[
    (sizeof(BillLinkedValue) == 8) ? 1 : -1];

#endif
