#ifndef DDS_PTY_ITEM_QUANTITY_UPDATE_VIEW_H

#define DDS_PTY_ITEM_QUANTITY_UPDATE_VIEW_H
#include "common.h"

/* Reader view: inventory update code consumes two four-byte-stride records.
 * Byte 3 is opaque, and these readers do not establish the table's full extent. */
typedef struct PtyItemQuantityUpdateView {
    u16 itemId;       /* 0x0 */
    u8 quantityDelta; /* 0x2 */
    u8 opaque03;      /* 0x3 */
} PtyItemQuantityUpdateView;

typedef char PtyItemQuantityUpdateView_size_must_be_4[
    (sizeof(PtyItemQuantityUpdateView) == 4) ? 1 : -1];

#endif /* DDS_PTY_ITEM_QUANTITY_UPDATE_VIEW_H */
