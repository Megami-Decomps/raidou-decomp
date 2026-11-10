#include "shared/interface/itfMesManager/decls.h"

/* Look up an entry-table item; only the item index has a bounds check. */
u8 *itfMesMngGetDataPtrImmediate(s32 id, s32 message, s32 page) {
    return GetMessagePagePtr(GetTypeHeader(pinst[id].mes, message)->ptr, page);
}
