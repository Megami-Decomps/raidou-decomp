#include "shared/interface/itfMesManager/decls.h"

u8 *GetMessagePagePtr(itfMesBinMesHeader_t *table, s32 index)
{
    s32 count = table->page;

    if (index < 0 || index >= count) {
        return NULL;
    }
    return EE_ADDR_AS(u8 *, table->addr[index]);
}
