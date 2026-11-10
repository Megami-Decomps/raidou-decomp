#ifndef DDS_DDS3_BASIC_ARRAY_CURSOR_H

#define DDS_DDS3_BASIC_ARRAY_CURSOR_H
#include "dds3obj.h"

/* Cursor advancement returns whether cursorIndex refers to a linked entry. */
typedef u32 Dds3BasicArrayCursorResult;

enum {
    DDS3_BASIC_ARRAY_CURSOR_EXHAUSTED = 0,
    DDS3_BASIC_ARRAY_CURSOR_HAS_ENTRY = 1
};

#endif /* DDS_DDS3_BASIC_ARRAY_CURSOR_H */
