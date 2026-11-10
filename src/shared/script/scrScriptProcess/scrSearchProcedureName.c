#include "shared/script/scrScriptProcess/decls.h"

s32 scrSearchProcedureName(void *scrdata, char *procname)
{
    scrHeader_t *header = scrdata;
    scrTypeTable_t *typeTable;
    scrProcTable_t *procTable = NULL;
    s32 i;
    s32 no;

    if (header == NULL) {
        return -1;
    }

    typeTable = (scrTypeTable_t *)(header + 1);
    for (i = 0; i < header->TypeTableCount; i++) {
        if (typeTable[i].Type == SCR_DATATYPE_PROCTABLE) {
            procTable = (scrProcTable_t *)((u8 *)header + typeTable[i].ItemAddress);
            break;
        }
    }

    if (procTable == NULL) {
        return -1;
    }

    for (no = 0; no < typeTable[i].ItemCount; no++) {
        if (strcmp((char *)procTable[no].Name, procname) == 0) {
            return no;
        }
    }
    return -1;
}
