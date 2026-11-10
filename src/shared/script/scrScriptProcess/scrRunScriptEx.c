#include "shared/script/scrScriptProcess/decls.h"

dds3ProcessID_t *scrRunScriptEx(s32 priority, scrHeader_t *header, scrTypeTable_t *typeTable,
                    scrProcTable_t *procTable, scrLabelTable_t *labelTable, scrCode_u *code,
                    u8 *messageData, u8 *stringData, s32 procNo)
{
    return scrStartScriptProcess(priority, scrPrintProcedureStart(header, typeTable, procTable, labelTable,
                                                           code, messageData, stringData, procNo));
}
