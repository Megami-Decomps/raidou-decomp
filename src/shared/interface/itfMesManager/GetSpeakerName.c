#include "shared/interface/itfMesManager/decls.h"

u8 *GetSpeakerName(instanceMes_tag *pinst)
{
    u32 *items = GetTypeHeader2(pinst->pdata)->type;

    return EE_ADDR_AS(u8 *, items[pinst->speaker.num]);
}
