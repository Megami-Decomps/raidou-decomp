#include "shared/interface/itfMesManager/decls.h"

itfMesBinTypeHeader_t *GetTypeHeader2(itfMesResource_t *pfileheader)
{
    itfMesBinTypeHeader_t *entries = pfileheader->entries;

    return &entries[pfileheader->entryCount];
}
