#include "shared/model/mdlEffect/decls.h"

/* Read the PIB motion count from its packed halfword. */
u16 mdlPibGetMotionCount(const pibHeaderChunk_t *record) {
    return record->tail.half.slotCount;
}
