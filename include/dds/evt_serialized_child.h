#ifndef DDS_EVT_SERIALIZED_CHILD_H

#define DDS_EVT_SERIALIZED_CHILD_H
#include "common.h"

/* One child row emitted by the event-viewer track serializer. */
typedef struct EvtSerializedChild {
    u16 groupType;
    u16 start;
    u16 span;
    u16 value;
    u8 opaque08[4];
    s32 body[8];
} EvtSerializedChild;

typedef char EvtSerializedChild_size_check[
    (sizeof(EvtSerializedChild) == 0x2C) ? 1 : -1];

#endif /* DDS_EVT_SERIALIZED_CHILD_H */
